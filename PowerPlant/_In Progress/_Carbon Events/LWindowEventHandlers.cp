// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	LWindowEventHandlers.cp		PowerPlant 2.2.2	©2001-2005 Metrowerks Inc.
// ===========================================================================

#include <LWindowEventHandlers.h>
#include <LWindow.h>
#include <PP_Messages.h>
#include <UCarbonEvents.h>
#include <UCursor.h>
#include <URegions.h>

PP_Begin_Namespace_PowerPlant


// ---------------------------------------------------------------------------
//	¥ LWindowEventHandlers
// ---------------------------------------------------------------------------

LWindowEventHandlers::LWindowEventHandlers(
	LWindow*	inWindow)
{
	mWindow = inWindow;
}


// ---------------------------------------------------------------------------
//	¥ ~LWindowEventHandlers
// ---------------------------------------------------------------------------

LWindowEventHandlers::~LWindowEventHandlers()
{
}


// ---------------------------------------------------------------------------
//	¥ InstallEventHandlers
// ---------------------------------------------------------------------------

void
LWindowEventHandlers::InstallEventHandlers()
{
	WindowPtr	windowP = mWindow->GetMacWindow();
	
	if (windowP == nil) {
		return;
	}

	EventTargetRef	eventTarget = ::GetWindowEventTarget(windowP);

	mDrawContent.Install(	eventTarget,
							kEventClassWindow,
							kEventWindowDrawContent,
							this,
						 	&LWindowEventHandlers::DrawContent );

	mActivated.Install(		eventTarget,
							kEventClassWindow,
							kEventWindowActivated,
							this,
							&LWindowEventHandlers::Activated );

	mDeactivated.Install(	eventTarget,
							kEventClassWindow,
							kEventWindowDeactivated,
							this,
							&LWindowEventHandlers::Deactivated );

	mClickContentRgn.Install(
							eventTarget,
							kEventClassWindow,
							kEventWindowClickContentRgn,
							this,
							&LWindowEventHandlers::ClickContentRgn );
							
	mGetMinimumSize.Install(
							eventTarget,
							kEventClassWindow,
							kEventWindowGetMinimumSize,
							this,
							&LWindowEventHandlers::GetMinimumSize );
							
	mGetMaximumSize.Install(
							eventTarget,
							kEventClassWindow,
							kEventWindowGetMaximumSize,
							this,
							&LWindowEventHandlers::GetMaximumSize );
							
	mBoundsChanged.Install(	eventTarget,
							kEventClassWindow,
							kEventWindowBoundsChanged,
							this,
							&LWindowEventHandlers::BoundsChanged );
							
	mZoom.Install(			eventTarget,
							kEventClassWindow,
							kEventWindowZoom,
							this,
							&LWindowEventHandlers::Zoom );

	mClose.Install(			eventTarget,
							kEventClassWindow,
							kEventWindowClose,
							this,
							&LWindowEventHandlers::Close );

	mMouseMoved.Install(	eventTarget,
							kEventClassMouse,
							kEventMouseMoved,
							this,
							&LWindowEventHandlers::MouseMoved );
}


// ---------------------------------------------------------------------------
//	¥ DrawContent
// ---------------------------------------------------------------------------

OSStatus
LWindowEventHandlers::DrawContent(
	EventHandlerCallRef	/* inCallRef */,
	EventRef			/* inEventRef */)
{
	mWindow->Draw(nil);

	return noErr;
}


// ---------------------------------------------------------------------------
//	¥ Activated
// ---------------------------------------------------------------------------

OSStatus
LWindowEventHandlers::Activated(
	EventHandlerCallRef	/* inCallRef */,
	EventRef			/* inEventRef */)
{
	mWindow->Activate();

	return noErr;
}


// ---------------------------------------------------------------------------
//	¥ Deactivated
// ---------------------------------------------------------------------------

OSStatus
LWindowEventHandlers::Deactivated(
	EventHandlerCallRef	/* inCallRef */,
	EventRef			/* inEventRef */)
{
	mWindow->Deactivate();

	return noErr;
}


