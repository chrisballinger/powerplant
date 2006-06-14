// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VESubModelContainer.h		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/25/96
//     $Date: 2006/01/18 01:33:12 $
//  $History: VESubModelContainer.h $
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 02/20/97   Time: 18:26
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/Data model
//	Improved commenting.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/30/96   Time: 18:18
//	Created in $/Constructor/Source files/Editors/Views/+ Generic view editor/Data model
//	Added file again. It was checked in as an ObjectMaster file before.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/25/96   Time: 00:36
//	Created in $/Constructor/Source files/Editors/Views/+ Generic view editor/Data model
//	Added class.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Attributes
#include "DMContainerAttribute.h"


// ===========================================================================
//		* VESubModelContainer
// ===========================================================================
//
//	VESubModelContainer is a special container attribute designed for use
//	as a sub-object list in the view editor. It enforces the "must be root"
//	flag of the VETypeEntry.
//
// ===========================================================================

class VESubModelContainer : public DMContainerAttribute {

	// data model
	
	DMClassID_				('vesm', VESubModelContainer, DMContainerAttribute);

	// constructor/destructor

public:
							VESubModelContainer();
							VESubModelContainer(LStream* inStream);
							VESubModelContainer(const VESubModelContainer& inOriginal);
	virtual					~VESubModelContainer();

	// clipboard/drag-and-drop primitives

protected:
	virtual Boolean			CanHostElementSelf(
									const DMElement*	inNewElement,
									DM_DragPlacement*	ioPlacement);

};
