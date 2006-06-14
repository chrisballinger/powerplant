// Copyright й2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyrightй 1995 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< е >>-----------------------------------------

FILE:					LGABox.cp

CLASSES:				LGABox

DESCRIPTION:		Implementation file for LGABox

AUTHOR:				Robin Mair

CREATION DATE :	95.12.19

CHANGE HISTORY :

		97.02.08		rtm	Changed the rendering of the control such that is it drawn as
								disabled when it is deactivated.
		97.01.24		rtm	Added a copy constructor.
		97.01.22		rtm	Added a parameterized conbstructor so that instances can be 
								created procedurally.
		96.12.08		rtm	Changed justification constants to use the new ones defined
								in the universal headers.
		96.11.05		rtm	Fixed a bug in the Refresh and DontRefresh methods where the two 
								rects being checked with SectRect were in different coordinate 
								systems one port and the other local, they are now both set to use 
								port coordinates. There was also a similar problem in the region
								calculation methods where the revealed rect needed to be converted
								from port to local coordinates.
		96.10.31		rtm	Changed the overrides of Enable and Disable back to EnableSelf
								and DisableSelf, but changed the code to call DrawSelf instead
								of the old call to Draw.
		96.10.27		rtm	Found a case where the title rect was not being calculated
								correctly this resulted in all of the title rect calculation
								methods changing.
		96.10.26		rtm	Another revision, the box classes were not using the title
								mixin class which is used in every other class which has a title,
								so for the sake of consistency I have changed the box classes
								to use the same mixin.  In addition, the calculation of the 
								various rects used in the box have been changed so that instead
								of caching the value they just calculate it on demand.  This 
								meant that we could get rid of the overrides of MoveBy and 
								ResizeBy. The CalculateBoxRects method has also been removed 
								as it was no longer needed with the above changes. These
								changes also made the rect methods more like the ones in 
								PowerPlant as they take a reference to a rect.
		96.10.21		rtm	Added a call to refresh after the text trait ID has been 
								changed so that the change will actually appear. Also changed
								the call to RefreshBoxTitle in SetDescriptor to a call to
								Refresh instead the idea being that we wan the border to be
								correctly as well so we might as well getit refreshed. This
								could be optimized a litte further by calling RefreshBoxBorder
								and RefreshBoxTitle so that the content does not get redrawn.
		96.10.21		rtm	Removed override of AdaptToSuperFrameSize.
		96.10.19		rtm	Changed the overrides of EnableSelf and DisableSelf to ones
								on Enable and Disable instead, reason being so that the draw
								would happen after all the subpanes had been changed.
		96.10.16		rtm	Fixed a problem where the title rect was not being calculated
								properly when the title was longer than the width of the box.
		96.10.12		rtm	Had to tweak the drawing of titles across multiple devices
								disabled.  Using the grayishTextOr text mode transfer was
								resulting in some weirdness that could only be solved by
								doing something similar to what grayishTextOr did, which was
								to basically lighten the text color.
		96.10.09		rtm	Changed the title drawing so that it now will respect a color
								provided in a text trait as  opposed to always drawing the title
								in black.
		96.10.04		rtm	Replaced all SetCPixel calls with a new PaintColorPixel call
								that has been added to UGraphicsUtilities. This should get
								around the problems with using SetCPixel.
		96.09.19		rtm	Changed the title drawing method so that the actual call to 
								draw the text is inside the while loop used to check the
								screen depth.
		96.09.16		rtm	Changed the title accessors to conform to the PowerPlant API 
								provided by LPane.  Also added a recalulate box rects call to
								the text traits setter.
		96.09.12		rtm	Fixed a problem where the border was being calculated incorrectly
								based on the font being used for the title.
		96.09.12		rtm	Fixed a bug where the border was being calculated inset by one 
								pixel when it did not need to be.
		96.09.04		rtm	PowerPlant conformance changes.
		96.09.02		rtm	Added overrides of EnableSelf and DisableSelf so that we can
								handle getting the control redrawn when the state changes.
		96.09.02		rtm	Changed the rendering of the dimmed state of the control to
								meet Apple's modified spec for grayscale controls.
		96.06.26		rtm	Change the border drawing routine so that the clipping is
								now set so that the border is not drawn where the title 
								would be located, if there is one.  this fix brings it in
								line with the changes made to LGAPrimaryBox.
		96.06.26		rtm	Added override of ResizeFrameBy so that the we could call
								CalculateBoxRects() so that thing would get redrawn correctly
								when resized.
		96.06.03		rtm	Modified to be independent of the graphics context stuff for
								use in the IDE and debugger.
		96.03.26		rtm	Modified the method that calculates the content rect so that
								it now correctly allows for the title position when building
								the content rect
		96.03.14		rtm	Changed the way we choose between drawing in color or B&W.
		96.02.22		rtm	Made some changes to make the appearance of the box match
								Apple's specifications for Copland.  This required tweaking
								the base location of teh default title and changing the use
								of colors.  The box no longer calculates the color based on
								the background color it just uses the defined colors.
		96.01.15		rtm	Modified to take into account changes that will be made to
								LPane to handle the fore and background colors.
		96.01.08		rtm	Modified the refreshing methods so that they do a focus before
								they call invalidate as the region was being in validates
								incorrectly.
		96.01.04		rtm	Changed the methods responsible for refreshing as they were
								not working as they should.
		95.12.20		rtm	Made some fixes so that the border was correctly erased under
								the title when the background is set to transparent.
		95.12.19		rtm	Completed implementation of LGABox class with all of the drawing
								permutations working.
		95.12.19		rtm	Initial creation of file

---------------------------------------<< е >>-----------------------------------------
*/

#ifdef PowerPlant_PCH
	#include PowerPlant_PCH
#endif

// е GRAYSCALE HEADERS
#include <LGABox.h>
#include <UGraphicsUtilities.h>
#include <UGAColorRamp.h>

// е POWERPLANT HEADERS
#include <LControl.h>
#include <LStream.h>
#include <LString.h>
#include <UDrawingState.h>
#include <UDrawingUtils.h>
#include <UTextTraits.h>
#include <PP_Messages.h>

// е TOOLBOX HEADERS
#include <MacWindows.h>


