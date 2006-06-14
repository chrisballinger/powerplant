// Copyright й2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyrightй 1995 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< е >>-----------------------------------------

FILE:					LGAPrimaryBox.cp

CLASSES:				LGAPrimaryBox

DESCRIPTION:		Implementation file for LGAPrimaryBox

AUTHOR:				Robin Mair

CREATION DATE:		95.12.19

CHANGE HISTORY:

		97.04.20		rtm	Removed the CreateFromStream method.
		97.02.08		rtm	Changed the rendering of the control such that is it drawn as
								disabled when it is deactivated, this is matches the approach
								Apple is taking with the Apearance Manager.
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
								correctly, CalcTitleRect was not correctly handling the check
								of the title width against the box width.  It now handles 
								this correctly such that there is always at least 10 pixels 
								of border on either end of the title.
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
		96.10.20		rtm	Fixed a problem where the border was drawing incorrectly in
								B&W when spanning devices, this was caused by the color drawing
								code modifying the size of the border rect which when drawing
								in B&W resulted in the border being drawn offset by one pixel.
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
		96.10.08		rtm	Added an override to MoveBy so that the box rects will get
								recalculated.
		96.10.04		rtm	Replaced all SetCPixel calls with a new PaintColorPixel call
								that has been added to UGraphicsUtilities. This should get
								around the problems with using SetCPixel.
		96.09.19		rtm	Changed the title drawing method so that the actual call to 
								draw the text is inside the while loop used to check the
								screen depth.
		96.09.16		rtm	Changed the title accessors to conform to the PowerPlant API 
								provided by LPane.  Also added a recalulate box rects call to
								the text traits setter.		
		96.09.12		rtm	Changed the title drawing method so that it always centers the
								title in the title rect regardless of the justification of the
								associated text trait.
		96.09.12		rtm	Fixed a problem where the border was being calculated incorrectly
								based on the font being used for the title.
		96.09.12		rtm	Fixed a bug where the border was being calculated inset by one 
								pixel when it did not need to be.
		96.09.04		rtm	PowerPlant conformance changes.
		96.09.02		rtm	Added overrides of EnableSelf and DisableSelf so that we can
								handle getting the control redrawn when the state changes.
		96.09.02		rtm	Changed the rendering of the dimmed state of the box to
								meet Apple's modified spec for grayscale appearance.
		96.06.26		rtm	Change the border drawing routine so that the clipping is
								now set that the border is not drawn where the title would be
								located, if there is one.
		96.06.03		rtm	Built a specific primary box that is preset to the required
								settings.
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
		95.12.19		rtm	Completed implementation of LGAPrimaryBox class with all of the 
								drawing permutations working.
		95.12.19		rtm	Initial creation of file

---------------------------------------<< е >>-----------------------------------------
*/

#ifdef PowerPlant_PCH
	#include PowerPlant_PCH
#endif

// е GRAYSCALE HEADERS
#include <LGAPrimaryBox.h>
#include <UGraphicsUtilities.h>
#include <UGAColorRamp.h>

// е POWERPLANT HEADERS
#include <LStream.h>
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


#pragma mark === CLASS: LGAPrimaryBox

//=====================================================================================
// CLASS:	LGAPrimaryBox
//=====================================================================================

#pragma mark -
#pragma mark === INITIALIZATION

//=====================================================================================
// ее INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LGAPrimaryBox::LGAPrimaryBox					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Default constructor

LGAPrimaryBox::LGAPrimaryBox ()
{

	// е Get all of our fields initialized
	mContentOffset = gEmptyRect;

}	//	LGAPrimaryBox::LGAPrimaryBox


//-------------------------------------------------------------------------------------
// LGAPrimaryBox::LGAPrimaryBox					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Copy constructor

LGAPrimaryBox::LGAPrimaryBox 	(	const LGAPrimaryBox &inOriginal )
							:	LView ( inOriginal ),
								LGATitleMixin ( inOriginal )
{

	mContentOffset = inOriginal.mContentOffset;

}	//	LGAPrimaryBox::LGAPrimaryBox


