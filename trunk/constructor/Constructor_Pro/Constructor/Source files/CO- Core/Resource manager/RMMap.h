// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RMMap.h						© 1995-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 10/09/95
//     $Date: 2006/01/18 01:33:25 $
//	$History: RMMap.h $
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:46
//	Updated in $/Constructor/Source files/CO- Core/Resource manager
//	Finished adaptation to refactored RM/RF classes.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/31/97   Time: 15:24
//	Updated in $/Constructor/Source files/CO- Core/Resource manager
//	Upgraded to John Cortell's revisions.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/27/97   Time: 14:01
//	Updated in $/Constructor/Source files/CO- Core/Resource manager
//	Removed the multi-file capability. It was never used anyway.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 15:08
//	Updated in $/Constructor/Source files/CO- Core/Resource manager
//	Refactored the RF* classes.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/23/97   Time: 14:06
//	Updated in $/Constructor/Source files/CO- Core/Resource manager
//	Cleaned up source in preparation for refactoring.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 02:09
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

#ifdef WINVER
	// PowerPlant includes. How is the MAC version building without
	// these? A pre-include header?
	#include "LBroadcaster.h"
	#include "LFile.h"
#endif


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class RMIdentifier;
class RMResource;
class RMType;


// ===========================================================================
//		* RMMap													[abstract]
// ===========================================================================
//
//	RMMap is a generic representation of a file of resources. It assumes
//	that there is a hierarchy of resource types and resource IDs, but makes
//	no assumptions about the nature of the files.
//
//	This class must be subclassed for each target platform's file type.
//
// ===========================================================================
	
class RMMap : public LBroadcaster {

public:
							RMMap();
	virtual					~RMMap();

	// resource accessors

	virtual RMResource*		FindResource(
									const RMIdentifier&	inResType,
									const RMIdentifier&	inResID,
									Boolean				inCreateIfNeeded = false) = 0;
	virtual RMType*			FindResType(
									const RMIdentifier&	inResType,
									Boolean				inCreateIfNeeded = false) = 0;

	inline LArray&			GetResTypes()
									{ return mResTypes; }

	// resource file updating

	virtual void			UpdateRF();
	virtual void			UpdateComplete();
	virtual Boolean			IsModified();

	// resource file access

	inline LFile*			GetMainFile() const
									{ return mMainFile; }
	
	virtual void			ScanNewMainFile(LFile* inFile);
	virtual void			SetMainFile(LFile* inFile);

	// implementation details
	
private:
	virtual void			ScanRF() = 0;


	// data members

protected:
	LFile*					mMainFile;				// primary resource file for this map
	LArray					mResTypes;				// (of RMType*)

};


// ===========================================================================
//		* Update messages
// ===========================================================================
//	A resource map (RMMap) is an LBroadcaster. User interface and data
//	model objects which act on this map should make themselves listeners
//	to the map so they can be made aware of changes to the map.

const MessageT ResourceType_Added		= 'RFT+';		// an RMType was added to the map
const MessageT Resource_Added			= 'RFR+';		// an RMResource was added to the map
const MessageT Resource_Removed			= 'RFR-';		// an RMResource was removed from the map
const MessageT Resource_DataChanged		= 'RFRC';		// a resource's data was changed
const MessageT Resource_AttrChanged		= 'RFRA';		// a resource's attributes were changed
const MessageT ResourceMap_GoingAway	= 'RFXX';		// the map is going away
	// parameter for all messages is RMMap_ChangeMessage*

struct RMMap_ChangeMessage {
	RMMap*				mRsrcMap;
	RMType*				mRsrcType;
	RMResource*			mResource;
};
