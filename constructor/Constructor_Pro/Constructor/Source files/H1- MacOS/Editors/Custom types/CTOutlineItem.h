// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	CTOutlineItem.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 12/10/96
//     $Date: 2006/01/18 01:33:50 $
//  $History: CTOutlineItem.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:02
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Custom types
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:42
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Custom types
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 12/10/96   Time: 14:23
//	Created in $/Constructor/Source files/Editors/Custom types
//	Baseline CTYP editor.
//	
// ===========================================================================

#pragma once

	// Core : Display classes (Eric) : Outline table
#include "OVItem.h"


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class CTModelObject;


// ===========================================================================
//		* CTOutlineItem
// ===========================================================================
//	Outline item for custom type model objects.

class CTOutlineItem : public OVItem {

public:
							CTOutlineItem();
	virtual					~CTOutlineItem();

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
	CTModelObject*			mModelObject;					// the object being portrayed at this
															//	position in the hierarchy

};