//-------------------------------------------------------------------------------------
// LGAPrimaryBox::LGAPrimaryBox					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Stream constructor

LGAPrimaryBox::LGAPrimaryBox (	LStream	*inStream ) : LView ( inStream )
{

	// е Read in the data from the stream
	inStream->ReadPString ( mTitle );
	*inStream >> mTextTraitsID;
	
	// е We will set up the offset with this default as there is no way to
	// handle custom pane type rect fields
	Rect r = { 4, 4, 4, 4 };
	mContentOffset = r;

}	//	LGAPrimaryBox::LGAPrimaryBox


//-------------------------------------------------------------------------------------
// LGAPrimaryBox::LGAPrimaryBox					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Parameterized constructor

LGAPrimaryBox::LGAPrimaryBox	(	const SPaneInfo	&inPaneInfo,
											const SViewInfo	&inViewInfo,
											Str255				inTitle,
											ResIDT				inTextTraitsID,
											Rect					&inContentOffset )
					: LView ( inPaneInfo, inViewInfo )
{

	// е Setup the classes fields
	mTitle = inTitle;
	mTextTraitsID = inTextTraitsID;
	mContentOffset = inContentOffset;

}	//	LGAPrimaryBox::LGAPrimaryBox


//-------------------------------------------------------------------------------------
// LGAPrimaryBox::~LGAPrimaryBox				---<<е DESTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LGAPrimaryBox::~LGAPrimaryBox ()
{

}	//	LGAPrimaryBox::~LGAPrimaryBox


#pragma mark -
#pragma mark === ACCESSORS

//=====================================================================================
// ее ACCESSORS
//-------------------------------------------------------------------------------------
// LGAPrimaryBox::GetDescriptor
//-------------------------------------------------------------------------------------

StringPtr
LGAPrimaryBox::GetDescriptor ( Str255	outTitle ) const
{

	return LString::CopyPStr ( mTitle, outTitle );

}	//	LGAPrimaryBox::GetDescriptor


//-------------------------------------------------------------------------------------
// LGAPrimaryBox::GetBoxBorderRegion
//-------------------------------------------------------------------------------------

RgnHandle
LGAPrimaryBox::GetBoxBorderRegion	(	const Rect	&inRevealed )
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
	
	// е Clip the region using the revealed rect
	RgnHandle	revealedRgn = ::NewRgn ();
	PortToLocalPoint ( topLeft ( inRevealed ));
	PortToLocalPoint ( botRight ( inRevealed ));
	::RectRgn ( revealedRgn, &inRevealed );
	::SectRgn ( borderRgn, revealedRgn, borderRgn );
	::DisposeRgn ( revealedRgn );

	// е Return the region forming the border portion of the banner box
	return borderRgn;
	
}	//	LGAPrimaryBox::GetBoxBorderRegion



//-------------------------------------------------------------------------------------
// LGAPrimaryBox::GetBoxContentRegion
//-------------------------------------------------------------------------------------

RgnHandle
LGAPrimaryBox::GetBoxContentRegion	(	const Rect	&inRevealed )
{

	// е Create a new region
	Rect	contentRect;
	CalcContentRect ( contentRect );
	RgnHandle	contentRgn = ::NewRgn ();
	::RectRgn ( contentRgn, &contentRect );
	
	// е Clip the region using the revealed rect
	RgnHandle	revealedRgn = ::NewRgn ();
	PortToLocalPoint ( topLeft ( inRevealed ));
	PortToLocalPoint ( botRight ( inRevealed ));
	::RectRgn ( revealedRgn, &inRevealed );
	::SectRgn ( contentRgn, revealedRgn, contentRgn );
	::DisposeRgn ( revealedRgn );
	
	// е Return the region forming the border portion of the banner box
	return contentRgn;
	
}	//	LGAPrimaryBox::GetBoxContentRegion


//------------------------------------------------------------------------------------
//	LGAPrimaryBox::CalcTitleRect
//------------------------------------------------------------------------------------
//