//=====================================================================================
// CONSTANTS
//=====================================================================================


//=====================================================================================
// DEFINES
//=====================================================================================


#pragma mark === CLASS: LGABox

//=====================================================================================
// CLASS:	LGABOX
//=====================================================================================

#pragma mark -
#pragma mark === INITIALIZATION

//=====================================================================================
// ее INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LGABox::LGABox					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Default constructor

LGABox::LGABox ()
{

	// е Get all of our fields initialized
	mHasBorder = false;
	mBorderStyle = borderStyleGA_NoBorder;
	mTitlePosition = titlePositionGA_None;
	mContentOffset = gEmptyRect;

}	//	LGABox::LGABox


//-------------------------------------------------------------------------------------
// LGABox::LGABox					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Copy constructor

LGABox::LGABox (	const LGABox &inOriginal )
									:	LView ( inOriginal ),
										LGATitleMixin ( inOriginal )
{

	// е Copy all of our fields 
	mHasBorder = inOriginal.mHasBorder;
	mBorderStyle = inOriginal.mBorderStyle;
	mTitlePosition = inOriginal.mTitlePosition;
	mContentOffset = inOriginal.mContentOffset;

}	//	LGABox::LGABox


//-------------------------------------------------------------------------------------
// LGABox::LGABox					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Stream constructor

LGABox::LGABox (	LStream	*inStream ) : LView ( inStream )
{

	// е Read in the data from the stream
	SInt16 borderStyle = 0;
	SInt16 titlePosition = 0;
	Boolean ignore;
	
	*inStream >> ignore;
	inStream->ReadPString ( mTitle );
	*inStream >> mTextTraitsID;
	*inStream >> mHasBorder;
	*inStream >> borderStyle;
	mBorderStyle = (EGABorderStyle)borderStyle;
	*inStream >> titlePosition;
	mTitlePosition = (EGATitlePosition)titlePosition;
	
	// е We will set up the offset with this default as there is no way to
	// handle custom pane type rect fields
	Rect r = { 4, 4, 4, 4 };
	mContentOffset = r;

}


//-------------------------------------------------------------------------------------
// LGABox::LGABox					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Parameterized constructor

LGABox::LGABox		(	const SPaneInfo	&inPaneInfo,
							const SViewInfo	&inViewInfo,
							Str255				inTitle,
							ResIDT				inTextTraitsID,
							Boolean				inHasBorder,
							SInt16					inBorderStyle,
							SInt16					inTitlePosition,
							Rect					&inContentOffset )
					: LView ( inPaneInfo, inViewInfo )
{

	// е Setup the classes fields
	mTitle = inTitle;
	mTextTraitsID = inTextTraitsID;
	mHasBorder = inHasBorder;
	mBorderStyle = (EGABorderStyle)inBorderStyle;
	mTitlePosition = (EGATitlePosition)inTitlePosition;
	mContentOffset = inContentOffset;

}	//	LGABox::LGABox


//-------------------------------------------------------------------------------------
// LGABox::~LGABox				---<<е DESTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LGABox::~LGABox ()
{
}	//	LGABox::~LGABox


#pragma mark -
#pragma mark === ACCESSORS

//=====================================================================================
// ее ACCESSORS - GETTERS
//-------------------------------------------------------------------------------------
// LGABox::GetDescriptor
//-------------------------------------------------------------------------------------

StringPtr
LGABox::GetDescriptor ( Str255	outTitle ) const
{

	return LString::CopyPStr ( mTitle, outTitle );

}	//	LGABox::GetDescriptor


//-------------------------------------------------------------------------------------
// LGABox::GetBoxBorderRegion
//-------------------------------------------------------------------------------------

RgnHandle
LGABox::GetBoxBorderRegion	(	const Rect	&inRevealed )
{
	// е Create a new region
	RgnHandle	borderRgn = ::NewRgn ();
	
	// е Open the region so that we can get it setup as the border
	// portion of our view, which is the border rect plus any offsets
	::OpenRgn ();
		Rect	borderRect;
		CalcBorderRect ( borderRect );
		::FrameRect ( &borderRect );					// Outer edge of border
		borderRect.left += mContentOffset.left;
		borderRect.top += mContentOffset.top;
		borderRect.bottom -= mContentOffset.bottom;
		borderRect.right -= mContentOffset.right;
		::FrameRect ( &borderRect );					// Inner edge of border
	::CloseRgn ( borderRgn );
	
	// е Clip the region using the revealed rect which we convert
	// to local coordinates as it is in port coordinates
	RgnHandle	revealedRgn = ::NewRgn ();
	PortToLocalPoint ( topLeft ( inRevealed ));
	PortToLocalPoint ( botRight ( inRevealed ));
	::RectRgn ( revealedRgn, &inRevealed );
	::SectRgn ( borderRgn, revealedRgn, borderRgn );
	::DisposeRgn ( revealedRgn );
	
	// е Return the region forming the border portion of the banner box
	return borderRgn;
	
}	//	LGABox::GetBoxBorderRegion



//-------------------------------------------------------------------------------------
// LGABox::GetBoxContentRegion
//-------------------------------------------------------------------------------------

RgnHandle
LGABox::GetBoxContentRegion	(	const Rect	&inRevealed )
{

	// е Create a new region
	Rect	contentRect;
	CalcContentRect ( contentRect );
	RgnHandle	contentRgn = ::NewRgn ();
	::RectRgn ( contentRgn, &contentRect );
	
	// е Clip the region using the revealed rect which we convert
	// to local coordinates as it is in port coordinates
	RgnHandle	revealedRgn = ::NewRgn ();
	PortToLocalPoint ( topLeft ( inRevealed ));
	PortToLocalPoint ( botRight ( inRevealed ));
	::RectRgn ( revealedRgn, &inRevealed );
	::SectRgn ( contentRgn, revealedRgn, contentRgn );
	::DisposeRgn ( revealedRgn );
	
	// е Return the region forming the border portion of the banner box
	return contentRgn;
	
}	//	LGABox::GetBoxContentRegion


//------------------------------------------------------------------------------------
//	LGABox::CalcTitleRect
//------------------------------------------------------------------------------------
//

