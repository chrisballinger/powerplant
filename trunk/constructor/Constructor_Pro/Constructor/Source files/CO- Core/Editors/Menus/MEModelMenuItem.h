// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	MEModelMenuItem.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 10/10/96
//	   $Date: 2006/01/18 01:33:10 $
//	$History: MEModelMenuItem.h $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 03/17/97   Time: 16:57
//	Updated in $/Constructor/Source files/CO- Core/Editors/Menus
//	Added object titles for property inspector. (Bug fix #1104.)
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
//	User: scouten      Date: 10/16/96   Time: 01:59
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Attributes
#include "DMBitfieldAttribute.h"
#include "DMContainerAttribute.h"
#include "DMIntegerAttribute.h"
#include "DMStringAttribute.h"

	// Core : Data model : Core objects
#include "DMObject.h"


// ===========================================================================
//		* MEModelMenuItem
// ===========================================================================
//	MEModelMenuItem is the data model object for a single menu item.

class MEModelMenuItem : public DMObject {

	// data model description

	DMClassID_				('memi', MEModelMenuItem, DMObject);
	DMStringAttribute_		('mDES', MenuText);
	DMIntegerAttribute_		('mDIV', DividerLine);
	DMIntegerAttribute_		('mICN', IconID);
	DMIntegerAttribute_		('mKEY', CommandKey);
	DMIntegerAttribute_		('mMRK', MarkCharacter);
	DMIntegerAttribute_		('mSCR', MenuScriptCode);
	DMIntegerAttribute_		('mSUB', SubMenuID);
	DMIntegerAttribute_		('mCMD', CommandNumber);
	DMIntegerAttribute_		('mRIC', UsesSmallICON);
	DMIntegerAttribute_		('mSIC', UsesSICN);
	DMIntegerAttribute_		('mENA', Enabled);
	DMBitfieldAttribute_	('mSTY', MenuStyle);
		DMIntegerAttribute_		('mEXT', Extend);
		DMIntegerAttribute_		('mCON', Condense);
		DMIntegerAttribute_		('mSHA', Shadow);
		DMIntegerAttribute_		('mOUT', Outline);
		DMIntegerAttribute_		('mUND', Underline);
		DMIntegerAttribute_		('mITA', Italic);
		DMIntegerAttribute_		('mBLD', Bold);
	// xmnu attributes
	DMIntegerAttribute_		('mCID', XmnuCommandID);
	DMBitfieldAttribute_	('mMOD', Modifiers);
		DMIntegerAttribute_		('mCOM', ModCommandKey);
		DMIntegerAttribute_		('mSFT', ModShiftKey);
		DMIntegerAttribute_		('mOPT', ModOptionKey);
		DMIntegerAttribute_		('mCTL', ModControlKey);
	DMIntegerAttribute_		('mTEC', TextEncoding);
	DMIntegerAttribute_		('mRC1', RefCon1);
	DMIntegerAttribute_		('mRC2', RefCon2);
	DMIntegerAttribute_		('mHIE', XmnuHierMenuID);
	DMIntegerAttribute_		('mFNT', FontID);
	DMIntegerAttribute_		('mGLY', KeyboardGlyph);


	// constructor/destructor

public:
							MEModelMenuItem(LStream* inStream);
							MEModelMenuItem(const MEModelMenuItem& inStream);
	virtual					~MEModelMenuItem();

	// accessors

	virtual void			GetDisplayableName(
									LStr255&			outTitle) const;


	// update notifications

protected:
	virtual void			SendUpdateMessage(
									MessageT			inMessage,
									void*				ioParam,
									DMElement*			inSubElement = nil);

	virtual void			EnableAttributes();

};
