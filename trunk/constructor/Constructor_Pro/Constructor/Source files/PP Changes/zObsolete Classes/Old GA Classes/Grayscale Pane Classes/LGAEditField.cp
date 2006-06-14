// Copyright й2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyrightй 1996 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< е >>-----------------------------------------

FILE:					LGAEditField.cp

CLASSES:				LGAEditField

DESCRIPTION:		Implementation file for LGAEditField

AUTHOR:				Robin Mair

CREATION DATE :	96.06.09

CHANGE HISTORY :

		97.04.20		rtm	Removed the CreateFromStream method.
		97.02.08		rtm	Changed the rendering of the control such that is it drawn as
								disabled when it is deactivated, this is matches the approach
								Apple is taking with the Apearance Manager.
		97.01.24		rtm	Added a copy constructor.
		97.01.22		rtm	Added a default constructor.
		97.01.20		rtm	Changed the EnableSelf and DisableSelf methods so that they now
								call Draw() instead of DrawSelf(), this means that the correct 
								check will be done to make sure the control is visible before
								being drawn.  It also means that if there are attachments on the
								control they can be executed.
		96.10.20		rtm	Fixed another cross device drawing problem where the pen normal
								call was in the wrong location which resulted in the border
								being drawn incorrectly when the B&W device was to the left of
								the color device.
		96.10.19		rtm	Added in Eric's fix for drawing across multiple devices where
								there was a case where the border would not be drawn correctly.
		96.10.19		rtm	Fixed a bug in the drawing of the edit field where the content
								of the edit field would not be erased to white when it was
								empty and got an update.  The fix is in DrawSelf.
		96.10.13		rtm	Had to do some rather crazy fussing around with the clipping
								in order to get the border to draw correctly across multiple
								devices of different bit depths.
		96.10.08		rtm	Got rid of using an attachment to handle the drawing of the
								shadows all drawing is now just handled by this class.
		96.10.07		rtm	Added an override of Refresh that accounts for the fact that
								the shadows for the field are being drawn outside its notmal
								bounds and increases the invalidated area by one pixel.
		96.10.07		rtm	Completely revamped this class so that it now handles drawing
								the correct appearance as per AGA.  Drawing is split between
								this class and LGAEditFieldShadowAttachment which handles the
								rendering of the shadows around the field. Additionally, this
								class is now sub-classed from LBroadcasterEditField which can
								be found in the folder е Constructor Additions:Text Edting.
		96.09.04		rtm	PowerPlant conformance changes.
		96.09.02		rtm	Number of changes to support the rendering of the field when
								disabled as specified by Apple's revised AGA spec. Also added
								overrides for EnableSelf and DisableSelf to handle redrawing
								the field when this state changes.
		96.08.05		rtm	Fixed a problem where the edit field would always draw with a
								background color that matched that of any 'wctb' resource that
								was associated with the window the field was in.
		96.08.05		rtm	Merged in a bunch of changes that Eric #1 had made to his
								LBroadcasterEditField.
		96.06.17		rtm	Fixed the edit field so that it now erases its content with
								white so that the content of the field is always white.
		96.06.10		rtm	Integrated Eric's GBroadcasterEditField into this class so that
								it is easier to transport around as there are many cases where
								we want it to handle broadcasting anyway.
		96.06.09		rtm	Initial creation of file

---------------------------------------<< е >>-----------------------------------------
*/

#ifdef PowerPlant_PCH
	#include PowerPlant_PCH
#endif

// е GRAYSCALE HEADERS
#include <LGAEditField.h>
#include <UGraphicsUtilities.h>
#include <UGAColorRamp.h>

// е POWERPLANT HEADERS
#include <LView.h>
#include <LStream.h>
#include <LString.h>
#include <UDrawingState.h>
#include <UDrawingUtils.h>
#include <UTextTraits.h>
#include <PP_Messages.h>
#include <UTETextAction.h>
#include <UKeyFilters.h>


//=====================================================================================
// CONSTANTS
//=====================================================================================


//=====================================================================================
// DEFINES
//=====================================================================================


#pragma mark === CLASS: LGAEditField

//=====================================================================================
// CLASS:	LGAEditField
//=====================================================================================

#pragma mark -
#pragma mark === INITIALIZATION

//=====================================================================================
// ее INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LGAEditField::LGAEditField					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Default constructor

LGAEditField::LGAEditField ()
{
}	//	LGAEditField::LGAEditField


//-------------------------------------------------------------------------------------
// LGAEditField::LGAEditField					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Copy constructor

