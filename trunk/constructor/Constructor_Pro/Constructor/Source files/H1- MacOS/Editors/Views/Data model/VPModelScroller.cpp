// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPModelScroller.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/22/96
//     $Date: 2006/01/18 01:33:55 $
//  $History: VPModelScroller.cpp $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:39
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:44
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/22/96   Time: 16:00
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added class.
//	
// ===========================================================================

#include "VPModelScroller.h"

	// MacOS : Editors : Views : Data model
#include "VPDataModel.h"


// ===========================================================================

#pragma mark *** VPModelScroller ***

// ---------------------------------------------------------------------------
//		* VPModelScroller()
// ---------------------------------------------------------------------------
//	Default constructor

VPModelScroller::VPModelScroller()
{
}


// ---------------------------------------------------------------------------
//		* VPModelScroller(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. No additional stream data is read.

VPModelScroller::VPModelScroller(
	LStream*	inStream)

: VPModelView(inStream)

{
}


// ---------------------------------------------------------------------------
//		* VPModelScroller(VPModelScroller&)
// ---------------------------------------------------------------------------
//	Copy constuctor

VPModelScroller::VPModelScroller(
	const VPModelScroller& inOriginal)

: VPModelView(inOriginal)

{
}


// ---------------------------------------------------------------------------
//		* ~VPModelScroller
// ---------------------------------------------------------------------------
//	Destructor

VPModelScroller::~VPModelScroller()
{
}


// ---------------------------------------------------------------------------
//		* DoSanityCheckSelf										[protected]
// ---------------------------------------------------------------------------
//	Make sure the scrolled view is in fact a view. Otherwise, the user's
//	program could crash at runtime, and this is a Bad Thing.

void
VPModelScroller::DoSanityCheckSelf()
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Do inherited sanity check.

	VPModelView::DoSanityCheckSelf();

	// Make sure scrolled view is in fact a view.
	
	BashSubPaneID(pPaneScrollingViewID, LView::class_ID);

}


// ---------------------------------------------------------------------------
//		* SendUpdateMessage										[protected]
// ---------------------------------------------------------------------------
//	If the scrolled view ID changes, do a sanity check on that value.

void
VPModelScroller::SendUpdateMessage(
	MessageT		inMessage,
	void*			ioParam,
	DMElement*		inSubElement)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Relay message as usual.

	VPModelView::SendUpdateMessage(inMessage, ioParam, inSubElement);
	
	// See if it's an attribute change message.
	
	if (inMessage == Attribute_ValueChange) {
		
		// It is, see if it's the scrolled view attribute.
		// If so, make sure the new ID is acceptable.

		DM_ValueChange* valueMsg = (DM_ValueChange*) ioParam;
		ValidateObject_(valueMsg->mAttribute);
		
		if (valueMsg->mAttribute->GetKey() == pPaneScrollingViewID)
			DoSanityCheckSelf();

	}
}
