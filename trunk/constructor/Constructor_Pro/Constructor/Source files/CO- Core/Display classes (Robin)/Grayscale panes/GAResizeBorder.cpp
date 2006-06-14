// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1996 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< * >>-----------------------------------------

FILE:					GAResizeBorder.cp

CLASSES:				GAResizeBorder

DESCRIPTION:		Implementation file for GAResizeBorder

AUTHOR:				Robin Mair

CREATION DATE :	96.06.28

CHANGE HISTORY :

		97.02.21		rtm	Tweaked AdjustResizeBars to noe use some new test methods that
								have been added to the resize bar for testing the obscurity
								of the first and second panes.
		97.02.20		rtm	Modified the AdjustResizeBars method, the vertical test did not
								correctly account for a set of borders that were setup with
								bindings that could result in the top pane disappear off the
								window.  The test was changed to look at the heights of both
								panes and if either was negative triggering the adjustment.
		96.11.24		rtm	Changed the way the commander to which the resize border manager
								attachment is found, we now have an additional field that is
								used to specify the pane ID for the commander that has the
								attachment attached to it.  This change allows us to more easily
								support allowing the user to add the ressize border attachment
								to any view that is also a commander.
		96.11.23		rtm	Added in support for the zoom view which allows borders
								to be embedded in views that can then have their embedded borders
								zoomed within the view instead of just a window as we previously
								has it working.  The zoom view is simply  view that is also a
								commander and therefore can handle passing commands on to the
								resize border manager attachment.  The changes were applied to
								FinishCreateSelf and ListenToMessage.
		96.08.30		rtm	Fixed bug in SetResizeBorderLayout where handles where being
								disposed that the handle stream was disposing.
		96.08.21		rtm	Removed the flag that was used to control the drawing of the
								border, there are  now a combination of flags that can be
								used from GAFocusBorder.
		96.08.20		rtm	Added code to FinishCreateSelf that handles the registering
								of a border with the border manager if the border is setup to
								be hidden when a zoom occurs.
		96.07.21		rtm	Changed AdjustBarsProportionally method so that a bar is only
								adjusted if it is enabled.
		96.07.20		rtm	Cleanup on the support for retrieving and restoring the data
								for a border that is needed when saving a browser layout.
		96.07.19		rtm	Added support for being able to extract data from the border
								that allows it to be saved off in a resource. This support
								allows both the entire layout for a window and the border
								info to be saved and restored.  It is up to the user to handle
								getting the data to and from the disk.
		96.07.08		rtm	Added suport for altering the resize direction of a border which
								will then correctly ensure that the resize bars and their assoc
								iated borders get correctly rearranged.  This mechanism works by
								simply calling the SetAdjustHorizontally() method, which in
								in turn iterates over the resize bars getting things changed.
		96.07.05		rtm	An entire day spent trying to tweak the handle of the resizing
								of panes when one either becomes onscured [easy part] or is
								smaller than its minimum size. It basically is working but there
								are some cases where it will do the adjusting when you might 
								thinl it shouldn't.
		96.07.02		rtm	Made the experimental version of this class the mainstream thus
								replacing the original version
		96.07.02		rtm	Tweaked the algorithm that handles the layout of the resize bars
								when a pane becomes to small or obscured.
		96.06.28		rtm	Initial creation of file

---------------------------------------<< * >>-----------------------------------------
*/

	// Prefix file for MSVC build
#ifdef WINVER
	#include "ctor.h"
#endif

#include "GAResizeBorder.h"
#include "GAResizeBar.h"
#include	"GAZoomToggle.h"
#include	"GResizeBorderMgr.h"

// * GRAYSCALE HEADERS
#include <UGraphicsUtilities.h>
#include <UGAColorRamp.h>


//=====================================================================================
// CONSTANTS
//=====================================================================================

//=====================================================================================
// DEFINES
//=====================================================================================

#pragma mark === CLASS: GAResizeBorder ===

//=====================================================================================
// ** STREAM CREATION
//-------------------------------------------------------------------------------------
// GAResizeBorder::CreateGAResizeBorderStream	
//-------------------------------------------------------------------------------------
//

