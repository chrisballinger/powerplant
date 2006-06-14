// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	CTModelObject.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 12/09/96
//     $Date: 2006/01/18 01:33:50 $
//  $History: CTModelObject.h $
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
//	User: scouten      QDate: 12/10/96   Time: 14:22
//	Created in $/Constructor/Source files/Editors/Custom types
//	Baseline CTYP editor.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Attributes
#include "DMContainerAttribute.h"
#include "DMStringAttribute.h"

	// Core : Data model : Core objects
#include "DMObject.h"

	// Core : Utility classes
#include "GSharableString.h"


// ===========================================================================
//		* Helper class definitions
// ===========================================================================

class CTModelObject;
typedef TSharablePtr<CTModelObject> CTModelObjectPtr;


// ===========================================================================
//		* CTModelObject
// ===========================================================================

class CTModelObject : public DMObject {

	// data model description
	
	DMClassID_				('ctmo', CTModelObject, DMObject);
	DMContainerAttribute_	('cATL', AttributeList);
	DMStringAttribute_		('cNAM', Title);
	
	// constructor/destructor

public:
							CTModelObject();
							CTModelObject(LStream* inStream);
							CTModelObject(const CTModelObject& inOriginal);
	virtual					~CTModelObject();
	
	// accessors
	
	virtual void			GetDisplayableObjectType(
									LStr255&		outObjectType) const;
	virtual void			GetDisplayableName(
									LStr255&			outTitle) const;

	virtual	Boolean			CanDeleteThis() const;
	
	// data members

protected:
	GSharableStringPtr		mTitle;					// attribute type title
	UInt32					mTypeID;				// type ID for this attribute
	UInt32					mParentTypeID;			// parent type ID for this attribute


	friend class CTEditor;
	friend class CTTypeList;
	
};
