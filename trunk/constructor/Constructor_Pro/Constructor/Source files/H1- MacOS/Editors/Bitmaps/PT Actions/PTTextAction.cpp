// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*
		97.02.15	ebs		Text baseline is now where the user clicks (not centered vertically)
		96.11.21	ebs		Misc fixes (caret Hook on 68k, undo bug, center justify, white text)
		96.11.20	ebs		Font/style support
		96.11.19	ebs		Fixed bug when user clicks near top of the window
		96.11.18	ebs		Support for color, optimize speed on larger images
							Added msg_TextActionDied broadcast
		96.11.15	ebs		Use TextEdit in order to handle "funny" scripts like Arabic
		96.11.13	ebs		Text editing support stuff
		96.11.12	ebs		Text editing support added to paint view
*/

/*
	Methodology Note:
	 
	We draw the text into a 1-bit gworld that has a TextEdit record attached
	to it. We then blit and colorize this gworld to the image buffer and then
	to the canvas. TextEdit provides support for languages like Arabic where
	each new character can affect the look of previous characters. Don't try
	this by hand unless you have a month to kill.
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "PTTextAction.h"
#include "SUSparePort.h"

#define TEXT_BASELINE_FIX

#if TARGET_RT_MAC_CFM || __MACH__
	static pascal void MyCaretHook( const Rect *, TEPtr );		// CFM version
#else
#ifdef WINVER
	S_PASCAL(void) MyCaretHook( const Rect *, TEPtr );			// MSVC compatible
#else
	static asm void MyCaretHook( void );						// 68K version
#endif
#endif

static CaretHookUPP		sCaretUPP = nil;

/*=================================
	Constructor
===================================*/
PTTextAction::PTTextAction( const SPaintAction &inAction ) : 
	PTTrackingPaintAction( inAction, str_UndoText )
{
	this->ThrowIfFileLocked();
	
	if ( !sCaretUPP )
	{
#if PP_Target_Carbon
		sCaretUPP = NewCaretHookUPP( MyCaretHook );
#else
		sCaretUPP = NewCaretHookProc( MyCaretHook );
#endif
		ThrowIfMemFail_( sCaretUPP );
	}
	
	mText = nil;
	mTextBuffer = nil;
	mImageBuffer = nil;
	mUndoBuffer = nil;
	mImageHeight = 0;
	mImageWidth = 0;
	
	mDrawnInsertionPoint = false;
	mNextInsertionTime = 0;
	
		// the paint view needs to know when we go away (listens for msg_TextActionDied)
	this->AddListener( mSettings.thePaintView );
}

/*=================================
	Destructor
===================================*/
PTTextAction::~PTTextAction()
{
	/*
		Let the PaintView know we died -- it won't know in the case of
		  Constructor-wide commands like Save.
		  
		Notes: 
		  We can't rely on the generic "broadcastDied" message because
		    by the time it is sent, the object is no longer a text action and
		    the paintview won't recognize it. (C++ is great, eh?).
		    
		  This may cause the paintview to call us to erase the insertion point.
		    This isn't a very good C++ practice (we can't be subclassed completely).
	*/
	this->BroadcastMessage( msg_TextActionDied, this );
	
	if ( mText ) 
	{
		::TEDispose( mText );
		mText = nil;
	}
	
	if ( mTextBuffer )
	{
		delete mTextBuffer;
		mTextBuffer = nil;
	}
}

/*=================================
	UndoSelf
===================================*/
void PTTextAction::UndoSelf()
{
	this->CloseEditSession();
	PTTrackingPaintAction::UndoSelf();
}

