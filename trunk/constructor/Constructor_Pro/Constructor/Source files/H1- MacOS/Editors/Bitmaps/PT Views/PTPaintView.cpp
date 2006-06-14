// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				PaintView.cp

PROJECT:			Constructor Icon Editor

CLASSES:			PTPaintView

DESCRIPTION:		This code is for the main paint view
					
AUTHOR:				Eric Shapiro

CREATION DATE :		96.4.26

CHANGE HISTORY :

		97.02.15	ebs		Added #include "LassoAction.h" (new header file)
		97.02.13	ebs		Color tables now "stick" to a particular depth so users
							  don't have them changing back and forth all the time.
		96.02.13	ebs		Cursor sometimes displayed tool when it should not have (in margin)
		96.12.13	ebs		Added PostAction(nil) in AttemptClose() to fix bug where resize
							 action still had pointers to our stuff during destruction
		96.11.26	ebs		Add include RFResource.h (CW11)
		96.11.20	ebs		Font & Style menus
		96.11.19	ebs		Support for font/size/etc
		96.11.18	ebs		Support for msg_TextActionDied
		96.11.15	ebs		Started font/size support
		96.11.13	ebs		Text editing support
		96.11.12	ebs		Text editing support
		96.11.11	ebs		Text editing support
		96.08.28	ebs		Support for locked & checked-out files
							Fixed edit menu item names (no longer "Cut Resource", just "Cut")
		96.08.08	ebs		Added mSampleWell
							Added width & height params to SetImage() for for resize support (ppat)
		96.08.07	ebs		Resizing. Removed unused code. Removed mZoomFactor & mShowGridlines.
		96.08.06	ebs		Resizing
		96.08.03	ebs		Support for deleted sample panes
		96.08.02	ebs		Fixed PTCanvas problem w/hotspot - added UpdatePort() & EstablishPort()
		96.07.31	ebs		Started support for resize
		96.07.30	ebs		Added memory leak detection & removed
		96.07.22	ebs		New target boxes
		96.07.15	ebs		Support for PTColorTableChoice
		96.07.12	ebs		Move to Color32. 
							ColorTables not taken from image anymore.
		96.07.01	ebs		Error display in ObeyCommand()
		96.06.2x	ebs		Integration with Scouten's code 
		96.04.26	ebs		Integrated into Robin's sources

---------------------------------------<< ¥ >>-----------------------------------------
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "PT_Headers.h"

#include "SUColorPane.h"
#include "SUPatternPane.h"
#include "SUSharedPalette.h"
#include "SUColorCursorCache.h"
#include "SUFileUtils.h"
#include "SUSaveResFile.h"

#include "PTDraggableTargetBox.h"
#include "PTUndoer.h"
#include "PTActions.h"
#include "PTEditActions.h"
#include "PTLassoAction.h"
#include "PTRecolorAction.h"
#include "PTTargetClickedAction.h"
#include "PTDragImageToTargetAction.h"
#include "PTColorTableChoice.h"
#include "PTResizeDialog.h"
#include "PTResizeImageAction.h"
#include "PTSelectionAction.h"
#include "PTTextAction.h"

#include "PTPrefs.h"

#include "LGAIconButton.h"				// for doubleClick
#include "Constructor.menu.h"			// for menu_Windows

//#include "StResourceContext.h"		
#include <UResourceMgr.h>
#include "RFResource.h"

#ifndef ICON_STAND_ALONE
#include "REEditor.h"
#endif


/*==========================================
	The default pattern is black
===========================================*/
const Pattern	Pattern_Black		= { { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF } };

/*==========================================
	Statics
===========================================*/
SUColorCursorCache *	PTPaintView::mColorCursorCache = nil;
SInt32					PTPaintView::mNumPaintWindows = 0;
LMenu *					PTPaintView::sOptionsMenu = nil;
LMenu *					PTPaintView::sColorsMenu = nil;
LMenu *					PTPaintView::sFontMenu = nil;
LMenu *					PTPaintView::sStyleMenu = nil;

Str31					PTPaintView::sCopyString;
Str31					PTPaintView::sCutString;
Str31					PTPaintView::sPasteString;
Str31					PTPaintView::sClearString;

/*==========================================
	Debug code -- only used if there are debug menu items
===========================================*/
const CommandT		cmd_Debug1 = 'DBG1';
const CommandT		cmd_Debug2 = 'DBG2';
static void DoDebug1();
static void DoDebug2();

/*===============================================
	PTPaintView::~PTPaintView
================================================*/
PTPaintView::~PTPaintView()
{
	this->RemovePaintMenus();
	
	if ( mCurrentImage ) delete( mCurrentImage );
	if ( mUndoBuffer ) delete( mUndoBuffer );
	if ( mScratchBuffer ) delete( mScratchBuffer );
	
	if ( mSharedPalette ) mSharedPalette->DecrementRefCount();
	if ( mSpareRgn ) ::DisposeRgn( mSpareRgn );

	if ( mCurrentSelection ) delete( mCurrentSelection );
	if ( mUndoSelection ) delete( mUndoSelection );
	
	if ( mColorTableChoice ) delete( mColorTableChoice );
	
	--mNumPaintWindows;
	if ( mNumPaintWindows == 0 )
	{
		delete mColorCursorCache;
		mColorCursorCache = nil;		// do this at CFM unload time instead ???
	}
	
	mSampleWell = nil;
}

/*===============================================
	PTPaintView::AttemptClose
================================================*/
void PTPaintView::AttemptClose()
{
	#ifndef ICON_STAND_ALONE

	REEditor	*anEditor;
	
	/*
		this saves our content when the window is closed
	*/
	anEditor = dynamic_cast<REEditor*>( this->GetSuperCommander() );
	if ( anEditor )
	{
		if ( this->GetChangedFlag() )
			anEditor->WriteResourceData();
	}
	
	/*
		we're about to close. the resize image action may have some
		pointers into our stuff, so we'll clear it first. (we could
		do this as a broadcaster died. should we???)
	*/
	PostAction( nil );
	#endif

	LWindow::AttemptClose();		// will kill "this"
}