GAResizeBorder*	
GAResizeBorder::CreateGAResizeBorderStream ( LStream *inStream )
{

	return ( new GAResizeBorder ( inStream ));
	
}	// GAResizeBorder::CreateGAResizeBorderStream


#pragma mark -
#pragma mark === INITIALIZATION ===

//=====================================================================================
// CLASS:	GAResizeBorder
//=====================================================================================

//=====================================================================================
// ** INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// GAResizeBorder::GAResizeBorder					---<<* CONSTRUCTOR *>>---
//-------------------------------------------------------------------------------------

GAResizeBorder::GAResizeBorder ()
{

	mMinimumSize.width = 0;
	mMinimumSize.height = 0;
	mAdjustBarsOnResize = true;
	mAdjustHorizontally = false;
	mAdjustOption = adjustOption_None;
	mCanZoom = false;
	mZoomWidgetID = PaneIDT_Undefined;
	mHideBorderOnZoom = false;
	mZoomCommanderID = PaneIDT_Undefined;
	
}	//	GAResizeBorder::GAResizeBorder


//-------------------------------------------------------------------------------------
// GAResizeBorder::GAResizeBorder					---<<* COPY CONSTRUCTOR *>>---
//-------------------------------------------------------------------------------------
//

GAResizeBorder::GAResizeBorder (	const GAResizeBorder	&inOriginal )
								:	LGAFocusBorder ( inOriginal )
{

	mMinimumSize = inOriginal.mMinimumSize;
	mAdjustBarsOnResize = inOriginal.mAdjustBarsOnResize;
	mAdjustHorizontally = inOriginal.mAdjustHorizontally;
	mAdjustOption = inOriginal.mAdjustOption;
	mCanZoom = inOriginal.mCanZoom;
	mZoomWidgetID = inOriginal.mZoomWidgetID;
	mHideBorderOnZoom = false;
	mZoomCommanderID = inOriginal.mZoomCommanderID;
	
}	//	GAResizeBorder::GAResizeBorder


//-------------------------------------------------------------------------------------
// GAResizeBorder::GAResizeBorder					---<<* CONSTRUCTOR *>>---
//-------------------------------------------------------------------------------------

GAResizeBorder::GAResizeBorder (	LStream*	inStream ) : LGAFocusBorder ( inStream )
{

	//	* Setup the data members for the view
	Int16 width, height;
	inStream->ReadData ( &width, sizeof (Int16));
	mMinimumSize.width = width;
	inStream->ReadData ( &height, sizeof (Int16));
	mMinimumSize.height = height;
	inStream->ReadData ( &mAdjustBarsOnResize, sizeof (Boolean));
	inStream->ReadData ( &mAdjustHorizontally, sizeof (Boolean));
	Int16	resizeOption;
	inStream->ReadData ( &resizeOption, sizeof (Int16));
	mAdjustOption = (EAdjustOptions)resizeOption;
	inStream->ReadData ( &mCanZoom, sizeof ( Boolean ));
	inStream->ReadData ( &mZoomWidgetID, sizeof ( PaneIDT ));
	inStream->ReadData ( &mHideBorderOnZoom, sizeof ( Boolean ));
	inStream->ReadData ( &mZoomCommanderID, sizeof (PaneIDT));
	
}	//	GAResizeBorder::GAResizeBorder


//-------------------------------------------------------------------------------------
// GAResizeBorder::~GAResizeBorder				---<<* DESTRUCTOR *>>---
//-------------------------------------------------------------------------------------

GAResizeBorder::~GAResizeBorder ()
{
}	//	GAResizeBorder::~GAResizeBorder


//-------------------------------------------------------------------------------------
// GAResizeBorder::FinishCreateSelf
//-------------------------------------------------------------------------------------

