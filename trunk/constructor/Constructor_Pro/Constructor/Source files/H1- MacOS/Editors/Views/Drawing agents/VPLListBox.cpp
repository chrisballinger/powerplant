// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLListBox.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 12/08/96
//     $Date: 2006/01/18 01:34:01 $
//  $History: VPLListBox.cpp $
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
//	User: scouten      QDate: 12/08/96   Time: 17:15
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added class.
//	
// ===========================================================================

#include "VPLListBox.h"

	// MacoS : Editors : Views : Data model
#include "VPDataModel.h"


// ===========================================================================

#pragma mark *** VPLListBox ***

// ---------------------------------------------------------------------------
//		* ValueChangedSelf										[protected]
// ---------------------------------------------------------------------------
//	An attribute of this model object has changed. Update the display
//	accordingly.

void
VPLListBox::ValueChangedSelf(
	FourCharCode	inAttributeKey,
	DMAttribute*	inAttribute)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Dispatch attribute key.

	switch (inAttributeKey) {

		// Rebuild list box whenever any parameter changes.

		case pPaneListHorizScrollBar:
		case pPaneListVertScrollBar:
		case pPaneListGrowBox:
		case pPaneListLDEFID:
		case pPaneListInitialItems:
		case pPaneListInitialItemText:
		case pPaneTextTraits:
			RebuildMe();
			return;
		
		// Otherwise, see if LPane wants it.

		default:
			VPLPane::ValueChangedSelf(inAttributeKey, inAttribute);

	}
}


// ---------------------------------------------------------------------------
//		* ContainerListChanged									[protected]
// ---------------------------------------------------------------------------
//	A container attribute of this model object has changed. Update the
//	display accordingly.

void
VPLListBox::ContainerListChanged(
	DM_ListChange*	inMessage)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inMessage->mContainer);
	
	// See if it's the initial text item list that's changing.
	
	DMAttribute* attr = (dynamic_cast<DMAttribute*>(inMessage->mContainer));
	if (attr == nil)
		return;
	
	ValidateObject_(attr);
	
	if (attr->GetKey() == pPaneTextTraits)
		RebuildMe();

}