/*=================================
	HandleMouseDown
===================================*/
void PTTextAction::HandleMouseDown( const SMouseDownEvent &inEvent )
{
	StSaveGWorld	aWorldSaver;
	Rect			destR, sourceR;
	PTPaintView		*thePaintView = mSettings.thePaintView;
	GrafPtr			aSparePort = SUSparePort::GetSparePort();
	SInt32			initialH, initialV;
	
	/********************************************
		get the initial cell hit by the user
	********************************************/
	if ( !mSettings.theCanvas->MapPointToCell( inEvent.whereLocal, &initialH, &initialV ) )
		return;								// missed it by *that* much
	
	/********************************************
		save the image & undo buffers 'cause we need them just about everywhere
	********************************************/
	mImageBuffer = mSettings.thePaintView->GetImageBuffer();
	mUndoBuffer = mSettings.thePaintView->GetUndoBuffer();
	SInt32	mImageWidth = mImageBuffer->GetWidth();
	SInt32	mImageHeight = mImageBuffer->GetHeight();

	/********************************************
		get the text traits from the paint view and then the font information
	********************************************/
	thePaintView->GetTextTraits( &mTextTraits );
	SUMiscUtils::SetPort( aSparePort );
	UTextTraits::SetPortTextTraits( &mTextTraits );
	::GetFontInfo( &mFontInfo );
	
	/********************************************
		find the rectangle we are drawing in relative to the paint image buffer. 
	********************************************/
	#ifdef TEXT_BASELINE_FIX
	mTextTop = MAX( 0, initialV - mFontInfo.ascent + 1 );	// no negatives allowed
	#else
	mTextTop = MAX( 0, initialV - mFontInfo.ascent/2 );		// no negatives allowed
	#endif
	
	switch( mTextTraits.justification )
	{
		case teForceLeft:
		case teFlushDefault:				// shouldn't happen (paint view changes this)
			::SetRect( &destR, initialH, mTextTop, mImageWidth, mImageHeight );
			break;
		case teJustRight:
			::SetRect( &destR, 0, mTextTop, initialH, mImageHeight );
			break;
		case teJustCenter:
			SInt32 centerWidth = MIN( initialH, mImageWidth - initialH );
			::SetRect( &destR, initialH - centerWidth, mTextTop, initialH + centerWidth, mImageHeight );
			break;
	}
	
	short textAreaWidth = destR.right - destR.left;
	short textAreaHeight = destR.bottom - destR.top;
	
	/********************************************
		find the rectangle we are drawing in relative to text buffer
	********************************************/
	::SetRect( &sourceR, 0, 0, textAreaWidth, textAreaHeight );
	mTextLeft = destR.left;
	
	/********************************************
		allocate a 1-bit buffer the same size as the text area
	********************************************/
	mTextBuffer = SUOffscreen::CreateBuffer( textAreaWidth, textAreaHeight, 1 );

	/********************************************
		draw into our offscreen in black always, since it's only 1-bit.
		(we'll colorize the text when we blit it to the image buffer later on)
	********************************************/
	TextTraitsRecord copyOfTraits = mTextTraits;
	copyOfTraits.color = kBlackColor;
	
	/********************************************
		create the text edit record
	********************************************/
	mTextBuffer->BeginDrawing();
		
		UTextTraits::SetPortTextTraits( &copyOfTraits );
		mText = ::TENew( &sourceR, &sourceR );
		ThrowIfMemFail_( mText );

			// this will set the alignment (not part of the port text style)		
		UTextTraits::SetTETextTraits( &copyOfTraits, mText );
		
			// and we draw the caret ourself
		(**mText).caretHook = sCaretUPP;

	mTextBuffer->EndDrawing();
	
	/********************************************
		we're undoable, so copy the current image to the undo buffer
	********************************************/
	mSettings.thePaintView->CopyToUndo();

	/********************************************
		Post us to the undo queue and let the paintview know we're active.
		The order of this is important.
	********************************************/
	this->PostAsAction();	
	mSettings.thePaintView->SetTextAction( this );
}

/*=================================
	HandleKeyDown
===================================*/
void PTTextAction::HandleKeyDown( const EventRecord &inEvent )
{
	StSaveGWorld		aSaver;
	Boolean				isArrowKey = false;
	Rect				rBefore, rAfter;
	
	if ( !mText || !mTextBuffer ) return;
	
	UInt8 theChar = inEvent.message & charCodeMask;

	/**************************************************
		erase the old insertion point or we get junk on the screen (esp left arrow key)
	**************************************************/
	this->DrawInsertionPoint( false );
	
	/**************************************************
		if an arrow key was hit, be sure to draw the insertion
		point at the bottom of this function or user won't be able 
		to see it until they release the key. 
	**************************************************/
	if ( (theChar == char_LeftArrow) || (theChar == char_RightArrow)
			|| (theChar == char_UpArrow) || (theChar == char_DownArrow) )
		isArrowKey = true;
	
	/**************************************************
		get the size of the area we've already drawn into.
		this is needed for the backspace key when the user
		deletes a CR.
	**************************************************/
	this->GetRectRelativeToImageBuffer( &rBefore );
	
	/**************************************************
		restore the main buffer to virgin status
	**************************************************/
	this->RestoreMainBufferFromUndoBuffer();
	
	/**************************************************
		tell TextEdit to do its thing
	**************************************************/
	mTextBuffer->BeginDrawing();
		::TEKey( theChar, mText );
// JWW - OS X requires an update call or else it won't draw while typing
#if PP_Target_Carbon
		Rect bigR = { -1000, -1000, 3000, 3000 };
		::TEUpdate( &bigR, mText );
#endif
	mTextBuffer->EndDrawing();
	
	/**************************************************
		copy the text buffer back to the main image buffer
	**************************************************/
	this->CopyTextBufferToMainBuffer();

	/**************************************************
		and update the portion of the canvas/screen that needs it.
		this may be one extra line of text if the user deleted a CR.
	**************************************************/
	this->GetRectRelativeToImageBuffer( &rAfter );
	rAfter.bottom = MAX( rAfter.bottom, rBefore.bottom );
	this->CopyMainBufferToCanvas( &rAfter );

	/**************************************************
		in case of repeating arrow keys, draw the insertion point now.
		for any other key, the paint view has been modified (this is
		  needed when the user saves while editing text).
	**************************************************/
	if ( isArrowKey )
		this->DrawInsertionPoint( true );
	else
		mSettings.thePaintView->SetChangedFlag( true );
}