void
GAResizeBorder::FinishCreateSelf ()
{

	LGAFocusBorder::FinishCreateSelf ();
	
	// * If requested build up a resize bar list
	if ( WantsBarsAdjusted ())
	{
		// * We need to iterate over each of our sub panes find the resize bars
		// getting them added to our list of resize bars
		LArrayIterator iterator ( mSubPanes, iterate_FromStart );
		LPane*	thePane;
		while ( iterator.Next ( &thePane )) 
		{
			if ( thePane )
			{
				GAResizeBar*	theBar = dynamic_cast<GAResizeBar*>(thePane);
				if ( theBar )
				{
					// * We are only interested in registering bars that are
					// working in the same direction as we are handling
					// pane adjustment, NOTE: these flags are opposite the one
					// in the resize bar indicates the direction of the bar
					// while the one in the border indicates the resize direction
					// yes I know its confusing but you're a big boy!!
					if ( IsAdjustHorizontal () != theBar->IsHorizontal ())
						RegisterResizeBar ( theBar );
				}
			}
		}
	}
	
	// * If the pane can be zoomed then we need to find its zoom widget
	// and get it ourselves setup to listen to it
	if ( CanZoom () && GetZoomWidgetID () != PaneIDT_Undefined )
	{
		GAZoomToggle* zoomToggle = (GAZoomToggle*)FindPaneByID ( GetZoomWidgetID () );
		if ( zoomToggle )
		{
			zoomToggle->AddListener ( this );
				
		}
	}
	
	// * Now we need to register all of the borders that want to be hidden when
	// a border within a window is zoomed
	if ( IsHiddenOnZoom ())
	{
		// * Finally we need to make sure we get the border registered with the attachment
		// that manages the zooming of panes so that the border can be shown and hidden
		// as needed when a border is zoomed.  For this we use the mZoomCommander field
		// which tells us which pane the attachment has been added to, if the ID is not
		// specified then we assume it has been added to the window, this means we are
		// backwards compatible with layouts that used this class before this field was
		// added
		LWindow*	theWindow = GetContainingWindow ();
		if ( mZoomCommanderID == PaneIDT_Undefined )
		{
			if ( theWindow )
				theWindow->ProcessCommand ( cmd_RegisterBorder, this );
		}
		else
		{
			LCommander*	theZoomCommander = dynamic_cast<LCommander*>(theWindow->FindPaneByID ( mZoomCommanderID ));
			if ( theZoomCommander )
				theZoomCommander->ProcessCommand ( cmd_RegisterBorder, this );
		}
	}
	
}	//	GAResizeBorder::FinishCreateSelf


#pragma mark -
#pragma mark === ACCESSORS ===

//=====================================================================================
// ** ACCESSORS
//-------------------------------------------------------------------------------------
// GAResizeBorder::GetSubBorders
//-------------------------------------------------------------------------------------

void
GAResizeBorder::GetSubBorders ( LList &outSubBorders )
{
	
	// * This method walks down through the borders hierarchy returning
	// a list containing all of the resize borders that are in the hierarchy
	LListIterator iterator ( mSubPanes, iterate_FromStart );
	LPane*	thePane;
	while ( iterator.Next ( &thePane )) 
	{
		if ( thePane )
		{
			GAResizeBorder*	theBorder = dynamic_cast<GAResizeBorder*>(thePane);
			if ( theBorder )
			{
				// * Add the to our temporary list
				outSubBorders.InsertItemsAt ( 1, 						//	Number of items
														arrayIndex_Last,		//	Position in array
														&theBorder );			//	Item being added
														
				// * Now walk down the hierarchy adding any other borders
				// that can be found
				theBorder->GetSubBorders ( outSubBorders );
			}
		}
	}
	
}	// GAResizeBorder::GetSubBorders

//-------------------------------------------------------------------------------------
// GAResizeBorder::CountAdjustingBorders
//-------------------------------------------------------------------------------------

