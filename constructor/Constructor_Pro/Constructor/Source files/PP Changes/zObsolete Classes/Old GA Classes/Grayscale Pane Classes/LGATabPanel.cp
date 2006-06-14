// Copyright й2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyrightй 1996 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< е >>-----------------------------------------

FILE:					LGATabPanel.cp

CLASSES:				LGATabPanel

DESCRIPTION:		Implementation file for LGATabPanel

AUTHOR:				Robin Mair

CREATION DATE :	96.12.12

CHANGE HISTORY :

		97.11.26		rtm	Fixed a real bad little bug that would never allow a large size
								tab button to be created, oops! The fix is in CalcTabButtonSize
								where the initial tab size had been set to 10 but then in the
								loading of the text trait if the text trait was setup to be
								the System font [0] no handle is loaded as it is built into PP
								and therefore the large size never got selected.  The fix was
								to set the initial value to 12.
		97.05.19		rtm	Changed the way a unique ID is built for a button, we now keep
								a running counter of assigned indices that is used to get an
								index that is used to build the ID for a button.  These indices
								are not currently recycled so if you repeatedly add and remove
								buttons you could run out of indices, the limit is 99 due to
								the formatting of the button ID which has 'tb' with the index
								appended. To get around this problem there is now code that
								can handle the compacting of the indices if they go out of
								range, this way we can never have IDs for the buttons that are
								wrong.
		97.05.18		rtm	Made a lot of changes to the whole API around the managing of
								the tab buttons.  A parallel list of page titles is no longer
								maintained so all of the calls that were associated with this
								list have been removed.  Any calls that deal with titles now
								get these titles from the buttons themselves.
								In addition, the API has been extended to support inserting
								and removing tab buttons, this also includes allwoing all of
								the buttons to be deleted.
								Obsolete methods: AddTabtitle, DeleteAllTabTitles.
								New methods: InsertTabButtonsAt, RemoveTabButtonAt, 
								GetTabButtonCount, DeleteAllTabButtons, GetNextIndex,
								ClearIndexCounter.
		97.04.20		rtm	Removed the CreateFromStream method.
		97.01.24		rtm	Added and additional parameterized constructor that allows the
								tab titles to be created from a string list resource.
		97.01.24		rtm	Added a copy constructor.
		97.01.20		rtm	Fixed a bug in BuildPanelHostView where the width instead of
								the height of rect was being used in the calculation.
		96.12.18		rtm	Completely changed the way the loading of tab titles is 
								handled, we now use the new capabilities of the CTyp resource
								to add the titles as a list directly within the PPob.  This is
								done from within the property inspector in Constructor.
		96.12.17		rtm	Changed the title loading code so that if we are handed a
								stringlist that has an ID of 0 we ignore it and do nothing.
		96.12.17		rtm	Fixes to get the disabling of the panel to show up correctly.
		96.12.16		rtm	The methods for setting the selected button have been changed
								so that they now have an additional parameter that can be used 
								to control whether the selected buttons value message is
								broadcast or not when the button is selected.  By default
								this parameter is true [suppress broadcast] as we don't
								want broadcasts when we are setting buttons up internally.
								Look in the GASample application to see how this is used if you
								want to be able to set a button procedurally and have the
								message be broadcast so the normal behavior of selecting the
								button will happen in your application.  As part of this the
								ListenToMessage method was also modified to account for the
								new approach to handling things.
		96.12.16		rtm	Changed the ActivateSelf and DeactivateSelf methods so that
								they now call refresh instead of draw.  Also got rid of the
								overrides of Activate and Deactivate where we were forcing
								the port to update immediately, this was causing a lot of
								unnecessary drawing when the panel was first displayed in a 
								window.  Unfortunately this change means that there is a lot
								more flickering when a panel is activated and deactivated at
								any time other than the opening of a window.
		96.12.16		rtm	Changed the panel host view so that it now is an LPlaceHolder
								view which does all the neat alignment stuff for us when we 
								install a panel in it, our API simply calls through to its API.
								As part of this process also added two new fields to the class
								that allow the user to specify whether the placeholder should
								be built or not and what alignment to use if it is built.  The
								alignment is specified using the alignment constants defined
								for the Icon Suite stuff [Icons.h].
		96.12.15		rtm	Added a method that can be used to retrieve a tab buttons title
								based on an index, this is there primarily for use when
								responding to a message from the panel which returns the index
								of the tab that was selected.
		96.12.15		rtm	Added an API for adding and removing panels that can be called
								by the user to get panels installed inside the tab panel. We 
								probably need to add some sort of flag to the constructors that
								will allow the user to define whether this panel actually gets 
								created or not as they might want to do their own thing inside
								the panel.
		96.12.15		rtm	Added a host view inside the panel into which the users panels
								are installed, we use this approach as it minimizes the amount
								of flickering that occurs when panels are added and removed
								in that clipping will be set to the host view when the tab
								panel is refreshed which will not cause the tab buttons to be
								redrawn unnecessarily in the process.
		96.12.14		rtm	Added some additional clipping when the border of the panel is
								being drawn that essentially clips out the area where the tab
								buttons will be drawn, this helps minimize the drawing that is
								seeing especially the drawing of the black frame around the
								panel.  A new method was added that calculates this rect.
		96.12.14		rtm	Changed the size of the button view so that its height now
								matches that of the size button being used.
		96.12.12		rtm	Initial creation of file

---------------------------------------<< е >>-----------------------------------------
*/

#ifdef PowerPlant_PCH
	#include PowerPlant_PCH
#endif

// е GRAYSCALE HEADERS
#include <LGATabPanel.h>
#include <LGATabButton.h>
#include <UGraphicsUtilities.h>
#include <UGAColorRamp.h>

// е POWERPLANT HEADERS
#include <LView.h>
#include <LArray.h>
#include <LStream.h>
#include <LVariableArray.h>
#include <LArrayIterator.h>
#include <UTextTraits.h>
#include <UDrawingUtils.h>
#include <UDrawingState.h>
#include <LString.h>
#include <PP_Messages.h>
#include <URegistrar.h>
#include <LPlaceHolder.h>

// е TOOLBOX HEADERS
#include <Quickdraw.h>
#include <TextEdit.h>
#include <MacWindows.h>

//#include <stdlib.h>

#pragma mark === CLASS: LGATabPanel

//=====================================================================================
// CONSTANTS
//=====================================================================================

enum
{

	tabPanel_TabButtonSlope 		= 11,
	tabPanel_10TabButtonHeight 	= 19,
	tabPanel_12TabButtonHeight 	= 24,
	tabPanel_BottomOffset 			= 3,
	tabPanel_LeftEdgeOffset 		= 7
	
};


//=====================================================================================
// DEFINES
//=====================================================================================

//=====================================================================================
// CLASS:	LGATabPanel
//=====================================================================================

#pragma mark -
#pragma mark === INITIALIZATION & DESTRUCTION

//=====================================================================================
// ее INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LGATabPanel::LGATabPanel					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Default constructor

