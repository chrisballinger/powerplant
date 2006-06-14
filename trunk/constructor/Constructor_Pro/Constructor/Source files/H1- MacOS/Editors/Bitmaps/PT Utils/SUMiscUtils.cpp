// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				SUMiscUtils.cp

CLASSES:			SUMiscUtils

DESCRIPTION:		Implementation file for Object

AUTHOR:				Eric Shapiro

CREATION DATE :		96.4.18

CHANGE HISTORY :

		96.08.28	ebs		Added ForceRectToScreen, FindDeviceViaPoint
		96.04.18	ebs		Integrated into Robin's sources

---------------------------------------<< ¥ >>-----------------------------------------
*/

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

#include <stdio.h>			// for sprintf

#include "SUMiscUtils.h"
#include "SUSaveGWorld.h"
#include "SUSaveResFile.h"
#include "UResourceMgr.h"
#include "SURegionUtils.h"

/*=================================
	DisposeHandle
	
	Disposes of a handle without having to know if it is a resource mgr or
	memory mgr one.
==================================*/
void SUMiscUtils::DisposeHandle( void *h )
{
	if ( !h ) return;	
	
		// assume a resource is purged (there's no good way of knowing for sure)
	if ( !*(Handle)h )				
	{
		::ReleaseResource( (Handle)h );	
		return;
	}

	Byte flags = ::HGetState( (Handle) h );
	if ( flags & 0x0020 )				/* a resource? */
		::ReleaseResource( (Handle) h );
	else
		::DisposeHandle( (Handle) h );
}


/*=================================
	GetMouseRelativeToPort
==================================*/
void SUMiscUtils::GetMouseRelativeToPort( Point *outPt )
{
	GrafPtr		currentPort;
	
	GetPort( &currentPort );
	ThrowIfNil_( currentPort );				// should never happen
	
	GetMouse( outPt );
	Rect theRect;
#if PP_Target_Carbon
	::GetPortBounds(currentPort, &theRect);
#else
	theRect = currentPort->portRect;
#endif
	outPt->h -= theRect.left;	// repair SetOrigin changes
	outPt->v -= theRect.top;
}

/*=================================
	GetMouseRelativeToView
	
	Note:
	A pane's coordinates are relative to the top/left of the view
	they are contained in. A view's coordinates are the top/left
	of the view itself.
	
	Changes the current view. (Should we save/restore it ???)
==================================*/
void SUMiscUtils::GetMouseRelativeToView( LPane *inPane, Point *outPt )
{
	ThrowIfNil_( inPane );
	inPane->FocusDraw();					// sets port and origin
	GetMouse( outPt );
}


/*=================================
	SetGWorld
==================================*/
void SUMiscUtils::SetGWorld( CGrafPtr inPort, GDHandle inDevice )
{
	ThrowIfNil_( inPort );					// shouldn't happen
	
	::SetGWorld( inPort, inDevice );
	
		// tell PP the port has changed since it caches it
	LView::OutOfFocus( nil );	
}

/*=================================
	SetPort
	
	Sets the current port, invalidates PowerPlant's LPane cache,
	sets the origin to 0,0, the GDevice to the main device,
	and the clip rect to the port rect.
==================================*/
void SUMiscUtils::SetPort( GrafPtr inPort )
{
	if ( inPort )
	{
		if ( UQDGlobals::GetCurrentPort() != inPort )
			::SetPort( inPort );
			
			// set the gdevice to the main device (if it isn't already)
			// the Window Mgr requires this when opening windows, etc
		GDHandle mainDevice = ::GetMainDevice();
		if ( GetGDevice() != mainDevice )
			::SetGDevice( mainDevice );
			
		::SetOrigin( 0, 0 );					// since PP changes this all the time
		Rect theRect;
#if PP_Target_Carbon
		::GetPortBounds(inPort, &theRect);
#else
		theRect = inPort->portRect;
#endif
		::ClipRect( &theRect );		// since PP changes this all the time
		
			// tell PP the port has changed since it caches it
		LView::OutOfFocus( nil );	
	}
}

