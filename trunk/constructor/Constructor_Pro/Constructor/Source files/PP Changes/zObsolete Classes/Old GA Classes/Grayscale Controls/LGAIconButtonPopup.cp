// Copyright й2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyrightй 1995 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< е >>-----------------------------------------

FILE:					LGAIconButtonPopup.cp

CLASSES:				LGAIconButtonPopup

DESCRIPTION:		Implementation file for LGAIconButtonPopup

AUTHOR:				Robin Mair

CREATION DATE :	96.06.03

CHANGE HISTORY :

		97.04.20		rtm	Removed the CreateFromStream method.
		97.04.20		rtm	Fixed a bug in SetMacMenuH where the resource ID was being
								extracted from the menu handle without checking to make sure
								the menu handle was not nil.
		97.03.07		rtm	Dixed a bug in SetMacMenuH where the menu hande could be changed
								but the menu resource ID would not be synched up with it.
		97.01.24		rtm	Added a copy constructor.
		96.01.20		rtm	Added a parameterized constructor so that the pane can be built
								procedurally.
		97.01.20		rtm	Implemented support for reading in a list of command numbers
								that are associated with the popup's menu, this also includes
								a function for getting a command number from an index. This
								also required a change to SetPopupMenuResID so that the command
								numbers will be reread if the res ID is changed.
		96.12.08		rtm	Made a small change to the HandlePopupMenuSelect method where
								we ow call InsertMenu before we twiddle the low mem font
								globals, Apple has a snippet that says this is the right thing 
								to do so like the good citizens we are we do it!
		96.10.19		rtm	Fixed a small bug where there was a case that the arrow
								would not get drawnon a b&W screen.
		96.10.19		rtm	Changed GetMacMenuH so that is no longer declared as 'const'.
		96.10.19		rtm	Added a call to LGAIconButton::FinishCreateSelf in this 
								classes FinishCreateSelf method.
		96.10.17		rtm	Added a new setter SetPopupMenuResID that can be called to
								change the resource ID for the menu associated with the
								popup.  Calling it will result in the old cached handle
								being disposed and the new one to be loaded and cached.
		96.10.17		rtm	Changed the whole way that LGAPopup works, it now uses the
								caching approach that was being used in the cached subclasses
								this means that we have ben able to get rid of the cached
								subclasses as they are redundant.  This class now detaches
								the menu handle once it has been loaded so that it is removed
								from the resource map.  This allows multiple popups to be built
								from the same menu resource without causing any interference
								between themselves.  A new method called SetMacMenuH has been
								added that is used to set the menu handle, this method takes
								care of the detaching of the resource.  This method also
								calls SetPopupMinMaxValues in order to get the min and max
								values for the  poup setup.
		96.10.09		rtm	Moved the setup of teh popup's min and max values from the
								stream constructor to the FinishCreateSelf method.
		96.10.08		rtm	Got rid of the override of DrawIconButton by overriding the
								DrawSelf method instead as we no longer have DrawIconButton.
		96.10.04		rtm	Replaced all SetCPixel calls with a new PaintColorPixel call
								that has been added to UGraphicsUtilities. This should get
								around the problems with using SetCPixel.
		96.09.25		rtm	Fixed a bug in CalcLocalIconRect where it was not calculating
								rectangle appropriately for a mini icon.
		96.09.23		rtm	Fixed some boundary cases that should never happen where the
								correct rendering was not being done when disabled.
		96.09.23		rtm	Got rid of the GetCurrentItem and SetCurrentItem methods and
								now use getValue and SetValue instead.
		96.09.13		rtm	Changed the name of GetPopupMenu to LoadPopupMenuH and added
								a GetMacMenuH method to be more consistent with PowerPlant.
		96.09.04		rtm	PowerPlant conformance changes.
		96.08.26		rtm	Changed the implementation of th popup tracking so that it
								is the same as that used in GAPopup.
		96.06.25		rtm	Got rid of an extra call to Draw() that was resulting in the
								button not hiliting correctly on a B&W screen.
		96.06.15		rtm	Completely re-wrote how this works to get around the problem
								of having multiple popups that are using the same menu resource
								now the menu is only loaded when needed and never cached.
		96.06.03		rtm	Initial creation of file

---------------------------------------<< е >>-----------------------------------------
*/

#ifdef PowerPlant_PCH
	#include PowerPlant_PCH
#endif