void
LGABox::CalcTitleRect (	Rect &outRect )
{

	// е We are going to dispatch from here to individual functions that handle the
	// calculations for the title rect, one for each choice
	switch ( mTitlePosition )
	{
		case titlePositionGA_None:
			outRect.left = outRect.right = outRect.top = outRect.bottom = 0;
		break;
		
		case titlePositionGA_Default:
			CalculateDefaultTitleRect ( outRect );
		break;
		
		case titlePositionGA_TopRight:
			CalculateTopRightTitleRect ( outRect );
		break;
		
		case titlePositionGA_TopLeftOut:
			CalculateTopLeftOutsideTitleRect ( outRect );
		break;
		
		case titlePositionGA_TopRightOut:
			CalculateTopRightOutsideTitleRect ( outRect );
		break;
		
		case titlePositionGA_CenterTopOut:
			CalculateCenterTopOutsideTitleRect ( outRect );
		break;
		
		case titlePositionGA_CenterTop:
			CalculateCenterTopTitleRect ( outRect );
		break;
		
		case titlePositionGA_CenterInside:
			CalculateCenterInsideTitleRect ( outRect );
		break;
		
		case titlePositionGA_CenterTopInside:
			CalculateCenterTopInsideTitleRect ( outRect );
		break;
		
		case titlePositionGA_CenterBottomInside:
			CalculateCenterBottomInsideTitleRect ( outRect );
		break;
		
		case titlePositionGA_CenterBottom:
			CalculateCenterBottomTitleRect ( outRect );
		break;
		
		case titlePositionGA_CenterBottomOut:
			CalculateCenterBottomOutsideTitleRect ( outRect );
		break;
		
	};

}	//	LGABox::CalcTitleRect


//------------------------------------------------------------------------------------
//	LGABox::CalcContentRect
//------------------------------------------------------------------------------------
//

void
LGABox::CalcContentRect (	Rect &outRect )
{
	// е Setup some rects
	CalcBorderRect ( outRect );
 	Rect	titleRect;
 	CalcTitleRect ( titleRect );
 	Rect	offsets = GetContentOffset ();
 	
  	// е Adjust the rectangle
	// е TOP
	if ( HasTitle ())
	{
		switch ( mTitlePosition )
		{
			case titlePositionGA_Default:
			case titlePositionGA_TopRight:
			case titlePositionGA_CenterTop:
				outRect.top += 2 + offsets.top;
			break;
			
			case titlePositionGA_CenterTopInside:
				outRect.top += UGraphicsUtilities::RectHeight ( titleRect ) + offsets.top;
			break;
			default:
				outRect.top += offsets.top;
			break;
		}
	}
	else
		outRect.top += offsets.top;
		
	// е LEFT
	outRect.left += offsets.left;
	
	// е BOTTOM
	if ( HasTitle ()) 
	{
		if ( mTitlePosition == titlePositionGA_CenterBottomInside )
			outRect.bottom -= UGraphicsUtilities::RectHeight ( titleRect ) + offsets.bottom;
		else if ( mTitlePosition == titlePositionGA_CenterBottom )
			outRect.bottom -= 2 + offsets.bottom;
		else
			outRect.bottom -= offsets.bottom;
	}
	else
		outRect.bottom -= offsets.bottom;
	
	// е RIGHT
	outRect.right -= offsets.right;
  
}	//	LGABox::CalcContentRect


//------------------------------------------------------------------------------------
//	LGABox::CalcBorderRect
//------------------------------------------------------------------------------------
//

void
LGABox::CalcBorderRect (	Rect &outRect )
{

	StTextState			theTextState;
	
	// е Get the port setup too the text traits values
	UTextTraits::SetPortTextTraits ( GetTextTraitsID ());
	
	// е Get the font info
	FontInfo	fi;
	::GetFontInfo ( &fi );
	
	// е Get the frame for the box
	CalcLocalFrameRect ( outRect );

	// е Make sure the rect is up to date
	Rect	titleRect;
	CalcTitleRect ( titleRect );
	
	// е Setup some values based on whether there is a title or not
	SInt16 titleHeight = HasTitle() ? UGraphicsUtilities::RectHeight ( titleRect ) : 0;
	SInt16	offsetTitleHeight = HasTitle() ? titleHeight - 5: 0;
	
	// е Calculate the correct border rect based on the title position
	switch ( mTitlePosition )
	{
		case titlePositionGA_Default:
			outRect.top += HasTitle() ? fi.ascent - 1: 0;
		break;
		
		case titlePositionGA_CenterTop:
		case titlePositionGA_TopRight:
			outRect.top += HasTitle() ? fi.ascent - 2: 0;
		break;
		
		case titlePositionGA_TopLeftOut:
			outRect.top += titleHeight + 2;
		break;
		
		case titlePositionGA_TopRightOut:
			outRect.top += titleHeight + 2;
		break;
		
		case titlePositionGA_CenterTopOut:
			outRect.top += titleHeight + 2;
		break;
		
		case titlePositionGA_CenterInside:
		case titlePositionGA_CenterTopInside:
		case titlePositionGA_CenterBottomInside:
		break;
		
		case titlePositionGA_CenterBottom:
			outRect.bottom -= offsetTitleHeight;
		break;
		
		case titlePositionGA_CenterBottomOut:
			outRect.bottom -= titleHeight;
		break;
		
		default:
		break;
	}
	
}	//	LGABox::CalcBorderRect

//=====================================================================================
// ее ACCESSORS - SETTERS
//-------------------------------------------------------------------------------------
// LGABox::SetDescriptor
//-------------------------------------------------------------------------------------

void
LGABox::SetDescriptor ( ConstStr255Param	inDescriptor )
{
	if ( mTitle != inDescriptor )
	{
		mTitle = inDescriptor;
		
		// е Then we need to get the title redrawn, if requested
		Refresh ();
			
	}
	
}	//	LGABox::SetDescriptor


//-------------------------------------------------------------------------------------
// LGABox::SetBoxHasBorder
//-------------------------------------------------------------------------------------

