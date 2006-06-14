// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RFMap.h				   		   © 1997 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 01/23/97
//     $Date: 2006/01/18 01:34:06 $
//  $History: RFMap.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:51
//	Updated in $/Constructor/Source files/H1- MacOS/Resource manager
//	Finished adaptation to refactored RM/RF classes.
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
#include "RMMap.h"


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class RFResource;
class RFType;


// ===========================================================================
//		* RFMap
// ===========================================================================
//
//	RFMap is the internal representation of the resource map of a 
//	Mac Resource file. It maintains the list of resources added and removed
//	since the file was last opened or saved. It may also be used without
//	a file (as in the case of new untitled documents or the resource
//	clipboard).
//
// ===========================================================================

class RFMap : public RMMap {

public:
							RFMap();
	virtual					~RFMap();

	// resource accessors

	virtual RMResource*		FindResource(
									const RMIdentifier&	inResType,
									const RMIdentifier&	inResID,
									Boolean				inCreateIfNeeded = false);
	RFResource*				FindResource(
									ResType				inResType,
									SInt16				inResID,
									Boolean				inCreateIfNeeded = false);

	virtual RMType*			FindResType(
									const RMIdentifier&	inResType,
									Boolean				inCreateIfNeeded = false);
	RFType*					FindResType(
									ResType				inResType,
									Boolean				inCreateIfNeeded = false);

	SInt16					GetResourceContext() const;

	// resource file updating

	virtual void			UpdateRF();
	
	// implementation details
	
private:
	virtual void			ScanRF();

};