// е GRAYSCALE HEADERS
#include <LGAIconButtonPopup.h>
#include <UGraphicsUtilities.h>
#include	<UGAColorRamp.h>

// е POWERPLANT HEADERS
#include <LStream.h>
#include <PP_Messages.h>
#include <UDrawingState.h>
#include <UDrawingUtils.h>
#include <LString.h>
#include <UTextTraits.h>

// е TOOLBOX HEADERS
#include <LowMem.h>
#include <ToolUtils.h>


//=====================================================================================
// CONSTANTS
//=====================================================================================


const	UInt16	gsPopup_SmallMark			=	'е';	// Mark used for small font popups

//=====================================================================================
// DEFINES
//=====================================================================================

#pragma mark === CLASS: LGAIconButtonPopup

//=====================================================================================
// CLASS:	LGAIconButtonPopup
//=====================================================================================

#pragma mark -
#pragma mark === INITIALIZATION

//=====================================================================================
// ее INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LGAIconButtonPopup::LGAIconButtonPopup					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Default constructor

LGAIconButtonPopup::LGAIconButtonPopup ()
{

	mControlMode = controlMode_Button;
	mPopupMenuResID = 0;
	mPulldown = true;
	mTextTraitsID = 0;
	mCachedMenuH = nil;
	mNumCommands = 0;
	mCommandNums = nil;
	
}	//	LGAIconButtonPopup::LGAIconButtonPopup


//-------------------------------------------------------------------------------------
// LGAIconButtonPopup::LGAIconButtonPopup					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Copy constructor

LGAIconButtonPopup::LGAIconButtonPopup (	const LGAIconButtonPopup &inOriginal )
									:	LGAIconButton ( inOriginal )
{

	mControlMode = inOriginal.mControlMode;
	mPopupMenuResID = inOriginal.mPopupMenuResID;
	mPulldown = inOriginal.mPulldown;
	mTextTraitsID = inOriginal.mTextTraitsID;
	
	// е We cannot simply copy the reference to the cached menu instead
	// we set it to nil and rely on the convention that the caller has
	// to also call FinishCreateSelf which is where these value will be set
	mCachedMenuH = nil;
	mNumCommands = inOriginal.mNumCommands;
	mCommandNums = nil;
	
}	//	LGAIconButtonPopup::LGAIconButtonPopup


//-------------------------------------------------------------------------------------
// LGAIconButtonPopup::LGAIconButtonPopup					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Stream constructor

LGAIconButtonPopup::LGAIconButtonPopup (	LStream	*inStream ) 
															: LGAIconButton ( inStream )
{

	SInt16	currItem;
	
	// е The hilite state always needs to start out false
	SetHiliteState ( false );
	
	// е Read in the data from the stream
	*inStream >> mPopupMenuResID;
	*inStream >> currItem;
	if ( currItem > 0 )
		mValue = currItem;
	else
		mValue = 1;
	*inStream >> mTextTraitsID;
	
	// е Setup some default values for some of the fields we won't be 
	// allowing the user to modify
	mCachedMenuH = nil;
	mControlMode = controlMode_Button;
	mIconPlacement = iconPosition_LeftCenter;
	mPulldown = true;
	mNumCommands = 0;
	mCommandNums = nil;

	// е NOTE: at this point in time the menu handle has not yet been
	// cached as that takes place in FinishCreateSelf.
	
}	//	LGAIconButtonPopup::LGAIconButtonPopup


//-------------------------------------------------------------------------------------
// LGAIconButtonPopup::LGAIconButtonPopup					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Parameterized constructor

LGAIconButtonPopup::LGAIconButtonPopup (	const SPaneInfo			&inPaneInfo,
														const SControlInfo		&inControlInfo,
														const SIconButtonInfo	&inIconButtonInfo,
														ResIDT						inPopupMenuResID,
														SInt16							inCurrentItem,
														ResIDT						inTextTraitsID ) 
												: LGAIconButton ( inPaneInfo, 
																		inControlInfo,
																		inIconButtonInfo )
{

	// е The hilite state always needs to start out false
	SetHiliteState ( false );
	
	// е Read in the data from the stream
	mPopupMenuResID = inPopupMenuResID;
	if ( inCurrentItem > 0 )
		mValue = inCurrentItem;
	else
		mValue = 1;
	mTextTraitsID = inTextTraitsID;
	
	// е Setup some default values for some of the fields we won't be 
	// allowing the user to modify
	mCachedMenuH = nil;
	mControlMode = controlMode_Button;
	mIconPlacement = iconPosition_LeftCenter;
	mPulldown = true;
	mNumCommands = 0;
	mCommandNums = nil;

	// е NOTE: at this point in time the menu handle has not yet been
	// cached as that takes place in FinishCreateSelf.
	
}	//	LGAIconButtonPopup::LGAIconButtonPopup


