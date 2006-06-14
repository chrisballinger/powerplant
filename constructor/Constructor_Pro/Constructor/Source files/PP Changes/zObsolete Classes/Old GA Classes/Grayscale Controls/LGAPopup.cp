// Copyright й2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyrightй 1995 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< е >>-----------------------------------------

FILE:					LGAPopup.cp

CLASSES:				LGAPopup

DESCRIPTION:		Implementation file for LGAPopup

AUTHOR:				Robin Mair

CREATION DATE :	96.06.03

CHANGE HISTORY :

		97.04.20		rtm	Removed the CreateFromStream method.
		97.04.20		rtm	Fixed a bug in SetMacMenuH where the resource ID was being
								extracted from the menu handle without checking to make sure
								the menu handle was not nil.
		97.04.11		rtm	There are two major changes that have been made to the popup
								first of all by default it now has the same behavior as the
								old style popup in that it is no longer a pulldown menu but
								a popup meaning that it pops up at the current selection.
								Secondly, the popup indicator arrows have been changed to a
								pair of arrows one pointing up and the other pointing down. All
								these changes are in line with the changes that Apple has made
								to the appearance manager.
								As part of this process the arrow only style of popup has been
								changed such that if the popup size is set to be less than the
								recommended minimums for this style of popup, then the size is
								automatically adjusted to the recommended size.
								NOTE: This class still supports the pulldown style, it is just
								not able to be set from constructor, you have to call the
								SetPulldown method in order to change it from one style to the
								other.  When the popup is a pulldown there is only a single
								arrow pointing down.
		97.04.11		rtm	Changed the LoadResourceMenu function so that it now calls
								AppendResMenu instead of InsertResMenu, this allows items to be
								append to the end of a menu instead of always having them
								inserted at the head of the menu.
		97.03.24		rtm	Fixed a bug where a menu command handle could be retrieved that
								was a valid handle but of zero length which would cause a crash
								when trying to read the number of menu commands from the handle.
								This is in ReadCommandNumbers.
		97.03.07		rtm	Fixed a bug in SetMacMenuH where the menu hande could be changed
								but the menu resource ID would not be synched up with it.
		97.02.08		rtm	Changed the rendering of the control such that it is drawn as
								disabled when it is deactivated, this is matches the approach
								Apple is taking with the Apearance Manager.
		97.01.24		rtm	Added a copy constructor.
		96.01.20		rtm	Added a parameterized constructor so that the pane can be built
								procedurally.
		97.01.20		rtm	Implemented support for reading in a list of command numbers
								that are associated with the popup's menu, this also includes
								a function for getting a command number from an index. This
								also required a change to SetPopupMenuResID so that the command
								numbers will be reread if the res ID is changed.
		96.12.08		rtm	Changed justification constants to use the new ones defined
								in the universal headers.
		96.12.08		rtm	Made a small change to the HandlePopupMenuSelect method where
								we ow call InsertMenu before we twiddle the low mem font
								globals, Apple has a snippet that says this is the right thing 
								to do so like the good citizens we are we do it!
		96.12.02		rtm	Implemented the "Fixed Width" feature as used in the standard
								toolbox popup which means that if the popup is set to not be
								fixed that its size will be adjusted to accomodate the width
								of the longest menu item in the associated menu.  In order for
								dynamic menus to work correctly the user must either use the
								SetMacMenuH API or they should call AdjustPopupWidth after an
								item has been added to the menu.  The toolbox will do this
								automatically, but unfortunately we will need to do it again
								because the toolbox always does the calculation using the
								system font and the user might have set the popup up to use
								another font and size. Ugh!!
								NOTE: because this flag was being ignored in previous versions
								of this popup the user is going to have to go back and reset
								all popups to be fixed width [recommended] if thats what they
								wanted the popup to do.  Constructor will be changed so that
								the default for this field will always be true in the future.
		96.10.29		rtm	Fixed a bug where the arrow only version of the popup was not
								drawing correctly for a size width smaller than 22 pixels, this
								was due to a constant that was being used for both cases where
								the width of the popup should have been used for the arrow 
								only case.  As part of this process fixed the drawing of the
								arrow which was using a similar approach, now the arrow draws 
								centered in the arrow only case.
		96.10.19		rtm	Fixed the drawing of title across multiple devices for the
								hilited case primarily as there was a situation where the
								title would not get drawn.  In the process cleaned up a lot
								of the drawing code for titles in B&W.  Also had to fix the
								drawing of the label in B&W as there were situations where
								it would not draw.
		96.10.19		rtm	Changed GetMacMenuH so that is no longer declared as 'const'.
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
		96.10.12		rtm	Had to tweak the drawing of titles across multiple devices
								disabled.  Using the grayishTextOr text mode transfer was
								resulting in some weirdness that could only be solved by
								doing something similar to what grayishTextOr did, which was
								to basically lighten the text color.
		96.10.09		rtm	Moved the setup of teh popup's min and max values from the
								stream constructor to the FinishCreateSelf method.
		96.10.09		rtm	Changed the title drawing so that it now will respect a color
								provided in a text trait as  opposed to always drawing the title
								in black.
		96.10.08		rtm	Got rid of DrawPopup by moving its contents to DrawSelf.
		96.10.04		rtm	Replaced all SetCPixel calls with a new PaintColorPixel call
								that has been added to UGraphicsUtilities. This should get
								around the problems with using SetCPixel.
		96.09.27		rtm	Changed the way the frame is drawn around the popup. AGA says
								that two of the corner pixels should be a gray 12 color doing
								this meant that one could not use FrameRoundRect to frame the
								popup.  So basically I opted to violate the AGA in favor of
								efficiency and draw the entire border in black
		96.09.25		rtm	Fixed a number of drawing problems where certain pixels were
								being overdrawn and then being drawn again, all of the drawing
								now appears to be drawing correctly.
		96.09.23		rtm	Got rid of tge GetCurrentItem and SetCurrentItem methods and
								now use getValue and SetValue instead.
		96.09.19		rtm	Changed the title drawing method so that it always left justifies
								the title and therefore ignores the justification of any
								associated text trait.
		96.09.19		rtm	Changed the title drawing method so that the actual call to 
								draw the text is inside the while loop used to check the
								screen depth.
		96.09.13		rtm	Changed the GetPopupMenu method's name to LoadPopupMenuH, and
								provided another method called GetMacMenuH that is used to get
								a reference to the menu handle.  This was done to be more
								consistent with PowerPlant.
		96.09.04		rtm	PowerPlant conformance changes.
		96.09.02		rtm	Added overrides of EnableSelf and DisableSelf so that we can
								handle getting the control redrawn when the state changes.
		96.09.02		rtm	Changed the rendering of the dimmed state of the control to
								meet Apple's modified spec for grayscale controls.
		96.08.26		rtm	Changed the implementation of the mouse tracking for the popup
								in order to get rid of the hack dicussed below.  The change was
								to add an override of TrackHotSpot which called PopupMenuSelect
								when the mouise was in the control and only return true if a
								menu item was selected from the popup. Thanks Greg Dow for this
								suggestion.
		96.08.22		rtm	Added an aweful hack to get around what appears to be a bug
								in PowerPlant that would result in the popup control never 
								being unhilited, to fix the problem we needed to add a
								override of ClickSelf in which we called SetHiliteState
								to force turning off the hiliting.
		96.08.21		rtm	Added support for building menus that contain a certain
								resource type, as specified through the property inspector
								in Constructor.  Also implemented the support for setting the
								style and justification for the popup's label.
		96.08.09		rtm	SetupCurrentMenuItem had some code that was no longer needed
								so I deleted it, it was related to the marking and unmarking
								of menu items.  Because the menu is no longer being cached
								there was no need to unmark the old menu item as a fresh menu
								is always being loaded without any marks.
		96.06.25		rtm	Tweaked the drawing of the current item title as it does not
								need to draw as part of the hiliting process as we simply will
								invert the contents of the entire button.
		96.06.20		rtm	Changed the SetHiliteState method so that it now handles the
								the redraw after a state change instead of all the other 
								places that were trying to take care of it.
		96.06.17		rtm	Tweaked the calculation of the popup rect so that we can handle
								the user specifying a size that is too small.
		96.06.15		rtm	Completely re-wrote how this works to get around the problem
								of having multiple popups that are using the same menu resource
								now the menu is only loaded when needed and never cached.
		96.06.03		rtm	Initial creation of file from original G3DPopup

---------------------------------------<< е >>-----------------------------------------
*/