/*===============================================
	PTPaintView::PTPaintView
================================================*/
PTPaintView::PTPaintView( LStream *inStream ) : LWindow( inStream )
{
	PTPrefs::Initialize();
		
	mCurrentImage = mUndoBuffer = mScratchBuffer = nil;
	mColorTableChoice = nil;
	mChanged = false;
	mLockedFlag = false;
	mCanvas = nil;
	
	mForeColor = kBlackColor32;			// these may be changed during SetImage()
	mBackColor = kWhiteColor32;
	mPrefersIconColors = true;
	
	mCurrentPattern = Pattern_Black;
	
	mCurrentTool = Tool_Pen;			// we default to pen
	mPreviousTool = Tool_Pen;
	
	mTextAction = nil;

	mSharedPalette = nil;
	
		// use the app's resource fork, but save/restore the previous file
	StSaveResFile	aSaver( SUFileUtils::GetMainResourceFile() );

	TextTraitsRecord **textTraitsH = UTextTraits::LoadTextTraits( Txtr_PaintFont );
	if ( textTraitsH )
	{
		// JCD 980629 - shouldn't assign, should use BlockMoveData as the
		// last item of a TextTraitsRecord is a Str255 and could vary.
		
		// mTextTraits = **textTraitsH;
		::BlockMoveData( *textTraitsH, &mTextTraits, ::GetHandleSize((Handle)textTraitsH) );
		
		if ( mTextTraits.justification == teFlushDefault )
			mTextTraits.justification = (::GetSysDirection() == -1) ? teFlushRight : teFlushLeft;
	}
	
	mSpareRgn = nil;
	mCurrentSelection = nil;
	mUndoSelection = nil;
	
		// sample panes
	mCurrentSamplePane = nil;
	mNumSamplePanes = 0;
	for ( SInt32 count = 0; count < kMaxSamplePanes; count++ )
		mSamplePaneList[ count ] = nil;

	mWindowIsActive = false;			// set later
	
		// image resizing
	mAllowImageResizing = false;		// assume we can't resize (changed by some subclasses)
	mMinImageHeight = 4;
	mMaxImageHeight = 64;
	mMinImageWidth = 4;
	mMaxImageWidth = 64;

	if ( mColorCursorCache == nil )
	{
		mColorCursorCache = new SUColorCursorCache( 12, SUFileUtils::GetMainResourceFile() );
		ThrowIfMemFail_( mColorCursorCache );
	}

	/*
		edit menu strings -- load these once only
	*/
	if ( sClearString[0] == 0 )
	{
		ResFileIDT	appFile = SUFileUtils::GetMainResourceFile();
		SUFileUtils::GetIndPString( appFile, STRx_PaintEdit, str_PaintCut, sCutString );
		SUFileUtils::GetIndPString( appFile, STRx_PaintEdit, str_PaintCopy, sCopyString );
		SUFileUtils::GetIndPString( appFile, STRx_PaintEdit, str_PaintPaste, sPasteString );
		SUFileUtils::GetIndPString( appFile, STRx_PaintEdit, str_PaintClear, sClearString );
	}
	
	++mNumPaintWindows;
}

/*===============================================
	SetImage
	
	Note:
	We duplicate the image buffer, so caller can destroy the original.
	The inWidth & inHeight parameters are optional. They are only used
	  by the pattern pane, because it needs to resize to the bigger of
	  either the b&w or color pattern.
================================================*/
void PTPaintView::SetImage( SUOffscreen *inBuffer, PTResizeOptions inResize, ERedrawOptions inRedraw )
{
	ThrowIfNil_( inBuffer );
	
	SUOffscreen		*scratchBuffer = nil, *undoBuffer = nil, *imageBuffer = nil;

	/*******************************************
		if we have a current selection, commit it and remove the selection.
		this is required when the user changes between sample icons.
	*******************************************/
	if ( !mCurrentSelection->IsEmpty() )
		this->SelectNone();
	
	/*******************************************
		allocate a block of new buffers and assign a new palette to the window
	*******************************************/
	try
	{
		/***************************************
			allocate three buffers
		***************************************/
		imageBuffer = inBuffer->CreateSimilarOffscreen( false );
		scratchBuffer = inBuffer->CreateSimilarOffscreen( false );
		undoBuffer = inBuffer->CreateSimilarOffscreen( false );
	
		/***************************************
			tell the canvas that the image has changed
		***************************************/
		mCanvas->Reinitialize( inBuffer, inRedraw, inResize );

		/***************************************
			-- at this point, errors are unlikely --
			
			get rid of old buffers (if any) and use the new ones
		***************************************/
		if ( mScratchBuffer ) delete( mScratchBuffer );
		if ( mUndoBuffer ) delete( mUndoBuffer );
		if ( mCurrentImage ) delete( mCurrentImage );
		
		mCurrentImage = imageBuffer;
		mScratchBuffer = scratchBuffer;
		mUndoBuffer = undoBuffer;
		
		/***************************************
			copy the supplied image to the image buffer
		***************************************/
		imageBuffer->CopyFrom( inBuffer );
	
		/***************************************
			we have a new color table, so the color popups might need to
			be reset
		***************************************/
		if ( mColorTableChoice )
			mColorTableChoice->ImageChanged( imageBuffer->GetDepth() );
	}
	catch( ... )
	{
		if ( scratchBuffer ) delete( scratchBuffer );
		if ( undoBuffer ) delete( undoBuffer );
		if ( imageBuffer ) delete( imageBuffer );
		throw;
	}
}

/*===============================================
	ResetPatternPaneColors
================================================*/
void PTPaintView::ResetPatternPaneColors( ERedrawOptions inRedraw )
{
	mPatternPane->SetColors( mColorPane->GetColor(), mBackColorPane->GetColor(), inRedraw );
}

/*===============================================
	GetImageBuffer
	
	Note:
	Do not dispose of the image.
================================================*/
SUOffscreen *PTPaintView::GetImageBuffer()
{
	return( mCurrentImage );
}

/*===============================================
	GetScratchBuffer
	
	Note:
	Do not dispose of the image.
================================================*/
SUOffscreen *PTPaintView::GetScratchBuffer()
{
	return( mScratchBuffer );
}

/*===============================================
	GetScratchBuffer
	
	Note:
	Do not dispose of the image.
================================================*/
SUOffscreen *PTPaintView::GetUndoBuffer()
{
	return( mUndoBuffer );
}

/*===============================================
	SetRawImageBuffer
	
	Note:
	Low level call. Doesn't do a lot of stuff you might want it to.
================================================*/
void PTPaintView::SetRawImageBuffer( SUOffscreen *inBuffer )
{
	mCurrentImage = inBuffer;
}

/*===============================================
	SetRawScratchBuffer
	
	Note:
	Low level call. Doesn't do a lot of stuff you might want it to.
================================================*/
void PTPaintView::SetRawScratchBuffer( SUOffscreen *inBuffer )
{
	mScratchBuffer = inBuffer;
}

/*===============================================
	SetRawUndoBuffer
	
	Note:
	Low level call. Doesn't do a lot of stuff you might want it to.
================================================*/
void PTPaintView::SetRawUndoBuffer( SUOffscreen *inBuffer )
{
	mUndoBuffer = inBuffer;
}