/*=================================
	Idle
===================================*/
void PTTextAction::Idle()
{
	if ( !mText ) return;
	if ( mNextInsertionTime > (UInt32) TickCount() ) return;
	
	this->DrawInsertionPoint( !mDrawnInsertionPoint );
	mNextInsertionTime = ((UInt32)TickCount()) + ::GetCaretTime();
}

/*=================================
	CloseEditSession
===================================*/
void PTTextAction::CloseEditSession()
{
	if ( mText )
	{
		this->DrawInsertionPoint( false );	// kill the insertion point
		
		::TEDispose( mText );
		mText = nil;
	}

	if ( mTextBuffer )
	{
		delete mTextBuffer;
		mTextBuffer = nil;
	}
}

/*=================================
	ChangeTextTraits
===================================*/
void PTTextAction::ChangeTextTraits( const TextTraitsRecord &inTraits )
{
	StSaveGWorld	aSaver;
	Rect			bigR = { -1000, -1000, 3000, 3000 };
	
	if ( !mText || !mTextBuffer ) return;

		// erase the insertion point -- its size may change
	this->DrawInsertionPoint( false );	

		// restore the image buffer to its initial state
	this->RestoreMainBufferFromUndoBuffer();
	
		// change the textedit record and our internal variables
	mTextTraits = inTraits;

		// we always draw in black since our text buffer is only 1-bit
	TextTraitsRecord copyOfTraits = mTextTraits;
	copyOfTraits.color = kBlackColor;

	mTextBuffer->BeginDrawing();
		UTextTraits::SetPortTextTraits( &copyOfTraits );
		::GetFontInfo( &mFontInfo );
		UTextTraits::SetTETextTraits( &copyOfTraits, mText, nil ); 
		::TECalText( mText );
		::TEUpdate( &bigR, mText );
	mTextBuffer->EndDrawing();
	
		// overlay the text into the main image buffer
	this->CopyTextBufferToMainBuffer();
	
		// redraw the entire canvas -- this covers all possible text changes
	Rect r;
	::SetRect( &r, 0, 0, mImageBuffer->GetWidth(), mImageBuffer->GetHeight() );
	this->CopyMainBufferToCanvas( &r );
	
		// tell the paint view we've changed
	mSettings.thePaintView->SetChangedFlag( true );
}


/*=================================
	RestoreMainBufferFromUndoBuffer
	
	Description:
	Restores the main buffer from the undo buffer. For speed, only copies the
	 currently used portion of the buffer.
===================================*/
void PTTextAction::RestoreMainBufferFromUndoBuffer()
{
	if ( !mText || !mImageBuffer || !mUndoBuffer ) return;

	Rect r;
	this->GetRectRelativeToImageBuffer( &r );
	mImageBuffer->CopyFrom( mUndoBuffer, &r, &r );
}

/*=================================
	CopyTextBufferToMainBuffer
===================================*/
void PTTextAction::CopyTextBufferToMainBuffer()
{
	if ( !mText || !mImageBuffer || !mTextBuffer ) return;

	Rect sourceR, destR;
	this->GetRectRelativeToImageBuffer( &destR );
	this->GetRectRelativeToTextBuffer( &sourceR );
	this->CopyAndColorize( mTextBuffer, mImageBuffer, sourceR, destR, mTextTraits.color );
}