//-------------------------------------------------------------------------------------
// LGAIconButtonPopup::~LGAIconButtonPopup				---<<е DESTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LGAIconButtonPopup::~LGAIconButtonPopup ()
{

	// е Get rid of the menu handle
	if ( mCachedMenuH )
	{
		::DisposeMenu ( mCachedMenuH );
		mCachedMenuH = nil;
	}
	
	// е Get rid of the command numbers if we have them
	if ( mCommandNums )
	{
		::DisposeHandle ( (Handle)mCommandNums );
		mCommandNums = nil;
	}
	
}	//	LGAIconButtonPopup::~LGAIconButtonPopup


//-------------------------------------------------------------------------------------
// LGAIconButtonPopup::FinishCreateSelf
//-------------------------------------------------------------------------------------

void
LGAIconButtonPopup::FinishCreateSelf ()
{

	// е Make sure we call our superclass
	LGAIconButton::FinishCreateSelf ();
	
	// е Even if the user has specified that we use a certain icon size
	// which is used to determine the bevel width for the button if the
	// button is below a certain height we will force it to a single
	// pixel bevel
	if ( mFrameSize.height <= 16 )
		SetBevelWidth ( 1, false );
		
	// е Make sure that the correct values are setup for the popup
	SetMacMenuH ( LoadPopupMenuH () );

	// е read in any command numbers associated with the popup's menu
	ReadCommandNumbers ();
	
}	//	LGAIconButtonPopup::FinishCreateSelf

	
#pragma mark -
#pragma mark === ACCESSORS

//=====================================================================================
// ее ACCESSORS
//-------------------------------------------------------------------------------------
// LGAIconButtonPopup::LoadPopupMenuH
//-------------------------------------------------------------------------------------

MenuHandle
LGAIconButtonPopup::LoadPopupMenuH () const
{

	return ::GetMenu ( mPopupMenuResID );
	
}	//	LGAIconButtonPopup::LoadPopupMenuH


//-------------------------------------------------------------------------------------
// LGAIconButtonPopup::GetCurrentItemTitle
//-------------------------------------------------------------------------------------

void
LGAIconButtonPopup::GetCurrentItemTitle ( Str255 outItemTitle )
{
	// е Load the popup menu and then find the title
	MenuHandle	popupMenu = GetMacMenuH ();
	if ( popupMenu )
	{
		::GetMenuItemText ( popupMenu, GetValue (), outItemTitle );
	}
	
}	//	LGAIconButtonPopup::GetCurrentItemTitle


//-------------------------------------------------------------------------------------
// LGAIconButtonPopup::ReadCommandNumbers
//-------------------------------------------------------------------------------------

void
LGAIconButtonPopup::ReadCommandNumbers ()
{

	// е Load the Mcmd resource, if we have one then we can proceed to extract
	// the menu command table
	SInt16**	theMcmdH = (SInt16**) ::GetResource ( 'Mcmd', mPopupMenuResID );
	if ( theMcmdH != nil ) 
	{
		mNumCommands = (*theMcmdH)[0];
		if ( mNumCommands > 0 ) 
		{
			
			// е Our command numbers list is the same as the 'Mcmd'
			// resource without the 2-byte count at the top. So we
			// can reuse the 'Mcmd' resource Handle by detaching it,
			// shifting the command numbers down by 2-bytes, and
			// resizing it.
			::DetachResource ( (Handle) theMcmdH );
			mCommandNums = (SInt32**) theMcmdH;
			SInt32	commandsSize = mNumCommands * sizeof ( SInt32 );
			::BlockMoveData ( *theMcmdH + 1, *mCommandNums, commandsSize);
			::SetHandleSize ( (Handle) mCommandNums, commandsSize );
			
		}
	}
	
}	//	LGAIconButtonPopup::ReadCommandNumbers


//-------------------------------------------------------------------------------------
// LGAIconButtonPopup::CommandFromIndex
//-------------------------------------------------------------------------------------

