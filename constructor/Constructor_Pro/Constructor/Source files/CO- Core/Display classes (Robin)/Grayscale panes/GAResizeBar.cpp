// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1996 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< * >>-----------------------------------------

FILE:					GAResizeBar.cpp

CLASSES:				GAResizeBar

DESCRIPTION:		Implementation file for GAResizeBar

AUTHOR:				Robin Mair

CREATION DATE :	96.06.22

CHANGE HISTORY :

		97.02.21		rtm	Added a few new accessors for checking whether the first or
								second panes have either their height or width obscured, these
								are used by the resize borders to handle automatic adjustment
								of borders on resizing.
		96.11.24		rtm	Changed the way the commander to which the resize border manager
								attachment is found, we now have an additional field that is
								used to specify the pane ID for the commander that has the
								attachment attached to it.  This change allows us to more easily
								support allowing the user to add the ressize border attachment
								to any view that is also a commander.
		96.11.23		rtm	Added in support for the zoom view which essentially allows bars
								to be embedded in views that can then have their embedded borders
								zoomed within the view instead of just a window as we previously
								has it working.
		96.08.20		rtm	Added a method for resetting the references to a bars panes, this
								is primarily used by the resize border management attachment
								when handling the zooming of borders.
		96.08.06		rtm	Changed the MoveBy method so that it now does a test on the
								mEnabled field instead of calling IsEnabled which also tested
								for visibilty which was preventing us from being able to restore
								a layout before a window was shown, now a layout can be restored
								and then the window can be shown.
		96.07.22		rtm	Found and fixed a nasty little bug in GetResizeBarInfo where
								the coordinates being saved for the resize bars position had
								to be converted to local coordinates before being saved, yikes!!
		96.07.19		rtm	Added support for retrieving and restoring positional info for
								the resize bar, this can be used to save off the current
								position of the resize bar.  This can then later be used to
								restore the last state of the bar.
		96.07.18		rtm	Added a utility function for positioning a resize bar at a
								specific location, a location value is passed to this function
								and the bar is positioned based on its current orientation.
		96.07.11		rtm	Fixed another problem with the switching of orientationn that
								would result in any middle panes being scrunched up onto their
								resize bar, essentially middle panes had to be tweaked by the
								half height/width of the resize bar.
		96.07.10		rtm	Fixed problems in SetOrientation method with the setting of 
								bindings when switching a three pane browser.
		96.07.10		rtm	Excorcised some last minute off by one division errors that were
								resulting in the borders sometimes being located in the wrong
								position, whew! this was a tough one.
		96.07.09		rtm	Altered the CalcMinAndMaxConstraint method so that it now correctly
								takes into account if there is a palette on either the top or left 
								edge of the window.
		96.07.08		rtm	Implemented the ability for the direction of a resize bar to be
								altered which will result in the correct thing happening to its
								associated borders.
		96.07.03		rtm	Needed to add a DisposeCCursor call in teh AdjustCorsor() method
								as it was leaking the way it was originally implemented.
		96.06.27		rtm	Tweaked the CalcMaxAndMinConstraints method so that it now does
								its calculations without using its superviews size, this will
								make it work better in situations where there might be more than
								one resize bar.
		96.06.26		rtm	Added a little more slop to the hit testing for the cursor.
		96.06.25		rtm	Fixed a couple of bugs in the vertical resize bar code which had
								not been tested until today, the problems were primarily in the
								calculations of the constraints for dragging.
		96.06.24		rtm	Changed the way the sizes of the panes associated with the bar
								are adjusted, this now happens when the bar is moved, thus we
								can also use this mechanism in cases where we need to adjust the
								panes because they no longer fit in a window
		96.06.23		rtm	Added the ability for the resize bar to broadcast a message when
								it is moved.
		96.06.22		rtm	Initial creation of file

---------------------------------------<< * >>-----------------------------------------
*/

	// Prefix file for MSVC build
#ifdef WINVER
	#include "ctor.h"
#endif

#include "GAResizeBar.h"
#include "GAResizeBorder.h"
#include "GResizeBorderMgr.h"

// * GRAYSCALE HEADERS
#include <UGraphicsUtilities.h>
#include <UGAColorRamp.h>

// * GENERAL
#include	<stdlib.h>


//=====================================================================================
// CONSTANTS
//=====================================================================================

//=====================================================================================
// DEFINES
//=====================================================================================

#pragma mark === CLASS: GAResizeBar

//=====================================================================================
// ** STREAM CREATION
//-------------------------------------------------------------------------------------
// GAResizeBar::CreateGAResizeBarStream	
//-------------------------------------------------------------------------------------
//

GAResizeBar*	
GAResizeBar::CreateGAResizeBarStream ( LStream *inStream )
{

	return ( new GAResizeBar ( inStream ));
	
}	// GAResizeBar::CreateGAResizeBarStream

#pragma mark -
#pragma mark === INITIALIZATION

//=====================================================================================
// CLASS:	GAResizeBar
//=====================================================================================

//=====================================================================================
// ** INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// GAResizeBar::GAResizeBar					---<<* CONSTRUCTOR *>>---
//-------------------------------------------------------------------------------------

GAResizeBar::GAResizeBar ()
{

	mFirstPaneID = PaneIDT_Unspecified;
	mSecondPaneID = PaneIDT_Unspecified;
	mHorizontal = true;
	mEndInsets.h = resizeBar_MinInset;
	mEndInsets.v = resizeBar_MinInset;
	mPaintBackground = false;
	mFirstPane = nil;
	mSecondPane = nil;
	mZoomCommanderID = PaneIDT_Undefined;
	mInitialMouseOffset = 0;
	
}	//	GAResizeBar::GAResizeBar


//-------------------------------------------------------------------------------------
// GAResizeBar::GAResizeBar					---<<* CONSTRUCTOR *>>---
//-------------------------------------------------------------------------------------