LGATabPanel::LGATabPanel ()
{

	mSelectedButton = nil;
	mPanelMessage = msg_AnyMessage;
	mTabButtons = nil;
	mPanelHost = nil;
	mStringListResID = 0;
	mStringListCount = 0;
	mSelectionIndex = 0;
	mInitialTabTitles = nil;
	mWantPlaceHolder = true;
	mPanelAlignment = atNone;
	mNextIndex = 0;
	
}	//	LGATabPanel::LGATabPanel


//-------------------------------------------------------------------------------------
// LGATabPanel::LGATabPanel					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Copy constructor

LGATabPanel::LGATabPanel (	const LGATabPanel	&inOriginal )
							:	LView ( inOriginal )
{

	mSelectedButton = inOriginal.mSelectedButton;
	mPanelMessage = inOriginal.mPanelMessage;
	mTabButtons = nil;
	mPanelHost = nil;
	mStringListResID = inOriginal.mStringListResID;
	mStringListCount = inOriginal.mStringListCount;
	mSelectionIndex = inOriginal.mSelectionIndex;
	mInitialTabTitles = nil;
	
	// е Build up the temporary list that will be used to build the actual tab buttons
	// from the FinishCreateSelf method, this means that we have to be sure to call
	// FinishCreate when we copy a tab button
	if ( mStringListResID == 0  && mStringListCount > 0 )
	{
		LVariableArray* titleArray = new LVariableArray ();
		LArray& tabButtons = GetTabButtons ();
		LArrayIterator iterator ( tabButtons, LArrayIterator::from_Start );
		Str255	theTitle;
		LGATabButton*	aTabButton = nil;
		while ( iterator.Next ( aTabButton )) 
		{
			if ( aTabButton )
			{
				aTabButton->GetDescriptor ( theTitle );
				
				// е Add the title to the array
				titleArray->InsertItemsAt ( 	1, 						//	Number of items
														LArray::index_Last,	//	Position in array
														theTitle, 				//	Item being added
														theTitle[0] + 1 );	//	Size of item being added
			}
		}
		
		// е Now we store the array in our temporary field
		mInitialTabTitles = titleArray;
	}

	mWantPlaceHolder = inOriginal.mWantPlaceHolder;
	mPanelAlignment = inOriginal.mPanelAlignment;
	mNextIndex = inOriginal.mNextIndex;
	
}	//	LGATabPanel::LGATabPanel


//-------------------------------------------------------------------------------------
// LGATabPanel::LGATabPanel					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Stream constructor

LGATabPanel::LGATabPanel (	LStream* inStream ) : LView ( inStream )
{

	mSelectedButton = nil;
	mTabButtons = nil;
	mPanelHost = nil;
	mStringListResID = 0;
	mStringListCount = 0;
	mSelectionIndex = 0;
	mInitialTabTitles = nil;
	mNextIndex = 0;

	// е Load the text traits ID
	*inStream >> mTextTraitsID;
	
	// е Load the controller message from the stream
	*inStream >> mPanelMessage;
	
	// е Read in the count of the number of title strings	
	*inStream >> mStringListCount;
	
	// е If the count is greater than 0 we are going to build a variable
	// array of titles that will be used to build the actual tab buttons
	if ( mStringListCount > 0 )
	{
		LVariableArray* titleArray = new LVariableArray ();
		Str255	titleString;
		for ( SInt16	index = 0; index < mStringListCount; index++ )
		{
			// е Read in the title
			inStream->ReadPString ( titleString );
			
			// е If we already have a tab title array then we simply add the
			// new title to the list
			titleArray->InsertItemsAt ( 	1, 						//	Number of items
													LArray::index_Last,		//	Position in array
													titleString, 			//	Item being added
													titleString[0] + 1 );//	Size of item being added
		}
		
		// е Now we store the array in our temporary field
		mInitialTabTitles = titleArray;
	}
	
	// е read in the tab button that will be initially selected
	*inStream >> mSelectionIndex;
	
	// е Load the data for the placeholder view
	*inStream >> mWantPlaceHolder;
	*inStream >> mPanelAlignment;
	
}	//	LGATabPanel::LGATabPanel


//-------------------------------------------------------------------------------------
// LGATabPanel::LGATabPanel					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Parameterized constructor

LGATabPanel::LGATabPanel (	const SPaneInfo 	&inPaneInfo,
									const SViewInfo 	&inViewInfo,
									ResIDT				inTextTraitsID,
									LVariableArray*	inTabTitles,
									ArrayIndexT			inSelectionIndex,
									MessageT				inMessage,
									Boolean				inWantPlaceHolder,
									SInt16					inAlignment ) 
							: LView ( 	inPaneInfo,
											inViewInfo )
{

	mSelectedButton = nil;
	mTabButtons = nil;
	mPanelHost = nil;
	mStringListResID = 0;
	mStringListCount = 0;
	mSelectionIndex = 0;
	mInitialTabTitles = nil;
	mNextIndex = 0;
	
	// е Setup the text traits field
	mTextTraitsID = inTextTraitsID;
	
	// е Tab title count
	if ( inTabTitles )
		mStringListCount = inTabTitles->GetCount ();
		
	// е Setup the temporary storage for the array of titles
	mInitialTabTitles = inTabTitles;
	
	// е Setup the index for the initally selected button
	mSelectionIndex = inSelectionIndex;
	
	// е Set the message that will be sent when a button in the
	// tab panel is selected
	mPanelMessage = inMessage;
	
	// е Setup the data used to build the placeholder for the user
	// defined panels that will be installed in the tab panel
	mWantPlaceHolder = inWantPlaceHolder;
	mPanelAlignment = inAlignment;
	
}	//	LGATabPanel::LGATabPanel


//-------------------------------------------------------------------------------------
// LGATabPanel::LGATabPanel					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Parameterized constructor

LGATabPanel::LGATabPanel (	const SPaneInfo 	&inPaneInfo,
									const SViewInfo 	&inViewInfo,
									ResIDT				inTextTraitsID,
									ResIDT				inStringListID,
									ArrayIndexT			inSelectionIndex,
									MessageT				inMessage,
									Boolean				inWantPlaceHolder,
									SInt16					inAlignment ) 
							: LView ( 	inPaneInfo,
											inViewInfo )
{

	mSelectedButton = nil;
	mTabButtons = nil;
	mPanelHost = nil;
	mStringListCount = 0;
	mSelectionIndex = 0;
	mInitialTabTitles = nil;
	mStringListCount = 0;
	mNextIndex = 0;
	
	// е Setup the class fields
	mTextTraitsID = inTextTraitsID;
	mStringListResID = inStringListID;
	mSelectionIndex = inSelectionIndex;
	mPanelMessage = inMessage;
	mWantPlaceHolder = inWantPlaceHolder;
	mPanelAlignment = inAlignment;
	
}	//	LGATabPanel::LGATabPanel


