// Copyright й2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyrightй 1996 Metrowerks Corp. All rights reserved

---------------------------------------<< е >>-----------------------------------------

FILE:					LAnimatedIcon.cp

CLASSES:				LAnimatedIcon

DESCRIPTION:		Implementation file for LAnimatedIcon

AUTHOR:				Robin Mair

CREATION DATE :	96.11.04

CHANGE HISTORY :

		96.11.04		rtm	Initial creation of file

---------------------------------------<< е >>-----------------------------------------
*/

#include "LAnimatedIcon.h"



//=====================================================================================
// CONSTANTS
//=====================================================================================

//=====================================================================================
// DEFINES
//=====================================================================================

#pragma mark === CLASS: LAnimatedIcon

//=====================================================================================
// ее STREAM CREATION
//-------------------------------------------------------------------------------------
// LAnimatedIcon::CreateLAnimatedIconStream	
//-------------------------------------------------------------------------------------
//

LAnimatedIcon*	
LAnimatedIcon::CreateLAnimatedIconStream ( LStream *inStream )
{

	return ( new LAnimatedIcon ( inStream ));
	
}	// LAnimatedIcon::CreateLAnimatedIconStream


#pragma mark -
#pragma mark === INITIALIZATION

//=====================================================================================
// CLASS:	LAnimatedIcon
//=====================================================================================

//=====================================================================================
// ее INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LAnimatedIcon::LAnimatedIcon					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LAnimatedIcon::LAnimatedIcon ()
{

	mFirstIconID = resID_Undefined;
	mLastIconID = resID_Undefined;
	mCurrIconID = resID_Undefined;
	mNextTime = 0;
	mIndicatorSpeed = 5;
	
}	//	LAnimatedIcon::LAnimatedIcon


//-------------------------------------------------------------------------------------
// LAnimatedIcon::LAnimatedIcon					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LAnimatedIcon::LAnimatedIcon (	LStream*	inStream ) : LPane( inStream )
{

	// е Get our fields setup from the stream
	inStream->ReadData( &mFirstIconID, sizeof ( ResIDT ) );
	inStream->ReadData( &mLastIconID, sizeof ( ResIDT ) );
	mCurrIconID = mFirstIconID;
	mNextTime = 0;
	inStream->ReadData( &mIndicatorSpeed, sizeof ( Int16 ) );

}	//	LAnimatedIcon::LAnimatedIcon


//-------------------------------------------------------------------------------------
// LAnimatedIcon::~LAnimatedIcon				---<<е DESTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LAnimatedIcon::~LAnimatedIcon ()
{
}	//	LAnimatedIcon::~LAnimatedIcon


#pragma mark -
#pragma mark === SPEND TIME

//=====================================================================================
// ее DRAWING
//-------------------------------------------------------------------------------------
// LAnimatedIcon::SpendTime
//-------------------------------------------------------------------------------------

void
LAnimatedIcon::SpendTime ( const EventRecord &/*inMacEvent*/ )
{

	// е Start by getting the current tick count
	Int32	currTicks = ::TickCount ();
	
	// е If the current tick count is greater than our specified next
	// time then we proceed to switch the PICT IDs
	if ( currTicks > mNextTime )
	{

		// е Refresh the old icon postion
		RefreshToIconMask ();

		// е Increment the icon ID so that the indicator is animated
		mCurrIconID++;
		
		// е Check to see if we have reached the last icon, if we have
		// then we need to start at the beginning again
		if ( mCurrIconID > mLastIconID )
			mCurrIconID = mFirstIconID;
			
		// е Now get new icon redrawn
		if ( FocusDraw ())
			DrawSelf ();
			
		// е Setup our next time which is calculated as the current ticks
		// plus the rotation ticks
		mNextTime = ::TickCount () + mIndicatorSpeed;
	}
	
}	//	LAnimatedIcon::SpendTime


#pragma mark -
#pragma mark === DRAWING

//=====================================================================================
// ее DRAWING
//-------------------------------------------------------------------------------------
// LAnimatedIcon::DrawSelf
//-------------------------------------------------------------------------------------

void
LAnimatedIcon::DrawSelf ()
{
	
	// е Get the icon rectangle
	Rect iconRect;
	CalcLocalFrameRect ( iconRect );
	
	// е  Now we plot the icon by calling the toolbox
	::PlotIconID ( &iconRect, atNone, ttNone, mCurrIconID );
		
}	//	LAnimatedIcon::DrawSelf


//-------------------------------------------------------------------------------------
// LAnimatedIcon::RefreshToIconMask
//-------------------------------------------------------------------------------------

void
LAnimatedIcon::RefreshToIconMask ()
{
	
	// е Get the icon rectangle
	Rect iconRect;
	CalcLocalFrameRect ( iconRect );
	
	// е  Get the region represented by the icons mask
	RgnHandle	iconRgn = ::NewRgn ();
	ThrowIfNil_ ( iconRgn );
	::IconIDToRgn ( iconRgn, &iconRect, atNone, mCurrIconID );
	
	// е Now get the region invalidated
	if ( FocusDraw ())
		::InvalRgn ( iconRgn );
		
	// е Clean up
	::DisposeRgn ( iconRgn );
	
}	//	LAnimatedIcon::RefreshToIconMask


