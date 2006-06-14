// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VECatalogPage.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/27/96
//     $Date: 2006/01/18 01:33:11 $
//  $History: VECatalogPage.cpp $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 18:25
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/Data model
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:07p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:29
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/Data model
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/30/96   Time: 18:06
//	Created in $/Constructor/Source files/Editors/Views/+ Generic view editor/Data model
//	Added class.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "VECatalogPage.h"


// ===========================================================================

#pragma mark *** VECatalogPage ***

// ---------------------------------------------------------------------------
//		* VECatalogPage(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. No additional data is read.

VECatalogPage::VECatalogPage(
	LStream*	inStream)

: DMObject(inStream)

{
	mParentClassID = 0;
}


// ---------------------------------------------------------------------------
//		* VECatalogPage(VECatalogPage&)
// ---------------------------------------------------------------------------
//	Copy constructor

VECatalogPage::VECatalogPage(
	const VECatalogPage&	inOriginal)

: DMObject(inOriginal)

{
	mParentClassID = inOriginal.mParentClassID;
	mPageName = inOriginal.mPageName;
	mPageIconID = inOriginal.mPageIconID;
}


// ---------------------------------------------------------------------------
//		* ~VECatalogPage
// ---------------------------------------------------------------------------
//	Destructor

VECatalogPage::~VECatalogPage()
{
}