GAResizeBar::GAResizeBar (	LStream*		inStream ) : LPane ( inStream )
{

	Int16	endInsetValue;
	
	//	* Setup the data members for the view
	mInitialMouseOffset = 0;
	mFirstPane = nil;
	mSecondPane = nil;
	inStream->ReadData ( &mFirstPaneID, sizeof (PaneIDT));
	inStream->ReadData ( &mSecondPaneID, sizeof (PaneIDT));
	inStream->ReadData ( &endInsetValue, sizeof (Int16));
	mEndInsets.h = endInsetValue;
	inStream->ReadData ( &endInsetValue, sizeof (Int16));
	mEndInsets.v = endInsetValue;
	inStream->ReadData ( &mPaintBackground, sizeof (Boolean));
	inStream->ReadData ( &mResizeMessage, sizeof (MessageT));
	inStream->ReadData ( &mZoomCommanderID, sizeof (PaneIDT));

	// * Figure out if we are a vertical or horizontal bar
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );
	if ( UGraphicsUtilities::RectWidth ( localFrame ) > 
				UGraphicsUtilities::RectHeight ( localFrame ))
		mHorizontal = true;
	else
		mHorizontal = false;
	
	// * Make sure we start broadcasting
	StartBroadcasting ();
	
}	//	GAResizeBar::GAResizeBar


//-------------------------------------------------------------------------------------
// GAResizeBar::~GAResizeBar				---<<* DESTRUCTOR *>>---
//-------------------------------------------------------------------------------------

GAResizeBar::~GAResizeBar ()
{
}	//	GAResizeBar::~GAResizeBar


//-------------------------------------------------------------------------------------
// GAResizeBar::FinishCreateSelf
//-------------------------------------------------------------------------------------

void
GAResizeBar::FinishCreateSelf ()
{

	// * Setup references to the first and second panes associated with this bar
	// if the pane id has not been specified then we just set the reference to nil
	ResetPaneReferences ();
	
	// * Finally we need to make sure we get the bar registered with the attachment
	// that manages the zooming of panes so that the bar can be shown and hidden
	// as needed when a border is zoomed.  For this we use the mZoomCommander field
	// which tells us which pane the attachment has been added to, if the ID is not
	// specified then we assume it has been added to the window, this means we are
	// backwards compatible with layouts that used this class before this field was
	// added
	LWindow*	theWindow = GetContainingWindow ();
	if ( mZoomCommanderID == PaneIDT_Undefined )
	{
		if ( theWindow )
			theWindow->ProcessCommand ( cmd_RegisterResizeBar, this );
	}
	else
	{
		LCommander*	theZoomCommander = dynamic_cast<LCommander*>(theWindow->FindPaneByID ( mZoomCommanderID ));
		if ( theZoomCommander )
			theZoomCommander->ProcessCommand ( cmd_RegisterResizeBar, this );
	}
		
}	//	GAResizeBar::FinishCreateSelf


#pragma mark -
#pragma mark === ACCESSORS

//=====================================================================================
// ** ACCESSORS
//-------------------------------------------------------------------------------------
// GAResizeBar::CalcLocalResizeBarRect
//-------------------------------------------------------------------------------------

void
GAResizeBar::CalcLocalResizeBarRect (	Rect	&outRect )
{
	
	CalcLocalFrameRect ( outRect );
	
	// * We need to adjust the size of the rectangle before we draw in case the
	// resize bar is larger than the minimum size of three pixels
	if ( IsHorizontal ())
	{
		if ( UGraphicsUtilities::RectHeight ( outRect ) > resizeBar_MinWidth )
		{
			outRect.top += ( UGraphicsUtilities::RectHeight ( outRect ) - resizeBar_MinWidth ) / 2;
			outRect.bottom = outRect.top + resizeBar_MinWidth;
		}
	}
	else
	{
		if ( UGraphicsUtilities::RectWidth ( outRect ) > resizeBar_MinWidth )
		{
			outRect.left += ( UGraphicsUtilities::RectWidth ( outRect ) - resizeBar_MinWidth ) / 2;
			outRect.right = outRect.left + resizeBar_MinWidth;
		}
	}
		
	// * Adjust the rect for the insets if needed
	if ( mEndInsets != gEmptyPoint )
	{
		//	* If both insets are the same we can simply setup the insets
		// by insetting the rect
		if ( mEndInsets.h == mEndInsets.v )
			::InsetRect ( 	&outRect, 
								IsHorizontal () ? mEndInsets.h : 0,
								IsHorizontal () ? 0 : mEndInsets.v );
		else
		{
			// * End insets are different so w have to adjust the rect
			// based on the orientation of the bar
			if ( IsHorizontal ())
			{
				outRect.left += mEndInsets.h;
				outRect.right -= mEndInsets.v;
			}
			else
			{
				outRect.top += mEndInsets.h;
				outRect.bottom -= mEndInsets.v;
			}
		}
	}
			
}	//	GAResizeBar::CalcLocalResizeBarRect


//-------------------------------------------------------------------------------------
// GAResizeBar::CalcResizeFeedbackRect
//-------------------------------------------------------------------------------------

void
GAResizeBar::CalcResizeFeedbackRect (	Rect		&inResizeRect,
													Int16		inPosition )
{
	
	if ( IsHorizontal ())
	{
		inResizeRect.top = inPosition - 1;
		inResizeRect.bottom = inResizeRect.top + 3;
	}
	else
	{
		inResizeRect.left = inPosition - 1;
		inResizeRect.right = inResizeRect.left + 3;
	}
			
}	//	GAResizeBar::CalcResizeFeedbackRect


//-------------------------------------------------------------------------------------
// GAResizeBar::CalcMinAndMaxConstraints
//-------------------------------------------------------------------------------------