CommandT
LGAIconButtonPopup::CommandFromIndex (	SInt16	inIndex )
{

	SInt32	theCommand = cmd_UseMenuItem;
	if ( inIndex <= mNumCommands ) 
	{
		theCommand = (*mCommandNums)[inIndex-1];
	}
	
	if ( theCommand == cmd_UseMenuItem ) 
	{
	
		// Command number is a special flag, either because it
		// was set that way in the command table or because the
		// index was greater than the command table size.
		// In this case, we return a synthetic command number,
		// which has the MENU id and item number embedded
		theCommand = SyntheticCommandFromIndex ( inIndex );
	}
	
	return theCommand;
	
}	//	LGAIconButtonPopup::CommandFromIndex


//-------------------------------------------------------------------------------------
// LGAIconButtonPopup::SyntheticCommandFromIndex
//-------------------------------------------------------------------------------------

CommandT
LGAIconButtonPopup::SyntheticCommandFromIndex ( SInt16	inIndex ) const
{

	return (-(((SInt32)mPopupMenuResID) << 16) - inIndex);
	
}	//	LGAIconButtonPopup::SyntheticCommandFromIndex


//-------------------------------------------------------------------------------------
// LGAIconButtonPopup::GetMenuFontSize
//-------------------------------------------------------------------------------------

SInt16
LGAIconButtonPopup::GetMenuFontSize () const
{

	// е Get the font size for the popup so we can figure out the correct
	// mark to apply
	StTextState			theTextState;

	ResIDT	textTID = GetTextTraitsID ();
	TextTraitsH traitsH = UTextTraits::LoadTextTraits ( textTID );
	if ( traitsH ) 
	{
		return (**traitsH).size;
	}
	
	// е Default size is 9 point
	return 9;
	
}	//	LGAIconButtonPopup::GetMenuFontSize


//-------------------------------------------------------------------------------------
// LGAIconButtonPopup::GetPopupMenuPosition
//-------------------------------------------------------------------------------------

void
LGAIconButtonPopup::GetPopupMenuPosition	( Point	&outPopupLoc )
{
	
	// е Get the local popup button frame, this will be correctly setup depending
	// on whether the button is arrow only or not
	Rect	popupRect;
	CalcLocalFrameRect ( popupRect );
	
	// е Setup the vertical value based on whether it is a pulldown or popup menu
	outPopupLoc.v = IsPulldownMenu() ? popupRect.bottom : popupRect.top;

	// е If the popup is setup to have a pulldown menu then calculate the
	// apropriate location for the popup menu to appear based on whether only
	// the arrow button is being displayed or the entire popup button is visible
	// If it is a normal popup then set up the horizontal vaue appropriately
	outPopupLoc.h = IsPulldownMenu () ? popupRect.left + 2 : popupRect.left;
	
	// е Because PopMenuSelect needs the location for the popup menu global 
	// coordinates we need to convert the location
	LocalToPortPoint ( outPopupLoc );
	PortToGlobalPoint ( outPopupLoc );
	
}	//	LGAIconButtonPopup::GetPopupMenuPosition


//-------------------------------------------------------------------------------------
// LGAIconButtonPopup::CalcLocalIconRect
//-------------------------------------------------------------------------------------

void
LGAIconButtonPopup::CalcLocalIconRect ( Rect &outRect ) const
{

	// е Setup the rectangle based on the location and size of the icon
	outRect.top = mIconLocation.v;
	outRect.left = mIconLocation.h;
	outRect.bottom = mIconLocation.v + mSizeSelector;
	outRect.right = outRect.left + (mSizeSelector == 12 ? 16 : mSizeSelector);
	
	// е If we are hilited and the user has requested that the icon be offset
	// then handle it now
	// 96.08.04		rtm 	This changed to fix a bug found by Eric Shapiro this
	//							is now back to the original code
	if ( IsHilited () && IsEnabled () && mOffsetIconOnHilite )
		::OffsetRect( &outRect, 1, 1 );
		
}	//	LGAIconButtonPopup::CalcLocalIconRect


//-------------------------------------------------------------------------------------
// LGAIconButtonPopup::SetMacMenuH
//-------------------------------------------------------------------------------------