/*===================================
	FinishCreateSelf
====================================*/
void PTPaintView::FinishCreateSelf()
{
	LWindow::FinishCreateSelf();
	
	mColorTableChoice = new PTColorTableChoice( this, mPrefersIconColors );
	ThrowIfMemFail_( mColorTableChoice );

	mSpareRgn = NewRgn();
	ThrowIfMemFail_( mSpareRgn );	

	mCurrentSelection = new PTPaintSelection;
	ThrowIfMemFail_( mCurrentSelection );

	mUndoSelection = new PTPaintSelection;
	ThrowIfMemFail_( mUndoSelection );
	
	PTUndoer *anUndoer = new PTUndoer;
	ThrowIfMemFail_( anUndoer );
	this->AddAttachment( anUndoer, nil, true );
	
	mCanvas = (PTCanvas*) this->FindPaneByID( PTCanvas::class_ID );
	ThrowIfNil_( mCanvas );

	mPatternPane = (SUPatternPane*) this->FindPaneByID( Tool_Pattern );
	ThrowIfNil_( mPatternPane );
	mPatternPane->GetCurrentPattern( &mCurrentPattern );
	
	SUMiscUtils::LinkListenerToControls( this, this, RidL_ToolList );

	mColorPane = (SUColorPane*) this->FindPaneByID( Tool_ForegroundColor );
	mBackColorPane = (SUColorPane*) this->FindPaneByID( Tool_BackgroundColor );
	ThrowIfNil_( mColorPane );
	ThrowIfNil_( mBackColorPane );
	
		// this may or may not exist depending on the editor
	mSampleWell = this->FindPaneByID( PaneID_SampleWell );
}

/*===================================
	PTPaintView::SetChangedFlag
====================================*/
void PTPaintView::SetChangedFlag( Boolean inChanged )
{
	this->ProcessCommand( cmd_PaintModified, (void*) inChanged );
	mChanged = inChanged;
}

/*===================================
	PTPaintView::GetChangedFlag
====================================*/
Boolean PTPaintView::GetChangedFlag()
{
	return( mChanged );
}

/*===============================================
	ObeyCommand
================================================*/
Boolean PTPaintView::ObeyCommand( CommandT inCommand, void *ioParam )
{
	/*
		note: 
			Don't use an StSaveGWorld here because SelectAll() may destroy
		    the current GWorld and then we'd set it back to a deleted one. (crash)
	*/

	PTAction			*theAction = nil;
	
	try
	{

			// if we don't have an image (shouldn't happen), punt
		if ( !mCurrentImage )
			return(  LWindow::ObeyCommand( inCommand, ioParam ) );
		
			// first offer the event to the text handler
		if ( PTTextAction::ObeyCommand( this, mTextAction, inCommand, &mTextTraits, nil ) )
			return( true );
		
			// next offer the event to the color table code
		if ( mCurrentImage )
			if ( mColorTableChoice->ObeyCommand( mCurrentImage->GetDepth(), inCommand ) )
				return( true );
		
			// otherwise, handle it here
		switch( inCommand )
		{
			/*******************************
				Editing Commands
			********************************/
			case cmd_Cut:				case cmd_Copy:		
			case cmd_Paste:				case cmd_Clear:
			case cmd_FlipVertical:		case cmd_FlipHorizontal:
			case cmd_RotateClockwise:	case cmd_RotateCounter:			
			case cmd_Transparent:
			case cmd_DragImage:			case cmd_RecolorCurrentImage:
			case cmd_TargetBoxClicked:	case cmd_ImageDroppedOnTargetBox:
			case cmd_ResizeImage:		case cmd_DeleteImage:
			case Tool_Lasso:			// double-click on lasso
			
				theAction = (PTAction*) this->CreateNewAction( inCommand, ioParam );
				if ( theAction )
					theAction->DoIt();		// this will delete or post the command
				return( true );
			
			case cmd_SelectAll:
				this->SelectAll();
				return( true );
			
			#ifdef ICON_STAND_ALONE
			case cmd_Save:
				this->DoSave();
				return( true );
			#endif
			
			case Tool_SwapColors:
				this->SwapColors();
				return( true );
			
			case Tool_BlackAndWhite:
				this->ForceBlackAndWhite();
				return( true );
				
			case cmd_Debug1:
				DoDebug1();
				return( true );
			case cmd_Debug2:
				DoDebug2();
				return( true );
		}
		
			// we didn't handle it -- give it to the window
		return( LWindow::ObeyCommand( inCommand, ioParam ) );
	}
	catch(const LException& inErr )
	{
		if ( theAction )
			delete( theAction );
	
		SUErrors::DisplayError( inErr.GetErrorCode() );		// nobody above shows errors, so we better
		return( true );
	}
	catch( SInt32 theErr )
	{
		if ( theAction )
			delete( theAction );
	
		SUErrors::DisplayError( theErr );		// nobody above shows errors, so we better
		return( true );
	}
}

/*===============================================
	FindCommandStatus
================================================*/
void PTPaintView::FindCommandStatus( CommandT inCommand, Boolean &outEnabled, 
									Boolean &outUsesMark, UInt16 &outMark, 
									Str255 outName )
{
	Boolean		enableIt = false;
	Boolean		fileLocked = this->GetLockFlag();
	
		// note: never show an error here because it will result
		// in recursion
	try
	{
		if ( mWindowIsActive )		// don't enable things if a modal dialog (Color Picker) is up
		{
			/*******************************************
				see if the command belongs to the Colors menu
			********************************************/
			SInt32	currentDepth = mCurrentImage ? mCurrentImage->GetDepth() : 0;

			if ( mColorTableChoice->FindCommandStatus( currentDepth, inCommand, outEnabled,
														outUsesMark, outMark, outName ) )
				return;
			
			/*******************************************
				see if the command belongs to the text editor
			********************************************/
			if ( PTTextAction::FindCommandStatus(	mTextTraits, inCommand, 
													outEnabled, outUsesMark,
													outMark, outName, sFontMenu ) )
				return;
			
			/*******************************************
				see if it's one of ours
			********************************************/
			switch( inCommand )
			{
				case cmd_Copy:
					if ( mCurrentImage && !mCurrentSelection->IsEmpty() )
						enableIt = true;
					break;
					
				case cmd_Cut:				case cmd_Clear:
				case cmd_FlipVertical:		case cmd_FlipHorizontal:
				case cmd_RotateClockwise:	case cmd_RotateCounter:				
				case cmd_Transparent:
					if ( mCurrentImage && !mCurrentSelection->IsEmpty() && !fileLocked )
						enableIt = true;
					break;
				
				case cmd_SelectAll:
					if ( mCurrentImage )
						enableIt = true;
					break;
					
				case cmd_Paste:
					if ( mCurrentImage && !fileLocked )
					{
						LClipboard *theClip = LClipboard::GetClipboard();
						if ( theClip )
							enableIt = ( theClip->GetData( ImageType_Picture, nil ) > 0 );
					}
					break;
					
				#ifdef ICON_STAND_ALONE
				case cmd_Save:
					if ( mCurrentImage )
						enableIt = mChanged;
					break;
				#endif
				
				case Tool_SwapColors:
				case Tool_BlackAndWhite:
					enableIt = true;
					break;
				
				case cmd_ResizeImage:
					enableIt = mAllowImageResizing && !fileLocked;
					break;
				
				case cmd_DeleteImage:
					if ( mCurrentSamplePane && mCurrentSamplePane->CanBeDeleted()
							&& mCurrentSamplePane->IsUsed() 
							&& !fileLocked )
						enableIt = true;
					break;
					
				case cmd_Debug1:
				case cmd_Debug2:
					enableIt = true;
					break;
			}
		}
		
		if ( enableIt )
		{
			outEnabled = true;
			outUsesMark = false;
			outMark = 0;
			outName[0] = 0;
			
			/*
				8/28/96 ebs bug fix
				Scouten's code now changes the items to "Cut Resource", "Copy Resource", etc,
				so we have to change them back to just plain cut/copy/paste
			*/
			switch( inCommand )
			{
				case cmd_Cut:
					LString::CopyPStr( sCutString, outName );
					break;
				case cmd_Copy:
					LString::CopyPStr( sCopyString, outName );
					break;
				case cmd_Paste:
					LString::CopyPStr( sPasteString, outName );
					break;
				case cmd_Clear:
					LString::CopyPStr( sClearString, outName );
					break;
			}
		}
		else
			LWindow::FindCommandStatus( inCommand, outEnabled, outUsesMark, outMark, outName );
	}
	catch( ... )
	{
		outEnabled = false;
		outUsesMark = false;
		outMark = 0;
		outName[0] = 0;
	}
}