Int16
GAResizeBorder::CountAdjustingBorders ()
{

	// * Walk the borders view hierarchy counting the number of resize
	// borders that are set to handle the adjusting of their resize bars
	Int16	counter = 0;
	LListIterator iterator ( mSubPanes, iterate_FromStart );
	LPane*	thePane;
	while ( iterator.Next ( &thePane )) 
	{
		if ( thePane )
		{
			GAResizeBorder*	theBorder = dynamic_cast<GAResizeBorder*>(thePane);
			if ( theBorder && theBorder->WantsBarsAdjusted ())
			{
				// * Walk down the hierarchy getting the count
				counter = theBorder->CountAdjustingBorders ();
				
				// * Make sure we add this border to the count
				counter++;
			}
		}
	}
	
	// * Return the count of the caller
	return counter;
	
}	//	GAResizeBorder::CountAdjustingBorders


//-------------------------------------------------------------------------------------
// GAResizeBorder::GetContainingWindow
//-------------------------------------------------------------------------------------

LWindow*
GAResizeBorder::GetContainingWindow ()
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
		
}	// GAResizeBorder::GetContainingWindow


//-------------------------------------------------------------------------------------
// GAResizeBorder::SetMinimumSize
//-------------------------------------------------------------------------------------

void
GAResizeBorder::SetMinimumSize (	SDimension16	inMinSize )
{

	// * Only change things if we have a new value
	if ( mMinimumSize.width != inMinSize.width ||
				mMinimumSize.height != inMinSize.height )
	{
		mMinimumSize = inMinSize;
	}
	
}	//	GAResizeBorder::SetMinimumSize


//-------------------------------------------------------------------------------------
// GAResizeBorder::SetAdjustHorizontally
//-------------------------------------------------------------------------------------

void
GAResizeBorder::SetAdjustHorizontally	(	Boolean	inAdjustHorizontal )
{
	
	// * We only change things if a change has actually occurred
	if ( mAdjustHorizontally != inAdjustHorizontal )
	{
		mAdjustHorizontally = inAdjustHorizontal;
		
		// * When the resize orientation of a border is changed
		// it means that everything inside the border needs to be
		// rearranged from its previous orientation, this primarily
		// involves getting the resize bars changed, we only do this
		// if the border handles bar adjustment
		if ( WantsBarsAdjusted ())
			RearrangeResizeBars ( inAdjustHorizontal );
	}
	
	
}	//	GAResizeBorder::SetAdjustHorizontally


#pragma mark -
#pragma mark === RESIZE BAR MANAGEMENT ===

//=====================================================================================
// ** RESIZE BAR MANAGEMENT
//-------------------------------------------------------------------------------------
// GAResizeBorder::RegisterResizeBar
//-------------------------------------------------------------------------------------

void
GAResizeBorder::RegisterResizeBar (	GAResizeBar*	inBarToRegister )
{
	
	// * If we already have a page title array then we simply add the
	// new title to the list
	mResizeBars.InsertItemsAt ( 1, 						//	Number of items
										arrayIndex_Last,		//	Position in array
										&inBarToRegister );	//	Item being added
	
}	//	GAResizeBorder::RegisterResizeBar


//-------------------------------------------------------------------------------------
// GAResizeBorder::RemoveResizeBar
//-------------------------------------------------------------------------------------

void
GAResizeBorder::RemoveResizeBar (	GAResizeBar*	inBarToRemove )
{
	
	// * Get the item removed from the array
	mResizeBars.Remove ( &inBarToRemove );
													
}	//	GAResizeBorder::RemoveResizeBar


//-------------------------------------------------------------------------------------
// GAResizeBorder::RemoveAllResizeBars
//-------------------------------------------------------------------------------------

void
GAResizeBorder::RemoveAllResizeBars ()
{

	LArrayIterator iterator ( mResizeBars, iterate_FromEnd );
	GAResizeBar*	theBar;
	while ( iterator.Previous ( &theBar )) 
	{
		mResizeBars.Remove ( theBar );
	}
	
}	//	GAResizeBorder::RemoveAllResizeBars


//-------------------------------------------------------------------------------------
// GAResizeBorder::RearrangeResizeBars
//-------------------------------------------------------------------------------------