/*=================================
	IsShiftKey
==================================*/
Boolean SUMiscUtils::IsShiftKey()
{
	Boolean isShiftKey;
	EventRecord theEvent;
	
	EventAvail(0, &theEvent);
	isShiftKey = (theEvent.modifiers & shiftKey) == shiftKey;
	
	return isShiftKey;
}

/*=================================
	IsOptionKey
==================================*/
Boolean SUMiscUtils::IsOptionKey( void )
{
	Boolean isOptionKey;
	EventRecord theEvent;
	
	EventAvail(0, &theEvent);
	isOptionKey = (theEvent.modifiers & optionKey) == optionKey;
	
	return isOptionKey;
}

/*============================================
	SUMiscUtils::ClearMemory
==============================================*/
void SUMiscUtils::ClearMemory( void *theBuffer, UInt32 numBytes )
{
	UInt8 *p = (UInt8*) theBuffer;
	
	while( numBytes-- > 0 )
		*p++ = 0;
}


/*============================================
	SUMiscUtils::SetMemory
==============================================*/
void SUMiscUtils::SetMemory( void *buffer, UInt8 toWhat, UInt32 numBytes )
{
	UInt8	*p = (UInt8*) buffer;
	
	ThrowIfNil_( p );					// can't hurt to check
	
	while( numBytes-- > 0 )
		*p++ = toWhat;
}

/*============================================
	SUMiscUtils::DuplicateHandle
==============================================*/
Handle SUMiscUtils::DuplicateHandle( Handle source )
{
	ThrowIf_ ( !source || !*source );
	
	SInt32 numBytes = GetHandleSize( source );
	Handle result = NewHandle( numBytes );
	ThrowIfMemFail_( result );
	
	BlockMoveData( *source, *result, numBytes );
	return( result );
}


/*============================================
	SUMiscUtils::GetTopView
==============================================*/
LView *SUMiscUtils::GetTopView( LPane *inPane )
{
	LView	*theView, *superView;
	
	ThrowIfNil_( inPane );

	/*
		note: if the pane has no superview, it might be the top view itself
		so we'll just return it.
		
		is this ok??? what if a pane just isn't part of any hierarchy and isn't
		an LView at all?
	*/	
	theView = (LView*) inPane;
	while( true )
	{
		superView = theView->GetSuperView();
		if ( !superView )
			break;
		else
			theView = superView;
	}
	return theView;
}

/*============================================
	SUMiscUtils::FindSibblingPaneByID
==============================================*/
LPane *SUMiscUtils::FindSibblingPaneByID( LPane *inPane, PaneIDT inPaneID )
{
	ThrowIfNil_( inPane );
	
	LView *topView = SUMiscUtils::GetTopView( inPane );
	ThrowIfNil_( topView );
	
	return( topView->FindPaneByID( inPaneID ) );
}

/*============================================
	SUMiscUtils::GetWindowGlobalBounds
==============================================*/
void SUMiscUtils::GetWindowGlobalBounds( WindowPtr inWindow, Rect &oRect )
{
	StSaveGWorld	aSaver;
	
	SetPortWindowPort( inWindow );
#if PP_Target_Carbon
	::GetWindowPortBounds(inWindow, &oRect);
#else
	oRect = inWindow->portRect;
#endif
	::LocalToGlobal( (Point*) &oRect );
	::LocalToGlobal( 1 + (Point*) &oRect );
}

/*============================================
	SUMiscUtils::LinkListenerToControls
	
	Note:
	This is the same as the UReanimator method, but this will work with
	any broadcaster instead of just LControl.
==============================================*/
void SUMiscUtils::LinkListenerToControls(
		LListener	*inListener,
		LView		*inControlContainer,
		ResIDT		inResListID)
{
	#pragma options align=mac68k

	typedef struct {
		SInt16	numberOfItems;
		PaneIDT	itemID[1];
	} SResList, *SResListP;

	#pragma options align=reset

	StResource	idList('RidL', inResListID);
	HLockHi(idList.mResourceH);
	SResListP	resListP = (SResListP) *idList.mResourceH;
	
	for (SInt16 i = 0; i < resListP->numberOfItems; i++) 
	{
		LPane* thePane = inControlContainer->FindPaneByID(resListP->itemID[i] ) ;
		LBroadcaster 	*theBroad = dynamic_cast<LBroadcaster*>(thePane);
		if (theBroad != nil) {
			theBroad->AddListener( inListener );
		}
	}
}

