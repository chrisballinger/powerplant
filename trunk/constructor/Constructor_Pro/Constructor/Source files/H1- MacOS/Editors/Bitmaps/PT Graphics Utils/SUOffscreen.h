// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				SUOffscreen.h

CLASSES:			SUOffscreen

DESCRIPTION:		

AUTHOR:				Eric Shapiro

CREATION DATE :		96.4.18

CHANGE HISTORY :

		96.07.11	ebs		Removed all locking/unlocking code for speed
		96.07.10	ebs		Major rewrite (32-bit support)
		96.4.18		ebs		Integrated into Robin's sources
	
---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

	This class is the equivalent of a GWorld -- an offscreen buffer
	where you can draw to.  It has many more functions, though, and
	is used throughout the Icon Editor instead of raw GWorlds.
	
	For speed in the icon editor, we keep the pixels locked at all
	times. This saves a lot of time when getting/setting individual
	pixels (flipping, rotating, etc).
	
---------------------------------------<< ¥ >>----------------------------------------
*/

#pragma once

#include <QDOffscreen.h>	
#include "SU_Types.h"
#include "SU_Constants.h"
#include "SUPixelHelper.h"

class SUOffscreen;
class SUSharedPalette;

#define SU_BUFFER_LEAK_CHECK	

/*==========================================
	SUOffscreen
===========================================*/
class SUOffscreen
{
	public:
		static SUOffscreen *	CreateBuffer( SInt32 inWidth, SInt32 inHeight, SInt32 inDepth,
												CTabHandle inTable = nil, 
												GDHandle inDevice = nil,
												Boolean inKeepLocal = true );

		static GWorldPtr		CreateGWorld( SInt32 inWidth, SInt32 inHeight, SInt32 inDepth,
												CTabHandle inTable = nil, 
												GDHandle inDevice = nil,
												Boolean inKeepLocal = true,
												SInt32 *outRowBytes = nil );

		virtual					~SUOffscreen();
		
		/***************************************
			getting buffer info
		****************************************/
		virtual SInt32			GetWidth();
		virtual SInt32			GetHeight();
		virtual SInt32			GetDepth();
	
		virtual SInt32			GetRowBytes();
		virtual CTabHandle		GetColorTable();
		virtual GWorldPtr		GetGWorld();
		virtual PixMapHandle	GetPixMap();
		
		virtual Boolean			HasDirectPixels();
		
		/***************************************
			getting & setting pixels
		****************************************/
		virtual PixelValue		GetPixel( SInt32 h, SInt32 v );
		virtual void			SetPixel( SInt32 h, SInt32 v, PixelValue );
		
		virtual Color32			GetPixelColor( SInt32 h, SInt32 v );
		virtual RGBColor		GetPixelRGB( SInt32 h, SInt32 v );
		
			// converting between pixel values and colors
		virtual Color32 		PixelValueToColor32( PixelValue thePixelValue );
		virtual PixelValue		Color32ToPixelValue( Color32 );
		virtual RGBColor		PixelValueToRGB( PixelValue thePixelValue );
		virtual PixelValue		RGBToPixelValue( const RGBColor & );
		
		/***************************************
			low-level calls available for speedy pixel access
		****************************************/
		virtual void			SetForeColor( Color32 );
		virtual void			SetBackColor( Color32 );
		virtual void			SetForePixelValue( PixelValue );
		virtual void			SetBackPixelValue( PixelValue );
		
		/***************************************
			low-level calls available for speedy pixel access
		****************************************/
		virtual RawPtr			RawGetRowPtr( SInt32 v );
		virtual PixelValue		RawGetPixel( RawPtr inRowPtr, SInt32 h );
		virtual void			RawSetPixel( RawPtr inRowPtr, SInt32 h, PixelValue inPixel );
		
		virtual void			RawSetPixelColor( RawPtr inRowPtr, SInt32 h, Color32 inColor );
		virtual Color32			RawGetPixelColor( RawPtr inRowPtr, SInt32 h );
	
		/***************************************
			For drawing to
		****************************************/
		virtual void			BeginDrawing();
		virtual void			EndDrawing();
	
		/***************************************
			Palette & Color Table Stuff
		****************************************/
		virtual void			AssignPalette( CTabHandle );
		virtual void			AssignPalette( SUSharedPalette * );
		virtual SUSharedPalette	*GetPalette();
		
		/***************************************
			handy utilities
		****************************************/
		virtual void			ResetPen();
		virtual void			ResetColors();
		virtual void			EraseToWhite();
		virtual Boolean			IsErased();
		