void
LGABox::SetBoxHasBorder ( 	Boolean inHasBorder,
									Boolean inRedraw )
{
	if ( HasBorder () != inHasBorder )
	{
		mHasBorder = inHasBorder;
		
		// е Then we need to get the border redrawn, if requested
		if ( inRedraw && IsVisible () && FocusDraw ())
			RefreshBoxBorder ();
			
	}
	
}	//	LGABox::SetBoxHasBorder


//-------------------------------------------------------------------------------------
// LGABox::SetBorderStyle
//-------------------------------------------------------------------------------------

void
LGABox::SetBorderStyle ( 	EGABorderStyle inBorderStyle,
									Boolean inRedraw )
{
	if ( mBorderStyle != inBorderStyle )
	{
		mBorderStyle = inBorderStyle;
					
		// е Then we need to get the border redrawn, if requested
		if ( inRedraw && IsVisible () &&  FocusDraw ())
			RefreshBoxBorder ();
			
	}
	
}	//	LGABox::SetBorderStyle


//-------------------------------------------------------------------------------------
// LGABox::SetTitlePosition
//-------------------------------------------------------------------------------------

void
LGABox::SetTitlePosition ( 	EGATitlePosition inTitlePosition,
										Boolean inRedraw )
{
	if ( mTitlePosition != inTitlePosition )
	{
		mTitlePosition = inTitlePosition;
		
		// е Then we need to get the title redrawn in the new position, if requested
		if ( inRedraw )
			Refresh ();
			
	}
	
}	//	LGABox::SetTitlePosition


//-------------------------------------------------------------------------------------
// LGABox::SetContentOffset
//-------------------------------------------------------------------------------------

void
LGABox::SetContentOffset ( 	const Rect &inContentOffset,
									Boolean inRedraw )
{
	mContentOffset = inContentOffset;
	
	// е Then we need to get the content redrawn withthe new offsets, if requested
	if ( inRedraw && FocusDraw ())
		RefreshBoxContent ();
			
}	//	LGABox::SetContentOffset


//-------------------------------------------------------------------------------------
// LGABox::SetTextTraitsID
//-------------------------------------------------------------------------------------

void
LGABox::SetTextTraitsID	( 	ResIDT inTextTraitsID )
{

	LGATitleMixin::SetTextTraitsID ( inTextTraitsID );
		
	// е Get the box refreshed so we can see the change
	Refresh ();
	
}	//	LGABox::SetTextTraitsID


#pragma mark -
#pragma mark === ENABLING & DISABLING

//=====================================================================================
// ее ENABLING & DISABLING
//-------------------------------------------------------------------------------------
// LGABox::EnableSelf
//-------------------------------------------------------------------------------------
//

void
LGABox::EnableSelf ()
{

	// е Get things redrawn so that we can see the state change
	Refresh ();

}	//	LGABox::EnableSelf


//-------------------------------------------------------------------------------------
// LGABox::DisableSelf
//-------------------------------------------------------------------------------------
//

void
LGABox::DisableSelf ()
{

	// е Get things redrawn so that we can see the state change
	Refresh ();

}	//	LGABox::DisableSelf


#pragma mark -
#pragma mark === ACTIVATION

//=====================================================================================
// ее ACTIVATION
//-------------------------------------------------------------------------------------
// LGABox::ActivateSelf
//-------------------------------------------------------------------------------------

void
LGABox::ActivateSelf ()
{

	// е Get everything redrawn so the changes take effect
	if ( mActive == triState_On )
		Refresh ();
					
}	//	LGABox::ActivateSelf


//-------------------------------------------------------------------------------------
// LGABox::DeactivateSelf
//-------------------------------------------------------------------------------------

void
LGABox::DeactivateSelf ()
{

	// е Get everything redrawn so the changes take effect
	if ( mActive == triState_Off || mActive == triState_Latent )
		Refresh ();
				
}	//	LGABox::DeactivateSelf


#pragma mark -
#pragma mark === DRAWING

//=====================================================================================
// ее DRAWING
//------------------------------------------------------------------------------------
//	LGABox::DrawSelf
//------------------------------------------------------------------------------------
//

void
LGABox::DrawSelf ()
{
	
	StColorPenState::Normalize ();

	// е Get the border for the box drawn
	if ( HasBorder () )
		DrawBoxBorder ();
	
	// е Get the title drawn
	if ( HasTitle ())
		DrawBoxTitle ();
	
}	//	LGABox::DrawSelf


//------------------------------------------------------------------------------------
//	LGABox::DrawBoxTitle
//------------------------------------------------------------------------------------
//

void
LGABox::DrawBoxTitle ()
{
	StColorPenState	theColorPenState;
	theColorPenState.Normalize ();
	StTextState			theTextState;
	Rect 					titleRect;
	CalcTitleRect ( titleRect );
	
	// е Get the port setup with the text traits values
	UTextTraits::SetPortTextTraits ( GetTextTraitsID ());
	
	// е Get the box's title
	Str255 boxTitle;
	GetDescriptor ( boxTitle );

	// е Save off the text color as setup by the TextTrait
	RGBColor	textColor;
	::GetForeColor ( &textColor );
	
	// е Loop over any devices we might be spanning and handle the drawing
	// appropriately for each devices screen depth
	StDeviceLoop	theLoop ( titleRect );
	SInt16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		if ( depth < 4 )		//	е BLACK & WHITE
		{
			// е If the control is dimmed then we use the grayishTextOr 
			// transfer mode to draw the text
			if ( !IsEnabled ())
				::TextMode ( grayishTextOr );
		}
		else	// е COLOR
		{
			// е If the box is dimmed then we have to do our own version of the
			// grayishTextOr as it does not appear to work correctly across
			// multiple devices
			if ( !IsEnabled () || !IsActive ())
			{
				textColor = UGraphicsUtilities::Lighten ( &textColor );
				::TextMode ( srcOr );
				::RGBForeColor ( &textColor );
			}
		}	
			
		// е Get the actual text drawn with the appropriate justification
		UStringDrawing::DrawJustifiedString ( 	boxTitle,
															titleRect,
															teCenter );
	}
						
}	//	LGABox::DrawBoxTitle


//------------------------------------------------------------------------------------
//	LGABox::DrawBoxBorder
//------------------------------------------------------------------------------------
//