#ifdef PowerPlant_PCH
	#include PowerPlant_PCH
#endif

// е GRAYSCALE HEADERS
#include <LGAPopup.h>
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

const SInt16 gsPopup_RightInset 		= 	24;	//	Used to position the title rect
const SInt16 gsPopup_ArrowLeftInset		= 	6;		// Used to position the popup arrow
const SInt16 gsPopup_TitleInset			= 	8;		// Apple specification
const SInt16 gsPopup_LabelOffset 		= 	2;		//	Offset of label from popup
const SInt16 gsPopup_ArrowButtonWidth 	= 	22;	//	Width used in drawing the arrow only
const SInt16 gsPopup_ArrowButtonHeight	= 	18;	//	Height used for drawing arrow only
const SInt16 gsPopup_ArrowHeight		= 	5;		//	Actual height of the arrow
const SInt16 gsPopup_ArrowWidth			= 	9;		//	Actual width of the arrow at widest
const SInt16 gsPopup_MultiArrowHeight	= 	10;	//	Actual height of the two arrow
const SInt16 gsPopup_MultiArrowWidth	= 	7;		//	Actual width of the arrow at widest


const	UInt16	gsPopup_SmallMark			=	'е';	// Mark used for small font popups

//=====================================================================================
// DEFINES
//=====================================================================================

#pragma mark === CLASS: LGAPopup

//=====================================================================================
// CLASS:	LGAPopup
//=====================================================================================

#pragma mark -
#pragma mark === INITIALIZATION

//=====================================================================================
// ее INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LGAPopup::LGAPopup					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Default constructor

LGAPopup::LGAPopup ()
{

	mPopupMenuResID = 0;
	mLabelWidth = 0;
	mLabelStyle = 0;
	mUseResMenu = false;
	mResType = 0;
	mFixedWidth = true;
	mPulldown = true;
	mArrowOnly = false;
	mHilited = false;
	mCachedMenuH = nil;
	mNumCommands = 0;
	mCommandNums = nil;
	
}	//	LGAPopup::LGAPopup


//-------------------------------------------------------------------------------------
// LGAPopup::LGAPopup					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Copy constructor

LGAPopup::LGAPopup (	const LGAPopup &inOriginal )
									:	LControl ( inOriginal ),
										LGATitleMixin ( inOriginal )
{

	mPopupMenuResID = inOriginal.mPopupMenuResID;
	mLabelWidth = inOriginal.mLabelWidth;
	mLabelStyle = inOriginal.mLabelStyle;
	mUseResMenu = inOriginal.mUseResMenu;
	mResType = inOriginal.mResType;
	mFixedWidth = inOriginal.mFixedWidth;
	mPulldown = inOriginal.mPulldown;
	mArrowOnly = inOriginal.mArrowOnly;
	mHilited = inOriginal.mHilited;
		
	// е We cannot simply copy the reference to the cached menu instead
	// we set it to nil and rely on the convention that the caller has
	// to also call FinishCreateSelf which is where these value will be set
	mCachedMenuH = nil;
	mNumCommands = inOriginal.mNumCommands;
	mCommandNums = nil;

	
}	//	LGAPopup::LGAPopup


//-------------------------------------------------------------------------------------
// LGAPopup::LGAPopup					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Stream constructor

LGAPopup::LGAPopup (	LStream	*inStream ) : LControl ( inStream )
{
	
	SInt16	controlKind;
	SInt32	macRefCon;
	SInt16	currItem;
	
	// е Read in the stream data
	*inStream >> controlKind;
	mUseResMenu = (controlKind & 4) != 0;
	mFixedWidth = controlKind & 1;
	*inStream >> mTextTraitsID;
	inStream->ReadPString ( mTitle );
	*inStream >> macRefCon;
	mResType = (OSType)macRefCon;
	*inStream >> currItem;
	
	// е Now we need to map some of the values as used by the standard popup
	// to our fields
	mCachedMenuH = nil;
	mPopupMenuResID = mMinValue;
	mLabelWidth = mMaxValue;
	mLabelStyle = (mValue >> 8) & 0xFF;
	mLabelJust = mValue & 0xFF;
	if ( mLabelJust == 255 )
		mLabelJust = teFlushRight;
	mNumCommands = 0;
	mCommandNums = nil;
	 	
	// е Setup some our private data
	mPulldown = false;						//	For AGA we always want a pulldown
	
	// е Figure out if we are going to display just the arrow portion of
	// the popup, we do this by looking at a number of factors: no label plus
	// a width of less than 24 pixels
	if ( mLabelWidth == 0 && mFrameSize.width < 24 )
		mArrowOnly = true;
	else
		mArrowOnly = false;

	// е Make sure we setup the value field with the current item
	if ( currItem > 0 )
		mValue = currItem;
	else
		mValue = 1;
	
	// е Make sure we start with the hilite state set to false
	mHilited = false;

	// е NOTE: at this point in time the menu handle has not yet been
	// cached as that takes place in FinishCreateSelf.
	
}	//	LGAPopup::LGAPopup


//-------------------------------------------------------------------------------------
// LGAPopup::LGAPopup					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Parameterized constructor

LGAPopup::LGAPopup (	const SPaneInfo		&inPaneInfo,
							const SControlInfo	&inControlInfo, 
							SInt32						inRefCon,
							SInt16						inControlKind,
							ResIDT					inTextTraitsID,
							Str255					inLabel,
							SInt16						inCurrentItem ) 
				: LControl ( 	inPaneInfo,
									inControlInfo.valueMessage,
									inControlInfo.value,
									inControlInfo.minValue,
									inControlInfo.maxValue )
{
	
	// е Get the fields for the class setup
	mUseResMenu = (inControlKind & 4) != 0;
	mFixedWidth = inControlKind & 1;
	mResType = (OSType)inRefCon;
	mTextTraitsID = inTextTraitsID;
	mTitle = inLabel;
	mCachedMenuH = nil;
	mPopupMenuResID = mMinValue;
	mLabelWidth = mMaxValue;
	mLabelStyle = (mValue >> 8) & 0xFF;
	mLabelJust = mValue & 0xFF;
	if ( mLabelJust == 255 )
		mLabelJust = teFlushRight;
	mNumCommands = 0;
	mCommandNums = nil;
	 	
	// е Setup some our private data
	mPulldown = false;						//	For AGA we always want a pulldown
	
	// е Figure out if we are going to display just the arrow portion of
	// the popup, we do this by looking at a number of factors: no label plus
	// a width of less than 24 pixels
	if ( mLabelWidth == 0 && mFrameSize.width < 24 )
		mArrowOnly = true;
	else
		mArrowOnly = false;

	// е Make sure we setup the value field with the current item
	if ( inCurrentItem > 0 )
		mValue = inCurrentItem;
	else
		mValue = 1;
	
	// е Make sure we start with the hilite state set to false
	mHilited = false;

	// е NOTE: at this point in time the menu handle has not yet been
	// cached as that takes place in FinishCreateSelf.
	
}	//	LGAPopup::LGAPopup


//-------------------------------------------------------------------------------------
// LGAPopup::LGAPopup					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Parameterized constructor

LGAPopup::LGAPopup (	const SPaneInfo		&inPaneInfo,
							MessageT					inValueMessage,
							ResIDT					inPopupMenuResID,
							Boolean					inUseResMenu,
							SInt32						inResType,
							Boolean					inFixedWidth,
							Str255					inLabel,
							ResIDT					inTextTraitsID,
							SInt16						inLabelWidth,
							SInt16						inLabelStyle,
							SInt16						inLabelJust,
							SInt16						inCurrentItem ) 
				: LControl ( 	inPaneInfo,
									inValueMessage,
									inCurrentItem,
									inPopupMenuResID,
									inLabelWidth )
{
	
	// е Get the fields for the class setup
	mUseResMenu = inUseResMenu;
	mFixedWidth = inFixedWidth;
	mResType = (OSType)inResType;
	mTextTraitsID = inTextTraitsID;
	mTitle = inLabel;
	mCachedMenuH = nil;
	mPopupMenuResID = inPopupMenuResID;
	mLabelWidth = inLabelWidth;
	mLabelStyle = inLabelStyle;
	mLabelJust = inLabelJust;
	if ( mLabelJust == 255 )
		mLabelJust = teFlushRight;
	mNumCommands = 0;
	mCommandNums = nil;
	 	
	// е Setup some our private data
	mPulldown = false;						//	For AGA we always want a pulldown
	
	// е Figure out if we are going to display just the arrow portion of
	// the popup, we do this by looking at a number of factors: no label plus
	// a width of less than 24 pixels
	if ( mLabelWidth == 0 && mFrameSize.width < 24 )
		mArrowOnly = true;
	else
		mArrowOnly = false;

	// е Make sure we setup the value field with the current item
	if ( inCurrentItem > 0 )
		mValue = inCurrentItem;
	else
		mValue = 1;
	
	// е Make sure we start with the hilite state set to false
	mHilited = false;

	// е NOTE: at this point in time the menu handle has not yet been
	// cached as that takes place in FinishCreateSelf.
	
}	//	LGAPopup::LGAPopup


