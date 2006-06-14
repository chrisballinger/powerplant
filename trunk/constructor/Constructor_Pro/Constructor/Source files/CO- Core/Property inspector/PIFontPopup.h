// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PIFontPopup.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 10/31/96
//     $Date: 2006/01/18 01:33:21 $
//	$History: PIFontPopup.h $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/21/97   Time: 11:15
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Improved commenting.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:31
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Fixed CR/LF problem
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/26/96   Time: 17:56
//	Updated in $/Constructor/Source files/Property inspector
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 10/31/96   Time: 17:28
//	Updated in $/Constructor/Source files/Property inspector
//	Added history information.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/31/96   Time: 16:27
//	Created
//	Comment: Added class.
//	
// ===========================================================================

#pragma once

#include <LPopupButton.h>


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class DMFontStringAttribute;


// ===========================================================================
//		* PIFontPopup
// ===========================================================================
//
//	PIFontPopup is a variant on the standard grayscale popup that knows
//	about the list of value labels. Used only by PIFontItem.
//
// ===========================================================================

class PIFontPopup : public LPopupButton {

public:
	enum { class_ID = 'PIfp' };

	static void*				CreateFromStream(LStream* inStream)
										{ return new PIFontPopup(inStream); }

								PIFontPopup(LStream* inStream);
	virtual						~PIFontPopup();

	// value accessors

	virtual void				SetValueFromAttribute();
	virtual void				GetFontName(LStr255& outFontName);

	// configuration

	virtual void				SetAttribute(DMFontStringAttribute* inAttribute);

	// dynamic popup menu

protected:
	virtual MenuHandle			LoadPopupMenuH() const;

	virtual void				AddPredefinedFonts(MenuHandle inMenu) const;
	virtual void				AddInstalledFonts(MenuHandle inMenu) const;


	// data members

protected:
	DMFontStringAttribute*		mAttribute;

};