void
LGABox::DrawBoxBorder ()
{
	
	StColorPenState	theColorState;
	Rect					borderRect;
	CalcBorderRect ( borderRect );
	
	// е Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( borderRect );
	SInt16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		// е BLACK & WHITE
		if ( depth < 4 )
		{
			// е If we are on a black & white screen then we only use black to draw
			DrawBlackAndWhiteBorder ( 	borderRect,
												mBorderStyle );
		}
		// е COLOR
		else
		{
			// е Handle the drawing to a color screen
			DrawColorBorder ( borderRect,
									mBorderStyle );
		}
	}
	
}	//	LGABox::DrawBoxBorder


//-------------------------------------------------------------------------------------
// LGABox::DrawBlackAndWhiteBorder	
//-------------------------------------------------------------------------------------
//

void
LGABox::DrawBlackAndWhiteBorder ( 	const Rect &inBorderRect,
												EGABorderStyle /* inBorderStyle */ )
{
	StClipRgnState	theClipState;
	
	Rect	theRect = inBorderRect;

	// е If the box has a title we need to set up the clipping such that no border
	// will be drawn where the title is to be drawn
	if ( HasTitle ())
	{
		Rect 	titleRect;
		CalcTitleRect ( titleRect );
		if ( !::EmptyRect ( &titleRect ))
		{
			RgnHandle	titleRgn = ::NewRgn ();
			::RectRgn ( titleRgn, &titleRect );
			RgnHandle 	currClipRgn = ::NewRgn ();
			::GetClip ( currClipRgn );
			::DiffRgn ( currClipRgn, titleRgn, currClipRgn );
			::SetClip ( currClipRgn );
			::DisposeRgn ( titleRgn );
			::DisposeRgn ( currClipRgn );
		}
	}
		
	// е We only draw the border as a black frome around the border rect
	::PenNormal ();
	RGBColor theColor = UGAColorRamp::GetBlackColor();
	::RGBForeColor ( &theColor);
	::FrameRect ( &theRect );
	
}	// LGABox::DrawBlackAndWhiteBorder


//-------------------------------------------------------------------------------------
// LGABox::DrawColorBorder	
//-------------------------------------------------------------------------------------
//

void
LGABox::DrawColorBorder ( const Rect &inBorderRect,
								EGABorderStyle inBorderStyle )
{
	
	StClipRgnState	theClipState;
	
	Rect		theRect = inBorderRect;
	RGBColor colorArray[2];

	// е If the box has a title we need to set up the clipping such that no border
	// will be drawn where the title is to be drawn
	if ( HasTitle ())
	{
		Rect 	titleRect;
		CalcTitleRect ( titleRect );
		if ( !::EmptyRect ( &titleRect ))
		{
			RgnHandle	titleRgn = ::NewRgn ();
			::RectRgn ( titleRgn, &titleRect );
			RgnHandle 	currClipRgn = ::NewRgn ();
			::GetClip ( currClipRgn );
			::DiffRgn ( currClipRgn, titleRgn, currClipRgn );
			::SetClip ( currClipRgn );
			::DisposeRgn ( titleRgn );
			::DisposeRgn ( currClipRgn );
		}
	}
		
	// е Setup the colors for the border
	RGBColor borderDarkColor = IsEnabled () && IsActive () ? UGAColorRamp::GetColor(7) : 
														UGAColorRamp::GetColor(4);
	RGBColor borderLightColor = IsEnabled () && IsActive () ? UGAColorRamp::GetWhiteColor() : 
														UGAColorRamp::GetColor(1);

	// е Start off with a clean pen
	::PenNormal ();

	// е ENABLED
	if ( IsEnabled () && IsActive ())
	{
		switch ( inBorderStyle )
		{
			case borderStyleGA_PlainBorder:
				RGBColor borderColor = UGAColorRamp::GetBlackColor();
				::RGBForeColor ( &borderColor );
				::FrameRect ( &theRect );
			break;
			
			case borderStyleGA_BezelBorder:			
				// е Frame the light portion of the border
				::RGBForeColor ( &borderLightColor );
				theRect.right -= 1;
				theRect.bottom -= 1;
				::OffsetRect ( &theRect, 1, 1 );
				::FrameRect ( &theRect );
				::OffsetRect ( &theRect, -1, -1 );

				// е Frame the dark portion of the border
				::RGBForeColor ( &borderDarkColor );
				::FrameRect ( &theRect );
				
				// е Fix upsome pixels
				UGraphicsUtilities::PaintColorPixel ( theRect.left, theRect.bottom, &borderLightColor );
				UGraphicsUtilities::PaintColorPixel ( theRect.right, theRect.top, &borderLightColor );
				
			break;
			
			case borderStyleGA_RecessedOneBorder:
				// е Frame the dark portion of the border
				::RGBForeColor ( &borderDarkColor );
				UGraphicsUtilities::TopLeftSide ( &theRect, 0, 0, 1, 1 );
			
				// е Frame the light portion of the border
				::RGBForeColor ( &borderLightColor );
				UGraphicsUtilities::BottomRightSide ( &theRect, 1, 1, 0, 0 );

			break;
			
			case borderStyleGA_RecessedTwoBorder:
				// е Frame the dark portion of the border
				colorArray[0] = borderDarkColor;
				colorArray[1] = borderDarkColor;
				UGraphicsUtilities::BevelledEdge ( &theRect, rectEdge_TopLeft, colorArray, 2 );

				// е Frame the light portion of the border
				colorArray[0] = borderLightColor;
				colorArray[1] = borderLightColor;
				UGraphicsUtilities::BevelledEdge ( &theRect, rectEdge_BottomRight, colorArray, 2 );

			break;
			
			case borderStyleGA_EmbossedOneBorder:
				// е Frame the dark portion of the border
				::RGBForeColor ( &borderLightColor );
				UGraphicsUtilities::TopLeftSide ( &theRect, 0, 0, 1, 1 );
			
				// е Frame the light portion of the border
				::RGBForeColor ( &borderDarkColor );
				UGraphicsUtilities::BottomRightSide ( &theRect, 1, 1, 0, 0 );

			break;
			
			case borderStyleGA_EmbossedTwoBorder:
				// е Frame the dark portion of the border
				colorArray[0] = borderLightColor;
				colorArray[1] = borderLightColor;
				UGraphicsUtilities::BevelledEdge ( &theRect, rectEdge_TopLeft, colorArray, 2 );

				// е Frame the light portion of the border
				colorArray[0] = borderDarkColor;
				colorArray[1] = borderDarkColor;
				UGraphicsUtilities::BevelledEdge ( &theRect, rectEdge_BottomRight, colorArray, 2 );

			break;
			
			default:
			break;
			
		}
	}
	else	//	е DISABLED
	{
		RGBColor borderColor = UGAColorRamp::GetColor(4);
		switch ( inBorderStyle )
		{
			case borderStyleGA_PlainBorder:
			case borderStyleGA_BezelBorder:			
				::RGBForeColor ( &borderColor );
				::FrameRect ( &theRect );
			break;
			
			case borderStyleGA_RecessedOneBorder:
			case borderStyleGA_RecessedTwoBorder:
			case borderStyleGA_EmbossedOneBorder:
			case borderStyleGA_EmbossedTwoBorder:
				// е Frame the border
				::RGBForeColor ( &borderColor );
				UGraphicsUtilities::TopLeftSide ( &theRect, 0, 0, 1, 1 );
				UGraphicsUtilities::BottomRightSide ( &theRect, 1, 1, 0, 0 );
			break;
			
			default:
			break;
		}
	}
	
}	// LGABox::DrawColorBorder


