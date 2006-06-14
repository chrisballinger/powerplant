// Copyright й2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyrightй 1996 Metrowerks Corp. All rights reserved

---------------------------------------<< е >>-----------------------------------------

FILE:					LGABackground.cp

CLASSES:				LGABackground

DESCRIPTION:		Implementation file for LGABackground

AUTHOR:				Robin Mair

CREATION DATE :	96.11.19

CHANGE HISTORY :

		96.11.19		rtm	Initial creation of file
								
---------------------------------------<< е >>-----------------------------------------
*/

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif


#ifdef PowerPlant_PCH
	#include PowerPlant_PCH
#endif

#include "LGABackground.h"

// е GRAYSCALE HEADERS
#include <UGraphicsUtilities.h>
#include	<UGAColorRamp.h>

// е POWERPLANT HEADERS
#include <PP_Messages.h>
#include <UDrawingUtils.h>
#include <UDrawingState.h>
#include <LAttachment.h>


//=====================================================================================
// CONSTANTS
//=====================================================================================

//=====================================================================================
// DEFINES
//=====================================================================================

#pragma mark === CLASS: LGABackground

#pragma mark -
#pragma mark === STREAM CREATION

//=====================================================================================
// ее STREAM CREATION
//-------------------------------------------------------------------------------------
// LGABackground::CreateLGABackgroundStream	
//-------------------------------------------------------------------------------------
//

LGABackground*	
LGABackground::CreateLGABackgroundStream ( LStream *inStream )
{

	return ( new LGABackground ( inStream ));
	
}	// LGABackground::CreateLGABackgroundStream


//=====================================================================================
// CLASS:	LGABackground
//=====================================================================================

#pragma mark -
#pragma mark === INITIALIZATION & DESTRUCTION

//=====================================================================================
// ее INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LGABackground::LGABackground					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LGABackground::LGABackground (	PaneIDT	inInsetSubPane,
											SInt16		inNotchSize,
											Boolean 	inPaintFace,
											Boolean 	inFrameSubPane,
											Boolean 	inNotchSubPane,
											Boolean 	inNotchFace,
											Boolean 	inExecuteHost )
												: LAttachment (	 msg_AnyMessage, 
																		inExecuteHost )
{

	// е Get all of the fields initialized
	mInsetSubPane = nil;
	mInsetSubPaneID = inInsetSubPane;
	mPaintFace = inPaintFace;
	mNotchFace = inNotchFace;
	mFrameInsetSubPane = inFrameSubPane;
	mNotchInsetFrame = inNotchSubPane;
	mNotchWidth = inNotchSize;
	
}	//	LGABackground::LGABackground


//-------------------------------------------------------------------------------------
// LGABackground::LGABackground					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LGABackground::LGABackground (	LStream*	inStream ) : LAttachment ( inStream )
{

	//	е Setup the data members for the view
	mInsetSubPane = nil;
	*inStream >> mInsetSubPaneID;
	*inStream >> mPaintFace;
	*inStream >> mNotchFace;
	*inStream >> mFrameInsetSubPane;
	*inStream >> mNotchInsetFrame;
	*inStream >> mNotchWidth;

}	//	LGABackground::LGABackground


#pragma mark -
#pragma mark === ACCESSORS

//=====================================================================================
// ее ACCESSORS
//-------------------------------------------------------------------------------------
// LGABackground::GetInsetSubPane
//-------------------------------------------------------------------------------------

LPane*
LGABackground::GetInsetSubPane ()
{

	// е Get a reference to our owner
	LPane*	thePane = dynamic_cast <LPane*>(mOwnerHost);
	ThrowIfNil_ ( thePane );
	
	// е We are going to setup the cahce for the pane only if we have to
	if ( mInsetSubPane == nil )
	{
		if ( mInsetSubPaneID != PaneIDT_Undefined )
			mInsetSubPane = thePane->FindPaneByID ( mInsetSubPaneID );
		else
			mInsetSubPane = nil;
	}
	
	// е Return the cached pane or nil
	return mInsetSubPane;

}	//	LGABackground::GetInsetSubPane


//-------------------------------------------------------------------------------------
// LGABackground::GetFaceRegion
//-------------------------------------------------------------------------------------

