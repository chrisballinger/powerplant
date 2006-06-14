// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	MEEditor.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 10/10/96
//	   $Date: 2006/04/12 08:48:12 $
//	$History: MEEditor.cpp $
//	
//	*****************  Version 15  *****************
//	User: scouten      QDate: 02/20/97   Time: 17:06
//	Updated in $/Constructor/Source files/CO- Core/Editors/Menus
//	Improved commenting.
//	
//	*****************  Version 14  *****************
//	User: scouten      QDate: 02/18/97   Time: 15:05
//	Updated in $/Constructor/Source files/CO- Core/Editors/Menus
//	Fixed a bug in ReadMenuResource that caused confusion when the 'Mcmd'
//	resource existed, but was empty. (Bug fix #1265.)
//	
//	*****************  Version 13  *****************
//	User: Andrew       Date: 2/04/97    Time: 8:57p
//	Updated in $/ConstructorWin/Sources
//	Added prefix file include for MSVC build
//	
//	*****************  Version 12  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:48
//	Updated in $/Constructor/Source files/CO- Core/Editors/Menus
//	Finished adaptation to refactored RM/RF classes.
//	
//	*****************  Version 11  *****************
//	User: scouten      QDate: 12/06/96   Time: 23:55
//	Updated in $/Constructor/Source files/Editors/Menus/+ Generic menu editor
//	Updated to Clint's drop 12/06/96.
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 11/30/96   Time: 19:39
//	Updated in $/Constructor/Source files/Editors/Menus
//	Fixed a bug in OpenEditorWindowSelf.
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 11/05/96   Time: 15:22
//	Updated in $/Constructor/Source files/Editors/Menus
//	Resource editor shell overhaul.
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 11/04/96   Time: 12:13
//	Updated in $/Constructor/Source files/Editors/Menus
//	Added ContainerListChanged method. Fixed a bug in WriteMenuItem
//	(divider lines could get garbled when written).
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 11/03/96   Time: 17:53
//	Updated in $/Constructor/Source files/Editors/Menus
//	Fixed a bug in UpdateResIDSelf.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 11/03/96   Time: 11:32
//	Updated in $/Constructor/Source files/Editors/Menus
//	Changed internal CTYP resources to DSPC.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 10/31/96   Time: 17:59
//	Updated in $/Constructor/Source files/Editors/Menus
//	Added support for deleting and renumbering menu resources.
//	
//	*****************  Version 4   *****************
//	User: scouten      Date: 10/24/96   Time: 01:17
//	Checked in '$/Constructor/Source files/Editors/Menus'
//	Comment: Finished menu editor.
//	
//	*****************  Version 3   *****************
//	User: scouten      Date: 10/23/96   Time: 20:56
//	Checked in '$/Constructor/Source files/Editors/Menus'
//	Comment: Baseline working version of menu editor.
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/16/96   Time: 22:21
//	Checked in '$/Constructor/Source files/Editors/Menus'
//	Comment: Fixed Mcmd parsing bug.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:58
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "MEEditor.h"

	// Core : Data model : Attributes
#include "DMContainerAttribute.h"

	// Core : Data model : Core objects
#include "DMReanimator.h"

	// Core : Display classes (Eric) : Window positioner
#include "LWindowPositioner.h"

	// Core : Documents : Generic resource file
#include "RSResource.h"

	// Core : Editors : Generic editor
#include "RESession.h"

	// Core : Editors : Menus
#include "MEModelMenuBar.h"
#include "MEModelMenu.h"
#include "MEModelMenuItem.h"
#include "MEOutlineTable.h"
#include "MEStrings.h"

	// Core : Resource manager
#include "RMIdentifier.h"

	// MacOS : Resource manager
#include "RFMap.h"
#include "RFResource.h"
#include "RFType.h"
//#include "StResourceContext.h"
#include <UResourceMgr.h>


// ===========================================================================
//		* Class variables
// ===========================================================================

Boolean MEEditor::sRegisteredClasses = false;


// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	PPob_MenuEditor			= 7000;
const PaneIDT	Pane_MenuOutlineTable	= 'MEOT';

const ResIDT	DSPC_MenuBar			= 7800;
const ResIDT	DSPC_Menu				= 7810;
const ResIDT	DSPC_MenuItem			= 7820;


// ===========================================================================

#pragma mark *** MEEditor ***

// ---------------------------------------------------------------------------
//		* MEEditor(LCommander*, RESession&)
// ---------------------------------------------------------------------------
//	Constructor

MEEditor::MEEditor(
	LCommander*			inSuper,
	RESession&			inSession)

: REEditor(inSuper, inSession)

{
}


// ---------------------------------------------------------------------------
//		* ~MEEditor
// ---------------------------------------------------------------------------
//	Destructor

MEEditor::~MEEditor()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** resource addition and deletion

// ---------------------------------------------------------------------------
//		* AddMenu
// ---------------------------------------------------------------------------
//	Call to create an MEModelMenu object which describes the data in
//	a MENU resource.