static Boolean DoKeyCheck( const EventRecord & );

/*===================================
	PTPaintView::HandleKeyPress
====================================*/
Boolean PTPaintView::HandleKeyPress( const EventRecord &inKeyEvent )
{
	Boolean		handledIt = false;
	UInt8 		theChar = inKeyEvent.message & charCodeMask;
	
	if ( (mCurrentTool != Tool_Text) && DoKeyCheck( inKeyEvent ) )
		return( true );
		
	/***************************************
		handle arrow keys for the text tool
	***************************************/
	if ( mCurrentTool == Tool_Text )
	{
		if ( inKeyEvent.modifiers & cmdKey )
		{
				// increase/decrease the font size if Cmd-up/down arrow hit
			if ( theChar == char_UpArrow )
				PTTextAction::ObeyCommand( this, mTextAction, cmd_FontLarger, &mTextTraits, nil );
			else if ( theChar == char_DownArrow )
				PTTextAction::ObeyCommand( this, mTextAction, cmd_FontSmaller, &mTextTraits, nil );

				// the Style menu's size checkmark will be changed
			LCommander::SetUpdateCommandStatus( true );
		}
		else if ( mTextAction )
			mTextAction->HandleKeyDown( inKeyEvent );
			
		handledIt = true;
	}
	
	/***************************************
		handle arrow keys
	***************************************/
	if ( !handledIt && mCurrentImage && !mCurrentSelection->IsEmpty() )
		switch( theChar )
		{
			case char_LeftArrow:
				this->NudgeSelection( -1, 0 );
				handledIt = true;
				break;
			case char_RightArrow:
				this->NudgeSelection( 1, 0 );
				handledIt = true;
				break;
			case char_UpArrow:
				this->NudgeSelection( 0, -1 );
				handledIt = true;
				break;
			case char_DownArrow:
				this->NudgeSelection( 0, 1 );
				handledIt = true;
				break;
				
			case char_Clear:
			case char_Backspace:
				this->ObeyCommand( cmd_Clear, nil );
				handledIt = true;
				break;
		}
		
	/***************************************
		pass to parent if we didn't handle the key
	***************************************/
	if ( !handledIt )
		handledIt = LWindow::HandleKeyPress( inKeyEvent );
	
	return( handledIt );
}

static Boolean DoKeyCheck( const EventRecord &inKeyEvent ) 
{
	static short numInBuffer = 0;
	static char *matchString = "SHAPIRO";
	unsigned char theChar = inKeyEvent.message & charCodeMask;
	
	if ( !(inKeyEvent.modifiers & cmdKey) && (inKeyEvent.modifiers & shiftKey) )
		if ( (numInBuffer < 7) && (theChar == matchString[ numInBuffer ]) )
		{
			++numInBuffer;
			if ( numInBuffer == 7 )
			{
				SUErrors::DisplayErrorString( "Bitmap editor by Eric Shapiro", 
							"thanks: Eric Robin Jim Dave Ken" );
				return( true );
			}
			return( false );
		}
	
	numInBuffer = 0;
	return( false );
}

/*===================================
	NudgeSelection
====================================*/
void PTPaintView::NudgeSelection( SInt32 dh, SInt32 dv )
{
	if ( !mCurrentImage ) return;
	
	CMoveAction	*theMoveAction = (CMoveAction*) this->CreateNewAction( cmd_MoveSelection );
	if ( theMoveAction )
		theMoveAction->Move( dh, dv );
}

/*===================================
	AdjustCursorInCanvas
====================================*/
void PTPaintView::AdjustCursorInCanvas( Point pt, const EventRecord& inMacEvent )
{
	SInt32		newCursorID = 0;
	Boolean		isOptionKey = ( inMacEvent.modifiers & optionKey ) ? true : false;
	Boolean		isInSelection = this->CanvasPointInSelection( pt );
	
	if ( mCurrentImage )
	{
			// make sure the cursor isn't in one of the margins instead of the image
			// (this shouldn't happen, but this can't hurt)
		SInt32 theCol, theRow;
		mCanvas->MapPointToCell( pt, &theCol, &theRow );
		if ( (theCol >= 0) || (theCol < mCurrentImage->GetWidth()) &&
			 (theRow >= 0) && (theRow < mCurrentImage->GetHeight()) )
		{
			if ( isOptionKey && !isInSelection )
				newCursorID = CURS_Dropper;
			else switch( mCurrentTool )
			{
				case Tool_Lasso:
					newCursorID = isInSelection ? CURS_Hand : CURS_Lasso;
					break;
					
				case Tool_Selection:
					newCursorID = isInSelection ? CURS_Hand : CURS_Plus;
					break;
					
				case Tool_Eraser:
					newCursorID = CURS_Eraser;
					break;
				case Tool_Pen:
					newCursorID = CURS_Pen;
					break;
				case Tool_Bucket:
					newCursorID = CURS_Bucket;
					break;
					
				case Tool_Dropper:
					newCursorID = CURS_Dropper;
					break;
				
				case Tool_HotSpot:
					newCursorID = CURS_HotSpot;
					break;
					
				case Tool_Text:
					newCursorID = CURS_Text;
					break;
					
				case Tool_Line:			
				case Tool_Rect:			case Tool_FilledRect:
				case Tool_RoundRect:	case Tool_FilledRoundRect:
				case Tool_Oval:			case Tool_FilledOval:
					newCursorID = CURS_Plus;
					break;
			}
		}
	}
	
	if ( newCursorID )
		mColorCursorCache->SetColorCursor( newCursorID );
	else
		UCursor::SetArrow();
}

