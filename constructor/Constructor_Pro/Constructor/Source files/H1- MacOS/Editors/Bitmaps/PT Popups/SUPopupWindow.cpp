// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				PopupWindow.cp

PROJECT:			Constructor Icon Editor

CLASSES:			SUPopupWindow

DESCRIPTION:		This code is for the main paint view
					
AUTHOR:				Eric Shapiro

CREATION DATE :		96.5.22

CHANGE HISTORY :

		96.08.14	ebs		Added check where mouse is within the popup but not over any item
		96.5.22		ebs		Integrated from old project into new one

---------------------------------------<< ¥ >>-----------------------------------------
*/

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "SU_Constants.h"
#include "SU_Macros.h"

#include "SUPopupWindow.h"
#include "SUSaveGWorld.h"
#include "SUMiscUtils.h"			// for MIN

/*=================================
	SUPopupWindow Constructor
==================================*/
SUPopupWindow::SUPopupWindow()
{
	mItems = 0;
	mRows = mCols = 0;
	mCellWidth = mCellHeight = 0;
	SetRect( &mFirstItemRect, 0, 0, 0, 0 );
}

/*=================================
	SUPopupWindow Destructor
==================================*/
SUPopupWindow::~SUPopupWindow()
{
}

/*=================================
	SUPopupWindow::DoPopup
	
	Returns the item number selected (0..n).
	Returns the default item number if nothing is selected.
	-1 means nothing selected and no default was specified.
==================================*/
SInt32 SUPopupWindow::DoPopup( Point inTopLeft, SInt32 defaultItem )
{
	SInt32			result = -1;
	WindowPtr		macWindow = nil;
#if !PP_Target_Carbon
	WindowRecord	windowStorage;			// lessen heap space & speed up creation
#endif
	StSaveGWorld	saveTheWorld;
	
	try
	{
		mDefaultItem = defaultItem;
		this->Prepare( inTopLeft );
#if PP_Target_Carbon
		macWindow = this->CreateMacWindow();
#else
		macWindow = this->CreateMacWindow( &windowStorage );
#endif
		this->DrawInitialState();
		result = this->TrackMouse();
	}
	catch( ... )
	{
		// don't want to throw an error here???
	}
	
	if ( macWindow )
#if PP_Target_Carbon
		::DisposeWindow( macWindow );
#else
		::CloseWindow( macWindow );
#endif

	return( result );
}

/*=================================
	SUPopupWindow::CreateMacWindow
	
	Note:
	Sets the current port to the window.
==================================*/
#if PP_Target_Carbon
#define inStorage NULL
WindowPtr SUPopupWindow::CreateMacWindow( void )
#else
WindowPtr SUPopupWindow::CreateMacWindow( WindowRecord *inStorage )
#endif
{
	WindowPtr	theWindow = nil;
	Rect		windowR;
	
	::SetGDevice( GetMainDevice() );	// Window Mgr doesn't like it if this isn't so
	
	/*******************************************
		move the window so it commpletely visible on a single screen.
		we use the mouse location rather then mWindowRect to determine the
		  screen for two cases:
		   (1) The popup control is at the very bottom of a screen and the
		       	popup window wouldn't be visible at all.
		   (2) The popup control spans two screens. It's cooler to show the
		     	popup window on the same screen as the mouse.
	*******************************************/
	Point pt;
	::GetMouse( &pt );
	::LocalToGlobal( &pt );
	GDHandle	theDevice = ::SUMiscUtils::FindDeviceViaPoint( pt );

	if ( !theDevice )
		theDevice = SUMiscUtils::FindDeviceViaPoint( topLeft(mWindowRect) );

	SUMiscUtils::ForceRectToScreen( mWindowRect, &windowR, theDevice );
	
	/*******************************************
		Prevent front window from deactivating title - create our
		window in back and then call BringToFront.
	*******************************************/
	theWindow = ::NewCWindow( inStorage, &windowR, (StringPtr)"", true /* visible */, plainDBox, 
							(WindowPtr) 0, false /* go away */, 0 /* refCon */);
	ThrowIfMemFail_( theWindow );
	::BringToFront( theWindow );
	::SetPortWindowPort( theWindow );
	
	return( theWindow );
}

/*=================================
	SUPopupWindow::TrackMouse
==================================*/
SInt32 SUPopupWindow::TrackMouse()
{
	SInt32		currentItem = -1, prevItem = -1;
	Point		pt;
	Boolean		mouseOutside = true;
	
	while( ::StillDown() )
	{
		::GetMouse( &pt );
		currentItem = this->LocalPointToIndex( pt );
		
			// default back to the default item if the mouse is outside the window
		if ( currentItem == -1 )
		{
			mouseOutside = true;
			currentItem = mDefaultItem;
		}
		else
			mouseOutside = false;
		
		if ( currentItem != prevItem )
		{
			this->HiliteItem( prevItem, false );
			this->HiliteItem( currentItem, true );
			prevItem = currentItem;
		}
	}

	if ( !mouseOutside )
		this->FlashItem( currentItem );
		
	return( currentItem );
}