/*=================================
	CopyMainBufferToCanvas
===================================*/
void PTTextAction::CopyMainBufferToCanvas( Rect *inDestR )
{
	if ( !mText || !mImageBuffer || !mTextBuffer ) return;
	
	Rect	r;
	
	if ( inDestR )
		r = *inDestR;
	else
		this->GetRectRelativeToImageBuffer( &r );

	mSettings.theCanvas->DrawFrom( mImageBuffer, &r );
	mSettings.thePaintView->RedrawSampleView();
}

/*=================================
	CopyAndColorize
===================================*/
void PTTextAction::CopyAndColorize( SUOffscreen *inSource, SUOffscreen *inDest,
									const Rect &inSourceR, const Rect &inDestR,
									RGBColor &inRGBColor )
{
	Rect sourceR, destR;
 
 	try
 	{
 		/*************************************************
 			pin the source & dest rectangles to the actual buffer size
 			so we don't overstep the bounds.
 		**************************************************/
		sourceR.left = MAX( 0, inSourceR.left );
		sourceR.right = MIN( inSource->GetWidth(), inSourceR.right );
		sourceR.top = MAX( 0, inSourceR.top );
		sourceR.bottom = MIN( inSource->GetHeight(), inSourceR.bottom );
		
		destR.left = MAX( 0, inDestR.left );
		destR.right = MIN( inDest->GetWidth(), inDestR.right );
		destR.top = MAX( 0, inDestR.top );
		destR.bottom = MIN( inDest->GetHeight(), inDestR.bottom );
		
 		/*************************************************
 			find the pixel value for the color we want
 		**************************************************/
		PixelValue 	destPixel = inDest->RGBToPixelValue( inRGBColor );

 		/*************************************************
 			loop through the source and set the appropriate pixels in the dest
 		**************************************************/
		SInt32		numRows = MIN( sourceR.bottom - sourceR.top, destR.bottom - destR.top );
		SInt32		numCols = MIN( sourceR.right - sourceR.left, destR.right - destR.left );
		
		for ( SInt32 rowCount = 0; rowCount < numRows; rowCount++ )
		{
			RawPtr	sourceRow = inSource->RawGetRowPtr( sourceR.top + rowCount );
			RawPtr	destRow = inDest->RawGetRowPtr( destR.top + rowCount );
			SInt32 	sourceCol = sourceR.left;
			SInt32 	destCol = destR.left;

			for ( SInt32 colCount = 0; colCount < numCols; colCount++ )
			{
				if ( inSource->RawGetPixel( sourceRow, sourceCol ) )
					inDest->RawSetPixel( destRow, destCol, destPixel );
					
				++sourceCol;
				++destCol;
			}
		}
	}
	catch( ... )
	{
	
	}
}

/*=================================
	GetRectRelativeToTextBuffer
===================================*/
Boolean PTTextAction::GetRectRelativeToTextBuffer( Rect *outR )
{
	if ( !mText || !mImageBuffer )
	{
		::SetRect( outR, 0, 0, 0, 0 );
		return( false );
	}
	
		// this gives us the left/right and top of the text buffer
	*outR = (**mText).destRect;

		// and this gives us the bottom
	outR->bottom = outR->top + this->CalcTextAreaHeight();

	::OffsetRect( outR, -outR->left, -outR->top );		// make sure it's 0,0-based
	return( ::EmptyRect( outR ) );
}

/*=================================
	CalcTextAreaHeight
===================================*/
SInt32 PTTextAction::CalcTextAreaHeight()
{
	if ( !mText ) return( 0 );

	SInt32 lineHeight = mFontInfo.ascent + mFontInfo.descent + mFontInfo.leading;
	SInt32 result = lineHeight * MAX( 1, (**mText).nLines );
	
	/*
		this fixes a bug in textedit (since 1984) - nLines is sometimes incorrect
	*/
	SInt32 numBytes = (**mText).teLength;
	if ( (numBytes > 0) && ((*(**mText).hText)[numBytes-1] == char_Return) )
		result += lineHeight;

	return( result );
}


