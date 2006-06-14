// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMClipboard.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 06/05/96
//	   $Date: 2006/01/18 01:32:39 $
//	$History: DMClipboard.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:13
//	Updated in $/Constructor/Source files/CO- Core/Data model/Utilities
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:22
//	Updated in $/Constructor/Source files/CO- Core/Data model/Utilities
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 12/07/96   Time: 12:23
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

#pragma once

	// PowerPlant : CA : Array classes
#include <LSharableArray.h>


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class DMElement;


// ===========================================================================
//		* DMClipboard
// ===========================================================================
//
//	DMClipboard implements a private scrap for data model objects.
//	Copy objects by calling AddScrapItem, paste them by cloning objects
//	from the scrap list.
//
// ===========================================================================

class DMClipboard : public LClipboard {

public:
							DMClipboard();
	virtual					~DMClipboard();
	
	// singleton accessor

	static DMClipboard*		GetDMClipboard()
									{ return sDMClipboard; }

	// private scrap accessor

	const LSharableArray&	GetDMScrap() const
									{ return mDMScrap; }

	// scrap changers

	virtual void			ClearDMScrap();
	virtual void			AddDMScrapItem(
									DMElement*		inElement,
									Boolean			inReset);

	// overrides to catch scrap changes
	
protected:
	virtual void			SetDataSelf(
									ResType			inDataType,
									Ptr				inDataPtr,
									SInt32			inDataLength,
									Boolean			inReset);
	virtual void			ImportSelf();


	// data members

protected:
	LSharableArray			mDMScrap;				// list of copied items


	// class variables

protected:
	static DMClipboard*		sDMClipboard;			// singleton pointer

};