/*=================================
	SUPopupWindow::HiliteItem
==================================*/
void SUPopupWindow::HiliteItem( SInt32 theItem, Boolean inHilited )
{
	this->DrawOneCell( theItem, inHilited );
}

#ifdef OLDWAY
/*=================================
	SUPopupWindow::HiliteItem
==================================*/
void SUPopupWindow::HiliteItem( Int32 theItem, Boolean inHilited )
{
	Rect	r;

	if ( !this->GetItemRect( theItem, &r ) )
		return;
		
	--r.top;
	--r.left;
	
	if ( inHilited )
	{
		::RGBForeColor( &kBlackColor );
		::FrameRect( &r );
	
		::RGBForeColor( &kWhiteColor );
		::InsetRect( &r, 2, 2 );
		::FrameRect( &r );
	}
	else
	{
		::RGBForeColor( &kWhiteColor );
		::FrameRect( &r );
		::RGBForeColor( &kBlackColor );
		
			// since the hilite destroyed a bit of the cell, redraw it
		this->DrawOneCell( theItem );
	}
	
}
#endif

#ifdef OLDWAY
/*=================================
	SUPopupWindow::HiliteItem
==================================*/
void SUPopupWindow::HiliteItem( Int32 theItem, Boolean /* hiliteIt */ )
{
	Rect	r;
	
	if ( !this->GetItemRect( theItem, &r ) )
		return;
	
	::RGBForeColor( &kBlackColor );
	::PenMode( patXor );
	::PenSize( 2, 2 );
	::FrameRect( &r );
}
#endif

/*==================================
	SUPopupWindow::LocalPointToIndex
===================================*/
SInt32 SUPopupWindow::LocalPointToIndex( Point pt )
{
	SInt32		index;
	GrafPtr		theWindow;
	
	::GetPort( &theWindow );
	Rect theRect;
#if PP_Target_Carbon
	::GetPortBounds(theWindow, &theRect);
#else
	theRect = theWindow->portRect;
#endif
	if ( !theWindow || !PtInRect( pt, &theRect ) )
		return( -1 );
		
	if ( (mCellWidth == 0) || (mCellHeight == 0) )
		return( -1 );
	
	SInt32	colNo = (pt.h - mFirstItemRect.left) / mCellWidth;
	if ( (colNo < 0) || (colNo > mCols) ) return( -1 );
	
	SInt32	rowNo = (pt.v - mFirstItemRect.top) / mCellHeight;
	if ( (rowNo < 0) || (rowNo > mRows) ) return( -1 );
	
	index = colNo + rowNo * mCols;
	
		// 8/14/96 ebs bug fix - if mouse is within popup, but not over an item (last row)
	if ( (index < 0) || (index >= mItems) )
		index = -1;
		
	return( index );
}

/*==================================
	SUPopupWindow::GetItemRect
===================================*/
Boolean SUPopupWindow::GetItemRect( SInt32 inItem, Rect *outR )
{
	if ( (inItem < 0) || (inItem >= mItems) || (mCols == 0) || (mRows == 0) )
		return( false );
		
	SInt32 whichCol = inItem % mCols;
	SInt32 whichRow = inItem / mCols;
	
	outR->left = mFirstItemRect.left + whichCol * mCellWidth;
	outR->right = outR->left + mCellWidth - 1;
	outR->top = mFirstItemRect.top + whichRow * mCellHeight;
	outR->bottom = outR->top + mCellHeight - 1;
	
	return( true );
}

/*=========================================
	SUPopupWindow::FlashItem
==========================================*/
void SUPopupWindow::FlashItem( SInt32 theItem )
{
	Rect		r;
	SInt32		numFlashes;
	Boolean		hiliteIt = false;					// unhilite it first time through
	UInt32		dummy;
	
	if ( !GetItemRect( theItem, &r ) )
		return;
		
		// we need to loop twice for each flash, plus 1 extra time to unhilite it initially
#if PP_Target_Carbon
	numFlashes = 3 * 2 + 1;
#else
	numFlashes = LMGetMenuFlash() * 2 + 1;
#endif
	while( numFlashes-- > 0 )
	{
		this->HiliteItem( theItem, hiliteIt );
		hiliteIt = !hiliteIt;
		Delay( 4, &dummy );
	}
	
}

