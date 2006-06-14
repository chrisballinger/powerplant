// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	StKeepSharableAlive.h		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 12/01/96
//     $Date: 2006/01/18 01:33:27 $
//  $History: StKeepSharableAlive.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/21/97   Time: 11:16
//	Updated in $/Constructor/Source files/CO- Core/Utility classes
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:33
//	Updated in $/Constructor/Source files/CO- Core/Utility classes
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 12/01/96   Time: 02:11
//	Created in $/Constructor/Source files/Utility classes
//	Added class.
//	
// ===========================================================================

#pragma once


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class LSharable;


// ===========================================================================
//		* StKeepSharableAlive
// ===========================================================================
//
//	StKeepSharableAlive is used by a few classes within their constructors.
//	It prevents the object from getting deleted while in the constructor
//	due to another routine calling AddUser and then calling RemoveUser.
//
// ===========================================================================

class StKeepSharableAlive {

public:
	StKeepSharableAlive(LSharable* inSharable);
	~StKeepSharableAlive();

protected:
	LSharable*			mSharable;

};
