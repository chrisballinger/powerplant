// Copyright й2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyrightй 1996 Metrowerks Corp. All rights reserved

---------------------------------------<< е >>-----------------------------------------

FILE:					LBarberPole.cp

CLASSES:				LBarberPole

DESCRIPTION:		Implementation file for LBarberPole

AUTHOR:				Robin Mair

CREATION DATE :	96.11.24

CHANGE HISTORY :

		96.11.24		rtm	Initial creation of file

---------------------------------------<< е >>-----------------------------------------
*/

#ifdef PowerPlant_PCH
	#include PowerPlant_PCH
#endif

#include "LBarberPole.h"

// е POWERPLANT HEADERS
#include <LPane.h>
#include <LPeriodical.h>
#include <UException.h>

// е TOOLBOX HEADERS
#include <Quickdraw.h>


//=====================================================================================
// CONSTANTS
//=====================================================================================

//=====================================================================================
// DEFINES
//=====================================================================================

#pragma mark === CLASS: LBarberPole

//=====================================================================================
// ее STREAM CREATION
//-------------------------------------------------------------------------------------
// LBarberPole::CreateLBarberPoleStream	
//-------------------------------------------------------------------------------------
//

LBarberPole*	
LBarberPole::CreateLBarberPoleStream ( LStream *inStream )
{

	return ( new LBarberPole ( inStream ));
	
}	// LBarberPole::CreateLBarberPoleStream


#pragma mark -
#pragma mark === INITIALIZATION

//=====================================================================================
// CLASS:	LBarberPole
//=====================================================================================

//=====================================================================================
// ее INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LBarberPole::LBarberPole					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LBarberPole::LBarberPole ()
{

	mPictureID = resID_Undefined;
	mAdornmentInset = 1;
	mAnimationStep = 2;
	mNextTime = 0;
	mIndicatorSpeed = 5;
	mPictureH = nil;
	mLastLoc = 0;
	
}	//	LBarberPole::LBarberPole


//-------------------------------------------------------------------------------------
// LBarberPole::LBarberPole					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LBarberPole::LBarberPole (	LStream*	inStream ) : LPane( inStream )
{

	// е Initialize our private fields
	mPictureH = nil;
	mNextTime = 0;
	mLastLoc = 0;
	
	// е Get our fields setup from the stream
	*inStream >> mPictureID;
	*inStream >> mAdornmentInset;
	*inStream >> mAnimationStep;
	*inStream >> mIndicatorSpeed;

}	//	LBarberPole::LBarberPole


//-------------------------------------------------------------------------------------
// LBarberPole::~LBarberPole				---<<е DESTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LBarberPole::~LBarberPole ()
{
	
	// е Make sure we dispose of the cached picture handle
	if ( mPictureH )
	{
		::DisposeHandle ( (Handle)mPictureH );
		mPictureH = nil;
	}
		
}	//	LBarberPole::~LBarberPole


//-------------------------------------------------------------------------------------
// LBarberPole::FinishCreateSelf
//-------------------------------------------------------------------------------------

void
LBarberPole::FinishCreateSelf ()
{
	
	// е Get the picture handle loaded, detached and cached
	PicHandle pictureH = ::GetPicture ( mPictureID );
	ThrowIfNil_ ( pictureH );
	if ( pictureH )
	{
		::DetachResource ( (Handle)pictureH );
		mPictureH = pictureH;
		
		// е Now that we have the picture we can get the initial
		// location setup
		mLastLoc = CalcInitialLocation ();
	}
	
}	//	LBarberPole::FinishCreateSelf


#pragma mark -
#pragma mark === ACCESSORS

//=====================================================================================
// ее ACCESSORS
//-------------------------------------------------------------------------------------
// LBarberPole::IsHorizontal
//-------------------------------------------------------------------------------------

Boolean
LBarberPole::IsHorizontal ()
{

	// е We check to see if the pole is wider than it is tall to determine
	// whether it is horizontal or not
	Rect localFrame;
	CalcLocalFrameRect ( localFrame );
	return localFrame.right - localFrame.left > localFrame.bottom - localFrame.top;
	
}	//	LBarberPole::IsHorizontal


//-------------------------------------------------------------------------------------
// LBarberPole::CalcInitialLocation
//-------------------------------------------------------------------------------------

Int16
LBarberPole::CalcInitialLocation ()
{

	//	е Only do something if we have a picture
	if ( mPictureH )
	{
		// е Get the area that the picture will be clipped to
		Rect	clippingRect;
		CalcPictureClippingRect ( clippingRect );
		Int16	clipWidth, clipHeight;
		clipWidth = clippingRect.right - clippingRect.left;
		clipHeight = clippingRect.bottom - clippingRect.top;
		
		// е We start first figuring out the size of the picture
		Int16 width, height;
		Rect pictureFrame = (*mPictureH)->picFrame;
		width = pictureFrame.right - pictureFrame.left;
		height = pictureFrame.bottom - pictureFrame.top;
		
		// е The initial location is the picture size less the size
		// of the are where the picture will be drawn
		if ( IsHorizontal ())
			return -(width - clipWidth);
		else
			return -(height - clipHeight);
	}
	
	// е If we have no picture simply return zero
	return 0;
	
}	//	LBarberPole::CalcInitialLocation