void
MEEditor::AddMenu(
	ResIDT			inMenuID,
	DMElementPtr&	inObjectPtr)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(&mSession);

	// Grab that resource for editing.
	
	RMResource* menuRes = mSession.AcquireResourceForEditing(
										this,				// inEditor
										GetMenuResourceType(),// inResType
										inMenuID,			// inResID
										false);				// inCreateIfNeeded
	
	if (menuRes != nil) {
		
		// Found the menu resource.
		
		ValidateSimpleObject_(menuRes);
		
		// Create a data model representation for the menu.
		
		ReadMenuResource((RFResource&) (*menuRes), inObjectPtr);
		ValidateObject_(inObjectPtr.GetObject());
		
	}
	else {
		
		// Menu resource doesn't exist in the file.
		// Create a non-editable stand-in.
	
		GetPrototypeMenu(inObjectPtr);
		
		MEModelMenu* menu = (dynamic_cast<MEModelMenu*>(inObjectPtr.GetObject()));
		ValidateObject_(menu);
	
		DMContainerAttribute* menuItemList = menu->GetMenuList();
		ValidateObject_(menuItemList);
		
		menuItemList->Disable();

		LStr255 resIDTitle((SInt32) inMenuID);
		menu->SetMenuTitle(resIDTitle);
		menu->SetMenuResID(inMenuID);
		menu->SetMenuID(inMenuID);
		menu->SetFakeMenu(true);
		
	}
}


// ---------------------------------------------------------------------------
//		* ReleaseMenu
// ---------------------------------------------------------------------------
//	Called by MEOutlineTable to signal that a MENU resource is no longer
//	part of the menu bar.

void
MEEditor::ReleaseMenu(
	MEModelMenu*	inMenu)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inMenu);
	
	// Write resource data (just in case user made changes
	// that would otherwise be lost).
	
	if (!inMenu->IsFakeMenu()) {
		
		// Grab the resource for writing.
		
		RMResource* menuRes = mSession.GetResource(GetMenuResourceType(), inMenu->GetMenuResID(), true);
		ValidateSimpleObject_(menuRes);
			
		// Write the resource data for the MENU resource
		// (only if menu was changed).
			
		if (inMenu->IsModified())
			WriteMenuResource((RFResource&) (*menuRes), inMenu);
			
		// Release the resource from editor.
		
		mSession.ReleaseResourceFromEditing(this, menuRes);

	}
}


// ---------------------------------------------------------------------------
//		* GetMenuResourceType
// ---------------------------------------------------------------------------
//	Return the resource type used for Menus.  

ResType			
MEEditor::GetMenuResourceType() const
{
	return 'MENU';
}


// ---------------------------------------------------------------------------
//		* GetDefaultTitleForPosition
// ---------------------------------------------------------------------------
//	Return a title string for a menu position  

void
MEEditor::GetDefaultTitleForPosition(
	SInt32 inPosition,
	LStr255 &outString)
{
	SInt16 stringNum;
	switch (inPosition) {
		case 1:
			stringNum = str_AppleMenuTitle;
			break;
		case 2:
			stringNum = str_FileMenuTitle;
			break;
		case 3:
			stringNum = str_EditMenuTitle;
			break;
		default:
			stringNum = str_UntitledMenuTitle;
	}

	LStr255 menuTitle(STR_MEMiscStrings, stringNum);
	outString = menuTitle;
}


// ===========================================================================

#pragma mark -
#pragma mark ** prototype menu item accessor

// ---------------------------------------------------------------------------
//		* GetPrototypeMenu
// ---------------------------------------------------------------------------
//	Return an empty menu object.

void
MEEditor::GetPrototypeMenu(
	DMElementPtr&	outMenu)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Get reanmiator and read the DSPC resource with
	// the default menu.

	DMReanimator* reanimator = DMReanimator::GetReanimator();
	ValidateObject_(reanimator);

	reanimator->ElementsFromResource('DSPC', DSPC_Menu, outMenu);
	ValidateObject_(outMenu.GetObject());

}


// ===========================================================================

#pragma mark -
#pragma mark ** session management implementation overrides

// ---------------------------------------------------------------------------
//		* ReadResourceDataSelf									[protected]
// ---------------------------------------------------------------------------
//	Read the MBAR and/or MENU resources.

void
MEEditor::ReadResourceDataSelf()
{

	// Validate pointers.

	ValidateThis_();
	ValidateSimpleObject_(mPrimaryResource);

	// See what kind of resource we have.
	
	RFResource* primaryRF = (dynamic_cast<RFResource*>(mPrimaryResource));
	ValidateObject_(primaryRF);
	
	ResType type = primaryRF->GetResType();
	
	if (GetMenuResourceType() == type)
		ReadMenuResource(*primaryRF, mDataModelRoot);
	else if (type =='MBAR')
		ReadMbarResource(*primaryRF, mDataModelRoot);
	else
		SignalCStr_("Opening invalid resource type.");
		

	ValidateObject_(mDataModelRoot.GetObject());
	
	// Get a selection for this menu editor.
	
	mSession.GetSelectionForEditor(this, mSelection);
	ValidateObject_(mSelection.GetObject());

	if (mSelection->GetRootElement() == nil)
		mSelection->SetRootElement(mDataModelRoot);

}


// ---------------------------------------------------------------------------
//		* WriteResourceDataSelf									[protected]
// ---------------------------------------------------------------------------
//	Write the MBAR and/or MENU resources.

void
MEEditor::WriteResourceDataSelf()
{

	// Validate pointers.

	ValidateThis_();
	ValidateSimpleObject_(mPrimaryResource);

	// See what kind of resource we have.
	
	RFResource* primaryRF = (dynamic_cast<RFResource*>(mPrimaryResource));
	ValidateObject_(primaryRF);
	
	ResType type = primaryRF->GetResType();
	if (GetMenuResourceType() == type) {
		MEModelMenu* menu = (dynamic_cast<MEModelMenu*>(mDataModelRoot.GetObject()));
		ValidateObject_(menu);
		WriteMenuResource((RFResource&) (*mPrimaryResource), menu);
	}
	else if (type =='MBAR') {
		MEModelMenuBar* menuBar = (dynamic_cast<MEModelMenuBar*>(mDataModelRoot.GetObject()));
		ValidateObject_(menuBar);
		WriteMbarResource((RFResource&) (*mPrimaryResource), menuBar);
	}
	else
		SignalCStr_("Writing invalid resource type.");
}


