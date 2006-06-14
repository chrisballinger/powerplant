// Copyright й2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyrightй 1996 Metrowerks Corp. All rights reserved

---------------------------------------<< е >>-----------------------------------------

FILE:					LAnimatedPicture.cp

CLASSES:				LAnimatedPicture

DESCRIPTION:		Implementation file for LAnimatedPicture

AUTHOR:				Robin Mair

CREATION DATE :	96.11.06

CHANGE HISTORY :

		96.11.06		rtm	Initial creation of file

---------------------------------------<< е >>-----------------------------------------
*/

#ifdef PowerPlant_PCH
	#include PowerPlant_PCH
#endif

#include "LAnimatedPicture.h"

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

#pragma mark === CLASS: LAnimatedPicture

//=====================================================================================
// ее STREAM CREATION
//-------------------------------------------------------------------------------------
// LAnimatedPicture::CreateLAnimatedPictureStream	
//-------------------------------------------------------------------------------------
//

LAnimatedPicture*	
LAnimatedPicture::CreateLAnimatedPictureStream ( LStream *inStream )
{

	return ( new LAnimatedPicture ( inStream ));
	
}	// LAnimatedPicture::CreateLAnimatedPictureStream


#pragma mark -
#pragma mark === INITIALIZATION

//=====================================================================================
// CLASS:	LAnimatedPicture
//=====================================================================================

//=====================================================================================
// ее INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LAnimatedPicture::LAnimatedPicture					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LAnimatedPicture::LAnimatedPicture ()
{

	mFirstPictID = resID_Undefined;
	mLastPictID = resID_Undefined;
	mCurrPictID = resID_Undefined;
	mNextTime = 0;
	mIndicatorSpeed = 5;
	
}	//	LAnimatedPicture::LAnimatedPicture


//-------------------------------------------------------------------------------------
// LAnimatedPicture::LAnimatedPicture					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LAnimatedPicture::LAnimatedPicture (	LStream*	inStream ) : LPane( inStream )
{

	// е Get our fields setup from the stream
	*inStream >> mFirstPictID;
	*inStream >> mLastPictID;
	mCurrPictID = mFirstPictID;
	mNextTime = 0;
	*inStream >> mIndicatorSpeed;

}	//	LAnimatedPicture::LAnimatedPicture


//-------------------------------------------------------------------------------------
// LAnimatedPicture::~LAnimatedPicture				---<<е DESTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LAnimatedPicture::~LAnimatedPicture ()
{
}	//	LAnimatedPicture::~LAnimatedPicture


#pragma mark -
#pragma mark === SPEND TIME

//=====================================================================================
// ее DRAWING
//-------------------------------------------------------------------------------------
// LAnimatedPicture::SpendTime
//-------------------------------------------------------------------------------------

void
LAnimatedPicture::SpendTime ( const EventRecord &/*inMacEvent*/ )
{

	// е Start by getting the current tick count
	Int32	currTicks = ::TickCount ();
	
	// е If the current tick count is greater than our specified next
	// time then we proceed to switch the PICT IDs
	if ( currTicks > mNextTime )
	{
		// е Increment the PICT ID so that the indicator is animated
		mCurrPictID++;
		
		// е Check to see if we have reached the last PICT, if we have
		// then we need to start at the beginning again
		if ( mCurrPictID > mLastPictID )
			mCurrPictID = mFirstPictID;
			
		// е Get everything redrawn by calling Draw() we don't do a refresh
		// as that creates too much flicker
		Draw ( nil );
		
		// е Setup our next time which is calculated as the current ticks
		// plus the rotation ticks
		mNextTime = ::TickCount () + mIndicatorSpeed;
	}
	
}	//	LAnimatedPicture::SpendTime


#pragma mark -
#pragma mark === DRAWING

//=====================================================================================
// ее DRAWING
//-------------------------------------------------------------------------------------
// LAnimatedPicture::DrawSelf
//-------------------------------------------------------------------------------------

void
LAnimatedPicture::DrawSelf ()
{

	Rect localFrame;
	CalcLocalFrameRect ( localFrame );
	
	// е Get the current PICT
	PicHandle pictureH = ::GetPicture ( mCurrPictID );
	if ( pictureH )
	{
		::DrawPicture ( pictureH, &localFrame );
		::ReleaseResource ( (Handle)pictureH );
	}
		
}	//	LAnimatedPicture::DrawSelf


