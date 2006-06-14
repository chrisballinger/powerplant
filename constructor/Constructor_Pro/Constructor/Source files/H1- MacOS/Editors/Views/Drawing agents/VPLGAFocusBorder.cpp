// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLGAFocusBorder.cpp		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/15/96
//     $Date: 2006/01/18 01:33:59 $
//  $History: VPLGAFocusBorder.cpp $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:56
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:46
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 13:52
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added drawing agent.
//	
// ===========================================================================

#include "VPLGAFocusBorder.h"

	// MacOS : Editors : Views : Data model
#include "VPDataModel.h"


// ===========================================================================

#pragma mark *** VPLGAFocusBorder ***

// ---------------------------------------------------------------------------
//		* CreateFromStream
// ---------------------------------------------------------------------------
//	Create PowerPlant pane for LGAFocusBorder. We substitute the
//	VPFGAFocusBorder class which has some extra safety checks.

LPane*
VPLGAFocusBorder::CreateFromStream(
	LStream*	inStream)
{
	return new VPFGAFocusBorder(inStream);
}


// ---------------------------------------------------------------------------
//		* ValueChangedSelf										[protected]
// ---------------------------------------------------------------------------
//	An attribute of this model object has changed. Update the display
//	accordingly.

void
VPLGAFocusBorder::ValueChangedSelf(
	FourCharCode	inAttributeKey,
	DMAttribute*	inAttribute)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mDisplayPane);
	ValidateObject_(mModelObject.GetObject());
	ValidateObject_(inAttribute);
	
	// Dispatch attribute key.

	switch (inAttributeKey) {

		// Basically all parameters for LGAFocusBorder result
		// in a rebuild of the drawing agent.
		
		case pPaneFocusInsetPaneID:
		case pPaneFocusCommanderID:
		case pPanePaintBorderFace:
		case pPaneFrameInsetSubPane:
		case pPaneNotchInsetSubPane:
		case pPaneNotchFaceForGrowbox:
		case pPaneNotchWidthHeight:
		case pPaneCanBeFocused:
		case pPaneHasFocus:
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
//	A subpane has been deleted. If it is the inset pane, make sure that
//	we remove the dangling pointer.

void
VPLGAFocusBorder::SubPaneDeletedSelf(
	LPane*	inSubPane)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inSubPane);

	// See if we've created a display pane.
	
	if (mDisplayPane != nil) {
		
		// Yes, cast it to focus border.
		
		ValidateObject_(mDisplayPane);
		VPFGAFocusBorder* focusBorder = (dynamic_cast<VPFGAFocusBorder*>(mDisplayPane));
		ValidateObject_(focusBorder);
		
		// Check the focusBorder's installed view. If it matches
		// this pane that's being deleted, remove the pointer
		// so we don't crash later.
		
		if (focusBorder->GetInsetSubPane() == inSubPane)
			focusBorder->SetInsetSubPane(nil);

	}
}


// ===========================================================================

#pragma mark -
#pragma mark *** VPFGAFocusBorder ***

// ---------------------------------------------------------------------------
//		* VPFGAFocusBorder()
// ---------------------------------------------------------------------------
//	Constructor

VPFGAFocusBorder::VPFGAFocusBorder(
	LStream*	inStream)

: LGAFocusBorder(inStream)

{
}


// ---------------------------------------------------------------------------
//		* ~VPFGAFocusBorder
// ---------------------------------------------------------------------------
//	Destructor

VPFGAFocusBorder::~VPFGAFocusBorder()
{
}


// ---------------------------------------------------------------------------
//		* FinishCreateSelf										[protected]
// ---------------------------------------------------------------------------
//	Overriden to add safety checks for inset sub pane, and to disable
//	the tab border list and commander sub pane. (Commander chains are
//	irrelevant inside the layout view.)

void
VPFGAFocusBorder::FinishCreateSelf()
{

	// Validate pointers.
	
	ValidateThis_();
	
	// See if the inset pane exists.
	
	LPane* insetSubPane = FindPaneByID(mInsetSubPaneID);
	if ((insetSubPane != nil) && (insetSubPane != this)) {
	
		// Validate the inset pane pointer.
		
		ValidateObject_(insetSubPane);
		
		// Install it.
		
		SetInsetSubPane(insetSubPane);

	}

	// Do *NOT* call inherited. We want to ignore the
	// tab border list and commander sub pane.

}