LGAEditField::LGAEditField (	const LGAEditField &inOriginal )
						:	LBroadcasterEditField ( inOriginal )
{
}	//	LGAEditField::LGAEditField


//-------------------------------------------------------------------------------------
// LGAEditField::LGAEditField					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Stream constructor

LGAEditField::LGAEditField ( LStream*	inStream ) : LBroadcasterEditField ( inStream )
{
}	//	LGAEditField::LGAEditField


//-------------------------------------------------------------------------------------
// LGAEditField::LGAEditField					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Parameterized constructor

LGAEditField::LGAEditField (	const SPaneInfo&	inPaneInfo,
										Str255				inString,
										ResIDT				inTextTraitsID,
										SInt16					inMaxChars,
										UInt8					inAttributes,
										TEKeyFilterFunc	inKeyFilter,
										LCommander*			inSuper)
					: LBroadcasterEditField ( 	inPaneInfo, 
														inString, 
														inTextTraitsID,
														inMaxChars, 
														inAttributes,
														inKeyFilter, 
														inSuper )

{
}	//	LGAEditField::LGAEditField


//-------------------------------------------------------------------------------------
// LGAEditField::~LGAEditField				---<<е DESTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LGAEditField::~LGAEditField ()
{
}	//	LGAEditField::~LGAEditField


#pragma mark -
#pragma mark === ENABLING & DISABLING

//=====================================================================================
// ее ENABLING & DISABLING
//-------------------------------------------------------------------------------------
// LGAEditField::EnableSelf
//-------------------------------------------------------------------------------------
//

void
LGAEditField::EnableSelf ()
{
	
	// е Instead of just redrawing the border as is done in LEditField
	// we will force a redraw so that the text gets redrawn in the
	// appropriate color
	Draw ( nil );
	
}	//	LGAEditField::EnableSelf


//-------------------------------------------------------------------------------------
// LGAEditField::DisableSelf
//-------------------------------------------------------------------------------------
//

void
LGAEditField::DisableSelf ()
{
	// е Shouldn't be on duty when disabled
	if ( IsOnDuty()) 
	{				
		SwitchTarget(GetSuperCommander());
	}

	// е Instead of just redrawing the border as is done in LEditField
	// we will force a redraw so that the text gets redrawn in the
	// appropriate color
	Draw ( nil );

}	//	LGAEditField::DisableSelf


#pragma mark -
#pragma mark === ACTIVATION

//=====================================================================================
// ее ACTIVATION
//-------------------------------------------------------------------------------------
// LGAEditField::ActivateSelf
//-------------------------------------------------------------------------------------
//

void
LGAEditField::ActivateSelf ()
{

	// е Get everything redrawn so the changes take effect
	if ( mActive == triState_On )
		Refresh ();
					
}	//	LGAEditField::ActivateSelf


//-------------------------------------------------------------------------------------
// LGAEditField::DeactivateSelf
//-------------------------------------------------------------------------------------
//

void
LGAEditField::DeactivateSelf ()
{

	// е Get everything redrawn so the changes take effect
	if ( mActive == triState_Off || mActive == triState_Latent )
		Refresh ();
				
}	//	LGAEditField::DeactivateSelf


#pragma mark -
#pragma mark === DRAWING

//=====================================================================================
// ее DRAWING
//-------------------------------------------------------------------------------------
// LGAEditField::Refresh
//-------------------------------------------------------------------------------------
//

void
LGAEditField::Refresh ()
{
	Rect	refreshRect;
	
	if ( IsVisible ()  && CalcPortFrameRect ( refreshRect )  && ( mSuperView != nil ) ) 
	{

		// е Make the rect a little bigger for the shadows
		::InsetRect ( &refreshRect, -1, -1 );
		
		Rect	superRevealed;
		mSuperView->GetRevealedRect ( superRevealed );
		if ( ::SectRect ( &refreshRect, &superRevealed, &refreshRect )) 
		{
			InvalPortRect ( &refreshRect );
		}
	}
	
}	//	LGAEditField::Refresh


//-------------------------------------------------------------------------------------
// LGAEditField::FocusDraw
//-------------------------------------------------------------------------------------
//
//	We have replaced this method so that we can set the background color to white and
//		thereby fake out text edit so that the background color is not used from the
//		window's wctb, we always want it to be white

Boolean
LGAEditField::FocusDraw ( LPane* inSubPane )
{
	
	Boolean	focused = LPane::FocusDraw ( inSubPane );
	UTextTraits::SetPortTextTraits ( mTextTraitsID );
	RGBColor theColor = UGAColorRamp::GetWhiteColor ();
	::RGBBackColor ( &theColor);
	
	return focused;
	
}	//	LGAEditField::FocusDraw