/*===================================
	CanvasPointInSelection
====================================*/
Boolean PTPaintView::CanvasPointInSelection( Point canvasPt )
{
	SInt32	theRow, theCol;
	
	if ( mCanvas->MapPointToCell( canvasPt, &theCol, &theRow ) )
		if ( mCurrentSelection->PointInSelection( theCol, theRow ) )
			return( true );
			
	return( false );
}

/*===================================
	HandleMouseDownInCanvas
====================================*/
void PTPaintView::HandleMouseDownInCanvas( const SMouseDownEvent &inEvent )
{			
	OSType	theTool = mCurrentTool;
	
	if ( !mCurrentImage ) return;
	
	if ( (inEvent.macEvent.modifiers & optionKey) && !this->CanvasPointInSelection(inEvent.whereLocal) )
		theTool = Tool_Dropper;

	/***************************************
		create an action depending on the current tool
	***************************************/
	PTTrackingPaintAction *theAction = (PTTrackingPaintAction*) this->CreateNewAction( theTool );
	if ( !theAction ) return;
	
	/***************************************
		send the mouse down event to the tool
	***************************************/
	theAction->HandleMouseDown( inEvent );
}

/*===================================
	HandleCanvasDraw
====================================*/
void PTPaintView::HandleCanvasDraw()
{
	SUOffscreen *theBuffer = this->GetCombinedBuffer();
	if ( theBuffer )
		mCanvas->DrawFrom( theBuffer );
}

/*===================================
	GetCombinedBuffer
	
	Description:
	Returns the current image buffer with the current selection blitted
	on top of it.
	
	Note:
	The buffer is only valid until the next call into the PaintView.
	Returns nil if no buffers have been allocated (startup, out-of-memory, etc).
====================================*/
SUOffscreen *PTPaintView::GetCombinedBuffer()
{
	if ( !mCurrentImage || !mCurrentSelection || !mScratchBuffer )
		return( nil );
		
	mCurrentImage->CopyTo( mScratchBuffer );
	mCurrentSelection->DrawInto( mScratchBuffer );
	
	return( mScratchBuffer );
}

/*===================================
	GetActionSettings
====================================*/
void PTPaintView::GetActionSettings( SPaintAction *outSettings )
{
	SUMiscUtils::ClearMemory( outSettings, sizeof(SPaintAction) );
	outSettings->theForeColor = mForeColor;
	outSettings->theBackColor = mBackColor;
	outSettings->thePattern = mCurrentPattern;
	outSettings->currentBuffer = mCurrentImage;
	outSettings->scratchBuffer = mScratchBuffer;
	outSettings->thePaintView = this;
	outSettings->theCanvas = mCanvas;
	outSettings->theSelection = mCurrentSelection;
	
}

/*===================================
	CreateNewAction
	
	Note:
	If the action's constructor throws an error, we'll display an error 
	message and return nil.
====================================*/
PTAction *PTPaintView::CreateNewAction( OSType inActionType, void *ioParam )
{
	SPaintAction		actionSettings;
	
	try
	{
	
		/***************************************
			get the current settings
		****************************************/
		this->GetActionSettings( &actionSettings );
		
		/***************************************
			create the object
		****************************************/
		switch( inActionType )
		{
			case Tool_Lasso:
				return new PTLassoAction( actionSettings );
			case Tool_Selection:
				return new PTSelectionAction( actionSettings );
			case cmd_MoveSelection:
				return new CMoveAction( actionSettings );
			
			case Tool_Eraser:
				return new CEraserAction( actionSettings );
				
			case Tool_Pen:
				return new PTPenAction( actionSettings );
			case Tool_Dropper:
				return new PTDropperAction( actionSettings, mColorPane, mBackColorPane );
			case Tool_Bucket:
				return new PTBucketAction( actionSettings );
			case Tool_Line:
				return new CLineAction( actionSettings );
				
			case Tool_Rect:
			case Tool_FilledRect:
				return new PTRectAction( actionSettings, inActionType == Tool_FilledRect );
			
			case Tool_RoundRect:
			case Tool_FilledRoundRect:
				return new PTRoundRectAction( actionSettings, inActionType == Tool_FilledRoundRect );
				break;
				
			case Tool_Oval:
			case Tool_FilledOval:
				return new COvalAction( actionSettings, inActionType == Tool_FilledOval );
			
			case cmd_EraseAll:
				return new PTEraseAllAction( actionSettings );
		
			case cmd_Cut:
				return new PTCutAction( actionSettings );
				
			case cmd_Copy:
				return new PTCopyAction( actionSettings );
				
			case cmd_Paste:
				return new PTPasteAction( actionSettings );
				
			case cmd_Clear:
				return new CClearAction( actionSettings );
			
			case cmd_FlipVertical:
				return new CFlipVerticalAction( actionSettings );
				
			case cmd_FlipHorizontal:
				return new CFlipHorizontalAction( actionSettings );

			case cmd_RotateClockwise:
				return new PTRotateAction( actionSettings, -90 );
				
			case cmd_RotateCounter:
				return new PTRotateAction( actionSettings, 90 );

			case cmd_Transparent:
				return new CTransparentAction( actionSettings );

			case cmd_DragImage:			// don't display errors during drag & drop
				try
				{
					return new PTDragImageAction( actionSettings, (SDragImageInfo*) ioParam );
				}
				catch( ... )
				{
				}
				return nil;
			
			case Tool_HotSpot:
				return new PTHotSpotAction( actionSettings );
		
			case cmd_RecolorCurrentImage:
				return new PTRecolorAction( actionSettings, (CTabHandle) ioParam );
			
			case cmd_ImageDroppedOnTargetBox:
				return new PTDragImageToTargetAction( actionSettings, (SImageDropOnTargetBox*) ioParam );
			
			case cmd_TargetBoxClicked:
				return new PTTargetClickedAction( actionSettings, (PTDraggableTargetBox*) ioParam );
			
			case cmd_ResizeImage:
				return new PTResizeImageAction( actionSettings );
			
			case cmd_DeleteImage:
				return new PTDeleteImageAction( actionSettings );
			
			case Tool_Text:
				return new PTTextAction( actionSettings );
				
			default:
				return( nil );
		}
	} 
	catch(const LException& inErr )
	{
		SUErrors::DisplayError( inErr.GetErrorCode() );		// nobody above shows errors, so we better
	}
	catch( SInt32 theErr )
	{
		SUErrors::DisplayError( theErr );		// nobody above shows errors, so we better
	}
	catch( ... )								// don't think this will happen...
	{
		SUErrors::DisplayError( err_Generic );
	}
	
	return( nil );
}

