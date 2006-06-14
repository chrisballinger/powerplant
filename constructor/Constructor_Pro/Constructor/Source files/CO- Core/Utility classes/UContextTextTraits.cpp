// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	UContextTextTraits.cpp		© 1993-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 11/15/95			(adapted from PowerPlant's UTextTraits.cp)
//	   $Date: 2006/01/18 01:33:27 $
//	$History: UContextTextTraits.cpp $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/21/97   Time: 11:16
//	Updated in $/Constructor/Source files/CO- Core/Utility classes
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/22/97   Time: 20:08
//	Updated in $/Constructor/Source files/CO- Core/Utility classes
//	Upgraded to CW11 PowerPlant.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/17/96   Time: 14:32
//	Updated in $/Constructor/Source files/Utility classes
//	Improved commenting.
//
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 02:10
//	Created
//	Comment: Baseline source 15 October 1996.
//
// ===========================================================================

// ===========================================================================
//
//	Functions for using TextTraits. A TextTraits Record stores the following
//	text characteristics:
//		Font Name
//		Text Size
//		Text Style
//		Text Drawing Mode
//		Text Color
//	The Font Number is determined from the Font Name at runtime.
//
//	This module supports two ways of using TextTraits Records:
//		(1) in memory
//		(2) from a 'Txtr' resource
//
//	Putting font information in a resource makes programs easier to
//	localize for other languages. Also, you should store fonts by name
//	rather than by number, since font numbers are not constant across
//	different Systems. The exceptions are font number zero (sysFont)
//	and font number one (appFont), which are always the System font
//	and Application font. However, these are different for different
//	Script systems. For the English language Script, the System font
//	is Chicago and the Application font is Geneva.
//
//	When creating a 'Txtr' resource, specify -1 for the font number, which
//	will force a lookup by font name at runtime. Similarly, for a
//	TextTraits Record in memory, set the font number to -1 (use the
//	constant UTextTraits::fontNumber_Unknown) and set the font name
//	string (which you'll commonly get from a Font menu item).
//
// ===========================================================================

#include <UTextTraits.h>

	// Core : Editors : Views : User interface
#include "VELayoutView.h"

	// Core : Resource manager
//#include "StResourceContext.h"
#include <UResourceMgr.h>

	// MacOS : Resource manager
#include "RFMap.h"
#include "RFResource.h"

#include "UCustomTextTraits.h"

// ===========================================================================

#pragma mark *** UContextTextTraits ***

// ---------------------------------------------------------------------------
//		* LoadSystemTraits
// ---------------------------------------------------------------------------
//	Fill in a TextTraits Record with the System Font default traits

void
UTextTraits::LoadSystemTraits(
	TextTraitsRecord&	outTextTraits)
{
	outTextTraits.size = 0;
	outTextTraits.style = 0;
	outTextTraits.justification = teFlushDefault;
	outTextTraits.mode = srcOr;
	outTextTraits.color = Color_Black;
	outTextTraits.fontNumber = systemFont;
	outTextTraits.fontName[0] = 0;
}
	

// ---------------------------------------------------------------------------
//		* LoadTextTraits
// ---------------------------------------------------------------------------
//	Load Font Number into a TextTraits record, if necessary
//
//	A TextTraits record should initially specify a font name and set
//	the font number to UTextTraits::fontNumber_Unknown.
//
//	When this function is called with a TextTraits record that has a
//	font number of fontNumber_Unknown, it looks up and stores the font
//	number for the TextTrait record's font name. Subsequent calls to this
//	function will then do nothing (since the font number will be valid).

void
UTextTraits::LoadTextTraits(
	TextTraitsPtr	ioTextTraits)
{
	SignalIf_(ioTextTraits == nil);
	
	switch (ioTextTraits->fontNumber) {

		case fontNumber_Unknown:
			::GetFNum(ioTextTraits->fontName, &ioTextTraits->fontNumber);
			break;

		case systemFont:
			ioTextTraits->fontNumber = ::GetSysFont();
			break;

		case applFont:
			ioTextTraits->fontNumber = ::GetAppFont();
			break;
	}
}


// ---------------------------------------------------------------------------
//		* SetPortTextTraits
// ---------------------------------------------------------------------------
//	Set the text characteristics of the current GrafPort. A GrafPort does
//	not have a text justification field, so this function returns the
//	justification value in case you need it.
//
//	If inTextTraits is nil, the standard System font traits are used.
//
//	For example, the Toolbox trap TextBox takes a justification parameter,
//	so you can use this function as follows:
//		TextBox(myTextPtr, myTextLen, &myRect,
//					UTextTraits::SetPortTextTraits(myTextTraitsPtr);