// ---------------------------------------------------------------------------
//		* OpenEditorWindowSelf									[protected]
// ---------------------------------------------------------------------------
//	Create the menu bar or menu editor window.

LWindow*
MEEditor::OpenEditorWindowSelf()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mDataModelRoot.GetObject());
	ValidateObject_(mSelection.GetObject());

	// Create the window.

	LWindow* window = nil;
	{
		StApplicationContext appContext;
		window = LWindow::CreateWindow(PPob_MenuEditor, this);
		ValidateObject_(window);
	}

	// Set the window title to the resource name.
	
	LStr255 title;
	mSession.GetWindowTitleForEditor(this, title);
	window->SetDescriptor(title);
	
	// Attach menu display view.

	MEOutlineTable* menuView = (dynamic_cast<MEOutlineTable*>
				(window->FindPaneByID(Pane_MenuOutlineTable)));

	ValidateObject_(menuView);
	menuView->SetSelection(mSelection);
	menuView->SetElement(mDataModelRoot);
	menuView->SetMEEditor(this);

	// Position & show the window.

	window->DoSetZoom(true);
	window->ProcessCommand(cmd_PositionWindow);
	window->Show();

	return window;

}


// ---------------------------------------------------------------------------
//		* ResourceDeletedSelf									[protected]
// ---------------------------------------------------------------------------
//	If a resource is deleted, make sure that it gets removed from
//	the menu bar.

void
MEEditor::ResourceDeletedSelf(
	RMResource*		inResource)
{

	// Validate pointers.

	ValidateThis_();
	ValidateSimpleObject_(inResource);

	// If this is a Menu resource, we might have an
	// MEModelMenu that matches it.
	
	RFResource* primaryRF = (dynamic_cast<RFResource*>(mPrimaryResource));
	ValidateObject_(primaryRF);
	
	if (primaryRF->GetResType() != GetMenuResourceType())
		return;
	
	// Make sure data model is put together.
	
	ReadResourceData();
	
	// Look for this menu resource.
	
	MEModelMenu* menu = FindMenu(primaryRF->GetResID());
	if (menu != nil) {
	
		// Found it. Delete it from the menu bar.
		// (It is a menu bar we're editing, right?)
		
		ValidateObject_(menu);
		ValidateObject_(mDataModelRoot.GetObject());

		MEModelMenuBar* mbar = (dynamic_cast<MEModelMenuBar*>(mDataModelRoot.GetObject()));
		ValidateObject_(mbar);
		
		DMContainerAttribute* menuList = mbar->GetMenus();
		ValidateObject_(menuList);
		
		menuList->RemoveElement(menu);
		
		// Rewrite the resource data now.
		
		WriteResourceData();
		
	}
}


// ---------------------------------------------------------------------------
//		* UpdateResIDSelf										[protected]
// ---------------------------------------------------------------------------
//	Overriden to update the menu ID when the resource ID changes.