//-------------------------------------------------------------------------------------
// LGATabPanel::~LGATabPanel				---<<е DESTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LGATabPanel::~LGATabPanel ()
{
	
	// е Clear out the references
	mTabButtons = nil;
	mPanelHost = nil;
	
}	//	LGATabPanel::~LGATabPanel


//-------------------------------------------------------------------------------------
// LGATabPanel::FinishCreateSelf
//-------------------------------------------------------------------------------------

void
LGATabPanel::FinishCreateSelf ()
{

	// е Build the view that holds the tab buttons
	BuildTabButtonView ();
	
	// е Build the view that is used to house panels when they are installed
	// into the tab panel
	if ( mWantPlaceHolder )
		BuildPanelHostView ();
	
	// е Now call our function that handles the actual loading of the tab
	// titles and buttons from the initial array of titles we were handed
	// we only do this is we have an initial array of titles, otherwise we
	// check to see if string resource is being used, if it is then we
	// load the tab titles from the resource
	if ( mInitialTabTitles )
	{
		LoadTabTitlesFromArray ( mInitialTabTitles, mSelectionIndex );

		// е Cleanup by deleting the array of titles
		delete mInitialTabTitles;

		// е Reset our reference which is only ever used temporarily
		mInitialTabTitles = nil;
	
	}
	else if ( mStringListResID > 0 )
		LoadTabTitlesFromStringList ( mStringListResID, mSelectionIndex );
	
	// е Make sure that we are listening for changes
	StartListening ();

}	//	LGATabPanel::FinishCreateSelf


#pragma mark -
#pragma mark === ACCESSORS

//=====================================================================================
// ее ACCESSORS
//-------------------------------------------------------------------------------------
// LGATabPanel::GetCurrentTabIndex												
//-------------------------------------------------------------------------------------

ArrayIndexT
LGATabPanel::GetCurrentTabIndex ()
{
	
	// е If there is a currently selected tab then we proceed to use its title
	// to figure out the index
	if ( GetSelectedTabButton ())
	{
		LGATabButton* currButton = GetSelectedTabButton ();
		LArray&	tabButtons = GetTabButtons ();

		// е Search through the list for the index
		return tabButtons.FetchIndexOf ( &currButton );
		
	}
	
	// е There was no selected button so return nothing, this should
	// never happen
	return 0;
		
}	//	LGATabPanel::GetCurrentTabIndex


//-------------------------------------------------------------------------------------
// LGATabPanel::GetTabIndex												
//-------------------------------------------------------------------------------------

ArrayIndexT
LGATabPanel::GetTabIndex ( ConstStr255Param inTabTitle )
{

	// е Get the index for the page title passed in
	LArray&	tabButtons = GetTabButtons ();
	LArrayIterator iterator ( tabButtons, LArrayIterator::from_Start );
	LGATabButton*	aTabButton = nil;
	while ( iterator.Next ( aTabButton )) 
	{
		if ( aTabButton )
		{
			Str255	theTitle;
			aTabButton->GetDescriptor ( theTitle );
			if ( theTitle == inTabTitle )
			{
				// е Return the index of the current item
				return iterator.GetCurrentIndex ();
			}
		}
	}
	
	// е Couldn't find the title so we simply return zero
	return 0;
	
}	//	LGATabPanel::GetTabIndex


//-------------------------------------------------------------------------------------
// LGATabPanel::CalcTabButtonWidth													
//-------------------------------------------------------------------------------------

SInt16
LGATabPanel::CalcTabButtonWidth ( ConstStr255Param inTabTitle )
{
	
	StTextState			theTextState;
	StColorPenState	thePenState;

	// е Start by making sure we setup the correct textstyle for
	// the view so that the title widths can be correctly calculated
	ResIDT	textTID = GetTextTraitsID ();
	UTextTraits::SetPortTextTraits ( textTID );
	
	// е Return the width of the title plus the button slop
	return ( ::StringWidth ( inTabTitle ) + (tabPanel_TabButtonSlope * 2) );
	
}	//	LGATabPanel::CalcTabButtonWidth


//-------------------------------------------------------------------------------------
// LGATabPanel::CalcLocalTabPanelRect													
//-------------------------------------------------------------------------------------

void
LGATabPanel::CalcLocalTabPanelRect (	Rect	&outRect )
{
	
	StColorPenState	thePenState;

	CalcLocalFrameRect ( outRect );
	
	// е We need to adjust the top of the panel down so that we can accomodate
	// the tab buttons, the amount is based on the button size
	outRect.top += CalcTabButtonSize () == 10 ? tabPanel_10TabButtonHeight - 3 :
									tabPanel_12TabButtonHeight - 3;
	
}	//	LGATabPanel::CalcLocalTabPanelRect


//-------------------------------------------------------------------------------------
// LGATabPanel::CalcTabButtonSize													
//-------------------------------------------------------------------------------------

SInt16
LGATabPanel::CalcTabButtonSize ()
{
	
	StTextState			theTextState;

	SInt16 tabSize = 12;
	
	// е Look in the associated text trait to figure out what the
	// point size forthe font is set to be so we can correctly
	// determine the size of the tab which can only be either 10 or 12
	ResIDT	textTID = GetTextTraitsID ();
	TextTraitsH traitsH = UTextTraits::LoadTextTraits ( textTID );
	if ( traitsH ) 
	{
		tabSize =  (**traitsH).size;
		if ( tabSize <= 10 )
			tabSize = 10;
		else if ( tabSize > 12 )
			tabSize = 12;
	}
	
	// е Return the tab size which affects the tabs height
	return tabSize;
	
}	//	LGATabPanel::CalcLocalTabPanelRect


//-------------------------------------------------------------------------------------
// LGATabPanel::CalcTabButtonClipping													
//-------------------------------------------------------------------------------------

void
LGATabPanel::CalcTabButtonClipping ( Rect	&outRect )
{
	
	// е Figure out a rectangle that will be used when we are drawing
	// the actual panel to clip out the area occupied by the buttons
	// what we are looking for is to end up with a rect that can be
	// used to clip out the drawing of the top edge of the panel where
	// the buttons are located
	CalcLocalTabPanelRect ( outRect );
	outRect.left += tabPanel_LeftEdgeOffset;
	outRect.bottom = outRect.top + 3;
	outRect.right = outRect.left + (GetLastTabButtonLocation () - 6);															
	
}	//	LGATabPanel::CalcTabButtonClipping


//-------------------------------------------------------------------------------------
// LGATabPanel::SetTextTraitsID
//-------------------------------------------------------------------------------------

void
LGATabPanel::SetTextTraitsID	( 	ResIDT inTextTraitsID )
{

	mTextTraitsID = inTextTraitsID;
	
}	//	LGATabPanel::SetTextTraitsID


//-------------------------------------------------------------------------------------
// LGATabPanel::SetSelectedTabButton
//-------------------------------------------------------------------------------------

