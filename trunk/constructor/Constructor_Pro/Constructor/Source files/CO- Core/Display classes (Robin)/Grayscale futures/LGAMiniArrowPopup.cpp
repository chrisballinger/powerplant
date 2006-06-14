// Copyright �2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright� 1996 Metrowerks Corp. All rights reserved

---------------------------------------<< � >>-----------------------------------------

FILE:					LGAMiniArrowPopup.cp

CLASSES:				LGAMiniArrowPopup

DESCRIPTION:		Implementation file for LGAMiniArrowPopup

AUTHOR:				Robin Mair

CREATION DATE :	96.12.14

CHANGE HISTORY :

		96.12.14		rtm	Initial creation of file

---------------------------------------<< � >>-----------------------------------------
*/

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

#ifdef PowerPlant_PCH
	#include PowerPlant_PCH
#endif

// � GRAYSCALE HEADERS
#include "LGAMiniArrowPopup.h"
#include <UGraphicsUtilities.h>
#include	<UGAColorRamp.h>

// � POWERPLANT HEADERS
#include <LControl.h>
#include <LStream.h>
#include <PP_Messages.h>
#include <UDrawingState.h>
#include <UDrawingUtils.h>
#include <LString.h>
#include <UException.h>
#include <UTextTraits.h>

// � TOOLBOX HEADERS
#include <Quickdraw.h>
#include <Icons.h>
#include <LowMem.h>
#include <Menus.h>
#include <TextEdit.h>
#include <ToolUtils.h>


//=====================================================================================
// CONSTANTS
//=====================================================================================

const	SInt16		miniPopup_Width				=	15;
const	SInt16		miniPopup_Height				=	12;

const	UInt16	miniPopup_SmallMark			=	'�';	// Mark used for small font popups

//=====================================================================================
// DEFINES
//=====================================================================================

#pragma mark === CLASS: LGAMiniArrowPopup

//=====================================================================================
// �� STREAM CREATION
//-------------------------------------------------------------------------------------
// LGAMiniArrowPopup::CreateLGAMiniArrowPopupStream	
//-------------------------------------------------------------------------------------
//

LGAMiniArrowPopup*	
LGAMiniArrowPopup::CreateLGAMiniArrowPopupStream ( LStream *inStream )
{

	return ( new LGAMiniArrowPopup ( inStream ));
	
}	// LGAMiniArrowPopup::CreateLGAMiniArrowPopupStream


//=====================================================================================
// CLASS:	LGAMiniArrowPopup
//=====================================================================================

#pragma mark -
#pragma mark === INITIALIZATION

//=====================================================================================
// �� INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LGAMiniArrowPopup::LGAMiniArrowPopup					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------

LGAMiniArrowPopup::LGAMiniArrowPopup ()
{

	mPopupMenuResID = 0;
	mUseResMenu = false;
	mResType = 0;
	mPulldown = true;
	mTextTraitsID = 0;
	mHilited = false;
	mCachedMenuH = nil;
	
}	//	LGAMiniArrowPopup::LGAMiniArrowPopup


//-------------------------------------------------------------------------------------
// LGAMiniArrowPopup::LGAMiniArrowPopup					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------

LGAMiniArrowPopup::LGAMiniArrowPopup (	LStream	*inStream ) : LControl ( inStream )
{
	
	SInt16		controlKind;
	SInt32		macRefCon;
	SInt16		currItem;
	Str255	title;
	
	// � Read in the stream data
	*inStream >> controlKind;
	mUseResMenu = (controlKind & 4) != 0;
	*inStream >> mTextTraitsID;
	inStream->ReadPString ( title );
	*inStream >> macRefCon;
	mResType = (OSType)macRefCon;
	*inStream >> currItem;
	
	// � Now we need to map some of the values as used by the standard popup
	// to our fields
	mCachedMenuH = nil;
	mPopupMenuResID = mMinValue;
	 	
	// � Setup some our private data
	mPulldown = true;						//	For AGA we always want a pulldown
	
	// � Make sure we setup the value field with the current item
	if ( currItem > 0 )
		mValue = currItem;
	else
		mValue = 1;
	
	// � Make sure we start with the hilite state set to false
	mHilited = false;

	// � NOTE: at this point in time the menu handle has not yet been
	// cached as that takes place in FinishCreateSelf.
	
}	//	LGAMiniArrowPopup::LGAMiniArrowPopup


