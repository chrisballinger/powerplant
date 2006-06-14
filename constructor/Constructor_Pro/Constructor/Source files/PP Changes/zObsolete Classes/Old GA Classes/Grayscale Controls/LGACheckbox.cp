// Copyright �2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright� 1996 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< � >>-----------------------------------------

FILE:					LGACheckbox.cp

CLASSES:				LGACheckbox

DESCRIPTION:		Implementation file for LGACheckbox

AUTHOR:				Robin Mair

CREATION DATE :	96.05.26

CHANGE HISTORY :

		97.04.29		rtm	Fixed the RefreshControl method which had a problem where the
								portion of the checkmark that draws outside the box would not
								get redrawn correctly in a view that was not doing any drawing
								of its background.
		97.04.20		rtm	Removed the CreateFromStream method.
		97.04.12		rtm	Fixed a bug in the RefreshControl method where the focus needed
								to be restored after the invalidation of the control portion of
								the checkbox.
		97.04.11		rtm	Added a flag that can be used to control whether the checkbox
								draws a classic style checkmark or the new style.  The new
								style is the default.  The classic style can only be set
								proceedurally by calling the method SetClassicCheckMark, this
								was provided for those that would like to stick with the old
								style mark. This was not added to the PPob format as we did
								not want to break existing layouts.  At somepoint in the future
								when other changes might result in a PPob break this will be
								accessible from within Constructor.
		97.04.11		rtm	Modified the rendering of the checkbox such that it now draws
								an actual checkmark instead of an 'x'.  This was changed to
								match the way Apple is now rendering checkboxes in the 
								appearance manager. This also involved adding a new method
								called RefreshControl which is used to refresh the control
								portion of the checkbox. This was needed so that we could
								correctly invalidate the control portion of the checkbox as the
								checkmark overhangs the background outside of the control box.
		97.02.08		rtm	Changed the rendering of the control such that is it drawn as
								disabled when it is deactivated.
		97.02.03		rtm	Added support for the mixed state for a checkbox as spec'ed
								by AGA.
		97.01.24		rtm	Added a copy constructor.
		96.01.20		rtm	Added a parameterized constructor so that the pane can be built
								procedurally.
		96.12.08		rtm	Changed justification constants to use the new ones defined
								in the universal headers.
		96.10.16		rtm	Added a small change to SetValue that checks to see if the 
								value has changed before it does anything, only does something
								if it has in fact changed.
		96.10.12		rtm	Had to tweak the drawing of titles across multiple devices
								disabled.  Using the grayishTextOr text mode transfer was
								resulting in some weirdness that could only be solved by
								doing something similar to what grayishTextOr did, which was
								to basically lighten the text color.
		96.10.09		rtm	Changed the title drawing so that it now will respect a color
								provided in a text trait as  opposed to always drawing the title
								in black.
		96.10.08		rtm	Got rid of DrawCheckbox by moving its contents to DrawSelf.
		96.10.04		rtm	Replaced all SetCPixel calls with a new PaintColorPixel call
								that has been added to UGraphicsUtilities. This should get
								around the problems with using SetCPixel.
		96.09.19		rtm	Changed the title drawing method so that it always left justifies
								the title and therefore ignores the justification of any
								associated text trait.
		96.09.19		rtm	Changed the title drawing method so that the actual call to 
								draw the text is inside the while loop used to check the
								screen depth.
		96.09.13		rtm	Completely changed the way this class is draw, it now handles
								the drawing directly which got rid of about 2K of data and
								seems to be about 50% faster drawing than before.  Its still
								not quite as fast as a standard toolbox checkbox, but then it
								never will be as the rendering is a lot more complicated than
								just drawing a box with an check mark in it.
		96.09.04		rtm	PowerPlant conformance changes.
		96.09.02		rtm	Added overrides of EnableSelf and DisableSelf so that we can
								handle getting the control redrawn when the state changes.
		96.09.01		rtm	Changed the rendering of the dimmed state of the control to
								meet Apple's modified spec for grayscale controls.
		96.08.11		rtm	Changed SetValue so that it now calls Draw if the hilite state
								is not set, thus allowing the change in value to take effect
								when called procedurally.
		96.06.20		rtm	Changed the SetHiliteState method so that it now handles the
								the redraw after a state change instead of all the other 
								places that were trying to take care of it.
		96.05.26		rtm	Removed all dependencies on the graphics context
		96.05.26		rtm	Initial creation of file

---------------------------------------<< � >>-----------------------------------------
*/


#ifdef PowerPlant_PCH
	#include PowerPlant_PCH
#endif

// � GRAYSCALE HEADERS
#include <LGACheckbox.h>
#include <UGraphicsUtilities.h>
#include <UGAColorRamp.h>

// � POWERPLANT HEADERS
#include <LStream.h>
#include <PP_Messages.h>
#include <UDrawingState.h>
#include <UDrawingUtils.h>
#include <LString.h>
#include <UTextTraits.h>


//=====================================================================================
// CONSTANTS
//=====================================================================================

enum
{
	checkboxButton_CheckboxWidth				=	12,
	checkboxButton_TextOffset					=	2
};



#pragma mark === CLASS: LGACheckbox