void
LGATabPanel::SetSelectedTabButton (	LGATabButton*	inSelectedButton,
												Boolean			inSelectButton,
												Boolean			inSuppressBroadcast )
{
	
	if ( GetSelectedTabButton () != inSelectedButton )
	{
		// е If we are going to select the button then we need make sure that
		// we turn off the current selection
		if ( mSelectedButton && inSelectButton )
			ToggleTabButtonState ( mSelectedButton, false, inSuppressBroadcast );
		
		// е Setup the reference to the button
		mSelectedButton = inSelectedButton;

		// е If as part of the process we are going to get the button
		// to show up selected then do it now
		if ( inSelectButton )
		{
			ToggleTabButtonState ( inSelectedButton, true, inSuppressBroadcast );
		}
	}
	
}	//	LGATabPanel::SetSelectedTabButton


//-------------------------------------------------------------------------------------
// LGATabPanel::SetSelectedTabButtonByIndex
//-------------------------------------------------------------------------------------

void
LGATabPanel::SetSelectedTabButtonByIndex (	ArrayIndexT	inSelectionIndex,
															Boolean		inSuppressBroadcast )
{	

	// е Now get the titles loaded from the array we were handed
	SInt16	tabCount = GetTabButtonCount ();
	if ( tabCount > 0 && inSelectionIndex <= tabCount )
	{
		// е Now find the button associated with this index
		LArray&	buttonList = GetTabButtons ();
		LGATabButton* tabButton;
		buttonList.FetchItemAt ( inSelectionIndex, &tabButton );
		if ( tabButton )
		{
			SetSelectedTabButton ( tabButton, true, inSuppressBroadcast );
		}
	}

}	//	LGATabPanel::SetSelectedTabButtonByIndex


//-------------------------------------------------------------------------------------
// LGATabPanel::ToggleTabButtonState						
//-------------------------------------------------------------------------------------

void
LGATabPanel::ToggleTabButtonState (	LGATabButton*	inSelectedButton,
												Boolean			inSelected,
												Boolean			inSuppressBroadcast )
{
	
	if ( inSelectedButton->IsSelected () != inSelected )
	{
		// е Either turn the button on and get its value toggled then redraw
		// everything, we redraw by calling the draw routine directly so
		// that things happen immeditiately, we only broadcast the change
		// if it is not being suppressed
		if ( inSuppressBroadcast )
		{
			MessageT storeMessage = inSelectedButton->GetValueMessage ();
			inSelectedButton->SetValueMessage ( 0 );
			inSelectedButton->SetValue ( inSelected ? 1 : 0 );
			inSelectedButton->SetValueMessage ( storeMessage );
		}
		else
		{
			inSelectedButton->SetValue ( inSelected ? 1 : 0 );
		}
			
		// е Now get the button redrawn
		inSelectedButton->Draw ( nil );
	}
	
}	//	LGATabPanel::ToggleTabButtonState


#pragma mark -
#pragma mark === PANELS

//=====================================================================================
// ее PANELS
//-------------------------------------------------------------------------------------
// LGATabPanel::InstallPanel
//-------------------------------------------------------------------------------------

void
LGATabPanel::InstallPanel ( 	LPane* 	inPanel,
										SInt16		inAlignment,
										Boolean	inRefresh )
{
	
	// е We add the panel to the pael host and then get the host
	// refreshed so the panel shows up, if needed. The panel host
	// is an instance of the LPlaceHolder class so we let it handle
	// all the details of aligning the panel
	if ( mPanelHost )
	{
		
		// е Install the panel
		mPanelHost->InstallOccupant ( inPanel, inAlignment );
		
		// е Refresh the host if needed
		if ( inRefresh )
			mPanelHost->Refresh ();
	}
	
}	//	LGATabPanel::InstallPanel


//-------------------------------------------------------------------------------------
// LGATabPanel::RemovePanel
//-------------------------------------------------------------------------------------

LPane*
LGATabPanel::RemovePanel ()
{
	
	// е We call the placeholder to get the panel removed, the placeholder
	// will restore any size changes that were made when the panel was
	// installed, typically this does not need to be called as the place
	// holder does this automatically when a new panel is installed
	if ( mPanelHost )
	{
		// е Remove the panel from the placeholder and return the panel
		return mPanelHost->RemoveOccupant ();
	}
	else
		return nil;
		
}	//	LGATabPanel::RemovePanel


#pragma mark -
#pragma mark === ACTIVATION

//=====================================================================================
// ее ACTIVATION
//-------------------------------------------------------------------------------------
// LGATabPanel::ActivateSelf
//-------------------------------------------------------------------------------------

void
LGATabPanel::ActivateSelf ()
{

	// е We have to use refresh here even though it causes a lot more
	// flickering because an activate happens when the window the panel
	// is displayed in is first opened which causes the panel to be drawn
	// twice if we use a call to Draw, this minimizes that at the expense
	// of clean drawing later on in the game.
	if ( mActive == triState_On )
		Refresh ();
			
		
}	//	LGATabPanel::ActivateSelf



//-------------------------------------------------------------------------------------
// LGATabPanel::DeactivateSelf
//-------------------------------------------------------------------------------------

void
LGATabPanel::DeactivateSelf ()
{

	// е We have to use refresh here even though it causes a lot more
	// flickering because an activate happens when the window the panel
	// is displayed in is first opened which causes the panel to be drawn
	// twice if we use a call to Draw, this minimizes that at the expense
	// of clean drawing later on in the game.
	if ( mActive == triState_Off || mActive == triState_Latent )
		Refresh ();
						
}	//	LGATabPanel::DeactivateSelf


#pragma mark -
#pragma mark === ENABLING & DISABLING

//=====================================================================================
// ее ENABLING & DISABLING
//-------------------------------------------------------------------------------------
// LGATabPanel::EnableSelf
//-------------------------------------------------------------------------------------
//

void
LGATabPanel::EnableSelf ()
{

	// е Redraw the panel so that the state change takes effect
	Refresh ();

}	//	LGATabPanel::EnableSelf


//-------------------------------------------------------------------------------------
// LGATabPanel::DisableSelf
//-------------------------------------------------------------------------------------
//

void
LGATabPanel::DisableSelf ()
{

	// е Redraw the panel so that the state change takes effect
	Refresh ();

}	//	LGATabPanel::DisableSelf


#pragma mark -
#pragma mark === TAB BUTTON MANAGEMENT

//=====================================================================================
// ее TAB BUTTON MANAGEMENT
//-------------------------------------------------------------------------------------
// LGATabPanel::AddTabButton														
//-------------------------------------------------------------------------------------

