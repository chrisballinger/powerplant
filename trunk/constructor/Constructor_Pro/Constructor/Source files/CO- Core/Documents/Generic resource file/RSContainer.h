// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RSContainer.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 05/28/96
//	   $Date: 2006/04/12 08:47:44 $
//	$History: RSContainer.h $
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:44
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Refactored RS* classes into PD* (generic) and RS* (MacOS resource
//	manager) classes.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 01/24/97   Time: 15:04
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Update for the refactored RFMap classes.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 12/18/96   Time: 12:10
//	Updated in $/Constructor/Source files/Documents/+ Generic resource file
//	Added some rather skanky code to allow dragging from the catalog to
//	project window.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/05/96   Time: 21:14
//	Updated in $/Constructor/Source files/Documents/+ Generic resource file
//	Made CanDeleteThis const to avoid hiding DMElement's method.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/05/96   Time: 14:50
//	Updated in $/Constructor/Source files/Documents/+ Generic resource file
//	Resource editor shell overhaul.
//
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/23/96   Time: 18:45
//	Checked in '$/Constructor/Source files/Documents/+ Generic resource file'
//	Comment: Deleted CanMakeChanges, updated to use new DMClassID_ macro.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:55
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Attributes
#include "DMContainerAttribute.h"

	// Core : Documents : Project document
#include "PDContainer.h"


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class RSResource;
class RSType;
class RFMap;
class VPModelObject;


// ===========================================================================
//		* RSContainer
// ===========================================================================
//
//	RSContainer is a base class for the top-level data model in MacOS
//	resource-file based documents. It provides common functionality
//	for source-code control and listening for resource-file changed
//	messages.
//
// ===========================================================================

// JCD 980515 - dunno if this hiding of FindResource was intentional or not,
// so I won't mess with the code, just quiet the compiler down

#ifndef __GNUC__
#pragma warn_hidevirtual off
#endif

class RSContainer :	public PDContainer {

	// data model description

	DMClassID_				('rsco', RSContainer, PDContainer);

	// constructors/destructor

public:
							RSContainer(LStream* inStream);
							RSContainer(const RSContainer& inOriginal);
	virtual					~RSContainer();

	// access control

	virtual Boolean			CanDeleteThis() const;

	// resource file accessors
	
	inline RFMap*			GetRFMap()
									{ return mRFMap; }

	// resource locators
	
	virtual PDType*			FindResType(
									const RMIdentifier&	inResType,
									Boolean				inCreateIfNeeded = false);
	virtual RSType*			FindResType(
									ResType				inResType,
									Boolean				inCreateIfNeeded = false);

	virtual RSResource*		FindResource(
									ResType				inResType,
									ResIDT				inResID,
									Boolean				inCreateIfNeeded = false);	
	// new resource creation

	virtual void			CopyMasterResourceSet(
									RETypeEntry*		inResTypeEntry,
									SInt32				inResIndex,
									const RMIdentifier&	inFileResID,
									ConstStr255Param	inFileResName);
	
	// resource change messages

protected:
	virtual void			HandleResourceTypeAdded(
									const RMMap_ChangeMessage* inMessage);
	virtual void			HandleResourceDataChanged(
									const RMMap_ChangeMessage* inMessage);
	
	// data model changes

	virtual Boolean			CanHostElementSelf(
									const DMElement*		inNewElement,
									DM_DragPlacement*		ioPlacement);
	virtual void			HostElementSelf(
									DMElement*				inNewElement,
									const DM_DragPlacement*	ioPlacement);
	virtual void			HostNewPPob(
									RSType*					inPPobType,
									VPModelObject*			inNewPPob);

	// configuration

	virtual void			SetRMMap(RMMap* inRMMap);

	// source control system

protected:
	virtual void			BuildSCCObject();

	// common initialization

private:
	void					InitRSContainer();
	

	// data members

protected:
	RFMap*					mRFMap;						// low-level resource map
	
	friend class RSDocument;

};

typedef TSharablePtr<RSContainer> RSContainerPtr;

#ifndef __GNUC__
#pragma warn_hidevirtual reset	// see above matching #pragma
#endif