//=====================================================================================
// CLASS:	LGACheckbox
//=====================================================================================

#pragma mark -
#pragma mark === INITIALIZATION

//=====================================================================================
// �� INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LGACheckbox::LGACheckbox					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------
//
//	Default constructor

LGACheckbox::LGACheckbox ()
{	

	mHilited = false;
	mClassicCheckMark = false;
	
}	//	LGACheckbox::LGACheckbox


//-------------------------------------------------------------------------------------
// LGACheckbox::LGACheckbox					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------
//
//	Copy constructor

LGACheckbox::LGACheckbox (	const LGACheckbox &inOriginal )
						:	LControl ( inOriginal ),
							LGATitleMixin ( inOriginal )
{	

	mHilited = inOriginal.mHilited;
	mClassicCheckMark = inOriginal.mClassicCheckMark;
	
}	//	LGACheckbox::LGACheckbox


//-------------------------------------------------------------------------------------
// LGACheckbox::LGACheckbox					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------
//
//	Stream constructor

LGACheckbox::LGACheckbox (	LStream	*inStream ) 
							: LControl ( inStream )
{

	SInt16	controlKind;
	SInt32	macRefCon;
	
	// � We read this but don't do anything with it
	*inStream >> controlKind;

	// � Read in the text trait ID
	*inStream >> mTextTraitsID;
	
	// � Read in the title string
	inStream->ReadPString ( mTitle );
	
	// � We read this but don't do anything with it
	*inStream >> macRefCon;
	
	// � Setup the hilite flag
	mHilited = false;
	mClassicCheckMark = false;
	
	// � Make sure we setup the max value for the control in case the user
	// did not do it in the PPob
	if ( GetMaxValue () < 2 )
		SetMaxValue ( 2 );
		
}	//	LGACheckbox::LGACheckbox


//-------------------------------------------------------------------------------------
// LGACheckbox::LGACheckbox					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------
//
//	Construct class from input parameters

LGACheckbox::LGACheckbox (	const SPaneInfo		&inPaneInfo,
									const SControlInfo	&inControlInfo,
									Str255			inTitle,
									ResIDT			inTextTraitID ) 
							: LControl ( 	inPaneInfo,
												inControlInfo.valueMessage,
												inControlInfo.value,
												inControlInfo.minValue,
												inControlInfo.maxValue )
{

	// � Setup our fields from the input parameters
	mTitle = inTitle;
	mTextTraitsID = inTextTraitID;
	
	// � Setup the hilite flag
	mHilited = false;
	mClassicCheckMark = false;
	
	// � Make sure we setup the max value for the control in case the user
	// did not do it in the PPob
	if ( GetMaxValue () < 2 )
		SetMaxValue ( 2 );
		
}	//	LGACheckbox::LGACheckbox


//-------------------------------------------------------------------------------------
// LGACheckbox::~LGACheckbox				---<<� DESTRUCTOR �>>---
//-------------------------------------------------------------------------------------

LGACheckbox::~LGACheckbox ()
{
}	//	LGACheckbox::~LGACheckbox


#pragma mark -
#pragma mark === ACCESSORS

//=====================================================================================
// �� ACCESSORS
//-------------------------------------------------------------------------------------
// LGACheckbox::GetDescriptor
//-------------------------------------------------------------------------------------

StringPtr
LGACheckbox::GetDescriptor ( Str255	outDescriptor ) const
{

	return LString::CopyPStr ( mTitle, outDescriptor );

}	//	LGACheckbox::GetDescriptor


//-------------------------------------------------------------------------------------
// LGACheckbox::SetDescriptor
//-------------------------------------------------------------------------------------

void
LGACheckbox::SetDescriptor (	ConstStr255Param	 inDescriptor )
{

	mTitle = inDescriptor;
	Refresh ();

}	//	LGACheckbox::SetDescriptor


//-------------------------------------------------------------------------------------
// LGACheckbox::SetHiliteState
//-------------------------------------------------------------------------------------

void			
LGACheckbox::SetHiliteState	(	Boolean	inHiliteState )
{
	
	// � Only make the change if we have too
	if ( mHilited != inHiliteState )
	{
		mHilited = inHiliteState;
		
		// � Invalidate the checkbox so that it will be redrawn in the
		// correct state, this only forces the actual control portion
		// to be redrawn
		RefreshControl ();
	}
			
}	//	LGACheckbox::SetHiliteState


//-------------------------------------------------------------------------------------
// LGACheckbox::SetValue
//-------------------------------------------------------------------------------------

void			
LGACheckbox::SetValue	( SInt32	inValue )
{

	// � We only do something if the value is changing
	if ( inValue != GetValue ())
	{
		// � Call our parent to handle the setting of the value
		LControl::SetValue ( inValue );
		
		// � And make sure we get the hilite state changed
		if ( IsHilited ())
			SetHiliteState ( false );
		else
			RefreshControl ();
	}
	
}	//	LGACheckbox::SetValue

//-------------------------------------------------------------------------------------
// LGACheckbox::SetSetClassicCheckMark
//-------------------------------------------------------------------------------------