//-------------------------------------------------------------------------------------
// LGAMiniArrowPopup::~LGAMiniArrowPopup				---<<� DESTRUCTOR �>>---
//-------------------------------------------------------------------------------------

LGAMiniArrowPopup::~LGAMiniArrowPopup ()
{

	// � Get rid of the menu handle
	if ( mCachedMenuH )
	{
		::DisposeMenu ( mCachedMenuH );
		mCachedMenuH = nil;
	}
	
}	//	LGAMiniArrowPopup::~LGAMiniArrowPopup


//-------------------------------------------------------------------------------------
// LGAMiniArrowPopup::FinishCreateSelf
//-------------------------------------------------------------------------------------

void
LGAMiniArrowPopup::FinishCreateSelf ()
{

	// � We are now going to check the size of the popup to ensure that
	// it is the size we require as we are setup up to be one specific
	// size
	SDimension16	frameSize;
	GetFrameSize ( frameSize );
	if ( frameSize.width != miniPopup_Width || frameSize.height != miniPopup_Height )
	{
		ResizeFrameTo ( miniPopup_Width, miniPopup_Height, false );
	}
	
	// � Make sure that the correct values are setup for the popup
	SetMacMenuH ( LoadPopupMenuH () );

}	//	LGAMiniArrowPopup::FinishCreateSelf

	
#pragma mark -
#pragma mark === ACCESSORS

//=====================================================================================
// �� ACCESSORS
//-------------------------------------------------------------------------------------
// LGAMiniArrowPopup::LoadPopupMenuH
//-------------------------------------------------------------------------------------

MenuHandle
LGAMiniArrowPopup::LoadPopupMenuH () const
{

	// � Load the menu handle and then if we are building the menu
	// from a resource type we get the types added to the menu
	MenuHandle	menuH = ::GetMenu ( mPopupMenuResID );
	if ( menuH && UseResourceMenu ())
		LoadResourceMenu ( menuH );
		
	return menuH;
	
}	//	LGAMiniArrowPopup::LoadPopupMenuH


//-------------------------------------------------------------------------------------
// LGAMiniArrowPopup::GetCurrentItemTitle
//-------------------------------------------------------------------------------------

void
LGAMiniArrowPopup::GetCurrentItemTitle ( Str255 outItemTitle )
{
	// � Load the popup menu and then find the title
	MenuHandle	popupMenu = GetMacMenuH ();
	if ( popupMenu )
	{			
		::GetMenuItemText ( popupMenu, GetValue (), outItemTitle );
	}
	
}	//	LGAMiniArrowPopup::GetCurrentItemTitle


//-------------------------------------------------------------------------------------
// LGAMiniArrowPopup::GetMenuFontSize
//-------------------------------------------------------------------------------------

SInt16
LGAMiniArrowPopup::GetMenuFontSize () const
{

	// � Get the font size for the popup so we can figure out the correct
	// mark to apply
	StTextState			theTextState;

	ResIDT	textTID = GetTextTraitsID ();
	TextTraitsH traitsH = UTextTraits::LoadTextTraits ( textTID );
	if ( traitsH ) 
	{
		return (**traitsH).size;
	}
	
	// � Default size is 9 point
	return 9;
	
}	//	LGAMiniArrowPopup::GetMenuFontSize


//-------------------------------------------------------------------------------------
// LGAMiniArrowPopup::SetMacMenuH
//-------------------------------------------------------------------------------------

void
LGAMiniArrowPopup::SetMacMenuH (	MenuHandle	inMenuHandle )
{

	// � If we have an existing handle make sure we get rid of
	// it first before we add the new one
	if ( mCachedMenuH && mCachedMenuH != inMenuHandle )
	{
		// � Get rid of the old cached menu
		::DisposeMenu ( mCachedMenuH );
		
		// � Add the new menu handle
		mCachedMenuH = inMenuHandle;
	}
	else if ( mCachedMenuH == nil )
	{
		// � Add the new menu handle
		mCachedMenuH = inMenuHandle;
	}
	
	// � We also detach the resource so we don't run into problems
	// for cases where there are multiple popup referencing the same
	// menu resource, this is only done if we are caching the handle
	if ( inMenuHandle )
		::DetachResource ( (Handle)inMenuHandle );
		
	// � The final step is to setup the popups min and max values
	SetPopupMinMaxValues ();
	
}	//	LGAMiniArrowPopup::SetMacMenuH


