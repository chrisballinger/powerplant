// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	MEModelMenuItem.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 10/10/96
//	   $Date: 2006/01/18 01:33:10 $
//	$History: MEModelMenuItem.cpp $
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

#include "MEModelMenuItem.h"


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

#pragma mark *** MEModelMenuItem ***

// ---------------------------------------------------------------------------
//		* MEModelMenuItem(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. No extra data is read for MEModelMenu.

MEModelMenuItem::MEModelMenuItem(
	LStream* inStream)

: DMObject(inStream)

{
}


// ---------------------------------------------------------------------------
//		* MEModelMenuItem(MEModelMenuItem&)
// ---------------------------------------------------------------------------
//	Copy constructor

MEModelMenuItem::MEModelMenuItem(
	const MEModelMenuItem& inOriginal)

: DMObject(inOriginal)

{
	ValidateObject_(&inOriginal);
}


// ---------------------------------------------------------------------------
//		* ~MEModelMenuItem
// ---------------------------------------------------------------------------
//	Destructor

MEModelMenuItem::~MEModelMenuItem()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** accessors

// ---------------------------------------------------------------------------
//		* GetDisplayableName
// ---------------------------------------------------------------------------
//	Make a displayable name containing the word Menu Item and the text
//	of the menu item.

void
MEModelMenuItem::GetDisplayableName(
	LStr255&	outTitle) const
{

	// Validate pointers.

	ValidateThis_();

	// Start with object type.
	
	outTitle = LStr255(STR_METitleStrings, GetDividerLine() ? str_MenuSeparator : str_MenuItem);

	// Add menu item title.
	
	if (!GetDividerLine()) {
		LStr255 title;
		GetMenuText(title);
		if (title.Length() > 0) {
			outTitle += LStr255(STR_METitleStrings, str_SpaceOpenQuote);
			outTitle += title;
			outTitle += LStr255(STR_METitleStrings, str_CloseQuote);
		}
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** update notifications

// ---------------------------------------------------------------------------
//		* SendUpdateMessage										[protected]
// ---------------------------------------------------------------------------
//	Overriden to enable/disable attributes as appropriate when
//	"separator line" flag changes.

void
MEModelMenuItem::SendUpdateMessage(
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
				
				// Menu text: Update divider line attribute.

				case attrKey_MenuText: {
					LStr255 text;
					GetMenuText(text);

					Boolean isDivider = (text[(UInt8)0] == 1) && (text[(UInt8)1] == '-');
					if (GetDividerLine() != isDivider)
						SetDividerLine(isDivider);
					break;
				}
				
				// Divider line: Update menu text attribute.
				
				case attrKey_DividerLine:
					EnableAttributes();
					if (GetDividerLine() != 0)
						SetEnabled(0);
					else
						SetEnabled(1);
					break;

			}
		}	
	}

	// Relay message as usual.

	DMObject::SendUpdateMessage(inMessage, ioParam, inSubElement);

}


// ---------------------------------------------------------------------------
//		* EnableAttributes										[protected]
// ---------------------------------------------------------------------------
//	Enable/disable attributes as divider line and other attributes change.

void
MEModelMenuItem::EnableAttributes()
{
	
	// Is this a divider line?
	
	if (GetDividerLine() == 1) {
	
		// Yes, everything else is disabled.

		GetMenuTextAttribute()			->Disable();
		GetIconIDAttribute()			->Disable();
		GetCommandKeyAttribute()		->Disable();
		GetMarkCharacterAttribute()		->Disable();
		GetMenuScriptCodeAttribute()	->Disable();
		GetSubMenuIDAttribute()			->Disable();
		GetCommandNumberAttribute()		->Disable();
		GetUsesSmallICONAttribute()		->Disable();
		GetUsesSICNAttribute()			->Disable();
		GetEnabledAttribute()			->Disable();
		GetMenuStyleAttribute()			->Disable();

		GetModifiersAttribute()			->Disable();
		GetTextEncodingAttribute()		->Disable();
		GetRefCon1Attribute()			->Disable();
		GetRefCon2Attribute()			->Disable();
		GetFontIDAttribute()			->Disable();
		GetKeyboardGlyphAttribute()		->Disable();
	
	}
	else {
	
		// Not a divider line. Most things are enabled.
	
		GetMenuTextAttribute()			->Enable();
		GetIconIDAttribute()			->Enable();
		GetCommandKeyAttribute()		->Enable();
		GetMarkCharacterAttribute()		->Enable();
		GetMenuScriptCodeAttribute()	->Enable();
		GetSubMenuIDAttribute()			->Enable();
		GetCommandNumberAttribute()		->Enable();
		GetUsesSmallICONAttribute()		->Enable();
		GetUsesSICNAttribute()			->Enable();
		GetEnabledAttribute()			->Enable();
		GetMenuStyleAttribute()			->Enable();

		GetModifiersAttribute()			->Enable();
		GetTextEncodingAttribute()		->Enable();
		GetRefCon1Attribute()			->Enable();
		GetRefCon2Attribute()			->Enable();
		GetFontIDAttribute()			->Enable();
		GetKeyboardGlyphAttribute()		->Enable();

			//! TEMPORARY: need to refine this...

	}
}