void			
LGACheckbox::SetSetClassicCheckMark	(	Boolean	inClassicMark )
{
	
	// � Only make the change if we have too
	if ( mClassicCheckMark != inClassicMark )
	{
		mClassicCheckMark = inClassicMark;
		
		// � Invalidate the checkbox so that it will be redrawn in the
		// correct state, this only forces the actual control portion
		// to be redrawn
		RefreshControl ();
	}
			
}	//	LGACheckbox::SetSetClassicCheckMark


//-------------------------------------------------------------------------------------
// LGACheckbox::CalcTitleRect
//-------------------------------------------------------------------------------------

void
LGACheckbox::CalcTitleRect	( Rect	&outRect )
{

	StTextState			theTextState;
	StColorPenState	thePenState;
	
	// � Get the text traits ID for the control
	ResIDT	textTID = GetTextTraitsID ();

	// � Get the port setup with the text traits
	UTextTraits::SetPortTextTraits ( textTID );
	
	// � Figure out the height of the text for the selected font
	FontInfo fi;
	::GetFontInfo ( &fi );
	SInt16		textHeight = fi.ascent + fi.descent;
		
	// � Get the local inset frame rectangle
	CalcLocalFrameRect ( outRect );
	SInt16		frameHeight = UGraphicsUtilities::RectHeight ( outRect );
	::InsetRect ( &outRect, 4, 2 );
	
	// � If the text height is greater than the frame height the
	// constrain it to the frame height
	if ( frameHeight < textHeight )
		textHeight = frameHeight;
		
	// � Now we modify the title rect so that it is to the right of the checkbox
	// control itself
	outRect.top += ((frameHeight - textHeight) / 2) - checkboxButton_TextOffset;
	outRect.bottom = outRect.top + textHeight;
	outRect.left += checkboxButton_CheckboxWidth + checkboxButton_TextOffset;

}	//	LGACheckbox::CalcTitleRect


//-------------------------------------------------------------------------------------
// LGACheckbox::CalcLocalControlRect
//-------------------------------------------------------------------------------------

void
LGACheckbox::CalcLocalControlRect ( Rect &outRect )
{
	// � Figure out the frame rect for the checkbox button
	CalcLocalFrameRect ( outRect );
	SInt16		frameHeight = UGraphicsUtilities::RectHeight ( outRect );

	// � Adjust the rect such that it is always centered vertically within
	// the checkbox buttons frame rect
	outRect.left += 1;
	outRect.right = outRect.left + checkboxButton_CheckboxWidth;
	outRect.top += ((frameHeight - checkboxButton_CheckboxWidth) / 2);
	outRect.bottom = outRect.top + checkboxButton_CheckboxWidth;
	
}	//	LGACheckbox::CalcLocalControlRect


#pragma mark -
#pragma mark === ENABLING & DISABLING

//=====================================================================================
// �� ENABLING & DISABLING
//-------------------------------------------------------------------------------------
// LGACheckbox::EnableSelf
//-------------------------------------------------------------------------------------
//

void
LGACheckbox::EnableSelf ()
{

	// � Get things redrawn so that we can see the state change
	Draw ( nil );

}	//	LGACheckbox::EnableSelf


//-------------------------------------------------------------------------------------
// LGACheckbox::DisableSelf
//-------------------------------------------------------------------------------------
//

void
LGACheckbox::DisableSelf ()
{

	// � Get things redrawn so that we can see the state change
	Draw ( nil );

}	//	LGACheckbox::DisableSelf


#pragma mark -
#pragma mark === ACTIVATION

//=====================================================================================
// �� ACTIVATION
//-------------------------------------------------------------------------------------
// LGACheckbox::ActivateSelf
//-------------------------------------------------------------------------------------

void
LGACheckbox::ActivateSelf ()
{

	// � Get everything redrawn so the changes take effect
	if ( mActive == triState_On )
		Refresh ();
					
}	//	LGACheckbox::ActivateSelf



//-------------------------------------------------------------------------------------
// LGACheckbox::DeactivateSelf
//-------------------------------------------------------------------------------------

void
LGACheckbox::DeactivateSelf ()
{

	// � Get everything redrawn so the changes take effect
	if ( mActive == triState_Off || mActive == triState_Latent )
		Refresh ();
				
}	//	LGACheckbox::DeactivateSelf


#pragma mark -
#pragma mark === DRAWING

//=====================================================================================
// �� DRAWING
//-------------------------------------------------------------------------------------
// LGACheckbox::DrawSelf
//-------------------------------------------------------------------------------------

void
LGACheckbox::DrawSelf	()
{

	StColorPenState::Normalize ();
	
	// � Get the checkbox drawn in its various states
	if ( IsEnabled () && IsActive ())
	{
		if ( IsHilited ())
			DrawCheckboxHilited ();
		else
			DrawCheckboxNormal ();
	}
	else
		DrawCheckboxDimmed ();

	// � Now get the title for the checkbox button drawn, if there is one
	if ( HasTitle ())
		DrawCheckboxTitle ();
	
}	//	LGACheckbox::DrawSelf


//-------------------------------------------------------------------------------------
// LGACheckbox::DrawCheckboxTitle
//-------------------------------------------------------------------------------------

