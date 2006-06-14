// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPModelDialogBox.cpp		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/22/96
//     $Date: 2006/01/18 01:33:54 $
//  $History: VPModelDialogBox.cpp $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:39
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:44
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/30/96   Time: 18:45
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Fixed a bug in SendUpdateMessage that caused it to bypass the
//	VPModelWindow::SendUpdateMessage call.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/22/96   Time: 15:56
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added class.
//	
// ===========================================================================

#include "VPModelDialogBox.h"

	// MacOS : Editors : Views : Data model
#include "VPDataModel.h"


// ===========================================================================

#pragma mark *** VPModelDialogBox ***

// ---------------------------------------------------------------------------
//		* VPModelDialogBox()
// ---------------------------------------------------------------------------
//	Default constructor

VPModelDialogBox::VPModelDialogBox()
{
}


// ---------------------------------------------------------------------------
//		* VPModelDialogBox(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. No additional stream data is read.

VPModelDialogBox::VPModelDialogBox(
	LStream*	inStream)

: VPModelWindow(inStream)

{
}


// ---------------------------------------------------------------------------
//		* VPModelDialogBox(VPModelDialogBox&)
// ---------------------------------------------------------------------------
//	Copy constructor

VPModelDialogBox::VPModelDialogBox(
	const VPModelDialogBox&		inOriginal)

: VPModelWindow(inOriginal)

{
}


// ---------------------------------------------------------------------------
//		* ~VPModelDialogBox
// ---------------------------------------------------------------------------
//	Destructor

VPModelDialogBox::~VPModelDialogBox()
{
}


// ---------------------------------------------------------------------------
//		* DoSanityCheckSelf										[protected]
// ---------------------------------------------------------------------------
//	Make sure cancel and default button IDs actually point to LControl.

void
VPModelDialogBox::DoSanityCheckSelf()
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Do inherited sanity check.

	VPModelWindow::DoSanityCheckSelf();

	// Make sure cancel and default buttons are in fact controls.
	
	BashSubPaneID(pPaneDialogDefaultButtonID, 'cntl');
	BashSubPaneID(pPaneDialogCancelButtonID, 'cntl');

}


// ---------------------------------------------------------------------------
//		* SendUpdateMessage										[protected]
// ---------------------------------------------------------------------------
//	If Cancel or default button ID was changed, make sure it's still valid.

void
VPModelDialogBox::SendUpdateMessage(
	MessageT		inMessage,
	void*			ioParam,
	DMElement*		inSubElement)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Relay message as usual.

	VPModelWindow::SendUpdateMessage(inMessage, ioParam, inSubElement);
	
	// See if it's an attribute change message.
	
	if (inMessage == Attribute_ValueChange) {
		
		// It is, see if it's the cancel or default button attribute.
		// If so, make sure the new ID is acceptable.

		DM_ValueChange* valueMsg = (DM_ValueChange*) ioParam;
		ValidateObject_(valueMsg->mAttribute);
		
		switch (valueMsg->mAttribute->GetKey()) {
			case pPaneDialogDefaultButtonID:
			case pPaneDialogCancelButtonID:
				DoSanityCheckSelf();
				break;
		}
	}
}