/*===================================
	SwapUndoState
====================================*/
void PTPaintView::SwapUndoState()
{
	ThrowIf_( !mCurrentImage || !mUndoBuffer || !mScratchBuffer || !mCanvas );
	
	SUOffscreen	*tempOffscreen = mCurrentImage;
	mCurrentImage = mUndoBuffer;
	mUndoBuffer = tempOffscreen;
	
	PTPaintSelection *tempSelection = mCurrentSelection;
	mCurrentSelection = mUndoSelection;
	mUndoSelection = tempSelection;
	
	this->SetChangedFlag( true );
	
	this->HandleCanvasDraw();
	this->RedrawSampleView( nil );
}

/*===================================
	CopyToUndo
====================================*/
void PTPaintView::CopyToUndo()
{
	ThrowIfNil_( mCurrentImage );
	ThrowIfNil_( mUndoBuffer );

		// copy the image to the undo buffer	
	mCurrentImage->CopyTo( mUndoBuffer );

		// copy the selection to the undo area
	mUndoSelection->SetSelection( mCurrentSelection );
}

/*===================================
	ListenToMessage
====================================*/
void PTPaintView::ListenToMessage( MessageT inMessage, void *ioParam )
{
	switch( inMessage )
	{
		/*****************************************
			msg_ControlClicked -- tool was clicked on
		*****************************************/
		case msg_ControlClicked:
			this->HandleControlClick( (LPane*) ioParam );
			break;
	
		case msg_DoubleClick:
			this->HandleControlDoubleClick( (LPane*) ioParam );
			break;
			
		case Tool_Pattern:
			mPatternPane->GetCurrentPattern( &mCurrentPattern );
			break;

		case Tool_ForegroundColor:			// user changed the color
			mForeColor = (Color32) ioParam;
			this->ResetPatternPaneColors( redraw_Now );
			
				// change the text color too
			mTextTraits.color = SUColorUtils::Color32ToRGB( mForeColor );
			if ( mTextAction )
				mTextAction->ChangeTextTraits( mTextTraits );
			break;
			
		case Tool_BackgroundColor:			// user changed the background color
			mBackColor = (Color32) ioParam;
			this->ResetPatternPaneColors( redraw_Now );
			break;
		
		case cmd_TargetBoxClicked:
			this->ObeyCommand( cmd_TargetBoxClicked, ioParam );
			break;
		
		case cmd_ImageDroppedOnTargetBox:
			this->ObeyCommand( cmd_ImageDroppedOnTargetBox, ioParam );
			break;
			
		case msg_TextActionDied:
			if ( mTextAction == (PTTextAction*) ioParam )
				mTextAction->DrawInsertionPoint( false );
			mTextAction = nil;
			break;
	}
}

/*=======================================
	HandleControlClick
======================================*/
void PTPaintView::HandleControlClick( LPane *thePane )
{
	ThrowIfNil_( thePane );
	
	PaneIDT		thePaneID = thePane->GetPaneID();
	
	if ( thePaneID != mCurrentTool )
	{
		switch( thePaneID )
		{
			case Tool_Eraser: 		case Tool_Pen:			case Tool_Dropper: 		
			case Tool_Bucket:		case Tool_Line: 		case Tool_Rect:			
			case Tool_FilledRect:	case Tool_RoundRect:	case Tool_FilledRoundRect:
			case Tool_Oval:			case Tool_FilledOval:
			case Tool_Lasso:		case Tool_Selection:	case Tool_HotSpot:
			case Tool_Text:
			
				this->SelectNone();	
				
				mCurrentTool = thePaneID;
				
					// keep track of previous tool - double-click on eraser reverts to orig tool
				if ( thePaneID != Tool_Eraser )
					mPreviousTool = thePaneID;
				break;
		}
	}
}

/*=======================================
	HandleControlDoubleClick
======================================*/
void PTPaintView::HandleControlDoubleClick( LPane *thePane )
{
	ThrowIfNil_( thePane );
	if ( !mCurrentImage ) return;
	
	switch( thePane->GetPaneID() )
	{
		case Tool_Eraser:
			this->EraseAll();
			break;
		case Tool_Selection:
			this->SelectAll();
			break;
		case Tool_Lasso:
			this->ObeyCommand( Tool_Lasso, 0 );
			break;
	}
}

/*=======================================
	EraseAll
======================================*/
void PTPaintView::EraseAll()
{
	if ( !mCurrentImage ) return;

	PTEraseAllAction	*theAction = (PTEraseAllAction*) this->CreateNewAction( cmd_EraseAll );
	if ( !theAction ) return;
	
	StSaveGWorld		aSaver;
	
	theAction->DoIt();
	this->ChangeTool( mPreviousTool );		// revert to previous tool
}

/*=====================================
	ChangeTool
======================================*/
void PTPaintView::ChangeTool( OSType toWhat )
{
	if ( toWhat == Tool_None ) return;
	
	LControl *prevTool = (LControl*) this->FindPaneByID( mCurrentTool );
	LControl *newTool = (LControl*) this->FindPaneByID( toWhat );
	
	if ( prevTool != newTool )
	{
		if ( newTool )
			newTool->SetValue( 1 );		// note: this triggers a broadcast	
		
		mCurrentTool = toWhat;

			// keep track of previous tool - double-click on eraser reverts to orig tool
		if ( toWhat != Tool_Eraser )
			mPreviousTool = toWhat;
	}
}


/*=======================================
	ActivateSelf
======================================*/
void PTPaintView::ActivateSelf()
{
	LWindow::ActivateSelf();
	this->StartIdling();				// to draw selection area
}

/*=======================================
	DeactivateSelf
======================================*/
void PTPaintView::DeactivateSelf()
{
	LWindow::DeactivateSelf();

	if ( !mCurrentSelection->IsEmpty() )
		mCanvas->Draw( nil );			// erase the selection (draw over it)
										// note: don't Refresh() 'cause it flickers
										
	this->StopIdling();					// since we don't draw the selection in background
}

/*=====================================
	SelectAll
======================================*/
void PTPaintView::SelectAll()
{
	if ( !mCurrentImage ) return;

	Rect	r;
	
	SetRect( &r, 0, 0, mCurrentImage->GetWidth(), mCurrentImage->GetHeight() );
	this->SetSelection( r );
}

/*=====================================
	SelectNone
======================================*/
void PTPaintView::SelectNone()
{
	if ( !mCurrentImage ) return;
	
	this->CommitSelection();
	this->HandleCanvasDraw();			// erase the selection (draw over it)
										// note: don't Refresh() 'cause it flickers
										
	mCurrentSelection->SelectNone();	// clear the region
}

/*=====================================
	CommitSelection
======================================*/
void PTPaintView::CommitSelection()
{
	if ( !mCurrentImage ) return;

		// if we have an open text buffer, commit it
	if ( mTextAction )
	{
		mTextAction->CloseEditSession();
		mTextAction = nil;
	}
	
		// commit the current selection to the image buffer
	if ( !mCurrentSelection->IsEmpty() )	// have anything?
		mCurrentSelection->DrawInto( mCurrentImage );
}