//-------------------------------------------------------------------------------------
// LGAPopup::~LGAPopup				---<<е DESTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LGAPopup::~LGAPopup ()
{

	// е Get rid of the menu handle
	if ( mCachedMenuH )
	{
		::DisposeMenu( mCachedMenuH );
		mCachedMenuH = nil;
	}
	
	// е Get rid of the command numbers if we have them
	if ( mCommandNums )
	{
		::DisposeHandle ( (Handle)mCommandNums );
		mCommandNums = nil;
	}

}	//	LGAPopup::~LGAPopup


//-------------------------------------------------------------------------------------
// LGAPopup::FinishCreateSelf
//-------------------------------------------------------------------------------------

void
LGAPopup::FinishCreateSelf ()
{

	// е If the popup is set o be arrow only then we will automatically make it
	// the minimum recommended size, if it isn't already
	if ( mArrowOnly )
		ResizeForArrowOnly ();
		
	// е Make sure that the correct values are setup for the popup
	SetMacMenuH ( LoadPopupMenuH () );

	// е Read in any associated command numbers for the popup's menu
	ReadCommandNumbers ();
	
}	//	LGAPopup::FinishCreateSelf

	
#pragma mark -
#pragma mark === ACCESSORS

//=====================================================================================
// ее ACCESSORS
//-------------------------------------------------------------------------------------
// LGAPopup::GetDescriptor
//-------------------------------------------------------------------------------------

StringPtr
LGAPopup::GetDescriptor ( Str255	outDescriptor ) const
{

	return LString::CopyPStr ( mTitle, outDescriptor );

}	//	LGAPopup::GetDescriptor


//-------------------------------------------------------------------------------------
// LGAPopup::LoadPopupMenuH
//-------------------------------------------------------------------------------------

MenuHandle
LGAPopup::LoadPopupMenuH () const
{

	// е Load the menu handle and then if we are building the menu
	// from a resource type we get the types added to the menu
	MenuHandle	menuH = ::GetMenu ( mPopupMenuResID );
	if ( menuH && UseResourceMenu ())
		LoadResourceMenu ( menuH );
		
	return menuH;
	
}	//	LGAPopup::LoadPopupMenuH


//-------------------------------------------------------------------------------------
// LGAPopup::GetCurrentItemTitle
//-------------------------------------------------------------------------------------

void
LGAPopup::GetCurrentItemTitle ( Str255 outItemTitle )
{
	// е Load the popup menu and then find the title
	MenuHandle	popupMenu = GetMacMenuH ();
	if ( popupMenu )
	{			
		::GetMenuItemText ( popupMenu, GetValue (), outItemTitle );
	}
	
}	//	LGAPopup::GetCurrentItemTitle


//-------------------------------------------------------------------------------------
// LGAPopup::ReadCommandNumbers
//-------------------------------------------------------------------------------------

void
LGAPopup::ReadCommandNumbers ()
{

	// е Load the Mcmd resource, if we have one then we can proceed to extract
	// the menu command table
	SInt16**	theMcmdH = (SInt16**) ::GetResource ( 'Mcmd', mPopupMenuResID );
	if ( theMcmdH != nil ) 
	{
		if ( ::GetHandleSize( (Handle)theMcmdH ) > 0 )
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
		else
			::ReleaseResource ( (Handle)theMcmdH );
	}
	
}	//	LGAPopup::ReadCommandNumbers


//-------------------------------------------------------------------------------------
// LGAPopup::CommandFromIndex
//-------------------------------------------------------------------------------------

CommandT
LGAPopup::CommandFromIndex (	SInt16	inIndex )
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
	
}	//	LGAPopup::CommandFromIndex


//-------------------------------------------------------------------------------------
// LGAPopup::SyntheticCommandFromIndex
//-------------------------------------------------------------------------------------

CommandT
LGAPopup::SyntheticCommandFromIndex ( SInt16	inIndex ) const
{
	return (-(((SInt32)mPopupMenuResID) << 16) - inIndex);
}

//-------------------------------------------------------------------------------------
// LGAPopup::GetMenuFontSize
//-------------------------------------------------------------------------------------

SInt16
LGAPopup::GetMenuFontSize () const
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
	
}	//	LGAPopup::GetMenuFontSize


//-------------------------------------------------------------------------------------
// LGAPopup::HasLabel
//-------------------------------------------------------------------------------------

Boolean
LGAPopup::HasLabel () const
{
	Str255	labelString;
	GetDescriptor ( labelString );
	
	return labelString[0] > 0;
	
}	//	LGAPopup::HasLabel


//-------------------------------------------------------------------------------------
// LGAPopup::SetMacMenuH
//-------------------------------------------------------------------------------------

void
LGAPopup::SetMacMenuH (	MenuHandle	inMenuHandle )
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
	
	// е If the popup has been setup to have a variable width and
	// the popup is not being dispalyed as only the arrow portion,then
	// we also need to make sure that we get the width adjusted
	if ( !mFixedWidth && !IsArrowOnly ())
		AdjustPopupWidth ();
	
}	//	LGAPopup::SetMacMenuH


//-------------------------------------------------------------------------------------
// LGAPopup::SetPopupMenuResID
//-------------------------------------------------------------------------------------

void
LGAPopup::SetPopupMenuResID (	ResIDT	inResID )
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

		// е Get things redrawn so the changes can be seen
		Draw ( nil);
	}

}	//	LGAPopup::SetPopupMenuResID


//-------------------------------------------------------------------------------------
// LGAPopup::SetDescriptor
//-------------------------------------------------------------------------------------

void
LGAPopup::SetDescriptor (	ConstStr255Param	 inDescriptor )
{

	mTitle = inDescriptor;
	Refresh ();

}	//	LGAPopup::SetDescriptor


//-------------------------------------------------------------------------------------
// LGAPopup::SetHiliteState
//-------------------------------------------------------------------------------------

void			
LGAPopup::SetHiliteState	(	Boolean	inHiliteState )
{
	
	// е Only change the state if have too
	if ( mHilited != inHiliteState )
	{
		mHilited = inHiliteState;
		
		// е Get the popup redrawn so that the state change can take effect
		Draw ( nil );
	}
			
}	//	LGAPopup::SetHiliteState


//-------------------------------------------------------------------------------------
// LGAPopup::SetLabelWidth
//-------------------------------------------------------------------------------------

void
LGAPopup::SetLabelWidth (	SInt16	inLabelWidth )
{

	// е Only make the change if we have to
	if ( mLabelWidth != inLabelWidth )
	{
		mLabelWidth = inLabelWidth;
		
		// е If the popup has been setup to have a variable width and
		// the popup is not being dispalyed as only the arrow portion,then
		// we also need to make sure that we get the width adjusted
		if ( !mFixedWidth && !IsArrowOnly ())
			AdjustPopupWidth ();
	
		// е Get the whole thing refreshed
		Refresh ();
	}
	
}	//	LGAPopup::SetLabelWidth


//-------------------------------------------------------------------------------------
// LGAPopup::SetLabelStyle
//-------------------------------------------------------------------------------------

void
LGAPopup::SetLabelStyle (	SInt16	inLabelStyle )
{

	// е Only make the change if we have to
	if ( mLabelStyle != inLabelStyle )
	{
		mLabelStyle = inLabelStyle;
		Refresh ();
	}
	
}	//	LGAPopup::SetLabelStyle


//-------------------------------------------------------------------------------------
// LGAPopup::SetLabelJust
//-------------------------------------------------------------------------------------

void
LGAPopup::SetLabelJust (	SInt16	inLabelJust )
{

	// е Only make the change if we have to
	if ( mLabelJust != inLabelJust )
	{
		mLabelJust = inLabelJust;
		Refresh ();
	}
	
}	//	LGAPopup::SetLabelJust