//-------------------------------------------------------------------------------------
// LGAMiniArrowPopup::SetPopupMenuResID
//-------------------------------------------------------------------------------------

void
LGAMiniArrowPopup::SetPopupMenuResID (	ResIDT	inResID )
{

	// � Only make the change if we have to
	if ( mPopupMenuResID != inResID )
	{
		mPopupMenuResID = inResID;
		
		// � Now we need to load the new menu and get it
		// saved into the cache
		SetMacMenuH ( LoadPopupMenuH () );
		
		// � Because the new menu might be shorter than the old one
		// we need to make sure the the current item is not off the
		// end of the menu, if it is we simply set the first item
		if ( GetValue () > GetMaxValue () )
			SetValue ( 1 );
			
		// � Get things redrawn so the changes can be seen
		Draw ( nil);
	}

}	//	LGAMiniArrowPopup::SetPopupMenuResID


//-------------------------------------------------------------------------------------
// LGAMiniArrowPopup::SetHiliteState
//-------------------------------------------------------------------------------------

void			
LGAMiniArrowPopup::SetHiliteState	(	Boolean	inHiliteState )
{
	
	// � Only change the state if have too
	if ( mHilited != inHiliteState )
	{
		mHilited = inHiliteState;
		
		// � Get the popup redrawn so that the state change can take effect
		Draw ( nil );
	}
			
}	//	LGAMiniArrowPopup::SetHiliteState


//-------------------------------------------------------------------------------------
// LGAMiniArrowPopup::SetValue
//-------------------------------------------------------------------------------------

void
LGAMiniArrowPopup::SetValue	(	SInt32	inValue )
{

	// � Setup the current item if needed
	if ( GetValue () != inValue )
	{
		// � Get the current item setup in the menu
		MenuHandle menuH = GetMacMenuH ();
		if ( menuH )
		{
			SetupCurrentMenuItem ( menuH, inValue );
		}

		// � Call our superclass to handle the setting of the value
		LControl::SetValue ( inValue );
	
		// � Now we need to get the popup redrawn so that the change
		// will be seen
		Draw ( nil );
	}
	
}	//	LGAMiniArrowPopup::SetValue


//-------------------------------------------------------------------------------------
// LGAMiniArrowPopup::SetupCurrentMenuItem
//-------------------------------------------------------------------------------------

void
LGAMiniArrowPopup::SetupCurrentMenuItem (	MenuHandle	inMenuH,
											SInt16			inCurrentItem )
{
	
	// � If the current item has changed then make it so, this
	// also involves removing the mark from any old item
	if ( inMenuH )
	{
		if ( GetValue () != inCurrentItem )
		{
			// � Get the current value
			SInt16		oldItem = GetValue ();
			
			if ((oldItem > 0) and (oldItem <= ::CountMenuItems(inMenuH)))
			{
				// � Remove the current mark
				::SetItemMark ( inMenuH, oldItem, 0 );
			}
		}
		
		if ((inCurrentItem > 0) and (inCurrentItem <= ::CountMenuItems(inMenuH)))
		{
			// � Always make sure item is marked
			UInt16	mark = GetMenuFontSize () < 12 ? miniPopup_SmallMark : checkMark;
			::SetItemMark ( 	inMenuH, 
									inCurrentItem, 
									mark );
		}
	}
	
}	//	LGAMiniArrowPopup::SetupCurrentMenuItem


//-------------------------------------------------------------------------------------
// LGAMiniArrowPopup::SetPulldown
//-------------------------------------------------------------------------------------

void
LGAMiniArrowPopup::SetPulldown (	Boolean	inPulldown )
{

	if ( mPulldown != inPulldown )
	{
		mPulldown = inPulldown;
	}
	
}	//	LGAMiniArrowPopup::SetPulldown


//-------------------------------------------------------------------------------------
// LGAMiniArrowPopup::SetPopupMinMaxValues
//-------------------------------------------------------------------------------------

void
LGAMiniArrowPopup::SetPopupMinMaxValues ()
{

	// � We will have to synthesize these values as we dont' have a real
	// control that we are talking too
	MenuHandle	popupMenu = GetMacMenuH ();
	if ( popupMenu )
	{
		// � Setup the values
		mMinValue = 1;
		mMaxValue = ::CountMenuItems ( popupMenu );
	}
		
}	//	LGAMiniArrowPopup::SetPopupMinMaxValues


