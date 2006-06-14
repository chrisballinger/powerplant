// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMFontStringAttribute.h		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 10/29/96
//	   $Date: 2006/01/18 01:32:31 $
//	$History: DMFontStringAttribute.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:13
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:19
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 10/31/96   Time: 18:11
//	Created in $/Constructor/Source files/Data model/Attributes
//	Added class.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Attributes
#include "DMStringAttribute.h"


// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	STR_FontStringAttribute		= 22550;
const SInt16	str_SystemFont				= 1;		// "System font"
const SInt16	str_ApplicationFont			= 2;		// "Application font"


// ===========================================================================
//		* DMFontStringAttribute
// ===========================================================================
//
//	DMFontStringAttribute represents a font reference in Txtr resources.
//	It can contain both a font number and a font name.
//
// ===========================================================================

class DMFontStringAttribute : public DMStringAttribute {

	DMClassID_				('fstr', DMFontStringAttribute, DMStringAttribute);

public:
							DMFontStringAttribute();
							DMFontStringAttribute(LStream* inStream);
							DMFontStringAttribute(const DMFontStringAttribute& inOriginal);
	virtual					~DMFontStringAttribute();

	// value accessors
	
	virtual void			GetTextValue(
									LStr255&			outValue) const;
	virtual void			SetTextValue(
									ConstStringPtr		inValue);

	inline SInt16			GetFontNumber() const
									{ return mFontNumber; }
	virtual void			SetFontNumber(
									SInt16				inFontNumber);

	virtual void			SetValueFromClone(
									const DMAttribute*	inValue);
	
	// display information
	
	inline Boolean			GetWriteFontNumber() const
									{ return mWriteFontNumber; }
	inline Boolean			GetIncludeSystemFont() const
									{ return mIncludeSystemFont; }
	inline Boolean			GetIncludeApplicatonFont() const
									{ return mIncludeApplicationFont; }
	inline Boolean			GetIncludeHiddenFonts() const
									{ return mIncludeHiddenFonts; }
	
	// raw data streaming

protected:
	virtual void			ReadStreamDataSelf(
									LStream*			inStream);
	virtual void			WriteStreamDataSelf(
									LStream*			inStream) const;


	// data members

protected:
	SInt16					mFontNumber;					// font number or -1 for use font name
	Boolean					mWriteFontNumber;				// true to write font number before name
	Boolean					mIncludeSystemFont;				// true to include system font in popups
	Boolean					mIncludeApplicationFont;		// true to include app font in popups
	Boolean					mIncludeHiddenFonts;			// true to include hidden fonts (%, .) in popups

};