void
LGATabPanel::AddTabButton (	Str255  	&inTabTitle,
										Boolean  inSelected )
{
	
	// е Now we need to get a button built for the the new title
	// this button is then added to the end of the button view
	// we start by getting an ID for the button, if for some reason
	// we run out of indices we will do a compact
	PaneIDT	newButtonID;
	GetTabButtonID ( newButtonID );
	
	// е Figure out the location at which the button will be placed
	SPoint32 	buttonLoc;
	buttonLoc.h = GetLastTabButtonLocation ();
	buttonLoc.v = 0;
	
	// е Figure out the size of the button
	Rect localFrame;
	mTabButtons->CalcLocalFrameRect ( localFrame );
	SDimension16	buttonSize;
	buttonSize.width = CalcTabButtonWidth ( inTabTitle );
	if ( CalcTabButtonSize () == 10 )
		buttonSize.height = tabPanel_10TabButtonHeight;
	else
		buttonSize.height = tabPanel_12TabButtonHeight;
	
	// е Get the button created
	CreateTabButton ( 	inTabTitle,
								newButtonID,
								buttonSize,
								buttonLoc,
								inSelected );	

}	//	LGATabPanel::AddTabButton


//-------------------------------------------------------------------------------------
// LGATabPanel::InsertTabButtonAt														
//-------------------------------------------------------------------------------------

void
LGATabPanel::InsertTabButtonAt (	Str255  		&inTabTitle,
											ArrayIndexT inAtIndex )
{

	// е Before we proceed we make sure that the index we have been handed
	// is not out of range, if it is greater than the current button count
	// then we are going to simply call AddTabButton which will get the
	// new button appended to the end of the button list
	SInt16	buttonCount = GetTabButtonCount ();
	Boolean compactIDs = false;
	ArrayIndexT buttonIndex = GetCurrentIndex () + 1;
	if ( inAtIndex > 0 && inAtIndex <= buttonCount )
	{

		// е We start out by first checking to make sure that there is a valid
		// index available for this button otherwise we will need to get the
		// indices compacted
		if ( buttonIndex > 99 )
			compactIDs = true;
	
		// е Now we need to get a button built for the the new title
		// this button is then added to the end of the button view
		// е Now get an ID for the new button
		PaneIDT	newButtonID;
		GetTabButtonID ( newButtonID );
		
		// е Figure out the button location
		LArray&	buttonList = GetTabButtons ();
		SPoint32 	buttonLoc;
		LGATabButton*	aTabButton;
		buttonList.FetchItemAt ( inAtIndex, &aTabButton );
		aTabButton->GetFrameLocation ( buttonLoc );
		
		// е Was the button selected
		Boolean	wasSelected = aTabButton->IsSelected ();
		
		// е We need to make sure that we subtract the amount of inset that
		// exists at the left edge of the panel as the coordinates of the button
		// will get adjusted when it gets added to the button view, as it is a
		// control and uses the coordinate system of its superview
		buttonLoc.h -= tabPanel_LeftEdgeOffset;
		
		// е Vertical location of the button is always 0
		buttonLoc.v = 0;
		
		// е Figure out the size of the button
		Rect localFrame;
		mTabButtons->CalcLocalFrameRect ( localFrame );
		SDimension16	buttonSize;
		buttonSize.width = CalcTabButtonWidth ( inTabTitle );
		buttonSize.height = UGraphicsUtilities::RectHeight ( localFrame );
		
		// е Get the button created
		CreateTabButton ( 	inTabTitle,
									newButtonID,
									buttonSize,
									buttonLoc,
									false );	

		// е Now we have to manipulate its location in the list as it is important
		// that it be in the correct location
		ArrayIndexT	lastIndex = GetTabButtonCount ();
		
		// е Move the item fromm the end of the list to the insertion location
		buttonList.MoveItem ( lastIndex, inAtIndex );
		
		// е Finally, we need to get the locations of all the buttons after 
		// this one adjusted to accomodate the new button
		buttonCount = buttonList.GetCount ();
		for ( SInt16 index = inAtIndex + 1; index <= buttonCount; index++ )
		{
			buttonList.FetchItemAt ( index, &aTabButton );
			if ( aTabButton )
			{	
				aTabButton->MoveBy ( buttonSize.width, 0, true );
			}
		}
		
		// е If the tab that was at this location was selected then we need to
		// make sure we do the same for the new button
		if ( wasSelected )
			SetSelectedTabButtonByIndex ( inAtIndex );
			
	}
	else if ( inAtIndex > buttonCount )
	{	
		// е We start out by first checking to make sure that there is a valid
		// index available for this button otherwise we will need to get the
		// indices compacted
		if ( buttonIndex > 99 )
			compactIDs = true;
	
		// е Add the button to the end of the button list
		AddTabButton ( inTabTitle, false );
	}
	
	// е If we are going to handle synchronizing the tab button IDs then we
	// do it now
	if ( compactIDs )
		RebuildButtonIdentifiers ();

	// е Get the tab button view refreshed so the new button shows up
	LView*	tabButtonView = GetTabButtonView ();
	if ( tabButtonView )
		tabButtonView->Refresh ();

}	//	LGATabPanel::InsertTabButtonAt


//-------------------------------------------------------------------------------------
// LGATabPanel::RemoveTabButtonAt														
//-------------------------------------------------------------------------------------

void
LGATabPanel::RemoveTabButtonAt (	ArrayIndexT inAtIndex )
{

	// е Before we proceed we make sure that the index we have been handed
	// is not out of range, if it is we simply do nothing
	SInt16	tabCount = GetTabButtonCount ();
	if ( inAtIndex > 0 && inAtIndex <= tabCount )
	{

		LArray&	tabButtons = GetTabButtons ();
		
		// е Find then button that is being removed
		LGATabButton*	tabButton;
		tabButtons.FetchItemAt ( inAtIndex, &tabButton );
		
		// е Was the button selected
		Boolean	wasSelected = tabButton->IsSelected ();
		
		// е Figure out the size of the button
		Rect localFrame;
		mTabButtons->CalcLocalFrameRect ( localFrame );
		SDimension16	buttonSize;
		tabButton->GetFrameSize ( buttonSize );
		
		// е Remove the button from the list
		tabButton->Refresh ();
		tabButton->Hide ();
		tabButton->PutInside ( nil );
		delete tabButton;
		
		// е We need to get the locations of all the buttons after 
		// this one adjusted to fill in the space occupied by the
		// removed tab button
		tabCount = tabButtons.GetCount ();
		for ( SInt16 index = inAtIndex; index <= tabCount; index++ )
		{
			tabButtons.FetchItemAt ( index, &tabButton );
			if ( tabButton )
			{	
				tabButton->MoveBy ( -buttonSize.width, 0, true );
			}
		}
		
		// е if the button was selected we need to get another one
		// selected, for that we just use the same index
		if ( wasSelected )
		{
			// е Clear the button reference
			mSelectedButton = nil;
			
			// е If the index is greater than the number of buttons we
			// will simply select the last tab button
			if ( tabCount < inAtIndex )
				inAtIndex = tabCount;
				
			// е Get the appropriate button selected
			SetSelectedTabButtonByIndex ( inAtIndex, false );
		}
		
		// е Get the tab button view refreshed
		LView*	tabButtonView = GetTabButtonView ();
		if ( tabButtonView )
			tabButtonView->Refresh ();

	}
	
}	//	LGATabPanel::RemoveTabButtonAt


//-------------------------------------------------------------------------------------
// LGATabPanel::CreateTabButton													
//-------------------------------------------------------------------------------------