void
LGACheckbox::DrawCheckboxTitle	()
{	
	StColorPenState	theColorPenState;
	StTextState			theTextState;
		
	// � Get some loal variables setup including the rect for the title
	ResIDT	textTID = GetTextTraitsID ();
	Rect	titleRect;
	
	// � Setup the port text style fomr the text traits
	UTextTraits::SetPortTextTraits ( textTID );
	
	// � Setup the justification which is always left justified
	SInt16	titleJust = teFlushDefault;
	
	// � Calculate the title rect
	CalcTitleRect ( titleRect );
	
	// � Get the control's title
	Str255 controlTitle;
	GetDescriptor ( controlTitle );

	// � Save off the text color as setup by the TextTrait
	RGBColor	textColor;
	::GetForeColor ( &textColor );
	
	// � Loop over any devices we might be spanning and handle the drawing
	// appropriately for each devices screen depth
	StDeviceLoop	theLoop ( titleRect );
	SInt16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		if ( depth < 4 )		//	� BLACK & WHITE
		{
			if ( !IsHilited ())
			{
				// � If the control is dimmed then we use the grayishTextOr 
				// transfer mode to draw the text
				if ( !IsEnabled ())
					::TextMode ( grayishTextOr );
				
				// � Get the actual text drawn with the appropriate justification
			UStringDrawing::DrawJustifiedString ( 	controlTitle,
																titleRect,
																titleJust );
			}
		}
		else	// � COLOR
		{
			// � If the control is dimmed then we have to do our own version of the
			// grayishTextOr as it does not appear to work correctly across
			// multiple devices
			if ( !IsEnabled () || !IsActive ())
			{
				textColor = UGraphicsUtilities::Lighten ( &textColor );
				::TextMode ( srcOr );
				::RGBForeColor ( &textColor );
			}
				
			// � Get the actual text drawn with the appropriate justification
			UStringDrawing::DrawJustifiedString ( 	controlTitle,
																titleRect,
																titleJust );
		}	
	}

}	//	LGACheckbox::DrawCheckboxTitle


//-------------------------------------------------------------------------------------
// LGACheckbox::DrawCheckboxNormal
//-------------------------------------------------------------------------------------