//-------------------------------------------------------------------------------------
// LGAPopup::SetValue
//-------------------------------------------------------------------------------------

void
LGAPopup::SetValue	(	SInt32	inValue )
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
		LControl::SetValue ( inValue );
	
		// е Now we need to get the popup redrawn so that the change
		// will be seen
		Draw ( nil );
	}
	
}	//	LGAPopup::SetValue


//-------------------------------------------------------------------------------------
// LGAPopup::SetupCurrentMenuItem
//-------------------------------------------------------------------------------------

void
LGAPopup::SetupCurrentMenuItem (	MenuHandle	inMenuH,
											SInt16			inCurrentItem )
{
	
	// е If the current item has changed then make it so, this
	// also involves removing the mark from any old item
	if ( inMenuH )
	{
		if ( GetValue () != inCurrentItem )
		{
			// е Get the current value
			SInt16		oldItem = GetValue ();
			
			// е Remove the current mark
			::SetItemMark ( inMenuH, oldItem, 0 );
		}
		
		// е Always make sure item is marked
		UInt16	mark = GetMenuFontSize () < 12 ? gsPopup_SmallMark : checkMark;
		::SetItemMark ( 	inMenuH, 
								inCurrentItem, 
								mark );
	}
	
}	//	LGAPopup::SetupCurrentMenuItem


//-------------------------------------------------------------------------------------
// LGAPopup::SetPulldown
//-------------------------------------------------------------------------------------

void
LGAPopup::SetPulldown (	Boolean	inPulldown )
{

	if ( mPulldown != inPulldown )
	{
		mPulldown = inPulldown;
		Refresh ();
	}
	
}	//	LGAPopup::SetPulldown


//-------------------------------------------------------------------------------------
// LGAPopup::SetArrowOnly
//-------------------------------------------------------------------------------------

void
LGAPopup::SetArrowOnly (	Boolean	inArrowOnly )
{

	if ( mArrowOnly != inArrowOnly )
	{
		mArrowOnly = inArrowOnly;
		ResizeForArrowOnly ();
		Refresh ();
	}
	
}	//	LGAPopup::SetArrowOnly


//-------------------------------------------------------------------------------------
// LGAPopup::SetPopupMinMaxValues
//-------------------------------------------------------------------------------------

void
LGAPopup::SetPopupMinMaxValues ()
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
		
}	//	LGAPopup::SetPopupMinMaxValues


//-------------------------------------------------------------------------------------
// LGAPopup::CalcLocalPopupFrameRect
//-------------------------------------------------------------------------------------
//

void
LGAPopup::CalcLocalPopupFrameRect	( Rect	&outRect ) const
{
	
	// е Get the local frame rectangle and adjust its width to the
	// width of the poups label
	CalcLocalFrameRect ( outRect );
	
	if ( IsArrowOnly ())
	{
		// е Make sure that we are at least the correct size for drawing
		// the arrow button, if not we will just be rendering in the size
		// specified with unpredictable results
		if ( mFrameSize.width >= gsPopup_ArrowButtonWidth )
			outRect.left = outRect.right - gsPopup_ArrowButtonWidth;
		
		// е Make sure that we can accomodate the height required for our
		// our drawing, if the popup height has been setup smaller then we
		// just draw to the specified height and hope for the best
		if ( mFrameSize.height >= gsPopup_ArrowButtonHeight )
		{
			outRect.top += ( UGraphicsUtilities::RectHeight ( outRect ) - gsPopup_ArrowButtonHeight) / 2;
			outRect.bottom = outRect.top + gsPopup_ArrowButtonHeight;
		}
	}
	else
		outRect.left += GetLabelWidth ();
			
}	//	LGAPopup::CalcLocalPopupFrameRect


//-------------------------------------------------------------------------------------
// LGAPopup::CalcTitleRect
//-------------------------------------------------------------------------------------
//
//	This method is used to calculate the title rect for the currently selected item in
// 	the popup, this title is drawn inside the popup

void
LGAPopup::CalcTitleRect	( Rect	&outRect )
{
	
	StTextState			theTextState;
	StColorPenState	thePenState;
	SInt16		bevelWidth = 2;
	
	// е Get some loal variables setup including the rect for the title
	ResIDT	textTID = GetTextTraitsID ();

	// е Get the port setup with the text traits
	UTextTraits::SetPortTextTraits ( textTID );
	
	// е Figure out the height of the text for the selected font
	FontInfo fInfo;
	GetFontInfo ( &fInfo );
	SInt16		textHeight = fInfo.ascent + fInfo.descent;
	SInt16		textBaseline = fInfo.ascent;
	
	// е Get the local inset frame rectangle
	CalcLocalPopupFrameRect ( outRect );
	::InsetRect ( &outRect, 0, bevelWidth );
	outRect.right -= gsPopup_RightInset;
	outRect.left += gsPopup_TitleInset;
	
	// е Adjust the title rect to match the height of the font
	outRect.top += (( UGraphicsUtilities::RectHeight ( outRect ) - textBaseline) / 2) - 2;
	outRect.bottom = outRect.top + textHeight;
		
}	//	LGAPopup::CalcTitleRect


//-------------------------------------------------------------------------------------
// LGAPopup::CalcLabelRect
//-------------------------------------------------------------------------------------

void
LGAPopup::CalcLabelRect	( Rect	&outRect )
{
	if ( HasLabel ())
	{
		StTextState			theTextState;
		StColorPenState	thePenState;
		
		// е Get some loal variables setup including the rect for the title
		ResIDT	textTID = GetTextTraitsID ();
	
		// е Get the port setup with the text traits
		UTextTraits::SetPortTextTraits ( textTID );
		
		// е Figure out the height of the text for the selected font
		FontInfo fInfo;
		GetFontInfo ( &fInfo );
		SInt16		textHeight = fInfo.ascent + fInfo.descent;
		SInt16		textBaseline = fInfo.ascent;
			
		// е Get the local inset frame rectangle and adjust its width to the
		// width of the poups label
		CalcLocalFrameRect ( outRect );
		outRect.right = outRect.left + (mLabelWidth - gsPopup_LabelOffset);
		::InsetRect ( &outRect, 0, 2 );
		
		// е Adjust the title rect to match the height of the font
		outRect.top += (( UGraphicsUtilities::RectHeight ( outRect ) - textBaseline) / 2) - 2;
		outRect.bottom = outRect.top + textHeight;
	}
	else
		outRect = gEmptyRect;
		
}	//	LGAPopup::CalcLabelRect


//-------------------------------------------------------------------------------------
// LGAPopup::GetPopupMenuPosition
//-------------------------------------------------------------------------------------

void
LGAPopup::GetPopupMenuPosition	( Point	&outPopupLoc )
{
	
	// е Get the local popup button frame, this will be correctly setup depending
	// on whether the button is arrow only or not
	Rect	popupRect;
	CalcLocalPopupFrameRect ( popupRect );
	Rect labelRect;
	SInt16	labelTop;
	
	// е In this next section of code we are going to figure out where the top of
	// the label text rect would be located as we use this value to position the
	// popup menu when it is selected.  If there is a label then we simply get the
	// label rect and use its top, otherwise we need to figure it out.
	if ( HasLabel ())
	{
		CalcLabelRect ( labelRect );
		labelTop = labelRect.top;
	}
	else
	{
		// е We don't really need the label rect when there is no label except
		// we still need to know where the top of the rect would be located as
		// we use that to position the popup
		StTextState			theTextState;
		StColorPenState	thePenState;
		
		ResIDT	textTID = GetTextTraitsID ();
	
		// е Get the port setup with the text traits
		UTextTraits::SetPortTextTraits ( textTID );
		
		// е Figure out the height of the text for the selected font
		FontInfo fInfo;
		GetFontInfo ( &fInfo );
		SInt16		textBaseline = fInfo.ascent;
			
		// е Get the local frame rectangle so that we can use it as the
		// basis for calculating the label top
		CalcLocalFrameRect ( labelRect );
		::InsetRect ( &labelRect, 0, 2 );
		labelTop = labelRect.top;
		labelTop += (( UGraphicsUtilities::RectHeight ( labelRect ) - textBaseline) / 2) - 2;
	}
	
	// е Setup the vertical value based on whether it is a pulldown or popup menu
	outPopupLoc.v = IsPulldownMenu() ? popupRect.bottom : labelTop;

	// е If the popup is setup to have a pulldown menu then calculate the
	// apropriate location for the popup menu to appear based on whether only
	// the arrow button is being displayed or the entire popup button is visible
	// If it is a normal popup then set up the horizontal vaue appropriately
	if ( IsPulldownMenu ())
	{	
		outPopupLoc.h = popupRect.left + 2;
	}
	else
	{
		outPopupLoc.h =  
			IsArrowOnly () ? popupRect.left + (popupRect.right - popupRect.left) - 
																	gsPopup_ArrowButtonWidth : 
																		popupRect.left;
	}
	
	// е Because PopMenuSelect needs the location for the popup menu global 
	// coordinates we need to convert the location
	LocalToPortPoint ( outPopupLoc );
	PortToGlobalPoint ( outPopupLoc );
	
}	//	LGAPopup::GetPopupMenuPosition


