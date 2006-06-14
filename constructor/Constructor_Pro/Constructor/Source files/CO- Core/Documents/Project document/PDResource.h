// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PDResource.h				   © 1997 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 02/03/97
//     $Date: 2006/01/18 01:33:04 $
//  $History: PDResource.h $
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:31
//	Created in $/Constructor/Source files/CO- Core/Documents/Project document
//	Refactored RS* classes into PD* (generic) and RS* (MacOS resource
//	manager) classes.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Core objects
#include "DMObject.h"


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class PDContainer;
class PDType;
class RETypeEntry;
class RMMap;
class RMResource;
class RMType;


// ===========================================================================
//		* PDResource											[abstract]
// ===========================================================================
//
//	PDResource is the data model object that represents a single
//	resource, or a group of resources that are treated as a unit.
//	It serves as a bridge between the data model and the low-level
//	resource map. This class is an abstract class that may be used
//	to represent resources from many platforms.
//
// ===========================================================================

class PDResource : public DMObject {

	// data model description

	DMAbstractClassID_		('pdrc', PDResource, DMObject);

	// constructor/destructor

public:
							PDResource(LStream* inStream);
							PDResource(const PDResource& inOriginal);
	virtual					~PDResource();

	// resource info accessors
	
	virtual ResType			GetResType() const = 0;
	virtual RETypeEntry*	GetResTypeEntry() const = 0;
	virtual StringPtr		GetResName(Str255 outResName) const = 0;
	virtual void			GetRelatedResources(LArray& outRMResources) const = 0;

	inline Boolean			GetResIDsSignificant() const
									{ return mResIDsSignificant; }
	inline void				SetResIDsSignificant(Boolean inResIDsSignificant)
									{ mResIDsSignificant = inResIDsSignificant; }

	// resource map accessors

	PDContainer*			GetPDContainer() const;
	PDType*					GetPDType() const;

	RMMap*					GetRMMap() const;
	RMType*					GetRMType() const;

	inline RMResource*		GetRMResource() const
									{ return mRMResource; }
	virtual void			SetRMResource(RMResource* inRMResource);
	
	// resource deletion

	virtual void			SetDeleted(Boolean inDeleted);

	virtual Handle			PackResData() const;
	virtual void			UnpackResData() = 0;

	// low-level resource info setters

protected:
	virtual void			PackOneResData(
									LHandleStream&		inStream,
									const RMResource*	inResource) const = 0;

	// common initialization

private:
	void					InitPDResource();


	// data members

protected:
	RMResource*				mRMResource;				// low-level resource entry for this resource
	Handle					mDeletedResData;			// packed resource data (for when this resource is deleted)
	Boolean					mResIDsSignificant;			// true if resource IDs are significant

	friend class PDContainer;
	friend class PDType;

};