void
LGAPrimaryBox::CalcTitleRect (	Rect &outRect )
{

	SInt16					width;
	SInt16					height;
	
	// е Get the height and width of the title
	CalculateTitleHeightAndWidth ( &height, &width );
	
	// е Get the frame for the box
	CalcLocalFrameRect ( outRect );
	
	// е Now get the rectangle tweaked to accomodate the title
	SInt16	frameWidth = ::UGraphicsUtilities::RectWidth ( outRect );
	if ( width > (frameWidth - 26) )
		width = frameWidth - 32;
	
	outRect.left += 10;
	outRect.bottom = outRect.top + height;
	outRect.right = outRect.left + width + 6;
	
}	//	LGAPrimaryBox::CalcTitleRect


//------------------------------------------------------------------------------------
//	LGAPrimaryBox::CalcContentRect
//------------------------------------------------------------------------------------
//

void
LGAPrimaryBox::CalcContentRect ( Rect	&outRect )
{

	// е Start by setting the rect to the border rect
	CalcBorderRect ( outRect );
 	
 	// е Get the content offset rect
 	Rect offsets = GetContentOffset ();
 	
  	// е Adjust the rectangle
	// е TOP
	if ( HasTitle ())
		outRect.top += 2 + offsets.top;
	else
		outRect.top += offsets.top;
		
	// е LEFT
	outRect.left += offsets.left;
	
	// е BOTTOM
	outRect.bottom -= offsets.bottom;
	
	// е RIGHT
	outRect.right -= offsets.right;
  
}	//	LGAPrimaryBox::CalcContentRect

//------------------------------------------------------------------------------------
//	LGAPrimaryBox::CalcBorderRect
//------------------------------------------------------------------------------------
//

void
LGAPrimaryBox::CalcBorderRect ( Rect	&outRect )
{

	StTextState			theTextState;
	
	// е Get the port setup too the text traits values
	UTextTraits::SetPortTextTraits ( GetTextTraitsID ());
	
	// е Get the font info
	FontInfo	fi;
	::GetFontInfo ( &fi );
	
	// е Get the frame for the box
	CalcLocalFrameRect ( outRect );

	// е Setup some values based on whether there is a title or not
	SInt16	offsetTitleHeight = HasTitle () ? fi.ascent - 2: 0;
	
	// е Calculate the border rect by offsetting its top to accomodate
	// the title, if needed
	if ( offsetTitleHeight > 0 )
		outRect.top += offsetTitleHeight;

}	//	LGAPrimaryBox::CalculateBorderRect


//-------------------------------------------------------------------------------------
// LGAPrimaryBox::SetDescriptor
//-------------------------------------------------------------------------------------

void
LGAPrimaryBox::SetDescriptor ( ConstStr255Param	inDescriptor )
{

	// е Only change the title if we have to
	if ( mTitle != inDescriptor )
	{
		mTitle = inDescriptor;
		
		// е Now do a  refresh so that the new title shows up
		Refresh ();
			
	}
	
}	//	LGAPrimaryBox::SetDescriptor


//-------------------------------------------------------------------------------------
// LGAPrimaryBox::SetContentOffset
//-------------------------------------------------------------------------------------

void
LGAPrimaryBox::SetContentOffset ( 	const Rect &inContentOffset,
									Boolean inRedraw )
{
	mContentOffset = inContentOffset;
	
	// е Then we need to get the content redrawn withthe new offsets, if requested
	if ( inRedraw && FocusDraw ())
		RefreshBoxContent ();
			
}	//	LGAPrimaryBox::SetContentOffset


//-------------------------------------------------------------------------------------
// LGAPrimaryBox::SetTextTraitsID
//-------------------------------------------------------------------------------------