		/***************************************
			copying buffers
		****************************************/
		virtual void			CopyTo( GrafPtr, const Rect *destR = nil, 
										const Rect *sourceR = nil, SInt16 xferMode = srcCopy, 
										RgnHandle inMaskRgn = nil );
		
		virtual void			CopyTo( SUOffscreen *, const Rect *destR = nil, 
										const Rect *sourceR = nil, SInt16 xferMode = srcCopy,
										RgnHandle inMaskRgn = nil );
		
		virtual void			CopyFrom( SUOffscreen *, const Rect *destR = nil,
										const Rect *sourceR = nil, SInt16 xferMode = srcCopy,
										RgnHandle inMaskRgn = nil );

		virtual void			CopyFromRawData( RawPtr sourceData, SInt32 sourceRowBytes );
		virtual void			CopyToRawData( RawPtr destData, SInt32 destRowBytes );

		virtual void 			CopyFromAndDownSample( SUOffscreen *inSource, 
														Color32 inMatchColor = kWhiteColor32,
														Color32 inDestColor = kBlackColor32 );
		
		/***************************************
			misc
		****************************************/
		virtual SUOffscreen		*CreateSimilarOffscreen( Boolean inUseSameDevice = false,
										SInt32 newWidth = -1, SInt32 newHeight = -1 );
		virtual void			IncreaseInverseTableSize();
		
		virtual RgnHandle		ConvertToRegion();
		virtual void			DebugShowInWindow();
		
		static SInt32			GetBufferCount();			// leak detection
		
		/***************************************
			locking & unlocking the buffer
		****************************************/
		#ifdef NOT_USED_ANYMORE
		virtual void			Lock();
		virtual void			Unlock();
		virtual Boolean			IsLocked();
		virtual void			ForceUnlock();
		#endif
		
	protected:
		GWorldPtr				mWorld;
		SInt32					mWidth, mHeight, mDepth, mRowBytes;
		Boolean					mHasDirectPixels;
		RawPtr					*mRowArray;			// pointers to each row of pixmap
		PixMapHandle			mPixMap;

			// this is used for leak detection
		static SInt32			sNumBuffers;
		
			// pointers to functions for getting/setting pixel values
		SUPixelGetter			mPixelGetter;
		SUPixelSetter			mPixelSetter;
	
		SUSharedPalette	*		mPalette;
		
								SUOffscreen( SInt32 inWidth, SInt32 inHeight, SInt32 inDepth,
												CTabHandle inTable, GDHandle inDevice,
												Boolean inKeepLocal );

		virtual void			ResetColorsSelf();
		virtual void			SetupRowArray();
};



#ifdef XXXXXXXX

#endif



/*==========================================
	StSaveAndDraw
===========================================*/
class StSaveAndDraw
{
	public:
					StSaveAndDraw( SUOffscreen * );
		virtual 	~StSaveAndDraw();
	
	protected:
		CGrafPtr	mOldPort;
		GDHandle	mOldDevice;
};

/*==========================================
	StBufferLeakDetector
===========================================*/
class StBufferLeakDetector
{
	public:
					StBufferLeakDetector();
		virtual 	~StBufferLeakDetector();
	
	protected:
		SInt32		mNumBuffers;
};

/*==========================================
	SUOffscreen inlines
===========================================*/
inline SInt32 SUOffscreen::GetWidth()
{
	return( mWidth );
}

inline SInt32 SUOffscreen::GetHeight()
{
	return( mHeight );
}

inline SInt32 SUOffscreen::GetDepth()
{
	return( mDepth );
}

inline GWorldPtr SUOffscreen::GetGWorld()
{
	return( mWorld );
}

inline SInt32 SUOffscreen::GetRowBytes()
{
	return( mRowBytes );
}

inline PixMapHandle SUOffscreen::GetPixMap()
{
	return( mPixMap );
}

inline RawPtr SUOffscreen::RawGetRowPtr( SInt32 v )
{
	return( mRowArray[ v ] );
}

inline PixelValue SUOffscreen::RawGetPixel( RawPtr inRowPtr, SInt32 h )
{
	return( (*mPixelGetter)( inRowPtr, h ) );
}

inline void SUOffscreen::RawSetPixel( RawPtr inRowPtr, SInt32 h, PixelValue inColor )
{
	(*mPixelSetter)( inRowPtr, h, inColor );
}

inline Boolean SUOffscreen::HasDirectPixels()
{
	return( mHasDirectPixels );
}

