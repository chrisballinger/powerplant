// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	UIconUtilities.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 10/09/96
//	   $Date: 2006/01/18 01:33:28 $
//	$History: UIconUtilities.h $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/19/97   Time: 19:32
//	Updated in $/Constructor/Source files/CO- Core/Utility classes
//	Added a quick hack for Windows version. Improved commenting.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:33
//	Updated in $/Constructor/Source files/CO- Core/Utility classes
//	Fixed CR/LF problem
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/20/96   Time: 11:49
//	Updated in $/Constructor/Source files/Utility classes
//	Added GetIconSuiteFromMap function.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/03/96   Time: 15:56
//	Updated in $/Constructor/Source files/Utility classes
//	Corrected commenting errors.
//
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 02:10
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class RFMap;


// ===========================================================================
//		* UIconUtilities
// ===========================================================================
//
//	UIconUtilities contains some handy routines for manipulating icon suites.
//
// ===========================================================================

class UIconUtilities {

public:
	static Handle			Get1DetachedIconSuite(
									ResIDT				inResID,
									IconSelectorValue	inSelector);
	static Handle			GetIconSuiteFromMap(
									RFMap*				inRFMap,
									ResIDT				inResID,
									IconSelectorValue	inSelector);
	static CIconHandle		GetCIconFromMap(
									RFMap*				inRFMap,
									ResIDT				inResID);

	static Handle			Get1DetachedResource(
									ResType				inResType,
									ResIDT				inResID);


	static ResIDT			GetRemappedIconID(
									ResIDT inResID);

protected:
	static Boolean			Add1IconToSuite(
									Handle				inIconSuite,
									ResType				inResType,
									ResIDT				inResID);
	static Boolean			AddIconToSuiteFromMap(
									RFMap*				inRFMap,
									Handle				inIconSuite,
									ResType				inResType,
									ResIDT				inResID);

};