//-------------------------------------------------------------------------------------
// LGAMiniArrowPopup::CalcLocalPopupFrameRect
//-------------------------------------------------------------------------------------
//

void
LGAMiniArrowPopup::CalcLocalPopupFrameRect	( Rect	&outRect )
{
	
	// � For this class the whole control is the popup
	CalcLocalFrameRect ( outRect );
				
}	//	LGAMiniArrowPopup::CalcLocalPopupFrameRect


//-------------------------------------------------------------------------------------
// LGAMiniArrowPopup::GetPopupMenuPosition
//-------------------------------------------------------------------------------------

void
LGAMiniArrowPopup::GetPopupMenuPosition	( Point	&outPopupLoc )
{
	
	// � Get the local popup button frame, this will be correctly setup depending
	// on whether the button is arrow only or not
	Rect	popupRect;
	CalcLocalPopupFrameRect ( popupRect );
	
	// � Setup the vertical value based on whether it is a pulldown or popup menu
	outPopupLoc.v = IsPulldownMenu() ? popupRect.bottom : popupRect.top;

	// � If the popup is setup to have a pulldown menu then calculate the
	// apropriate location for the popup menu to appear
	if ( IsPulldownMenu ())
	{	
		outPopupLoc.h = popupRect.left + 2;
	}
	else
	{
		outPopupLoc.h =  popupRect.left;
	}
	
	// � Because PopMenuSelect needs the location for the popup menu global 
	// coordinates we need to convert the location
	LocalToPortPoint ( outPopupLoc );
	PortToGlobalPoint ( outPopupLoc );
	
}	//	LGAMiniArrowPopup::GetPopupMenuPosition


#pragma mark -
#pragma mark === ENABLING & DISABLING

//=====================================================================================
// �� ENABLING & DISABLING
//-------------------------------------------------------------------------------------
// LGAMiniArrowPopup::EnableSelf
//-------------------------------------------------------------------------------------
//

void
LGAMiniArrowPopup::EnableSelf ()
{

	// � Get things redrawn so that we can see the state change
	Draw ( nil );

}	//	LGAMiniArrowPopup::EnableSelf


//-------------------------------------------------------------------------------------
// LGAMiniArrowPopup::DisableSelf
//-------------------------------------------------------------------------------------
//

void
LGAMiniArrowPopup::DisableSelf ()
{

	// � Get things redrawn so that we can see the state change
	Draw ( nil );

}	//	LGAMiniArrowPopup::DisableSelf


#pragma mark -
#pragma mark === ACTIVATION

//=====================================================================================
// �� ACTIVATION
//-------------------------------------------------------------------------------------
// LGAMiniArrowPopup::ActivateSelf
//-------------------------------------------------------------------------------------

void
LGAMiniArrowPopup::ActivateSelf ()
{

	if ( FocusDraw ())
		DrawSelf ();
				
}	//	LGAMiniArrowPopup::ActivateSelf



//-------------------------------------------------------------------------------------
// LGAMiniArrowPopup::DeactivateSelf
//-------------------------------------------------------------------------------------

void
LGAMiniArrowPopup::DeactivateSelf ()
{

	if ( FocusDraw ())
		DrawSelf ();
				
}	//	LGAMiniArrowPopup::DeactivateSelf


//-------------------------------------------------------------------------------------
// LGAMiniArrowPopup::Activate
//-------------------------------------------------------------------------------------

void
LGAMiniArrowPopup::Activate ()
{

	LControl::Activate ();
	
	//	� Get the border redrawn
	UpdatePort ();
		
}	//	LGAMiniArrowPopup::Activate



//-------------------------------------------------------------------------------------
// LGAMiniArrowPopup::Deactivate
//-------------------------------------------------------------------------------------

void
LGAMiniArrowPopup::Deactivate ()
{

	LControl::Deactivate ();
	
	//	� Get the border redrawn
	UpdatePort ();
		
}	//	LGAMiniArrowPopup::Deactivate


#pragma mark -
#pragma mark === DRAWING

//=====================================================================================
// �� DRAWING
//-------------------------------------------------------------------------------------
// LGAMiniArrowPopup::DrawSelf
//-------------------------------------------------------------------------------------

void
LGAMiniArrowPopup::DrawSelf	()
{
	
	StColorPenState theSavedPenState;
	theSavedPenState.Normalize ();
		
	// � Get the control drawn in its various states
	if ( IsHilited ())
		DrawPopupHilited ();
	else
		DrawPopupNormal ();
	
}	//	LGAMiniArrowPopup::DrawSelf


