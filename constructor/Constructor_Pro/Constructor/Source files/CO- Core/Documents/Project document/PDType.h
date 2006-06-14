// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PDType.h					   © 1997 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 02/03/97
//     $Date: 2006/01/18 01:33:05 $
//  $History: PDType.h $
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:31
//	Created in $/Constructor/Source files/CO- Core/Documents/Project document
//	Refactored RS* classes into PD* (generic) and RS* (MacOS resource
//	manager) classes.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Core classes
#include "DMObject.h"

	// Core : Data model : Attributes
#include "DMContainerAttribute.h"


// ===========================================================================
//		* Forward class definitions
// ===========================================================================

class PDResource;
class RETypeEntry;
class RMIdentifier;
class RMType;
class RMMap;

struct RMMap_ChangeMessage;

enum PD_ConflictRsp {
	icr_NotAsked,
	icr_Renumber,
	icr_Replace
};


// ===========================================================================
//		* PDType												[abstract]
// ===========================================================================
//
//	PDType is the data model object that serves as the container
//	for all resources of a single conceptual ResType. Note that one
//	PDType may exist for several RMTypes, since resource types
//	are sometimes coalesced for display in the project window (most
//	notably icon suites).
//
// ===========================================================================

class PDType :	public DMObject,
				public LListener {

	// data model description

	DMAbstractClassID_		('pdty', PDType, DMObject);
	DMContainerAttribute_	('rSCL', ResourceList);

	// constructor/destructor

public:
							PDType(LStream* inStream);
							PDType(const PDType& inOriginal);
	virtual					~PDType();

	// resource type accessors

	virtual Boolean			MatchResType(ResType inResType) const;
	virtual ResType			GetPrimaryResType() const;

	inline RETypeEntry*		GetTypeEntry() const
									{ return mTypeEntry; }

	// resource map accessors

	inline RMMap*			GetRMMap() const
									{ return mRMMap; }
	virtual RMType*			GetRMType(ResType inResType) const;

	// resource list
	
	virtual SInt32			CountResources() const;

	virtual void			ExpandResourceList() = 0;
	virtual void			CollapseResourceList();

	inline Boolean			IsExpanded() const
									{ return mExpanded; }

	virtual PDResource*		FindResource(
									const RMIdentifier&	inResType,
									const RMIdentifier&	inResID,
									Boolean				inCreateIfNeeded = false) = 0;
	virtual ResIDT			UniqueID(
									ResIDT				inStartingResID) = 0;

	// resource ID conflict resolution

	static void				SetResIDConflictResult(PD_ConflictRsp inConflictResult)
									{ sConflictResult = inConflictResult; }
	inline Boolean			GetResIDsSignificant() const
									{ return mResIDsSignificant; }

	// resource deletion

	virtual void			SetDeleted(Boolean inDeleted);

	// update notifications

protected:
	virtual void			ListenToMessage(
									MessageT			inMessage,
									void*				ioParam);

	virtual void			HandleResourceAdded(
									const RMMap_ChangeMessage* inMessage) = 0;
	

	// resource creation

	virtual PDResource*		CreateResource(
									const RMIdentifier&	inResType,
									const RMIdentifier&	inResID);

	// clipboard/drag-and-drop primitives

	virtual Boolean			CanHostElementSelf(
									const DMElement*	inNewElement,
									DM_DragPlacement*	ioPlacement);
	virtual void			HostElementSelf(
									DMElement*			inNewElement,
									const DM_DragPlacement* ioPlacement);

	virtual void			HostResType(
									PDType*				inNewType);
	virtual void			HostResource(
									PDResource*			inNewResource) = 0;

	// resource ID conflict resolution

	virtual PD_ConflictRsp	ShowResourceIDConflictDialog(
									const PDResource*	inExistingResource);

	// configuration
	
	inline void				SetPrototypeResource(DMElement* inPrototype)
									{ mPrototypeResource = inPrototype; }
	inline void				SetResIDsSignificant(Boolean inResIDsSignificant)
									{ mResIDsSignificant = inResIDsSignificant; }
	inline void				SetTypeEntry(RETypeEntry* inTypeEntry)
									{ mTypeEntry = inTypeEntry; }
	virtual void			SetRMMap(RMMap* inMap);


	// data members

protected:
	RMMap*					mRMMap;						// low-level resource map
	RETypeEntry*			mTypeEntry;					// resource capabilities
	Boolean					mExpanded;					// true if resource data model is inflated
	Boolean					mResIDsSignificant;			// true if resource IDs are significant

	DMElementPtr			mPrototypeResource;			// prototype of the resource object
														//  created whenever a new resource is noticed

	// class variables

protected:
	static PD_ConflictRsp	sConflictResult;			// response to resource ID conflicts

	friend class PDContainer;

};