void
LGAIconButtonPopup::SetMacMenuH (	MenuHandle	inMenuHandle )
{

	// е If we have an existing handle make sure we get rid of
	// it first before we add the new one
	if ( mCachedMenuH && mCachedMenuH != inMenuHandle )
	{
		// е Get rid of the old cached menu
		::DisposeMenu ( mCachedMenuH );
		
		// е Add the new menu handle
		mCachedMenuH = inMenuHandle;
	}
	else if ( mCachedMenuH == nil )
	{
		// е Add the new menu handle
		mCachedMenuH = inMenuHandle;
	}
	
	// е We also detach the resource so we don't run into problems
	// for cases where there are multiple popups referencing the same
	// menu resource
	if ( inMenuHandle )
	{
		::DetachResource ( (Handle)inMenuHandle );
		
		// е Make sure we synch up the menu resource ID with that of the
		// menuhandle being handed to us
#if PP_Target_Carbon
		mPopupMenuResID = GetMenuID(inMenuHandle);
#else
		mPopupMenuResID = (**inMenuHandle).menuID;
#endif
	}
	
	// е The final step is to setup the popups min and max values
	SetPopupMinMaxValues ();
	
}	//	LGAIconButtonPopup::SetMacMenuH


//-------------------------------------------------------------------------------------
// LGAIconButtonPopup::SetPopupMenuResID
//-------------------------------------------------------------------------------------

void
LGAIconButtonPopup::SetPopupMenuResID (	ResIDT	inResID )
{

	// е Only make the change if we have to
	if ( mPopupMenuResID != inResID )
	{
		mPopupMenuResID = inResID;
		
		// е Now we need to load the new menu and get it
		// saved into the cache
		SetMacMenuH ( LoadPopupMenuH () );
		
		// е Because the new menu might be shorter than the old one
		// we need to make sure the the current item is not off the
		// end of the menu, if it is we simply set the first item
		if ( GetValue () > GetMaxValue () )
			SetValue ( 1 );
			
		// е Make sure we also read in any command numbers associated
		// with the popup's menu
		ReadCommandNumbers ();
	}

}	//	LGAIconButtonPopup::SetPopupMenuResID


//-------------------------------------------------------------------------------------
// LGAIconButtonPopup::SetValue
//-------------------------------------------------------------------------------------

void
LGAIconButtonPopup::SetValue	(	SInt32	inValue )
{

	// е Setup the current item if needed
	if ( GetValue () != inValue )
	{
	
		// е Get the current item setup in the menu
		MenuHandle menuH = GetMacMenuH ();
		if ( menuH )
		{
			SetupCurrentMenuItem ( menuH, inValue );
		}

		// е Call our superclass to handle the setting of the value
		// NOTE: we specifically do not call the LGAIconButton superclass
		// but instead its superclass as we don't want the popup to become
		// selected when we set the value which is used differently for a popup
		LControl::SetValue ( inValue );
	
	}
	
}	//	LGAIconButtonPopup::SetValue


//-------------------------------------------------------------------------------------
// LGAIconButtonPopup::SetupCurrentMenuItem
//-------------------------------------------------------------------------------------

void
LGAIconButtonPopup::SetupCurrentMenuItem (	MenuHandle	inMenuH,
															SInt16			inCurrentItem )
{
	
	// е If the current item has changed then make it so, this
	// also involves removing the mark from any old
	if ( inMenuH )
	{
		if ( GetValue () != inCurrentItem )
		{
			SInt16		oldItem = GetValue ();
			
			// е Remove the old marked menu item
			::SetItemMark ( inMenuH, oldItem, 0 );
		}
		
		// е Always make sure item is marked
		UInt16	mark = GetMenuFontSize () < 12 ? gsPopup_SmallMark : checkMark;
		::SetItemMark ( 	inMenuH, 
								inCurrentItem, 
								mark );
	}
	
}	//	LGAIconButtonPopup::SetupCurrentMenuItem


//-------------------------------------------------------------------------------------
// LGAIconButtonPopup::SetPopupMinMaxValues
//-------------------------------------------------------------------------------------

void
LGAIconButtonPopup::SetPopupMinMaxValues ()
{

	// е We will have to synthesize these values as we dont' have a real
	// control that we are talking too
	MenuHandle	popupMenu = GetMacMenuH ();
	if ( popupMenu )
	{
		// е Setup the values
		mMinValue = 1;
		mMaxValue = ::CountMenuItems ( popupMenu );
	}
	
}	//	LGAIconButtonPopup::SetPopupMinMaxValues


//-------------------------------------------------------------------------------------
// LGAIconButtonPopup::SetTextTraitsID
//-------------------------------------------------------------------------------------
//

