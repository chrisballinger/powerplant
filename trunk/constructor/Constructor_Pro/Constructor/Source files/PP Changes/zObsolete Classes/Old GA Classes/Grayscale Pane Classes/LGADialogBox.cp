// Copyright �2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright� 1996 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< � >>-----------------------------------------

FILE:					LGADialogBox.cp

CLASSES:				LGADialogBox

DESCRIPTION:		Implementation file for LGADialogBox

AUTHOR:				Robin Mair

CREATION DATE :	96.03.21

CHANGE HISTORY :

		97.04.20		rtm	Removed the CreateFromStream method.
		97.01.24		rtm	Added a copy constructor.
		96.09.25		rtm	Changed so that now the default button is setup from the ID
								passed in.  This means that you should no longer use a default
								button class just the normal LGAPushButton class then the dialog
								box will set it up correctly, based on the default ID passed.
		96.09.04		rtm	PowerPlant conformance changes.
		96.03.21		rtm	Initial creation of file

---------------------------------------<< � >>-----------------------------------------
*/

#ifdef PowerPlant_PCH
	#include PowerPlant_PCH
#endif

// � GRAYSCALE HEADERS
#include <LGADialogBox.h>
#include <LGAPushButton.h>
#include	<UGraphicsUtilities.h>

// � POWERPLANT HEADERS
#include <LStdControl.h>
#include <LStream.h>
#include <LString.h>
#include <PP_Messages.h>
#include <UReanimator.h>
#include <UKeyFilters.h>
#include <PP_KeyCodes.h>
#include <PP_Messages.h>


//=====================================================================================
// CONSTANTS
//=====================================================================================

//=====================================================================================
// DEFINES
//=====================================================================================

//=====================================================================================
// CLASS:	LGADialogBox
//=====================================================================================

#pragma mark === CLASS: LGADialogBox
#pragma mark -
#pragma mark === INITIALIZATION

//=====================================================================================
// �� INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LGADialogBox::LGADialogBox					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------
//
//	Default constructor

LGADialogBox::LGADialogBox ()
{

	mDefaultButtonID = PaneIDT_Undefined;
	mCancelButtonID = PaneIDT_Undefined;
	
}	//	LGADialogBox::LGADialogBox


//-------------------------------------------------------------------------------------
// LGADialogBox::LGADialogBox					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------
//
//	Copy constructor

LGADialogBox::LGADialogBox (	const LGADialogBox &inOriginal )
							:	LWindow ( inOriginal )
{

	mDefaultButtonID = inOriginal.mDefaultButtonID;
	mCancelButtonID = inOriginal.mCancelButtonID;
	
}	//	LGADialogBox::LGADialogBox


//-------------------------------------------------------------------------------------
// LGADialogBox::LGADialogBox					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------
//
//	Stream constructor

LGADialogBox::LGADialogBox (	LStream* inStream )
							: LWindow ( inStream )
{

	*inStream >> mDefaultButtonID;
	*inStream >> mCancelButtonID;
	
}	//	LGADialogBox::LGADialogBox


//-------------------------------------------------------------------------------------
// LGADialogBox::LGADialogBox					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------
//
//	Parameterized constructor

LGADialogBox::LGADialogBox ( SWindowInfo	&inWindowInfo )
												: LWindow ( inWindowInfo )
{

	mDefaultButtonID = PaneIDT_Undefined;
	mCancelButtonID = PaneIDT_Undefined;
	
}	//	LGADialogBox::LGADialogBox


//-------------------------------------------------------------------------------------
// LGADialogBox::LGADialogBox					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------
//
//	Parameterized constructor

LGADialogBox::LGADialogBox (	ResIDT		inWINDid,
										UInt32		inAttributes,
										LCommander	*inSuper )
							: LWindow ( inWINDid, inAttributes, inSuper )
{

	mDefaultButtonID = PaneIDT_Undefined;
	mCancelButtonID = PaneIDT_Undefined;
	
}	//	LGADialogBox::LGADialogBox


//-------------------------------------------------------------------------------------
// LGADialogBox::~LGADialogBox				---<<� DESTRUCTOR �>>---
//-------------------------------------------------------------------------------------

LGADialogBox::~LGADialogBox ()
{
}	//	LGADialogBox::~LGADialogBox


//-------------------------------------------------------------------------------------
// LGADialogBox::FinishCreateSelf
//-------------------------------------------------------------------------------------

