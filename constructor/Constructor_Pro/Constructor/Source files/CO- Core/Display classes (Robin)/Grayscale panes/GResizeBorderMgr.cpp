// Copyright �2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright� 1995 Metrowerks Corp. All rights reserved

---------------------------------------<< � >>-----------------------------------------

FILE:					GResizeBorderMgr.cp

CLASSES:				GResizeBorderMgr

DESCRIPTION:		Implementation file for GResizeBorderMgr

AUTHOR:				Robin Mair

CREATION DATE :	96.08.19

CHANGE HISTORY :

		96.11.24		rtm	The GAResizeBorder and GAResizeBar classes have now been
								changed so that they have an additional field appended to the
								end that is used to specify the commander to which the resize
								border manager attachment has been attached.
								NOTE:  IF THE FIELD IS LEFT UNSPECIFIED, THE DEFAULT, IT WILL
								ALWAYS LOOK AT THE WINDOW.  THIS MAKES THIS BACKWARD COMPATIBLE
								WITH ANY EXISTING LAYOUT WHICH UP TILL NOW HAVE ALL HAD THE
								ATTACHMENT ADDED TO THE WINDOW.
		96.11.23		rtm	Changed the manager so that it now can be attached to a view
								it does not have to be attached to a window.  This is so we can
								use this within windows by attaching it to a view, for cases
								where we do not want to zoom to fill an entire window. NOTE: a 
								specific view has been created for this purpose that is a
								commander as well so that the resize border manager attachment
								can be attached to it.  I'm considering creating an additional
								field for the CPPbs for the resize border and the resize bar
								that specifies which commander is going to have the border
								manager associated with it, this will allow the user to use 
								any view that is a commander to host zooming borders.
		96.11.01		rtm	Added in some fixes made by the IDE team, these were mainly
								all in DoZoomBorder and involved refreshing and setup the tab
								selection.
		96.08.21		rtm	Added a tweak to the zooming method that correctly takes into
								account borders that do not have their bottom right corners 
								notched, in that case the border is only zoomed to the top of
								the grow box so that it does not become obscured.
		96.08.20		rtm	Implemented the zooming support which now correctly handles all 
								the cases where the window can undergo change while a border is
								zoomed and still correctly maintain the appropriate state for
								the border.
		96.08.19		rtm	Initial creation of file

								
---------------------------------------<< � >>-----------------------------------------
*/

#include "GResizeBorderMgr.h"

#include "GAResizeBorder.h"
#include "GAResizeBar.h"
#include	"GAZoomToggle.h"

// � POWERPLANT HEADERS
#include <LAttachment.h>

//=====================================================================================
// CONSTANTS
//=====================================================================================

//=====================================================================================
// DEFINES
//=====================================================================================

#pragma mark === CLASS: GResizeBorderMgr ===

//=====================================================================================
// CLASS:	GResizeBorderMgr
//=====================================================================================

//=====================================================================================
// �� STREAM CREATION
//-------------------------------------------------------------------------------------
// GResizeBorderMgr::CreateGResizeBorderMgrStream	
//-------------------------------------------------------------------------------------
//

GResizeBorderMgr*	
GResizeBorderMgr::CreateGResizeBorderMgrStream ( LStream *inStream )
{

	return ( new GResizeBorderMgr ( inStream ));
	
}	// GResizeBorderMgr::CreateGResizeBorderMgrStream


#pragma mark -
#pragma mark === INITIALIZATION ===

//=====================================================================================
// �� INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// GResizeBorderMgr::GResizeBorderMgr					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------

GResizeBorderMgr::GResizeBorderMgr ()
{

	mHostView = nil;
	mZoomedBorder = nil;
	mSurrogateBorder = nil;
	
}	//	GResizeBorderMgr::GResizeBorderMgr



//-------------------------------------------------------------------------------------
// GResizeBorderMgr::GResizeBorderMgr					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------