void
LGAPrimaryBox::SetTextTraitsID	( 	ResIDT inTextTraitsID )
{

	// е Call our superclass to handle the setting of the ID
	LGATitleMixin::SetTextTraitsID ( inTextTraitsID );
		
	// е We need to make sure we redraw the box when the text 
	// trait ID changes we have to do a complete refresh because
	// the change in the text trait might result in the border
	// needing to be redrawn to accomodate the title
	Refresh ();
	
}	//	LGAPrimaryBox::SetTextTraitsID


#pragma mark -
#pragma mark === ENABLING & DISABLING

//=====================================================================================
// ее ENABLING & DISABLING
//-------------------------------------------------------------------------------------
// LGAPrimaryBox::EnableSelf
//-------------------------------------------------------------------------------------
//

void
LGAPrimaryBox::EnableSelf ()
{

	// е Get things redrawn so that we can see the state change
	Refresh ();

}	//	LGAPrimaryBox::EnableSelf


//-------------------------------------------------------------------------------------
// LGAPrimaryBox::DisableSelf
//-------------------------------------------------------------------------------------
//

void
LGAPrimaryBox::DisableSelf ()
{

	// е Get things redrawn so that we can see the state change
	Refresh ();

}	//	LGAPrimaryBox::DisableSelf


#pragma mark -
#pragma mark === ACTIVATION

//=====================================================================================
// ее ACTIVATION
//-------------------------------------------------------------------------------------
// LGAPrimaryBox::ActivateSelf
//-------------------------------------------------------------------------------------

void
LGAPrimaryBox::ActivateSelf ()
{

	// е Get everything redrawn so the changes take effect
	if ( mActive == triState_On )
		Refresh ();
					
}	//	LGAPrimaryBox::ActivateSelf


//-------------------------------------------------------------------------------------
// LGAPrimaryBox::DeactivateSelf
//-------------------------------------------------------------------------------------

void
LGAPrimaryBox::DeactivateSelf ()
{

	// е Get everything redrawn so the changes take effect
	if ( mActive == triState_Off || mActive == triState_Latent )
		Refresh ();
				
}	//	LGAPrimaryBox::DeactivateSelf


#pragma mark -
#pragma mark === DRAWING

//====================================================================================
// ее DRAWING
//------------------------------------------------------------------------------------
//	LGAPrimaryBox::DrawSelf
//------------------------------------------------------------------------------------
//

void
LGAPrimaryBox::DrawSelf ()
{
	
	// е Get the border for the box drawn
	DrawBoxBorder ();
	
	// е Get the title drawn
	if ( HasTitle () )
		DrawBoxTitle ();
	
}	//	LGAPrimaryBox::DrawSelf


//------------------------------------------------------------------------------------
//	LGAPrimaryBox::DrawBoxTitle
//------------------------------------------------------------------------------------
//

void
LGAPrimaryBox::DrawBoxTitle ()
{
	StColorPenState	theColorPenState;
	theColorPenState.Normalize ();
	StTextState			theTextState;
	Rect 					titleRect;
	CalcTitleRect ( titleRect );
	
	// е Get the port setup too the text traits values
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
			// е If the control is dimmed then we have to do our own version of the
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
	
}	//	LGAPrimaryBox::DrawBoxTitle


//------------------------------------------------------------------------------------
//	LGAPrimaryBox::DrawBoxBorder
//------------------------------------------------------------------------------------
//