void
LGADialogBox::FinishCreateSelf ()
{
	LGAPushButton* theButton;		
	
	// � Link the default and cancel buttons to the dialog listeners
	// DEFAULT
	if ( mDefaultButtonID != PaneIDT_Undefined )
	{
		theButton = (LGAPushButton*) FindPaneByID ( mDefaultButtonID );
		if ( theButton != nil ) 
		{
			theButton->AddListener ( this );
			theButton->SetDefaultButton ( true );
		}
	}
	// CANCEL
	if ( mCancelButtonID != PaneIDT_Undefined )
	{
		theButton = (LGAPushButton*) FindPaneByID ( mCancelButtonID );
		if ( theButton != nil ) 
			theButton->AddListener ( this );
	}
	
}	//	LGADialogBox::FinishCreateSelf


#pragma mark -
#pragma mark === ACCESSORS

//=====================================================================================
// �� ACCESSORS
//-------------------------------------------------------------------------------------
// LGADialogBox::SetDefaultButton
//-------------------------------------------------------------------------------------

void
LGADialogBox::SetDefaultButton ( PaneIDT	inButtonID )
{

	LGAPushButton* theButton;		
	
	// � Only make the changes if we have to
	if ( mDefaultButtonID != inButtonID )
	{
	
		// � First we need to turn off the old default button
		theButton = (LGAPushButton*) FindPaneByID ( mDefaultButtonID );
		if ( theButton != nil ) 
			theButton->SetDefaultButton ( false );		
		
		// � Set the reference to the new button
		mDefaultButtonID = inButtonID;
		
		// � Now we can setup the new outline
		theButton = (LGAPushButton*) FindPaneByID ( inButtonID );
		if ( theButton != nil ) 
		{
			theButton->AddListener ( this );
			theButton->SetDefaultButton ( true );
		}
	}
	
}	//	LGADialogBox::SetDefaultButton


//-------------------------------------------------------------------------------------
// LGADialogBox::SetCancelButton
//-------------------------------------------------------------------------------------

void
LGADialogBox::SetCancelButton ( PaneIDT	inButtonID )
{

	// � Only make the changes if we have to
	if ( mCancelButtonID != inButtonID )
	{
		mCancelButtonID = inButtonID;
		LControl	*theButton = (LControl*) FindPaneByID ( inButtonID );
		if ( theButton != nil )
			theButton->AddListener ( this );
	}
	
}	//	LGADialogBox::SetCancelButton


#pragma mark -
#pragma mark === KEY PRESS

//=====================================================================================
// �� KEY PRESS
//-------------------------------------------------------------------------------------
// LGADialogBox::HandleKeyPress
//-------------------------------------------------------------------------------------
//
//	LGADialogBox handles keyboard equivalents for hitting the Default and
//	Cancel Buttons.
//
//		Default Button: Enter, Return
//		Cancel Button:  Escape, Command-Period

Boolean
LGADialogBox::HandleKeyPress ( const EventRecord	&inKeyEvent )
{
	Boolean		keyHandled = false;
	LControl*	keyButton = nil;
	
	switch ( inKeyEvent.message & charCodeMask ) 
	{
	
		case char_Enter:
		case char_Return:
			keyButton = (LControl*) FindPaneByID ( mDefaultButtonID );
		break;
			
		case char_Escape:
			if ( (inKeyEvent.message & keyCodeMask) == vkey_Escape ) 
			{
				keyButton =  (LControl*) FindPaneByID ( mCancelButtonID );
			}
		break;

		default:
			if ( UKeyFilters::IsCmdPeriod ( inKeyEvent )) 
			{
				keyButton =  (LControl*) FindPaneByID ( mCancelButtonID );
			} else 
			{
				keyHandled = LWindow::HandleKeyPress ( inKeyEvent );
			}
		break;
	}
			
	if ( keyButton != nil ) 
	{
		keyButton->SimulateHotSpotClick ( kControlButtonPart );
		keyHandled = true;
	}
	
	return keyHandled;
	
}	//	LGADialogBox::HandleKeyPress


#pragma mark -
#pragma mark === LISTENING

//=====================================================================================
// �� LISTENING
//-------------------------------------------------------------------------------------
// LGADialogBox::ListenToMessage
//-------------------------------------------------------------------------------------

void
LGADialogBox::ListenToMessage (	MessageT	inMessage,
											void*		ioParam )
{
	if ( inMessage == cmd_Close ) 
	{
		DoClose ();
	
	} 
	else if ( inMessage < 0 ) 
	{
		// � Relay message to supercommander
		if ( GetSuperCommander () != nil ) 
		{
			SLGADialogResponse	theResponse;
			theResponse.dialogBox = this;
			theResponse.messageParam = ioParam;
			ProcessCommand ( inMessage, &theResponse );
		}
	}
	
}	//	LGADialogBox::ListenToMessage


