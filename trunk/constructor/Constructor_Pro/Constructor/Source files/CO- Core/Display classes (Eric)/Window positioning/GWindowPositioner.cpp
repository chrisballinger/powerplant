// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	GWindowPositioner.cpp		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 08/05/96
//	   $Date: 2006/01/18 01:32:47 $
//	$History: GWindowPositioner.cpp $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:58
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Window positioning
//	Improved commenting.
//	
//	*****************  Version 4  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:01p
//	Updated in $/ConstructorWin/Sources
//	Initial merge with MSVC build
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:25
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Window positioning
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 12/12/96   Time: 14:55
//	Updated in $/Constructor/Source files/Display classes (Eric)/Window positioning
//	Fixed stray preferences resource. (Bug fix #1072.) Improved commenting.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:45
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "GWindowPositioner.h"

	// Application object
#include "CAPreferencesFile.h"


// ===========================================================================

#pragma mark *** GWindowPositioner ***

// ---------------------------------------------------------------------------
//		* CreateWindowPositionerStream							[static]
// ---------------------------------------------------------------------------
//	Stream creation function

void*
GWindowPositioner::CreateWindowPositionerStream(
	LStream*	inStream)
{
	return new GWindowPositioner(inStream);
}


// ---------------------------------------------------------------------------
//		* GWindowPositioner(...)
// ---------------------------------------------------------------------------
//	Procedural constructor

GWindowPositioner::GWindowPositioner(
	LWindow*			inWindow,
	ResIDT				inResID,
	SInt16				inHorizStagger,
	SInt16				inVertStagger,
	Boolean				inAdjustPosition,
	Boolean				inAdjustSize)

: LWindowPositioner(inWindow, inResID, inHorizStagger, inVertStagger, inAdjustPosition, inAdjustSize)

{
}


// ---------------------------------------------------------------------------
//		* GWindowPositioner(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor

GWindowPositioner::GWindowPositioner(
	LStream*	inStream)

: LWindowPositioner(inStream)

{
}


// ---------------------------------------------------------------------------
//		* ~GWindowPositioner
// ---------------------------------------------------------------------------
//	Destructor

GWindowPositioner::~GWindowPositioner()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** window position resource

// ---------------------------------------------------------------------------
//		* ReadWindowPosition									[protected]
// ---------------------------------------------------------------------------
//	Read the WSPC resource from Constructor prefs file.

SWindowPositionH
GWindowPositioner::ReadWindowPosition(
	ResIDT	inResID)
{
	StPreferencesContext prefs;
	return LWindowPositioner::ReadWindowPosition(inResID);
}


// ---------------------------------------------------------------------------
//		* WriteWindowPosition									[protected]
// ---------------------------------------------------------------------------
//	Write the WSPC resource to Constructor prefs file.

void
GWindowPositioner::WriteWindowPosition(
	ResIDT				inResID,
	SWindowPositionH	inWindowPositionH)
{
	StPreferencesContext prefs;
	if (prefs.IsValid())						//* 2.4a2: BUG FIX #1072: added if statement
		LWindowPositioner::WriteWindowPosition(inResID, inWindowPositionH);
}
