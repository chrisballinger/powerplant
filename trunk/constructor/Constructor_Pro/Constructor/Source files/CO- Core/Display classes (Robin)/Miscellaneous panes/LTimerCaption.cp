// Copyright й2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyrightй 1996 Metrowerks Corp. All rights reserved

---------------------------------------<< е >>-----------------------------------------

FILE:					LTimerCaption.cp

CLASSES:				LTimerCaption

DESCRIPTION:		Implementation file for LTimerCaption

AUTHOR:				Robin Mair

CREATION DATE :	96.11.07

CHANGE HISTORY :

		96.11.10		rtm	Changed the setting of the time ormat so that it now correctly
								gtes the international 0 resource for the time format.
		96.11.07		rtm	Initial creation of file

---------------------------------------<< е >>-----------------------------------------
*/

#include "LTimerCaption.h"


// е TOOLBAOX HEADERS
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
// ее STREAM CREATION
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
// ее INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LTimerCaption::LTimerCaption					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LTimerCaption::LTimerCaption ()
{

	mNextTime = 0;
	mIndicatorSpeed = 5;
	mStartTime = 0;
	mTimeFormatH = nil;
	
}	//	LTimerCaption::LTimerCaption


//-------------------------------------------------------------------------------------
// LTimerCaption::LTimerCaption					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LTimerCaption::LTimerCaption (	LStream*	inStream ) : LCaption ( inStream )
{

	// е Initialize our fields
	mNextTime = 0;
	mIndicatorSpeed = 60;
	mStartTime = 0;

	// е Get the time string formatting setup
	mTimeFormatH = (Intl0Hndl)::GetIntlResource ( 0 );
	(*mTimeFormatH)->timeCycle = 0;	
	(*mTimeFormatH)->timeFmt |= secLeadingZ | minLeadingZ | hrLeadingZ;

}	//	LTimerCaption::LTimerCaption


//-------------------------------------------------------------------------------------
// LTimerCaption::~LTimerCaption				---<<е DESTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LTimerCaption::~LTimerCaption ()
{
}	//	LTimerCaption::~LTimerCaption


#pragma mark -
#pragma mark === TIMER CONTROL

//=====================================================================================
// ее TIMER CONTROL
//-------------------------------------------------------------------------------------
// LTimerCaption::StartTimer
//-------------------------------------------------------------------------------------

void
LTimerCaption::StartTimer ()
{

	// е Start by getting the starting time saved off
	::GetDateTime ( &mStartTime );
	
	// е Get the initial time displayed in the field which is always going
	// to be zeroed out
	Str255 startTimeString;

	// е Convert the time to the appropriate string that we can then
	// display in the caption, NOTE: we do an update port so that the
	// time shows up immediately
	::IUTimePString ( 0, true, startTimeString, (Handle)mTimeFormatH );

	// е Now get the string displayed in the caption
	SetDescriptor ( startTimeString );
	UpdatePort ();
	
	// е Then we get the timer added into the repeater queue
	StartRepeating ();
	
}	//	LTimerCaption::StartTimer


//-------------------------------------------------------------------------------------
// LTimerCaption::StopTimer
//-------------------------------------------------------------------------------------

void
LTimerCaption::StopTimer ()
{

	// е Stop the timer
	StopRepeating ();	
	
	// е Clear the start time field
	mStartTime = 0;
	
}	//	LTimerCaption::StopTimer


#pragma mark -
#pragma mark === SPEND TIME

//=====================================================================================
// ее SPEND TIME
//-------------------------------------------------------------------------------------
// LTimerCaption::SpendTime
//-------------------------------------------------------------------------------------

void
LTimerCaption::SpendTime ( const EventRecord &/*inMacEvent*/ )
{

	// е Start by getting the current tick count
	Int32	currTicks = ::TickCount ();
	
	// е If the current tick count is greater than our specified next
	// time then we proceed to switch update the timer
	if ( currTicks > mNextTime )
	{
		UInt32	elapsedTime;
		
		// е Start by getting the current time and then subtracting the
		// the start time so that we get the elapsed time
		::GetDateTime ( &elapsedTime );
		elapsedTime -= mStartTime;

		// е Convert the time to the appropriate string that we can then
		// display in the caption, NOTE: we do an update port so that the
		// time shows up immediately
		Str255 elapsedTimeString;
		::IUTimePString ( elapsedTime, true, elapsedTimeString, (Handle)mTimeFormatH );
		
		// е Now get the text set in the caption and update the port so
		// we can see the change
		SetDescriptor ( elapsedTimeString );
		UpdatePort ();	
			
		// е Now get new caption redrawn redrawn
		if ( FocusDraw ())
			DrawSelf ();
			
		// е Setup our next time which is calculated as the current ticks
		// plus the rotation ticks
		mNextTime = ::TickCount () + mIndicatorSpeed;
	}
	
}	//	LTimerCaption::SpendTime