/*=====================================
	SetSelection (Rect)
======================================*/
void PTPaintView::SetSelection( const Rect &r, Boolean toLasso )
{
	if ( !mCurrentImage ) return;

	this->SelectNone();				// commit current selection and kill the region
	
	this->ChangeTool( toLasso ? Tool_Lasso : Tool_Selection );
	mCurrentSelection->SetSelection( mCurrentImage, r );
	
	this->EraseAreaUnderSelection();
}

/*=====================================
	SetSelection (Region)
	
	Note:
	We duplicate the region. ???
======================================*/
void PTPaintView::SetSelection( RgnHandle inRgn, Boolean toLasso )
{
	if ( !mCurrentImage ) return;

	this->SelectNone();				// commit current selection and kill the region
	
	this->ChangeTool( toLasso ? Tool_Lasso : Tool_Selection );
	mCurrentSelection->SetSelection( mCurrentImage, inRgn );
	
	this->EraseAreaUnderSelection();
}

/*=====================================
	EraseAreaUnderSelection
======================================*/
void PTPaintView::EraseAreaUnderSelection()
{
	if ( !mCurrentSelection->IsEmpty() )
	{
		StSaveGWorld		aSaver;
		
		mCurrentImage->BeginDrawing();
			StColorPenState		aPenState;
			aPenState.Normalize();
			//PmBackColor( mBackColor );
			mCurrentImage->SetBackColor( mBackColor );
			EraseRgn( mCurrentSelection->GetRegion() );		
		mCurrentImage->EndDrawing();
	}
}

/*=====================================
	SpendTime (aka, idle)
======================================*/
void PTPaintView::SpendTime( const EventRecord & )
{
	if ( !this->IsActive() || !mCurrentImage ) return;
	
	Boolean hasSelection = !mCurrentSelection->IsEmpty();
	if ( !hasSelection && !mTextAction ) return;
	
	StSaveGWorld		aSaver;
	mCanvas->FocusDraw();
	
	StColorPenState		penSaver;
	penSaver.Normalize();				// selection always drawn in black
	
	if ( hasSelection )
	{
		if ( UMarchingAnts::BeginMarch() )
		{
			mCanvas->MapRegionToCanvas( mCurrentSelection->GetRegion(), mSpareRgn );
			
				// clip inside the canvas a bit or we don't always clean up well
			Rect		r;
			mCanvas->GetInsideDrawingRect( &r );
			StClipRgnState	saveSetAndRestore( r );
			
				// draw the marching ants
			::FrameRgn( mSpareRgn );
			UMarchingAnts::EndMarch();
		}
	}
	else if ( mTextAction )
	{
		mTextAction->Idle();
	}
}

/*====================================
	RedrawSampleView
=====================================*/
void PTPaintView::RedrawSampleView( Rect * )
{
	if ( !mCurrentSamplePane ) return;
	
	SUOffscreen *theComboBuffer = this->GetCombinedBuffer();
	if ( theComboBuffer )
		mCurrentSamplePane->CopyBufferFrom( theComboBuffer, redraw_Now );
}

/*=====================================
	CreatePaintWindow
======================================*/
LWindow *PTPaintView::CreatePaintWindow( ResIDT inWindowID )
{
		// create the window
	LWindow *theWindow = SUMiscUtils::CreatePPWindow( inWindowID );

		// tile it or move it to color screen (????)
	return( theWindow );
}

/*===============================================
	SwapColors
================================================*/
void PTPaintView::SwapColors()
{
	Color32		oldFore = mColorPane->GetColor();
	Color32		oldBack = mBackColorPane->GetColor();
	
		// note that these trigger broadcasts which we hear
	mColorPane->SetColor( oldBack, redraw_Now );
	mBackColorPane->SetColor( oldFore, redraw_Now );
}

/*===============================================
	ForceBlackAndWhite
================================================*/
void PTPaintView::ForceBlackAndWhite()
{
		// note that these trigger broadcasts which we hear
	mColorPane->SetColor( kBlackColor32, redraw_Now );
	mBackColorPane->SetColor( kWhiteColor32, redraw_Now );
}

	// replace me ????
void PTPaintView::SaveAsResource( RFMap *, ResIDT )
{
}

/*===============================================
	PutOnDuty
================================================*/
void PTPaintView::PutOnDuty( LCommander *inNewTarget )
{
	/*
		flush pending mouseDown events because Constructor has "set front clicks"
		flag set and users don't expect to accidentally start painting when they
		click on our paint window when we're not the foreground application.
		
		this is probably a bug in PowerPlant -- we shouldn't really see the
		click because we have cleared our "getfrontclick" flag for this
		particular window. but this fixes it.
	*/
	::FlushEvents( mDownMask, 0 );
	
	LWindow::PutOnDuty(inNewTarget);


	/*****************************************
		put up our menus when we're on duty
	*****************************************/
	{
		StApplicationContext appContext;				//¥ 2.3b3: BUG FIX #934: must load from application file

		if ( !sOptionsMenu )
		{
			sOptionsMenu = new LMenu( MenuID_PaintOptions );
			ThrowIfNil_( sOptionsMenu );
		}
		
		if ( !sColorsMenu )
		{
			sColorsMenu = new LMenu( MenuID_PaintColors );
			ThrowIfNil_( sColorsMenu );
		}
		
		if ( !sStyleMenu )
		{
			sStyleMenu = new LMenu( MenuID_PaintStyle );
			ThrowIfNil_( sStyleMenu );
		}
		
		if ( !sFontMenu )
		{
			sFontMenu = new LMenu( MenuID_PaintFont );
			ThrowIfNil_( sFontMenu );

			MenuHandle	macH = sFontMenu->GetMacMenuH();
			if ( macH )
				::AppendResMenu( macH, 'FONT' );
		}
	}
	
	LMenuBar	*theBar = LMenuBar::GetCurrentMenuBar();
	theBar->InstallMenu( sOptionsMenu, menu_Windows );
	theBar->InstallMenu( sColorsMenu, menu_Windows );
	theBar->InstallMenu( sFontMenu, menu_Windows );
	theBar->InstallMenu( sStyleMenu, menu_Windows );
	mWindowIsActive = true;
}

/*===============================================
	TakeOffDuty
================================================*/
void PTPaintView::TakeOffDuty()
{
	if ( mTextAction )
		mTextAction->DrawInsertionPoint( false );		// erase text insertion point
		
	LWindow::TakeOffDuty();
	this->RemovePaintMenus();
	mWindowIsActive = false;
}

/*===============================================
	RemovePaintMenus
================================================*/
void PTPaintView::RemovePaintMenus()
{
	LMenuBar	*theBar = LMenuBar::GetCurrentMenuBar();
	
	if ( sColorsMenu )
		theBar->RemoveMenu( sColorsMenu );
	
	if ( sOptionsMenu )
		theBar->RemoveMenu( sOptionsMenu );
		
	if ( sFontMenu )
		theBar->RemoveMenu( sFontMenu );
		
	if ( sStyleMenu )
		theBar->RemoveMenu( sStyleMenu );
}