#pragma mark -
#pragma mark === ENABLING & DISABLING

//=====================================================================================
// ее ENABLING & DISABLING
//-------------------------------------------------------------------------------------
// LGAPopup::EnableSelf
//-------------------------------------------------------------------------------------
//

void
LGAPopup::EnableSelf ()
{

	// е Get things redrawn so that we can see the state change
	Draw ( nil );

}	//	LGAPopup::EnableSelf


//-------------------------------------------------------------------------------------
// LGAPopup::DisableSelf
//-------------------------------------------------------------------------------------
//

void
LGAPopup::DisableSelf ()
{

	// е Get things redrawn so that we can see the state change
	Draw ( nil );

}	//	LGAPopup::DisableSelf


#pragma mark -
#pragma mark === ACTIVATION

//=====================================================================================
// ее ACTIVATION
//-------------------------------------------------------------------------------------
// LGAPopup::ActivateSelf
//-------------------------------------------------------------------------------------

void
LGAPopup::ActivateSelf ()
{

	// е Get everything redrawn so the changes take effect
	if ( mActive == triState_On )
		Refresh ();
					
}	//	LGAPopup::ActivateSelf


//-------------------------------------------------------------------------------------
// LGAPopup::DeactivateSelf
//-------------------------------------------------------------------------------------

void
LGAPopup::DeactivateSelf ()
{

	// е Get everything redrawn so the changes take effect
	if ( mActive == triState_Off || mActive == triState_Latent )
		Refresh ();
				
}	//	LGAPopup::DeactivateSelf


#pragma mark -
#pragma mark === DRAWING

//=====================================================================================
// ее DRAWING
//-------------------------------------------------------------------------------------
// LGAPopup::DrawSelf
//-------------------------------------------------------------------------------------

void
LGAPopup::DrawSelf	()
{

	StColorPenState::Normalize ();
	
	// е Get the control drawn in its various states
	if ( IsEnabled () && IsActive ())
	{
		if ( IsHilited ())
			DrawPopupHilited ();
		else
			DrawPopupNormal ();
	}
	else
		DrawPopupDimmed ();

	// е Get the arrow drawn
	DrawPopupArrow ();
	
	// е Draw the popup Label
	if ( !IsArrowOnly ())
		DrawPopupLabel ();

	// е Get the title for the popup drawn
	if ( !IsArrowOnly ())
		DrawPopupTitle ();
	
}	//	LGAPopup::DrawSelf


//-------------------------------------------------------------------------------------
// LGAPopup::DrawPopupNormal
//-------------------------------------------------------------------------------------

void
LGAPopup::DrawPopupNormal	()
{
	
	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalPopupFrameRect ( localFrame );

	// е Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( localFrame );
	SInt16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		// е BLACK & WHITE
		if ( depth < 4 )
		{
			DrawPopupNormalBW ();
		}
		// е COLOR
		else
		{
			DrawPopupNormalColor ();
		}	
	}
	
}	//	LGAPopup::DrawPopupNormal


//-------------------------------------------------------------------------------------
// LGAPopup::DrawPopupHilited
//-------------------------------------------------------------------------------------

void
LGAPopup::DrawPopupHilited	()
{
	
	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalPopupFrameRect ( localFrame );

	// е Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( localFrame );
	SInt16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		// е BLACK & WHITE
		if ( depth < 4 )
		{
			DrawPopupHilitedBW ();
		}
		// е COLOR
		else
		{
			DrawPopupHilitedColor ();
		}	
	}
	
}	//	LGAPopup::DrawPopupHilited


//-------------------------------------------------------------------------------------
// LGAPopup::DrawPopupDimmed
//-------------------------------------------------------------------------------------

void
LGAPopup::DrawPopupDimmed	()
{
	
	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalPopupFrameRect ( localFrame );

	// е Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( localFrame );
	SInt16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		// е BLACK & WHITE
		if ( depth < 4 )
		{
			DrawPopupDimmedBW ();
		}
		// е COLOR
		else
		{
			DrawPopupDimmedColor ();
		}	
	}
	
}	//	LGAPopup::DrawPopupDimmed


//-------------------------------------------------------------------------------------
// LGAPopup::DrawPopupTitle
//-------------------------------------------------------------------------------------

void
LGAPopup::DrawPopupTitle	()
{
	StColorPenState	theColorPenState;
	StTextState 		theTextState;
	
	// е Get some loal variables setup including the rect for the title
	ResIDT	textTID = GetTextTraitsID ();
	Rect	titleRect;
	Str255 title;
	GetCurrentItemTitle ( title );
	
	// е Figure out what the justification is from the text trait and 
	// get the port setup with the text traits
	UTextTraits::SetPortTextTraits ( textTID );
	
	// е Setup the title justification which is always left justified
	SInt16	titleJust = teFlushLeft;
	
	// е Calculate the title rect
	CalcTitleRect ( titleRect );
	
	// е Setup the text color which by default is black
	RGBColor	textColor;
	::GetForeColor ( &textColor );
	
	// е Get the current item's title
	Str255 currentItemTitle;
	GetCurrentItemTitle ( currentItemTitle );

	// е Loop over any devices we might be spanning and handle the drawing
	// appropriately for each devices screen depth
	StDeviceLoop	theLoop ( titleRect );
	SInt16				depth;
	RGBColor theColor;
	while ( theLoop.NextDepth ( depth )) 
	{
		if ( depth < 4 )		//	е BLACK & WHITE
		{
			// е If the control is dimmed then we use the grayishTextOr 
			// transfer mode to draw the text
			if ( !IsEnabled ())
			{
				theColor = UGAColorRamp::GetBlackColor ();
				::RGBForeColor ( &theColor );
				::TextMode ( grayishTextOr );
			}
			else if ( IsEnabled () && IsHilited () )
			{
				// е When we are hilited we simply draw the title in white
				theColor = UGAColorRamp::GetWhiteColor ();
				::RGBForeColor ( &theColor );
			}
			
			// е Now get the actual title drawn with all the appropriate settings
			UStringDrawing::DrawJustifiedString ( 	currentItemTitle,
																titleRect,
																titleJust );
		}
		else	// е COLOR
		{
			// е If control is selected we always draw the text in the title
			// hilite color, if requested
			if ( IsHilited ())
			{
				theColor = UGAColorRamp::GetWhiteColor();
				::RGBForeColor ( &theColor );
			}
			
			// е If the box is dimmed then we have to do our own version of the
			// grayishTextOr as it does not appear to work correctly across
			// multiple devices
			if ( !IsEnabled () || !IsActive ())
			{
				textColor = UGraphicsUtilities::Lighten ( &textColor );
				::TextMode ( srcOr );
				::RGBForeColor ( &textColor );
			}
				
			// е Now get the actual title drawn with all the appropriate settings
			UStringDrawing::DrawJustifiedString ( 	currentItemTitle,
																titleRect,
																titleJust );
		}	
	}
														
}	//	LGAPopup::DrawPopupTitle


//-------------------------------------------------------------------------------------
// LGAPopup::DrawPopupLabel
//-------------------------------------------------------------------------------------

