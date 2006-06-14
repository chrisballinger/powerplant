// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	MEMenuBarItem.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 10/20/96
//	   $Date: 2006/01/18 01:33:08 $
//	$History: MEMenuBarItem.cpp $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 17:06
//	Updated in $/Constructor/Source files/CO- Core/Editors/Menus
//	Improved commenting.
//	
//	*****************  Version 3   *****************
//	User: Andrew       Date: 02/04/97   Time: 21:04
//	Checked in '$/ConstructorWin/Sources'
//	Comment: Initial merge for MSVC-hosted build
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 12/06/96   Time: 23:55
//	Checked in '$/Constructor/Source files/Editors/Menus/+ Generic menu editor'
//	Comment: Updated to Clint's drop 12/06/96.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/23/96   Time: 20:00
//	Created
//	Comment: First version of class.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "MEMenuBarItem.h"

	// Core : Editors : Menus
#include "MEModelMenu.h"
#include "MEModelMenuBar.h"
#include "MEOutlineTable.h"


// ===========================================================================
//		* Class variables
// ===========================================================================

Handle MEMenuBarItem::sAppleMenuIconSuite = nil;


// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT Icon_AppleMenu			= -16386;		// from system file


// ===========================================================================

#pragma mark *** MEMenuBarItem ***

// ---------------------------------------------------------------------------
//		* MEMenuBarItem()
// ---------------------------------------------------------------------------
//	Constructor

MEMenuBarItem::MEMenuBarItem()
{
	mMenuBar = nil;

	if (sAppleMenuIconSuite == nil)
		::GetIconSuite(&sAppleMenuIconSuite, Icon_AppleMenu, svAllSmallData);
}


// ---------------------------------------------------------------------------
//		* ~MEMenuBarItem
// ---------------------------------------------------------------------------
//	Destructor

MEMenuBarItem::~MEMenuBarItem()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** drawing behaviors

// ---------------------------------------------------------------------------
//		* DrawRowAdornments										[protected]
// ---------------------------------------------------------------------------
//	Overriden to draw the other menu headers from the menu bar.
//	Also draws the rounded menu bar corner.

void
MEMenuBarItem::DrawRowAdornments(
	const Rect&		inLocalRowRect)
{
	
	ValidateThis_();

	// Draw inherited adornments.

	MEMenuHeaderItem::DrawRowAdornments(inLocalRowRect);

#if !PP_Target_Carbon
	// Draw rounded corner in upper left.
	
	StColorPenState::Normalize();
	::MoveTo(0, 4);
	::LineTo(0, 0);
	::LineTo(4, 0);
	::MoveTo(1, 2);
	::LineTo(1, 1);
	::LineTo(2, 1);
#endif

	// Draw other menu titles.

	DrawOtherMenuTitles(inLocalRowRect);

}


// ---------------------------------------------------------------------------
//		* DrawOtherMenuTitles									[protected]
// ---------------------------------------------------------------------------
//	Iterates over the menu bar's menu list, drawing titles of menus
//	as appropriate.

void
MEMenuBarItem::DrawOtherMenuTitles(
	const Rect&		inLocalRowRect)
{

	ValidateThis_();
	ValidateObject_(mMenuBar);

	// Iterate thru screen depths.
	
	StClipRgnState clip;
	clip.Save();
	
	{
		StDeviceLoop devIter(inLocalRowRect);
		SInt16 depth;

		while (devIter.NextDepth(depth)) {
		
			// Iterate thru menus, drawing their titles as requested.
			
			MEMenuBarIterator iter(mMenuBar);
			while (iter.NextMenu()) {

				// Get the current menu object.

				MEModelMenu* menu = iter.CurrentMenu();

				// Skip this menu if it's the currently shown menu.

				if (menu == mMenuHeader)
					continue;

				// See if this is the apple menu.
				// JWW - ...and don't draw it if under OS X 'cuz for some reason it doesn't work
				
				if (menu->GetAppleMenu() && (depth >= 4) && (UEnvironment::GetOSVersion() < 0x1000)) {
					
					// It is the apple menu, draw the icon instead
					// (but only if the screen depth allows it).
					
					Rect iconRect;
					iter.GetTextRect(iconRect);
					
					iconRect.top = 1;
					iconRect.bottom = 17;
					SInt16 center = (iconRect.left + iconRect.right) >> 1;
					iconRect.left = center - 8;
					iconRect.right = center + 8;
					::PlotIconSuite(&iconRect, atNone, ttNone, sAppleMenuIconSuite);
				
				}
				else if (menu->GetAppleMenu() and (depth >= 4) and UEnvironment::HasFeature(env_HasAquaTheme))
				{
					// icw -- We'll use Icon Services to draw the Apple menu.  I don't know how
					// to get the Aqua colored Apple logo that X uses, just the old rainbow logo.
					
					Rect iconRect;
					iter.GetTextRect(iconRect);
					
					iconRect.top = 1;
					iconRect.bottom = 17;
					SInt16 center = (iconRect.left + iconRect.right) >> 1;
					iconRect.left= center - 8;
					iconRect.right = center + 8;
					
					OSErr err;
					IconRef appleIconRef;
					err = ::GetIconRef(kOnSystemDisk, kSystemIconsCreator, kAppleLogoIcon, &appleIconRef);
					if (err == noErr and ::IsValidIconRef(appleIconRef))
					{
						err = ::PlotIconRef(&iconRect, atNone, ttNone, kIconServicesNormalUsageFlag, appleIconRef);
						::ReleaseIconRef(appleIconRef);
					}
				}
				else {
				
					// Not an apple menu (or on a B&W screen).
					// Draw the text normally.
				
					Point textLoc;
					iter.GetTextOrigin(textLoc);
					::MoveTo(textLoc.h, textLoc.v);

					LStr255 text;
					iter.GetText(text);

					if (menu->IsFakeMenu() || (menu->GetEnabled() == 0))
						::TextMode(grayishTextOr);				//* 2.2d9: BUG FIX #553: draw missing menus in gray
					else
						::TextMode(srcOr);

					::DrawString(text);

				}
			}
		}
	}

	// Normalize the text drawing.

	::TextMode(srcOr);
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** drawing behaviors

// ---------------------------------------------------------------------------
//		* SetElementSelf										[protected]
// ---------------------------------------------------------------------------
//	Once we know that we have an MEModelMenu, scan up the data model
//	until we find the menu bar that contains it.

void
MEMenuBarItem::SetElementSelf(
	DMElement*	inElement)
{

	// Sanity checks.

	ValidateThis_();
	ValidateObject_(inElement);
	
	// Make sure we really have a MEModelMenu.
	
	MEMenuHeaderItem::SetElementSelf(inElement);
	
	// Now make sure it's sitting inside a menu bar.
	
	DMElement* super1 = inElement->GetSuperElement();		// menu list container
	ValidateObject_(super1);

	DMElement* super2 = super1->GetSuperElement();			// menu bar object
	ValidateObject_(super2);

	mMenuBar = (dynamic_cast<MEModelMenuBar*>(super2));
	ValidateObject_(mMenuBar);

}