// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*
		97.02.13	ebs		Color tables now "stick" to a particular depth so users
							  don't have them changing back and forth all the time.
		96.12.13	ebs		Tried some color table changes, then removed
		96.12.09	ebs		Added DefaultToFullColorTable() for Robin
		96.08.28	ebs		Support for locked files
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "PT_Headers.h"
#include "PTColorTableChoice.h"
#include "SUSaveResFile.h"
#include "SUFileUtils.h"
#include "SUColorPane.h"
#include "PTPrefs.h"

/*
	Notes:
	
	¥ 	Most of the color resource types are edited using 32-bit buffers.
		This means that they support all of the various color table choices.
		
	¥	B&W buffers (including masks) don't support any choices (b&w only).
	
	¥	Icon families are the only place where the editor uses 4 & 8-bit
		buffers. Things are enabled as follows for them:
		  icl8, ics8, icm8	-> Apple 256 table or Apple Icon Colors
		  icl4, ics4, icm4	-> Apple 16 table
		  b&w				-> No tables
	
*/

/*===================================
	Color Menu Command IDs
====================================*/
const CommandT		cmd_ColorTableAppleIcon		=	'CLT1';
const CommandT		cmd_ColorTableApple256		=	'CLT2';
const CommandT		cmd_ColorTableApple256Gray	=	'CLT3';
const CommandT		cmd_ColorTableApple16		=	'CLT4';
const CommandT		cmd_ColorTableApple16Gray	=	'CLT5';
const CommandT		cmd_ColorTableApple4Gray	=	'CLT6';
const CommandT		cmd_ColorTablePicker		=	'CLT7';

	// this isn't in the menu, but is used below
const CommandT		cmd_ColorTableApple2		=	'CLT8';

	// this is a phony id used to indicate "the color picker"
const ResIDT		ColorTable_UsePicker		=	0;



/*===================================
	Constructor
====================================*/
PTColorTableChoice::PTColorTableChoice( PTPaintView *inPaintView, Boolean inUsesIconColors )
{
	mPaintView = inPaintView;
	mCurrentTable = nil;
	this->InitTableCommands( inUsesIconColors );
}

/*===================================
	Destructor
====================================*/
PTColorTableChoice::~PTColorTableChoice()
{
	this->DisposeCurrentTable();
}

#ifdef NOT_USED_DONE_ANOTHER_WAY
void PTColorTableChoice::SetPrefersIconColors( Boolean inPrefersIconColors )
{
	mPrefersIconColors = true;
}
#endif

/*===================================
	GetCurrentTable
====================================*/
CTabHandle PTColorTableChoice::GetCurrentTable()
{
	return( mCurrentTable );
}


/*===================================
	FindCommandStatus
====================================*/
Boolean PTColorTableChoice::FindCommandStatus( SInt32 inDepth, CommandT inCommand, 
										Boolean &outEnabled, Boolean &outUsesMark, 
										UInt16 &outMark, Str255  )
{
	Boolean		handled = false;
	Boolean		isLocked = mPaintView->GetLockFlag();
	
	outEnabled = false;
	
	switch( inCommand )
	{
		case cmd_ColorTableAppleIcon:
			outEnabled = (inDepth >= 8) && !isLocked;
			handled = true;
			break;	
		case cmd_ColorTableApple256:
			outEnabled = (inDepth >= 8) && !isLocked;;
			handled = true;
			break;	
		case cmd_ColorTableApple16:
			outEnabled = (inDepth >= 16) && !isLocked;;
			handled = true;
			break;	
		case cmd_ColorTableApple256Gray:
			outEnabled = (inDepth >= 16) && !isLocked;;
			handled = true;
			break;	
		case cmd_ColorTableApple16Gray:
			outEnabled = (inDepth >= 16) && !isLocked;;
			handled = true;
			break;	
		case cmd_ColorTableApple4Gray:
			outEnabled = (inDepth >= 16) && !isLocked;;
			handled = true;
			break;	
		case cmd_ColorTablePicker:
			outEnabled = (inDepth >= 16) && !isLocked;;
			handled = true;
			break;	

		case cmd_RecolorCurrentImage:
			Boolean usingPicker = (mTableCommands[inDepth] == cmd_ColorTablePicker);
			outEnabled = (inDepth >=16) && !usingPicker && !isLocked;
			handled = true;
			break;	
	}
	
	if ( handled )
	{
		if ( mTableCommands[inDepth] == inCommand )
		{
			outUsesMark = true;
			outMark = checkMark;
		}
		else
		{
				// note: we have to set outUsesMark to true or the checkmarks never
				// go away
			outUsesMark = true;
			outMark = 0;
		}
	}
	
	return( handled );
}

