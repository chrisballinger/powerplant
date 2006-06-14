// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	OVItemFactory.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 05/11/96
//     $Date: 2006/01/18 01:32:43 $
//	$History: OVItemFactory.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:35
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Outline table
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:24
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Outline table
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/30/96   Time: 19:19
//	Updated in $/Constructor/Source files/Display classes (Eric)/Outline table
//	Improved commenting.
//
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:44
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// PowerPlant : Feature classes
#include <LSharable.h>


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class DMElement;
class OVItem;


// ===========================================================================
//		* OVItemFactory											[abstract]
// ===========================================================================
//
//	OVItemFactory is a helper class for creating new outline items.
//	Since both OVTable and OVItem want to create items, this behavior
//	was factored out into this class. This is a pure abstract base class.
//	Override CreateItemForElement to return the appropriate subclass
//	of OVItem for the given element.
//
// ===========================================================================

class OVItemFactory : public LSharable {

public:
						OVItemFactory() {}
	virtual				~OVItemFactory() {}

	virtual OVItem*		CreateItemForElement(DMElement* inElement) = 0;

};