//-------------------------------------------------------------------------------------
// LGAMiniArrowPopup::DrawPopupNormal
//-------------------------------------------------------------------------------------

void
LGAMiniArrowPopup::DrawPopupNormal	()
{
	
	// � Get the frame for the control
	Rect	localFrame;
	CalcLocalPopupFrameRect ( localFrame );

	// � Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( localFrame );
	SInt16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{

		// � BORDER
		RGBColor theColor;
		if ( depth < 4 )
		{
			theColor = UGAColorRamp::GetBlackColor();
			::RGBForeColor ( &theColor );
		}
		else
		{
			if ( IsEnabled () && IsActive ())
			{
				theColor = UGAColorRamp::GetBlackColor();
				::RGBForeColor ( &theColor );
			}
			else
			{
				theColor = UGAColorRamp::GetColor(colorRamp_Gray7);
				::RGBForeColor ( &theColor );
			}
		}
		
		// � Draw the frame around the entire control
		UGraphicsUtilities::NoCornerPixelFrame ( &localFrame );
			
		// � Inset the popup frame
		::InsetRect ( &localFrame, 1, 1 );
		
		// � BLACK & WHITE
		if ( depth < 4 )
		{
			// � Before we draw anything we need to erase the control in case we
			// we had previously been hilited
			theColor = UGAColorRamp::GetWhiteColor();
			::RGBBackColor ( &theColor);
			::EraseRect ( &localFrame );

			// ��DRAW ACTUAL ARROW
			// � Always draw in black for B&W
			theColor = UGAColorRamp::GetBlackColor();
			::RGBForeColor ( &theColor );

			// � If we are disabled we draw in gray
			if ( !IsEnabled ())
			{
				Pattern thePattern;
				UQDGlobals::GetGrayPat(&thePattern);
				::PenPat ( &thePattern );
			}

			// � Handle the drawing of the actual arrow
			DrawPopupArrow ( localFrame );
		}
		// � COLOR
		else
		{
			// � We need to normail in case we have just drawn a portion in
			// B&W in which case the pen pattern will be off
			StColorPenState::Normalize ();
			
			// � First make sure the face of the control is drawn
			theColor = UGAColorRamp::GetColor(2);
			::RGBForeColor ( &theColor);
			::PaintRect ( &localFrame );
			
			// � We only draw the bevels if we are enabled
			if ( IsEnabled () && IsActive ())
			{
				// �� LIGHT BEVELS
				// � Setup Colors for top left edges
				theColor = UGAColorRamp::GetWhiteColor();
				::RGBForeColor ( &theColor );
				UGraphicsUtilities::TopLeftSide ( &localFrame, 0, 0, 1, 1 );

				// �� SHADOW BEVELS
				// � Setup Colors for bottom right edges
				theColor = UGAColorRamp::GetColor(5);
				::RGBForeColor ( &theColor );
				UGraphicsUtilities::BottomRightSide ( &localFrame, 1, 1, 0, 0 );
			}
	
			// � DRAW ACTUAL ARROW
			// � Setup the color for either the enabled or disabled states
			if ( IsEnabled () && IsActive ())\
			{
				theColor = UGAColorRamp::GetBlackColor();
				::RGBForeColor ( &theColor);
			}
			else
			{
				theColor = UGAColorRamp::GetColor(7);
				::RGBForeColor ( &theColor);
			}

			// � Handle the drawing of the actual arrow
			DrawPopupArrow ( localFrame );
		}	
	}
	
}	//	LGAMiniArrowPopup::DrawPopupNormal


//-------------------------------------------------------------------------------------
// G3DButton::DrawPopupHilited
//-------------------------------------------------------------------------------------