void
LGAPopup::DrawPopupLabel	()
{

	StTextState theTextState;
			
	// е Get some loal variables setup including the rect for the title
	ResIDT	textTID = GetTextTraitsID ();
	Rect	labelRect;
	
	// е Get the text traits setup for the port
	 UTextTraits::SetPortTextTraits ( textTID );
	
	// е Calculate the title rect
	CalcLabelRect ( labelRect );
	
	
	// е Now get the actual title drawn with all the appropriate settings
	Str255	popupLabel;
	GetDescriptor ( popupLabel );
												
	// е Setup the labels style
	::TextFace ( GetLabelStyle () );

	// е Save off the text color as setup by the TextTrait
	RGBColor	textColor;
	::GetForeColor ( &textColor );
	
	// е Loop over any devices we might be spanning and handle the drawing
	// appropriately for each devices screen depth
	StDeviceLoop	theLoop ( labelRect );
	SInt16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		if ( depth < 4 )		//	е BLACK & WHITE
		{
			// е If the control is dimmed then we use the grayishTextOr 
			// transfer mode to draw the text
			if ( !IsEnabled ())
				::TextMode ( grayishTextOr );
			
			// е Now get the actual title drawn with all the appropriate settings
			UStringDrawing::DrawJustifiedString ( 	popupLabel,
																labelRect,
																GetLabelJust () );
		}
		else	// е COLOR
		{
			// е If the control is dimmed then we have to do our own version of the
			// grayishTextOr as it does not appear to work correctly across
			// multiple devices
			if ( !IsEnabled () || !IsActive () )
			{
				textColor = UGraphicsUtilities::Lighten ( &textColor );
				::TextMode ( srcOr );
				::RGBForeColor ( &textColor );
			}
				
			// е Now get the actual title drawn with all the appropriate settings
			UStringDrawing::DrawJustifiedString ( 	popupLabel,
																labelRect,
																GetLabelJust () );
		}	
	}
	
}	//	LGAPopup::DrawPopupLabel


//-------------------------------------------------------------------------------------
// LGAPopup::DrawPopupArrow
//-------------------------------------------------------------------------------------

void
LGAPopup::DrawPopupArrow	()
{
	StColorPenState	theColorPenState;
	
	// е Get the local popup frame rect
	Rect	popupFrame;
	CalcLocalPopupFrameRect ( popupFrame );
	
	// е Setup some variables used in the drawing loop
	SInt16		bevelWidth = 3;
	
	// е Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( popupFrame );
	SInt16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		RGBColor	indicatorColor = UGAColorRamp::GetBlackColor();
		SInt16		leftEdge; 
		SInt16		rightEdge;
		SInt16  counter;
		
		// е BLACK & WHITE
		if ( depth < 4 )
		{
			// е We only draw the indicator when we are not hilited as the
			// entire button including the indicator is inverted for the
			// hilited state
			// е We draw the indicator in black when normal, white when hilited
			// and with a gray pattern when the button is dimmed
			indicatorColor = IsHilited () && IsEnabled () ? UGAColorRamp::GetWhiteColor () :
												UGAColorRamp::GetBlackColor();
			if ( !IsEnabled ())
			{
				Pattern thePattern;
				UQDGlobals::GetGrayPat(&thePattern);
				::PenPat ( &thePattern );
			}
		}
		// е COLOR
		else
		{
			// е We draw the indicator in black when normal gray 7 when dimmed
			// and white when hilited
			if ( !IsEnabled () || !IsActive ())
				 indicatorColor = UGAColorRamp::GetColor(7);
			if ( IsHilited ())
				indicatorColor = IsEnabled () ? UGAColorRamp::GetWhiteColor() :
																		UGAColorRamp::GetColor(7);
		}	
		
		// е Set the fore color for drawing
		::RGBForeColor ( &indicatorColor );

		if ( !IsPulldownMenu ())
		{
			SInt16		topStart = bevelWidth + 1;
			SInt16		offset = ((gsPopup_ArrowButtonWidth - ((bevelWidth * 2) +
												gsPopup_MultiArrowWidth)) / 2) + 1;
			leftEdge = bevelWidth + gsPopup_MultiArrowWidth + offset;
			rightEdge = leftEdge - (gsPopup_MultiArrowWidth - 1);
													
			// ее TOP ARROW
			// е Arrow drawing loop draws 4 rows to make the arrow, the top arrow
			// points upwards
			for ( counter = 0; counter <= 3; counter++ )
			{
				::MoveTo ( popupFrame.right - (leftEdge - (3 - counter)),
								popupFrame.top + topStart + counter );
				::LineTo ( popupFrame.right - (rightEdge + (3 - counter)),
								popupFrame.top + topStart + counter );
			}

			// ее BOTTOM ARROW
			// е Arrow drawing loop draws 4 rows to make the arrow, the bottom arrow
			// points downwards
			SInt16		botStart = topStart + 6;
			for ( counter = 0; counter <= 3; counter++ )
			{
				::MoveTo ( popupFrame.right - (leftEdge - counter),
								popupFrame.top + botStart + counter );
				::LineTo ( popupFrame.right - (rightEdge + counter),
								popupFrame.top + botStart + counter );
			}
		}
		else
		{
			// ее For the pulldown menu there is only one arrow drawn that faces down
			// е Figure out the left and right edges based on whether we are drawing
			// only the arrow portion or the entire popup
			SInt16		start = (( UGraphicsUtilities::RectHeight ( popupFrame ) - 
																gsPopup_ArrowHeight) / 2) + 1;
																
			if ( mArrowOnly )
			{
				SInt16	width = UGraphicsUtilities::RectWidth ( popupFrame);
				leftEdge = width - ((width - gsPopup_ArrowWidth) / 2);
				rightEdge = leftEdge - (gsPopup_ArrowWidth - 1);
			}
			else
			{
				leftEdge = gsPopup_ArrowButtonWidth - 6;
				rightEdge = leftEdge - (gsPopup_ArrowWidth - 1);
			}
	
			// е Arrow drawing loop draws 5 rows to make the arrow
			SInt16  counter;
			for ( counter = 0; counter <= 4; counter++ )
			{
				::MoveTo ( popupFrame.right - (leftEdge - counter),
								popupFrame.top + start + counter );
				::LineTo ( popupFrame.right - (rightEdge + counter),
								popupFrame.top + start + counter );
			}
		}
	}
	
}	//	LGAPopup::DrawPopupArrow


//-------------------------------------------------------------------------------------
// LGAPopup::DrawPopupNormalBW
//-------------------------------------------------------------------------------------

void
LGAPopup::DrawPopupNormalBW	()
{

	StColorPenState	saveColorPenState;

	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalPopupFrameRect ( localFrame );

	// е Before we draw anything we need to erase the control in case we
	// we had previously been hilited
	RGBColor theColor = UGAColorRamp::GetWhiteColor();
	::RGBBackColor ( &theColor);
	::InsetRect ( &localFrame, 1, 1 );
	::EraseRoundRect ( &localFrame, 4, 4 );
	::InsetRect ( &localFrame, -1, -1 );
	
	// е Draw the frame for the control
	theColor = UGAColorRamp::GetBlackColor();
	::RGBForeColor ( &theColor );
	::FrameRoundRect ( &localFrame, 8, 8 );
		
}	//	LGAPopup::DrawPopupNormalBW


//-------------------------------------------------------------------------------------
// LGAPopup::DrawPopupHilitedBW
//-------------------------------------------------------------------------------------

void
LGAPopup::DrawPopupHilitedBW	()
{

	StColorPenState	saveColorPenState;
	
	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalPopupFrameRect ( localFrame );

	// е Draw the frame for the control
	RGBColor theColor = UGAColorRamp::GetBlackColor();
	::RGBForeColor ( &theColor );
	::FrameRoundRect ( &localFrame, 8, 8 );
		
	// е Get the frame for the control
	::InsetRect ( &localFrame, 1, 1 );
	::PaintRoundRect ( &localFrame, 4, 4 );
	
}	//	LGAPopup::DrawPopupHilitedBW


//-------------------------------------------------------------------------------------
// LGAPopup::DrawPopupDimmedBW
//-------------------------------------------------------------------------------------

void
LGAPopup::DrawPopupDimmedBW	()
{

	StColorPenState	saveColorPenState;
	
	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalPopupFrameRect ( localFrame );
	
	// е Before we draw anything we need to erase the control in case we
	// we had previously been hilited
	RGBColor theColor = UGAColorRamp::GetWhiteColor();
	::RGBBackColor ( &theColor);
	::InsetRect ( &localFrame, 1, 1 );
	::EraseRoundRect ( &localFrame, 4, 4 );
	::InsetRect ( &localFrame, -1, -1 );
	
	// е Draw the frame for the control
	theColor = UGAColorRamp::GetBlackColor();
	::RGBForeColor ( &theColor );
	::FrameRoundRect ( &localFrame, 8, 8 );
		
}	//	LGAPopup::DrawPopupDimmedBW


//-------------------------------------------------------------------------------------
// LGAPopup::DrawPopupNormalColor
//-------------------------------------------------------------------------------------