void
GAResizeBorder::RearrangeResizeBars (	Boolean	inHorizontal )
{
	
	// * We start by figuring out where the bars will be positioned
	// within their superview, which is us
	Int16	barCount = mResizeBars.GetCount () + 1;
	Int16	barSpacing = inHorizontal ? mFrameSize.width / barCount :
											mFrameSize.height / barCount;
	
	// * Iterate over all of our bars and change their orientation
	LArrayIterator iterator ( mResizeBars, iterate_FromStart );
	GAResizeBar*	theBar;
	Int16				newPosition = barSpacing;
	Int16				currBar = 1;
	while ( iterator.Next ( &theBar )) 
	{
		if ( theBar )
		{
			// * Get the bar to change its orientation, this will result
			// in the bar repositioning its associated borders and then
			// adjusting itself
			theBar->SetOrientation ( 	!inHorizontal, 			// Bars use flag in opposite way
												newPosition, 				//	New position for bar
												mResizeBars.GetCount (),//	Number of bars
												currBar );					//	Current bar being changed
			
			// * Increment the bar position
			newPosition += barSpacing;
			
			// * Increment the bar count
			currBar++;
		}
	}

}	//	GAResizeBorder::RearrangeResizeBars


#pragma mark -
#pragma mark === RESIZE BORDER INFO ===

//=====================================================================================
// ** RESIZE BORDER INFO
//-------------------------------------------------------------------------------------
// GAResizeBorder::GetResizeBorderInfo
//-------------------------------------------------------------------------------------

Handle
GAResizeBorder::GetResizeBorderInfo	()
{

	// * Setup the handle stream that we will be using to hold the data
	Handle	borderInfoH = ::NewHandle (0);
	ThrowIfNil_ ( borderInfoH );
	LHandleStream	borderStream ( borderInfoH );
	
	// * Create a handle to an SResizeBarInfo struct and fill it with
	// the data we care about from the resize bar
	PaneIDT	paneID = GetPaneID ();
	Boolean	orientation = IsAdjustHorizontal ();
	Int16		barCount = mResizeBars.GetCount ();
	borderStream.WriteData ( &paneID, sizeof ( PaneIDT ));
	borderStream.WriteData ( &orientation, sizeof ( Boolean ));
	borderStream.WriteData ( &barCount, sizeof ( Int16 ));
	
	// * Iterate over all of our bars getting their info and adding it to the
	// borders data stream
	LArrayIterator iterator ( mResizeBars, iterate_FromStart );
	GAResizeBar*	theBar;
	while ( iterator.Next ( &theBar )) 
	{
		if ( theBar )
		{
			Handle	resizeBarH;
			
			// * Get the bar too provide us with the information that it wants saved
			resizeBarH = theBar->GetResizeBarInfo ();
			ThrowIfNil_ ( resizeBarH );
			
			// * Write the handle into the stream
			borderStream.WriteHandle ( resizeBarH );
			
			// * Make sure we get rid of the handle
			::DisposeHandle ( resizeBarH );
		}
	}

	
	// * Return the handle that was being popuplated by the stream
	// the recipient will be responsible for disposing of the handle
	return borderStream.DetachDataHandle ();
	
}	//	GAResizeBorder::GetResizeBorderInfo


//-------------------------------------------------------------------------------------
// GAResizeBorder::SetResizeBorderInfo
//-------------------------------------------------------------------------------------

void
GAResizeBorder::SetResizeBorderInfo	(	Handle	inResizeBorderInfoH )
{

	// * Setup the handle stream that we will be using to hold the data
	ThrowIfNil_ ( inResizeBorderInfoH );
	LHandleStream	borderStream ( inResizeBorderInfoH );
	
	// * Extract some of the data from the stream
	PaneIDT	paneID;
	Boolean	orientation;
	Int16		barCount;
	borderStream.ReadData ( &paneID, sizeof ( PaneIDT ));
	borderStream.ReadData ( &orientation, sizeof ( Boolean ));
	borderStream.ReadData ( &barCount, sizeof ( Int16 ));
	
	// * Reset the orientation if needed
	if ( IsAdjustHorizontal () != orientation )
		SetAdjustHorizontally ( orientation );
		
	// * If the border's ID matches that of the data we were handed and 
	// there is at least one resize bar then we proceed
	if ( paneID == GetPaneID () && barCount == mResizeBars.GetCount () )
	{
		// * Iterate over all of our bars getting their info and then get
		// the resize bars positioned as specified
		LArrayIterator iterator ( mResizeBars, iterate_FromStart );
		GAResizeBar*	theBar;
		Handle			resizeBarH;
		while ( iterator.Next ( &theBar )) 
		{
			if ( theBar )
			{
				// * Get the bar too provide us with the information that it wants saved
				borderStream.ReadHandle ( resizeBarH );
				ThrowIfNil_ ( resizeBarH );
				
				// * Get the resize bar setup from the saved info, the handle will
				// be disposed by the resize bar
				theBar->SetResizeBarInfo ( resizeBarH );
			}
		}
		
	}
	
}	//	GAResizeBorder::SetResizeBorderInfo


