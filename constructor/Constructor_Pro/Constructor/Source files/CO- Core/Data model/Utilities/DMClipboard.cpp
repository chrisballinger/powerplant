// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMClipboard.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 06/05/96
//	   $Date: 2006/01/18 01:32:39 $
//	$History: DMClipboard.cpp $
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:13
//	Updated in $/Constructor/Source files/CO- Core/Data model/Utilities
//	Improved commenting.
//	
//	*****************  Version 5  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:00p
//	Updated in $/ConstructorWin/Sources
//	Initial merge with MSVC build
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:22
//	Updated in $/Constructor/Source files/CO- Core/Data model/Utilities
//	Fixed CR/LF problem
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 12/13/96   Time: 17:51
//	Updated in $/Constructor/Source files/Data model/Utilities
//	Fixed problems importing external scraps. (Bug fix #1062.).
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 12/07/96   Time: 12:24
//	Updated in $/Constructor/Source files/Data model/Utilities
//	Rolled in a bug fix from Eric Shapiro. Swapped ClearDMScrap &
//	SetDataSelf calls in SetDataSelf(). Improved commenting.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:31
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include <UScrap.h>
#include "DMClipboard.h"

	// Core : Data model : Core classes
#include "DMElement.h"


// ===========================================================================
//		* Class variables
// ===========================================================================

DMClipboard* DMClipboard::sDMClipboard = nil;


// ===========================================================================

#pragma mark *** DMClipboard ***

// ---------------------------------------------------------------------------
//		* DMClipboard()
// ---------------------------------------------------------------------------
//	Constructor

DMClipboard::DMClipboard()
{
	sDMClipboard = this;
}


// ---------------------------------------------------------------------------
//		* ~DMClipboard
// ---------------------------------------------------------------------------
//	Destructor

DMClipboard::~DMClipboard()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** scrap changers

// ---------------------------------------------------------------------------
//		* ClearDMScrap
// ---------------------------------------------------------------------------
//	Remove everything from the private scrap.

void
DMClipboard::ClearDMScrap()
{

	// Validate pointers.

	ValidateThis_();
	
	// Clear the private scrap.

	mDMScrap.RemoveItemsAt(mDMScrap.GetCount(), 1);

}


// ---------------------------------------------------------------------------
//		* AddDMScrapItem
// ---------------------------------------------------------------------------
//	Add a single item to the scrap.

void
DMClipboard::AddDMScrapItem(
	DMElement*	inElement,
	Boolean		inReset)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inElement);

	// Clear the scrap if appropriate.

	if (inReset) {
		UScrap::ClearData();
		ClearDMScrap();
	}
	
	// Add this element to the scrap list.

	mDMScrap.InsertItemAt(LArray::index_Last, inElement);

}


// ===========================================================================

#pragma mark -
#pragma mark ** overrides to catch scrap changes

// ---------------------------------------------------------------------------
//		* SetDataSelf											[protected]
// ---------------------------------------------------------------------------
//	Overriden to reset the private scrap if the primary scrap is reset.

void
DMClipboard::SetDataSelf(
	ResType		inDataType,
	Ptr			inDataPtr,
	SInt32		inDataLength,
	Boolean		inReset)
{

	// Validate pointers.

	ValidateThis_();
	ValidatePtr_(inDataPtr);

	// Clear the private scrap if appropriate.

	if (inReset)
		ClearDMScrap();
	
	// Add this object to the system scrap list.

	LClipboard::SetDataSelf(inDataType, inDataPtr, inDataLength, inReset);

}


// ---------------------------------------------------------------------------
//		* ImportSelf											[protected]
// ---------------------------------------------------------------------------
//	Overriden to erase the private scrap.

void
DMClipboard::ImportSelf()
{

	// Validate pointers.

	ValidateThis_();

	// Erase the private scrap.

	ClearDMScrap();

	// Import the external scrap.
	
	LClipboard::ImportSelf();

}
