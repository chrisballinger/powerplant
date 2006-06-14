// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	MEModelMenuBar.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 10/10/96
//	   $Date: 2006/01/18 01:33:09 $
//	$History: MEModelMenuBar.cpp $
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 03/17/97   Time: 16:57
//	Updated in $/Constructor/Source files/CO- Core/Editors/Menus
//	Added object titles for property inspector. (Bug fix #1104.)
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/20/97   Time: 17:07
//	Updated in $/Constructor/Source files/CO- Core/Editors/Menus
//	Improved commenting.
//	
//	*****************  Version 4   *****************
//	User: Andrew       Date: 02/04/97   Time: 21:04
//	Checked in '$/ConstructorWin/Sources'
//	Comment: Initial merge for MSVC-hosted build
//	
//	*****************  Version 3   *****************
//	User: scouten      Date: 12/06/96   Time: 23:55
//	Checked in '$/Constructor/Source files/Editors/Menus/+ Generic menu editor'
//	Comment: Updated to Clint's drop 12/06/96.
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/23/96   Time: 20:56
//	Checked in '$/Constructor/Source files/Editors/Menus'
//	Comment: Baseline working version of menu editor.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:59
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "MEModelMenuBar.h"

	// Core : Data model : Attributes
#include "DMContainerAttribute.h"


// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	STR_METitleStrings		= 7010;
const ResIDT	str_MenuBar				= 1;
const ResIDT	str_Menu				= 2;
const ResIDT	str_MenuItem			= 3;
const ResIDT	str_MenuSeparator		= 4;
const ResIDT	str_SpaceOpenQuote		= 5;
const ResIDT	str_CloseQuote			= 6;


// ===========================================================================

#pragma mark *** MEModelMenuBar ***

// ---------------------------------------------------------------------------
//		* MEModelMenuBar(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. No extra data is read for MEModelMenu.

MEModelMenuBar::MEModelMenuBar(
	LStream* inStream)

: DMObject(inStream)

{
}


// ---------------------------------------------------------------------------
//		* MEModelMenuBar(MEModelMenuBar&)
// ---------------------------------------------------------------------------
//	Copy constructor

MEModelMenuBar::MEModelMenuBar(
	const MEModelMenuBar& inOriginal)

: DMObject(inOriginal)

{
	ValidateObject_(&inOriginal);
}


// ---------------------------------------------------------------------------
//		* ~MEModelMenuBar
// ---------------------------------------------------------------------------
//	Destructor

MEModelMenuBar::~MEModelMenuBar()
{
	ValidateThis_();
}


// ---------------------------------------------------------------------------
//		* GetDisplayableName
// ---------------------------------------------------------------------------
//	Make a displayable name for property inspector. Just returns the string
//	"Menu Bar".

void
MEModelMenuBar::GetDisplayableName(
	LStr255&	outTitle) const
{

	// Validate pointers.

	ValidateThis_();

	// Start with object type.
	
	outTitle = LStr255(STR_METitleStrings, str_MenuBar);

}