void
LGAMiniArrowPopup::DrawPopupHilited	()
{
	
	StColorPenState theSavedPenState;
	theSavedPenState.Normalize ();
	
	// � Get the frame for the control
	Rect	localFrame;
	CalcLocalPopupFrameRect ( localFrame );

	// � Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( localFrame );
	SInt16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{

		// � BORDER
		// � Draw the frame around the entire control
		RGBColor theColor = UGAColorRamp::GetBlackColor();
		::RGBForeColor ( &theColor );
		UGraphicsUtilities::NoCornerPixelFrame ( &localFrame );
			
		// � Inset the popup frame
		::InsetRect ( &localFrame, 1, 1 );
		
		// � BLACK & WHITE
		if ( depth < 4 )
		{
			// � First make sure the face of the control is drawn
			theColor = UGAColorRamp::GetBlackColor();
			::RGBForeColor ( &theColor );
			::PaintRect ( &localFrame );
			
			// � Draw the actual arrow which we only do this when drawing in color
			theColor = UGAColorRamp::GetWhiteColor();
			::RGBForeColor ( &theColor );
			DrawPopupArrow ( localFrame );
		}
		else
		{
			// � First make sure the face of the control is drawn
			theColor = UGAColorRamp::GetColor(colorRamp_Gray8);
			::RGBForeColor ( &theColor);
			::PaintRect ( &localFrame );
			
			// �� LIGHT BEVELS
			// � Setup Colors for top left edges
			theColor = UGAColorRamp::GetColor(colorRamp_Gray10);
			::RGBForeColor ( &theColor );
			UGraphicsUtilities::TopLeftSide ( &localFrame, 0, 0, 1, 1 );

			// �� SHADOW BEVELS
			// � Setup Colors for bottom right edges
			theColor = UGAColorRamp::GetColor(colorRamp_Gray6);
			::RGBForeColor ( &theColor );
			UGraphicsUtilities::BottomRightSide ( &localFrame, 1, 1, 0, 0 );

			// � Draw the actual arrow which we only do this when drawing in color
			theColor = UGAColorRamp::GetWhiteColor();
			::RGBForeColor ( &theColor );
			DrawPopupArrow ( localFrame );
		}
	}
	
}	//	LGAMiniArrowPopup::DrawPopupHilited


//-------------------------------------------------------------------------------------
// LGAMiniArrowPopup::DrawPopupArrow
//-------------------------------------------------------------------------------------

void
LGAMiniArrowPopup::DrawPopupArrow	(	Rect		&inRect )
{

	// � Loop around four times to draw the four rows of the arrow
	for ( SInt16 index = 0; index < 4; index++ )
	{
		::MoveTo ( 	inRect.left + (3 + index), inRect.top + (3 + index) );
		::LineTo ( inRect.right - (4 + index), inRect.top + (3 + index) );
	}
	
}	//	LGAMiniArrowPopup::DrawActualArrow


#pragma mark -
#pragma mark === MOUSE TRACKING

//=====================================================================================
// �� MOUSE TRACKING
//-------------------------------------------------------------------------------------
// LGAMiniArrowPopup::TrackHotSpot
//-------------------------------------------------------------------------------------
//

Boolean
LGAMiniArrowPopup::TrackHotSpot (	SInt16		inHotSpot,
												Point 	inPoint,
												SInt16	/* inModifiers */ )
{

	// � We only want the popup menu to appear if the mouse went down
	// in the our hot spot which is the popup portion of the control
	// not the label area
	if ( PointInHotSpot ( inPoint, inHotSpot ))
	{
		// � Get things started off on the right foot
		Boolean		currInside = true;
		Boolean		prevInside = false;
		HotSpotAction ( inHotSpot, currInside, prevInside );

		// � We skip the normal tracking that is done in the control as
		// the call to PopupMenuSelect will take control of the tracking
		// once the menu is up
		// � Now we need to handle the display of the actual popup menu
		// we start by setting up some values that we will need
		SInt16	menuID = 0;
		SInt16 menuItem = GetValue ();
		SInt16	currItem = IsPulldownMenu () ? 1 : GetValue ();
		Point popLoc;
		GetPopupMenuPosition ( popLoc );
		
		// � Call our utility function which handles the display of the menu
		// menu is disposed of inside this function
		HandlePopupMenuSelect ( popLoc, currItem, menuID, menuItem );
		
		// � Setup the current item field with the newly selected item this
		// will trigger a broadcast of the new value
		if ( menuItem > 0 )
			SetValue ( menuItem );
			
		// � Make sure that we get the HotSpotAction called one last time
		HotSpotAction ( inHotSpot, false, true );
		
		return menuItem > 0;
	}
	else
		return false;
		
}	//	LGAMiniArrowPopup::TrackHotSpot


//-------------------------------------------------------------------------------------
// LGAMiniArrowPopup::HotSpotAction
//-------------------------------------------------------------------------------------
//
//	We have overridden this method from our superclass as we are not interested in the
//		control mode we always treat the popup as a button regardless of what the user has
//		set the control mode too