void
MEEditor::UpdateResIDSelf(
	RMResource*		inResource,
	ResIDT			inOldResID,
	ResIDT			inNewResID)
{

	// Validate pointers.

	ValidateThis_();
	ValidateSimpleObject_(inResource);

	// If this isn't a Menu resource, ignore it.
	
	RFResource* primaryRF = (dynamic_cast<RFResource*>(mPrimaryResource));
	ValidateObject_(primaryRF);

	if (primaryRF->GetResType() != GetMenuResourceType())
		return;

	// Look for this menu resource.
	
	MEModelMenu* menu = FindMenu(inOldResID);
	if (menu == nil)
		menu = FindMenu(inNewResID);

	if (menu != nil) {
	
		// Found it. Update menu IDs.
	
		ValidateObject_(menu);
		menu->SetMenuID(inNewResID);
		menu->SetMenuResID(inNewResID);
		
		// Rewrite the resource data now.
		
		WriteResourceData();
		
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** menu streaming helpers
	
// ---------------------------------------------------------------------------
//		* ReadMbarResource										[protected]
// ---------------------------------------------------------------------------
//	Build up the menu editor data model by reading an MBAR resource
//	and all associated MENU resources.

void
MEEditor::ReadMbarResource(
	RFResource&		inResource,
	DMElementPtr&	inObjectPtr)
{

	// Validate pointers.

	ValidateThis_();
	ValidateSimpleObject_(&inResource);

	// Build input stream for the resource.
	
	Handle mbarData = inResource.GetResData();
	ValidateHandle_(mbarData);
	
	LHandleStream mbarStream(mbarData);

	// Build a menu bar item.
	
	DMReanimator* reanimator = DMReanimator::GetReanimator();
	ValidateObject_(reanimator);

	{
		StApplicationContext appContext;
		reanimator->ElementsFromResource('DSPC', DSPC_MenuBar, inObjectPtr);
		ValidateObject_(inObjectPtr.GetObject());
	}
	
	MEModelMenuBar* mbar = (dynamic_cast<MEModelMenuBar*>(inObjectPtr.GetObject()));
	ValidateObject_(mbar);

	DMContainerAttribute* mbarMenuList = mbar->GetMenus();
	ValidateObject_(mbarMenuList);
	
	// Got the menu bar object built. Let's read the MBAR resource
	// to see what menus belong in it.
	
	SInt16 menuCount = 0;
	mbarStream >> menuCount;

	while (menuCount--) {
	
		// Read a resource ID.
		
		SInt16 menuResID = 0;
		mbarStream >> menuResID;
		
		// Grab that resource for editing.
		
		DMElementPtr menuObject;
		AddMenu(menuResID, menuObject);

		ValidateObject_(menuObject.GetObject());
			
		// Add this menu to the menu bar's data model.
		
		mbarMenuList->InsertElementAt(
								LArray::index_Last,			// inAtIndex
								menuObject,					// inElement
								true);						// inBeSuperElement
			
	}	
}


// ---------------------------------------------------------------------------
//		* ReadMenuResource										[protected]
// ---------------------------------------------------------------------------
//	Build up a data model for a MENU resource and all of its
//	associated menu items. Called by InflateDataModelSelf or
//	InflateMbarResource.

void
MEEditor::ReadMenuResource(
	RFResource&		inResource,
	DMElementPtr&	inObjectPtr)
{

	// Validate pointers.

	ValidateThis_();
	ValidateSimpleObject_(&inResource);

	// Build input stream for the resource.
	
	Handle menuData = inResource.GetResData();
	ValidateHandle_(menuData);
	
	LHandleStream menuStream(menuData);
	LHandleStream mcmdStream;
	LHandleStream xmnuStream;
	
	// See if there's an 'Mcmd' resource with PowerPlant
	// command numbers.
	
	RMResource* mcmdRes = mSession.AcquireResourceForEditing(
										this,					// inEditor
										'Mcmd',					// inResType
										inResource.GetResID(),	// inResID
										false);					// inCreateIfNeeded

	if (mcmdRes != nil) {
		
		// Got the menu command resource object. Get its data.
		
		ValidateSimpleObject_(mcmdRes);
		
		if (!mcmdRes->IsDeleted())	//CCP 11/7/96 
		{
			Handle mcmdData = mcmdRes->GetResData();
			if (mcmdData != nil) {			//* EAS 02/12/97
				ValidateHandle_(mcmdData);
			
				mcmdStream.SetDataHandle(mcmdData);
			
				// Skip the item count.
			
				mcmdStream.SetMarker(2, streamFrom_Start);
			}
		}
		
	}

	// See if there's an 'xmnu' resource
	RMResource* xmnuRes = mSession.AcquireResourceForEditing(
										this,					// inEditor
										'xmnu',					// inResType
										inResource.GetResID(),	// inResID
										false);					// inCreateIfNeeded
	if (xmnuRes != nil)
	{
		// Got the menu command resource object. Get its data.
		ValidateSimpleObject_(xmnuRes);
		
		if (!xmnuRes->IsDeleted())
		{
			Handle xmnuData = xmnuRes->GetResData();
			if (xmnuData != nil)
			{
				ValidateHandle_(xmnuData);
			
				xmnuStream.SetDataHandle(xmnuData);
				SInt16	xmnuVersion;
				SInt16	xmnuItemExtensionsCount;
				
				xmnuStream >> xmnuVersion;
				xmnuStream >> xmnuItemExtensionsCount;
			}
		}
	}

	// Build a menu header item.
	
	GetPrototypeMenu(inObjectPtr);
	
	MEModelMenu* menu = (dynamic_cast<MEModelMenu*>(inObjectPtr.GetObject()));
	ValidateObject_(menu);

	DMContainerAttribute* menuItemList = menu->GetMenuList();
	ValidateObject_(menuItemList);
	
	// Got the menu object built. 
	// Read the MENU resource header.
	
		// * menu ID
	SInt16 menuID;
	menuStream >> menuID;
	menu->SetMenuID(menuID);
	menu->SetMenuResID(inResource.GetResID());

		// * width / height placeholders (ignored)	
	SInt16 foo;
	menuStream >> foo >> foo;
	
		// * MDEF ID
	SInt16 mdefID;
	menuStream >> mdefID;
	menu->SetMdefID(mdefID);
	
		// * placeholder for MDEF pointer (ignored)
	menuStream >> foo;
	
		// * enabled flags for menu and menu items
	UInt32 enabledFlags;
	menuStream >> enabledFlags;
	menu->SetEnabled(enabledFlags & 1);

		// * menu title	
	Str255 title;
	menuStream >> title;
	menu->SetMenuTitle(title);

	
	// Done with menu header. Start reading menu items.

	while (menuStream.GetMarker() < (menuStream.GetLength() - 1)) {
	
		// See if this new menu item will be enabled.
		
		enabledFlags >>= 1;
		enabledFlags |= 0x80000000;
		
		// See what the menu command number is.
		
		SInt32 menuCommand = 0;
		if (mcmdStream.GetMarker() <= (mcmdStream.GetLength() - 4))
			mcmdStream >> menuCommand;
		
		// Create a data model representation for the next menu item.
		
		DMElementPtr menuItemObject;

		ReadMenuItem(
					menuStream,					// inMenuStream
					xmnuStream,					// inXmnuStream
					enabledFlags & 1, 			// inEnabled
					menuCommand,				// inMenuCommand
					menuItemObject);			// inObjectPtr

		if (menuItemObject == nil) {
			SignalCStr_("Failed to build menu item object.");
			break;
		}

		ValidateObject_(menuItemObject.GetObject());
		
		// Add this menu item to the menu's data model.
		
		menuItemList->InsertElementAt(
								LArray::index_Last,			// inAtIndex
								menuItemObject,				// inElement
								true);						// inBeSuperElement
	
	}
}


// ---------------------------------------------------------------------------
//		* ReadMenuItem											[protected]
// ---------------------------------------------------------------------------
//	Called by InflateMenuResource to build up the data model for a
//	single menu item.

void
MEEditor::ReadMenuItem(
	LStream&		inMenuStream,
	LStream&		inXmnuStream,
	Boolean			inEnabled,
	SInt32			inMenuCommand,
	DMElementPtr&	inObjectPtr)
{

	// Validate pointers.

	ValidateThis_();

	// Create a menu item. For speed reasons, we don't read the DSPC
	// every time through here, but rather clone it from a prototype
	// object.
	
	ValidateObject_(mPrototypeMenuItem.GetObject());
	
	inObjectPtr = mPrototypeMenuItem->Clone();
	ValidateObject_(inObjectPtr.GetObject());
	
	MEModelMenuItem* item = (dynamic_cast<MEModelMenuItem*>(inObjectPtr.GetObject()));
	ValidateObject_(item);
	
	// Set enabled and menu command parameters.
	
	item->SetEnabled(inEnabled);
	item->SetCommandNumber(inMenuCommand);

	// Read menu title
	
	Str255 text;
	inMenuStream >> text;
	item->SetMenuText(text);
	
	// Interpret the four bytes after menu text. These fields are
	// badly multiplexed and so the parsing code looks a little odd.

	UInt8 iconNumber;
	UInt8 keyEquiv;
	UInt8 markChar;
	UInt8 menuStyle;
	inMenuStream >> iconNumber >> keyEquiv >> markChar >> menuStyle;
	
	// The key equivalent code determines how to interpret
	// the mark character and icon number fields. If the key equivalent
	// is a control character, it has special meaning.

	switch (keyEquiv) {
	
		// $1B: Menu has a submenu and no mark character.
	
		case 0x1B:
			item->SetSubMenuID(markChar);
			item->SetIconID(iconNumber ? iconNumber + 256 : 0);
			break;
		
		// $1C: Menu uses an alternate script system. It has no icon.
		
		case 0x1C:
			item->SetMenuScriptCode(iconNumber);
			item->SetMarkCharacter(markChar);
			break;
		
		// $1D: Menu uses an ICON resource reduced to 16x16.
		
		case 0x1D:
			item->SetUsesSmallICON(true);
			item->SetIconID(iconNumber ? iconNumber + 256 : 0);
			item->SetMarkCharacter(markChar);
			break;
		
		// $1E: Menu uses a SICN icon.

		case 0x1E:			
			item->SetUsesSICN(true);
			item->SetIconID(iconNumber ? iconNumber + 256 : 0);
			item->SetMarkCharacter(markChar);
			break;
		
		// Otherwise: Normal menu item.
	
		default:
			item->SetIconID(iconNumber ? iconNumber + 256 : 0);
			item->SetMarkCharacter(markChar);
			item->SetCommandKey(keyEquiv);
	
	}
	
	// Read the menu style field. This may later be replaced with
	// Mercutio-sensitive code.
	
	item->SetMenuStyle(menuStyle);
	
	SInt16	xmnuItemType;
	UInt32	xmnuCommandID;
	UInt8	xmnuModifiers;
	SInt8	xmnuFillByte;
	SInt32	xmnuFillLong;
	UInt32	xmnuTextEncoding;
	UInt32	xmnuRefCon1;
	UInt32	xmnuRefCon2;
	UInt16	xmnuHierMenuID;
	UInt16	xmnuFontID;
	SInt16	xmnuKeyboardGlyph;
	
	// WB1-31150 -- If the 'xmnu' resource doesn't exist, skip trying to read it
	if ((inXmnuStream.GetLength() > 0) and
		(inXmnuStream.GetMarker() <= static_cast<SInt32>(inXmnuStream.GetLength() - sizeof(xmnuItemType))))
	{
		inXmnuStream >> xmnuItemType;
		if ((xmnuItemType == 1) and (inXmnuStream.GetLength() - 
			(sizeof(xmnuCommandID) + sizeof(xmnuModifiers) + sizeof(xmnuFillByte) +
			sizeof(xmnuFillLong) + sizeof(xmnuTextEncoding) + sizeof(xmnuRefCon1) +
			sizeof(xmnuRefCon2) + sizeof(xmnuHierMenuID) + sizeof(xmnuFontID) +
			sizeof(xmnuKeyboardGlyph))))
		{
			inXmnuStream >> xmnuCommandID >> xmnuModifiers >> xmnuFillByte >> xmnuFillLong;
			inXmnuStream >> xmnuTextEncoding >> xmnuRefCon1 >> xmnuRefCon2 >> xmnuHierMenuID;
			inXmnuStream >> xmnuFontID >> xmnuKeyboardGlyph;

			item->SetXmnuCommandID(xmnuCommandID);
			item->SetModCommandKey((xmnuModifiers & kMenuNoCommandModifier) != kMenuNoCommandModifier);
			item->SetModShiftKey((xmnuModifiers & kMenuShiftModifier) == kMenuShiftModifier);
			item->SetModOptionKey((xmnuModifiers & kMenuOptionModifier) == kMenuOptionModifier);
			item->SetModControlKey((xmnuModifiers & kMenuControlModifier) == kMenuControlModifier);
			item->SetTextEncoding(xmnuTextEncoding);
			item->SetRefCon1(xmnuRefCon1);
			item->SetRefCon2(xmnuRefCon2);
			item->SetFontID(xmnuFontID);
			item->SetKeyboardGlyph(xmnuKeyboardGlyph);
		}
	}
}


// ---------------------------------------------------------------------------
//		* WriteMbarResource										[protected]
// ---------------------------------------------------------------------------
//	Called by WriteResourceData to write the contents of an MEModelMenuBar
//	to an MBAR resource.

void
MEEditor::WriteMbarResource(
	RFResource&			inResource,
	MEModelMenuBar*		inMenuBar)
{

	// Validate pointers.

	ValidateThis_();
	ValidateSimpleObject_(&inResource);
	ValidateObject_(inMenuBar);

	// Build output stream for the resource.
	
	LHandleStream mbarStream;

	// Write menu count.

	DMContainerAttribute* mbarMenuList = inMenuBar->GetMenus();
	ValidateObject_(mbarMenuList);
	
	SInt16 menuCount = mbarMenuList->GetSubElements().GetCount();
	mbarStream << menuCount;

	// Write menu resource IDs and MENU resources' data.
	
	DMFastIterator iter(mbarMenuList->GetSubElements());
	while (iter.NextElement()) {
	
		// Sanity check: Make sure this is a menu.
		
		MEModelMenu* menu = (dynamic_cast<MEModelMenu*>(iter.CurrentElement()));
		ValidateObject_(menu);
		
		// If this is a "new" menu, create an ID for it.
		
		if (menu->IsNewMenu()) {
			
			// Find Menu resource type object.
			
			ValidateSimpleObject_(mPrimaryResource);
			
			RMMap* map = &(mPrimaryResource->GetRMMap());
			ValidateObject_(map);
			
			RFType* menuType = 
				(dynamic_cast<RFType*>(map->FindResType(RMIdentifier(GetMenuResourceType()), true)));
			ValidateObject_(menuType);
			
			// Got Menu type object, ask it for unique ID.
			
			ResIDT newMenuID = menuType->UniqueID(128);
			
			// Assign this resource ID to the new menu.
			
			menu->SetMenuResID(newMenuID);
			menu->SetMenuID(newMenuID);
			menu->SetNewMenu(false);
			menu->MakeModified();
			
		}

		// Write its resource ID.
	
		SInt16 menuResID = menu->GetMenuResID();
		mbarStream << menuResID;

		// Write resource data if menu was changed.

		if (!(menu->IsFakeMenu())) {
			if (menu->IsModified() || menu->IsSubModified()) {
	
				// Grab the resource for writing.
				
				RMResource* menuRes = mSession.GetResource(GetMenuResourceType(), menuResID, true);
				ValidateSimpleObject_(menuRes);
				
				// Write the resource data for the MENU resource.
				
				WriteMenuResource((RFResource&) (*menuRes), menu);
	
			}
		}
	}

	// Write the 'MBAR' resource data.
	
	inResource.SetResData(mbarStream.GetDataHandle());

}


// ---------------------------------------------------------------------------
//		* WriteMenuResource										[protected]
// ---------------------------------------------------------------------------
//	Called by WriteResourceData or WriteMbarResource to write the contents
//	of an MEModelMenu to a MENU resource.

void
MEEditor::WriteMenuResource(
	RFResource&		inResource,
	MEModelMenu*	inMenu)
{

	// Validate pointers.

	ValidateThis_();
	ValidateSimpleObject_(&inResource);
	ValidateObject_(inMenu);

	// Build output stream for the resources.
	
	LHandleStream menuStream;
	LHandleStream mcmdStream;
	LHandleStream xmnuStream;
	Boolean mcmdNeeded = false;		// will be set to true if there are any non-zero command numbers
	Boolean xmnuNeeded = false;

	// Get menu item list.
	
	DMContainerAttribute* menuItems = inMenu->GetMenuList();
	ValidateObject_(menuItems);
	
	// Write 'Mcmd' item count.

	SInt16 itemCount = menuItems->GetSubElements().GetCount();
	mcmdStream << itemCount;

	// Write 'xmnu' version and item count
	SInt16	xmnuVersion = 0;
	xmnuStream << xmnuVersion;
	xmnuStream << itemCount;

	// Write the MENU resource header.
	
		// * menu ID
	SInt16 menuID = inMenu->GetMenuID();
	menuStream << menuID;

		// * width / height placeholders (ignored)	
	SInt16 foo = 0;
	menuStream << foo << foo;
	
		// * MDEF ID
	SInt16 mdefID = inMenu->GetMdefID();
	menuStream << mdefID;
	
		// * placeholder for MDEF pointer (ignored)
	menuStream << foo;
	
		// * enabled flags for menu and menu items
	UInt32 enabledFlags = inMenu->GetEnabled() ? 1 : 0;
	UInt32 enabledMask = 2;
	
	DMFastIterator iter(menuItems->GetSubElements());
	while (iter.NextElement()) {
	
		// Sanity check: Make sure this is a menu.

		MEModelMenuItem* item = (dynamic_cast<MEModelMenuItem*>(iter.CurrentElement()));
		ValidateObject_(item);
		
		// If it's enabled, set this bit in the enabled mask.
		
		if (item->GetEnabled())
			enabledFlags |= enabledMask;

		// Bump enabled mask so for subsequent menu items.
		// If we've done the first 31 items, stop. (It's a 31-bit
		// field, so there's no point in writing future items.)

		enabledMask <<= 1;
		if (enabledMask == 0)
			break;

	}
	
	while (enabledMask) {				// fill remaining bits with ones
		enabledFlags |= enabledMask;
		enabledMask <<= 1;
	}
		
	menuStream << enabledFlags;

		// * menu title	
	LStr255 title;
	inMenu->GetMenuTitle(title);
	menuStream << (ConstStringPtr) title;

	// While we're looking at the title, let's make sure the resource name
	// matches the menu title.
	
	LStr255 resName;
	inResource.GetResName(resName);
	
	if (resName.Length() == 0) {
		resName = title;
		if (inMenu->GetAppleMenu()) {
			StApplicationContext appContext;
			::GetIndString(resName, STR_MEMiscStrings, str_AppleMenuResTitle);
		}
		inResource.SetResName(resName);
	}


	// Done with menu header. Start writing menu items.
	
	iter.ResetTo(0);
	
	while (iter.NextElement()) {
	
		// Sanity check: Make sure this is a menu.
		
		MEModelMenuItem* item = (dynamic_cast<MEModelMenuItem*>(iter.CurrentElement()));
		ValidateObject_(item);
		
		// Write menu item data.
		
		WriteMenuItem(menuStream, xmnuStream, item);
		
		// Write menu command number.
		
		SInt32 menuCommand = item->GetCommandNumber();
		mcmdStream << menuCommand;
		
		if (menuCommand != 0)
			mcmdNeeded = true;
	
	}

	// Write zero byte at end of menu list.
	
	SInt8 zeroByte = 0;
	menuStream << zeroByte;
	
	// Write the Menu resource data.
	
	inResource.SetResData(menuStream.GetDataHandle());

	// Create or remove the 'Mcmd' resource as needed.
	
	RMResource* mcmdRes = mSession.GetResource('Mcmd', inResource.GetResID(), mcmdNeeded);
	if (mcmdNeeded) {
	
		// There were non-zero command numbers, make sure
		// 'Mcmd' data gets written.
	
		ValidateSimpleObject_(mcmdRes);
		mcmdRes->SetResData(mcmdStream.GetDataHandle());
		
		// Also make sure its name matches that of the Menu resource.
		
		LStr255 menuName;
		inResource.GetResName(menuName);
		mcmdRes->SetResName(menuName);
		
	}
	else if (mcmdRes != nil) {
	
		// There are no command numbers, but an 'Mcmd' resource exists
		// anyway. Delete it.

		ValidateSimpleObject_(mcmdRes);
		mcmdRes->DeleteResource();
	
	}

	// The 'xmnu' resource is only needed if we added menu item data to the stream
	// in WriteMenuItem.  If we did add anything, the stream size should be larger
	// than the version and item count.
	if (xmnuStream.GetLength() > static_cast<SInt32>(sizeof(xmnuVersion) + sizeof(itemCount)))
		xmnuNeeded = true;

	// Create or remove the 'xmnu' resource as needed.
	RMResource* xmnuRes = mSession.GetResource('xmnu', inResource.GetResID(), xmnuNeeded);
	if (xmnuNeeded)
	{
		// Make sure 'xmnu' data gets written.
		ValidateSimpleObject_(xmnuRes);
		xmnuRes->SetResData(xmnuStream.GetDataHandle());
		
		// Also make sure its name matches that of the Menu resource.
		LStr255 menuName;
		inResource.GetResName(menuName);
		xmnuRes->SetResName(menuName);
	}
	else if (xmnuRes != nil)
	{
		// The 'xmnu' isn't needed, but an 'xmnu' resource exists
		// anyway. Delete it.
		ValidateSimpleObject_(xmnuRes);
		xmnuRes->DeleteResource();
	}
}


// ---------------------------------------------------------------------------
//		* WriteMenuItem											[protected]
// ---------------------------------------------------------------------------
//	Called by WriteMenuResource to write the contents of an MEModelMenuItem
//	to part of a MENU resource.

void
MEEditor::WriteMenuItem(
	LStream&			inMenuStream,
	LStream&			inXmnuStream,
	MEModelMenuItem*	inMenuItem)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inMenuItem);
	
	// See if this is a menu divider. If so, ignore all other parameters.
	
	if (inMenuItem->GetDividerLine()) {

		// Write the menu title.
		
		StApplicationContext appContext;
		LStr255 dividerText(STR_MEMiscStrings, str_DividerLine);
		inMenuStream << (ConstStringPtr) dividerText;
		
		// Write the other four bytes. They are always zeroed out for divider lines.
		
		SInt32 zero = 0;
		inMenuStream << zero;
		
		// Skip this item in the 'xmnu'
		SInt16	xmnuItemType = 0;
		inXmnuStream << xmnuItemType;
	}
	else {

		// Write the menu title.
		
		LStr255 text;
		inMenuItem->GetMenuText(text);
		inMenuStream << (ConstStringPtr) text;
		
		// Figure out what the next four bytes. These are hopelessly
		// overlaid. We'll fill in the default values, then sort out
		// the reuse of values. Argh.

		UInt8 iconNumber = inMenuItem->GetIconID() - 256;
		UInt8 keyEquiv = inMenuItem->GetCommandKey();
		UInt8 markChar = inMenuItem->GetMarkCharacter();
		UInt8 menuStyle = inMenuItem->GetMenuStyle();
		
		if (inMenuItem->GetSubMenuID()) {
			keyEquiv = 0x1B;
			markChar = inMenuItem->GetSubMenuID();
		}
		else if (inMenuItem->GetMenuScriptCode()) {
			keyEquiv = 0x1C;
			iconNumber = inMenuItem->GetMenuScriptCode();
		}
		else if (inMenuItem->GetUsesSmallICON())
			keyEquiv = 0x1D;
		else if (inMenuItem->GetUsesSICN())
			keyEquiv = 0x1E;

		inMenuStream << iconNumber;					// icon #, script code, or 0
		inMenuStream << keyEquiv;					// key equiv, $1B, $1C, $1D, $1E, or 0
		inMenuStream << markChar;					// marking character, submenu ID, or 0
		inMenuStream << menuStyle;					// style of the menu item
		
		// Write this item in the 'xmnu'
		SInt16	xmnuItemType = 1;
		inXmnuStream << xmnuItemType;

		UInt32	xmnuCommandID = inMenuItem->GetXmnuCommandID();

		UInt8	xmnuModifiers = kMenuNoModifiers;
		if (inMenuItem->GetModCommandKey() == 0)
			xmnuModifiers |= kMenuNoCommandModifier;
		if (inMenuItem->GetModShiftKey())
			xmnuModifiers |= kMenuShiftModifier;
		if (inMenuItem->GetModOptionKey())
			xmnuModifiers |= kMenuOptionModifier;
		if (inMenuItem->GetModControlKey())
			xmnuModifiers |= kMenuControlModifier;

		SInt8	xmnuFillByte = 0;
		SInt32	xmnuFillLong = 0;
		UInt32	xmnuTextEncoding = inMenuItem->GetTextEncoding();
		UInt32	xmnuRefCon1 = inMenuItem->GetRefCon1();
		UInt32	xmnuRefCon2 = inMenuItem->GetRefCon2();
		UInt16	xmnuHierMenuID = inMenuItem->GetSubMenuID();
		UInt16	xmnuFontID = inMenuItem->GetFontID();
		SInt16	xmnuKeyboardGlyph = inMenuItem->GetKeyboardGlyph();

		inXmnuStream << xmnuCommandID;
		inXmnuStream << xmnuModifiers;
		inXmnuStream << xmnuFillByte;
		inXmnuStream << xmnuFillLong;
		inXmnuStream << xmnuTextEncoding;
		inXmnuStream << xmnuRefCon1;
		inXmnuStream << xmnuRefCon2;
		inXmnuStream << xmnuHierMenuID;
		inXmnuStream << xmnuFontID;
		inXmnuStream << xmnuKeyboardGlyph;
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** initialization

// ---------------------------------------------------------------------------
//		* FinishCreateSelf										[protected]
// ---------------------------------------------------------------------------

void
MEEditor::FinishCreateSelf()
{

	// Validate pointers.

	ValidateThis_();
	
	// Register menu editor classes.

	DMReanimator* reanimator = DMReanimator::GetReanimator();
	ValidateObject_(reanimator);

	if (!sRegisteredClasses) {
		sRegisteredClasses = true;
		RegisterMEClasses(reanimator);
	}
	
	// Cache prototype menu item.

	reanimator->ElementsFromResource('DSPC', DSPC_MenuItem, mPrototypeMenuItem);
	ValidateObject_(mPrototypeMenuItem.GetObject());

}


// ===========================================================================

#pragma mark -
#pragma mark ** update notifications

// ---------------------------------------------------------------------------
//		* ContainerListChanged									[protected]
// ---------------------------------------------------------------------------
//	If a menu is added to or removed from the menu bar, we should acquire
//	or release the resource for editing.

void
MEEditor::ContainerListChanged(
	DM_ListChange*	inMessage)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inMessage->mContainer);
	ValidateObject_(inMessage->mMovedElement);
	
	// See if we're changing the menu list.
	
	DMContainerAttribute* container = (dynamic_cast<DMContainerAttribute*>(inMessage->mContainer));
	if (container != nil) {
	
		// It's a container attribute, check the attribute key.
		
		ValidateObject_(container);
		if (container->GetKey() == MEModelMenuBar::attrKey_Menus) {

			// It is the menu list, find the menu we're adding.
		
			MEModelMenu* menu = (dynamic_cast<MEModelMenu*>(inMessage->mMovedElement));
			ValidateObject_(menu);

			// It is the menu list, see what's happening...
			
			if (inMessage->mElementAdded) {
			
				// Adding a menu... make sure we have this resource for editing.
			
				mSession.AcquireResourceForEditing(this, GetMenuResourceType(), menu->GetMenuResID(), false);
			
			}
			else {
			
				// Removing a menu... make sure we release the resource from editing.
				
				ReleaseMenu(menu);
			
			}
		}
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** menu helper

// ---------------------------------------------------------------------------
//		* FindMenu												[protected]
// ---------------------------------------------------------------------------
//	Find the data model object that corresponds to the menu whose resource
//	ID we've been given.

MEModelMenu*
MEEditor::FindMenu(
	ResIDT	inMenuID)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mDataModelRoot.GetObject());

	// Look for this menu resource.
	
	MEModelMenu* menu = (dynamic_cast<MEModelMenu*>(mDataModelRoot.GetObject()));
	if (menu != nil) {
	
		// Root is a menu. Make sure it matches the ID we're seeking.
		
		ValidateObject_(menu);
		if (menu->GetMenuResID() == inMenuID)
			return menu;
	
	}
	else {

		// Root isn't a menu. It should be a menu bar.

		MEModelMenuBar* mbar = (dynamic_cast<MEModelMenuBar*>(mDataModelRoot.GetObject()));
		ValidateObject_(mbar);
		
		// Iterate through menus until we find one with this ID.
		
		DMContainerAttribute* menuList = mbar->GetMenus();
		ValidateObject_(menuList);
		
		DMFastIterator iter(menuList->GetSubElements());
		while (iter.NextElement()) {
		
			menu = (dynamic_cast<MEModelMenu*>(iter.CurrentElement()));
			ValidateObject_(menu);
			
			if (menu->GetMenuResID() == inMenuID)
				return menu;
		
		}
	}	

	// Couldn't find the menu.
	
	return nil;

}

	
// ===========================================================================

#pragma mark -
#pragma mark ** class registration
	
// ---------------------------------------------------------------------------
//		* RegisterMEClasses								[static, protected]
// ---------------------------------------------------------------------------
//	Register display classes and data model classes used by menu editor.

void
MEEditor::RegisterMEClasses(
	DMReanimator*	inReanimator)
{
	// Register menu editor view classes.
	
	URegistrar::RegisterClass(MEOutlineTable::class_ID, (ClassCreatorFunc) MEOutlineTable::CreateFromStream);

	// Register menu editor data model classes.

	inReanimator->RegisterClass(MEModelMenuBar::class_ID, MEModelMenuBar::CreateFromStream);
	inReanimator->RegisterClass(MEModelMenu::class_ID, MEModelMenu::CreateFromStream);
	inReanimator->RegisterClass(MEModelMenuItem::class_ID, MEModelMenuItem::CreateFromStream);
}
