// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PDContainer.h				   © 1997 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 02/03/97
//     $Date: 2006/01/18 01:33:03 $
//  $History: PDContainer.h $
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:31
//	Created in $/Constructor/Source files/CO- Core/Documents/Project document
//	Refactored RS* classes into PD* (generic) and RS* (MacOS resource
//	manager) classes.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Attributes
#include "DMContainerAttribute.h"

	// Core : Data model : Core objects
#include "DMObject.h"


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class PDResource;
class PDSourceControl;
class PDType;
class RESession;
class RETypeEntry;
class RMIdentifier;
class RMMap;

struct RMMap_ChangeMessage;


// ===========================================================================
//		* PDContainer
// ===========================================================================
//
//	PDContainer is a base class for the top-level data model in
//	project documents. It provides common functionality for source-code
//	control and listening for resource-file changed messages. Subclasses
//	of PDContainer are built for specific project formats (e.g. MacOS
//	Resource Manager, Win32 .RES file).
//
// ===========================================================================

class PDContainer :	public DMObject,
					public LListener {

	// data model description

	DMAbstractClassID_		('pdco', PDContainer, DMObject);
	DMContainerAttribute_	('rTYP', ResourceTypeList);

	// constructors/destructor

public:
							PDContainer(LStream* inStream);
							PDContainer(const PDContainer& inOriginal);
	virtual					~PDContainer();

	// access control

	virtual Boolean			CanDeleteThis() const;
	inline PDSourceControl*	GetSourceControl()
									{ return mSourceControl; }

	// resource file accessors
	
	inline RMMap*			GetRMMap()
									{ return mRMMap; }
	inline RESession*		GetSession()
									{ return mSession; }
	inline Boolean			GetResIDsSignificant() const
									{ return mResIDsSignificant; }

	// resource locators
	
	virtual PDType*			FindResType(
									const RMIdentifier&	inResType,
									Boolean				inCreateIfNeeded = false) = 0;
	virtual PDResource*		FindResource(
									const RMIdentifier&	inResType,
									const RMIdentifier&	inResID,
									Boolean				inCreateIfNeeded = false);
	virtual Boolean			IsEditableResourceType(
									const RMIdentifier&	inResType) const;
	
	// new resource creation

	virtual void			CopyMasterResourceSet(
									RETypeEntry*		inResType,
									SInt32				inResIndex,
									const RMIdentifier&	inFileResID,
									ConstStr255Param	inFileResName);
	
	// resource change messages

protected:
	virtual void			ListenToMessage(
									MessageT			inMessage,
									void*				ioParam);

	virtual void			HandleResourceTypeAdded(
									const RMMap_ChangeMessage* inMessage);
	virtual void			HandleResourceAdded(
									const RMMap_ChangeMessage* inMessage);
	virtual void			HandleResourceRemoved(
									const RMMap_ChangeMessage* inMessage);
	virtual void			HandleResourceDataChanged(
									const RMMap_ChangeMessage* inMessage);
	virtual void			HandleResourceAttrsChanged(
									const RMMap_ChangeMessage* inMessage);

	// resource creation

	virtual PDType*			CreateResourceType(
									ResType					inResType);
	virtual void			CopyMasterResourceToFile(
									RETypeEntry*			inResType,
									SInt32					inResIndex,
									const RMIdentifier&		inCopyResType,
									const RMIdentifier&		inFileResID,
									ConstStr255Param		inFileResName);

	// data model changes

	virtual Boolean			CanHostElementSelf(
									const DMElement*		inNewElement,
									DM_DragPlacement*		ioPlacement);
	

	// configuration

	virtual void			SetRMMap(RMMap* inRMMap);
	inline void				SetSession(RESession* inSession)
									{ mSession = inSession; }

	// source control system

	virtual void			BuildSCCObject();

	// common initialization

private:
	void					InitPDContainer();
	

	// data members

protected:
	RMMap*					mRMMap;						// low-level resource map
	PDSourceControl*		mSourceControl;				// source-control information
	RESession*				mSession;					// editor session
	
	DMElementPtr			mPrototypeType;				// prototype for PDType objects
	DMElementPtr			mPrototypeResource;			// prototype for PDResource objects
	Boolean					mResIDsSignificant;			// true if resource IDs are significant

	friend class PDDocument;
	friend class PRDocument;

};

typedef TSharablePtr<PDContainer> PDContainerPtr;