void
LGACheckbox::DrawCheckboxNormal	()
{
	
	StColorPenState theSavedPenState;
	
	// � Get the frame for the control
	Rect	localFrame, loopRect;
	CalcLocalControlRect ( localFrame );
	loopRect = localFrame;
	loopRect.right += 2;
	
	RGBColor tempColor;
	
	// � Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( loopRect );
	SInt16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		// � BLACK & WHITE
		if ( depth < 4 )
		{
			// � Now we need to erase the interior of the box so that state
			// changes will show up
			::EraseRect ( &loopRect );

			// � BORDER
			tempColor = UGAColorRamp::GetBlackColor();
			::RGBForeColor ( &tempColor );
			::FrameRect ( &localFrame );
			
			// � Now we need to erase the interior of the box so that state
			// changes will show up
			::InsetRect ( &localFrame, 1, 1 );
			::EraseRect ( &localFrame );
			::InsetRect ( &localFrame, -1, -1 );
		}
		// � COLOR
		else
		{
			// � Start by rendering the border and background
			// � BORDER
			tempColor = UGAColorRamp::GetBlackColor();
			::RGBForeColor ( &tempColor );
			::FrameRect ( &localFrame );
			
			// � PAINT FACE
			::InsetRect ( &localFrame, 1, 1 );
			tempColor = UGAColorRamp::GetColor(2);
			::RGBForeColor ( &tempColor );
			::PaintRect ( &localFrame );
			
			// � LIGHT EDGES
			tempColor = UGAColorRamp::GetWhiteColor();
			::RGBForeColor ( &tempColor );
			UGraphicsUtilities::TopLeftSide ( &localFrame, 0, 0, 1, 1 );
			
			// � SHADOW EDGES
			tempColor = UGAColorRamp::GetColor(7);
			::RGBForeColor ( &tempColor );
			UGraphicsUtilities::BottomRightSide ( &localFrame, 1, 1, 0, 0 );
			
			// � Prepare for the drawing of the check mark
			::InsetRect ( &localFrame, -1, -1 );
			
			// � CHECK MARK SHADOWS
			if ( IsMixedState ())
			{
				tempColor = UGAColorRamp::GetColor(5);
				::RGBForeColor ( &tempColor );
				::PenSize ( 1, 1 );
				::MoveTo ( 	localFrame.left + 4,  
								localFrame.top + 7 );
				::LineTo ( 	localFrame.right - 3, 
								localFrame.top + 7 );
				::LineTo ( 	localFrame.right - 3, 
								localFrame.top + 5 );
			}
			else if ( IsSelected ())
			{
				if ( mClassicCheckMark )
				{
					::PenSize ( 1, 1 );
					RGBColor theColor = UGAColorRamp::GetColor(8);
					::RGBForeColor ( &theColor );
					::MoveTo ( localFrame.left + 4,  localFrame.bottom - 4 );
					::LineTo ( localFrame.right - 3, localFrame.top + 3 );
					theColor = UGAColorRamp::GetColor(5);
					::RGBForeColor ( &theColor );
					::MoveTo ( localFrame.left + 4,  localFrame.bottom - 3 );
					::LineTo ( localFrame.right - 3, localFrame.top + 4 );
					
					// � Fixup a couple of pixels
					theColor = UGAColorRamp::GetColor(8);
					UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 3,
										localFrame.bottom - 4,
										&theColor );
					theColor = UGAColorRamp::GetColor(5);
					UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 3,
										localFrame.bottom - 3,
										&theColor );
				}
				else
				{
					// � Draw the light gray shadows
					tempColor = UGAColorRamp::GetColor(colorRamp_Gray5);
					UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 3,
																		localFrame.top + 7,
																		&tempColor );
					UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 4,
																		localFrame.top + 8,
																		&tempColor );
					UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 6,
																		localFrame.top + 9,
																		&tempColor );
					UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 7,
																		localFrame.top + 8,
																		&tempColor );
					UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 8,
																		localFrame.top + 7,
																		&tempColor );
					UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 9,
																		localFrame.top + 6,
																		&tempColor );
																		
					tempColor = UGAColorRamp::GetColor(colorRamp_Gray8);
					UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 10,
																		localFrame.top + 5,
																		&tempColor );
																		
					tempColor = UGAColorRamp::GetColor(colorRamp_Gray5);
					UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 12,
																		localFrame.top + 3,
																		&tempColor );
					UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 13,
																		localFrame.top + 2,
																		&tempColor );

					// � DARK SHADOWS
					tempColor = UGAColorRamp::GetColor(colorRamp_Gray8);
					UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 5,
																		localFrame.top + 9,
																		&tempColor );
					UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 6,
																		localFrame.top + 8,
																		&tempColor );
					UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 7,
																		localFrame.top + 7,
																		&tempColor );
					UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 8,
																		localFrame.top + 6,
																		&tempColor );
					UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 9,
																		localFrame.top + 5,
																		&tempColor );
																		
					tempColor = UGAColorRamp::GetColor(colorRamp_Gray10);
					UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 10,
																		localFrame.top + 4,
																		&tempColor );
																		
					tempColor = UGAColorRamp::GetColor(colorRamp_Gray8);
					UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 12,
																		localFrame.top + 2,
																		&tempColor );
				}
			}
		}	
		
		// � CHECK MARK
		// � Draw the check mark inside the checkbox this is always drawn in black
		// so we just draw it outside the depth stuff
		if ( IsMixedState ())
		{
			tempColor = UGAColorRamp::GetBlackColor();
			::RGBForeColor ( &tempColor );
			::PenSize ( 1, 2 );
			::MoveTo ( 	localFrame.left + 3,  
							localFrame.top + 5 );
			::LineTo ( 	localFrame.right - 4, 
							localFrame.top + 5 );
		}
		else if ( IsSelected ())
		{
			tempColor = UGAColorRamp::GetBlackColor();
			::RGBForeColor ( &tempColor );
			::PenSize ( 1, 1 );

			if ( mClassicCheckMark )
			{
				::PenSize ( 1, 2 );
				::MoveTo ( localFrame.left + 3,  localFrame.top + 2 );
				::LineTo ( localFrame.right - 4, localFrame.bottom - 5 );
				::MoveTo ( localFrame.left + 3,  localFrame.bottom - 5 );
				::LineTo ( localFrame.right - 4, localFrame.top + 2 );
			}
			else
			{
				::MoveTo ( localFrame.left + 2,  localFrame.top + 5 );
				::LineTo ( localFrame.left + 2 + 1, localFrame.top + 5 );
				::MoveTo ( localFrame.left + 3,  localFrame.top + 6 );
				::LineTo ( localFrame.left + 3 + 1, localFrame.top + 6 );
				::MoveTo ( localFrame.left + 4,  localFrame.top + 7 );
				::LineTo ( localFrame.left + 4 + 2, localFrame.top + 7 );
				::MoveTo ( localFrame.left + 6,  localFrame.top + 6 );
				::LineTo ( localFrame.left + 6 + 1, localFrame.top + 6 );
				::MoveTo ( localFrame.left + 7,  localFrame.top + 5 );
				::LineTo ( localFrame.left + 7 + 1, localFrame.top + 5 );
				::MoveTo ( localFrame.left + 8,  localFrame.top + 4 );
				::LineTo ( localFrame.left + 8 + 1, localFrame.top + 4 );
				::MoveTo ( localFrame.left + 9,  localFrame.top + 3 );
				::LineTo ( localFrame.left + 9 + 1, localFrame.top + 3 );
				::MoveTo ( localFrame.left + 10,  localFrame.top + 2 );
				::LineTo ( localFrame.left + 10 + 1, localFrame.top + 2 );
				::MoveTo ( localFrame.left + 11,  localFrame.top + 1 );
				::LineTo ( localFrame.left + 11 + 1, localFrame.top + 1 );
				UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 5,
																	localFrame.top + 8,
																	&tempColor );
			}
		}
	}
	
}	//	LGACheckbox::DrawCheckboxNormal


//-------------------------------------------------------------------------------------
// LGACheckbox::DrawCheckboxHilited
//-------------------------------------------------------------------------------------