void
LGAPrimaryBox::DrawBoxBorder ()
{
	
	StColorPenState	theColorPenState;
	theColorPenState.Normalize ();
	StClipRgnState		theClipState;
	Rect					borderRect;
	CalcBorderRect ( borderRect );
	
	// е Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( borderRect );
	SInt16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		// е If the box has a title we need to set up the clipping such that no border
		// will be drawn where the title is to be drawn
		if ( HasTitle ())
		{
			Rect 	titleRect;
			CalcTitleRect ( titleRect );
			RgnHandle	titleRgn = ::NewRgn ();
			::RectRgn ( titleRgn, &titleRect );
			RgnHandle 	currClipRgn = ::NewRgn ();
			::GetClip ( currClipRgn );
			::DiffRgn ( currClipRgn, titleRgn, currClipRgn );
			::SetClip ( currClipRgn );
			::DisposeRgn ( titleRgn );
			::DisposeRgn ( currClipRgn );
		}
		
		// е BLACK & WHITE
		if ( depth < 4 )
		{
			// е We only draw the border as a black frome around the border rect
			::PenNormal ();
			RGBColor theColor = UGAColorRamp::GetBlackColor();
			::RGBForeColor ( &theColor);
			::FrameRect ( &borderRect );
		}
		// е COLOR
		else
		{
			// е Setup the colors for the border
			RGBColor borderDarkColor = IsEnabled () && IsActive () ? 
									UGAColorRamp::GetColor(7) : UGAColorRamp::GetColor(4);
			RGBColor borderLightColor = IsEnabled () && IsActive () ? 
									UGAColorRamp::GetWhiteColor() : UGAColorRamp::GetColor(2);

			// е Start off with a clean pen
			::PenNormal ();

			// е Frame the light portion of the border
			::RGBForeColor ( &borderLightColor );
			borderRect.right -= 1;
			borderRect.bottom -= 1;
			::OffsetRect ( &borderRect, 1, 1 );
			::FrameRect ( &borderRect );
			::OffsetRect ( &borderRect, -1, -1 );

			// е Frame the dark portion of the border
			::RGBForeColor ( &borderDarkColor );
			::FrameRect ( &borderRect );
			
			// е Fix upsome pixels
			UGraphicsUtilities::PaintColorPixel ( borderRect.left, borderRect.bottom, &borderLightColor );
			UGraphicsUtilities::PaintColorPixel ( borderRect.right, borderRect.top, &borderLightColor );

			// е Restore the border rect to its original size
			CalcBorderRect ( borderRect );
			
		}
	}
	
}	//	LGAPrimaryBox::DrawBoxBorder



//------------------------------------------------------------------------------------
//	LGAPrimaryBox::RefreshBoxBorder
//------------------------------------------------------------------------------------
//
//	Invalidate the border area of the box so it will 
//	get redrawn during the next update

void
LGAPrimaryBox::RefreshBoxBorder ()
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
			
			// е Get the content invalidated so that it will be refreshed
			InvalPortRgn(borderRgn);
//			::InvalRgn ( borderRgn );
						
			// е Cleanup
			::DisposeRgn ( borderRgn );
		}
	}
}	//	LGAPrimaryBox::RefreshBoxBorder


//------------------------------------------------------------------------------------
//	LGAPrimaryBox::DontRefreshBoxBorder
//------------------------------------------------------------------------------------
//
//	Validate the area occupied by the border of the banner box so it won't 
//	get redrawn during the next update

void
LGAPrimaryBox::DontRefreshBoxBorder (	Boolean	inOKIfHidden )
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
}	//	LGAPrimaryBox::DontRefreshBoxBorder


//------------------------------------------------------------------------------------
//	LGAPrimaryBox::RefreshBoxContent
//------------------------------------------------------------------------------------
//
//	Invalidate the content area of the box so it will 
//	get redrawn during the next update

void
LGAPrimaryBox::RefreshBoxContent ()
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
	
}	//	LGAPrimaryBox::RefreshBoxContent


//------------------------------------------------------------------------------------
//	LGAPrimaryBox::RefreshBoxTitle
//------------------------------------------------------------------------------------
//
//	Invalidate the title area of the box so it will get redrawn during the next update

void
LGAPrimaryBox::RefreshBoxTitle ()
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
}	//	LGAPrimaryBox::RefreshBoxTitle


#pragma mark -
#pragma mark === MISCELLANEOUS

//=====================================================================================
// ее MISCELLANEOUS
//------------------------------------------------------------------------------------
//	LGAPrimaryBox::CalculateTitleHeightAndWidth
//------------------------------------------------------------------------------------
//

void
LGAPrimaryBox::CalculateTitleHeightAndWidth ( 	SInt16 *outHeight,
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

}	//	LGAPrimaryBox::CalculateTitleHeightAndWidth


