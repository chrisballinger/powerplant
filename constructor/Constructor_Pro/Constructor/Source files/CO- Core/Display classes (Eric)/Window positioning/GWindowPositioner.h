// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	GWindowPositioner.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 08/05/96
//	   $Date: 2006/01/18 01:32:48 $
//	$History: GWindowPositioner.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:58
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Window positioning
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:25
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Window positioning
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 12/12/96   Time: 14:57
//	Updated in $/Constructor/Source files/Display classes (Eric)/Window positioning
//	Fixed stray preferences resource. (Bug fix #1072.) Improved commenting.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:45
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// Core : Display classes (Eric) : Window positioning
#include "LWindowPositioner.h"


// ===========================================================================
//		* GWindowPositioner
// ===========================================================================
//
//	Constructor-specific version of LWindowPositioner that records
//	position in the Constructor preferences file.
//
// ===========================================================================

class GWindowPositioner : public LWindowPositioner {

public:
	enum { class_ID = 'Gpos' };

	static void*				CreateWindowPositionerStream(
										LStream*			inStream);

								GWindowPositioner(
										LWindow*			inWindow,
										ResIDT				inResID,
										SInt16				inHorizStagger = 16,
										SInt16				inVertStagger = 16,
										Boolean				inAdjustPosition = true,
										Boolean				inAdjustSize = false);
								GWindowPositioner(
										LStream*			inStream);
		
	virtual						~GWindowPositioner();

	// window position resource
	
	virtual SWindowPositionH	ReadWindowPosition(
										ResIDT				inResID);
	virtual void				WriteWindowPosition(
										ResIDT				inResID,
										SWindowPositionH	inWindowPositionH);			

};