//------------------------------------------------------------------------------------
//	LGABox::RefreshBoxBorder
//------------------------------------------------------------------------------------
//
//	Invalidate the border area of the box so it will 
//	get redrawn during the next update

void
LGABox::RefreshBoxBorder ()
{
	// е We are only going to worry about doing a refresh if the box 
	// is currently visible and it has a frame and a super view
	Rect	theFrame;
	if ( IsVisible ()  && CalcPortFrameRect ( theFrame )  && ( mSuperView )) 
	{
		
		Rect	revealed;
		mSuperView->GetRevealedRect ( revealed );
		if ( ::SectRect ( &theFrame, &revealed, &revealed )) 
		{
			
			// е Get the content region figured out so that we can use it for the inval
			RgnHandle	borderRgn = GetBoxBorderRegion ( revealed );
			ThrowIfNil_ ( borderRgn );
			
			// е Get the border invalidated so that it will be refreshed
			InvalPortRgn(borderRgn);
//			::InvalRgn ( borderRgn );
			
			// е Cleanup
			::DisposeRgn ( borderRgn );
		}
	}
}	//	LGABox::RefreshBoxBorder


//------------------------------------------------------------------------------------
//	LGABox::DontRefreshBoxBorder
//------------------------------------------------------------------------------------
//
//	Validate the area occupied by the border of the banner box so it won't 
//	get redrawn during the next update

void
LGABox::DontRefreshBoxBorder (	Boolean	inOKIfHidden )
{
	// е We are only going to worry about doing a refresh if the banner box 
	// is currently visible and it has a frame and a super view
	Rect	theFrame;
	if (( IsVisible() || inOKIfHidden )  && 
					CalcPortFrameRect ( theFrame )  && ( mSuperView )) 
	{

		Rect	revealed;
		mSuperView->GetRevealedRect ( revealed );
		if ( ::SectRect ( &theFrame, &revealed, &revealed )) 
		{
			RgnHandle	borderRgn = GetBoxBorderRegion ( revealed );
			ValidPortRgn ( borderRgn );
			::DisposeRgn ( borderRgn );
		}
	}
}	//	LGABox::DontRefreshBoxBorder


//------------------------------------------------------------------------------------
//	LGABox::RefreshBoxContent
//------------------------------------------------------------------------------------
//
//	Invalidate the content area of the box so it will 
//	get redrawn during the next update

void
LGABox::RefreshBoxContent ()
{
	// е We are only going to worry about doing a refresh if the box 
	// is currently visible and it has a frame and a super view
	Rect	theFrame;
	if ( IsVisible ()  && CalcPortFrameRect ( theFrame )  && ( mSuperView )) 
	{

		Rect	revealed;
		mSuperView->GetRevealedRect ( revealed );
		if ( ::SectRect ( &theFrame, &revealed, &revealed )) 
		{
			// е Get the content region figured out so that we can use it for the inval
			RgnHandle	contentRgn = GetBoxContentRegion ( revealed );
			ThrowIfNil_ ( contentRgn );
					
			// е Get the content invalidated so that it will be refreshed
			InvalPortRgn(contentRgn);
//			::InvalRgn ( contentRgn );
			
			// е Cleanup
			::DisposeRgn ( contentRgn );
		}
	}
	
}	//	LGABox::RefreshBoxContent


//------------------------------------------------------------------------------------
//	LGABox::RefreshBoxTitle
//------------------------------------------------------------------------------------
//
//	Invalidate the title area of the box so it will get redrawn during the next update

void
LGABox::RefreshBoxTitle ()
{
	// е We are only going to worry about doing a refresh if the box 
	// is currently visible and it has a frame and a super view
	Rect	theFrame;
	if ( IsVisible ()  && CalcPortFrameRect ( theFrame )  && ( mSuperView )) 
	{

		Rect	revealed;
		mSuperView->GetRevealedRect ( revealed );
		if ( ::SectRect ( &theFrame, &revealed, &revealed )) 
		{
			// е Before we do anything make sure the rect is up to date
			Rect titleRect;
			CalcTitleRect ( titleRect );
	
			// е Get the title region figured out so that we can use it for the inval
			RgnHandle	titleRgn = ::NewRgn ();
			::RectRgn ( titleRgn, &titleRect );
			
			// е Get the title invalidated so that it will be refreshed
			InvalPortRgn(titleRgn);
//			::InvalRgn ( titleRgn );
			
			// е Cleanup
			::DisposeRgn ( titleRgn );
		}
	}
}	//	LGABox::RefreshBoxTitle


#pragma mark -
#pragma mark === RECTANGLE CALCULATIONS

//=====================================================================================
// ее RECTANGLE CALCULATIONS
//------------------------------------------------------------------------------------
//	LGABox::CalculateTitleHeightAndWidth
//------------------------------------------------------------------------------------
//