RgnHandle
LGABackground::GetFaceRegion ()
{
	//	е Setup a region that will represent the face of the border
	RgnHandle faceRgn = ::NewRgn ();
	ThrowIfNil_ ( faceRgn );
	
	// е Get a reference to our owner
	LPane*	thePane = dynamic_cast <LPane*>(mOwnerHost);
	ThrowIfNil_ ( thePane );

	// е Calculate the border frame rect
	Rect localFrame;
	thePane->CalcLocalFrameRect ( localFrame );
	::RectRgn ( faceRgn, &localFrame);

	// е If requested remove the portion of the border that would be
	// occupied by the growbox from the face region
	if ( WantsFaceNotched ())  
	{
		RgnHandle growRgn = ::NewRgn ();
		ThrowIfNil_ ( growRgn );
		Rect growRect = localFrame;
		growRect.left = growRect.right - mNotchWidth;
		growRect.top = growRect.bottom - mNotchWidth;
		::RectRgn ( growRgn, &growRect );
		::DiffRgn ( faceRgn, growRgn, faceRgn );
		::DisposeRgn ( growRgn );
	}

	// е Now we need to removed the portion of the face occupied by
	// the inset sub pane, if this has been requested and we have
	// a reference to a sub pane
	LPane*	insetSubPane = GetInsetSubPane ();
	if ( insetSubPane )  
	{
		RgnHandle insetRgn = ::NewRgn ();
		ThrowIfNil_ ( insetRgn );
		Rect insetRect;
		insetSubPane->CalcPortFrameRect ( insetRect );
		thePane->PortToLocalPoint ( topLeft ( insetRect ));
		thePane->PortToLocalPoint ( botRight( insetRect ));
		::RectRgn ( insetRgn, &insetRect );
		
		// е Now if we also need to notch out the inset frame then handle it now
		if ( WantsInsetFrameNotched ())
		{
			RgnHandle notchRgn = ::NewRgn ();
			ThrowIfNil_ ( notchRgn );
			Rect	notchRect = insetRect;
			notchRect.left = notchRect.right - mNotchWidth;
			notchRect.top = notchRect.bottom - mNotchWidth;
			::RectRgn ( notchRgn, &notchRect );
			::DiffRgn ( insetRgn, notchRgn, insetRgn );
			::DisposeRgn ( notchRgn );
		}
		
		// е Now we can get the inset region removed from the face region
		::DiffRgn ( faceRgn, insetRgn, faceRgn );
		::DisposeRgn ( insetRgn );
	}
	
	//	е Now that is all done we can return the region to the caller
	// whow will be responsible for disposing of it when done
	return faceRgn;

}	//	LGABackground::GetFaceRegion


//-------------------------------------------------------------------------------------
// LGABackground::GetInsetFrameRegion
//-------------------------------------------------------------------------------------