#ifdef OLDWAY_TOO_SLOW
/*=================================
	GetRectRelativeToTextBuffer
===================================*/
Boolean PTTextAction::GetRectRelativeToTextBuffer( Rect *outR )
{
	if ( !mText || !mImageBuffer )
	{
		::SetRect( outR, 0, 0, 0, 0 );
		return( false );
	}
	
	*outR = (**mText).destRect;

		// bug in textedit (since 1984) - add lineHeight if the last char is a return
	short numBytes = (**mText).teLength;
	if ( (numBytes > 0) && ((*(**mText).hText)[numBytes-1] == char_Return) )
		outR->bottom += mFontInfo.ascent + mFontInfo.descent + mFontInfo.leading;

	::OffsetRect( outR, -outR->left, -outR->top );		// make sure it's 0,0-based
	return( ::EmptyRect( outR ) );
}
#endif

/*=================================
	GetRectRelativeToImageBuffer
===================================*/
Boolean PTTextAction::GetRectRelativeToImageBuffer( Rect *outR )
{
	this->GetRectRelativeToTextBuffer( outR );	// will be 0,0-based here
	::OffsetRect( outR, mTextLeft, mTextTop );
	return( ::EmptyRect( outR ) );
}

/*=================================
	DrawInsertionPoint
	
	Draws or erases the insertion point.
===================================*/
void PTTextAction::DrawInsertionPoint( Boolean inDraw )
{
	StSaveGWorld		aSaver;
	PTCanvas 			*theCanvas = mSettings.theCanvas;
	
	if ( !mText ) return;					// we're closed

	Rect sourceR, destR;
	this->GetInsertionPoint( &sourceR );
	::OffsetRect( &sourceR, mTextLeft, mTextTop );	// make it rel to image
	theCanvas->MapRectToCanvas( sourceR, &destR );

	theCanvas->FocusDraw();
	
	if ( inDraw )
	{
		StColorPenState		penSaver;
		penSaver.Normalize();				// selection always drawn in black
		
			// clip inside the canvas a bit or we don't always clean up well
		Rect		r;
		theCanvas->GetInsideDrawingRect( &r );
		StClipRgnState	saveSetAndRestore( r );

		::MoveTo( destR.left, destR.bottom );
		::LineTo( destR.left, destR.top );
		mDrawnInsertionPoint = true;
	}
	else
	{
		RgnHandle tempRgn = ::NewRgn();
		if ( !tempRgn ) return;
		
		::InsetRect( &destR, -2, -2 );		// make sure we hit the insertion point
		theCanvas->LocalToPortPoint( topLeft( destR ) );
		theCanvas->LocalToPortPoint( botRight( destR ) );
		::RectRgn( tempRgn, &destR );
		theCanvas->Draw( tempRgn );
		::DisposeRgn( tempRgn );

		mDrawnInsertionPoint = false;
	}
}


/*=================================
	GetInsertionPoint
	
	Description:
	Returns a rectangle which indicates where the insertion point is (rel to text).
	Returns false if no insertion point can be drawn.
===================================*/
Boolean PTTextAction::GetInsertionPoint( Rect *outR )
{
	if ( !mText )
	{
		::SetRect( outR, 0, 0, 0, 0 );
		return( false );
	}
	
	short selStart = (**mText).selStart;
	Point pt = ::TEGetPoint( selStart, mText );
	
		// bug in textedit (since 1984) - add lineHeight if the last char is a return
	if ( selStart == (**mText).teLength )
		if ( ((*(**mText).hText)[selStart-1] == char_Return) )
			pt.v += mFontInfo.ascent + mFontInfo.descent + mFontInfo.leading;

	outR->left = pt.h;
	outR->right = pt.h + 1;			// canvas doesn't like zero-width rectangles
	outR->bottom = pt.v - mFontInfo.descent;
	outR->top = outR->bottom - mFontInfo.ascent;
	return( true );
}

/*=================================
	MyCaretHook
	
	Description:
	If we don't put a dummy caret hook in the TERecord, sometimes the caret
	gets drawn into our offscreen buffer and never erased, leaving trails
	on the screen when the arrow keys are used. blech.
	
	Note: 
	On the 68K side, we have to use assembly language because the Rect *
	is passed on the stack *after* the return address and TEPtr is in
	D3, I think. blech. blech. See TextEdit.h and TextEditControl.c on
	the "Tools" CD from Apple.
===================================*/
#if TARGET_RT_MAC_CFM || __MACH__
static pascal void MyCaretHook( const Rect *, TEPtr )		// CFM version
{
}
#else
#ifdef WINVER
S_PASCAL(void) MyCaretHook( const Rect *, TEPtr )			// MSVC compatible
{
}
#else
static asm void MyCaretHook( void )
{
	move.l	(A7)+,D0
	rts
}
#endif
#endif

