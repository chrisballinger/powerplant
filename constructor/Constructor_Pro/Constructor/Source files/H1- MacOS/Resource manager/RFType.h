// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RFType.h					   © 1997 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 01/23/97
//     $Date: 2006/01/18 01:34:07 $
//  $History: RFType.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:51
//	Updated in $/Constructor/Source files/H1- MacOS/Resource manager
//	Finished adaptation to refactored RM/RF classes.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/31/97   Time: 15:20
//	Updated in $/Constructor/Source files/H1- MacOS/Resource manager
//	Upgraded to John Cortell's revisions.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/27/97   Time: 14:00
//	Updated in $/Constructor/Source files/H1- MacOS/Resource manager
//	Removed the multi-file capability. It was never used anyway.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 01/24/97   Time: 15:03
//	Created in $/Constructor/Source files/H1- MacOS/Resource manager
//	Refactored the RF* classes.
//	
// ===========================================================================

#pragma once

	// Core : Resource manager
#include "RMType.h"


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class RFMap;
class RFResource;


// ===========================================================================
//		* RFType
// ===========================================================================
//
//	The low-level representation of a single ResType entry in a Mac resource
//	file. Contains a list of the individual resources of this ResType that
//	exist in the file.
//
//	Note that the constructor and destructor are declared private. RFTypes
//	should only be created by calling RFMap::FindResType, and should only
//	be deleted by the RFMap destructor.
//
// ===========================================================================

class RFType : public RMType {

protected:
							RFType(
									RFMap&				inMap,
									ResType				inResType);
							~RFType();
	// resource accessors

public:
	virtual RMResource*		FindResource(
									const RMIdentifier&	inResID,
									Boolean				inCreateIfNeeded = false);
	RFResource*				FindResource(
									SInt16				inResID,
									Boolean				inCreateIfNeeded = false);

	inline ResType			GetResType() const
									{ return mResType; }
	inline RFMap&			GetRFMap() const
									{ return mResMap; }

	// resource ID arbitration

	SInt16					UniqueID(SInt16 inStartingFrom);

	// implementation details

private:
	virtual void			ScanRF();


	// data members

protected:
	RFMap&					mResMap;					// the resource map we belong to
	ResType					mResType;					// resource type represented by this object

	friend class RFMap;

};