RgnHandle
LGABackground::GetInsetFrameRegion ()
{

	// е Now we need to removed the portion of the face occupied by
	// the inset sub pane, if this has been requested and we have
	// a reference to a sub pane
	LPane*	insetSubPane = GetInsetSubPane ();
	if ( insetSubPane )  
	{
		// е Get a reference to our owner
		LPane*	thePane = dynamic_cast <LPane*>(mOwnerHost);
		ThrowIfNil_ ( thePane );

		RgnHandle insetRgn = ::NewRgn ();
		ThrowIfNil_ ( insetRgn );
		Rect insetRect;
		insetSubPane->CalcPortFrameRect ( insetRect );
		thePane->PortToLocalPoint ( topLeft ( insetRect ));
		thePane->PortToLocalPoint ( botRight( insetRect ));
		::RectRgn ( insetRgn, &insetRect );
		
		// е Now if we also need to notch out the inset frame then handle it now
		if ( WantsInsetFrameNotched ())
		{
			RgnHandle notchRgn = ::NewRgn ();
			ThrowIfNil_ ( notchRgn );
			Rect	notchRect = insetRect;
			notchRect.left = notchRect.right - mNotchWidth;
			notchRect.top = notchRect.bottom - mNotchWidth;
			::RectRgn ( notchRgn, &notchRect );
			::DiffRgn ( insetRgn, notchRgn, insetRgn );
			::DisposeRgn ( notchRgn );
		}
		
		// е Now we need to build the outer portion of the region
		RgnHandle outerRgn = ::NewRgn ();
		ThrowIfNil_ ( outerRgn );
		::InsetRect ( &insetRect, -2, -2 );
		::RectRgn ( outerRgn, &insetRect );
		
		// е Now if we also need to notch out the inset frame then handle it now
		if ( WantsInsetFrameNotched ())
		{
			RgnHandle outerNotchRgn = ::NewRgn ();
			ThrowIfNil_ ( outerNotchRgn );
			Rect	outerNotchRect = insetRect;
			outerNotchRect.left = outerNotchRect.right - mNotchWidth;
			outerNotchRect.top = outerNotchRect.bottom - mNotchWidth;
			::RectRgn ( outerNotchRgn, &outerNotchRect );
			::DiffRgn ( outerRgn, outerNotchRgn, outerRgn );
			::DisposeRgn ( outerNotchRgn );
		}
		
		// е Now we can get the inset region removed from the face region
		::DiffRgn ( outerRgn, insetRgn, outerRgn );
		::DisposeRgn ( insetRgn );

		//	е Now that is all done we can return the region to the caller
		// whow will be responsible for disposing of it when done
		return outerRgn;

	}
	
	//	е We don't have an inset pane to frame so we just return nil
	return nil;

}	//	LGABackground::GetInsetFrameRegion


//-------------------------------------------------------------------------------------
// LGABackground::SetPaintFace
//-------------------------------------------------------------------------------------

void
LGABackground::SetPaintFace	(	Boolean	inPaintFace,
											Boolean	inRedraw  )
{

	// е We only change the sub pane if we have too
	if ( mPaintFace != inPaintFace )
	{
		mPaintFace = inPaintFace;
		
		// е Get the border redrawn if requested
		if ( inRedraw )
			RefreshFace ();
	}
	
}	//	LGABackground::SetPaintFace


//-------------------------------------------------------------------------------------
// LGABackground::SetInsetSubPane
//-------------------------------------------------------------------------------------

void
LGABackground::SetInsetSubPane	(	LPane*	inInsetSubPane )
{

	// е We only change the sub pane if we have too,in addition
	// to setting the reference we also need to make sure that
	// we get things invalidated so that the new sub pane shows
	// up and gets framed if requested
	if ( mInsetSubPane != inInsetSubPane )
	{
		
		// е Before we switch to the new inset subpane make sure we
		// refresh the old so that it is erased, this only happens if
		// there actually is one
		RefreshInsetSubPane ();
		
		// е Now we cansetup the new reference
		mInsetSubPane = inInsetSubPane;
		
		// е Now we get the new subpane refreshed so that it will
		// show up, NOTE: this will only happen if there is one as
		// there are many occasions where the subpane could be set
		// to nil, this check is done in the refresh method
		RefreshInsetSubPane ();
	}
	
}	//	LGABackground::SetInsetSubPane


//-------------------------------------------------------------------------------------
// LGABackground::SetNotchWidth
//-------------------------------------------------------------------------------------

void
LGABackground::SetNotchWidth	(	SInt16		inNotchWidth,
											Boolean	inRedraw )
{

	// е We only change the sub pane if we have too
	if ( mNotchWidth != inNotchWidth )
	{
		mNotchWidth = inNotchWidth;
		
		// е Get the border redrawn if requested
		if ( inRedraw )
			RefreshFace ();
	}
	
}	//	LGABackground::SetNotchWidth


#pragma mark -
#pragma mark === ACCESSORS

//=====================================================================================
// ее EXECUTION
//-------------------------------------------------------------------------------------
// LGABackground::ExecuteSelf
//-------------------------------------------------------------------------------------
//
//	Yes! I know this is a very long method but it has to do some fairly interesting
// drawing in order to minimize the amount of flicker that could be generated by painting
// the background a color with every update.  At somepoint I will look into breaking it
// into smaller more readable chunks.

