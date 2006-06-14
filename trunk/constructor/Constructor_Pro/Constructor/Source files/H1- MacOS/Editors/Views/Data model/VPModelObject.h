// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPModelObject.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 06/14/96
//	   $Date: 2006/01/18 01:33:55 $
//	$History: VPModelObject.h $
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:39
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Improved commenting.
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:44
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Fixed CR/LF problem
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 12/06/96   Time: 18:27
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added ReadStreamData override.
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 12/06/96   Time: 18:00
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added GetDisplayableObjectType method.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 12/06/96   Time: 16:35
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added BashNegativeSize method. Updated MoveBy and ResizeBy to call it.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 11/25/96   Time: 15:21
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added GetDescriptor method.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/24/96   Time: 19:39
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Finished AdaptToSuperFrameSize.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/22/96   Time: 15:59
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added DoSanityCheck, DoSanityCheckSelf, and BashSubPaneID methods.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/17/96   Time: 10:58
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added GetPrintableID and IsSizeAttribute methods.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/31/96   Time: 13:59
//	Created
//	Comment: 
//	
// ===========================================================================

#pragma once

	// Core : Editors : Views : Data model
#include "VEModelObject.h"

	// Core : Data model : Attributes
#include "DMContainerAttribute.h"
#include "DMIntegerAttribute.h"


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class VPTypeEntry;


// ===========================================================================
//		* VPModelObject
// ===========================================================================
//
//	VPModelObject is the common base class for data model representations of
//	PowerPlant objects (pane, attachment, radio group, or tab group).
//	Specific subclasses exist for some of these object types.
//
// ===========================================================================

class VPModelObject : public VEModelObject {

	// data model description
	
	DMClassID_				('vpmo', VPModelObject, VEModelObject);
	DMContainerAttribute_	('pATX', AttachmentList);
	DMIntegerAttribute_		('cALI', ClassAlias);
	
	// constructor/destructor

public:
							VPModelObject();
							VPModelObject(LStream* inStream);
							VPModelObject(const VPModelObject& inOriginal);
	virtual					~VPModelObject();
	
	// pane ID utilities

	SInt32					GetPaneID() const;

	virtual VPModelObject*	FindPaneByID(
									PaneIDT			inPaneID) const;
	virtual VPModelObject*	FindPaneByIDBackwards(
									PaneIDT			inPaneID) const;
	virtual Boolean			CheckForDuplicateID(
									PaneIDT			inPaneID) const;

	virtual void			GetDescriptor(
									LStr255&		outTitle) const;
	virtual void			GetPrintableID(
									LStr255&		outIDString) const;
	
	// subobject list
	
	DMContainerAttribute*	GetSubPaneList() const;
	
	// class information accessors

	inline VPTypeEntry*		GetVPTypeEntry() const
									{ return mVPTypeEntry; }
	virtual void			SetTypeEntry(
									VETypeEntry*	inTypeEntry);

	virtual void			GetDisplayableObjectType(
									LStr255&		outTitle) const;

	// handy pane moving/sizing utilities
	
	virtual void			MoveBy(
									SInt32			inHorizDelta,
									SInt32			inVertDelta);
	virtual void			ResizeBy(
									SInt32			inHorizDelta,
									SInt32			inVertDelta);
	virtual void			ResizeTo(
									SInt32			inHorizSize,
									SInt32			inVertSize);

	virtual Boolean			IsSizeAttribute(
									FourCharCode	inAttributeKey) const;

	// data model consistency checks
	
	virtual void			DoSanityCheck();
	
	// raw data streaming
	
	virtual void			ReadStreamData(
									LStream*		inStream,
									FourCharCode	inStreamSelector = 0);

	// pane resizing helper
	
protected:
	virtual void			AdaptToSuperFrameSize(
									SInt32			inSurrWidthDelta,
									SInt32			inSurrHeightDelta);

	// data model consistency checks
	
	virtual void			DoSanityCheckSelf();

	virtual void			BashSubPaneID(
									FourCharCode	inAttributeKey,
									ClassIDT		inRequiredSubClass);

	virtual void			BashNegativeSize();


	// data members

protected:
	TSharablePtr<VPTypeEntry> mVPTypeEntry;			// PowerPlant type description
	Boolean					mReadingStreamData;		// true if inside ReadStreamData call


	friend class VPModelView;

};


typedef TSharablePtr<VPModelObject> VPModelObjectPtr;
