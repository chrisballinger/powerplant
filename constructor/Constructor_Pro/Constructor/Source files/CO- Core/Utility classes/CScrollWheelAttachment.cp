// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*
File:		CScrollWheelAttachment.cp
Contact:	Richard Buckle, Sailmaker Software Ltd
			<mailto:richardb@sailmaker.co.uk>
			<http://www.sailmaker.co.uk/>
Version:	1.0.2
Purpose:	Attachment to add Carbon scroll wheel support to windows
Status:		Public domain
*/

// See CScrollWheelAttachment.h for important usage notes

// Modified quite a bit by Isaac Wankerl to get the scrolling to behave
// as Apple's Mac OS X apps behave (i.e. Finder, iTunes, Project Builder).

#include "CScrollWheelAttachment.h"
#include "VPLScroller.h"
#include "VPLScrollerView.h"
#include <LScrollerView.h>
#include <LScroller.h>

#if PP_Target_Carbon && (__PowerPlant__	< 0x02208000)
	EventHandlerUPP	TEventHandler<CScrollWheelAttachment>::sHandlerUPP = 0;
#endif

CScrollWheelAttachment::CScrollWheelAttachment()
{

#if PP_Target_Carbon

	mScrollHandler.Install( ::GetApplicationEventTarget(), 
								kEventClassMouse, 
								kEventMouseWheelMoved, this, 
								&CScrollWheelAttachment::HandleScroll );

#endif	

}
	
CScrollWheelAttachment::~CScrollWheelAttachment()
{
}

#if PP_Target_Carbon

OSStatus
CScrollWheelAttachment::HandleScroll(
	EventHandlerCallRef	/*inCallRef*/,
	EventRef			inEvent)
{
	EventMouseWheelAxis	axis;
	SInt32	delta;
	Point	mouseLoc;
	UInt32	modifiers;
	bool	handledEvent = false;

	::GetEventParameter(inEvent, kEventParamMouseWheelAxis, typeMouseWheelAxis,
		NULL, sizeof(axis), NULL, &axis);
	
	::GetEventParameter(inEvent, kEventParamMouseWheelDelta, typeLongInteger,
		NULL, sizeof(delta), NULL, &delta);

	::GetEventParameter(inEvent, kEventParamMouseLocation, typeQDPoint,
		NULL, sizeof(mouseLoc), NULL, &mouseLoc);

	::GetEventParameter(inEvent, kEventParamKeyModifiers, typeUInt32,
		NULL, sizeof(modifiers), NULL, &modifiers);

	handledEvent = DoScrollEvent((axis == kEventMouseWheelAxisY), delta, mouseLoc, modifiers);

	if (handledEvent)
		return noErr;
	
	return eventNotHandledErr;
}


bool
CScrollWheelAttachment::DoScrollEvent(
	Boolean	inIsVertical, 
	SInt32	inDelta, 
	Point	inMouseLoc,
	UInt32	inModifiers)
{

	WindowRef 		macHitWindow = nil;
	WindowPartCode	hitPart = ::FindWindow(inMouseLoc, &macHitWindow);
	LWindow* 		hitWindow = nil;
	bool			handledEvent = false;
	LView*			viewToScroll = nil;
	LScroller*		scroller = nil;
	LScrollerView*	scrollerView = nil;

	if (macHitWindow && (hitPart == inContent))
	{
		hitWindow = LWindow::FetchWindowObject(macHitWindow);
	}
		
	if (hitWindow && UDesktop::WindowIsSelected(hitWindow))
	{
		// The mouse was over the active window so we will see if the mouse was
		// over a view that can be scrolled.

		LPane*	hitPane;
		
		hitWindow->GlobalToPortPoint(inMouseLoc);
		hitPane = hitWindow->FindDeepSubPaneContaining(inMouseLoc.h, inMouseLoc.v);
		while (hitPane)
		{
			scroller = dynamic_cast<LScroller*>(hitPane);
			if ((scroller != nil) and
				(dynamic_cast<VPFScroller*>(scrollerView) == NULL)) // WB1-31154
			{
				viewToScroll = scroller->GetScrollingView();
				break;
			}

			scrollerView = dynamic_cast<LScrollerView*>(hitPane);
			if ((scrollerView != nil) and
				(dynamic_cast<VPFScrollerView*>(scrollerView) == NULL)) // WB1-31154
			{
				viewToScroll = scrollerView->GetScrollingView();
				break;
			}

			hitPane = hitPane->GetSuperView();
		}
	}

	if (viewToScroll == nil)
	{
		// The mouse is not over a view that can scroll or it was not over the active window.
		// Apple says that you should scroll the control that has the keyboard focus.  With
		// The Carbon APIs, it's easy to find the control by calling GetUserFocusWindow and
		// then GetKeyboardFocus.  With PowerPlant we'll just get the current LCommander
		// target and see if it is derived from LPane.  If so, we'll walk up it's super
		// views and test if we have a scrolling view.

		WindowRef	macWindow;
		LCommander* currentTarget;
		LPane*		targetPane;
		
		macWindow = ::GetUserFocusWindow();
		if (::IsValidWindowRef(macWindow))
		{
			currentTarget = LCommander::GetTarget();
			if (currentTarget)
			{
				targetPane = dynamic_cast<LPane*>(currentTarget);
				if (targetPane)
				{
					while (targetPane)
					{
						scroller = dynamic_cast<LScroller*>(targetPane);
						if (scroller != nil)
						{
							viewToScroll = scroller->GetScrollingView();
							break;
						}

						scrollerView = dynamic_cast<LScrollerView*>(targetPane);
						if (scrollerView != nil)
						{
							viewToScroll = scrollerView->GetScrollingView();
							break;
						}

						targetPane = targetPane->GetSuperView();
					}
				}
			}
		}
	}

	if (viewToScroll != nil)
	{
		SPoint32 scrollUnit;
		
		if (inModifiers & optionKey)
		{
			// Mac OS X 10.1 Finder uses the option key to quadruple the normal
			// scroll unit

			inDelta *= 4;
		}

		viewToScroll->GetScrollUnit(scrollUnit);
		if (inIsVertical)
		{
			scrollUnit.h = 0;
			scrollUnit.v *= -inDelta;
		}
		else
		{
			scrollUnit.h *= -inDelta;
			scrollUnit.v = 0;
		}
		
		viewToScroll->ScrollPinnedImageBy(scrollUnit.h, scrollUnit.v, Refresh_Yes);
		handledEvent = true;
	}

	return handledEvent;
}

#endif // PP_Target_Carbon