void
LGABackground::ExecuteSelf (	MessageT	inMessage,
										void*		ioParam )
{

	switch ( inMessage )
	{
		case msg_DrawOrPrint:
		{
			StColorPenState	theColorState;
			
			// е A rectangle is passed in when the attachment message is draw or print
			Rect	theRect = *(Rect*) ioParam;
			
			// е Setup a device loop so that we can handle drawing at the correct bit depth
			StDeviceLoop	theLoop ( theRect );
			SInt16				depth;
			while ( theLoop.NextDepth ( depth )) 
			{
				// е Call our method that handles the drawing of the background
				DrawBackground ( depth );
			}
		}
		break;
	}
	
}	//	LGABackground::ExecuteSelf


#pragma mark -
#pragma mark === DRAWING

//=====================================================================================
// ее DRAWING
//-------------------------------------------------------------------------------------
// LGABackground::DrawBackground
//-------------------------------------------------------------------------------------

void
LGABackground::DrawBackground (	SInt16	inBitDepth )
{

	// е Local face region 
	RgnHandle 	faceRgn = nil;
	
	// е Get a reference to our owner
	LPane*	thePane = dynamic_cast <LPane*>(mOwnerHost);
	ThrowIfNil_ ( thePane );

	// е Get a reference to the inset subpane
	LPane*	insetSubPane = GetInsetSubPane ();
	ThrowIfNil_ ( insetSubPane );

	// е COLOR
	if ( inBitDepth >= 4 )
	{
		// е First we start out by painting the face of the border
		if ( WantsFacePainted ())
		{
			faceRgn = GetFaceRegion ();
			if ( faceRgn != nil )
			{
				RGBColor theColor = UGAColorRamp::GetColor(2);
				::RGBForeColor ( &theColor );
				::PaintRgn ( faceRgn );
			}
		}
		
		// е If requested then we go ahead and frame the inset sub pane if there is one
		if ( WantsInsetSubPaneFramed () && insetSubPane ) 
		{
			// е We need to know if the inset pane is active and enabled
			// еее NOTE: we are currently forcing this to be true as I haven't yet
			// figured out a way to no when there is an activate or deactivate 
			// on the owner
			Boolean active = true; // insetSubPane->IsActive ();
			Boolean enabled = true; // insetSubPane->IsEnabled ();

			// е Figure out the inset pane's bounding rect
			Rect	insetRect;
			insetSubPane->CalcPortFrameRect ( insetRect );
			thePane->PortToLocalPoint ( topLeft ( insetRect ));
			thePane->PortToLocalPoint ( botRight ( insetRect ));
			
			// е No we can proceed with the drawing of the frame around the
			// inset sub pane
			// е BLACK INNER FRAME
			//	е TOPLEFT EDGES
			RGBColor theColor;
			enabled ? ( active ? theColor = UGAColorRamp::GetBlackColor() : 
								theColor = UGAColorRamp::GetColor(10)) :
								theColor = UGAColorRamp::GetColor(10);
			::RGBForeColor (&theColor); 
			UGraphicsUtilities::TopLeftSide ( &insetRect, -1, -1, -1, -1 );
			
			//	е BOTTOM RIGHT EDGES
			//	If a notch has been requested then we draw the frame with a notch
			// otherwise we simply frame it
			if ( WantsInsetFrameNotched ())
				UGraphicsUtilities::BottomRightSideWithNotch ( &insetRect, -1, -1, -1, -1, mNotchWidth );
			else
				UGraphicsUtilities::BottomRightSide  (&insetRect, -1, -1, -1, -1 );
				
			// е 3D SHADOWS
			// We only draw the shadows if the border is enabled and active
			if ( enabled && active )
			{
				// е TOPLEFT SHADOW
				::InsetRect ( &insetRect, -1, -1 );
				RGBColor theColor = UGAColorRamp::GetColor(5);
				::RGBForeColor ( &theColor );
				UGraphicsUtilities::TopLeftSide ( &insetRect, -1, -1, 0, 0 );

				// е BOTTOM RIGHT LIGHT EDGES
				theColor = UGAColorRamp::GetWhiteColor();
				::RGBForeColor ( &theColor );
				
				// е If a notch has been requested then handle it, otherwise just
				// do the edge drawing
				if ( WantsInsetFrameNotched ())
					UGraphicsUtilities::BottomRightSideWithNotch ( &insetRect, 0, 0, -1, -1, mNotchWidth );
				else
					UGraphicsUtilities::BottomRightSide ( &insetRect, 0, 0, -1, -1 );
			}
		}
	}
	else		//	е BLACK & WHITE
	{
		// е For B&W all we do is erase the entire face of the border too white
		if ( WantsFacePainted ())
		{
			faceRgn = GetFaceRegion ();
			if ( faceRgn != nil )
			{
				RGBColor theColor = UGAColorRamp::GetWhiteColor();
				::RGBBackColor ( &theColor );	
				::EraseRgn ( faceRgn );
			}
		}
		
		// е If requested then we go ahead and frame the inset sub pane
		if ( WantsInsetSubPaneFramed () && insetSubPane ) 
		{
			// е Figure out the inset pane's bounding rect
			Rect	insetRect;
			insetSubPane->CalcPortFrameRect ( insetRect );
			thePane->PortToLocalPoint ( topLeft ( insetRect ));
			thePane->PortToLocalPoint ( botRight ( insetRect ));
			
			// е No we can proceed with the drawing of the frame around the
			// inset sub pane
			// е BLACK INNER FRAME
			//	е TOPLEFT EDGES
			RGBColor theColor = UGAColorRamp::GetBlackColor();
			::RGBForeColor (&theColor ); 
			UGraphicsUtilities::TopLeftSide ( &insetRect, -1, -1, -1, -1 );
			
			//	е BOTTOM RIGHT EDGES
			//	If a notch has been requested then we draw the frame with a notch
			// otherwise we simply frame it
			if ( WantsInsetFrameNotched ())
				UGraphicsUtilities::BottomRightSideWithNotch ( &insetRect, -1, -1, -1, -1, mNotchWidth );
			else
				UGraphicsUtilities::BottomRightSide  (&insetRect, -1, -1, -1, -1 );
		}
	}
	
	// е Get rid of the face region
	if ( faceRgn != nil )
		::DisposeRgn ( faceRgn );

}	//	LGABackground::DrawBackground