SInt16
UTextTraits::SetPortTextTraits(
	ConstTextTraitsPtr	inTextTraits)
{
	SInt16 justification = teFlushDefault;
	
	if (inTextTraits == nil) {		// Use standard System font traits
		::TextFont(systemFont);
		::TextSize(0);
		::TextFace(0);
		::TextMode(srcOr);
		
		if (UEnvironment::HasFeature(env_SupportsColor)) {
			::RGBForeColor(&Color_Black);
			::RGBBackColor(&Color_White);
		}
	
	} else {						// Set characteristics from TextTraits
								
			// Cast away "const" here. Logically, the TextTraits Record
			// is not changed--the font number is cached so that future
			// references will be faster.
			
		LoadTextTraits(const_cast<TextTraitsPtr>(inTextTraits));

		::TextFont(inTextTraits->fontNumber);
		::TextSize(inTextTraits->size);
		::TextFace(inTextTraits->style);
		::TextMode(inTextTraits->mode);
		justification = inTextTraits->justification;
		
		if (UEnvironment::HasFeature(env_SupportsColor)) {
			::RGBForeColor(&inTextTraits->color);
			::RGBBackColor(&Color_White);
		}
	}
	
	return justification;
}


// ---------------------------------------------------------------------------
//		* SetTETextTraits
// ---------------------------------------------------------------------------
//	Set the text characteristics of a mono-style TextEdit Record (created
//	by TENew)
//
//	If inTextTraits is nil, the standard System font traits are used.
//
//	A TextEdit Record does not store the text color (it uses the
//	foreground color of the GrafPort). Pass the address of an RGBColor
//	record for the ioColor parameter if you want the color information.
//	Pass nil (the default value) if you don't want the color information.
//
//	This routine will change the text characteristics of the current
//	port to the specified text traits.

void
UTextTraits::SetTETextTraits(
	ConstTextTraitsPtr	inTextTraits,
	TEHandle			inMacTEH,
	RGBColor*			ioColor)
{
	
	// Build a valid text traits record (either from pointer or system traits)
	
	TextTraitsRecord txtr;
	if (inTextTraits != nil) {
		txtr = *inTextTraits;
		LoadTextTraits(&txtr);
	}
	else {
		txtr.size = 0;
		txtr.style = 0;
		txtr.justification = teFlushDefault;
		txtr.mode = srcOr;
		txtr.color.red = txtr.color.green = txtr.color.blue = 0;
		txtr.fontNumber = 0;
	}
	
	// Set text edit record to use this style.
	
	if ((**inMacTEH).txSize >= 0) {
	
		// Old-style TE record.
	
									// Jam text traits into TE Record
		(**inMacTEH).txFont = txtr.fontNumber,
		(**inMacTEH).txSize = txtr.size;
		(**inMacTEH).txFace = txtr.style;
		(**inMacTEH).txMode = txtr.mode;
		::TESetAlignment(txtr.justification, inMacTEH);
	
									// Set line spacing based on Font
		SetPortTextTraits(inTextTraits);
		FontInfo	info;
		::GetFontInfo(&info);
		(**inMacTEH).lineHeight = info.ascent + info.descent + info.leading;
		(**inMacTEH).fontAscent = info.ascent;

	}
	else {
		
		// New-style TE record.

									// Record old selection.
		SInt16 selStart = (**inMacTEH).selStart;
		SInt16 selEnd = (**inMacTEH).selEnd;
		
									// Quietly select all to set style.
		(**inMacTEH).selStart = 0;
		(**inMacTEH).selEnd = max_Int16;
		
									// Create one style run with this style.
		TextStyle newStyle;
		newStyle.tsFont = txtr.fontNumber;
		newStyle.tsFace = txtr.style;
		newStyle.tsSize = txtr.size;
		newStyle.tsColor = txtr.color;
		::TESetAlignment(txtr.justification, inMacTEH);
		::TESetStyle(doAll, &newStyle, true, inMacTEH);
		::TECalText(inMacTEH);
		
									// Now select nothing & do it again to set null scrap.
		(**inMacTEH).selStart = 0;
		(**inMacTEH).selEnd = 0;
		::TESetStyle(doAll, &newStyle, true, inMacTEH);
	
									// Restore the old selection.
		(**inMacTEH).selStart = selStart;
		(**inMacTEH).selEnd = selEnd;
	
	}

	// Pass back color information

	if (ioColor != nil) {
		*ioColor = txtr.color;
	}
}
	

// ---------------------------------------------------------------------------
//		* LoadTextTraits
// ---------------------------------------------------------------------------
//	Return a handle to a TextTraits Resource

TextTraitsH
UTextTraits::LoadTextTraits(
	ResIDT	inTextTraitsID)

	// ES: changed this routine for Constructor to restrict resource search
	// to current resource fork. Even then we only search if the current resource fork
	// is valid.

{
	TextTraitsH traitsH = nil;

	if (StResourceContext::CurrentContextIsValid()) {
		StResourceContext justInCase(StResourceContext::GetCurrentRefNum());
		traitsH = (TextTraitsH) ::Get1Resource('Txtr', inTextTraitsID);
	}
	// icw -- hack to build the IDE pref panels
	if (traitsH == nil)
	{
		traitsH = UCustomTextTraits::FindTextTraitsByID(inTextTraitsID);
	}
	
	if (traitsH != nil) {
		StHandleLocker	lock((Handle)traitsH);
		LoadTextTraits(*traitsH);
	} 

	return traitsH;
}