void
LGACheckbox::DrawCheckboxHilited	()
{
	
	// � Get the frame for the control
	Rect	localFrame, loopRect;
	CalcLocalControlRect ( localFrame );
	loopRect = localFrame;
	loopRect.right += 2;
	
	RGBColor tempColor;
	
	// � Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( loopRect );
	SInt16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		// � BLACK & WHITE
		if ( depth < 4 )
		{
			if ( IsHilited ())
			{
				// � BORDER
				tempColor = UGAColorRamp::GetBlackColor();
				::RGBForeColor ( &tempColor );
				::PenSize ( 2, 2 );
				::FrameRect ( &localFrame );
			
				// � Now we need to erase the interior of the box to get rid of
				// any previous state
				::InsetRect ( &localFrame, 2, 2 );
				::EraseRect ( &localFrame );
				::InsetRect ( &localFrame, -2, -2 );
			}
		}
		// � COLOR
		else
		{
			if ( IsHilited ())
			{
				// � Start by rendering the border and background
				// � BORDER
				tempColor = UGAColorRamp::GetBlackColor();
				::RGBForeColor ( &tempColor );
				::FrameRect ( &localFrame );
				
				// � PAINT FACE
				::InsetRect ( &localFrame, 1, 1 );
				tempColor = UGAColorRamp::GetColor(8);
				::RGBForeColor ( &tempColor );
				::PaintRect ( &localFrame );
				
				// � SHADOW EDGES
				tempColor = UGAColorRamp::GetColor(10);
				::RGBForeColor ( &tempColor );
				UGraphicsUtilities::TopLeftSide ( &localFrame, 0, 0, 1, 1 );
				
				// � LIGHT EDGES
				tempColor = UGAColorRamp::GetColor(6);
				::RGBForeColor ( &tempColor );
				UGraphicsUtilities::BottomRightSide ( &localFrame, 1, 1, 0, 0 );
			
				// � Prepare for the drawing of the check mark
				::InsetRect ( &localFrame, -1, -1 );
			
				// � CHECK MARK SHADOWS
				if ( IsMixedState ())
				{
					tempColor = UGAColorRamp::GetColor(10);
					::RGBForeColor ( &tempColor );
					::PenSize ( 1, 1 );
					::MoveTo ( 	localFrame.left + 4,  
									localFrame.top + 7 );
					::LineTo ( 	localFrame.right - 3, 
									localFrame.top + 7 );
					::LineTo ( 	localFrame.right - 3, 
									localFrame.top + 5 );
				}
				else if ( IsSelected ())
				{
					if ( mClassicCheckMark )
					{
						::PenSize ( 1, 1 );
						RGBColor theColor = UGAColorRamp::GetColor(11);
						::RGBForeColor ( &theColor );
						::MoveTo ( localFrame.left + 4,  localFrame.bottom - 4 );
						::LineTo ( localFrame.right - 3, localFrame.top + 3 );
						theColor = UGAColorRamp::GetColor(10);
						::RGBForeColor ( &theColor );
						::MoveTo ( localFrame.left + 4,  localFrame.bottom - 3 );
						::LineTo ( localFrame.right - 3, localFrame.top + 4 );
						
						// � Fixup a couple of pixels
						theColor = UGAColorRamp::GetColor(11);
						UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 3,
											localFrame.bottom - 4,
											&theColor );
						theColor = UGAColorRamp::GetColor(10);
						UGraphicsUtilities::PaintColorPixel ( 	localFrame.right - 3,
											localFrame.bottom - 3,
											&theColor );
					}
					else
					{
						// � Draw the light gray shadows
						tempColor = UGAColorRamp::GetColor(colorRamp_Gray10);
						UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 3,
																			localFrame.top + 7,
																			&tempColor );
						UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 4,
																			localFrame.top + 8,
																			&tempColor );
						UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 6,
																			localFrame.top + 9,
																			&tempColor );
						UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 7,
																			localFrame.top + 8,
																			&tempColor );
						UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 8,
																			localFrame.top + 7,
																			&tempColor );
						UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 9,
																			localFrame.top + 6,
																			&tempColor );
																			
						tempColor = UGAColorRamp::GetColor(colorRamp_Gray11);
						UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 10,
																			localFrame.top + 5,
																			&tempColor );
																			
						tempColor = UGAColorRamp::GetColor(colorRamp_Gray10);
						UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 12,
																			localFrame.top + 3,
																			&tempColor );
						UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 13,
																			localFrame.top + 2,
																			&tempColor );

						// � DARK SHADOWS
						tempColor = UGAColorRamp::GetColor(colorRamp_Gray11);
						UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 5,
																			localFrame.top + 9,
																			&tempColor );
						UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 6,
																			localFrame.top + 8,
																			&tempColor );
						UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 7,
																			localFrame.top + 7,
																			&tempColor );
						UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 8,
																			localFrame.top + 6,
																			&tempColor );
						UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 9,
																			localFrame.top + 5,
																			&tempColor );
																			
						tempColor = UGAColorRamp::GetColor(colorRamp_Gray12);
						UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 10,
																			localFrame.top + 4,
																			&tempColor );
																			
						tempColor = UGAColorRamp::GetColor(colorRamp_Gray11);
						UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 12,
																			localFrame.top + 2,
																			&tempColor );
					}
				}
			}
		}	
		
		// � CHECK MARK
		// � Draw the check mark inside the checkbox this is always drawn in black
		// so we just draw it outside the depth stuff
		if ( IsMixedState ())
		{
			RGBColor theColor = UGAColorRamp::GetBlackColor();
			::RGBForeColor ( &theColor );
			::PenSize ( 1, 2 );
			::MoveTo ( 	localFrame.left + 3,  
							localFrame.top + 5 );
			::LineTo ( 	localFrame.right - 4, 
							localFrame.top + 5 );
		}
		else if ( IsHilited () && IsSelected ())
		{
			tempColor = UGAColorRamp::GetBlackColor();
			::RGBForeColor ( &tempColor );
			::PenSize ( 1, 1 );

			if ( mClassicCheckMark )
			{
				::PenSize ( 1, 2 );
				::MoveTo ( localFrame.left + 3,  localFrame.top + 2 );
				::LineTo ( localFrame.right - 4, localFrame.bottom - 5 );
				::MoveTo ( localFrame.left + 3,  localFrame.bottom - 5 );
				::LineTo ( localFrame.right - 4, localFrame.top + 2 );
			}
			else
			{
				::MoveTo ( localFrame.left + 2,  localFrame.top + 5 );
				::LineTo ( localFrame.left + 2 + 1, localFrame.top + 5 );
				::MoveTo ( localFrame.left + 3,  localFrame.top + 6 );
				::LineTo ( localFrame.left + 3 + 1, localFrame.top + 6 );
				::MoveTo ( localFrame.left + 4,  localFrame.top + 7 );
				::LineTo ( localFrame.left + 4 + 2, localFrame.top + 7 );
				::MoveTo ( localFrame.left + 6,  localFrame.top + 6 );
				::LineTo ( localFrame.left + 6 + 1, localFrame.top + 6 );
				::MoveTo ( localFrame.left + 7,  localFrame.top + 5 );
				::LineTo ( localFrame.left + 7 + 1, localFrame.top + 5 );
				::MoveTo ( localFrame.left + 8,  localFrame.top + 4 );
				::LineTo ( localFrame.left + 8 + 1, localFrame.top + 4 );
				::MoveTo ( localFrame.left + 9,  localFrame.top + 3 );
				::LineTo ( localFrame.left + 9 + 1, localFrame.top + 3 );
				::MoveTo ( localFrame.left + 10,  localFrame.top + 2 );
				::LineTo ( localFrame.left + 10 + 1, localFrame.top + 2 );
				::MoveTo ( localFrame.left + 11,  localFrame.top + 1 );
				::LineTo ( localFrame.left + 11 + 1, localFrame.top + 1 );
				UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 5,
																	localFrame.top + 8,
																	&tempColor );
			}
		}
	}
	
}	//	LGACheckbox::DrawCheckboxHilited