void
GAResizeBar::CalcMinAndMaxConstraints	(	Int16		&outMin,
														Int16		&outMax )
{
	// * Only proceed if we have the two associated panes
	if ( mFirstPane && mSecondPane )
	{
		// * Figure out how much space is available for the two associated
		// views to be resized within by figure out there bounds
		// and from that determining the avaiable space either vertically or 
		// horizontally
		Int16		availableSpace;
		Rect		portFrame;
		mFirstPane->CalcPortFrameRect ( portFrame );
		Rect		barFrame;
		CalcLocalFrameRect ( barFrame );
		Int16		halfBarWidth = IsHorizontal () ? UGraphicsUtilities::RectHeight ( barFrame ) / 2 : 
													UGraphicsUtilities::RectWidth ( barFrame ) / 2;
		availableSpace = IsHorizontal () ? portFrame.top : portFrame.left;
		mSecondPane->CalcPortFrameRect ( portFrame );
		availableSpace = (IsHorizontal () ? portFrame.bottom : portFrame.right) - 
													availableSpace;
		availableSpace -= halfBarWidth;
		
		// * Figure out what the minimum and maximum size can be so
		// that we can correctly constrain the drag, we do this by getting
		// the minimum size for the first and second panes and using those
		// values to determine the min and max - min is minimum of first pane
		// max is available space for the two panes minus minimum of second pane
		mFirstPane->CalcPortFrameRect ( portFrame );
		SPoint32		superLoc;
		mSuperView->GetFrameLocation ( superLoc );
		if ( IsHorizontal ())
			outMin = (portFrame.top + mFirstPane->GetMinimumHeight () + halfBarWidth) - 
							superLoc.v;
		else
			outMin = (portFrame.left + mFirstPane->GetMinimumWidth () + halfBarWidth) - 
							superLoc.h;
		
		Int16 resizeMin2;
		if ( IsHorizontal ())
			resizeMin2 = mSecondPane->GetMinimumHeight ();
		else
			resizeMin2 = mSecondPane->GetMinimumWidth ();
		
		// * Figure out the maximum value
		if ( IsHorizontal ())
			outMax =  (portFrame.top + (availableSpace - resizeMin2)) - superLoc.v;
		else
			outMax = (portFrame.left + (availableSpace - resizeMin2)) - superLoc.h;
			
	}
	
}	//	GAResizeBar::CalcMinAndMaxConstraints


//-------------------------------------------------------------------------------------
// GAResizeBar::GetContainingWindow
//-------------------------------------------------------------------------------------

LWindow*
GAResizeBar::GetContainingWindow ()
{
	
	LPane*	super = mSuperView;
	if ( super )
	{
		while ( super != nil ) 
		{
			if ( super->GetSuperView ())
				super = super->GetSuperView ();
			else
				return (LWindow*)super;
		}
	}

	return nil;
		
}	// GAResizeBar::GetContainingWindow


//-------------------------------------------------------------------------------------
// GAResizeBar::GetFirstPaneWidth
//-------------------------------------------------------------------------------------

Int16
GAResizeBar::GetFirstPaneWidth ()
{
	if ( mFirstPane )
	{
		SDimension16	frameSize;
		mFirstPane->GetFrameSize ( frameSize );
		return frameSize.width;
	}
	else
		return 0;
		
}	//	GAResizeBar::GetFirstPaneWidth


//-------------------------------------------------------------------------------------
// GAResizeBar::GetFirstPaneHeight
//-------------------------------------------------------------------------------------

Int16
GAResizeBar::GetFirstPaneHeight ()
{
	if ( mFirstPane )
	{
		SDimension16	frameSize;
		mFirstPane->GetFrameSize ( frameSize );
		return frameSize.height;
	}
	else
		return 0;
		
}	//	GAResizeBar::GetFirstPaneHeight


//-------------------------------------------------------------------------------------
// GAResizeBar::GetSecondPaneWidth
//-------------------------------------------------------------------------------------

Int16
GAResizeBar::GetSecondPaneWidth ()
{
	if ( mSecondPane )
	{
		SDimension16	frameSize;
		mSecondPane->GetFrameSize ( frameSize );
		return frameSize.width;
	}
	else
		return 0;
		
}	//	GAResizeBar::GetSecondPaneWidth


//-------------------------------------------------------------------------------------
// GAResizeBar::GetSecondPaneHeight
//-------------------------------------------------------------------------------------

Int16
GAResizeBar::GetSecondPaneHeight ()
{
	if ( mSecondPane )
	{
		SDimension16	frameSize;
		mSecondPane->GetFrameSize ( frameSize );
		return frameSize.height;
	}
	else
		return 0;
		
}	//	GAResizeBar::GetSecondPaneHeight


//-------------------------------------------------------------------------------------
// GAResizeBar::GetFirstPaneMinWidth
//-------------------------------------------------------------------------------------

Int16
GAResizeBar::GetFirstPaneMinWidth ()
{
	if ( mFirstPane )
		return mFirstPane->GetMinimumWidth ();
	else
		return 0;
		
}	//	GAResizeBar::GetFirstPaneMinWidth


//-------------------------------------------------------------------------------------
// GAResizeBar::GetFirstPaneMinHeight
//-------------------------------------------------------------------------------------

Int16
GAResizeBar::GetFirstPaneMinHeight ()
{
	if ( mFirstPane )
		return mFirstPane->GetMinimumHeight ();
	else
		return 0;
		
}	//	GAResizeBar::GetFirstPaneMinHeight


//-------------------------------------------------------------------------------------
// GAResizeBar::GetSecondPaneMinWidth
//-------------------------------------------------------------------------------------

Int16
GAResizeBar::GetSecondPaneMinWidth ()
{
	if ( mSecondPane )
		return mSecondPane->GetMinimumWidth ();
	else
		return 0;
		
}	//	GAResizeBar::GetSecondPaneMinWidth