void			
LGAIconButtonPopup::SetTextTraitsID	( ResIDT inTextTraitID )
{
	//  е If the ID is not currently set to the value passed in the set it
	if ( GetTextTraitsID () != inTextTraitID )
		mTextTraitsID = inTextTraitID;
		
}	//	LGAIconButtonPopup::SetTextTraitsID


#pragma mark -
#pragma mark === MOUSE TRACKING

//=====================================================================================
// ее MOUSE TRACKING
//-------------------------------------------------------------------------------------
// LGAIconButtonPopup::TrackHotSpot
//-------------------------------------------------------------------------------------
//

Boolean
LGAIconButtonPopup::TrackHotSpot (	SInt16		inHotSpot,
												Point 	inPoint,
												SInt16		/* inModifiers */ )
{

	// е We only want the popup menu to appear if the mouse went down
	// in the our hot spot which is the popup portion of the control
	if ( PointInHotSpot ( inPoint, inHotSpot ))
	{
		// е Get things started off on the right foot
		Boolean		currInside = true;
		Boolean		prevInside = false;
		HotSpotAction ( inHotSpot, currInside, prevInside );

		// е We skip the normal tracking that is done in the control as
		// the call to PopupMenuSelect will take control of the tracking
		// once the menu is up
		// е Now we need to handle the display of the actual popup menu
		// we start by setting up some values that we will need
		SInt16	menuID = 0;
		SInt16 menuItem = GetValue ();
		SInt16	currItem = IsPulldownMenu () ? 1 : GetValue ();
		Point popLoc;
		GetPopupMenuPosition ( popLoc );
		
		// е Call our utility function which handles the display of the menu
		// menu is disposed of inside this function
		HandlePopupMenuSelect ( popLoc, currItem, menuID, menuItem );
		
		// е Setup the current item field with the newly selected item this
		// will trigger a broadcast of the new value
		if ( menuItem > 0 )
			SetValue ( menuItem );
			
		// е Make sure that we get the HotSpotAction called one last time
		HotSpotAction ( inHotSpot, false, true );
		
		return menuItem > 0;
	}
	else
		return false;
		
}	//	LGAIconButtonPopup::TrackHotSpot



//-------------------------------------------------------------------------------------
// LGAIconButtonPopup::HotSpotAction
//-------------------------------------------------------------------------------------
//
//	We have overridden this method from our superclass as we are not interested in the
//		control mode we always treat the popup as a button regardless of what the user has
//		set the control mode too
//

void
LGAIconButtonPopup::HotSpotAction	(	SInt16		/* inHotSpot */,
												Boolean	inCurrInside,
												Boolean	inPrevInside )
{

	// е If the mouse moved in or out of the hot spot handle the
	// hiliting of the control, NOTE: for the popup we ignore the
	// control mode as it does not make any sense to have a popup
	// that behaves like either a radio button or a switch
	if ( inCurrInside != inPrevInside )
	{
		// е Toggle the hilite state of the control and get things redrawn
		SetHiliteState ( inCurrInside );
	}
		
}	//	LGAIconButtonPopup::HotSpotAction


//-------------------------------------------------------------------------------------
// LGAIconButtonPopup::HotSpotResult
//-------------------------------------------------------------------------------------
//
//	We have overridden this method so that we can suppress the additional call that
// 	was being made to BroadcastValueMessage due to the fact that it was both called
//		when we set the new menu item and when the muse button was released as part of
//		the general behavior of its superclass

void
LGAIconButtonPopup::HotSpotResult	(	SInt16 inHotSpot )
{
	
	// е Make sure that by default we turn the hiliting off
	HotSpotAction ( inHotSpot, false, true );
		
}	//	LGAIconButtonPopup::HotSpotResult


//-------------------------------------------------------------------------------------
// LGAIconButtonPopup::HandlePopupMenuSelect
//-------------------------------------------------------------------------------------