GResizeBorderMgr::GResizeBorderMgr (	LStream*	inStream )
											: LAttachment ( inStream )
{

	mZoomedBorder = nil;
	mSurrogateBorder = nil;

	// � Get a reference setup to the owning view
	mHostView = dynamic_cast<LView*>(mOwnerHost);
	ThrowIfNil_ ( mHostView );

	// � Make sure we are setup to always pass message on through to host
	SetExecuteHost ( true );
	
	// � We want to be able to handle all messages
	SetMessage ( msg_AnyMessage );
	
}	//	GResizeBorderMgr::GResizeBorderMgr



//-------------------------------------------------------------------------------------
// GResizeBorderMgr::~GResizeBorderMgr					---<<� DESTRUCTOR �>>---
//-------------------------------------------------------------------------------------

GResizeBorderMgr::~GResizeBorderMgr ()
{
}	//	GResizeBorderMgr::~GResizeBorderMgr



#pragma mark -
#pragma mark === MESSAGE HANDLING ===

//=====================================================================================
// �� MESSAGE HANDLING
//-------------------------------------------------------------------------------------
// GResizeBorderMgr::ExecuteSelf
//-------------------------------------------------------------------------------------
//

void
GResizeBorderMgr::ExecuteSelf (	MessageT	inMessage,
											void		*ioParam )
{

	// � Depending on the message we convert the ioParam to the appropriate
	// class for the operation
	GAResizeBorder*	theBorder;
	GAResizeBar*		theBar;
	if ( inMessage == cmd_RegisterResizeBar )
		theBar = (GAResizeBar*) ioParam;
	else
		theBorder = (GAResizeBorder*) ioParam;
		
	// � Handle the operations performed by the attachment which are all
	// instigated by calling PerformCommand which is then caught here
	switch ( inMessage )  
	{
		// � Adding a border to the manager
		case cmd_RegisterBorder:
			if ( theBorder )
				RegisterBorder ( theBorder );
		break;

		//	� Adding a resize bar to the manager
		case cmd_RegisterResizeBar:
			if ( theBar )
				RegisterResizeBar ( theBar );
		break;

		// � Zooming a border
		case cmd_ZoomBorder:
			if ( theBorder )
			{
				// � Find the zoom box
				GAZoomToggle* zoomToggle = (GAZoomToggle*)theBorder->FindPaneByID ( theBorder->GetZoomWidgetID () );
				if ( zoomToggle )
				{
					Int16	value = zoomToggle->GetValue ();
					HandleZoomBorder ( theBorder, value == 1 ? true : false );
				}
			}
		break;

	}

}	//	GResizeBorderMgr::ExecuteSelf



#pragma mark -
#pragma mark === BORDER MANAGEMENT ===

//=====================================================================================
// �� BORDER MANAGEMENT
//-------------------------------------------------------------------------------------
// GResizeBorderMgr::RegisterBorder	
//-------------------------------------------------------------------------------------
//

void
GResizeBorderMgr::RegisterBorder (	GAResizeBorder*	inBorder )
{
	
	// � Add the border to the list of borders that we are managing
	mBorders.InsertItemsAt ( 	1, 						//	Number of items
										arrayIndex_Last,		//	Position in array
										&inBorder );			//	Item being added
	
}	// GResizeBorderMgr::RegisterBorder



//-------------------------------------------------------------------------------------
// GResizeBorderMgr::RemoveBorder	
//-------------------------------------------------------------------------------------
//

void
GResizeBorderMgr::RemoveBorder (	GAResizeBorder*	inBorder )
{
	
	// � Get the border removed from our list
	mBorders.Remove ( &inBorder );
													
}	// GResizeBorderMgr::RemoveBorder



//-------------------------------------------------------------------------------------
// GResizeBorderMgr::RegisterResizeBar	
//-------------------------------------------------------------------------------------
//

void
GResizeBorderMgr::RegisterResizeBar (	GAResizeBar*	inBar )
{
	
	// � Add the border to the list of borders that we are managing
	mResizeBars.InsertItemsAt ( 	1, 						//	Number of items
											arrayIndex_Last,		//	Position in array
											&inBar );				//	Item being added
	
}	// GResizeBorderMgr::RegisterResizeBar