LGATabButton*
LGATabPanel::CreateTabButton (	ConstStr255Param	inButtonTitle,
											PaneIDT				inButtonID,
											SDimension16		inButtonSize,
											SPoint32				inButtonPosition,
											Boolean				inButtonPushed )
{

	// е Setup the pane info
	SPaneInfo paneInfo;

	paneInfo.paneID = inButtonID;
	paneInfo.width = inButtonSize.width;
	paneInfo.height = inButtonSize.height;
	paneInfo.visible = true;
	paneInfo.enabled = true;
	paneInfo.bindings.left = false;
	paneInfo.bindings.top = false;
	paneInfo.bindings.right = false;
	paneInfo.bindings.bottom = false;
	paneInfo.left = inButtonPosition.h;
	paneInfo.top = inButtonPosition.v;
	paneInfo.userCon = nil;
	paneInfo.superView = GetTabButtonView ();

	// е Setup the control info
	SControlInfo	controlInfo;
	
	controlInfo.valueMessage = inButtonID;
	controlInfo.value = inButtonPushed ? 1 : 0;
	controlInfo.minValue = 0;
	controlInfo.maxValue = 1;
	
	// е Now create the actual button
	LGATabButton* aTabButton = new LGATabButton ( 	paneInfo,
																	controlInfo,
																	inButtonTitle,
																	true,
																	GetTextTraitsID () );
	
	// е Make sure that we got a tab button
	ThrowIfNil_ ( aTabButton );

	// е We need to make sure that we call FinishCreate so that
	// everything is happy
	aTabButton->FinishCreate ();
	
	// е Add the tab panel as a listener to the button
	aTabButton->AddListener ( this );

	// е Make sure the button is set to broadcast
	aTabButton->StartBroadcasting ();
	
	// е If the button is set to be pushed then get it setup that way
	if ( inButtonPushed )
		SetSelectedTabButton ( aTabButton );

	// е Now return the newly built button
	return aTabButton;
	
}	//	LGATabPanel::CreateTabButton


//-------------------------------------------------------------------------------------
// LGATabPanel::GetTabButtonCount
//-------------------------------------------------------------------------------------

SInt16
LGATabPanel::GetTabButtonCount ()
{
	// е Get the list of sub panes for the button view
	LArray& buttonList = GetTabButtons ();
	
	// е return the button count which is the sub pane count
	return (SInt16)buttonList.GetCount ();
	
}	// LGATabPanel::GetTabButtonCount


//-------------------------------------------------------------------------------------
// LGATabPanel::GetLastTabButtonLocation
//-------------------------------------------------------------------------------------

SInt16
LGATabPanel::GetLastTabButtonLocation ()
{
	// е If there are buttons then find the last one and return the
	// current coordinate for its right edge
	LArray&	buttonList = GetTabButtons ();
	if ( buttonList.GetCount () > 0 )
	{
		LGATabButton*	lastButton;
		buttonList.FetchItemAt ( buttonList.GetCount (), &lastButton );
		Rect lastButtonFrame;
		lastButton->CalcLocalFrameRect ( lastButtonFrame );	
		
		// е Return right edge coordinate
		return lastButtonFrame.right;
	}
	
	// е There are currently no buttons so just return 6
	return 6;
	
}	// LGATabPanel::GetLastTabButtonLocation


//-------------------------------------------------------------------------------------
// LGATabPanel::DeleteAllTabButtons
//-------------------------------------------------------------------------------------

void
LGATabPanel::DeleteAllTabButtons ()
{

	// е To delete all of the tab buttons we simply call the DeleteAllSubPanes
	// method in LView which works nicely for us, we also then need to do a
	// refresh so that the change shows up
	if ( mTabButtons )
	{
		mTabButtons->DeleteAllSubPanes ();
		Refresh ();
	}
	
	// е Make sure we zero out the index count
	ClearIndexCounter ();
	
}	//	LGATabPanel::DeleteAllTabButtons


//-------------------------------------------------------------------------------------
// LGATabPanel::LoadTabTitlesFromArray													
//-------------------------------------------------------------------------------------
//

void		
LGATabPanel::LoadTabTitlesFromArray (	LVariableArray*	inStringArray,
													SInt16					inSelectionIndex )
{
	LArrayIterator iterator ( *inStringArray, LArrayIterator::from_Start );
	Str255	theTitle;
	while ( iterator.Next ( theTitle )) 
	{
		// е Now add the title to the tab panel
		AddTabButton ( theTitle, false );
	}
		
	// е Once all the titles have been loaded set the initially
	// selected tab button
	SetSelectedTabButtonByIndex	( inSelectionIndex );
	
}	//	LGATabPanel::LoadTabTitlesFromArray


//-------------------------------------------------------------------------------------
// LGATabPanel::LoadTabTitlesFromStringList														
//-------------------------------------------------------------------------------------

void		
LGATabPanel::LoadTabTitlesFromStringList (	ResIDT	inStringListID,
															SInt16		inSelectionIndex )
{

	// е Before we can do anything we need to figure out how many
	// titles are in the string list using a rather gross approach
	SInt16		titleCount = 0;
	Handle stringListH = ::GetResource ( 'STR#', inStringListID );
   if ( ::ResError () == noErr )
	{
	   HLock ( stringListH );
	   HNoPurge ( stringListH );
	   ::BlockMove( *stringListH, &titleCount, sizeof ( SInt16 ));
	   ::ReleaseResource ( stringListH );
	 }
	  
	 // е If we have a title count that is greater than 0 we can proceed
	 if ( titleCount > 0 )
	 {
		SInt16		index = 0;
		for ( index = 1; index <= titleCount; index++ ) 
		{
			// е Get the string from the list
			Str255	theTitle;
			::GetIndString ( theTitle, inStringListID, index );
			
			// е Now add the title to the tab panel
			AddTabButton ( theTitle, false );
		}
		
		// е Once all the titles have been loaded set the initially
		// selected tab button
		if ( inSelectionIndex > 0 )
			SetSelectedTabButtonByIndex	( inSelectionIndex );

	}

}	//	LGATabPanel::LoadTabTitlesFromStringList


#pragma mark -
#pragma mark === LISTENING

//=====================================================================================
// ее LISTENING
//-------------------------------------------------------------------------------------
// LGATabPanel::ListenToMessage														
//-------------------------------------------------------------------------------------