//-------------------------------------------------------------------------------------
// LBarberPole::CalcPictureClippingRect
//-------------------------------------------------------------------------------------

void
LBarberPole::CalcPictureClippingRect (	Rect	&outPicRect )
{

	// е We start from our local rect
	CalcLocalFrameRect ( outPicRect );
	
	// е Then we inset it by the adornment inset
	::InsetRect ( &outPicRect, mAdornmentInset, mAdornmentInset );
	
}	//	LBarberPole::CalcPictureClippingRect


//-------------------------------------------------------------------------------------
// LBarberPole::CalcPictureFrame
//-------------------------------------------------------------------------------------

void
LBarberPole::CalcPictureFrame (	Rect	&outPicFrame )
{

	// е If we have a picture we can proceed
	if ( mPictureH )
	{
	
		// е Get the area that the picture will be clipped to
		Rect	clippingRect;
		CalcPictureClippingRect ( clippingRect );
		
		// е We start first figuring out the size of the picture
		Int16 width, height;
		Rect pictureFrame = (*mPictureH)->picFrame;
		outPicFrame = pictureFrame;
		width = outPicFrame.right - outPicFrame.left;
		height = outPicFrame.bottom - outPicFrame.top;
		
		// е Now we need to position the rect correctly based on the
		// last location
		if ( IsHorizontal ())
		{
			// е We need to make sure that the position doesn't need resetting
			// because we have reached the end of the picture
			if ( mLastLoc > pictureFrame.left )
				mLastLoc = CalcInitialLocation ();
		
			outPicFrame.left = clippingRect.left + mLastLoc;
			outPicFrame.right = outPicFrame.left + width;
			outPicFrame.top = clippingRect.top;
			outPicFrame.bottom = outPicFrame.top + height;
		}
		else
		{
			// е We need to make sure that the position doesn't need resetting
			// because we have reached the end of the picture
			if ( mLastLoc > pictureFrame.top )
				mLastLoc = CalcInitialLocation ();
		
			outPicFrame.left = clippingRect.left;
			outPicFrame.right = outPicFrame.left + width;
			outPicFrame.top = clippingRect.top + mLastLoc;
			outPicFrame.bottom = outPicFrame.top + height;
		}
	}
	
}	//	LBarberPole::CalcPictureFrame


#pragma mark -
#pragma mark === SPEND TIME

//=====================================================================================
// ее DRAWING
//-------------------------------------------------------------------------------------
// LBarberPole::SpendTime
//-------------------------------------------------------------------------------------

void
LBarberPole::SpendTime ( const EventRecord &/*inMacEvent*/ )
{

	// е Start by getting the current tick count
	Int32	currTicks = ::TickCount ();
	
	// е If the current tick count is greater than our specified next
	// time then we proceed to switch the PICT IDs
	if ( currTicks > mNextTime )
	{
		// е Increment the last location by the nimation step amount
		mLastLoc += mAnimationStep;
				
		// е Get everything redrawn by calling Draw() we don't do a refresh
		// as that creates too much flicker
		if ( FocusDraw ())
			DrawBarberPole ();
		
		// е Setup our next time which is calculated as the current ticks
		// plus the rotation ticks
		mNextTime = ::TickCount () + mIndicatorSpeed;
	}
	
}	//	LBarberPole::SpendTime


#pragma mark -
#pragma mark === DRAWING

//=====================================================================================
// ее DRAWING
//-------------------------------------------------------------------------------------
// LBarberPole::DrawSelf
//-------------------------------------------------------------------------------------

void
LBarberPole::DrawSelf ()
{

	StColorPenState::Normalize ();
	
	// е Draw any adornment that the barber pole requires, by default
	// this is just a black frame
	DrawBarberPoleAdornment ();
	
	// е Now draw the barber pole, this will only happen once from
	// here after the intial drawing of the pole the animation is 
	// done by calling the DrawBarberPole method directly
	DrawBarberPole ();
		
}	//	LBarberPole::DrawSelf


//-------------------------------------------------------------------------------------
// LBarberPole::DrawBarberPoleAdornment
//-------------------------------------------------------------------------------------

void
LBarberPole::DrawBarberPoleAdornment ()
{

	Rect localFrame;
	CalcLocalFrameRect ( localFrame );
	
	// е By default all we do is draw a black frame around the pane
	RGBColor	black = { 0, 0, 0 };
	::RGBForeColor ( &black );
	::FrameRect ( &localFrame );
		
}	//	LBarberPole::DrawBarberPoleAdornment


//-------------------------------------------------------------------------------------
// LBarberPole::DrawBarberPole
//-------------------------------------------------------------------------------------

void
LBarberPole::DrawBarberPole ()
{

	Rect localFrame;
	CalcLocalFrameRect ( localFrame );
	
	// е Setup the clipping so that it excludes the area set aside for the
	// adornment
	StClipRgnState	theClipState;
	CalcPictureClippingRect ( localFrame );
	::ClipRect ( &localFrame );
	
	// е Now we get the picture drawn we always draw it whatever its full size
	// is and rely on the clipping to cut off the excess
	if ( mPictureH )
	{
		Rect pictureFrame;
		CalcPictureFrame ( pictureFrame );
		::DrawPicture ( mPictureH, &pictureFrame );
	}
		
}	//	LBarberPole::DrawBarberPole