void
LGAIconButtonPopup::HandlePopupMenuSelect	(	Point			inPopupLoc,
															SInt16			inCurrentItem,
															SInt16			&outMenuID,
															SInt16			&outMenuItem )
{
	
	// е Load the menu from the resource
	MenuHandle	menuH = GetMacMenuH ();
	ThrowIfNil_ ( menuH );
	if ( menuH )
	{
		// е Save off the current system font family and size
		SInt16 saveFont = ::LMGetSysFontFam ();
		SInt16 saveSize = ::LMGetSysFontSize ();
		
		// е Enclose this all in a try catch block so that we can
		// at least reset the system font if something goes wrong
		try 
		{

			// е Handle the actual insertion into the hierarchical menubar
			::InsertMenu ( menuH, hierMenu );
			
			// е Reconfigure the system font so that the menu will be drawn in
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

			// е Before we display the menu we need to make sure that we have the
			// current item marked in the menu. NOTE: we do NOT use the current
			// item that has been passed in here as that always has a value of one
			// in the case of a pulldown menu
			SetupCurrentMenuItem ( menuH, GetValue () );
	
			// е Then we call PopupMenuSelect and wait for it to return
			SInt32 result = ::PopUpMenuSelect ( 	menuH,
															inPopupLoc.v,
															inPopupLoc.h,
															inCurrentItem );
			
			// е Then we extract the values from the returned result
			// these are then passed back out to the caller
			outMenuID = HiWord ( result );
			outMenuItem = LoWord ( result );
			
		}
		catch (...) 
		{
			// ignore errors╔
		}

		// е Restore the system font
		::LMSetSysFontFam ( saveFont );
		::LMSetSysFontSize ( saveSize );
		::LMSetLastSPExtra ( -1L );

		// е Finally get the menu removed
		::DeleteMenu ( GetPopupMenuResID ());
	}
	
}	//	LGAIconButtonPopup::HandlePopupMenuSelect


#pragma mark -
#pragma mark === DRAWING

//=====================================================================================
// ее DRAWING
//-------------------------------------------------------------------------------------
// LGAIconButtonPopup::DrawSelf
//-------------------------------------------------------------------------------------

void
LGAIconButtonPopup::DrawSelf	()
{

	StColorPenState::Normalize ();
	
	// е Get the control drawn in its various states by calling our superclass
	LGAIconButton::DrawSelf ();

	// е Always get the popup indicator drawn
	DrawPopupIndicator ();
	
}	//	LGAIconButtonPopup::DrawSelf


//-------------------------------------------------------------------------------------
// LGAIconButtonPopup::DrawPopupIndicator
//-------------------------------------------------------------------------------------

void
LGAIconButtonPopup::DrawPopupIndicator	()
{
	
	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );

	// е Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( localFrame );
	SInt16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		// е BLACK & WHITE
		if ( depth < 4 )
		{
			// е We only draw the indicator when we are not hilited as the
			// entire button including the indicator is inverted for the
			// hilited state
			// е We draw the indicator in black when normal, white when hilited
			// and with a gray pattern when the button is dimmed
			RGBColor theColor = IsHilited () && IsEnabled () ? UGAColorRamp::GetWhiteColor () :
												UGAColorRamp::GetBlackColor();
			::RGBForeColor ( &theColor );
			if ( !IsEnabled ())
			{
				Pattern thePattern;
				UQDGlobals::GetGrayPat(&thePattern);
				::PenPat ( &thePattern );
			}
			
			// е The indicator is drawn in the bottom right corner of the button
			::MoveTo ( localFrame.right - (GetBevelWidth () + 3 + 4 ),
							localFrame.bottom - (GetBevelWidth () + 3 + 3) );
			::LineTo ( localFrame.right - (GetBevelWidth () + 3 ),
							localFrame.bottom - (GetBevelWidth () + 3 + 3) );
			::MoveTo ( localFrame.right - (GetBevelWidth () + 3 + 3 ),
							localFrame.bottom - (GetBevelWidth () + 3 + 2) );
			::LineTo ( localFrame.right - (GetBevelWidth () + 4 ),
							localFrame.bottom - (GetBevelWidth () + 3 + 2) );
#if __PowerPlant__ < 0x02218000
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - (GetBevelWidth () + 3 + 2 ),
								localFrame.bottom - (GetBevelWidth () + 3 + 1),
								&(IsHilited () && IsEnabled () ? UGAColorRamp::GetWhiteColor() :
										 UGAColorRamp::GetBlackColor()) );
#else
			RGBColor theWhiteColor = UGAColorRamp::GetWhiteColor();
			RGBColor theBlackColor = UGAColorRamp::GetBlackColor();

			UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - (GetBevelWidth () + 3 + 2 ),
								localFrame.bottom - (GetBevelWidth () + 3 + 1),
								&(IsHilited () && IsEnabled () ? theWhiteColor : theBlackColor) );
