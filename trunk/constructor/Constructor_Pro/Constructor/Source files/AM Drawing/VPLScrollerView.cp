// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLScrollerView.c				©1997 Metrowerks Inc. All rights reserved.
// ===========================================================================

#include "VPLScrollerView.h"
#include "DMIntegerAttribute.h"
#include "VPDataModel.h"

#include <LScrollBar.h>


// ===========================================================================

#pragma mark *** VPLScrollerView ***

// ---------------------------------------------------------------------------
//		* CreateFromStream
// ---------------------------------------------------------------------------
//	Create PowerPlant pane for LScrollerView. We substitute the
//	VPFScrollerView class which has some extra safety checks.

LPane*
VPLScrollerView::CreateFromStream(
	LStream*	inStream)
{
	return new VPFScrollerView(inStream);
}


// ---------------------------------------------------------------------------
//		* DrawDisplayObject
// ---------------------------------------------------------------------------
//	Draw the ScrollerView. Overridden to ensure that the scroll bars are shown
//	(since the standard DrawDisplayObject doesn't call Draw).

void
VPLScrollerView::DrawDisplayObject()
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
	
		// Cast the pane to an LScrollerView.
		
		VPFScrollerView* ScrollerView = (dynamic_cast<VPFScrollerView*>(pane));
		ValidateObject_(ScrollerView);
	
		// Draw the ScrollerView itself.
	
		pane->DrawSelf();
		
		// Now draw the ScrollerView's scroll bars.
		
		LScrollBar* horizBar = ScrollerView->mHorizontalBar;
		LScrollBar* vertBar = ScrollerView->mVerticalBar;
		
		if (horizBar != nil) {
			ValidateObject_(horizBar);
			horizBar->Draw(nil);
		}
		
		if (vertBar != nil) {
			ValidateObject_(vertBar);
			vertBar->Draw(nil);
		}
	}
}


// ---------------------------------------------------------------------------
//		* ValueChangedSelf										[protected]
// ---------------------------------------------------------------------------
//	An attribute of this model object has changed. Update the display
//	accordingly.

void
VPLScrollerView::ValueChangedSelf(
	FourCharCode	inAttributeKey,
	DMAttribute*	inAttribute)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mDisplayPane);
	ValidateObject_(mModelObject.GetObject());
	ValidateObject_(inAttribute);
	
	// Cast display pane to LScrollerView.
	
	LScrollerView* ScrollerView = (dynamic_cast<LScrollerView*>(mDisplayPane));
	ValidateObject_(ScrollerView);
	
	// Dispatch attribute key.

	switch (inAttributeKey) {

		// Basically all parameters for LScrollerView result
		// in a rebuild of the drawing agent.
		
		case pPaneScrollLeftIndent:
		case pPaneScrollRightIndent:
		case pPaneScrollTopIndent:
		case pPaneScrollBottomIndent:
		case pPaneScrollingViewID:
		case pRebuildMe:
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
VPLScrollerView::SubPaneDeletedSelf(
	LPane*	inSubPane)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inSubPane);

	// See if we've created a display pane.
	
	if (mDisplayPane != nil) {
		
		// Yes, cast it to ScrollerView.
		
		ValidateObject_(mDisplayPane);
		VPFScrollerView* ScrollerView = (dynamic_cast<VPFScrollerView*>(mDisplayPane));
		ValidateObject_(ScrollerView);
		
		// Check the ScrollerView's installed view. If it matches
		// this pane that's being deleted, remove the pointer
		// so we don't crash later.
		
		if (ScrollerView->mScrollingView == inSubPane)
			ScrollerView->InstallView(nil);

	}
}


// ===========================================================================

#pragma mark -
#pragma mark *** VPFScrollerView ***

// ---------------------------------------------------------------------------
//		* VPFScrollerView(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor

VPFScrollerView::VPFScrollerView(
	LStream*	inStream)
		: LScrollerView(inStream)
{
}


// ---------------------------------------------------------------------------
//		* ~VPFScrollerView
// ---------------------------------------------------------------------------
//	Destructor

VPFScrollerView::~VPFScrollerView()
{
}


// ---------------------------------------------------------------------------
//		* FinishCreateSelf										[protected]
// ---------------------------------------------------------------------------
//	Overridden to ensure that the scrolled view really is derived from
//	LView. Bad Things happen if it isn't.

void
VPFScrollerView::FinishCreateSelf()
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