void		
LGATabPanel::ListenToMessage (	MessageT inMessage, 
											void*		ioParam	)
{
	
	SInt32	value = *(SInt32 *) ioParam;

	// е Locate the button that was pressed and then do the right thing about
	// turning off the current button and then broadcasting the index of the
	// newly selected tab
	LGATabButton* tabButton = (LGATabButton*)FindPaneByID ( inMessage );
	if ( value == Button_On && tabButton )
	{
		// е Get the currently selected button
		LGATabButton* currButton = GetSelectedTabButton ();

		// е Make sure that the currently selected button is turned off
		// if it is not the button that has just been selected, this would
		// never happen when clicking on buttons but it could happen if a
		// button is being set procedurally
		if ( currButton != nil && currButton != tabButton )
		{
			ToggleTabButtonState ( currButton, false );
		}
		
		// е Setup our reference to the new tab button, we don't need to select
		// the button as this was done by the user clicking on it so we just setup
		// the reference
		SetSelectedTabButton ( tabButton, false );
		
		// е Finally, figure out the index of the new tab and broadcast that
		// along with the controller message
		SInt16 currTabIndex = GetCurrentTabIndex ();
		if ( currTabIndex > 0 )
			BroadcastMessage ( GetPanelMessage (), &currTabIndex );
	}
	
}	//	LGATabPanel::ListenToMessage


#pragma mark -
#pragma mark === DRAWING

//=====================================================================================
// ее DRAWIING
//-------------------------------------------------------------------------------------
// LGATabPanel::DrawSelf																
//-------------------------------------------------------------------------------------

void		
LGATabPanel::DrawSelf ()
{
	
	StColorPenState thePenState;
	StColorPenState::Normalize ();
	StClipRgnState	theClipState;
	
	// е Get the frame for the panel
	Rect localFrame;
	CalcLocalTabPanelRect ( localFrame );			

	// е Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( localFrame );
	SInt16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{

		// е We are going to modify the clipping region so that it excludes the
		// area where buttons will be drawn, that way we don't get unnecessary
		// drawing behind the buttons
		Rect 	buttonClipping;
		CalcTabButtonClipping ( buttonClipping );
		RgnHandle	buttonRgn = ::NewRgn ();
		::RectRgn ( buttonRgn, &buttonClipping );
		RgnHandle 	currClipRgn = ::NewRgn ();
		::GetClip ( currClipRgn );
		::DiffRgn ( currClipRgn, buttonRgn, currClipRgn );
		::SetClip ( currClipRgn );
		::DisposeRgn ( buttonRgn );
		::DisposeRgn ( currClipRgn );
		
		RGBColor theColor;

		// е BLACK & WHITE
		if ( depth < 4 )
		{
			StColorPenState::Normalize ();

			// е Draw a black line along the left, top and right edges
			theColor = UGAColorRamp::GetBlackColor();
			::RGBForeColor ( &theColor);
			::FrameRect ( &localFrame );
			
			// е We also need to erase the contents of the panel so that
			// things redraw properly
			::InsetRect ( &localFrame, 1, 1 );
			::EraseRect ( &localFrame );
		}
		// е COLOR
		else 
		{
			DrawColorTabPanel ();
		}	
	}
	
}	//	LGATabPanel::DrawSelf


//-------------------------------------------------------------------------------------
// LGATabPanel::DrawColorTabPanel																
//-------------------------------------------------------------------------------------

void		
LGATabPanel::DrawColorTabPanel ()
{

	StColorPenState thePenState;
	thePenState.Normalize ();
	
	Rect localFrame;
	CalcLocalTabPanelRect ( localFrame );
	
	// е First we frame the panel
	RGBColor theColor = IsActive () && IsEnabled () ? UGAColorRamp::GetBlackColor() : 
															UGAColorRamp::GetColor(colorRamp_Gray10);
	::RGBForeColor ( &theColor );
	::FrameRect ( &localFrame );
	
	// е First make sure the face of the panel is drawn
	theColor = IsActive () && IsEnabled () ? UGAColorRamp::GetColor(1) : 
															UGAColorRamp::GetColor(colorRamp_Gray2);
	::RGBForeColor ( &theColor );
	::InsetRect ( &localFrame, 1, 1 );
	::PaintRect ( &localFrame );
	
	// е All of the shadow drawing is only done if we are active
	if ( IsActive () && IsEnabled ())
	{
		// е Render the gray shade along the left and top edges
		RGBColor theColor = UGAColorRamp::GetColor(colorRamp_Gray3);
		::RGBForeColor ( &theColor );
		UGraphicsUtilities::TopLeftSide ( &localFrame, 0, 0, 1, 1 );
		
		// е Render the gray shade along the right and bottom edges
		theColor = UGAColorRamp::GetColor(colorRamp_Gray6);
		::RGBForeColor ( &theColor );
		UGraphicsUtilities::BottomRightSide ( &localFrame, 1, 1, 0, 0 );
		
		// е Render the white edge along the left and top edges
		theColor = UGAColorRamp::GetWhiteColor();
		::RGBForeColor ( &theColor );
		UGraphicsUtilities::TopLeftSide ( &localFrame, 1, 1, 2, 2 );
		
		// е Render the next gray shade along the right and bottom edges
		theColor = UGAColorRamp::GetColor(colorRamp_Gray4);
		::RGBForeColor ( &theColor );
		UGraphicsUtilities::BottomRightSide ( &localFrame, 2, 2, 1, 1 );
		
		// е Now fix up the two corner pixels
		// е BOTTOM LEFT
		theColor = UGAColorRamp::GetColor(colorRamp_Gray2);
		UGraphicsUtilities::PaintColorPixel ( 	localFrame.left, 
															localFrame.bottom - 1,
															&theColor );
								
		// е TOP RIGHT
		theColor = UGAColorRamp::GetColor(colorRamp_Gray2);
		UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 1, 
															localFrame.top,
															&theColor );
								
	}
		
}	//	LGATabPanel::DrawColorTabPanel


//------------------------------------------------------------------------------------
//	LGATabPanel::RefreshPanelHost
//------------------------------------------------------------------------------------
//

void
LGATabPanel::RefreshPanelHost ()
{
	// е This method can be called to get the content area of the panel
	// refreshed and hence any panel that might be installed in the tab panel
	if ( mPanelHost )
		mPanelHost->Refresh ();
	
}	//	LGATabPanel::RefreshPanelHost


#pragma mark -
#pragma mark === LAYOUT

//=====================================================================================
// ее LAYOUT
//-------------------------------------------------------------------------------------
// LGATabPanel::BuildTabButtonView												
//-------------------------------------------------------------------------------------

void		
LGATabPanel::BuildTabButtonView ()
{

	Rect localFrame;
	CalcLocalFrameRect ( localFrame );
	
	// е Setup the pane info for the button view
	SPaneInfo paneInfo;

	paneInfo.paneID = 'BUTV';
	paneInfo.width = UGraphicsUtilities::RectWidth ( localFrame );
	paneInfo.height = CalcTabButtonSize () == 10 ? tabPanel_10TabButtonHeight :
										tabPanel_12TabButtonHeight;
	paneInfo.visible = true;
	paneInfo.enabled = true;
	paneInfo.bindings.left = true;
	paneInfo.bindings.top = false;
	paneInfo.bindings.right = true;
	paneInfo.bindings.bottom = false;
	paneInfo.left = localFrame.left + 1;
	paneInfo.top = localFrame.top;
	paneInfo.userCon = nil;
	paneInfo.superView = this;

	// е Setup the control info
	SViewInfo	viewInfo;
	
	viewInfo.imageSize.width = 0;
	viewInfo.imageSize.height = 0;
	viewInfo.scrollPos.v = 0;
	viewInfo.scrollPos.h = 0;
	viewInfo.scrollUnit.h = 0;
	viewInfo.scrollUnit.v = 0;
	viewInfo.reconcileOverhang = false;
	
	// е Now create the actual view
	LView* buttonView = new LView ( 	paneInfo,
												viewInfo );
																	
	// е Make sure it got created
	ThrowIfNil_ ( buttonView );
												
	// е Finally, call FinishCreate like a good boy!
	buttonView->FinishCreate ();
	
	// е Save off a reference to the button view
	mTabButtons = buttonView;
	
}	//	LGATabPanel::BuildTabButtonView


