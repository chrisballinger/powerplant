// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
#pragma once

#include "PTActions.h"
#include "UTextTraits.h"

/*==========================================
	PTTextAction 
===========================================*/
class PTTextAction : public PTTrackingPaintAction, public LBroadcaster
{
	public:
							PTTextAction( const SPaintAction & );
		virtual				~PTTextAction();
		
		virtual void 		HandleMouseDown( const SMouseDownEvent &inEvent );
		virtual void		HandleKeyDown( const EventRecord &inEvent );
		virtual void		Idle();
		
		virtual void		CloseEditSession();
		virtual void		ChangeTextTraits( const TextTraitsRecord & );
		virtual void		UndoSelf();
		
		virtual void		DrawInsertionPoint( Boolean inDraw = true );
		
			// these are in PTTextActionCmds.cpp
		static Boolean		FindCommandStatus( 
								const TextTraitsRecord &inTraits,
								CommandT inCommand, Boolean &outEnabled, 
								Boolean &outUsesMark, UInt16 &outMark, 
								Str255 outName, LMenu *inFontMenu );
								
		static Boolean		ObeyCommand( 	PTPaintView *inPaintView,
											PTTextAction *inTextAction,
											MessageT inMessage, 
											TextTraitsRecord *ioRec,
											Boolean *outChanged );
									
	protected:
		TEHandle			mText;
		SUOffscreen			*mTextBuffer;
		
		SUOffscreen			*mImageBuffer, *mUndoBuffer;
		SInt32				mImageWidth, mImageHeight;
				
		SInt32				mTextLeft, mTextTop;
		SInt32				mCurrentH, mCurrentV;
		
		Boolean				mDrawnInsertionPoint;
		UInt32				mNextInsertionTime;
		
		TextTraitsRecord	mTextTraits;
		FontInfo			mFontInfo;
		
		virtual void		CopyTextBufferToMainBuffer();
		virtual void		RestoreMainBufferFromUndoBuffer();
		virtual void		CopyMainBufferToCanvas( Rect *inDestR  = nil );
				
		virtual Boolean		GetRectRelativeToImageBuffer( Rect *outR );
		virtual Boolean		GetRectRelativeToTextBuffer( Rect *outR );
		virtual Boolean 	GetInsertionPoint( Rect *outR );
		
		virtual void		CopyAndColorize( 	SUOffscreen *inSource, SUOffscreen *inDest,
												const Rect &inSourceR, const Rect &inDestR,
												RGBColor &inRGBColor );
		virtual SInt32		CalcTextAreaHeight();
};

