// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1996 Metrowerks Corp. All rights reserved

---------------------------------------<< ¥ >>----------------------------------------

FILE:					LGATabPanel.h

CLASSES:				LGATabPanel

AUTHOR:				Robin Mair

CREATION DATE :	96.12.12
	
---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

This class implements the tab panels design as specified by the Apple Grayscale
Appearance document.

It supports two sizes of tabs which are calculated based on the font size specified
in the text trait that is associated with the tab panel, the two sizes are 10pt. and 
12pt., which affects the height of the tab only not its width.

This class uses the LGATabButton class to create the tab buttons. These are created
proceedurally by the tab panel when it is constructed.  The titles for the tab buttons
are specified in a list of strings that are created directly within the prpoperty
inspector within Constructor 2.4. Tab buttons cannot be placed in a layout using 
Constructor, they are strictly for the internal use of the tab panel and therefore
have no stream creation function or stream constructor.

When a tab is selected the tab panel broadcasts the message specified for the tab
panel as well as the index for the tab selected.  If you need the selected tabs title
you can call FindTabTitleByIndex using the index handed to you when the message is
broadcast.

The tab panel builds two additional views when it is created, one that is used to
hold the tab buttons, and one which serves as the host for panels that are installed
in the tab panel.  Methods are provided for adding and removing panels in the tab
panel.  The InstallPanel method also allows the user to specify that the panel being
installed be fit to the panel host, this will cause the panel to be relocated and
resized as needed.

The panel host is used as it provides a simple mechanism for minimizing the amount 
of flickering that takes place in the tab panel and its buttons when panels are
installed and removed.  All refreshing done during this process uses the host view
which causes the rest of the tab panel to be clipped out of the drawing process, which
really helps stop flickering.  Believe me I tried it!  For the host view we use the
LPlaceHolder class which allows us to support the alignment and positioning of panels
when they are installed in the tab panel.  The alignment is specified using the
constants provided by Apple for the icon suites, these can be found in Icons.h.  By
default the alignment is set to be -1 in the alignment parameter for InstallPanel, this
means that the alignment specified in the PPob will be used.  This can be specified
from within Constructor.  There is a flag that can be used to control whether this
host view gets created or not, thereby allowing the user to do their own thing within
the tab panel if they so desire.  NOTE: if there is no host panel any calls to the
panel installation API will result in nothing happening.

The rendering of the tab panel changes when a window is deactivated  and activated 
as specified by AGA.

Currently, it is the users responsibility to provide a string list with the correct
number of titles for the tabs, such that the titles don't run off the end of the tab 
panel.

The GASample application gives an example of how the tab panel can be used in its main
window, where the tab panel is used to provide grouping os the various types of
classes provided in the library.

NOTE:	The tab size cannot be changed on the fly, that is, if the text trait for the
panel is changed the tab buttons will not be changed, but the text will be redrawn
with the new text trait the next time there is an update.  If we hear a strong demand
for support for dynamically changing the tab button size, we will consider making the
changes necessary to support this.  There just didn't seem to be a real need for it.


---------------------------------------<< ¥ >>----------------------------------------
*/

#ifndef _H_LGATabPanel
	#define _H_LGATabPanel
	#pragma once

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import on
#endif

// ¥ GRAYSCALE HEADERS
#include <LGATabButton.h>

// ¥ POWERPLANT HEADERS
#include <LView.h>
#include <LListener.h>
#include <LPlaceHolder.h>
#include <LBroadcaster.h>
#include <LVariableArray.h>


//==================================================================================== 
//	OBJECT
//==================================================================================== 
	