//-------------------------------------------------------------------------------------
// LGATabPanel::BuildPanelHostView												
//-------------------------------------------------------------------------------------

void		
LGATabPanel::BuildPanelHostView ()
{
	
	Rect localFrame;
	CalcLocalFrameRect ( localFrame );
	
	// е Setup the pane info for the button view
	SPaneInfo paneInfo;

	paneInfo.paneID = 'PHST';
	paneInfo.width = UGraphicsUtilities::RectWidth ( localFrame ) - 6;
	paneInfo.height = UGraphicsUtilities::RectHeight ( localFrame ) -
								(CalcTabButtonSize () == 10 ? tabPanel_10TabButtonHeight + 5 :
												tabPanel_12TabButtonHeight + 5);
	paneInfo.visible = true;
	paneInfo.enabled = true;
	paneInfo.bindings.left = true;
	paneInfo.bindings.top = true;
	paneInfo.bindings.right = true;
	paneInfo.bindings.bottom = true;
	paneInfo.left = localFrame.left + 3;
	paneInfo.top = localFrame.top + (CalcTabButtonSize () == 10 ? 
														tabPanel_10TabButtonHeight + 2 :
																tabPanel_12TabButtonHeight + 2);
	paneInfo.userCon = nil;
	paneInfo.superView = this;

	// е Setup the view info
	SViewInfo	viewInfo;
	
	viewInfo.imageSize.width = 0;
	viewInfo.imageSize.height = 0;
	viewInfo.scrollPos.v = 0;
	viewInfo.scrollPos.h = 0;
	viewInfo.scrollUnit.h = 0;
	viewInfo.scrollUnit.v = 0;
	viewInfo.reconcileOverhang = false;
	
	// е Now create the actual view
	LPlaceHolder* placeHolder = new LPlaceHolder ( 	paneInfo,
																	viewInfo,
																	mPanelAlignment );
																	
	// е Make sure it got created
	ThrowIfNil_ ( placeHolder );
												
	// е Finally, call FinishCreate like a good boy!
	placeHolder->FinishCreate ();
	
	// е Save off a reference to the panel host view
	mPanelHost = placeHolder;
	
}	//	LGATabPanel::BuildPanelHostView


#pragma mark -
#pragma mark === SEARCHING

//=====================================================================================
// ее SEARCHING
//-------------------------------------------------------------------------------------
// LGATabPanel::FindTabButtonByTitle														
//-------------------------------------------------------------------------------------

LGATabButton*		
LGATabPanel::FindTabButtonByTitle ( ConstStr255Param	inTabTitle )
{

	// е Iterate over the list of tab buttons looking for a match with
	// the tab title passed in to us
	LArrayIterator iterator ( GetTabButtons (), LArrayIterator::from_Start );
	LGATabButton* theButton;
	while ( iterator.Next ( &theButton )) 
	{
		
		// е Check the button's name against the tab title and if there
		// is a match then return the button
		LStr255 buttonName;
		theButton->GetDescriptor ( buttonName );
		if ( buttonName.CompareTo ( inTabTitle ) == 0 ) 
			return theButton;
	}
	
	// е No button was found
	return nil;
	
}	//	LGATabPanel::FindTabButtonByTitle


//-------------------------------------------------------------------------------------
// LGATabPanel::FindTabTitleByIndex
//-------------------------------------------------------------------------------------

Boolean
LGATabPanel::FindTabTitleByIndex ( 	Str255		outTitle,
												ArrayIndexT	inIndex )
{

	// е Retrieve the title from our title list if the index is not
	// a valid one false will be returned
	LArray&	tabButtons = GetTabButtons ();
	LGATabButton*	aTabButton = nil;
	tabButtons.FetchItemAt ( inIndex, &aTabButton );
	if ( aTabButton )
	{
		// е Check the button's name against the tab title and if there
		// is a match then return the button
		aTabButton->GetDescriptor ( outTitle );
		
		// е We got the title so return true
		return true;
	}
	
	// е Title wasn't found so return false
	return false;
	
}	//	LGATabPanel::FindTabTitleByIndex


#pragma mark -
#pragma mark === MISCELLANEOUS

//=====================================================================================
// ее MISCELLANEOUS
//-------------------------------------------------------------------------------------
// LGATabPanel::GetTabButtonID														
//-------------------------------------------------------------------------------------

ArrayIndexT		
LGATabPanel::GetTabButtonID ( PaneIDT	&outPaneID )
{

	// е Start by creating an initial ID which is 'tb'
	OSType tempID = 'tb  ';
	Str255 idString;
	LString::FourCharCodeToPStr ( tempID, idString );
		
	// е Next figure out the index for the tab title and concatenate
	// that onto the ID
	ArrayIndexT tabIndex = GetNextIndex ();
	Str255 indexString;
	::NumToString ( tabIndex, indexString );
	idString[0] = 2;
	LString::AppendPStr ( idString, indexString );
	
	// е Convert it all back to the pane ID
	LString::PStrToFourCharCode ( idString, (FourCharCode&)outPaneID );
			
	return tabIndex;
	
}	//	LGATabPanel::GetTabButtonID


//-------------------------------------------------------------------------------------
// LGATabPanel::RebuildButtonIdentifiers														
//-------------------------------------------------------------------------------------
//
//	This function is called when the page index counter overflows, what it does is
// simply rebuild the button IDs starting from 1.  This will get called when we
//	run out of indices at 99, the most common cause will be that the user has added
// and deleted too many buttons and we have simply exhausted the available range of
// indices that can be used to build IDs.

void		
LGATabPanel::RebuildButtonIdentifiers ()
{

	// е We start by first clearing the index counter
	ClearIndexCounter ();
	
	// е Iterate over the list of tab buttons changing all of their
	// IDs so that we get the ID range compacted
	LArrayIterator iterator ( GetTabButtons (), LArrayIterator::from_Start );
	LGATabButton* theButton;
	while ( iterator.Next ( &theButton )) 
	{
		// е Get a new pane ID
		PaneIDT	newID;
		GetTabButtonID ( newID );
		
		// е Change the button's ID to the new one
		if ( theButton ) 
			theButton->SetPaneID ( newID );
	}
	
}	//	LGATabPanel::RebuildButtonIdentifiers


