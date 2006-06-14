// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPModelPane.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/14/96
//     $Date: 2006/01/18 01:33:55 $
//  $History: VPModelPane.cpp $
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 01/29/97   Time: 18:54
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Added DoSanityCheckSelf method to ensure that superview field has -1 or
//	0 as indicated by the class ID. (Bug fix #1253.)
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:44
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Fixed CR/LF problem
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/24/96   Time: 21:57
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added IsVisible method (overridden from VEModelObject).
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/24/96   Time: 19:41
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added AdaptToSuperFrameSize method (overridden from VPModelObject).
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/20/96   Time: 20:16
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added GetSubObjectList method.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 10:59
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added class.
//	
// ===========================================================================

#include "VPModelPane.h"

	// Core : Data model : Undo/redo
#include "StDMTransactionBuilder.h"

	// MacOS : Editors : Views : Data model
#include "VPDataModel.h"
#include "VPTypeEntry.h"


// ===========================================================================

#pragma mark *** VPModelPane ***

// ---------------------------------------------------------------------------
//		* VPModelPane()
// ---------------------------------------------------------------------------
//	Default constructor

VPModelPane::VPModelPane()
{
}


// ---------------------------------------------------------------------------
//		* VPModelPane(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. No additional data is read.

VPModelPane::VPModelPane(
	LStream*	inStream)

: VPModelObject(inStream)

{
}


// ---------------------------------------------------------------------------
//		* VPModelPane(VPModelPane&)
// ---------------------------------------------------------------------------
//	Copy constructor

VPModelPane::VPModelPane(
	const VPModelPane&	inOriginal)

: VPModelObject(inOriginal)

{
}


// ---------------------------------------------------------------------------
//		* ~VPModelPane
// ---------------------------------------------------------------------------
//	Destructor

VPModelPane::~VPModelPane()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** accessors

// ---------------------------------------------------------------------------
//		* IsVisible
// ---------------------------------------------------------------------------
//	Returns true if the object is considered visible.

Boolean
VPModelPane::IsVisible() const
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Find "visible" attribute.
	
	DMIntegerAttribute* visAttr = FindIntegerAttribute(pPaneVisible);
	ValidateObject_(visAttr);
	
	// Return its value.
	
	return visAttr->GetIntValue() != 0;

}


// ---------------------------------------------------------------------------
//		* GetSubObjectList
// ---------------------------------------------------------------------------
//	Return a pointer to the attachments list.

DMContainerAttribute*
VPModelPane::GetSubObjectList() const
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Look for the attachments list.

	return FindContainerAttribute(pPaneAttachmentsList);

}


// ===========================================================================

#pragma mark -
#pragma mark ** pane resizing helper

// ---------------------------------------------------------------------------
//		* AdaptToSuperFrameSize									[protected]
// ---------------------------------------------------------------------------
//	Called by VPModelView::FrameSizeChanged whenever the superview's
//	size changes.

void
VPModelPane::AdaptToSuperFrameSize(
	SInt32	inSurrWidthDelta,
	SInt32	inSurrHeightDelta)
{

	// If undoing a change, don't adapt.

	if (StDMTransactionBuilder::GetSubActionHost() == nil)
		return;

	// See what our current size and pane bindings are.

	SInt32 width = FindIntegerAttribute(pPaneWidth)->GetIntValue();
	SInt32 height = FindIntegerAttribute(pPaneHeight)->GetIntValue();

	SInt32 bindLeft = FindIntegerAttribute(pPaneBoundLeft)->GetIntValue();
	SInt32 bindRight = FindIntegerAttribute(pPaneBoundRight)->GetIntValue();
	SInt32 bindTop = FindIntegerAttribute(pPaneBoundTop)->GetIntValue();
	SInt32 bindBottom = FindIntegerAttribute(pPaneBoundBottom)->GetIntValue();

	// If resizing a pane with -1 for either width or height, don't change that value.

	if (width == -1)
		inSurrWidthDelta = 0;
	if (height == -1)
		inSurrHeightDelta = 0;

	// Remainder is adapted from LPane::AdaptToSuperFrameSize.

	// When SuperView changes size, a Pane may move or change
	// size, depending on how each of its sides is "bound" to
	// that of its SuperView. A side that is bound always remains
	// the same distance from the corresponding side of its SuperView.
		
	SInt32 widthDelta = 0;
	SInt32 heightDelta = 0;
	SInt32 horizDelta = 0;
	SInt32 vertDelta = 0;

	if (bindRight) {
		if (bindLeft) {
										// Both right and left are bound
										// Pane resizes horizontally
			widthDelta = inSurrWidthDelta;
		} else {
										// Right bound, left free
										// Pane moves horizontally
			horizDelta = inSurrWidthDelta;
		}
	}
	
	if (bindBottom) {
		if (bindTop) {
										// Both bottom and top are bound
										// Pane resizes vertically
			heightDelta = inSurrHeightDelta;
		} else {
										// Bottom bound, left free
										// Pane moves vertically
			vertDelta = inSurrHeightDelta;
		}
	}
	
	if ((widthDelta != 0) || (heightDelta != 0))
		ResizeBy(widthDelta, heightDelta);
	
	if ((horizDelta != 0) || (vertDelta != 0))
		MoveBy(horizDelta, vertDelta);

}


// ---------------------------------------------------------------------------
//		* DoSanityCheckSelf										[protected]
// ---------------------------------------------------------------------------
//	Make sure the superview field has a -1 or 0.

void
VPModelPane::DoSanityCheckSelf()
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Find the superview attribute.
	
	DMAttribute* svAttr = FindAttributeByKey(pPaneSuperview);
	if (svAttr != nil) {
		
		// Cast it to integer attribute.
		
		ValidateObject_(svAttr);

		DMIntegerAttribute* intAttr = (dynamic_cast<DMIntegerAttribute*>(svAttr));
		ValidateObject_(intAttr);
		
		// Make sure the superview attribute has the correct magic value.
		
		SInt32 magic = -1;
		if (GetVPTypeEntry()->TypeDerivesFrom(LGrafPortView::class_ID))
			magic = 0;
		
		intAttr->SetIntValue(magic);
		
	}
}