// ---------------------------------------------------------------------------
//	¥ ClickContentRgn
// ---------------------------------------------------------------------------

OSStatus
LWindowEventHandlers::ClickContentRgn(
	EventHandlerCallRef	/* inCallRef */,
	EventRef			inEventRef)
{
		// Create old-style EventRecord by extracting the
		// appropriate data from the EventRef

	EventRecord		clickEvent;
	UCarbonEvents::MakeOldEventRecord(inEventRef, clickEvent);
						 
	clickEvent.what = mouseDown;
						 
	mWindow->ClickInContent(clickEvent);
	
	return noErr;
}


// ---------------------------------------------------------------------------
//	¥ GetMinimumSize
// ---------------------------------------------------------------------------

OSStatus
LWindowEventHandlers::GetMinimumSize(
	EventHandlerCallRef	/* inCallRef */,
	EventRef			inEventRef)
{
	Rect	minMaxSize;
	mWindow->GetMinMaxSize(minMaxSize);

	Point		minSize = topLeft(minMaxSize);
	
	::SetEventParameter( inEventRef,
						  kEventParamDimensions,
						  typeQDPoint,
						  sizeof(Point),
						  &minSize );
	
	return noErr;
}


// ---------------------------------------------------------------------------
//	¥ GetMaximumSize
// ---------------------------------------------------------------------------

OSStatus
LWindowEventHandlers::GetMaximumSize(
	EventHandlerCallRef	/* inCallRef */,
	EventRef			inEventRef)
{
	Rect	minMaxSize;
	mWindow->GetMinMaxSize(minMaxSize);

	Point		maxSize = botRight(minMaxSize);
	
	::SetEventParameter( inEventRef,
						  kEventParamDimensions,
						  typeQDPoint,
						  sizeof(Point),
						  &maxSize );
	
	return noErr;
}


// ---------------------------------------------------------------------------
//	¥ BoundsChanged
// ---------------------------------------------------------------------------

OSStatus
LWindowEventHandlers::BoundsChanged(
	EventHandlerCallRef	/* inCallRef */,
	EventRef			inEventRef)
{
	OSStatus	status;
								  
	UInt32	attributes;
	status = ::GetEventParameter( inEventRef,
								  kEventParamAttributes,
								  typeUInt32, nil,
								  sizeof(UInt32), nil,
								  &attributes );
								  
	mWindow->AdaptToBoundsChange(attributes);

	return noErr;
}


// ---------------------------------------------------------------------------
//	¥ Zoom
// ---------------------------------------------------------------------------

OSStatus
LWindowEventHandlers::Zoom(
	EventHandlerCallRef	/* inCallRef */,
	EventRef			/* inEventRef */)
{
	mWindow->SendAESetZoom();

	return noErr;
}


// ---------------------------------------------------------------------------
//	¥ Close
// ---------------------------------------------------------------------------

OSStatus
LWindowEventHandlers::Close(
	EventHandlerCallRef	/* inCallRef */,
	EventRef			/* inEventRef */)
{
	mWindow->ProcessCommand(cmd_Close);

	return noErr;
}


// ---------------------------------------------------------------------------
//	¥ MouseMoved
// ---------------------------------------------------------------------------

OSStatus
LWindowEventHandlers::MouseMoved(
	EventHandlerCallRef	/* inCallRef */,
	EventRef			inEventRef)
{
	EventRecord		event;
	UCarbonEvents::MakeOldEventRecord(inEventRef, event);
	
	Rect	contentBounds;
	::GetWindowBounds(mWindow->GetMacWindow(), kWindowContentRgn, &contentBounds);
	
	if (::MacPtInRect(event.where, &contentBounds)) {
		Point	portMouse = event.where;
		mWindow->GlobalToPortPoint(portMouse);
		
		StRegion	mouseRgn;
		mWindow->AdjustContentMouse(portMouse, event, mouseRgn);
		
	} else {
	
		UCursor::SetArrow();
	}

	return noErr;
}


PP_End_Namespace_PowerPlant