//-------------------------------------------------------------------------------------
// GAResizeBorder::GetResizeBorderLayout
//-------------------------------------------------------------------------------------

Handle
GAResizeBorder::GetResizeBorderLayout	()
{

	// * Setup the handle stream that we will be using to hold the data
	Handle	borderlayoutH = ::NewHandle (0);
	ThrowIfNil_ ( borderlayoutH );
	LHandleStream	layoutStream ( borderlayoutH );
	
	// * Start adding data to the stream by setting the ID of this pane
	// and the count of borders
	PaneIDT	paneID = GetPaneID ();
	Int16		borderCount = CountAdjustingBorders ();
	layoutStream.WriteData ( &paneID, sizeof ( PaneIDT ));
	layoutStream.WriteData ( &borderCount, sizeof ( Int16 ));
	
	// * Now we need to get a list built of all the the borders that
	// can be adjusted within our hierarchy
	Handle	resizeBorderInfoH;
	LList	borderList;
	GetSubBorders ( borderList );
	
	// * First we need to get ourselves added to the layout, we only do this if
	// we handle adjusting of any resize bars we might have
	if ( WantsBarsAdjusted ())
	{
		// * Get the border too provide us with the information that it wants saved
		resizeBorderInfoH = GetResizeBorderInfo ();
		ThrowIfNil_ ( resizeBorderInfoH );
		
		// * Write the handle into the stream
		layoutStream.WriteHandle ( resizeBorderInfoH );

		// * Dispose of the handle
		::DisposeHandle ( resizeBorderInfoH );
	
	}
	
	LArrayIterator iterator ( borderList, iterate_FromStart );
	GAResizeBorder*	theBorder;
	while ( iterator.Next ( &theBorder )) 
	{
		if ( theBorder && theBorder->WantsBarsAdjusted ())
		{
			// * Get the border too provide us with the information that it wants saved
			resizeBorderInfoH = theBorder->GetResizeBorderInfo ();
			ThrowIfNil_ ( resizeBorderInfoH );
			
			// * Write the handle into the stream
			layoutStream.WriteHandle ( resizeBorderInfoH );

			// * Dispose of the handle
			::DisposeHandle ( resizeBorderInfoH );
		}
	}

	// * Return the handle that was being popuplated by the stream
	// the recipient will be responsible for disposing of the handle
	return layoutStream.DetachDataHandle ();
	
}	//	GAResizeBorder::GetResizeBorderLayout


//-------------------------------------------------------------------------------------
// GAResizeBorder::SetResizeBorderLayout
//-------------------------------------------------------------------------------------