//-------------------------------------------------------------------------------------
// GAResizeBar::GetSecondPaneMinHeight
//-------------------------------------------------------------------------------------

Int16
GAResizeBar::GetSecondPaneMinHeight ()
{
	if ( mSecondPane )
		return mSecondPane->GetMinimumHeight ();
	else
		return 0;
		
}	//	GAResizeBar::GetSecondPaneMinHeight


//-------------------------------------------------------------------------------------
// GAResizeBar::GetFirstPaneOffset
//-------------------------------------------------------------------------------------

Int16
GAResizeBar::GetFirstPaneOffset ()
{

	// * If we have a reference to the pane then we are going figure out the
	// offset from the edge of the pane to the edge of the bar based on the
	// orientation of the bar
	if ( mFirstPane )
	{
		Rect	barBounds;
		CalcLocalFrameRect ( barBounds );
		Rect	paneBounds;
		mFirstPane->CalcPortFrameRect ( paneBounds );
		Int16	height = UGraphicsUtilities::RectHeight ( barBounds );
		Int16	width = UGraphicsUtilities::RectWidth ( barBounds );
		
		return IsHorizontal () ? (barBounds.top + (height / 2)) - paneBounds.bottom :
							(barBounds.left + (width / 2)) - paneBounds.right;
	}
	else
		return 0;
		
}	//	GAResizeBar::GetFirstPaneOffset


//-------------------------------------------------------------------------------------
// GAResizeBar::GetSecondPaneOffset
//-------------------------------------------------------------------------------------

Int16
GAResizeBar::GetSecondPaneOffset ()
{

	// * If we have a reference to the pane then we are going figure out the
	// offset from the edge of the pane to the edge of the bar based on the
	// orientation of the bar
	if ( mSecondPane )
	{
		Rect	barBounds;
		CalcPortFrameRect ( barBounds );
		Rect	paneBounds;
		mSecondPane->CalcPortFrameRect ( paneBounds );
		Int16	height = UGraphicsUtilities::RectHeight ( barBounds );
		Int16	width = UGraphicsUtilities::RectWidth ( barBounds );
		
		return IsHorizontal () ? paneBounds.top - (barBounds.bottom - (height / 2)):
							paneBounds.left - (barBounds.right - (width / 2));
	}
	else
		return 0;
		
}	//	GAResizeBar::GetSecondPaneOffset


//-------------------------------------------------------------------------------------
// GAResizeBar::IsFirstPaneHeightObscured
//-------------------------------------------------------------------------------------

Boolean
GAResizeBar::IsFirstPaneHeightObscured ()
{

	return GetFirstPaneHeight () < 0 || GetFirstPaneHeight () < GetFirstPaneMinHeight ();
		
}	//	GAResizeBar::IsFirstPaneHeightObscured


//-------------------------------------------------------------------------------------
// GAResizeBar::IsFirstPaneWidthObscured
//-------------------------------------------------------------------------------------

Boolean
GAResizeBar::IsFirstPaneWidthObscured ()
{

	return GetFirstPaneWidth () < 0 || GetFirstPaneWidth () < GetFirstPaneMinWidth ();
		
}	//	GAResizeBar::IsFirstPaneWidthObscured


//-------------------------------------------------------------------------------------
// GAResizeBar::IsSecondPaneHeightObscured
//-------------------------------------------------------------------------------------

Boolean
GAResizeBar::IsSecondPaneHeightObscured ()
{

	return GetSecondPaneHeight () < 0 || GetSecondPaneHeight () < GetSecondPaneMinHeight ();
		
}	//	GAResizeBar::IsSecondPaneHeightObscured


//-------------------------------------------------------------------------------------
// GAResizeBar::IsSecondPaneWidthObscured
//-------------------------------------------------------------------------------------

Boolean
GAResizeBar::IsSecondPaneWidthObscured ()
{

	return GetSecondPaneWidth () < 0 || GetSecondPaneWidth () < GetSecondPaneMinWidth ();
		
}	//	GAResizeBar::IsSecondPaneWidthObscured


//-------------------------------------------------------------------------------------
// GAResizeBar::SetEndInsets
//-------------------------------------------------------------------------------------

void
GAResizeBar::SetEndInsets	(	Point		inEndInsets,
										Boolean	inRedraw )
{

	// * First we only change the insets if they are different
	if ( mEndInsets.h != inEndInsets.h || mEndInsets.v != inEndInsets.v )
	{
		mEndInsets = inEndInsets;
		
		// * If the user has requested that we redraw the bar then handle that now
		// we handle the drawing differently if we paint the background as opposed
		// to not painting, when we are painting we just need to do a Draw () which
		// is smoother otherwise we need a refresh
		if ( inRedraw )
			DoesPaintBackground () ? Draw ( nil ) : Refresh ();
	}
			
}	//	GAResizeBar::SetEndInsets


//-------------------------------------------------------------------------------------
// GAResizeBar::SetPaintBackground
//-------------------------------------------------------------------------------------

void
GAResizeBar::SetPaintBackground	(	Boolean	inPaint,
												Boolean	inRedraw )
{

	// * First we only change the insets if they are different
	if ( mPaintBackground != inPaint )
	{
		mPaintBackground = inPaint;
		
		// * If the user has requested that we redraw the bar then handle that now
		// we handle the drawing differently between the two states 
		if ( inRedraw )
			inPaint ? Draw ( nil ) : Refresh ();
	}
			
}	//	GAResizeBar::SetPaintBackground


//-------------------------------------------------------------------------------------
// GAResizeBar::SetOrientation
//-------------------------------------------------------------------------------------
//
//	This method is called to change the direction in which the bar moves.  A change will
//		result in the bars associated panes being adjusted to meet the new direction, this
//		was implemented to support the feature used in the IDE for altering the config-
//		uration of a browser such that the lists are on the left or the top.

