// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	MEMenuBarItem.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 10/20/96
//	   $Date: 2006/01/18 01:33:08 $
//	$History: MEMenuBarItem.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 17:07
//	Updated in $/Constructor/Source files/CO- Core/Editors/Menus
//	Improved commenting.
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
//	User: scouten      Date: 10/23/96   Time: 20:00
//	Created
//	Comment: First version of class.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Utilities
#include "DMIterator.h"

	// Core : Editors : Menus
#include "MEMenuHeaderItem.h"


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class MEModelMenuBar;


// ===========================================================================
//		* MEMenuBarItem
// ===========================================================================
//	MEMenuBarItem is an outline item that draws the menu bar.

class MEMenuBarItem : public MEMenuHeaderItem {

public:
							MEMenuBarItem();
	virtual					~MEMenuBarItem();
	
	// drawing behaviors

protected:
	virtual void			DrawRowAdornments(
									const Rect&			inLocalRowRect);
	virtual void			DrawOtherMenuTitles(
									const Rect&			inLocalRowRect);

	// configuration

	virtual void			SetElementSelf(
									DMElement*			inElement);
	

	// data members

protected:
	MEModelMenuBar*			mMenuBar;					// the menu bar we're drawing

	static Handle			sAppleMenuIconSuite;		// apple menu icon


};