void
GAResizeBorder::SetResizeBorderLayout	( 	Handle	inBorderLayoutH )
{

	// * Setup the handle stream that we will be using to hold the data
	ThrowIfNil_ ( inBorderLayoutH );
	LHandleStream	borderLayoutStream ( inBorderLayoutH );
	
	// * Extract some of the data from the stream
	PaneIDT	paneID;
	Int16		borderCount;
	borderLayoutStream.ReadData ( &paneID, sizeof ( PaneIDT ));
	borderLayoutStream.ReadData ( &borderCount, sizeof ( Int16 ));
	
	// * If the border's ID matches that of the data we were handed and 
	// there is at least one resize bar then we proceed
	if ( paneID == GetPaneID () && CountAdjustingBorders () == borderCount )
	{
		// * Now we need to get a list built of all the the borders that
		// can be adjusted within our hierarchy
		Handle	resizeBorderInfoH;
		LList	borderList;
		GetSubBorders ( borderList );
	
		// * First we need to get ourselves setup from the layout, we only do this if
		// we handle adjusting of any resize bars we might have
		if ( WantsBarsAdjusted ())
		{
			// * Get the border too provide us with the information that it wants saved
			borderLayoutStream.ReadHandle ( resizeBorderInfoH );
			ThrowIfNil_ ( resizeBorderInfoH );
			
			// * Now get our data setup
			SetResizeBorderInfo ( resizeBorderInfoH );
		}
	
		// * Now extract the data for each of the adjustable borders in the list
		LArrayIterator iterator ( borderList, iterate_FromStart );
		GAResizeBorder*	theBorder;
		while ( iterator.Next ( &theBorder )) 
		{
			if ( theBorder && theBorder->WantsBarsAdjusted ())
			{
				// * Get the border too provide us with the information that it wants saved
				borderLayoutStream.ReadHandle ( resizeBorderInfoH );
				ThrowIfNil_ ( resizeBorderInfoH );
				
				// * Now get our data setup
				theBorder->SetResizeBorderInfo ( resizeBorderInfoH );
			}
		}
	}

}	//	GAResizeBorder::GetResizeBorderLayout


#pragma mark -
#pragma mark === RESIZING ===

//=====================================================================================
// ** RESIZING
//-------------------------------------------------------------------------------------
// GAResizeBorder::ResizeFrameBy
//-------------------------------------------------------------------------------------

void
GAResizeBorder::ResizeFrameBy	( 	Int16		inWidthDelta,
												Int16		inHeightDelta,
												Boolean	inRefresh )
{
	// * Start by first saving off the size before the resize takes place
	Rect	oldBounds;
	CalcPortFrameRect ( oldBounds );
	
	// * Now call our superclass to get the resize to happen
	LGAFocusBorder::ResizeFrameBy ( inWidthDelta, inHeightDelta, inRefresh );
	
	// * Next call our function that is going to make sure that we
	// don't accidentally obscure a border when the view is resized
	if ( WantsBarsAdjusted ())
	{
		
		// * Before we call the routine that handles the adjusting of the
		// resize bars we do an additional check to see if it is even 
		// needed, if the frame did not grow in our direction we don't
		// care about the change and therefore ignore it
		if ( IsAdjustHorizontal () && inWidthDelta != 0 )
			AdjustResizeBars ( oldBounds );
		else if ( !IsAdjustHorizontal () && inHeightDelta != 0 )
			AdjustResizeBars ( oldBounds );
			
	}
	
}	//	GAResizeBorder::ResizeFrameBy


//-------------------------------------------------------------------------------------
// GAResizeBorder::AdjustResizeBars
//-------------------------------------------------------------------------------------
//
//	This version always does a proportional spreading of the resize bars contrained in
//		the border

void
GAResizeBorder::AdjustResizeBars	(	Rect	/*inOldBounds*/	)
{
	
	// * If proportional adjustment is always on then we merely call the
	// method that handles it, otherwise if we are going to make sure that
	// we don't allow any pane to be obscured then we handle that case.  The
	// second case uses the proportional layout algorithm but only invokes it
	// if a pane becomes either obscured or were it would end up smaller than
	// its minimum size
	if ( mAdjustOption == adjustOption_Proportional )
	{
		AdjustBarsProportionally ();
	}
	else if ( mAdjustOption == adjustOption_KeepVisible )
	{
		// * Get a reference to the last bar so that we can check
		// its second pane to see if its size has gone negative
		// if it has then we need to make sure that we adjust the
		// bars so that all the panes are visible
		if ( mResizeBars.GetCount() != 0 )
		{
			GAResizeBar*	theBar;
			mResizeBars.FetchItemAt ( mResizeBars.GetCount(), &theBar );
			if ( theBar )
			{
				// * ADJUST HORIZONTALLY
				if ( IsAdjustHorizontal ())
				{
					if ( theBar->IsFirstPaneWidthObscured () || 
								theBar->IsSecondPaneWidthObscured ())
					{
						AdjustBarsProportionally ();
					}
				}
				else	// * ADJUST VERTICALLY
				{
					if ( theBar->IsFirstPaneHeightObscured () || 
								theBar->IsSecondPaneHeightObscured ())
					{
						AdjustBarsProportionally ();
					}
				}
			}
		}
	}
	
}	//	GAResizeBorder::AdjustResizeBars