/*============================================
	SUMiscUtils::RedrawPaneAsIndicated
==============================================*/
void SUMiscUtils::RedrawPaneAsIndicated( LPane *inPane, ERedrawOptions inOptions )
{
	if ( inOptions == redraw_Now )
		inPane->Draw( nil );
	else if ( inOptions == redraw_Later )
		inPane->Refresh();
}

/*=====================================
	CreatePPWindow
	
	Description:
	Safer way to create a window.
======================================*/
LWindow *SUMiscUtils::CreatePPWindow( ResIDT inWindowID )
{
	StSaveGWorld			aSaver;
	LWindow 				*theWindow = nil;
	
		// create the window
	::SetGDevice( GetMainDevice() );				// bug in Mac toolbox requires this
	theWindow = LWindow::CreateWindow( inWindowID, LCommander::GetTopCommander() );
	return( theWindow );
}

/*=========================================
	SUMiscUtils::ForceRectToScreen
	
	Description:
	Pulls a rectangle to the screen it's on.
	
	Note:
	Requires color qd.
==========================================*/
void SUMiscUtils::ForceRectToScreen( const Rect &inRect, Rect *outRect, GDHandle inScreen )
{
	GDHandle	theDevice = nil;
	Rect		deviceR, tempR;
		
	*outRect = inRect;			// assume no changes to rectangle
	
	/****************************************
		figure out which screen to use
	****************************************/
	if ( inScreen )
		theDevice = inScreen;
	else
		theDevice = UWindows::FindDominantDevice( inRect );
		
	if ( !theDevice )
		theDevice = ::GetMainDevice();
	if ( !theDevice )
		return;						// shouldn't happen
	
	deviceR = (**theDevice).gdRect;
	if ( theDevice == ::GetMainDevice() )
		deviceR.top += ::GetMBarHeight();
	
	/****************************************
		see if it is completely within a single monitor rectangle
	****************************************/
	::SectRect( &deviceR, &inRect, &tempR );
	if ( !::EqualRect( &tempR, &inRect ) )
	{
		/****************************************
			move it as needed
		****************************************/
		if ( inRect.left < deviceR.left )
			::OffsetRect( outRect, deviceR.left - inRect.left, 0 );
		else if ( inRect.right > deviceR.right )
			::OffsetRect( outRect, deviceR.right - inRect.right, 0 );
			
		if ( inRect.top < deviceR.top )
			::OffsetRect( outRect, 0, deviceR.top - inRect.top );
		else if ( inRect.bottom > deviceR.bottom )
			::OffsetRect( outRect, 0, deviceR.bottom - inRect.bottom );
	}
}

/*=========================================
	SUMiscUtils::FindDeviceViaPoint
	
	Note:
	Requires color qd.
==========================================*/
GDHandle SUMiscUtils::FindDeviceViaPoint( Point inGlobalPt )
{
	GDHandle		theDevice = ::GetDeviceList();
	
	while( theDevice )
	{
		if ( UDrawingUtils::IsActiveScreenDevice( theDevice ) )
		{
			Rect	deviceR = (**theDevice).gdRect;
		
			if ( ::PtInRect( inGlobalPt, &deviceR ) )
				return( theDevice );
		}
		
		theDevice = ::GetNextDevice( theDevice );
	}
	
	return( nil );			// no hit
}

#ifdef EBS_OUT		// can't do this -- no way to tell a LControl (which is a broadcaster)
					//   from an LPane (which isn't) ???
					
void SUMiscUtils::LinkListenerToEntireWindow()
{
		LListIterator iterator(mSubPanes, iterate_FromStart);
		LPane	*theSub;
		while (iterator.Next(&theSub)) {
			thePane = theSub->FindPaneByID(inPaneID);
			if (thePane != nil) break;
		}

}
#endif