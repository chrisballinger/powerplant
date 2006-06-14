// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RSSourceStatusButton.h		© 1995-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 03/12/95
//	   $Date: 2006/01/18 01:33:02 $
//	$History: RSSourceStatusButton.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:45
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Refactored RS* classes into PD* (generic) and RS* (MacOS resource
//	manager) classes.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:28
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/05/96   Time: 15:00
//	Created in $/Constructor/Source files/Documents/+ Generic resource file
//	Resource editor shell overhaul.
//
// ===========================================================================

#pragma once


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class PDSourceControl;


// ===========================================================================
//		* RSSourceStatusButton
// ===========================================================================
//
//	This class implements a button that portrays the source code
//	settings for a file. Designed to work with RSSourceControl.
//
// ===========================================================================

class RSSourceStatusButton :	public LControl,
								public LListener {

public:
	enum { class_ID = 'PROJ' };

	static LPane*			CreateFromStream(LStream* inStream)
									{ return new RSSourceStatusButton(inStream); }
	
							RSSourceStatusButton(LStream* inStream);
							~RSSourceStatusButton();

	// configuration

	virtual void			AttachSourceControl(
									PDSourceControl*	inSettings);

	// drawing behaviors

protected:	
	virtual void			DrawSelf();
	
	// mouse-down behaviors
	
	virtual void			HotSpotResult(SInt16 inHotSpot);

	// source status updates
	
	virtual void			ListenToMessage(
									MessageT		inMessage,
									void*			ioParam);
	virtual void			StatusBlink();


	// data members

protected:
	PDSourceControl*		mSettings;				// source control settings
	
};