/*===================================
	ObeyCommand
====================================*/
Boolean PTColorTableChoice::ObeyCommand( SInt32 inDepth, CommandT inCommand )
{
	Boolean		didIt = false;
	ResIDT		theID;
	
	/*
		watch out for recursion when we get the recolor command
	*/
	static Boolean busy = false;
	if ( busy ) return( false );
	
	try
	{
		busy = true;

		if ( inCommand == cmd_RecolorCurrentImage )
		{
			if ( mCurrentTable )
				mPaintView->ObeyCommand( cmd_RecolorCurrentImage, mCurrentTable );
			didIt = true;
		}
		else
		{
			if ( this->MapCommandToTableID( inCommand, &theID ) )
			{
				this->ChangePopupColorTables( theID );
				mTableCommands[ inDepth ] = inCommand;
				didIt = true;
			}
		}
	}
	catch(...)
	{
		busy = false;
		throw;
	}
	
	busy = false;	
	return( didIt );
}

/*===================================
	MapCommandToTableID
====================================*/
Boolean PTColorTableChoice::MapCommandToTableID( CommandT inCommand, ResIDT *outResID )
{
	switch( inCommand )
	{
		case cmd_ColorTableAppleIcon:
			*outResID = ColorTable_IconColors;
			return( true );
			
		case cmd_ColorTableApple256:
			*outResID = 8;
			return( true );
			
		case cmd_ColorTableApple16:
			*outResID = 4;
			return( true );
			
		case cmd_ColorTableApple256Gray:
			*outResID = 40;
			return( true );
			
		case cmd_ColorTableApple16Gray:
			*outResID = 36;
			return( true );
			
		case cmd_ColorTableApple4Gray:
			*outResID = 34;
			return( true );
			
		case cmd_ColorTablePicker:
			*outResID = ColorTable_UsePicker;
			return( true );
			
		case cmd_ColorTableApple2:
			*outResID = 1;
			return( true );
	}
	
	return( false );
}

/*===================================
	ChangePopupColorTables
====================================*/
void PTColorTableChoice::ChangePopupColorTables( ResIDT inResID )
{
	StSaveResFile	aSaver;
	
	this->DisposeCurrentTable();

	if ( inResID == ColorTable_UsePicker )
		mCurrentTable = nil;
	else
		mCurrentTable = SUColorUtils::GetColorTable( inResID );

	/*
		change the color table popups.
		
		if this results in a change of the current color, they will
		broadcast a message to the paint view which will also redraw
		the pattern popup.
	*/
	SUColorPane		*forePane = (SUColorPane*) mPaintView->FindPaneByID( Tool_ForegroundColor );
	SUColorPane		*backPane = (SUColorPane*) mPaintView->FindPaneByID( Tool_BackgroundColor );
	
	if ( forePane )
		forePane->SetColorTable( mCurrentTable, true, redraw_Now );

	if ( backPane )
		backPane->SetColorTable( mCurrentTable, true, redraw_Now );

	LCommander::SetUpdateCommandStatus(true);
}

/*===================================
	ImageChanged
	
	Description:
	Whenever the displayed image is changed, we may have to change
	the color table to a more suitable one.
====================================*/
void PTColorTableChoice::ImageChanged( SInt32 inNewDepth )
{
	this->ObeyCommand( inNewDepth, mTableCommands[ inNewDepth ] );
	LCommander::SetUpdateCommandStatus(true);
}

/*===================================
	DisposeCurrentTable
====================================*/
void PTColorTableChoice::DisposeCurrentTable()
{
	if ( mCurrentTable )
		::DisposeCTable( mCurrentTable );

	mCurrentTable = nil;
}

/*===================================
	InitTableCommands
====================================*/
void PTColorTableChoice::InitTableCommands( Boolean inIcon )
{
	CommandT	preferredCmd;
	
	SUMiscUtils::ClearMemory( mTableCommands, sizeof( mTableCommands ) );	// ???
	
	/*
		icons default to Apple's preferred icon colors
	*/
	if ( inIcon && !PTPrefs::DefaultToFullColorTable() )
		preferredCmd = cmd_ColorTableAppleIcon;
	else
		preferredCmd = cmd_ColorTableApple256;

	mTableCommands[ 1 ] 	=	cmd_ColorTableApple2;
	mTableCommands[ 2 ] 	=	cmd_ColorTableApple4Gray;
	mTableCommands[ 4 ] 	=	cmd_ColorTableApple16;
	mTableCommands[ 8 ] 	=	preferredCmd;
	mTableCommands[ 16 ] 	=	preferredCmd;
	mTableCommands[ 24 ]	= 	preferredCmd;		// not used, but can't hurt
	mTableCommands[ 32 ] 	=	preferredCmd;
}