//-------------------------------------------------------------------------------------
// LGAEditField::DrawSelf
//-------------------------------------------------------------------------------------

void
LGAEditField::DrawSelf ()
{
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );
	
	// е Draw Border if necessary
	if ( mHasBox ) 
	{				
		DrawBox ();
		::InsetRect ( &localFrame, 2, 2 );
	}

	//	е We erase the content to white as we always want a white background
	// for the text to draw against, we don't need to set the background 
	// color to white aws that has already been done for us in FocusDraw
	::InsetRect ( &localFrame, -1, -1 );
	::EraseRect ( &localFrame );
	::InsetRect ( &localFrame, 1, 1 );
	
	// е A Mac TERec stores a pointer to its owner port  We have to
	// change it to the current port in case we are drawing into
	// a port that is not the owner port. This happens when we are
	// printing or drawing into an offscreen port.
	GrafPtr	savePort = (**mTextEditH).inPort;
	(**mTextEditH).inPort = UQDGlobals::GetCurrentPort ();
	
	// е If the field is disabled we need to get the text drawn in a gray
	// color from the AGA spec, we only do this if we are drawing in color
	{
		StDeviceLoop	theLoop ( localFrame );
		SInt16				depth;
		while ( theLoop.NextDepth ( depth )) 
		{
			if ( depth >= 4 && !IsEnabled () || !IsActive ())
			{
				RGBColor theColor = UGAColorRamp::GetColor(7);
				::RGBForeColor ( &theColor );
			}
		}
	}
	
	// е Update the text
	::TEUpdate ( &localFrame, mTextEditH );
	
	// е Restore the old port
	(**mTextEditH).inPort = savePort;
	
}	//	LGAEditField::DrawSelf


//-------------------------------------------------------------------------------------
// LGAEditField::DrawBox
//-------------------------------------------------------------------------------------

void
LGAEditField::DrawBox ()
{

	StColorPenState	theColorPenState;

	Rect					localFrame;
	CalcLocalFrameRect ( localFrame );
	
	// е Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( localFrame );
	SInt16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
			
		// е Normalize the pen state
		StColorPenState::Normalize ();
		
		// е INNER FRAME - we always do this
		RGBColor theColor =  IsEnabled () && IsActive () ? UGAColorRamp::GetBlackColor() : 
											(depth < 4 ? UGAColorRamp::GetBlackColor() : 
													UGAColorRamp::GetColor(7));
		::RGBForeColor ( &theColor );
		::FrameRect ( &localFrame );

		// COLOR
		if ( depth >= 4 )
		{
			StClipRgnState	theClipState;

			// е Setup the correct clipping for the border, basically what we are
			// attempting to do here is to find out how the current clipping overlaps
			// our bounds so that we can setup the new clipping appropriately, the goal
			// being to add enough to the clipping to allow the shadows to be rendered
			::InsetRect ( &localFrame, -1, -1 );
			Rect newClipRect = localFrame;
			RgnHandle	currClip = ::NewRgn ();
			::GetClip ( currClip );
			Rect clipRect;
#if PP_Target_Carbon
			::GetRegionBounds(currClip, &clipRect);
#else
			clipRect = (**currClip).rgnBBox;
#endif
			::DisposeRgn ( currClip );
			
			// е Now we are going to setup the new clipping taking care of the
			// boundary cases across multiple devices
			if ( clipRect.left - 1 > newClipRect.left )
				newClipRect.left = clipRect.left;
			else if ( clipRect.right + 1 < newClipRect.right )
				newClipRect.right = clipRect.right;
			::ClipRect ( &newClipRect );

			// е DRAW 3D SHADOWS
			// е TOPLEFT SHADOW
			theColor = IsEnabled () && IsActive () ? UGAColorRamp::GetColor(5) :
										 UGAColorRamp::GetColor(2);
			::RGBForeColor ( &theColor );
			UGraphicsUtilities::TopLeftSide ( &localFrame, 0, 0, 1, 1 );

			// е BOTTOM RIGHT LIGHT EDGES
			theColor = IsEnabled () && IsActive () ? UGAColorRamp::GetWhiteColor() :
										 UGAColorRamp::GetColor(2);
			::RGBForeColor ( &theColor );
			UGraphicsUtilities::BottomRightSide ( &localFrame, 1, 1, 0, 0 );
		
			// е Inset the rect for the drawing of the text
			::InsetRect ( &localFrame, 1, 1 );
			
		}	
	}
	
}	//	LGAEditField::DrawBox


