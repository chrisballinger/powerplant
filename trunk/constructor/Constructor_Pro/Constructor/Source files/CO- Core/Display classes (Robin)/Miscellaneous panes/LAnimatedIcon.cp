// Copyright �2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright� 1996 Metrowerks Corp. All rights reserved

---------------------------------------<< � >>-----------------------------------------

FILE:					LAnimatedIcon.cp

CLASSES:				LAnimatedIcon

DESCRIPTION:		Implementation file for LAnimatedIcon

AUTHOR:				Robin Mair

CREATION DATE :	96.11.04

CHANGE HISTORY :

		96.11.04		rtm	Initial creation of file

---------------------------------------<< � >>-----------------------------------------
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
// �� STREAM CREATION
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
// �� INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LAnimatedIcon::LAnimatedIcon					---<<� CONSTRUCTOR �>>---
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
// LAnimatedIcon::LAnimatedIcon					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------

LAnimatedIcon::LAnimatedIcon (	LStream*	inStream ) : LPane( inStream )
{

	// � Get our fields setup from the stream
	inStream->ReadData( &mFirstIconID, sizeof ( ResIDT ) );
	inStream->ReadData( &mLastIconID, sizeof ( ResIDT ) );
	mCurrIconID = mFirstIconID;
	mNextTime = 0;
	inStream->ReadData( &mIndicatorSpeed, sizeof ( Int16 ) );

}	//	LAnimatedIcon::LAnimatedIcon


//-------------------------------------------------------------------------------------
// LAnimatedIcon::~LAnimatedIcon				---<<� DESTRUCTOR �>>---
//-------------------------------------------------------------------------------------

LAnimatedIcon::~LAnimatedIcon ()
{
}	//	LAnimatedIcon::~LAnimatedIcon


#pragma mark -
#pragma mark === SPEND TIME

//=====================================================================================
// �� DRAWING
//-------------------------------------------------------------------------------------
// LAnimatedIcon::SpendTime
//-------------------------------------------------------------------------------------

void
LAnimatedIcon::SpendTime ( const EventRecord &/*inMacEvent*/ )
{

	// � Start by getting the current tick count
	Int32	currTicks = ::TickCount ();
	
	// � If the current tick count is greater than our specified next
	// time then we proceed to switch the PICT IDs
	if ( currTicks > mNextTime )
	{

		// � Refresh the old icon postion
		RefreshToIconMask ();

		// � Increment the icon ID so that the indicator is animated
		mCurrIconID++;
		
		// � Check to see if we have reached the last icon, if we have
		// then we need to start at the beginning again
		if ( mCurrIconID > mLastIconID )
			mCurrIconID = mFirstIconID;
			
		// � Now get new icon redrawn
		if ( FocusDraw ())
			DrawSelf ();
			
		// � Setup our next time which is calculated as the current ticks
		// plus the rotation ticks
		mNextTime = ::TickCount () + mIndicatorSpeed;
	}
	
}	//	LAnimatedIcon::SpendTime


#pragma mark -
#pragma mark === DRAWING

//=====================================================================================
// �� DRAWING
//-------------------------------------------------------------------------------------
// LAnimatedIcon::DrawSelf
//-------------------------------------------------------------------------------------

void
LAnimatedIcon::DrawSelf ()
{
	
	// � Get the icon rectangle
	Rect iconRect;
	CalcLocalFrameRect ( iconRect );
	
	// �  Now we plot the icon by calling the toolbox
	::PlotIconID ( &iconRect, atNone, ttNone, mCurrIconID );
		
}	//	LAnimatedIcon::DrawSelf


//-------------------------------------------------------------------------------------
// LAnimatedIcon::RefreshToIconMask
//-------------------------------------------------------------------------------------

void
LAnimatedIcon::RefreshToIconMask ()
{
	
	// � Get the icon rectangle
	Rect iconRect;
	CalcLocalFrameRect ( iconRect );
	
	// �  Get the region represented by the icons mask
	RgnHandle	iconRgn = ::NewRgn ();
	ThrowIfNil_ ( iconRgn );
	::IconIDToRgn ( iconRgn, &iconRect, atNone, mCurrIconID );
	
	// � Now get the region invalidated
	if ( FocusDraw ())
		::InvalRgn ( iconRgn );
		
	// � Clean up
	::DisposeRgn ( iconRgn );
	
}	//	LAnimatedIcon::RefreshToIconMask


