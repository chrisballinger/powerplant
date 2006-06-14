// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VECatalogPage.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/27/96
//     $Date: 2006/01/18 01:33:11 $
//  $History: VECatalogPage.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/20/97   Time: 18:26
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/Data model
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:29
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/Data model
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/30/96   Time: 18:06
//	Created in $/Constructor/Source files/Editors/Views/+ Generic view editor/Data model
//	Added class.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Attributes
#include "DMContainerAttribute.h"

	// Core : Data model : Core objects
#include "DMObject.h"


// ===========================================================================
//		* VECatalogPage
// ===========================================================================
//
//	VECatalogPages are created by VETypeList to organize the content of the
//	catalog. They are purely for display purposes, and serve no purpose to
//	the view editor engine itself.
//
// ===========================================================================

class VECatalogPage : public DMObject {

	// data model description
	
	DMClassID_				('vecp', VECatalogPage, DMObject);
	DMContainerAttribute_	('SCLS', SubClasses);
	
	// constructor/destructor

public:
							VECatalogPage(LStream* inStream);
							VECatalogPage(const VECatalogPage& inOriginal);
	virtual					~VECatalogPage();

	// accessors
	
	inline ClassIDT			GetPageClassID() const
									{ return mParentClassID; }
	inline void				SetPageClassID(ClassIDT inClassID)
									{ mParentClassID = inClassID; }
	
	inline void				GetPageTitle(LStr255& outTitle) const
									{ outTitle = mPageName; }
	inline void				SetPageTitle(const LStr255& inTitle)
									{ mPageName = inTitle; }

	inline SInt16			GetPageIconID() const
									{ return mPageIconID; }
	inline void				SetPageIconID(SInt16 inIconID)
									{ mPageIconID = inIconID; }


	// data members

protected:
	ClassIDT				mParentClassID;			// parent class of all members of this page
	LStr255					mPageName;				// visible name of this catalog page
	SInt16					mPageIconID;			// visible icon of this catalog page

};