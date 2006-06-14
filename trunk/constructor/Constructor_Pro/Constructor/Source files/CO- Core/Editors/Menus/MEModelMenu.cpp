// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	MEModelMenu.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 10/10/96
//	   $Date: 2006/01/18 01:33:09 $
//	$History: MEModelMenu.cpp $
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 03/17/97   Time: 16:57
//	Updated in $/Constructor/Source files/CO- Core/Editors/Menus
//	Added object titles for property inspector. (Bug fix #1104.)
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 02/20/97   Time: 17:07
//	Updated in $/Constructor/Source files/CO- Core/Editors/Menus
//	Improved commenting.
//	
//	*****************  Version 5   *****************
//	User: Andrew       Date: 02/04/97   Time: 21:04
//	Checked in '$/ConstructorWin/Sources'
//	Comment: Initial merge for MSVC-hosted build
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

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "MEModelMenu.h"

#include "MEModelMenuBar.h"


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

#pragma mark *** MEModelMenu ***

// ---------------------------------------------------------------------------
//		* MEModelMenu(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. No extra data is read for MEModelMenu.

MEModelMenu::MEModelMenu(
	LStream* inStream)

: DMObject(inStream)

{
	mFakeMenu = false;
	mIsNewMenu = false;
}


// ---------------------------------------------------------------------------
//		* MEModelMenu(MEModelMenu&)
// ---------------------------------------------------------------------------
//	Copy constructor

MEModelMenu::MEModelMenu(
	const MEModelMenu& inOriginal)

: DMObject(inOriginal)

{
	ValidateObject_(&inOriginal);
	mFakeMenu = inOriginal.mFakeMenu;
	mIsNewMenu = true;
}


// ---------------------------------------------------------------------------
//		* ~MEModelMenu
// ---------------------------------------------------------------------------
//	Destructor

MEModelMenu::~MEModelMenu()
{
	ValidateThis_();
}


// ===========================================================================

#pragma mark -
#pragma mark ** accessors

// ---------------------------------------------------------------------------
//		* GetDisplayableName
// ---------------------------------------------------------------------------
//	Make a displayable name containing the menu title.

void
MEModelMenu::GetDisplayableName(
	LStr255&	outTitle) const
{

	// Validate pointers.

	ValidateThis_();

	// Start with object type.
	
	outTitle = LStr255(STR_METitleStrings, str_Menu);

	// Add menu item title.
	
	LStr255 title;
	GetMenuTitle(title);
	if (title.Length() > 0) {
		outTitle += LStr255(STR_METitleStrings, str_SpaceOpenQuote);
		outTitle += title;
		outTitle += LStr255(STR_METitleStrings, str_CloseQuote);
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** update notifications

// ---------------------------------------------------------------------------
//		* SendUpdateMessage										[protected]
// ---------------------------------------------------------------------------
//	Overriden to keep the menu title and Apple menu flag attributes in sync.

void
MEModelMenu::SendUpdateMessage(
	MessageT			inMessage,
	void*				ioParam,
	DMElement*			inSubElement)
{

	ValidateThis_();

	// Intercept change messages from attributes.

	if (inMessage == Attribute_ValueChange) {
		
		// Make sure it's this object that's affected.
		
		DM_ValueChange* valueMsg = (DM_ValueChange*) ioParam;
		if (valueMsg->mObject == this) {
		
			ValidateObject_(valueMsg->mAttribute);
			
			// It is. Check attribute key.
			
			switch (valueMsg->mAttribute->GetKey()) {
				
				// Menu title: Update apple menu attribute.

				case attrKey_MenuTitle: {
					LStr255 text;
					GetMenuTitle(text);

					Boolean isAppleMenu = (text[(UInt8)0] == 1) && (text[(UInt8)1] == 0x14);
					if (GetAppleMenu() != isAppleMenu)
						SetAppleMenu(isAppleMenu);
					break;
				}
				
				// Apple menu: Update menu text attribute.
				
				case attrKey_AppleMenu: {
					LStr255 text;
					GetMenuTitle(text);
					
					Boolean isAppleMenu = (text[(UInt8)0] == 1) && (text[(UInt8)1] == 0x14);
					if (GetAppleMenu() != isAppleMenu) {
						if (GetAppleMenu()) {
							text[(UInt8)0] = 1;
							text[(UInt8)1] = 0x14;
							SetMenuTitle(text);
						}
						else {
							text = (StringPtr)"\004menu";				//! TEMPORARY: magic string
							SetMenuTitle(text);
						}
					}
					break;
				}

			}
		}	
	}

	// Relay message as usual.

	DMObject::SendUpdateMessage(inMessage, ioParam, inSubElement);

}


// ---------------------------------------------------------------------------
//	* CanDeleteThis
// ---------------------------------------------------------------------------
//	We override this here to prevent MW00431 from happening.
//
//	In the MENU editor, if you delete this item (the top level, the menu itself)
//	then it causes a crash. So we need to deny this action.
//
//	However, this same object is used in the menu bar editor because within
//	the menu bar editor you can edit menus. In the context of the menu bar
//	editor, we do want to have the ability to delete menus because you are
//	editing a menu bar... it's a totally desired behavior. So we just
//	can't return false (as the above paragraph would love to have as a simple
//	fix :-)
//
//	When we are in this context, we can examine the mSuperElement data member
//	(it comes from DMElement, the ultimate base class here). From what I've
//	watched in the debugger, the mSuperElement will be of type RSResource.
//	When we are in the menu bar editor context, the RSResource's
//	mSuperElement will be of type MEModelMenuBar. However, when we are in the
//	menu editor context, the RSResource's mSuperElement will be of type
//	DMContainerAttribute!! Bonus for us, let's hear it for polymorphism and
//	RTTI to bail us out here :-)
//
//	When we are within the menu editor context, we'll definately deny the
//	delete. But any other context we'll allow on through.

Boolean
MEModelMenu::CanDeleteThis() const
{
	Boolean	canDelete = false;	// assume we cannot

	DMElement	*theSuper = GetSuperElement();
	
	while (	theSuper != nil ) {
		
		MEModelMenuBar *theBar = dynamic_cast<MEModelMenuBar*>(theSuper);
		
		if ( theBar != nil ) {
			canDelete = true;
			break;
		}
		
		theSuper = theSuper->GetSuperElement();
	}

	return canDelete;
}