void
GAResizeBar::SetOrientation	(	Boolean	inHorizontal,
											Int16		inNewPosition,
											Int16		inBarCount,
											Int16		inCurrBarIndex )
{

	// * Only make the change if in fact there is a difference
	if ( IsHorizontal () != inHorizontal )
	{
		mHorizontal = inHorizontal;
		
		// * We start by first adjusting the position and sizes of
		// our associated borders
		if ( mFirstPane && mSecondPane )
		{
			// * Get the position and size for our superview
			SDimension16	superFrameSize;
			mSuperView->GetFrameSize ( superFrameSize );
			SPoint32			superFramePos;
			mSuperView->GetFrameLocation ( superFramePos );
			
			Int16 			borderSize;
			SDimension16	newBarSize;
			GetFrameSize ( newBarSize );
			
			// * Turn off the bar bindings
			SBooleanRect bindings = { false, false, false, false };
			SetFrameBinding ( bindings );
			
			// * HORIZONTAL BAR
			if ( inHorizontal )
			{
				// * Figure out the new bar size
				newBarSize.height = newBarSize.width;
				newBarSize.width = superFrameSize.width;
				borderSize = (inNewPosition / inCurrBarIndex) - (newBarSize.height / 2);
				
				// * ADJUST FIRST BORDER
				//	* SIZE
				// * We only adjust the size of the first pane if this is the
				// first resize bar otherwise its size gets setup as the second pane
				// for the previous bar
				if ( inCurrBarIndex == 1 )
				{
					// * We need to tweak the first panes bindings such that the right
					// edge is bound
					mFirstPane->GetFrameBinding ( bindings );
					bindings.top = true;
					bindings.left = true;
					bindings.bottom = false;
					bindings.right = true;
					mFirstPane->SetFrameBinding ( bindings );
				
					mFirstPane->ResizeFrameTo ( superFrameSize.width, borderSize, false );
				}
				else
				{
					// * For any middle panes we need to make sure that they are
					// only bound to the left and right edges
					bindings.top = false;
					bindings.left = true;
					bindings.bottom = false;
					bindings.right = true;
					mFirstPane->SetFrameBinding ( bindings );
				}
				
				// * ADJUST SECOND BORDER
				// * POSITION
				mSecondPane->PlaceInSuperFrameAt ( 	0,
																inNewPosition + (newBarSize.height / 2),
																false );
																
				//	* SIZE
				// * Before we resize the second pane we need to make sure that
				// border size is in synch with that of our superview so that we
				// don't have any glitches on the edges of the window.  We only
				// need to worry about this for the scond pane which is always at
				// either the right or bottom edge of the window.  If the size is
				// out of synch we synch it up!!  If it doesn't need synching we
				// also need to check to make sure we tweak the border size for
				// any panes that are essentially in the middle, not touching the
				// windows edge, for the half height of the resize bar
				if ( inBarCount == inCurrBarIndex )
				{
					Int16	correctHeight = superFrameSize.height - (inNewPosition + (newBarSize.height / 2));
					if ( borderSize != correctHeight )
					{
						borderSize = correctHeight;
					}
				}
				else
				{
					if ( inBarCount > 1 && inCurrBarIndex == 1 || inCurrBarIndex < inBarCount )
						borderSize -= newBarSize.height / 2;
				}
				mSecondPane->ResizeFrameTo ( newBarSize.width, borderSize, false );
				
				// * POSITION & SIZE OF RESIZE BAR
				// * NOTE: here we use a feature of the resize bar that will suppress
				// the adjusting of the associated panes when the bar is moved, this
				// is done by disabling the bar
				Disable ();
				PlaceInSuperFrameAt ( 	0, 
												inNewPosition - (newBarSize.height / 2),
												false );
				ResizeFrameTo ( newBarSize.width, newBarSize.height, false );
				Enable ();
				
				// * Final step is to get our superview refreshed so that all of
				// these changes can take effect
				mSuperView->Refresh ();
				
				// * Now fixup the bars bindings
				bindings.left = true;
				bindings.right = true;
				SetFrameBinding ( bindings );
				
			}
			else	//	* VERTICAL BAR
			{
				// * Figure out the new bar size
				newBarSize.width = newBarSize.height;
				newBarSize.height = superFrameSize.height;
				borderSize = (inNewPosition / inCurrBarIndex) - (newBarSize.width / 2);
				
				// * Adjust the size of the first border
				//	* SIZE
				if ( inCurrBarIndex == 1 )
				{
					// * We need to tweak the first panes bindings such that the right
					// edge is bound
					mFirstPane->GetFrameBinding ( bindings );
					bindings.right = false;
					bindings.bottom = true;
					mFirstPane->SetFrameBinding ( bindings );
				
					mFirstPane->ResizeFrameTo ( borderSize, superFrameSize.height, false );
				}
				else
				{
					// * For any middle panes we need to make sure that they are
					// only bound to the left and right edges
					bindings.top = true;
					bindings.left = false;
					bindings.bottom = true;
					bindings.right = false;
					mFirstPane->SetFrameBinding ( bindings );
				}
				
				// * Adjust the size and position of the second border
				// * POSITION
				mSecondPane->PlaceInSuperFrameAt (  inNewPosition + (newBarSize.width / 2),
																0,
																false );
																
				//	* SIZE
				// * Before we resize the second pane we need to make sure that
				// border size is in synch with that of our superview so that we
				// don't have any glitches on the edges of the window.  We only
				// need to worry about this for the scond pane which is always at
				// either the right or bottom edge of the window.  If the size is
				// out of synch we synch it up!!  If it doesn't need synching we
				// also need to check to make sure we tweak the border size for
				// any panes that are essentially in the middle, not touching the
				// windows edge, for the half width of the resize bar
				if ( inBarCount == inCurrBarIndex )
				{
					Int16	correctWidth = superFrameSize.width - (inNewPosition + (newBarSize.width / 2));
					if ( borderSize != correctWidth )
					{
						borderSize = correctWidth;
					}
				}
				else
				{
					if ( inBarCount > 1 && inCurrBarIndex == 1 || inCurrBarIndex < inBarCount )
						borderSize -= newBarSize.width / 2;
				}
				mSecondPane->ResizeFrameTo ( borderSize, newBarSize.height, false );

				// * POSITION & SIZE OF RESIZE BAR
				// * NOTE: here we use a feature of the resize bar that will suppress
				// the adjusting of the associated panes when the bar is moved, this
				// is done by disabling the bar
				Disable ();
				PlaceInSuperFrameAt ( 	inNewPosition - (newBarSize.width / 2),
												0, 
												false );
				ResizeFrameTo ( newBarSize.width, newBarSize.height, false );
				Enable ();
				
				// * Final step is to get our superview refreshed so that all of
				// these changes can take effect
				mSuperView->Refresh ();
				
				// * Now fixup the bars bindings
				bindings.top = true;
				bindings.bottom = true;
				SetFrameBinding ( bindings );
				
			}
		}
	}
			
}	//	GAResizeBar::SetOrientation


