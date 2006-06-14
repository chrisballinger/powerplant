// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLStdControl.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/15/96
//     $Date: 2006/01/18 01:34:02 $
//  $History: VPLStdControl.cpp $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/19/97   Time: 19:53
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:48
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/17/96   Time: 14:46
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Fixed a missing include.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 14:13
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added drawing agent.
//	
// ===========================================================================

#include "VPLStdControl.h"

	// Core : Data model : Attributes
#include "DMIntegerAttribute.h"

	// MacOS : Editors : Views : Data model
#include "VPDataModel.h"


// ===========================================================================

#pragma mark *** VPLStdControl ***

// ---------------------------------------------------------------------------
//		* CreateFromStream
// ---------------------------------------------------------------------------
//	Make sure that we don't create a popup menu.

LPane*
VPLStdControl::CreateFromStream(
	LStream*	inStream)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mModelObject.GetObject());
	
	// Check the control kind of this control.
	// We can't allow the popup-menu CDEF (CDEF #63)
	// to be loaded. It can do very evil things to a program.
	
	DMIntegerAttribute* controlKindAttr = mModelObject->FindIntegerAttribute(pPaneControlKind);
	ValidateObject_(controlKindAttr);
	
	SInt32 controlKind = controlKindAttr->GetIntValue();
	if ((controlKind >> 4) == 63)
		return nil;				//! TEMPORARY: not sure what to do here...
	else
		return new LStdControl(inStream);
	
}


// ---------------------------------------------------------------------------
//		* ValueChangedSelf										[protected]
// ---------------------------------------------------------------------------
//	An attribute of this model object has changed. Update the display
//	accordingly.

void
VPLStdControl::ValueChangedSelf(
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

		// Control kind, text traits: Rebuild control.

		case pPaneControlKind:
		case pPaneTextTraits:
			RebuildMe();
			break;

		// Otherwise, see if LControl wants it.

		default:
			VPLControl::ValueChangedSelf(inAttributeKey, inAttribute);

	}
}
