// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	TTSampleCaption.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 10/29/96
//	   $Date: 2006/01/18 01:33:53 $
//	$History: TTSampleCaption.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:18
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Text traits
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:43
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Text traits
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 12/07/96   Time: 12:35
//	Updated in $/Constructor/Source files/Editors/Text traits
//	Changed DrawSelf() to use luminance calculation (provided by Eric
//	Shapiro). Improved commenting.
//
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/30/96   Time: 18:36
//	Created
//	Comment: Baseline Txtr editor.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Utility classes
#include "DMListener.h"
#include <LStaticText.h>

// ===========================================================================
//		* TTSampleCaption
// ===========================================================================
//
//	TTSampleCaption displays a sample bit of text in the style of a text
//	traits resource that's being edited.
//
// ===========================================================================

class TTSampleCaption :	public LStaticText,
						public DMListener {

public:
	enum { class_ID = 'TTSC' };

	static LPane*			CreateFromStream(LStream* inStream)
									{ return new TTSampleCaption(inStream); }

							TTSampleCaption(LStream* inStream);
	virtual					~TTSampleCaption();

	// configuration

	virtual void			SetObject(DMObject* inObject);

	// drawing behaviors

protected:
	virtual void			DrawSelf();
	virtual void			ValueChanged(DM_ValueChange* inMessage);


	// data members

protected:
	DMObjectPtr				mTxtrObject;				// text traits data model object

};