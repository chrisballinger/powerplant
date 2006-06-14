// Copyright й2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyrightй 1995 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< е >>-----------------------------------------

FILE:					LGASecondaryBox.cp

CLASSES:				LGASecondaryBox

DESCRIPTION:		Implementation file for LGASecondaryBox

AUTHOR:				Robin Mair

CREATION DATE :	95.12.19

CHANGE HISTORY :

		97.04.20		rtm	Removed the CreateFromStream method.
		97.02.08		rtm	Changed the rendering of the control such that is it drawn as
								disabled when it is deactivated, this is matches the approach
								Apple is taking with the Apearance Manager.
		97.01.24		rtm	Added a copy constructor.
		97.01.22		rtm	Added a parameterized constructor so that instances can be 
								created procedurally.
		96.10.26		rtm	Changed to synch up with the name changes in LGAprimaryBox.
		96.09.12		rtm	Fixed a problem where the border was being calculated incorrectly
								based on the font being used for the title.
		96.09.04		rtm	PowerPlant conformance changes.
		96.09.02		rtm	Changed the rendering of the dimmed state of the box to
								meet Apple's modified spec for grayscale appearance.
		96.06.26		rtm	Change the border drawing routine so that the clipping is
								now set that the border is not drawn where the title would be
								located, if there is one.
		96.06.03		rtm	Built a specific primary box that is preset to the required
								settings.
		95.12.19		rtm	Initial creation of file

---------------------------------------<< е >>-----------------------------------------
*/


#ifdef PowerPlant_PCH
	#include PowerPlant_PCH
#endif

// е GRAYSCALE HEADERS
#include <LGASecondaryBox.h>
#include <UGraphicsUtilities.h>
#include <UGAColorRamp.h>

// е POWERPLANT HEADERS
#include <LStream.h>
#include <UTextTraits.h>
#include <UDrawingState.h>
#include <UDrawingUtils.h>
#include <PP_Messages.h>
#include <LArrayIterator.h>

//=====================================================================================
// CONSTANTS
//=====================================================================================

//=====================================================================================
// DEFINES
//=====================================================================================

#pragma mark === CLASS: LGASecondaryBox

//=====================================================================================
// CLASS:	LGASecondaryBox
//=====================================================================================

#pragma mark -
#pragma mark === INITIALIZATION

//=====================================================================================
// ее INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LGASecondaryBox::LGASecondaryBox					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Default constructor

LGASecondaryBox::LGASecondaryBox ()
{
}	//	LGASecondaryBox::LGASecondaryBox


//-------------------------------------------------------------------------------------
// LGASecondaryBox::LGASecondaryBox					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Copy constructor

LGASecondaryBox::LGASecondaryBox (	const LGASecondaryBox &inOriginal )
								:	LGAPrimaryBox ( inOriginal )
{
}	//	LGASecondaryBox::LGASecondaryBox


//-------------------------------------------------------------------------------------
// LGASecondaryBox::LGASecondaryBox					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Stream constructor

LGASecondaryBox::LGASecondaryBox (	LStream	*inStream ) : LGAPrimaryBox ( inStream )
{
}


//-------------------------------------------------------------------------------------
// LGASecondaryBox::LGASecondaryBox					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Parameterized constructor

LGASecondaryBox::LGASecondaryBox	(	const SPaneInfo	&inPaneInfo,
												const SViewInfo	&inViewInfo,
												Str255				inTitle,
												ResIDT				inTextTraitsID,
												Rect					&inContentOffset )
					: LGAPrimaryBox ( inPaneInfo, 
											inViewInfo,
											inTitle,
											inTextTraitsID,
											inContentOffset )
{
}	//	LGASecondaryBox::LGASecondaryBox

//-------------------------------------------------------------------------------------
// LGASecondaryBox::~LGASecondaryBox				---<<е DESTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LGASecondaryBox::~LGASecondaryBox ()
{
}	//	LGASecondaryBox::~LGASecondaryBox


#pragma mark -
#pragma mark === ACCESSORS

//====================================================================================
// ее ACCESSORS
//-------------------------------------------------------------------------------------
//	LGASecondaryBox::CalcBorderRect
//-------------------------------------------------------------------------------------
//

void
LGASecondaryBox::CalcBorderRect ( Rect &outRect )
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
	SInt16	offsetTitleHeight = HasTitle() ? fi.ascent - 1: 0;
	
	// е Calculate the border rect by offsetting its top to accomodate
	// the title, if needed
	if ( offsetTitleHeight > 0 )
		outRect.top += offsetTitleHeight;

}	//	LGASecondaryBox::CalcBorderRect


#pragma mark -
#pragma mark === DRAWING

//====================================================================================
// ее DRAWING
//------------------------------------------------------------------------------------
//	LGASecondaryBox::DrawBoxBorder
//------------------------------------------------------------------------------------
//

void
LGASecondaryBox::DrawBoxBorder ()
{
	
	StColorPenState	theColorState;
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
								UGAColorRamp::GetWhiteColor() : UGAColorRamp::GetColor(1);

			// е Start off with a clean pen
			::PenNormal ();

			// е Frame the dark portion of the border
			::RGBForeColor ( &borderDarkColor );
			UGraphicsUtilities::TopLeftSide ( &borderRect, 0, 0, 1, 1 );
		
			// е Frame the light portion of the border
			::RGBForeColor ( &( IsEnabled () && IsActive () ? borderLightColor : borderDarkColor ));
			UGraphicsUtilities::BottomRightSide ( &borderRect, 1, 1, 0, 0 );
		}
	}
	
}	//	LGASecondaryBox::DrawBoxBorder