//-------------------------------------------------------------------------------------
// GAResizeBar::ResetPaneReferences
//-------------------------------------------------------------------------------------

void
GAResizeBar::ResetPaneReferences ()
{

	// * Setup references to the first and second panes associated with this bar
	// if the pane id has not been specified then we just set the reference to nil
	if ( mFirstPaneID != 0 || mFirstPaneID != PaneIDT_Unspecified && 
				mSuperView != nil )
	{
		mFirstPane = (GAResizeBorder*)mSuperView->FindPaneByID ( mFirstPaneID );
		ThrowIfNil_ ( mFirstPane );
	}
	else
		mFirstPane = nil;
		
	if ( mSecondPaneID != 0 || mSecondPaneID != PaneIDT_Unspecified && 
				mSuperView != nil  )
	{
		mSecondPane = (GAResizeBorder*)mSuperView->FindPaneByID ( mSecondPaneID );
		ThrowIfNil_ ( mSecondPane );
	}
	else
		mSecondPane = nil;
		
}	//	GAResizeBar::ResetPaneReferences


#pragma mark -
#pragma mark === DRAWING

//=====================================================================================
// ** DRAWING
//-------------------------------------------------------------------------------------
// GAResizeBar::DrawSelf
//-------------------------------------------------------------------------------------

void
GAResizeBar::DrawSelf ()
{

	StColorPenState	theColorPenState;
	theColorPenState.Normalize ();
	
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );
	
	// * Setup a device loop so that we can pass our drawing routine the
	// correct screen bit depth for the drawing process
	StDeviceLoop	theLoop ( localFrame );
	Int16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		// * Get the resize bar drawn in the appropriate screen depth
		DrawResizeBar ( depth );
	}
		
}	//	GAResizeBar::DrawSelf


//-------------------------------------------------------------------------------------
// GAResizeBar::DrawResizeBar
//-------------------------------------------------------------------------------------

void
GAResizeBar::DrawResizeBar (	Int16		inBitDepth )
{

	Boolean	enabled = IsEnabled ();
	Boolean	active = IsActive ();
	
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );
	
	// * First we start out by painting the face of the border, if needed
	if ( DoesPaintBackground ())
	{
		::RGBForeColor ( inBitDepth < 4 ? &UGAColorRamp::GetWhiteColor () :
											&UGAColorRamp::GetColor(2) );
		::PaintRect ( &localFrame );
	}

	// * Now get the rectangle that we use to draw the resize bar this
	// rectangle is adjusted such that it is always centered on the bar
	// for a given location and has the insets applied
	CalcLocalResizeBarRect ( localFrame );
	
	// * COLOR
	if ( inBitDepth >= 4 )
	{
		// * No we can proceed with the drawing of the frame around the
		// inset sub pane
		//	* TOPLEFT EDGES
		::RGBForeColor ( enabled ? ( active ? &UGAColorRamp::GetWhiteColor() : 
															&UGAColorRamp::GetColor(1)) :
																&UGAColorRamp::GetColor(1)); 
		UGraphicsUtilities::TopLeftSide ( &localFrame, 0, 0, 1, 1 );
		
		//	* BOTTOM RIGHT EDGES
		::RGBForeColor ( enabled ? ( active ? &UGAColorRamp::GetColor(5) : 
															&UGAColorRamp::GetColor(4)) :
																&UGAColorRamp::GetColor(4)); 
		UGraphicsUtilities::BottomRightSide  (&localFrame, 1, 1, 0, 0 );
	}
	else		//	* BLACK & WHITE
	{
		// * No we can proceed with the drawing of the frame around the
		// inset sub pane
		//	* TOPLEFT EDGES
		::RGBForeColor ( &UGAColorRamp::GetBlackColor() ); 
		UGraphicsUtilities::TopLeftSide ( &localFrame, 0, 0, 1, 1 );
		
		//	* BOTTOM RIGHT EDGES
		UGraphicsUtilities::BottomRightSide  (&localFrame, 1, 1, 0, 0 );
	}
			
}	//	GAResizeBar::DrawResizeBar


//-------------------------------------------------------------------------------------
// GAResizeBar::DrawResizeFeedback
//-------------------------------------------------------------------------------------