void
LGAMiniArrowPopup::HotSpotAction	(	SInt16		/* inHotSpot */,
									Boolean	inCurrInside,
									Boolean	inPrevInside )
{

	// � If the mouse moved in or out of the hot spot handle the
	// hiliting of the control, NOTE: for the popup we ignore the
	// control mode as it does not make any sense to have a popup
	// that behaves like either a radio button or a switch
	if ( inCurrInside != inPrevInside )
	{
		// � Toggle the hilite state of the control
		SetHiliteState ( inCurrInside );
	}
		
}	//	LGAMiniArrowPopup::HotSpotAction


//-------------------------------------------------------------------------------------
// LGAMiniArrowPopup::HotSpotResult
//-------------------------------------------------------------------------------------
//
//	We have overridden this method so that the hiliting of the popup will get turned off

void
LGAMiniArrowPopup::HotSpotResult	(	SInt16 inHotSpot )
{
	
	// � Make sure that by default we turn the hiliting off
	HotSpotAction ( inHotSpot, false, true );
		
}	//	LGAMiniArrowPopup::HotSpotResult


#pragma mark -
#pragma mark === POPUP MENU HANDLING

//=====================================================================================
// �� POPUP MENU HANDLING
//-------------------------------------------------------------------------------------
// LGAMiniArrowPopup::HandlePopupMenuSelect
//-------------------------------------------------------------------------------------

void
LGAMiniArrowPopup::HandlePopupMenuSelect	(	Point		inPopupLoc,
												SInt16		inCurrentItem,
												SInt16		&outMenuID,
												SInt16		&outMenuItem )
{
	
	MenuHandle	menuH = GetMacMenuH ();
	ThrowIfNil_ ( menuH );
	if ( menuH )
	{
		// � Save off the current system font family and size
		SInt16 saveFont = ::LMGetSysFontFam ();
		SInt16 saveSize = ::LMGetSysFontSize ();
		
		// � Enclose this all in a try catch block so that we can
		// at least reset the system font if something goes wrong
		try 
		{

			// � Handle the actual insertion into the hierarchical menubar
			::InsertMenu ( menuH, hierMenu );
			
			// � Reconfigure the system font so that the menu will be drawn in
			// our desired font and size
			FocusDraw ();
			{				
				ResIDT	textTID = GetTextTraitsID ();
				TextTraitsH traitsH = UTextTraits::LoadTextTraits ( textTID );
				if ( traitsH ) 
				{
					::LMSetSysFontFam ( (**traitsH).fontNumber );
					::LMSetSysFontSize ( (**traitsH).size );
					::LMSetLastSPExtra ( -1L );
				}
			}

			// � Before we display the menu we need to make sure that we have the
			// current item marked in the menu. NOTE: we do NOT use the current
			// item that has been passed in here as that always has a value of one
			// in the case of a pulldown menu
			SetupCurrentMenuItem ( menuH, GetValue () );
	
			// � Then we call PopupMenuSelect and wait for it to return
			SInt32 result = ::PopUpMenuSelect ( 	menuH,
															inPopupLoc.v,
															inPopupLoc.h,
															inCurrentItem );
			
			// � Then we extract the values from the returned result
			// these are then passed back out to the caller
			outMenuID = HiWord ( result );
			outMenuItem = LoWord ( result );
			
		}
		catch (...) 
		{
			// ignore errors�
		}

		// � Restore the system font
		::LMSetSysFontFam ( saveFont );
		::LMSetSysFontSize ( saveSize );
		::LMSetLastSPExtra ( -1L );

		// � Finally get the menu removed
		::DeleteMenu ( GetPopupMenuResID ());
		
	}
	
}	//	LGAMiniArrowPopup::HandlePopupMenuSelect


#pragma mark -
#pragma mark === MISCELLANEOUS

//=====================================================================================
// �� MISCELLANEOUS
//-------------------------------------------------------------------------------------
// LGAMiniArrowPopup::LoadResourceMenu
//-------------------------------------------------------------------------------------
//

void
LGAMiniArrowPopup::LoadResourceMenu	(	MenuHandle	inMenu ) const
{

	// � Get all of the resources of a specific typoe loaded into the
	// menu passed in
	if ( UseResourceMenu () && inMenu )
	{
		::InsertResMenu ( inMenu, GetResourceMenuType (), 0 );
	}
		
}	//	LGAMiniArrowPopup::LoadResourceMenu


