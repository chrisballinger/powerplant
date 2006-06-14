// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLScroller.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/14/96
//     $Date: 2006/04/12 08:48:30 $
//  $History: VPLScroller.cpp $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/19/97   Time: 19:53
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:48
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 14:12
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added drawing agent.
//	
// ===========================================================================

#include "VPLScroller.h"

	// Core : Data model : Attributes
#include "DMIntegerAttribute.h"

	// MacOS : Editors : Views : Data model
#include "VPDataModel.h"


// ===========================================================================

#pragma mark *** VPLScroller ***

// ---------------------------------------------------------------------------
//		* CreateFromStream
// ---------------------------------------------------------------------------
//	Create PowerPlant pane for LScroller. We substitute the
//	VPFScroller class which has some extra safety checks.

LPane*
VPLScroller::CreateFromStream(
	LStream*	inStream)
{
	return new VPFScroller(inStream);
}


// ---------------------------------------------------------------------------
//		* DrawDisplayObject
// ---------------------------------------------------------------------------
//	Draw the scroller. Overridden to ensure that the scroll bars are shown
//	(since the standard DrawDisplayObject doesn't call Draw).

void
VPLScroller::DrawDisplayObject()
{

	// Validate pointers.
	
	ValidateThis_();

	// See if we've created a display pane for this object.
	// If so, we can provide default behavior for drawing.
	
	LPane* pane = GetDisplayPane();
	if (pane == nil)
		return;
	ValidateObject_(pane);

	// There is a pane, ask it to draw itself and its scrollbars,
	// but not other subpanes.
	
	if (pane->FocusDraw()) {
	
		// Cast the pane to an LScroller.
		
		VPFScroller* scroller = (dynamic_cast<VPFScroller*>(pane));
		ValidateObject_(scroller);
	
		// Draw the scroller itself.
	
		pane->DrawSelf();
		
		// Now draw the scroller's scroll bars.
		
		LPane* horizBar = scroller->mHorizontalBar;
		LPane* vertBar = scroller->mVerticalBar;
		
		if (horizBar != nil) {
			ValidateObject_(horizBar);
			horizBar->DrawSelf();
		}
		
		if (vertBar != nil) {
			ValidateObject_(vertBar);
			vertBar->DrawSelf();
		}
	}
}


// ---------------------------------------------------------------------------
//		* ValueChangedSelf										[protected]
// ---------------------------------------------------------------------------
//	An attribute of this model object has changed. Update the display
//	accordingly.

void
VPLScroller::ValueChangedSelf(
	FourCharCode	inAttributeKey,
	DMAttribute*	inAttribute)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mDisplayPane);
	ValidateObject_(mModelObject.GetObject());
	ValidateObject_(inAttribute);
	
	// Cast display pane to LScroller.
	
	LScroller* scroller = (dynamic_cast<LScroller*>(mDisplayPane));
	ValidateObject_(scroller);
	
	// Dispatch attribute key.

	switch (inAttributeKey) {

		// Basically all parameters for LScroller result
		// in a rebuild of the drawing agent.
		
		case pPaneScrollLeftIndent:
		case pPaneScrollRightIndent:
		case pPaneScrollTopIndent:
		case pPaneScrollBottomIndent:
		case pPaneScrollingViewID:
			RebuildMe();
			break;
	
		// Otherwise, see if LView wants it.

		default:
			VPLView::ValueChangedSelf(inAttributeKey, inAttribute);

	}
}

// ---------------------------------------------------------------------------
//		* SubPaneDeletedSelf									[protected]
// ---------------------------------------------------------------------------
//	A subpane has been deleted. If it is the scrolling view, make sure that
//	we remove the dangling pointer.

void
VPLScroller::SubPaneDeletedSelf(
	LPane*	inSubPane)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inSubPane);

	// See if we've created a display pane.
	
	if (mDisplayPane != nil) {
		
		// Yes, cast it to scroller.
		
		ValidateObject_(mDisplayPane);
		VPFScroller* scroller = (dynamic_cast<VPFScroller*>(mDisplayPane));
		ValidateObject_(scroller);
		
		// Check the scroller's installed view. If it matches
		// this pane that's being deleted, remove the pointer
		// so we don't crash later.
		
		if (scroller->mScrollingView == inSubPane)
			scroller->InstallView(nil);

	}
}


// ===========================================================================

#pragma mark -
#pragma mark *** VPFScroller ***

// ---------------------------------------------------------------------------
//		* VPFScroller(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor

VPFScroller::VPFScroller(
	LStream*	inStream)

: LScroller(inStream)

{
}


// ---------------------------------------------------------------------------
//		* ~VPFScroller
// ---------------------------------------------------------------------------
//	Destructor

VPFScroller::~VPFScroller()
{
}


// ---------------------------------------------------------------------------
//		* FinishCreateSelf										[protected]
// ---------------------------------------------------------------------------
//	Overridden to ensure that the scrolled view really is derived from
//	LView. Bad Things happen if it isn't.

void
VPFScroller::FinishCreateSelf()
{

	// Validate pointers.
	
	ValidateThis_();
	
	// See if the scrolling view is in fact a view.
	
	LView* scrollingView = (dynamic_cast<LView*>(FindPaneByID(mScrollingViewID)));
	if (scrollingView != nil) {
	
		// Validate the scrolling view pointer.
		
		ValidateObject_(scrollingView);
		
		// Install the view. Suppress drawing while this is done
		// so the scroll bars don't flash wildly.
	
		GrafPtr	myPort = GetMacPort();
		if (myPort == nil)
			myPort = UQDGlobals::GetCurrentPort();
		ValidatePtr_((Ptr) myPort);

#if PP_Target_Carbon
		StClipRgnState suppressDrawing(nil);
#else
		StVisRgn suppressDrawing(myPort);
#endif
		InstallView(scrollingView);

	}
}