//-------------------------------------------------------------------------------------
// LGACheckbox::DrawCheckboxDimmed
//-------------------------------------------------------------------------------------

void
LGACheckbox::DrawCheckboxDimmed	()
{
	
	// � Get the frame for the control
	Rect	localFrame, loopRect;
	CalcLocalControlRect ( localFrame );
	loopRect = localFrame;
	loopRect.right += 2;
	
	RGBColor tempColor;
	
	// � Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( loopRect );
	SInt16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		// � BLACK & WHITE
		if ( depth < 4 )
		{
			if ( !IsEnabled ())
			{
				// � BORDER
				tempColor = UGAColorRamp::GetBlackColor();
				::RGBForeColor ( &tempColor );
				::FrameRect ( &localFrame );
			}
		}
		// � COLOR
		else
		{
			if ( !IsEnabled () || !IsActive ())
			{
				// � Start by rendering the border and background
				// � BORDER
				tempColor = UGAColorRamp::GetColor(7);
				::RGBForeColor ( &tempColor );
				::FrameRect ( &localFrame );
				
				// � PAINT FACE
				::InsetRect ( &localFrame, 1, 1 );
				tempColor = UGAColorRamp::GetColor(2);
				::RGBForeColor ( &tempColor );
				::PaintRect ( &localFrame );
				
				// � Prepare for the drawing of the check mark
				::InsetRect ( &localFrame, -1, -1 );
			}
		}	
		
		// � CHECK MARK
		// � Draw the check mark inside the checkbox this is always drawn in black
		// so we just draw it outside the depth stuff
		if ( IsMixedState ())
		{
			tempColor = depth >= 4 ? UGAColorRamp::GetColor(7) :
										 UGAColorRamp::GetBlackColor();
			::RGBForeColor ( &tempColor );
			::PenSize ( 1, 2 );
			::MoveTo ( 	localFrame.left + 3,  
							localFrame.top + 5 );
			::LineTo ( 	localFrame.right - 4, 
							localFrame.top + 5 );
		}
		else if ( IsSelected ())
		{

			tempColor = depth >= 4 ? UGAColorRamp::GetColor(7) :
										 UGAColorRamp::GetBlackColor();
			::RGBForeColor ( &tempColor );
			
			if ( mClassicCheckMark )
			{
				::PenSize ( 1, 2 );
				::MoveTo ( localFrame.left + 3,  localFrame.top + 2 );
				::LineTo ( localFrame.right - 4, localFrame.bottom - 5 );
				::MoveTo ( localFrame.left + 3,  localFrame.bottom - 5 );
				::LineTo ( localFrame.right - 4, localFrame.top + 2 );
			}
			else
			{
				::PenSize ( 1, 1 );
				::MoveTo ( localFrame.left + 2,  localFrame.top + 5 );
				::LineTo ( localFrame.left + 2 + 1, localFrame.top + 5 );
				::MoveTo ( localFrame.left + 3,  localFrame.top + 6 );
				::LineTo ( localFrame.left + 3 + 1, localFrame.top + 6 );
				::MoveTo ( localFrame.left + 4,  localFrame.top + 7 );
				::LineTo ( localFrame.left + 4 + 2, localFrame.top + 7 );
				::MoveTo ( localFrame.left + 6,  localFrame.top + 6 );
				::LineTo ( localFrame.left + 6 + 1, localFrame.top + 6 );
				::MoveTo ( localFrame.left + 7,  localFrame.top + 5 );
				::LineTo ( localFrame.left + 7 + 1, localFrame.top + 5 );
				::MoveTo ( localFrame.left + 8,  localFrame.top + 4 );
				::LineTo ( localFrame.left + 8 + 1, localFrame.top + 4 );
				::MoveTo ( localFrame.left + 9,  localFrame.top + 3 );
				::LineTo ( localFrame.left + 9 + 1, localFrame.top + 3 );
				::MoveTo ( localFrame.left + 10,  localFrame.top + 2 );
				::LineTo ( localFrame.left + 10 + 1, localFrame.top + 2 );
				::MoveTo ( localFrame.left + 11,  localFrame.top + 1 );
				::LineTo ( localFrame.left + 11 + 1, localFrame.top + 1 );
				UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + 5,
																	localFrame.top + 8,
																	&tempColor );
			}
		}
	}
	
}	//	LGACheckbox::DrawCheckboxDimmed


