// Copyright �2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright� 1996 Metrowerks Corp. All rights reserved

---------------------------------------<< � >>-----------------------------------------

FILE:					LTimerCaption.cp

CLASSES:				LTimerCaption

DESCRIPTION:		Implementation file for LTimerCaption

AUTHOR:				Robin Mair

CREATION DATE :	96.11.07

CHANGE HISTORY :

		96.11.10		rtm	Changed the setting of the time ormat so that it now correctly
								gtes the international 0 resource for the time format.
		96.11.07		rtm	Initial creation of file

---------------------------------------<< � >>-----------------------------------------
*/

#include "LTimerCaption.h"


// � TOOLBAOX HEADERS
#include	<Packages.h>
#include	<Script.h>


//=====================================================================================
// CONSTANTS
//=====================================================================================

//=====================================================================================
// DEFINES
//=====================================================================================

#pragma mark === CLASS: LTimerCaption

//=====================================================================================
// �� STREAM CREATION
//-------------------------------------------------------------------------------------
// LTimerCaption::CreateLTimerCaptionStream	
//-------------------------------------------------------------------------------------
//

LTimerCaption*	
LTimerCaption::CreateLTimerCaptionStream ( LStream *inStream )
{

	return ( new LTimerCaption ( inStream ));
	
}	// LTimerCaption::CreateLTimerCaptionStream


#pragma mark -
#pragma mark === INITIALIZATION

//=====================================================================================
// CLASS:	LTimerCaption
//=====================================================================================

//=====================================================================================
// �� INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LTimerCaption::LTimerCaption					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------

LTimerCaption::LTimerCaption ()
{

	mNextTime = 0;
	mIndicatorSpeed = 5;
	mStartTime = 0;
	mTimeFormatH = nil;
	
}	//	LTimerCaption::LTimerCaption


//-------------------------------------------------------------------------------------
// LTimerCaption::LTimerCaption					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------

LTimerCaption::LTimerCaption (	LStream*	inStream ) : LCaption ( inStream )
{

	// � Initialize our fields
	mNextTime = 0;
	mIndicatorSpeed = 60;
	mStartTime = 0;

	// � Get the time string formatting setup
	mTimeFormatH = (Intl0Hndl)::GetIntlResource ( 0 );
	(*mTimeFormatH)->timeCycle = 0;	
	(*mTimeFormatH)->timeFmt |= secLeadingZ | minLeadingZ | hrLeadingZ;

}	//	LTimerCaption::LTimerCaption


//-------------------------------------------------------------------------------------
// LTimerCaption::~LTimerCaption				---<<� DESTRUCTOR �>>---
//-------------------------------------------------------------------------------------

LTimerCaption::~LTimerCaption ()
{
}	//	LTimerCaption::~LTimerCaption


#pragma mark -
#pragma mark === TIMER CONTROL

//=====================================================================================
// �� TIMER CONTROL
//-------------------------------------------------------------------------------------
// LTimerCaption::StartTimer
//-------------------------------------------------------------------------------------

void
LTimerCaption::StartTimer ()
{

	// � Start by getting the starting time saved off
	::GetDateTime ( &mStartTime );
	
	// � Get the initial time displayed in the field which is always going
	// to be zeroed out
	Str255 startTimeString;

	// � Convert the time to the appropriate string that we can then
	// display in the caption, NOTE: we do an update port so that the
	// time shows up immediately
	::IUTimePString ( 0, true, startTimeString, (Handle)mTimeFormatH );

	// � Now get the string displayed in the caption
	SetDescriptor ( startTimeString );
	UpdatePort ();
	
	// � Then we get the timer added into the repeater queue
	StartRepeating ();
	
}	//	LTimerCaption::StartTimer


//-------------------------------------------------------------------------------------
// LTimerCaption::StopTimer
//-------------------------------------------------------------------------------------

void
LTimerCaption::StopTimer ()
{

	// � Stop the timer
	StopRepeating ();	
	
	// � Clear the start time field
	mStartTime = 0;
	
}	//	LTimerCaption::StopTimer


#pragma mark -
#pragma mark === SPEND TIME

//=====================================================================================
// �� SPEND TIME
//-------------------------------------------------------------------------------------
// LTimerCaption::SpendTime
//-------------------------------------------------------------------------------------

void
LTimerCaption::SpendTime ( const EventRecord &/*inMacEvent*/ )
{

	// � Start by getting the current tick count
	Int32	currTicks = ::TickCount ();
	
	// � If the current tick count is greater than our specified next
	// time then we proceed to switch update the timer
	if ( currTicks > mNextTime )
	{
		UInt32	elapsedTime;
		
		// � Start by getting the current time and then subtracting the
		// the start time so that we get the elapsed time
		::GetDateTime ( &elapsedTime );
		elapsedTime -= mStartTime;

		// � Convert the time to the appropriate string that we can then
		// display in the caption, NOTE: we do an update port so that the
		// time shows up immediately
		Str255 elapsedTimeString;
		::IUTimePString ( elapsedTime, true, elapsedTimeString, (Handle)mTimeFormatH );
		
		// � Now get the text set in the caption and update the port so
		// we can see the change
		SetDescriptor ( elapsedTimeString );
		UpdatePort ();	
			
		// � Now get new caption redrawn redrawn
		if ( FocusDraw ())
			DrawSelf ();
			
		// � Setup our next time which is calculated as the current ticks
		// plus the rotation ticks
		mNextTime = ::TickCount () + mIndicatorSpeed;
	}
	
}	//	LTimerCaption::SpendTime