void
LGAPopup::DrawPopupNormalColor	()
{

	StColorPenState	savePenState;
	
	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalPopupFrameRect ( localFrame );

	// е Draw the frame for the control outside of everything else
	RGBColor theColor = UGAColorRamp::GetBlackColor();
	::RGBForeColor ( &theColor );
	::FrameRoundRect ( &localFrame, 8, 8 );
	
	// е First make sure the face of the control is drawn
	::InsetRect ( &localFrame, 1, 1 );
	theColor = UGAColorRamp::GetColor(2);
	::RGBForeColor ( &theColor);
	::PaintRoundRect ( &localFrame, 4, 4 );
	::InsetRect ( &localFrame, -1, -1 );
	
	if ( !IsArrowOnly ())
	{
		// ее EDGES ON BODY OF POPUP
		// е LIGHT EDGES
		// е Start by rendering the bevelled edges of the sides facing the light
		// source
		theColor = UGAColorRamp::GetWhiteColor();
		::RGBForeColor ( &theColor);
		::MoveTo ( localFrame.left + 1, localFrame.bottom - 3 );
		::LineTo ( localFrame.left + 1, localFrame.top + 2 );
		::MoveTo ( localFrame.left + 2, localFrame.top + 1 );
		::LineTo ( localFrame.right - gsPopup_ArrowButtonWidth, localFrame.top + 1 );
	
		// е SHADOW EDGES
		// е Render the shadow bevels
		theColor = UGAColorRamp::GetColor(5);
		::RGBForeColor ( &theColor);
		UGraphicsUtilities::BottomRightSide ( &localFrame, 2, 2, 1, gsPopup_ArrowButtonWidth - 1);
		theColor = UGAColorRamp::GetColor(1);
		UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - (gsPopup_ArrowButtonWidth), 
							localFrame.top + 1, &theColor);
	}

	// ее EDGES ON ARROW BUTTON
	// е LIGHT EDGES
	// е Start by rendering the bevelled edges of the sides facing the light
	// source
	// е Setup the appropriate width for the popup arrow portion of the
	// rendering
	SInt16	arrowButtonWidth = gsPopup_ArrowButtonWidth;
	if ( IsArrowOnly ())
		arrowButtonWidth = UGraphicsUtilities::RectWidth ( localFrame );
	
	theColor = UGAColorRamp::GetWhiteColor();
	::RGBForeColor ( &theColor);
	UGraphicsUtilities::TopLeftSide ( 	&localFrame, 
						2, 																			//	TOP
						UGraphicsUtilities::RectWidth ( localFrame ) - (arrowButtonWidth - 2), 	//	LEFT
						3, 																			// BOTTOM
						3 );																			// RIGHT
	theColor = UGAColorRamp::GetColor(4);
	UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 3, localFrame.top + 1, &theColor);
	theColor = UGAColorRamp::GetColor(5);
	UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 2, localFrame.top + 2, &theColor);

	// е SHADOW EDGES
	// е Render the shadow bevels
	theColor = UGAColorRamp::GetColor(8);
	::RGBForeColor ( &theColor);
	::MoveTo ( 	localFrame.left + (UGraphicsUtilities::RectWidth ( localFrame ) - (arrowButtonWidth - 2)), 
					localFrame.bottom - 2 );
	::LineTo ( 	localFrame.right - 3, 
					localFrame.bottom - 2 );
	::MoveTo ( localFrame.right - 2, localFrame.bottom - 3 );
	::LineTo ( localFrame.right - 2, localFrame.top +  3 );

	theColor = UGAColorRamp::GetColor(5);
	::RGBForeColor ( &theColor);
	UGraphicsUtilities::BottomRightSide ( &localFrame, 
							3, 																		//	TOP
							UGraphicsUtilities::RectWidth (localFrame) - (arrowButtonWidth - 3), //	LEFT
							2, 																		// BOTTOM
							2 );																		// RIGHT
	theColor = UGAColorRamp::GetColor(8);
	UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 3, localFrame.bottom - 3, &theColor);

	if ( !IsArrowOnly ())
	{
		theColor = UGAColorRamp::GetColor(4);
		UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - (gsPopup_ArrowButtonWidth - 1), 
							localFrame.bottom - 2, &theColor);
	}
}	//	LGAPopup::DrawPopupNormalColor


//-------------------------------------------------------------------------------------
// LGAPopup::DrawPopupHilitedColor
//-------------------------------------------------------------------------------------

void
LGAPopup::DrawPopupHilitedColor	()
{

	StColorPenState	savePenState;
	
	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalPopupFrameRect ( localFrame );

	// е Draw the frame for the control outside of everything else
	RGBColor theColor = UGAColorRamp::GetBlackColor();
	::RGBForeColor ( &theColor );
	::FrameRoundRect ( &localFrame, 8, 8 );
	
	// е First make sure the face of the control is drawn
	::InsetRect ( &localFrame, 1, 1 );
	theColor = UGAColorRamp::GetColor(9);
	::RGBForeColor ( &theColor);
	::PaintRoundRect ( &localFrame, 4, 4 );
	::InsetRect ( &localFrame, -1, -1 );
	
	if ( !IsArrowOnly ())
	{
		// ее EDGES ON BODY OF POPUP
		// е LIGHT EDGES
		// е Start by rendering the bevelled edges of the sides facing the light
		// source
		theColor = UGAColorRamp::GetColor(10);
		::RGBForeColor ( &theColor);
		::MoveTo ( localFrame.left + 1, localFrame.bottom - 3 );
		::LineTo ( localFrame.left + 1, localFrame.top + 2 );
		::MoveTo ( localFrame.left + 2, localFrame.top + 1 );
		::LineTo ( localFrame.right - (gsPopup_ArrowButtonWidth - 1), 
						localFrame.top + 1 );
	
		// е SHADOW EDGES
		// е Render the shadow bevels
		theColor = UGAColorRamp::GetColor(8);
		::RGBForeColor ( &theColor);
		UGraphicsUtilities::BottomRightSide ( &localFrame, 2, 2, 1, gsPopup_ArrowButtonWidth - 1);
	}

	// ее EDGES ON ARROW BUTTON
	// е LIGHT EDGES
	// е Start by rendering the bevelled edges of the sides facing the light
	// source
	// е Setup the appropriate width for the popup arrow portion of the
	// rendering
	SInt16	arrowButtonWidth = gsPopup_ArrowButtonWidth;
	if ( IsArrowOnly ())
		arrowButtonWidth = UGraphicsUtilities::RectWidth ( localFrame );

	theColor = UGAColorRamp::GetColor(11);
	::RGBForeColor ( &theColor);
	UGraphicsUtilities::TopLeftSide ( 	&localFrame, 
						1, 																			//	TOP
						UGraphicsUtilities::RectWidth ( localFrame ) - (arrowButtonWidth - 1), 	//	LEFT
						1, 																			// BOTTOM
						2 );																			// RIGHT
	theColor = UGAColorRamp::GetColor(10);
	::RGBForeColor ( &theColor);
	UGraphicsUtilities::TopLeftSide ( 	&localFrame, 
						2, 																			//	TOP
						UGraphicsUtilities::RectWidth ( localFrame ) - (arrowButtonWidth - 2), 	//	LEFT
						3, 																			// BOTTOM
						3 );																			// RIGHT

	// е SHADOW EDGES
	// е Render the shadow bevels
	theColor = UGAColorRamp::GetColor(7);
	::RGBForeColor ( &theColor);
	::MoveTo ( 	localFrame.left + (UGraphicsUtilities::RectWidth ( localFrame ) - (arrowButtonWidth - 2)), 
					localFrame.bottom - 2 );
	::LineTo ( 	localFrame.right - 3, 
					localFrame.bottom - 2 );
	::MoveTo ( localFrame.right - 2, localFrame.bottom - 3 );
	::LineTo ( localFrame.right - 2, localFrame.top +  3 );

	theColor = UGAColorRamp::GetColor(8);
	::RGBForeColor ( &theColor);
	UGraphicsUtilities::BottomRightSide ( &localFrame, 
							3, 																		//	TOP
							UGraphicsUtilities::RectWidth (localFrame) - (gsPopup_ArrowButtonWidth - 3), //	LEFT
							2, 																		// BOTTOM
							2 );																		// RIGHT
	theColor = UGAColorRamp::GetColor(7);
	UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 3, localFrame.bottom - 3, &theColor);
	
}	//	LGAPopup::DrawPopupHilitedColor