void
GAResizeBar::DrawResizeFeedback ( 	Rect	&inFrame,
												Int16	inPosition )
{

	// * Setup the resize rect starting from the frame 
	// that was passed into us
	Rect resizeRect = inFrame;
	CalcResizeFeedbackRect ( resizeRect, inPosition );
	
	// * Draw the resize bars feedback based on whether it is a
	// horizoantal or vertical bar, all we do for feedback is draw
	// a line that is three pixels wide either horizontally or
	// vertically
	if ( IsHorizontal ())
	{
		::PenSize ( 1, 3 );
		MoveTo ( resizeRect.left, resizeRect.top );
		LineTo ( resizeRect.right, resizeRect.top );
	}
	else
	{
		::PenSize ( 3, 1 );
		MoveTo ( resizeRect.left, resizeRect.top );
		LineTo ( resizeRect.left, resizeRect.bottom );
	}
	
}	//	GAResizeBar::DrawResizeFeedback


#pragma mark -
#pragma mark === MOVING

//=====================================================================================
// ** MOVING
//-------------------------------------------------------------------------------------
// GAResizeBar::MoveBy
//-------------------------------------------------------------------------------------

void
GAResizeBar::MoveBy	(	Int32		inHorizDelta,
								Int32		inVertDelta,
								Boolean	inRefresh )
{

	// * Call our superclass so that we get moved to the new position
	LPane::MoveBy ( inHorizDelta, inVertDelta, inRefresh );
	
	// * We only do this if the resize bar is enabled as there are times
	// when we might want to move the resize bar without having the
	// associated panes moved, this can then be accomplished by disabling
	// the bar first
	if ( mEnabled != triState_Off ) // 96.08.06	rtm	Changed from IsEnabled() test
	{
		
		// * Now we call AdjustPanes to get the panes adjusted to the new
		// position of the resize bar
		// * NOTE: because of the way this works only one of the deltas
		// that we are handed is the value to use the other is always zero
		AdjustPanes ( inHorizDelta != 0 ? inHorizDelta : inVertDelta );
		
		// * The final step of the process is to broadcast our message that
		// a resize has taken place in case someone wants to do something
		// we pass along the delta for the resize
		BroadcastMessage ( 	GetResizeMessage (), 
									inHorizDelta != 0 ? &inHorizDelta : &inVertDelta );	
	}
	
}	//	GAResizeBar::MoveBy


//-------------------------------------------------------------------------------------
// GAResizeBar::PositionResizeBarAt
//-------------------------------------------------------------------------------------

void
GAResizeBar::PositionResizeBarAt	(	Int32		inPosition,
												Boolean	inRefresh )
{

	// * Based on the current orientation of the bar we setup
	// the new position within its superframe
	PlaceInSuperFrameAt ( 	IsHorizontal () ? 0 : inPosition,
	 								IsHorizontal () ? inPosition : 0,
	 								inRefresh );	
	
}	//	GAResizeBar::PositionResizeBarAt


#pragma mark -
#pragma mark === RESIZE BAR INFO

//=====================================================================================
// ** RESIZE BAR INFO
//-------------------------------------------------------------------------------------
// GAResizeBar::GetResizeBarInfo
//-------------------------------------------------------------------------------------

Handle
GAResizeBar::GetResizeBarInfo	()
{

	// * Setup the handle stream that we will be using to hold the data
	Handle	barInfoH = ::NewHandle (0);
	ThrowIfNil_ ( barInfoH );
	LHandleStream	barStream ( barInfoH );
	
	// * Add the data we want saved to the stream
	SPoint32	location;
	GetFrameLocation ( location );
	PaneIDT	paneID = GetPaneID ();
	Boolean	horizontal = IsHorizontal ();
	
	// * Because a resize bar is an LPane subclass we need to change
	// the coordinates we save to local coordinates so that when we
	// call
	Point	localPoint = { location.v, location.h };
	PortToLocalPoint ( localPoint );
	Int16		position = IsHorizontal () ? localPoint.v : localPoint.h;
	
	// * Write out the data
	barStream.WriteData ( &paneID, sizeof ( PaneIDT ));
	barStream.WriteData ( &position, sizeof ( Int16 ));
	barStream.WriteData ( &horizontal, sizeof ( Boolean ));
	
	// * Return a handle to the resize bars data to the caller who
	// is responsible for disposing of the handle
	return barStream.DetachDataHandle ();
	
}	//	GAResizeBar::GetResizeBarInfo


//-------------------------------------------------------------------------------------
// GAResizeBar::SetResizeBarInfo
//-------------------------------------------------------------------------------------

void
GAResizeBar::SetResizeBarInfo	(	Handle	inResizeBarInfoH )
{

	// * Setup the handle stream that we will be using to hold the data
	ThrowIfNil_ ( inResizeBarInfoH );
	LHandleStream	barStream ( inResizeBarInfoH );
	
	// * Read in the pane ID
	PaneIDT	paneID;
	barStream.ReadData ( &paneID, sizeof ( PaneIDT ));
	
	// * Get the position of the resize bar setup from saved data
	if ( GetPaneID () == paneID )
	{
		Int16	position;
		barStream.ReadData ( &position, sizeof ( Int16 ));
		PositionResizeBarAt ( position, true );
	}
	
}	//	GAResizeBar::SetResizeBarInfo


#pragma mark -
#pragma mark === MOUSE TRACKING

//=====================================================================================
// ** MOUSE TRACKING
//-------------------------------------------------------------------------------------
// GAResizeBar::ClickSelf
//-------------------------------------------------------------------------------------