//-------------------------------------------------------------------------------------
// LGABackground::RefreshFace
//-------------------------------------------------------------------------------------

void
LGABackground::RefreshFace ()
{
	
	// е Get the face region and then invalidate it if it is valid
	RgnHandle	faceRgn = GetFaceRegion ();
	if ( faceRgn != nil )
	{
		// е Get a reference to our owner
		LPane*	thePane = dynamic_cast <LPane*>(mOwnerHost);
		if ( thePane )
		{
			thePane->FocusDraw ();
			thePane->InvalPortRgn ( faceRgn );
			::DisposeRgn ( faceRgn );
		}
	}
	
}	//	LGABackground::RefreshFace



//-------------------------------------------------------------------------------------
// LGABackground::RefreshInsetFrame
//-------------------------------------------------------------------------------------

void
LGABackground::RefreshInsetFrame ()
{
	
	// е Get the face region and then invalidate it, GetInsetFrameRegion will
	// return nil if there is no inset pane so we only perform the invalidation
	// when we get a valid region
	RgnHandle	insetRgn = GetInsetFrameRegion ();
	if ( insetRgn != nil )
	{
		LPane*	thePane = dynamic_cast <LPane*>(mOwnerHost);
		if ( thePane )
		{
			thePane->FocusDraw ();
			thePane->InvalPortRgn ( insetRgn );
			::DisposeRgn ( insetRgn );
		}
	}
	
}	//	LGABackground::RefreshInsetFrame


//-------------------------------------------------------------------------------------
// LGABackground::RefreshInsetSubPane
//-------------------------------------------------------------------------------------

void
LGABackground::RefreshInsetSubPane ()
{
	
	// е We only do it if we infact do have an inset sub pane
	LPane*	insetSubPane = GetInsetSubPane ();
	if ( insetSubPane )
	{
		// е First refresh the pane itself
		insetSubPane->Refresh ();
		
		// е Then we refesh the frame of teh pane if there is one
		if ( WantsInsetSubPaneFramed ())
			RefreshInsetFrame ();
			
	}
	
}	//	LGABackground::RefreshInsetSubPane