void
LGABox::CalculateTitleHeightAndWidth ( SInt16 *outHeight,
													SInt16 *outWidth )
{

	StTextState			theTextState;
	
	// е Get the port setup too the text traits values
	 UTextTraits::SetPortTextTraits ( GetTextTraitsID ());
	
	// е Get the font info
	FontInfo	fi;
	::GetFontInfo ( &fi );
	
	// е Setup some string data
	*outWidth = ::StringWidth ( mTitle );
	*outHeight = fi.ascent + fi.descent + fi.leading;

}	//	LGABox::CalculateDefaultTitleRect


//------------------------------------------------------------------------------------
//	LGABox::CalculateDefaultTitleRect
//------------------------------------------------------------------------------------
//

void
LGABox::CalculateDefaultTitleRect ( Rect	&outRect )
{

	SInt16					width;
	SInt16					height;
	
	// е Get the height and width of the title
	CalculateTitleHeightAndWidth ( &height, &width );
	
	// е Get the frame for the box
	CalcLocalFrameRect ( outRect );

	// е Now get the rectangle tweaked to accomodate the title
	SInt16	frameWidth = ::UGraphicsUtilities::RectWidth (outRect);
	if ( width > (frameWidth - 26) )
		width = frameWidth - 32;

	outRect.left += 10;
	outRect.bottom = outRect.top + height;
	outRect.right = outRect.left + width + 6;
	
}	//	LGABox::CalculateDefaultTitleRect


//------------------------------------------------------------------------------------
//	LGABox::CalculateTopRightTitleRect
//------------------------------------------------------------------------------------
//

void
LGABox::CalculateTopRightTitleRect ( Rect	&outRect )
{

	SInt16					width;
	SInt16					height;
	
	// е Get the height and width of the title
	CalculateTitleHeightAndWidth ( &height, &width );
	
	// е Get the frame for the box
	CalcLocalFrameRect ( outRect );

	// е Now get the rectangle tweaked to accomodate the title
	SInt16	frameWidth = ::UGraphicsUtilities::RectWidth (outRect);
	if ( width > (frameWidth - 26) )
		width = frameWidth - 32;

	outRect.right -= 10;
	outRect.left = outRect.right - ( width + 6 );
	outRect.bottom = outRect.top + height;
	
}	//	LGABox::CalculateTopRightTitleRect


//------------------------------------------------------------------------------------
//	LGABox::CalculateTopLeftOutsideTitleRect
//------------------------------------------------------------------------------------
//

void
LGABox::CalculateTopLeftOutsideTitleRect ( Rect	&outRect )
{

	SInt16					width;
	SInt16					height;
	
	// е Get the height and width of the title
	CalculateTitleHeightAndWidth ( &height, &width );
	
	// е Get the frame for the box
	CalcLocalFrameRect ( outRect );

	// е Now get the rectangle tweaked to accomodate the title
	SInt16	frameWidth = ::UGraphicsUtilities::RectWidth (outRect);
	if ( width > (frameWidth - 3) )
		width = frameWidth - 3;

	outRect.left += 1;
	outRect.bottom = outRect.top + height;
	outRect.right = outRect.left + width + 2;
	
}	//	LGABox::CalculateTopLeftOutsideTitleRect


//------------------------------------------------------------------------------------
//	LGABox::CalculateTopRightOutsideTitleRect
//------------------------------------------------------------------------------------
//

void
LGABox::CalculateTopRightOutsideTitleRect ( Rect	&outRect )
{

	SInt16					width;
	SInt16					height;
	
	// е Get the height and width of the title
	CalculateTitleHeightAndWidth ( &height, &width );
	
	// е Get the frame for the box
	CalcLocalFrameRect ( outRect );

	// е Now get the rectangle tweaked to accomodate the title
	SInt16	frameWidth = ::UGraphicsUtilities::RectWidth (outRect);
	if ( width > (frameWidth - 3) )
		width = frameWidth - 3;

	outRect.left = outRect.right - width - 2;
	outRect.bottom = outRect.top + height;
	outRect.right = outRect.left + width + 2;
	
}	//	LGABox::CalculateTopRightOutsideTitleRect


//------------------------------------------------------------------------------------
//	LGABox::CalculateCenterTopOutsideTitleRect
//------------------------------------------------------------------------------------
//

void
LGABox::CalculateCenterTopOutsideTitleRect ( Rect	&outRect )
{

	SInt16					width;
	SInt16					height;
	
	// е Get the height and width of the title
	CalculateTitleHeightAndWidth ( &height, &width );
	
	// е Get the frame for the box
	CalcLocalFrameRect ( outRect );

	// е Now get the rectangle tweaked to accomodate the title
	SInt16	frameWidth = ::UGraphicsUtilities::RectWidth (outRect);
	if ( width > (frameWidth - 9) )
		width = frameWidth - 9;
	
	SInt16	halfWidth = ( outRect.right - outRect.left ) / 2;
	SInt16	halfTitleWidth = width / 2;
	
	outRect.left += ( halfWidth - halfTitleWidth ) - 3;
	outRect.bottom = outRect.top + height;
	outRect.right = outRect.left + width + 6;
	
}	//	LGABox::CalculateCenterTopOutsideTitleRect


//------------------------------------------------------------------------------------
//	LGABox::CalculateCenterTopTitleRect
//------------------------------------------------------------------------------------
//

void
LGABox::CalculateCenterTopTitleRect ( Rect	&outRect )
{

	SInt16					width;
	SInt16					height;
	
	// е Get the height and width of the title
	CalculateTitleHeightAndWidth ( &height, &width );
	
	// е Get the frame for the box
	CalcLocalFrameRect ( outRect );

	// е Now get the rectangle tweaked to accomodate the title
	SInt16	frameWidth = ::UGraphicsUtilities::RectWidth (outRect);
	if ( width > (frameWidth - 26) )
		width = frameWidth - 32;
	
	SInt16	halfWidth = ( outRect.right - outRect.left ) / 2;
	SInt16	halfTitleWidth = width / 2;
	
	outRect.left += ( halfWidth - halfTitleWidth ) - 3;
	outRect.bottom = outRect.top + height;
	outRect.right = outRect.left + width + 6;
	
}	//	LGABox::CalculateCenterTopTitleRect


