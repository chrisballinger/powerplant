// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	MEModelMenu.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 10/10/96
//	   $Date: 2006/01/18 01:33:09 $
//	$History: MEModelMenu.h $
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
//	User: scouten      Date: 12/06/96   Time: 23:55
//	Checked in '$/Constructor/Source files/Editors/Menus/+ Generic menu editor'
//	Comment: Updated to Clint's drop 12/06/96.
//	
//	*****************  Version 3   *****************
//	User: scouten      Date: 10/24/96   Time: 01:18
//	Checked in '$/Constructor/Source files/Editors/Menus'
//	Comment: Finished menu editor.
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

#pragma once

	// Core : Data model : Attributes
#include "DMContainerAttribute.h"
#include "DMIntegerAttribute.h"
#include "DMStringAttribute.h"

	// Core : Data model : Core objects
#include "DMObject.h"


// ===========================================================================
//		* MEModelMenu
// ===========================================================================
//	MEModelMenu is the data model object for a MacOS menu resource.

class MEModelMenu : public DMObject {

	// data model description

	DMClassID_				('menu', MEModelMenu, DMObject);
	DMContainerAttribute_	('mITM', MenuList);
	DMStringAttribute_		('mDES', MenuTitle);
	DMIntegerAttribute_		('mAPM', AppleMenu);
	DMIntegerAttribute_		('ID  ', MenuID);
	DMIntegerAttribute_		('mMDF', MdefID);
	DMIntegerAttribute_		('mRES', MenuResID);
	DMIntegerAttribute_		('mENA', Enabled);


	// constructor/destructor

public:
							MEModelMenu(LStream* inStream);
							MEModelMenu(const MEModelMenu& inOriginal);
	virtual					~MEModelMenu();
	
	// accessors
	
	inline Boolean			IsFakeMenu() const
									{ return mFakeMenu; }
	inline void				SetFakeMenu(Boolean inFakeMenu)
									{ mFakeMenu = inFakeMenu; }

	inline Boolean			IsNewMenu() const
									{ return mIsNewMenu; }
	inline void				SetNewMenu(Boolean inNewMenu)
									{ mIsNewMenu = inNewMenu; }

	virtual void			GetDisplayableName(
									LStr255&			outTitle) const;

	virtual	Boolean			CanDeleteThis() const;
	
	// update notifications

protected:
	virtual void			SendUpdateMessage(
									MessageT			inMessage,
									void*				ioParam,
									DMElement*			inSubElement = nil);


	// data members

protected:
	Boolean					mFakeMenu;
	Boolean					mIsNewMenu;

};
