// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VEHierOutlineItem.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/13/96
//     $Date: 2006/01/18 01:33:16 $
//  $History: VEHierOutlineItem.h $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/20/97   Time: 18:27
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Improved commenting.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:30
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Fixed CR/LF problem
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 12/06/96   Time: 18:04
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Updated GetDrawContentsSelf to use
//	VEModelObject::GetDisplayableObjectType.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/19/96   Time: 17:24
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Added support for dynamic columns in hierarchy window.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/13/96   Time: 13:42
//	Created in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Added class. (Refactoring hierarchy view to generic view editor.)
//	
// ===========================================================================

#pragma once

	// Core : Display classes (Eric) : Outline table
#include "OVItem.h"


// ===========================================================================
//		* VEHierOutlineItem
// ===========================================================================

class VEModelObject;


// ===========================================================================
//		* VEHierOutlineItem
// ===========================================================================
//
//	Outline item for VEModelObject objects in the view editor's
//	Show Hierarchy window.
//
// ===========================================================================

class VEHierOutlineItem : public OVItem {

public:
							VEHierOutlineItem();
	virtual					~VEHierOutlineItem();

	// attribute accessors

	virtual DM_AttributeKey	GetAttributeKeyForCell(
									TableIndexT				inColumn) const;

	// drawing setup

protected:
	virtual void			GetDrawContentsSelf(
									const STableCell&		inCell,
									SOutlineDrawContents&	ioDrawContents);

	// configuration
	
	virtual void			SetElementSelf(
									DMElement*				inElement);


	// data members

protected:
	VEModelObject*			mModelObject;					// the object being portrayed at this
															//	position in the hierarchy

};
