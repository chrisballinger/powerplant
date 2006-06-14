// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RSResource.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 05/28/96
//	   $Date: 2006/01/18 01:33:01 $
//	$History: RSResource.h $
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 02/28/97   Time: 17:29
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Rolled in Clint's fixes for RM/RF classes.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/18/97   Time: 15:54
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Added ResourceDeleted method to keep links current when primary
//	resource changes.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:45
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Refactored RS* classes into PD* (generic) and RS* (MacOS resource
//	manager) classes.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:28
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/05/96   Time: 14:56
//	Updated in $/Constructor/Source files/Documents/+ Generic resource file
//	Resource editor shell overhaul.
//
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:55
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Attributes
#include "DMBitfieldAttribute.h"
#include "DMIntegerAttribute.h"
#include "DMStringAttribute.h"

	// Core : Data model : Core objects
#include "DMObject.h"

	// Core : Documents : Project document
#include "PDResource.h"

	// Core : Resource manager
#include "RFResource.h"
#include "RFType.h"


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class RFMap;
class RSContainer;
class RSType;


// ===========================================================================
//		* RSResource
// ===========================================================================
//
//	RSResource is the data model object that represents a single
//	resource, or a group of resources that are treated as a unit.
//	It serves as a bridge between the data model and the low-level
//	resource map.
//
// ===========================================================================

class RSResource : public PDResource {

	// data model description

	DMClassID_				('rsrc', RSResource, PDResource);
	DMIntegerAttribute_		('rTYP', ResourceType);
	DMIntegerAttribute_		('ID  ', ResourceID);
	DMStringAttribute_		('pnam', ResourceName);
	DMBitfieldAttribute_	('rATR', ResourceAttributes);
		DMIntegerAttribute_		('rSYS', SystemHeap);
		DMIntegerAttribute_		('rPUR', Purgeable);
		DMIntegerAttribute_		('rLCK', Locked);
		DMIntegerAttribute_		('rPRO', Protected);
		DMIntegerAttribute_		('rPRL', Preloaded);

	// constructor/destructor

public:
							RSResource(LStream* inStream);
							RSResource(const RSResource& inOriginal);
	virtual					~RSResource();

	// resource info accessors
	
	virtual ResIDT			GetResID() const;
	virtual ResType			GetResType() const;
	virtual SInt16			GetResAttrs() const;
	virtual RETypeEntry*	GetResTypeEntry() const;

	virtual StringPtr		GetResName(Str255 outResName) const;
	virtual void			GetDisplayableName(LStr255& outTitle) const;

	virtual void			GetRelatedResources(LArray& outRFResources) const;

	// resource map accessors

	RSContainer*			GetRSContainer() const;
	RSType*					GetRSType() const;

	RFMap*					GetRFMap() const;
	RFType*					GetRFType() const;

	inline RFResource*		GetRFResource() const
									{ return dynamic_cast<RFResource*>(mRMResource); }
	virtual void			SetRMResource(
									RMResource*			inRMResource);
	
	// resource deletion

	virtual void			UnpackResData();

	// update notifications

protected:
	virtual void			SendUpdateMessage(
									MessageT			inMessage,
									void*				ioParam,
									DMElement*			inSubElement = nil);

	// low-level resource info setters

	virtual void			PrimSetResID(ResIDT inResID);
	virtual void			PrimSetResName(ConstStringPtr inResName);
	virtual void			PrimSetResAttrs(SInt32 inResAttrs);
	virtual void			PreflightResID(ResIDT& inResID);

	virtual void			UpdateResIDData(
									ResIDT				inOldResID,
									ResIDT				inNewResID);

	virtual void			PackOneResData(
									LHandleStream&		inStream,
									const RMResource*	inResource) const;

	virtual void			ResourceDeleted(
									RFResource*			inResource);

	// common initialization

private:
	void					InitRSResource();


	// data members

protected:
	Boolean					mChangingID;				// true if we're in the midst of changing resource IDs
	Boolean					mUnpackChangeID;			// true to change ID when deleted resource is unpacked
	ResIDT					mUnpackResID;				// if mUnpackChangeID is true, this is the ID to change to

	friend class RSContainer;
	friend class RSType;

};