// ---------------------------------------------------------------------------
//		* SetPortTextTraits
// ---------------------------------------------------------------------------
//	Set the text characteristics of the current GrafPort. A GrafPort does
//	not have a text justification field, so this function returns the
//	justification value in case you need it.
//
//	For example, the Toolbox trap TextBox takes a justification parameter,
//	so you can use this function as follows:
//		TextBox(myTextPtr, myTextLen, &myRect,
//					UTextTraits::SetPortTextTraits(myTextTraitsID);

SInt16
UTextTraits::SetPortTextTraits(
	ResIDT	inTextTraitsID)
{
	TextTraitsPtr traitsPtr = nil;
	
	if (inTextTraitsID != Txtr_SystemFont) {
		TextTraitsH traitsH = nil;

		// Below is the extra code which allows Constructor to
		// fetch text traits from user files.

		RFMap* currentMap = VELayoutView::GetCurrentMap();
		if (currentMap != nil) {

			RFResource* txtrRF = currentMap->FindResource('Txtr', inTextTraitsID, false);
			if (txtrRF != nil) {
				ValidateSimpleObject_(txtrRF);
				traitsH = (TextTraitsH) txtrRF->GetResData();
			}
			else
			{
				// icw -- hack to build the IDE pref panels
				traitsH = UCustomTextTraits::FindTextTraitsByID(inTextTraitsID);
			}
			if (traitsH != nil) {
				SInt16 returnJust;
				{
					StHandleLocker	lock((Handle)traitsH);
					LoadTextTraits(*traitsH);
					returnJust = SetPortTextTraits(*traitsH);
				}
				::DisposeHandle((Handle) traitsH);
				return returnJust;
			}
			return SetPortTextTraits(traitsPtr);
		}

		traitsH = LoadTextTraits(inTextTraitsID);
		
		if (traitsH != nil) {
			StHandleLocker	lock((Handle)traitsH);
			return SetPortTextTraits(*traitsH);
		}
	}
	
	return SetPortTextTraits(traitsPtr);
}


// ---------------------------------------------------------------------------
//		* SetTETextTraits
// ---------------------------------------------------------------------------
//	Set the text characteristics of a mono-style TextEdit Record (created
//	by TENew)
//
//	A TextEdit Record does not store the text color (it uses the
//	foreground color of the GrafPort). Pass the address of an RGBColor
//	record for the ioColor parameter if you want the color information.
//	Pass nil (the default value) if you don't want the color information.
//
//	This routine will change the text characteristics of the current
//	port to the specified text traits.

void
UTextTraits::SetTETextTraits(
	ResIDT		inTextTraitsID,
	TEHandle	inMacTEH,
	RGBColor*	ioColor)
{
	TextTraitsPtr	traitsPtr = nil;
	
	if (inTextTraitsID != Txtr_SystemFont) {

		TextTraitsH traitsH = nil;
		RFMap* currentMap = VELayoutView::GetCurrentMap();
		if (currentMap != nil) {

			RFResource* txtrRF = currentMap->FindResource('Txtr', inTextTraitsID, false);
			if (txtrRF != nil) {
				ValidateSimpleObject_(txtrRF);
				traitsH = (TextTraitsH) txtrRF->GetResData();
			}
			else
			{
				// icw -- hack to build the IDE pref panels
				traitsH = UCustomTextTraits::FindTextTraitsByID(inTextTraitsID);
			}
			if (traitsH != nil) {
				{
					StHandleLocker	lock((Handle)traitsH);
					LoadTextTraits(*traitsH);
					SetTETextTraits(*traitsH, inMacTEH, ioColor);
				}
				::DisposeHandle((Handle) traitsH);
			}
			SetPortTextTraits(traitsPtr);
			return;
		}

		traitsH = LoadTextTraits(inTextTraitsID);
		if (traitsH != nil) {
			StHandleLocker	lock((Handle)traitsH);
			SetTETextTraits(*traitsH, inMacTEH, ioColor);
			return;
		}
	}
	
	SetTETextTraits(traitsPtr, inMacTEH, ioColor);
}


// ---------------------------------------------------------------------------
//		¥ LoadTextTraits
// ---------------------------------------------------------------------------
//	Fill in a TextTraitsRecord from a TextTraits resource specified by ID

void
UTextTraits::LoadTextTraits(
	ResIDT				inTextTraitsID,
	TextTraitsRecord	&outTextTraits)
{
	TextTraitsH	traitsH = LoadTextTraits(inTextTraitsID);
	if (traitsH != nil) {
		::BlockMoveData(*traitsH, &outTextTraits,
										::GetHandleSize((Handle) traitsH));
	} else {
		LoadSystemTraits(outTextTraits);
	}
}