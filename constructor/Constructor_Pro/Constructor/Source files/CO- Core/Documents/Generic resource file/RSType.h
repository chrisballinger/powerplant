// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RSType.h					© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 05/28/96
//	   $Date: 2006/01/18 01:33:02 $
//	$History: RSType.h $
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 02/28/97   Time: 11:30
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Added CanDeleteThis override for Pilot so you can't delete types from
//	the project window.
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 02/18/97   Time: 15:59
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Added code to keep links current when primary resource changes.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:45
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Refactored RS* classes into PD* (generic) and RS* (MacOS resource
//	manager) classes.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/29/97   Time: 14:33
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Fixed a bug in CountResources. It now counts icon suites properly. (Bug
//	fix #1087.)
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 15:04
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Update for the refactored RFMap classes.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 12/01/96   Time: 02:02
//	Updated in $/Constructor/Source files/Documents/+ Generic resource file
//	Changed Clone to call FinishCreate. Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/05/96   Time: 15:02
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

	// Documents : Project document
#include "PDType.h"


// ===========================================================================
//		* Forward class definitions
// ===========================================================================

class RFMap;
class RFType;
class RSResource;

typedef PD_ConflictRsp RS_ConflictRsp;


// ===========================================================================
//		* RSType
// ===========================================================================
//
//	RSType is the data model object that serves as the container
//	for all resources of a single conceptual ResType. Note that one
//	RSType may exist for several RFTypes, since resource types
//	are sometimes coalesced for display in the project window (most
//	notably icon suites).
//
// ===========================================================================

class RSType : public PDType {

	// data model description

	DMClassIDNoClone_		('rsty', RSType, PDType);

	// constructor/destructor

public:
							RSType(LStream* inStream);
							RSType(const RSType& inOriginal);
	virtual					~RSType();

	virtual DMElement*		Clone() const;

#if Constructor_ForPilot || Constructor_ForMacOS
	// icw -- Don't delete this on Mac either.  This is to workaround IL9901-1592.
	virtual Boolean			CanDeleteThis() const
								{ return false; }
#endif

	// resource map accessors

	inline RFMap*			GetRFMap() const
									{ return mRFMap; }
	virtual RFType*			GetRFType(ResType inResType) const;

	// resource list
	
	virtual void			ExpandResourceList();

	virtual PDResource*		FindResource(
									const RMIdentifier&	inResType,
									const RMIdentifier&	inResID,
									Boolean				inCreateIfNeeded = false);
	virtual RSResource*		FindResource(
									ResType				inResType,
									ResIDT				inResID,
									Boolean				inCreateIfNeeded);
	virtual ResIDT			UniqueID(
									ResIDT				inStartingResID);

	// resource ID conflict resolution

	inline Boolean			GetResIDsSignificant() const
									{ return mResIDsSignificant; }

	// update notifications

protected:
	virtual void			HandleResourceAdded(
									const RMMap_ChangeMessage* inMessage);
	virtual void			HandleResourceDeleted(
									const RMMap_ChangeMessage* inMessage);

	// clipboard/drag-and-drop primitives
	
	virtual void			HostResource(PDResource* inNewResource);

	// configuration
	
	virtual void			SetRMMap(RMMap*	inMap);


	// data members

protected:
	RFMap*					mRFMap;						// low-level resource map


	friend class RSContainer;

};