//-------------------------------------------------------------------------------------
// GAResizeBorder::AdjustBarsProportionally
//-------------------------------------------------------------------------------------
//

void
GAResizeBorder::AdjustBarsProportionally	()
{

	Boolean			horizontal = !IsAdjustHorizontal ();
	Int16				newLoc;
	SDimension16	containerSize;
	GetFrameSize ( containerSize );
	
	// * Figure out the available space for positioning the bars
	Int16			availableSpace;
	availableSpace = horizontal ? containerSize.height : containerSize.width;

	// * What is the position for the first bar
	newLoc = availableSpace / (mResizeBars.GetCount () + 1);

	// * Iterate over all of the bars getting them relocated as needed
	LArrayIterator iterator ( GetResizeBars (), iterate_FromStart );
	GAResizeBar*	theBar;
	Int16				nextLoc = newLoc;
	while ( iterator.Next ( &theBar )) 
	{
	
		// * We only adjust a bar if it is enabled, this feature can be used
		// in some cases to control what happens when a window is resized
		if ( theBar &&  theBar->GetEnabledState () != triState_Off )
		{
			// * Figure out where the bar is currently located and then what the delta
			// is to get it moved to the new location
			SPoint32		frameLoc;
			theBar->GetFrameLocation ( frameLoc );
			
			// * Now we can go ahead and move the resize bar which
			// will trigger the adjustment of the positions of the
			// associated views
			Int16 delta;
			if ( horizontal )
			{
				delta = nextLoc - frameLoc.v;
				if ( delta != frameLoc.v )
					theBar->MoveBy ( 0, delta, true );
			}
			else
			{
				delta = nextLoc - frameLoc.h;
				if ( delta != frameLoc.h )
					theBar->MoveBy ( delta, 0, true );
			}
		}
		
		// * Increment the next location by the new location
		nextLoc += newLoc;
	}
	
}	//	GAResizeBorder::AdjustBarsProportionally



#pragma mark -
#pragma mark === LISTENING ===

//=====================================================================================
// ** LISTENING
//-------------------------------------------------------------------------------------
// GAResizeBorder::ListenToMessage														
//-------------------------------------------------------------------------------------

void		
GAResizeBorder::ListenToMessage (	MessageT inMessage, 
												void*		ioParam	)
{
	
	// * Handle zooming of a border
	if ( inMessage == cmd_ZoomBorder )
	{
		GAZoomToggle*	toggle = (GAZoomToggle*)ioParam;
		
		// * Find the zoom box
		GAZoomToggle* zoomToggle = (GAZoomToggle*)FindPaneByID ( GetZoomWidgetID () );
		if ( zoomToggle && zoomToggle == toggle )
		{
			// * Handle the zooming of the border by calling process command on
			// the commander to which the resize border manager has been added
			LWindow*	theWindow = GetContainingWindow ();
			if ( mZoomCommanderID == PaneIDT_Undefined )
			{
				if ( theWindow )
					theWindow->ProcessCommand ( cmd_ZoomBorder, this );
			}
			else
			{
				LCommander*	theZoomCommander = dynamic_cast<LCommander*>(theWindow->FindPaneByID ( mZoomCommanderID ));
				if ( theZoomCommander )
					theZoomCommander->ProcessCommand ( cmd_ZoomBorder, this );
			}
		}
	}
	
}	//	GAResizeBorder::ListenToMessage