//------------------------------------------------------------------------------------
//	LGABox::CalculateCenterTopInsideTitleRect
//------------------------------------------------------------------------------------
//

void
LGABox::CalculateCenterTopInsideTitleRect ( Rect	&outRect )
{

	SInt16					width;
	SInt16					height;
	
	// е Get the height and width of the title
	CalculateTitleHeightAndWidth ( &height, &width );
	
	// е Get the frame for the box
	CalcLocalFrameRect ( outRect );

	// е Get the offsets
	Rect	offsets = GetContentOffset ();
	
	// е Now get the rectangle tweaked to accomodate the title
	SInt16	frameWidth = ::UGraphicsUtilities::RectWidth (outRect);
	if ( width > (frameWidth - (offsets.left + offsets.right)) )
		width = frameWidth - ((offsets.left + offsets.right) + 6);
	
	SInt16	halfWidth = ( outRect.right - outRect.left ) / 2;
	SInt16	halfTitleWidth = width / 2;
	
	outRect.top += 2;
	outRect.left += ( halfWidth - halfTitleWidth ) - 3;
	outRect.bottom = outRect.top + height;
	outRect.right = outRect.left + width + 6;
	
}	//	LGABox::CalculateCenterTopInsideTitleRect


//------------------------------------------------------------------------------------
//	LGABox::CalculateCenterInsideTitleRect
//------------------------------------------------------------------------------------
//

void
LGABox::CalculateCenterInsideTitleRect ( Rect	&outRect )
{

	SInt16					width;
	SInt16					height;
	
	// е Get the height and width of the title
	CalculateTitleHeightAndWidth ( &height, &width );
	
	// е Get the frame for the box
	CalcLocalFrameRect ( outRect );

	// е Get the offsets
	Rect	offsets = GetContentOffset ();
	
	// е Now get the rectangle tweaked to accomodate the title
	SInt16	frameWidth = ::UGraphicsUtilities::RectWidth (outRect);
	if ( width > (frameWidth - (offsets.left + offsets.right)) )
		width = frameWidth - ((offsets.left + offsets.right) + 6);
	
	SInt16	halfWidth = ( outRect.right - outRect.left ) / 2;
	SInt16	halfTitleWidth = width / 2;
	
	outRect.top += ( outRect.bottom - outRect.top ) / 2 - height / 2;
	outRect.left += ( halfWidth - halfTitleWidth ) - 3;
	outRect.bottom = outRect.top + height;
	outRect.right = outRect.left + width + 6;
	
}	//	LGABox::CalculateCenterInsideTitleRect


//------------------------------------------------------------------------------------
//	LGABox::CalculateCenterBottomInsideTitleRect
//------------------------------------------------------------------------------------
//

void
LGABox::CalculateCenterBottomInsideTitleRect ( Rect	&outRect )
{

	SInt16					width;
	SInt16					height;
	
	// е Get the height and width of the title
	CalculateTitleHeightAndWidth ( &height, &width );
	
	// е Get the frame for the box
	CalcLocalFrameRect ( outRect );

	// е Get the offsets
	Rect	offsets = GetContentOffset ();
	
	// е Now get the rectangle tweaked to accomodate the title
	SInt16	frameWidth = ::UGraphicsUtilities::RectWidth (outRect);
	if ( width > (frameWidth - (offsets.left + offsets.right)) )
		width = frameWidth - ((offsets.left + offsets.right) + 6);
	
	SInt16	halfWidth = ( outRect.right - outRect.left ) / 2;
	SInt16	halfTitleWidth = width / 2;
	
	outRect.top += ( outRect.bottom - outRect.top ) - height - 3;
	outRect.left += ( halfWidth - halfTitleWidth ) - 3;
	outRect.bottom = outRect.top + height;
	outRect.right = outRect.left + width + 6;
	
}	//	LGABox::CalculateCenterBottomInsideTitleRect


//------------------------------------------------------------------------------------
//	LGABox::CalculateCenterBottomTitleRect
//------------------------------------------------------------------------------------
//

void
LGABox::CalculateCenterBottomTitleRect ( Rect	&outRect )
{

	SInt16					width;
	SInt16					height;
	
	// е Get the height and width of the title
	CalculateTitleHeightAndWidth ( &height, &width );
	
	// е Get the frame for the box
	CalcLocalFrameRect ( outRect );

	// е Now get the rectangle tweaked to accomodate the title
	SInt16	frameWidth = ::UGraphicsUtilities::RectWidth (outRect);
	if ( width > (frameWidth - 26) )
		width = frameWidth - 32;
	
	SInt16	halfWidth = ( outRect.right - outRect.left ) / 2;
	SInt16	halfTitleWidth = width / 2;
	
	outRect.top += ( outRect.bottom - outRect.top ) - height;
	outRect.left += ( halfWidth - halfTitleWidth ) - 3;
	outRect.bottom = outRect.top + height;
	outRect.right = outRect.left + width + 6;
	
}	//	LGABox::CalculateCenterBottomTitleRect


//------------------------------------------------------------------------------------
//	LGABox::CalculateCenterBottomOutsideTitleRect
//------------------------------------------------------------------------------------
//

void
LGABox::CalculateCenterBottomOutsideTitleRect ( Rect	&outRect )
{

	SInt16					width;
	SInt16					height;
	
	// е Get the height and width of the title
	CalculateTitleHeightAndWidth ( &height, &width );
	
	// е Get the frame for the box
	CalcLocalFrameRect ( outRect );

	// е Now get the rectangle tweaked to accomodate the title
	SInt16	frameWidth = ::UGraphicsUtilities::RectWidth (outRect);
	if ( width > (frameWidth - 9) )
		width = frameWidth - 9;
	
	SInt16	halfWidth = ( outRect.right - outRect.left ) / 2;
	SInt16	halfTitleWidth = width / 2;
	
	outRect.top += ( outRect.bottom - outRect.top ) - height;
	outRect.left += ( halfWidth - halfTitleWidth ) - 3;
	outRect.bottom = outRect.top + height;
	outRect.right = outRect.left + width + 6;
	
}	//	LGABox::CalculateCenterBottomOutsideTitleRect