//-------------------------------------------------------------------------------------
// LGAPopup::DrawPopupDimmedColor
//-------------------------------------------------------------------------------------

void
LGAPopup::DrawPopupDimmedColor	()
{

	StColorPenState	savePenState;
	
	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalPopupFrameRect ( localFrame );

	// е Draw the frame for the control outside of everything else
	RGBColor theColor = UGAColorRamp::GetColor(7);
	::RGBForeColor ( &theColor );
	::FrameRoundRect ( &localFrame, 8, 8 );

	// е First make sure the face of the control is drawn
	::InsetRect ( &localFrame, 1, 1 );
	theColor = UGAColorRamp::GetColor(2);
	::RGBForeColor ( &theColor);
	::PaintRoundRect ( &localFrame, 4, 4 );
	::InsetRect ( &localFrame, -1, -1 );
	
	// е Separator between arrow and body
	if ( !IsArrowOnly ())
	{
		theColor = UGAColorRamp::GetColor(5);
		::RGBForeColor ( &theColor);
		::MoveTo ( 	localFrame.right - gsPopup_ArrowButtonWidth,
						localFrame.top + 2 );
		::LineTo ( localFrame.right - gsPopup_ArrowButtonWidth,
						localFrame.bottom - 2 );
	}
				
}	//	LGAPopup::DrawPopupDimmedColor


#pragma mark -
#pragma mark === MOUSE TRACKING

//=====================================================================================
// ее MOUSE TRACKING
//-------------------------------------------------------------------------------------
// LGAPopup::PointInHotSpot
//-------------------------------------------------------------------------------------
//

Boolean
LGAPopup::PointInHotSpot (	Point 	inPoint,
									SInt16		/*inHotSpot*/ ) const
{

	Rect	popupRect;
	CalcLocalPopupFrameRect ( popupRect );
	
	// е We only return true if the mouse is in the popup portion of the
	// control not in the label
	return ::PtInRect ( inPoint, &popupRect );
	
}	//	LGAPopup::PointInHotSpot


//-------------------------------------------------------------------------------------
// LGAPopup::TrackHotSpot
//-------------------------------------------------------------------------------------
//

Boolean
LGAPopup::TrackHotSpot (	SInt16		inHotSpot,
								Point 	inPoint,
								SInt16		/* inModifiers */ )
{

	// е We only want the popup menu to appear if the mouse went down
	// in the our hot spot which is the popup portion of the control
	// not the label area
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
		
}	//	LGAPopup::TrackHotSpot



//-------------------------------------------------------------------------------------
// LGAPopup::HotSpotAction
//-------------------------------------------------------------------------------------
//
//	We have overridden this method from our superclass as we are not interested in the
//		control mode we always treat the popup as a button regardless of what the user has
//		set the control mode too

void
LGAPopup::HotSpotAction	(	SInt16		/* inHotSpot */,
									Boolean	inCurrInside,
									Boolean	inPrevInside )
{

	// е If the mouse moved in or out of the hot spot handle the
	// hiliting of the control, NOTE: for the popup we ignore the
	// control mode as it does not make any sense to have a popup
	// that behaves like either a radio button or a switch
	if ( inCurrInside != inPrevInside )
	{
		// е Toggle the hilite state of the control
		SetHiliteState ( inCurrInside );
	}
		
}	//	LGAPopup::HotSpotAction


//-------------------------------------------------------------------------------------
// LGAPopup::HotSpotResult
//-------------------------------------------------------------------------------------
//
//	We have overridden this method so that the hiliting of the menu will get turned off

void
LGAPopup::HotSpotResult	(	SInt16 inHotSpot )
{
	
	// е Make sure that by default we turn the hiliting off
	HotSpotAction ( inHotSpot, false, true );
		
}	//	LGAPopup::HotSpotResult


#pragma mark -
#pragma mark === POPUP MENU HANDLING

//=====================================================================================
// ее POPUP MENU HANDLING
//-------------------------------------------------------------------------------------
// LGAPopup::HandlePopupMenuSelect
//-------------------------------------------------------------------------------------

void
LGAPopup::HandlePopupMenuSelect	(	Point		inPopupLoc,
												SInt16		inCurrentItem,
												SInt16		&outMenuID,
												SInt16		&outMenuItem )
{
	
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
	
}	//	LGAPopup::HandlePopupMenuSelect


#pragma mark -
#pragma mark === MISCELLANEOUS

//=====================================================================================
// ее MISCELLANEOUS
//-------------------------------------------------------------------------------------
// LGAPopup::LoadResourceMenu
//-------------------------------------------------------------------------------------
//

void
LGAPopup::LoadResourceMenu	(	MenuHandle	inMenu ) const
{

	// е Get all of the resources of a specific type loaded into the
	// menu passed in
	if ( UseResourceMenu () && inMenu )
	{
		::AppendResMenu ( inMenu, GetResourceMenuType () );
	}
		
}	//	LGAPopup::LoadResourceMenu


//-------------------------------------------------------------------------------------
// LGAPopup::AdjustPopupWidth
//-------------------------------------------------------------------------------------
//

void
LGAPopup::AdjustPopupWidth	()
{
	
	// е We are going to look in the menu info record to see what the maximum
	// width of the menu has been setup as, we then use that to figure out the
	// width for the popup portion of the popup.  To handle this correctly we
	// need to do the skanky low memory global tweaking we do when displaying
	// the menu to make sure that menu size is calculated for the correct text
	// trait settings, otherwise the size is based on Chicago 12 which means 
	// we could end up resizing the popup to the wrong size
	if ( mCachedMenuH )
	{
		
		// е Get the rect that represents the frame of the control portion of
		// the popup
		Rect popupFrame;
		CalcLocalPopupFrameRect ( popupFrame );
		
		// е Save off the current system font family and size
		SInt16 saveFont = ::LMGetSysFontFam ();
		SInt16 saveSize = ::LMGetSysFontSize ();
		
		// е Enclose this all in a try catch block so that we can
		// at least reset the system font if something goes wrong
		try 
		{

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

			// е Get the menu size recalculated
			::CalcMenuSize ( mCachedMenuH );
		
		}
		catch (...) 
		{
			// ignore errors╔
		}

		// е Figure out the menu width and popup width
#if PP_Target_Carbon
		SInt16 menuWidth = GetMenuWidth(mCachedMenuH);
#else
		SInt16 menuWidth = (*mCachedMenuH)->menuWidth;
#endif
		SInt16 popupWidth = UGraphicsUtilities::RectWidth ( popupFrame );
		SInt16 newWidth = 0;
		
		// е We set the new width if the popup width and menu width differ
		if ( popupWidth != menuWidth )
			newWidth = menuWidth;
		
		// е We only adjust the size if there is a new width
		if ( newWidth > 0 )
		{
			// е Set the width of the popup to be the new width plus the 
			// label width and an additional amount that is based on the
			// width of the button portion of the popup.  If we don't
			// add this additional amount the longest item in the menu will
			// always end up beig truncated which is not what we want
			newWidth += mLabelWidth + ((gsPopup_ArrowButtonWidth / 2) + 2);
			
			// е Get the current frame size
			SDimension16 frameSize;
			GetFrameSize ( frameSize );
			
			// е Now setup the new width for the popup
			ResizeFrameTo ( newWidth, frameSize.height, true );
		}

		// е Restore the system font
		::LMSetSysFontFam ( saveFont );
		::LMSetSysFontSize ( saveSize );
		::LMSetLastSPExtra ( -1L );

	}
		
}	//	LGAPopup::AdjustPopupWidth


//-------------------------------------------------------------------------------------
// LGAPopup::ResizeForArrowOnly
//-------------------------------------------------------------------------------------

void
LGAPopup::ResizeForArrowOnly ()
{

	// е If the popup is set o be arrow obnly then we will automatically make it
	// the minimum recommended size, if it isn't already
	SDimension16	newSize = {mFrameSize.width, mFrameSize.height};
	if ( mArrowOnly && mFrameSize.width < gsPopup_ArrowButtonWidth )
		newSize.width = gsPopup_ArrowButtonWidth;
		
	if ( mArrowOnly && mFrameSize.height < gsPopup_ArrowButtonHeight )
		newSize.height = gsPopup_ArrowButtonHeight;
		
	// е Now adjust the size if needed
	if ( newSize.width != mFrameSize.width || newSize.height != mFrameSize.height )
		ResizeFrameTo ( newSize.width, newSize.height, true );
		
}	//	LGAPopup::ResizeForArrowOnly

	