//-------------------------------------------------------------------------------------
// GResizeBorderMgr::HandleZoomBorder	
//-------------------------------------------------------------------------------------
//

void
GResizeBorderMgr::HandleZoomBorder (	GAResizeBorder*	inBorder,
											Boolean				inZoom )
{

	// � Now get the actual border zoomed so that it fills the view
	DoZoomBorder ( inBorder, inZoom );
	
	// � Iterate over all of the registered borders hide all
	// of them other than the one that was zoomed
	LListIterator iterator ( mBorders, iterate_FromStart );
	GAResizeBorder*	theBorder;
	while ( iterator.Next ( &theBorder )) 
	{
		if ( theBorder && theBorder != inBorder )
		{
			if ( inZoom )
				theBorder->Hide ();
			else
				theBorder->Show ();
		}
	}

	// � Iterate over all of the registered resize bars hide or show all
	// of them as needed
	LListIterator barIterator ( mResizeBars, iterate_FromStart );
	GAResizeBar*	theBar;
	while ( barIterator.Next ( &theBar )) 
	{
		if ( theBar )
		{
			if ( inZoom )
				theBar->Hide ();
			else
				theBar->Show ();
				
			// � We also reset the bar references at the same time 
			theBar->ResetPaneReferences ();
		}
	}

}	// GResizeBorderMgr::HandleZoomBorder



//-------------------------------------------------------------------------------------
// GResizeBorderMgr::DoZoomBorder	
//-------------------------------------------------------------------------------------
//