#endif
		}
		// е COLOR
		else
		{
			// е We draw the indicator in black when normal gray 7 when dimmed
			// and white when hilited
			RGBColor	indicatorColor = UGAColorRamp::GetBlackColor();
			if ( !IsEnabled () || !IsActive ())
				 indicatorColor = UGAColorRamp::GetColor(7);
			if ( IsHilited ())
				indicatorColor = IsEnabled () ? UGAColorRamp::GetWhiteColor() :
																		UGAColorRamp::GetColor(7);
			::RGBForeColor ( &indicatorColor );

			// е The indicator is drawn in the bottom right corner of the button
			::MoveTo ( localFrame.right - (GetBevelWidth () + 3 + 5 ),
							localFrame.bottom - (GetBevelWidth () + 3 + 3) );
			::LineTo ( localFrame.right - (GetBevelWidth () + 3 + 1 ),
							localFrame.bottom - (GetBevelWidth () + 3 + 3) );
			::MoveTo ( localFrame.right - (GetBevelWidth () + 3 + 4 ),
							localFrame.bottom - (GetBevelWidth () + 3 + 2) );
			::LineTo ( localFrame.right - (GetBevelWidth () + 3 + 2 ),
							localFrame.bottom - (GetBevelWidth () + 3 + 2) );
			UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - (GetBevelWidth () + 3 + 3 ),
								localFrame.bottom - (GetBevelWidth () + 3 + 1),
								&indicatorColor );
		}	
	}
	
}	//	LGAIconButtonPopup::DrawPopupIndicator


//-------------------------------------------------------------------------------------
// LGAIconButtonPopup::DrawIconButtonDimmedColor
//-------------------------------------------------------------------------------------

void
LGAIconButtonPopup::DrawIconButtonDimmedColor	()
{
	
	StColorPenState::Normalize ();
	
	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );
	SInt16	bevelWidth = GetBevelWidth ();
	
	// ее FRAME BUTTON
	RGBColor theColor = UGAColorRamp::GetColor(5);
	::RGBForeColor ( &theColor );
	UGraphicsUtilities::TopLeftSide ( &localFrame, 0, 0, 0, 0 );				

	
	bevelWidth == 3 ? theColor = UGAColorRamp::GetColor(7) : theColor = UGAColorRamp::GetColor(6);
	::RGBForeColor (&theColor);
	UGraphicsUtilities::BottomRightSide ( &localFrame, 1, 1, 0, 0 );				


	// е TOP RIGHT
	if ( bevelWidth == 3 )
	{
		theColor = UGAColorRamp::GetColor(6);
		UGraphicsUtilities::PaintColorPixel ( localFrame.right - 1, 
								localFrame.top, 
								&theColor );
			
		// е BOTTOM LEFT
		theColor = UGAColorRamp::GetColor(6);
		UGraphicsUtilities::PaintColorPixel ( localFrame.left, 
								localFrame.bottom - 1, 
								&theColor );
	}

	// е Paint the face of the button
	::InsetRect ( &localFrame, 1, 1 );
	theColor = UGAColorRamp::GetColor(2);
	::RGBForeColor ( &theColor);
	::PaintRect ( &localFrame );
	
}	//	LGAIconButtonPopup::DrawIconButtonDimmedColor


#pragma mark -
#pragma mark === MISCELLANEOUS

//=====================================================================================
// ее MISCELLANEOUS
//-------------------------------------------------------------------------------------
// LGAIconButtonPopup::CalcIconLocation
//-------------------------------------------------------------------------------------

void
LGAIconButtonPopup::CalcIconLocation ()
{

	Rect		localFrame;
	SInt16		bevelWidth = GetBevelWidth ();

	// е We need to make sure that if the edge offset has been set
	// to be less than the bevel width we adjust it accordingly
	// which in this case means that we always want the offset to be
	// thebevel width plus a 2 pixel buffer
	if ( mEdgeOffset < bevelWidth )
		mEdgeOffset =  bevelWidth + 2;
	
	// е Get the local inset frame rect
	CalcLocalFrameRect ( localFrame );
	mIconLocation.h = localFrame.left;
	mIconLocation.v = localFrame.top;
	
	// е The location is always equivalent to left/center so we only handle
	// the calculation of that position
	mIconLocation.h += mEdgeOffset;
	mIconLocation.v += ( UGraphicsUtilities::RectHeight ( localFrame ) / 2 ) - ( mSizeSelector / 2 );
	
}	//	LGAIconButtonPopup::CalcIconLocation