void
GAResizeBar::ClickSelf (	const SMouseDownEvent &inMouseDown )
{

	StColorPenState	theColorPenState;
	
	// * Start off with the point passed into us in the event record
	// which we convert from port to local coordinates
	Point localPt = inMouseDown.whereLocal;

	// * Get the local frame of the bar as our starting point for
	// handling the resizing feedback, this works as it should
	// because a resize bar is an LPane subclass and therefore
	// uses the coordinate system of its superview
	Rect 		localFrame;
	CalcLocalFrameRect ( localFrame );
	
	// * Setup the starting and last position
	Int16 	currPos = IsHorizontal () ? localPt.v : localPt.h;
	Int16 	lastPos = currPos;
	
	// * Figure out what the minimum and maximum size can be so
	// that we can correctly constrain the drag
	Int16 resizeMin; 
	Int16	resizeMax;
	CalcMinAndMaxConstraints ( resizeMin, resizeMax );
	
	// * If the current position is already less than the minimum
	// then set it to the minimum, this will result in the feedback
	// line on the screen jumping to the minimum location providing
	// the user with an idea as to the approriate minimum, we do the
	// same for the maximum if needed
	if ( currPos < resizeMin )
	{
		currPos = resizeMin;
		lastPos = currPos;
	}
	else if ( currPos > resizeMax )
	{
		currPos = resizeMax;
		lastPos = currPos;
	}
	
	// * Setup the offset value, this is the distance from either the left
	// or top edge of the bar that the mouse went down.
	if ( IsHorizontal ())
		mInitialMouseOffset = currPos - localFrame.top;
	else
		mInitialMouseOffset = currPos - localFrame.left;
		
	// * Now we start getting ready to draw the feedback which is drawn
	// with a gray pattern
	FocusDraw ();
	PenMode ( patXor );
	PenPat ( &qd.gray );
	::RGBForeColor ( &UGAColorRamp::GetBlackColor ());
	DrawResizeFeedback ( localFrame, currPos );
	
	// * While the mouse button is down we keep tracking the cursor and
	// adjusting the feedback
	while ( ::StillDown ())
	{
		Point	nextPoint;
		::GetMouse ( &nextPoint );
		currPos = IsHorizontal () ? nextPoint.v : nextPoint.h;
		
		// * Make sure we constrain the feedback appropriately
		if ( currPos < resizeMin || currPos > resizeMax )
			currPos = lastPos;
			
		// * If the position has actually changed then we do something
		if ( currPos != lastPos ) 
		{
			// * Erase the last position of the feedback
			DrawResizeFeedback ( localFrame, lastPos );
			
			// * Draw the new feedback
			DrawResizeFeedback ( localFrame, currPos );
			lastPos = currPos;
		}
	}
	
	// * we do one last drawing of the feedback when we exit in
	// order to erase the last position
	DrawResizeFeedback ( localFrame, lastPos );
	
	// * Now we need to calculate the delta by which the resize bar
	// was moved so that we can adjust the position of the bar which
	//in turn will result in the associated panes being adjusted
	Int16	delta = IsHorizontal () ? currPos - localPt.v : currPos - localPt.h;
	
	// * Get the panes adjusted to the new delta, we only do something 
	// if the delta is not zero, this is accomplished by moving the
	// resize bar itself which will then trigger the adjusting of the
	// associated panes
	if ( delta != 0 )
	{
		MoveBy ( IsHorizontal () ? 0 : delta, 
					IsHorizontal () ? delta : 0, 
					false );
	}
	
	// * The last thing we do is clear the initial offset field
	mInitialMouseOffset = 0;
	
}	//	GAResizeBar::ClickSelf



//-------------------------------------------------------------------------------------
// GAResizeBar::AdjustPanes
//-------------------------------------------------------------------------------------

void
GAResizeBar::AdjustPanes	(	Int32 inDelta )
{

	// * Now we need to move and resize the associated panes
	if ( mFirstPane )
	{
		// * The first pane only needs to be resized as it is either
		// at the top or left of the resize bar and therefore fixed
		if ( IsHorizontal ())
		{
			mFirstPane->ResizeFrameBy ( 0, inDelta, false );
			mSuperView->Refresh ();
		}
		else
		{
			mFirstPane->ResizeFrameBy ( inDelta, 0, false );
			mSuperView->Refresh ();
		}
	}
	
	// * For the second pane we need to both change the size of the pane
	// as well as move it as needed
	if ( mSecondPane )
	{
		if ( IsHorizontal ())
		{
			mSecondPane->ResizeFrameBy ( 0, -inDelta, false );
			mSecondPane->MoveBy ( 0, inDelta, false );
			mSuperView->Refresh ();
		}
		else
		{
			mSecondPane->ResizeFrameBy ( -inDelta, 0, false );
			mSecondPane->MoveBy ( inDelta, 0, false );
			mSuperView->Refresh ();
		}
	}
	
}	//	GAResizeBar::AdjustPanes



#pragma mark -
#pragma mark === CURSOR TRACKING

//=====================================================================================
// ** CURSOR TRACKING
//-------------------------------------------------------------------------------------
// GAResizeBar::AdjustCursorSelf
//-------------------------------------------------------------------------------------

void
GAResizeBar::AdjustCursorSelf		(	Point 				inPortPt,
												const EventRecord &/*inMacEvent*/ )
{

	// * Get the resize rect so that we can use it to do the hit testing
	Rect	resizeRect;
	CalcLocalResizeBarRect ( resizeRect );
	PortToLocalPoint ( inPortPt );
	
	// * Check to see if the cursor is in the actual resize bar not just the
	// pane, this will allow the pane to be made larger but still have the
	// tracking occur from the correct location
	if ( ::PtInRect ( inPortPt, &resizeRect ))
	{
		// * Switch the cursor to our resize cursor which is different for
		// each type of resize bar - horizontal or vertical
		CCrsrHandle	theCCrsrH = ::GetCCursor ( IsHorizontal () ? resizeBar_HResizeCursor :
																			resizeBar_VResizeCursor );
		// * Adjust the cursor if it is over the resize bar
		if ( theCCrsrH != nil ) 									
		{
			// * Set the resize cursor.
			::SetCCursor ( theCCrsrH );
			
			// * Inside Mac says this has to be thrown away after being used
			// other wise the next call to GetCCursor will just load another
			// one creating a memory leak
			::DisposeCCursor ( theCCrsrH );
		}
	}
	
}	//	GAResizeBar::AdjustCursorSelf