void
GResizeBorderMgr::DoZoomBorder (	GAResizeBorder*	inBorder,
											Boolean				inZoom )
{

	// � Setup some IDs for the surrogate border and pane, also
	// get a reference to the inset pane
	PaneIDT	surroID = 'surr';
	PaneIDT	surroBorderID = 'surb';
	LPane*	insetSubPane = inBorder->GetInsetSubPane ();
	
	// � Zoom the border to fill the entire view
	if ( inZoom )
	{
		// � The first thing we need to do is make a copy of the border that
		// will be left in place in the view hierarchy to maintain the borders state
		mSurrogateBorder = new GAResizeBorder ( *inBorder );
		if ( mSurrogateBorder )
		{
			// � We change the borders pane ID so that we don't interfere with the
			// layout in the background
			inBorder->SetPaneID ( surroBorderID );
			
			// � Make sure we get the superview for the surrogate setup
			LView*	super = inBorder->GetSuperView ();
			if ( super )
				mSurrogateBorder->PutInside ( super );
			
			// � We also need to create a duplicate of the inset pane in the border
			// so that we can correctly restore its position and size
			if ( insetSubPane )
			{
				LPane*	surroInsetPane = new LPane ( *insetSubPane );
				if ( surroInsetPane )
				{
					// � Give it an ID so that we can find it later
					surroInsetPane->SetPaneID ( surroID );
					
					// � Now add the pane into the surrogate border
					surroInsetPane->PutInside ( mSurrogateBorder );
				}
			}
			
			// � Now get the superview for the border altered
			inBorder->PutInside ( mHostView );
			
			// � Setup the borders new size and location such that it occupies the
			// entire view
			SDimension16	viewSize;
			mHostView->GetFrameSize ( viewSize );
			
			// � Change the location
			inBorder->PlaceInSuperFrameAt ( 1, 1, false );
			
			// � Change the size
			inBorder->ResizeFrameTo ( viewSize.width - 2, viewSize.height - 2, true );

			// � We position the border such that it is optimized for the appearance we
			// are interested in which in our case is a 3 pixel border, we manipulate
			// the inset sub pane to achieve the desired appearance
			// NOTE: we will have to do some additional tweaking here if we are running
			// under Aaron, MacOS 8, or Harmony
			if ( insetSubPane )
			{
				insetSubPane->PlaceInSuperFrameAt ( 4, 4, false );
				
				// � If the border is notching the inset frame then we can set the size 
				// to the full view width otherwise we need to set up the zoom so that
				// the inset pane is up from the bottom of the view so that it does not
				// cover the grow box
				if ( inBorder->NotchInsetFrame ())
				{
					insetSubPane->ResizeFrameTo ( viewSize.width - 10, 
															viewSize.height - 10, 
															true );
				}
				else
				{
					insetSubPane->ResizeFrameTo ( viewSize.width - 10, 
															viewSize.height - 25, 
															true );
				}
			}	
		
			// � Make sure that we turn focusing off as there is no reason to have
			// it on when we are zoomed
			inBorder->SetCanFocus ( false );
			
			// � We also need to make sure that the border is setup to notch around
			// the grow box even if that is not its normal behavior
			inBorder->SetNotchBorderFace ( true );
			
			// � We also need to ensure that the bindings are setup to respond to the
			// resizing of the view, which by default is all on
			SBooleanRect	newBindings = { true, true, true, true };
			inBorder->SetFrameBinding ( newBindings );
			
			// � Make sure we hide the surrogate border
			mSurrogateBorder->Hide ();
			
			// � If we can handle tab selection then set it up now
			if ( inBorder->CanTabSelect ())
				inBorder->DoTabSelect ();
		}
	}
	else	// � Collapse the border back into its original location in the view
	{
		// � We can only proceed if we have a surrogate border otherwise
		// there is information missing that is needed to get our border
		// bavck into the correct state
		if ( mSurrogateBorder )
		{
			// � Get the border refreshed before we start
			inBorder->Refresh ();
			
			// � Restore the border's pane ID
			inBorder->SetPaneID ( mSurrogateBorder->GetPaneID ());
			
			// � Make sure we get the superview for the surrogate setup
			LView*	super = mSurrogateBorder->GetSuperView ();
			if ( super )
				inBorder->PutInside ( super );
			
			// � Setup the location of the border by getting the surrogates location
			// converting it to local coordinates and then setting it
			Rect	surroPortRect;
			mSurrogateBorder->CalcPortFrameRect ( surroPortRect );
			super->PortToLocalPoint ( topLeft ( surroPortRect ));
			inBorder->PlaceInSuperFrameAt ( 	surroPortRect.left, 
														surroPortRect.top, 
														false );
			
			// � Setup the borders new size and location such that it occupies the
			// entire view
			SDimension16	surrogateSize;
			mSurrogateBorder->GetFrameSize ( surrogateSize );
			inBorder->ResizeFrameTo ( surrogateSize.width, surrogateSize.height, false );

			// � Get the surrogate pane that we use to represent the inset
			// sub pane in the border
			if ( insetSubPane )
			{
				LPane*	surroPane = mSurrogateBorder->FindPaneByID ( surroID );
				if ( surroPane )
				{
					Rect	portRect;
					surroPane->CalcPortFrameRect ( portRect );
					inBorder->PortToLocalPoint ( topLeft ( portRect ));
					
					// � Tweak the suub pane to its original location
					insetSubPane->PlaceInSuperFrameAt ( portRect.left, 
																	portRect.top, 
																	false );
					
					// � Tweak the inset pane to its original size
					surroPane->GetFrameSize ( surrogateSize );
					insetSubPane->ResizeFrameTo ( surrogateSize.width, 
															surrogateSize.height, 
															false );

					// � Now refresh the border
					inBorder->Refresh ();
					
					// � Get rid of the surrogate pane
					surroPane->PutInside ( nil );
					delete surroPane;
				}
			}
			
			// � Restore the ability to focus
			inBorder->SetCanFocus ( mSurrogateBorder->CanFocus () );
			
			// � Restore the original state for notching the face
			inBorder->SetNotchBorderFace ( mSurrogateBorder->NotchBorderFace () );
			
			// � Restore the original bindings
			SBooleanRect	oldBindings;
			mSurrogateBorder->GetFrameBinding ( oldBindings );
			inBorder->SetFrameBinding ( oldBindings );
			
			// � Take the surrogate border out of its superview
			mSurrogateBorder->PutInside ( nil );
			
			// � Now delete the surrogate border
			delete mSurrogateBorder;
			
		}
	}
	
	
}	// GResizeBorderMgr::DoZoomBorder


