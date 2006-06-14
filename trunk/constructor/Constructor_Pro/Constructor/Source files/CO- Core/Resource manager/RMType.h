// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RMType.h					© 1995-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 10/09/95
//     $Date: 2006/01/18 01:33:25 $
//	$History: RMType.h $
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:46
//	Updated in $/Constructor/Source files/CO- Core/Resource manager
//	Finished adaptation to refactored RM/RF classes.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 01/31/97   Time: 15:24
//	Updated in $/Constructor/Source files/CO- Core/Resource manager
//	Upgraded to John Cortell's revisions.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/27/97   Time: 14:01
//	Updated in $/Constructor/Source files/CO- Core/Resource manager
//	Removed the multi-file capability. It was never used anyway.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 15:08
//	Updated in $/Constructor/Source files/CO- Core/Resource manager
//	Refactored the RF* classes.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/23/97   Time: 15:03
//	Updated in $/Constructor/Source files/CO- Core/Resource manager
//	Fixed a syntax error.
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


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class RMIdentifier;
class RMMap;
class RMResource;


// ===========================================================================
//		* RMType												[abstract]
// ===========================================================================
//
//	RMType maintains a list of resources belonging to a map that
//	share the same resource type. It provides accessors for finding
//	individual resources and choosing a unique resource ID within
//	this type.
//
//	This class is not intended to be subclassed. To save the memory,
//	no functions are declared virtual.
//
// ===========================================================================

class RMType {

	// constructor/destructor

protected:
							RMType(RMMap& inResMap);
	virtual					~RMType();

	// resource accessors

public:
	virtual RMResource*		FindResource(
									const RMIdentifier&	inResID,
									Boolean				inCreateIfNeeded = false) = 0;

	inline RMMap&			GetRMMap() const
									{ return mResMap; }
	inline LArray&			GetResources()
									{ return mResources; }

	virtual Boolean			IsModified();
	
	// implementation details

protected:
	virtual void			ScanRF() = 0;
	virtual void			UpdateRF();
	virtual void			UpdateComplete();

	virtual void			ResourceDeleted(RMResource* inResource);


	// data members

protected:
	RMMap&					mResMap;				// the resource map we belong to
	LArray					mResources;				// (of RMResource*) resource objects

	friend class RMMap;
	friend class RMResource;

};