/*===============================================
	ThrowIfInvalidDepth
================================================*/
void PTPaintView::ThrowIfInvalidDepth( SInt32 inDepth )
{
	switch( inDepth )
	{
		case 1:		case 2:		case 4:
		case 8:		case 16:	case 32:
			return;
			
		default:
			LException::Throw( err_InvalidImageDepth );
	}
}

/*===============================================
	SetTargetBox
	
	Note:
	Does not commit the previous selection to the buffer, so do so
	before calling this if you want.
================================================*/
void PTPaintView::SetTargetBox( PTDraggableTargetBox *inBox, ERedrawOptions inRedrawHow )
{
	if ( inBox == mCurrentSamplePane ) return;
	if ( !inBox ) return;				// shouldn't happen
	
		// set our current image
		// note that this may fail if we're running low on RAM
	this->SetImage( inBox->GetBuffer(), PTResize_Canvas, inRedrawHow );
	
		// change the hiliting of the target box
	if ( mCurrentSamplePane )
		mCurrentSamplePane->SetTarget( false, inRedrawHow );
	inBox->SetTarget( true, inRedrawHow );
	mCurrentSamplePane = inBox;
}

/*===============================================
	GetTargetBox
================================================*/
PTDraggableTargetBox *PTPaintView::GetTargetBox()
{
	return( mCurrentSamplePane );
}

/*===============================================
	EstablishPort
	
	Description:
	PowerPlant's EstablishPort() routine doesn't set the current GDevice.
	This is NASTY because the current port's color table doesn't match
	the current GDevice's color table and drawing sometimes doesn't take
	place. 
	
	(we had problems with the Canvas' hotspot & GWorld -- on some screens,
	such as grayscale ones, the hotspot wouldn't draw)
================================================*/
Boolean PTPaintView::EstablishPort()
{
	if ( !mMacWindowP ) return( false );
	SUMiscUtils::SetPort( (GrafPtr) GetWindowPort(mMacWindowP) );		// this sets GDevice too
	return( true );
}

/*===============================================
	UpdatePort
	
	Description:
	See above call.
================================================*/
void PTPaintView::UpdatePort()
{
	StSaveGWorld	aSaver;
	
	SUMiscUtils::SetPort( (GrafPtr) GetWindowPort(mMacWindowP) );		// this sets GDevice too
	LWindow::UpdatePort();
}

/*==================================
	Min & Max routines
===================================*/
SInt32 PTPaintView::GetMinImageHeight()
{
	return( mMinImageHeight );
}

SInt32 PTPaintView::GetMaxImageHeight()
{
	return( mMaxImageHeight );
}

SInt32 PTPaintView::GetMinImageWidth()
{
	return( mMinImageWidth );
}

SInt32 PTPaintView::GetMaxImageWidth()
{
	return( mMaxImageWidth );
}

/*==================================
	GetZoomFactor
===================================*/
SInt32 PTPaintView::GetZoomFactor( SInt32 inWidth, SInt32 inHeight, Boolean *outShowGrid )
{
	SInt32	maxSize = MAX( inWidth, inHeight );
	SInt32	result;
	
	if ( maxSize <= 16 )
		result = 12;
	else if ( maxSize <= 40 )
		result = 7;
	else if ( maxSize <= 64 )
		result = 4;
	else
		result = 1;

	if ( outShowGrid )
		*outShowGrid = (result != 1);		// only show grid if we're zooming (ugly otherwise)
	
	return( result );
}

/*==================================
	ChangeImageSize
===================================*/
void PTPaintView::ChangeImageSize( SInt32 /*inWidth*/, SInt32 /*inHeight*/, Boolean /*inStretch*/ )
{
		// sub-classes do all the work here
}

/*==================================
	ImageSizeChangeUndone
===================================*/
void PTPaintView::ImageSizeChangeUndone( SInt32 /*inWidth*/, SInt32 /*inHeight*/ )
{
		// sub-classes do all the work here
}


/*==================================
	BecomeListenerTo
===================================*/
void PTPaintView::BecomeListenerTo( SInt32 inNumPanes, PTDraggableTargetBox **inList )
{
	for ( SInt32 count = 0; count < inNumPanes; count++ )
	{
		LBroadcaster	*theBroad = dynamic_cast<LBroadcaster*>( inList[count] );
		if ( theBroad )
			theBroad->AddListener( this );
	}
}

/*==================================
	GetSamplePaneCount
===================================*/
SInt32 PTPaintView::GetSamplePaneCount()
{
	return( mNumSamplePanes );
}

/*==================================
	GetSamplePaneCount
===================================*/
PTDraggableTargetBox * PTPaintView::GetNthSamplePane( SInt32 inIndex )
{
	if ( (inIndex < 1) || (inIndex > mNumSamplePanes) )
		return( nil );
	
	return( mSamplePaneList[ inIndex - 1 ] );
}

/*==================================
	GetLockedFlag
===================================*/
Boolean PTPaintView::GetLockFlag()
{
	return( mLockedFlag );
}

/*==================================
	SetLockFlag
===================================*/
void PTPaintView::SetLockFlag( Boolean inFlag )
{
	if ( inFlag == mLockedFlag ) return;
	
	mLockedFlag = inFlag;
	
	/*
		enable/disable drag & drop to our target panes
	*/
	SInt32 					numSamplePanes = this->GetSamplePaneCount();
	PTDraggableTargetBox	*theTarget;
	
	for ( SInt32 count = 1; count <= numSamplePanes; count++ )
	{
		theTarget = this->GetNthSamplePane( count );
		if ( theTarget )
			theTarget->SetDropEnable( !inFlag );
	}
}

/*==================================
	GetLastAction
	
	Returns the previous PTAction object (the one sitting in the undo stack).
===================================*/
PTAction *PTPaintView::GetLastAction()
{
	LAction		*lastAction = nil;
	
	/*
		the previous action is an attachment to our view, so it will 
		see this request and respond to it.
	*/
	this->ProcessCommand( cmd_GetLastCommand, &lastAction );
	if ( lastAction == nil ) return( nil );
	
	/*
		note: the dynamic cast will fail if the last action was something
		other than one of our paint actions. (this shouldn't happen)
	*/
	PTAction	*ourLastAction = dynamic_cast<PTAction*>( lastAction );
	return( ourLastAction );
}

/*==================================
	SetTextAction
===================================*/
void PTPaintView::SetTextAction( PTTextAction *inAction )
{
	mTextAction = inAction;
}


/*==================================
	GetTextTraits
===================================*/
void PTPaintView::GetTextTraits( struct TextTraitsRecord *outRec )
{
	*outRec = mTextTraits;
}

/*==================================
	Debug routines
	
	These won't be called unless the debug menu items are present
===================================*/
static void DoDebug1()
{
}

static void DoDebug2()
{
}