class LGATabPanel : 	public LView,
							public LListener,
							public LBroadcaster
{

	//====<< ¥ CLASS ID ¥ >>===========================================================

public:

	enum { class_ID = 'gtbp' };
	
	//====<< ¥ FIELDS ¥ >>=============================================================

protected:	
	
	LVariableArray		mTabTitles;			// This is a list of all the tab titles for  
													//		the tabs being managed by the controller
	MessageT				mPanelMessage;		//	Message to be sent when the controller has a
													//		tab change selected
	LGATabButton*		mSelectedButton;	// Currently selected tab button
	ResIDT				mTextTraitsID;		//	ID of the controller's text traits
	Boolean				mWantPlaceHolder;	//	This flag is used to tell us if we need to 
													//		create the placeholder or not
	SInt16					mPanelAlignment;	//	If we have a placeholder this is the align-
													//		ment that will be used when a panel is
													//		added to the placeholder, unless the user
													//		passes in a different value when install-
													//		ing the panel, we use the same constants
													//		as defined for the Icon Suite alignment
													
private:

	LView*				mTabButtons;		//	View that contains the tab buttons, we
													//		keep a reference to the view so that
													//		we can easily access the buttons
	LPlaceHolder*		mPanelHost;			//	This is the view that panels get installed
													//		into when they are displayed in the tab
													//		panel, an API is provided for installing
													//		and removing panels														
	ResIDT				mStringListResID;	//	ID for the string list if we are building
													//		the tab panel procedurally from a string
													//		list resource
	SInt16					mStringListCount;	//	Count for the number of tab titles in the
													//		tab panels PPob
	SInt16					mSelectionIndex;	//	Index for the initially selected button
	LVariableArray*	mInitialTabTitles;//	This field is used to temprarily hold the
													//		list of titles for the tab buttons it
													//		is then used from within FinishCreateSelf
													//		to create all of the buttons
	ArrayIndexT			mNextIndex;			//	This is used to keep a running count for
													//		assigned indices, it is only used to build
													//		IDs for buttons.  This means that you have
													//		to be very careful when adding and removing
													//		buttons as this could overflow. Clearing
													//		all buttons causes this to get cleared.									
														
	//====<< ¥ METHODS ¥ >>============================================================
		
	//----<< ¥ INITIALIZATION & DISPOSAL ¥ >>------------------------------------------

public:

				LGATabPanel 			();						//	¥ Default Constructor
				LGATabPanel 			(	const LGATabPanel	&inOriginal );	
																		//	¥ Copy Constructor
				LGATabPanel 			(	LStream* inStream );				
																		//	¥ Stream Constructor
				LGATabPanel 			(	const SPaneInfo 	&inPaneInfo,
													const SViewInfo 	&inViewInfo,
													ResIDT				inTextTraitsID,
													LVariableArray*	inTabTitles,
													ArrayIndexT			inSelectionIndex,
													MessageT				inMessage,
													Boolean				inWantPlaceHolder = true,
													SInt16					inAlignment = 0 );
																		//	¥ Parameterized Constructor
				LGATabPanel 			(	const SPaneInfo 	&inPaneInfo,
													const SViewInfo 	&inViewInfo,
													ResIDT				inTextTraitsID,
													ResIDT				inStringListID,
													ArrayIndexT			inSelectionIndex,
													MessageT				inMessage,
													Boolean				inWantPlaceHolder = true,
													SInt16					inAlignment = 0 );
																		//	¥ Parameterized Constructor
	virtual	~LGATabPanel 			();						//	¥ Destructor
									
	virtual	void					FinishCreateSelf	();						// ¥ OVERRIDE
				
				
	//----<< ¥ ACCESSORS ¥ >>----------------------------------------------------------

	// ¥ GETTERS
	
	virtual	ResIDT				GetTextTraitsID			()
													{ return mTextTraitsID; }

	virtual	LArray&				GetTabButtons				()
							{	return mTabButtons->GetSubPanes (); }

	virtual	ArrayIndexT			GetNextIndex				()
													{	return ++mNextIndex;	}
	virtual	ArrayIndexT			GetCurrentIndex			()
													{	return mNextIndex;	}
													
	virtual	ArrayIndexT			GetCurrentTabIndex		();
	virtual	ArrayIndexT			GetTabIndex					( ConstStr255Param inTabTitle );
	virtual	LGATabButton*		GetSelectedTabButton		() const
													{	return mSelectedButton; }
	virtual	MessageT				GetPanelMessage			() const
													{	return mPanelMessage; }
	virtual	SInt16					CalcTabButtonWidth		( 	ConstStr255Param inTabTitle );
	virtual	void					CalcLocalTabPanelRect	(	Rect	&outRect );
	virtual	void					CalcTabButtonClipping	(	Rect	&outRect );
	virtual	SInt16					CalcTabButtonSize			();
	virtual	LView*				GetTabButtonView			()
													{	return mTabButtons; }
	virtual	LPlaceHolder*		GetPanelHostView			()
													{	return mPanelHost; }
													
	// ¥ SETTERS
	
	virtual	void		SetTextTraitsID			( 	ResIDT 			inTextTraitsID );
	
	virtual	void		SetSelectedTabButton	(	LGATabButton*	inSelectedButton,
															Boolean			inSelectButton = true,
															Boolean			inSuppressBroadcast = true );
	virtual	void		SetSelectedTabButtonByIndex	(	ArrayIndexT	inSelectionIndex,
																		Boolean		inSuppressBroadcast = true );
	virtual	void		ToggleTabButtonState	(	LGATabButton*	inSelectedButton,
															Boolean			inSelected = true,
															Boolean			inSuppressBroadcast = true );
	virtual	void		ClearIndexCounter	()
													{	mNextIndex = 0;	}
													
	
	//----<< ¥ PANELS ¥ >>-------------------------------------------------------------
	
	virtual	void		InstallPanel 				(  LPane* 	inPanel,
																SInt16		inAlignment = -1,
																Boolean	inRefresh = true );
	virtual	LPane*	RemovePanel 				();
		
		
	//----<< ¥ ACTIVATION ¥ >>---------------------------------------------------------
	
	virtual	void		ActivateSelf				();								//	¥ OVERRIDE
	virtual	void		DeactivateSelf				();								//	¥ OVERRIDE
	
	
	//----<< ¥ ENABLING & DISABLING ¥ >>-----------------------------------------------

	virtual 	void		EnableSelf					();								// ¥ OVERRIDE
	virtual 	void		DisableSelf					();								// ¥ OVERRIDE


	//----<< ¥ TAB BUTTON MANAGEMENT ¥ >>---------------------------------------------

	virtual	void		AddTabButton 				(	Str255  	&inTabTitle,
																Boolean  inSelected	);
	virtual	void		InsertTabButtonAt 		(	Str255  		&inTabTitle,
																ArrayIndexT inAtIndex );
	virtual	void		RemoveTabButtonAt			(	ArrayIndexT	inAtIndex );
	virtual	LGATabButton*	CreateTabButton 	(	ConstStr255Param	inButtonTitle,
																PaneIDT				inButtonID,
																SDimension16		inButtonSize,
																SPoint32				inButtonPosition,
																Boolean				inButtonPushed );
																
	virtual	SInt16		GetTabButtonCount			();
	virtual	SInt16		GetLastTabButtonLocation 	();
	
	virtual	void		DeleteAllTabButtons		();
	virtual	void		LoadTabTitlesFromArray	(	LVariableArray*	inStringArray,
																SInt16		inSelectionIndex );
	virtual	void		LoadTabTitlesFromStringList (	ResIDT	inStringListID,
																	SInt16		inSelectionIndex );
															
															
	//----<< ¥ LISTENING ¥ >>----------------------------------------------------------

	virtual	void		ListenToMessage 			(	MessageT inMessage, 
																void*		ioParam	);		// ¥ OVERRIDE
																
																		
	//----<< ¥ DRAWING ¥ >>------------------------------------------------------------

	virtual	void		DrawSelf						();								//	¥ OVERRIDE
	virtual	void		DrawColorTabPanel			();

	virtual	void		RefreshPanelHost			();
	
	
	//----<< ¥ LAYOUT ¥ >>-------------------------------------------------------------

	virtual	void		BuildTabButtonView		();
	virtual	void		BuildPanelHostView		();
	
	
	//----<< ¥ SEARCHING ¥ >>----------------------------------------------------------

	virtual	LGATabButton*	FindTabButtonByTitle	(	ConstStr255Param	inTabTitle );
																
	virtual	Boolean			FindTabTitleByIndex	(	Str255		outTitle,
																	ArrayIndexT	inIndex );
																	
																	
	//----<< ¥ MISCELLANEOUS ¥ >>------------------------------------------------------

protected:

	virtual	ArrayIndexT		GetTabButtonID					( 	PaneIDT	&outPaneID );
	virtual	void				RebuildButtonIdentifiers	();
	
																
};	// LGATabPanel

#if defined (__CFM68K__) && !defined (__USING_STATIC_LIBS__)
	#pragma import reset
#endif

#endif