#pragma mark -
#pragma mark === MOUSE TRACKING

//=====================================================================================
// �� MOUSE TRACKING
//-------------------------------------------------------------------------------------
// LGACheckbox::HotSpotAction
//-------------------------------------------------------------------------------------

void
LGACheckbox::HotSpotAction	(	SInt16		/* inHotSpot */,
										Boolean	inCurrInside,
										Boolean	inPrevInside )
{

	// � If the mouse moved in or out of the hot spot handle the
	// hiliting of the control
	if ( inCurrInside != inPrevInside )
		SetHiliteState ( inCurrInside );
		
}	//	LGACheckbox::HotSpotAction


//-------------------------------------------------------------------------------------
// LGACheckbox::HotSpotResult
//-------------------------------------------------------------------------------------

void
LGACheckbox::HotSpotResult	(	SInt16 /*inHotSpot*/ )
{
	
	// � Because a checkbox is a switch we toggle
	// the value field of the control to indicate that
	// it is on or off
	SetValue ( GetValue () == 2 ? 1 : 1 - GetValue () );
	
}	//	LGACheckbox::HotSpotResult


#pragma mark -
#pragma mark === MISCELLANEOUS

//=====================================================================================
// �� MISCELLANEOUS
//-------------------------------------------------------------------------------------
// LGACheckbox::RefreshControl
//-------------------------------------------------------------------------------------

void
LGACheckbox::RefreshControl ()
{

	StColorPenState theColorPenState;
	theColorPenState.Normalize ();
	
	// � We only bother doing anything if the control is visible
	if ( IsVisible () && FocusDraw ())
	{
		Rect controlRect, extraRect;
		CalcLocalControlRect ( controlRect );
		controlRect.right += 2;
		extraRect = controlRect;
		extraRect.left = extraRect.right - 2;
		
		// � Convert to port coordinates
		LocalToPortPoint ( topLeft ( controlRect ));
		LocalToPortPoint ( botRight ( controlRect ));
		
		// � We are going to start by getting the fore and back colors setup
		// we then check to see if the back color is white, which tells us we
		// we can then simply just do an erase of the background alongside the
		// control where the checkmark overhangs the edge of the checkbox,
		// otherwise we don't do anything except the invalidate
		// NOTE: the only reason we even bother doing this is that there are a
		// number of cases where the view the checkbox is embedded within does
		// no drawing of its background and therefore the piece of the checkmark
		// that overhangs the box woould never get redrawn, hence this!!
		ApplyForeAndBackColors ();
		RGBColor	backColor, theWhiteColor;
		::GetBackColor ( &backColor );
		theWhiteColor = UGAColorRamp::GetWhiteColor ();
		if ( backColor.red == theWhiteColor.red && 
				backColor.green == theWhiteColor.green &&
				backColor.blue == theWhiteColor.blue)
			::EraseRect ( &extraRect );
			
		// � Invalidate the area occupied by the control portion of the
		// checkbox, including the small piece that hangs outside the box
		InvalPortRect ( &controlRect );
		UpdatePort ();
	
		// � Restore the focus which is changed by the call to UpdatePort
		FocusDraw();
	}

}	//	LGACheckbox::RefreshControl

