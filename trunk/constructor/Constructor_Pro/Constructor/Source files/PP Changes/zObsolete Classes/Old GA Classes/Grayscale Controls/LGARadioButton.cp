// Copyright й2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyrightй 1996 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< е >>-----------------------------------------

FILE:					LGARadioButton.cp

CLASSES:				LGARadioButton

DESCRIPTION:		Implementation file for LGARadioButton

AUTHOR:				Robin Mair

CREATION DATE :	96.05.25

CHANGE HISTORY :

		97.04.20		rtm	Removed the CreateFromStream method.
		97.02.08		rtm	Changed the rendering of the control such that is it drawn as
								disabled when it is deactivated.
		97.02.03		rtm	Added support for the mixed state for a radio button as spec'ed
								by AGA.
		97.01.24		rtm	Added a copy constructor.
		96.01.20		rtm	Added a parameterized constructor so that the pane can be built
								procedurally.
		96.12.08		rtm	Changed justification constants to use the new ones defined
								in the universal headers.
		96.12.06		rtm	Tweaked some of the color constants as the indices have changed
								a little in the color ramp. Actually changed them so that they
								use the new color constants defined in the color ramp.
		96.12.03		rtm	Moved the color constants defined in the header file for the
								UGAColorRamp, to this source file so that they can be
								removed from the header file.
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
		96.10.08		rtm	Got rid of DrawRadioButton by moving the contents to DrawSelf.
		96.10.04		rtm	Replaced all SetCPixel calls with a new PaintColorPixel call
								that has been added to UGraphicsUtilities. This should get
								around the problems with using SetCPixel.
		96.09.25		rtm	Removed a redundant call to BroadcastValueMessage in HotSpotResult.
		96.09.19		rtm	Changed the title drawing method so that it always left justifies
								the title and therefore ignores the justification of any
								associated text trait.
		96.09.19		rtm	Changed the title drawing method so that the actual call to 
								draw the text is inside the while loop used to check the
								screen depth.
		96.09.04		rtm	PowerPlant conformance changes.
		96.09.02		rtm	Added overrides of EnableSelf and DisableSelf so that we can
								handle getting the control redrawn when the state changes.
		96.09.01		rtm	Changed the rendering of the dimmed state of the control to
								meet Apple's modified spec for grayscale controls.
		96.08.11		rtm	Changed SetValue so that it now handles the redraw in the case
								where the radio was not currently hilited, this means that we
								should be able to use LRadioGroup for grouping LGARadioButtons.
		96.06.20		rtm	Changed SetHiliteState() so that it handles the drawing when
								the state changes so that it is only handled in thsi one 
								location, instead of in the HotSpotAction() method as well.
		96.05.26		rtm	Removed all dependencies on the graphics context
		96.05.26		rtm	Added support for rendering black and white versions of the
								radio button.
		96.05.25		rtm	Initial creation of file

---------------------------------------<< е >>-----------------------------------------
*/


#ifdef PowerPlant_PCH
	#include PowerPlant_PCH
#endif

// е GRAYSCALE HEADERS
#include <LGARadioButton.h>
#include <UGraphicsUtilities.h>
#include <UGAColorRamp.h>

// е POWERPLANT HEADERS
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
	radioRendering_NormalColorOff			=	0,
	radioRendering_NormalColorOn			=	1,
	radioRendering_NormalColorMixed		=	2,
	radioRendering_PushedColorOff			=	3,
	radioRendering_PushedColorOn			=	4,
	radioRendering_PushedColorMixed		=	5,
	radioRendering_DimmedColorOff			=	6,
	radioRendering_DimmedColorOn			=	7,
	radioRendering_DimmedColorMixed		=	8,
	radioRendering_NormalBWOff				=	9,
	radioRendering_NormalBWOn				=	10,
	radioRendering_NormalBWMixed			=	11,
	radioRendering_PushedBWOff				=	12,
	radioRendering_PushedBWOn				=	13,
	radioRendering_PushedBWMixed			=	14
};

enum
{
	radioButton_RadioWidth			=	12,
	radioButton_TextOffset			=	2
};

enum
{
	radio_Choice			=	15,
	radio_Rows				=	12,
	radio_Columns			=	12
};


// е COLOR CONSTANTS

const int 	B	=	colorRamp_Black;		//	Index for black
const int	W	=	colorRamp_White;		//	Index for white
const int	N	=	-1;						//	Index for no color which means draw nothing


//=====================================================================================
// DEFINES
//=====================================================================================

signed char LGARadioButton::sRadioRendering[radio_Choice][radio_Rows][radio_Columns] = 
{
	// OFF Normal - color	radioRendering_NormalColorOff
	{
		{ N,	N, 	N,		5, 	11,  	B,  	B, 	11,  	5, 	N, 	N, 	N },		//	0
		{ N, 	N,  	B, 	10,  	2,  	2,  	2,  	4, 	11,  	B, 	N, 	N },		//	1
		{ N,  B,  	4,  	2,  	1,  	W,  	W,  	W,  	2,  	7,  	B, 	N },		//	2
		{ 5, 	10,  	2,  	1,  	W,  	W,  	1,  	1,  	2,  	4, 	11,  	5 },		//	3
		{ 11, 2,  	1, 	W,  	W,  	1,  	1,  	2,  	2,  	4,  	7, 	11 },		//	4
		{ B,  2,  	W, 	W,  	1,  	1,  	2,  	2,  	4,  	4,  	7,  	B },		//	5
		{ B,  2,  	W,  	1,  	1,  	2,  	2,  	4,  	4,  	5,  	7,  	B },		//	6
		{ 11, 4, 	W,  	1,  	2,  	2,  	4,  	4,  	5,  	5,  	7, 	11 },		//	7
		{ 5, 	10,  	2,  	2,  	2,  	4,  	4,  	5,  	5,  	7, 	11,  	5 },		//	8
		{ N,  B,  	7,  	4,  	4,  	4,  	5,  	5,  	7,  	7,  	B, 	N },		//	9
		{ N, 	N,  	B, 	10,  	7,  	7,  	7,  	7, 	11,  	B,	 	N, 	N },		//	10
		{ N,	N,		N, 	5, 	11,  	B,  	B, 	11,  	5, 	N, 	N, 	N }		//	11
	},
	//	ON Normal - color		radioRendering_NormalColorOn
	{
		{ N, 	N, 	N,  	5, 	12,  	B,  	B, 	12,  	5, 	N, 	N, 	N },
		{ N, 	N, 	12,  	B, 	11, 	10, 	10, 	10, 	11, 	B, 	N, 	N },
		{ N, 	12, 	11, 	10,  	8,  	8,  	8,  	7,  	7,  	6,  	B,	 	N },
		{ 5,  B, 	10,  	8,  	B,  	B,  	B,  	B,  	6,  	6, 	11,  	5 },
		{ 12, 11,  	8,  	B,  	B,  	B, 	B,  	B,  	B,  	6,  	4, 	11 },
		{ B, 	10,  	8,  	B,  	B,  	B,  	B,  	B,  	B,  	4,  	4,  	B },
		{ B, 	10,  	8,  	B,  	B,  	B,  	B,  	B,  	B,  	4,  	2,  	B },
		{ 12, 10,  	7,  	B,  	B,  	B,  	B,  	B,  	B,  	2,  	W, 	11 },
		{ 5, 	12,  	7,  	6,  	B,  	B,  	B,  	B,  	2,  	W, 	11,  	5 },
		{ N, 	12,  	6,  	6,  	6,  	4,  	4,  	2, 	W,  	W,  	B, 	N },
		{ N, 	N, 	12, 	11,  	4,  	4,  	2,  	W, 	11,  	B, 	N, 	N },
		{ N, 	N, 	N,  	5, 	11,  	B,  	B, 	11,  	5, 	N, 	N, 	N }
	},
	//	MIXED Normal - color		radioRendering_NormalColorMixed
	{
		{ N, 	N, 	N,  	5, 	11,  	B,  	B, 	11,  	5, 	N, 	N, 	N },
		{ N, 	N,  	B, 	10,  	2,  	2,  	2,  	4, 	11,  	B, 	N, 	N },
		{ N,  B,  	4,  	2,  	1,  	W,  	W,  	W,  	2,  	7,  	B, 	N },
		{ 5, 	10,  	2,  	1,  	W,  	W,  	1,  	1,  	2,  	4, 	11,  	5 },
		{ 11, 2,  	1,  	W,  	W,  	1,  	1,  	2,  	2,  	4,  	7, 	11 },
		{ B,  2,  	W,  	B,  	B,  	B,  	B,  	B,  	B,  	4,  	7,  	B },
		{ B,  2,  	W,  	B,  	B,  	B,  	B,  	B,  	B,  	5,  	7,  	B },
		{ 11, 4,  	W,  	1,  	2,  	2,  	4,  	4,  	5,  	5,  	7, 	11 },
		{ 5, 	10,  	2,  	2,  	2,  	4,  	4,  	5,  	5,  	7, 	11,  	5 },
		{ N,  B,  	7,  	4,  	4,  	4,  	5,  	5,  	7,  	7,  	B, 	N },
		{ N,	N,  	B, 	10,  	7,  	7,  	7,  	7, 	11,  	B, 	N, 	N },
		{ N, 	N, 	N,  	5, 	11,  	B,  	B, 	11,  	5, 	N, 	N, 	N }
	},
	//	OFF Pushed - color		radioRendering_PushedColorOff
	{
		{ N, 	N, 	N,  	5, 	12, 	B,  	B, 	12,  	5, 	N, 	N, 	N },
		{ N, 	N, 	12,  	B, 	11, 	11, 	11, 	11, 	11,  	B,	 	N, 	N },
		{ N, 	12, 	11, 	11, 	10, 	10,  	9,  	9,  	9,  	8,  	B, 	N },
		{ 5,  B, 	11, 	10, 	10,  	9,  	9,  	8,  	8,  	8, 	11,  	5 },
		{ 12, 11, 	10, 	10,  	9,  	9,  	8,  	8,  	8,  	7,  	6, 	11 },
		{ B, 	11, 	10,  	9,  	9,  	8,  	8,  	8,  	7,  	7,  	6,  	B },
		{ B, 	11,  	9,  	9,  	8,  	8,  	8,  	7,  	7,  	6,  	6,  	B },
		{ 12, 11,  	9,  	8,  	8,  	8,  	7,  	7,  	6,  	6,  	4, 	11 },
		{ 5, 	12,  	9,  	8,  	8,  	7,  	7,  	6,  	6,  	4, 	10,  	5 },
		{ N, 	11,  	8,  	8,  	7,  	7,  	6,  	6,  	4,  	4,  	B, 	N },
		{ N,	N, 	12, 	11,  	6,  	6,  	6,  	4, 	10, 	B, 	N, 	N },
		{ N, 	N, 	N, 	5, 	11,  	B,  	B, 	11,  	5, 	N, 	N, 	N }
	},
	//	ON Pushed - color			radioRendering_PushedColorOn
	{
		{ N, 	N, 	N,  	5, 	12,  	B,  	B, 	12,  	5, 	N, 	N, 	N },
		{ N, 	N, 	12,  	B, 	11, 	11, 	11, 	11, 	11,  	B, 	N, 	N },
		{ N, 	12, 	11, 	11, 	10, 	10,  	9,  	9,  	9,  	8,  	B, 	N },
		{ 5,  B, 	11, 	10,  	B,  	B,  	B,  	B,  	8,  	8, 	11,  	5 },
		{ 12, 11, 	10, 	B,  	B,  	B,  	B,  	B,  	B,  	7,  	6, 	11 },
		{ B, 	11, 	10,  	B,  	B,  	B,  	B,  	B,  	B,  	7,  	6,  	B },
		{ B, 	11,  	9,  	B, 	B,  	B,  	B,  	B,  	B,  	6,  	6,  	B },
		{ 12, 11,  	9,  	B,  	B,  	B,  	B,  	B,  	B,  	6,  	4, 	11 },
		{ 5, 	12,  	9,  	8,  	B,  	B,  	B,  	B,  	6,  	4, 	10,  	5 },
		{ N, 	11,  	8,  	8,  	7,  	7,  	6,  	6,  	4,  	4,  	B, 	N },
		{ N, 	N, 	12, 	11,  	6,  	6,  	6,  	4, 	10,  	B, 	N, 	N },
		{ N, 	N, 	N,  	5, 	11,  	B,  	B, 	11,  	5, 	N, 	N, 	N }
	},
	//	MIXED Pushed - color		radioRendering_PushedColorMixed
	{
		{ N, 	N, 	N,  	5, 	12,  	B,  	B, 	12,  	5, 	N, 	N, 	N },
		{ N, 	N, 	12,  	B, 	11, 	11, 	11, 	11, 	11,  	B, 	N, 	N },
		{ N, 	2, 	11, 	11, 	10, 	10,  	9,  	9,  	9,  	8,  	B, 	N },
		{ 5, 	B, 	11, 	10, 	10,  	9,  	9,  	8,  	8,  	8, 	11,  	5 },
		{ 12, 11, 	10, 	10,  	9,  	9,  	8,  	8,  	8,  	7,  	6,  	B },
		{ B, 	11, 	10,  	B,  	B,  	B,  	B,  	B,  	B,  	7,  	6,  	B },
		{ B, 	11,  	9,  	B,  	B,  	B,  	B, 	B,  	B,  	6,  	6,  	B },
		{ 12, 11,  	9,  	8,  	8,  	8,  	7,  	7,  	6,  	6,  	4, 	11 },
		{ 5, 	12,  	9,  	8,  	8,  	7,  	7,  	6,  	6,  	4, 	10,  	5 },
		{ N, 	11, 	8,  	8,  	7,  	7,  	6,  	6,  	4,  	4,  	B, 	N },
		{ N, 	N, 	12, 	11,  	6,  	6,  	6,  	4, 	10,  	B, 	N, 	N },
		{ N, 	N, 	N,  	5, 	11, 	 B,  	B, 	11,  	5, 	N, 	N, 	N }
	},
	//	OFF Dimmed - color		radioRendering_DimmedColorOff
	{
		{ N, 	N, 	N,  	N,  	7,  	7,  	7,  	7,  	N, 	N, 	N, 	N },
		{ N, 	N,  	7,  	7,  	2,  	2,  	2,  	2,  	7,  	7, 	N,	 	N },
		{ N,  7,  	2,  	2,  	2,  	2,  	2,  	2,  	2,  	2,  	7,  	N },
		{ N,  7,  	2,  	2,  	2,  	2,  	2,  	2,  	2,  	2,  	7,  	N },
		{ 7,  2,  	2,  	2,  	2,  	2,  	2,  	2,  	2,  	2,  	2,  	7 },
		{ 7,  2,  	2, 	2,  	2, 	2,  	2,  	2,  	2,  	2,  	2,  	7 },
		{ 7,  2,  	2, 	2,  	2,  	2,  	2,  	2,  	2,  	2,  	2,  	7 },
		{ 7,  2,  	2,  	2,  	2,  	2,  	2,  	2,  	2,  	2,  	2,  	7 },
		{ N,  7,  	2,  	2,  	2,  	2,  	2,  	2,  	2,  	2,  	7,  	N },
		{ N,  7,  	2,  	2,  	2,  	2,  	2,  	2,  	2,  	2,  	7, 	N },
		{ N, 	N,  	7,  	7,  	2,  	2,  	2,  	2,  	7,  	7, 	N, 	N },
		{ N, 	N, 	N,  	N,  	7,  	7,  	7,  	7,  	N, 	N, 	N, 	N }
	},
	//	ON Dimmed - color			radioRendering_DimmedColorOn
	{
		{ N, 	N, 	N,  	N,  	7,  	7,  	7,  	7,  	N, 	N, 	N, 	N },
		{ N, 	N,  	7,  	7,  	2,  	2,  	2,  	2,  	7,  	7, 	N, 	N },
		{ N,  7,  	2,  	2,  	2, 	2,  	2,  	2,  	2,  	2,  	7, 	N },
		{ N,  7,  	2, 	2,  	8,  	8,  	8,  	8,  	2,  	2,  	7,  	N },
		{ 7,  2,  	2, 	8,  	8,  	8,  	8,  	8,  	8,  	2,  	2,  	7 },
		{ 7,  2,  	2,  	8,  	8,  	8,  	8,  	8,  	8,  	2,  	2,  	7 },
		{ 7,  2,  	2,  	8,  	8,  	8,  	8,  	8,  	8,  	2,  	2,  	7 },
		{ 7,  2,  	2,  	8,  	8,  	8,  	8,  	8,  	8,  	2,  	2,  	7 },
		{ N, 	7,  	2,  	2,  	8, 	8,  	8,  	8, 	2,  	2,  	7,  	N },
		{ N,  7,  	2,  	2,  	2,  	2,  	2,  	2,  	2,  	2,  	7, 	N },
		{ N, 	N,  	7,  	7,  	2,  	2,  	2, 	2,  	7,  	7, 	N, 	N },
		{ N, 	N, 	N,  	N,  	7,  	7,  	7,  	7,  	N, 	N, 	N, 	N }
	},
	//	MIXED Dimmed - color		radioRendering_DimmedColorMixed
	{
		{ N, 	N, 	N,  	N,  	7,  	7,  	7,  	7,  	N, 	N, 	N, 	N },
		{ N, 	N,  	7,  	7,  	2,  	2,  	2,  	2,  	7,  	7, 	N, 	N },
		{ N,  7,  	2,  	2,  	2,  	2,  	2,  	2,  	2,  	2,  	7, 	N },
		{ N,  7,  	2,  	2,  	2,  	2, 	2,  	2,  	2,  	2,  	7,  	N },
		{ 7,  2,  	2,  	2,  	2,  	2,  	2,  	2,  	2,  	2,  	2,  	7 },
		{ 7,  2,  	2,  	8,  	8,  	8,  	8,  	8,  	8,  	2,  	2,  	7 },
		{ 7,  2,  	2,  	8,  	8,  	8,  	8,  	8,  	8,  	2,  	2,  	7 },
		{ 7,  2,  	2,  	2,  	2,  	2,  	2,  	2,  	2,  	2,  	2,  	7 },
		{ N,  7,  	2,  	2,  	2,  	2,  	2,  	2,  	2,  	2,  	7,  	N },
		{ N,  7,  	2,  	2,  	2,  	2,  	2,  	2,  	2,  	2,  	7, 	N },
		{ N, 	N,  	7,  	7,  	2,  	2,  	2,  	2,  	7,  	7, 	N, 	N },
		{ N, 	N, 	N,  	N,  	7,  	7,  	7,  	7,  	N, 	N, 	N, 	N }
	},
	// OFF Normal - B&W			radioRendering_NormalBWOff
	{
		{ N,	N, 	N,		N, 	B,  	B,  	B, 	B,  	N, 	N, 	N, 	N },
		{ N, 	N,  	B, 	B,  	W,  	W,  	W,  	W, 	B,  	B, 	N, 	N },
		{ N,  B,  	W,  	W,  	W,  	W,  	W,  	W,  	W,  	W,  	B, 	N },
		{ N, 	B,  	W,  	W,  	W,  	W,  	W,  	W,  	W,  	W, 	B,  	N },
		{ B, 	W,  	W, 	W,  	W,  	W,  	W,  	W,  	W,  	W,  	W, 	B },
		{ B,  W,  	W, 	W,  	W,  	W,  	W,  	W,  	W,  	W,  	W,  	B },
		{ B,  W,  	W,  	W,  	W,  	W,  	W,  	W,  	W,  	W,  	W,  	B },
		{ B, 	W, 	W,  	W,  	W,  	W,  	W,  	W,  	W,  	W,  	W, 	B },
		{ N, 	B,  	W,  	W,  	W,  	W,  	W,  	W,  	W,  	W, 	B,  	N },
		{ N,  B,  	W,  	W,  	W,  	W,  	W,  	W,  	W,  	W,  	B, 	N },
		{ N, 	N,  	B, 	B,  	W,  	W,  	W,  	W, 	B,  	B,	 	N, 	N },
		{ N,	N,		N, 	N, 	B,  	B,  	B, 	B,  	N, 	N, 	N, 	N }
	},
	//	ON Normal - B&W			radioRendering_NormalBWOn
	{
		{ N, 	N, 	N,  	N, 	B,  	B,  	B, 	B,  	N, 	N, 	N, 	N },
		{ N, 	N, 	B,  	B, 	W, 	W, 	W, 	W, 	B, 	B, 	N, 	N },
		{ N, 	B, 	W, 	W,  	W,  	W,  	W,  	W,  	W,  	W,  	B,	 	N },
		{ N,  B, 	W,  	W,  	B,  	B,  	B,  	B,  	W,  	W, 	B,  	N },
		{ B, 	W,  	W,  	B,  	B,  	B, 	B,  	B,  	B,  	W,  	W, 	B },
		{ B, 	W,  	W,  	B,  	B,  	B,  	B,  	B,  	B,  	W,  	W,  	B },
		{ B, 	W,  	W,  	B,  	B,  	B,  	B,  	B,  	B,  	W,  	W,  	B },
		{ B, 	W,  	W,  	B,  	B,  	B,  	B,  	B,  	B,  	W,  	W, 	B },
		{ N, 	B,  	W,  	W,  	B,  	B,  	B,  	B,  	W,  	W, 	B,  	N },
		{ N, 	B,  	W,  	W,  	W,  	W,  	W,  	W, 	W,  	W,  	B, 	N },
		{ N, 	N, 	B, 	B,  	W,  	W,  	W,  	W, 	B,  	B, 	N, 	N },
		{ N, 	N, 	N,  	N, 	B,  	B,  	B, 	B,  	N, 	N, 	N, 	N }
	},
	//	MIXED Normal - B&W		radioRendering_NormalBWMixed
	{
		{ N, 	N, 	N,  	N, 	B,  	B,  	B, 	B,  	N, 	N, 	N, 	N },
		{ N, 	N,  	B, 	B,  	W,  	W,  	W,  	W, 	B,  	B, 	N, 	N },
		{ N,  B,  	W,  	W,  	W,  	W,  	W,  	W,  	W,  	W,  	B, 	N },
		{ N, 	B,  	W,  	W,  	W,  	W,  	W,  	W,  	W,  	W, 	B,  	N },
		{ B, 	W,  	W,  	W,  	W,  	W,  	W,  	W,  	W,  	W,  	W, 	B },
		{ B,  W,  	W,  	B,  	B,  	B,  	B,  	B,  	B,  	W,  	W,  	B },
		{ B,  W,  	W,  	B,  	B,  	B,  	B,  	B,  	B,  	W,  	W,  	B },
		{ B, 	W,  	W,  	W,  	W,  	W,  	W,  	W,  	W,  	W,  	W, 	B },
		{ N, 	W,  	W,  	W,  	W,  	W,  	W,  	W,  	W,  	W, 	B,  	N },
		{ N,  B,  	W,  	W,  	W,  	W,  	W,  	W,  	W,  	W,  	B, 	N },
		{ N,	N,  	B, 	B,  	W,  	W,  	W,  	W, 	B,  	B, 	N, 	N },
		{ N, 	N, 	N,  	N, 	B,  	B,  	B, 	B,  	N, 	N, 	N, 	N }
	},
	//	OFF Pushed - B&W			radioRendering_PushedBWOff
	{
		{ N, 	N, 	N,  	N, 	B, 	B,  	B, 	B,  	N, 	N, 	N, 	N },
		{ N, 	N, 	B,  	B, 	B, 	B, 	B, 	B, 	B,  	B,	 	N, 	N },
		{ N, 	B, 	B, 	B, 	W, 	W,  	W,  	W,  	B,  	B,  	B, 	N },
		{ N,  B, 	B, 	W, 	W,  	W,  	W,  	W,  	W,  	B, 	B,  	N },
		{ B, 	B, 	W, 	W,  	W,  	W,  	W,  	W,  	W,  	W,  	B, 	B },
		{ B, 	B, 	W,  	W,  	W,  	W,  	W,  	W,  	W,  	W,  	B,  	B },
		{ B, 	B,  	W,  	W,  	W,  	W,  	W,  	W,  	W,  	W,  	B,  	B },
		{ B, 	B,  	W,  	W,  	W,  	W,  	W,  	W,  	W,  	W,  	B, 	B },
		{ N, 	B,  	B,  	W,  	W,  	W,  	W,  	W,  	W,  	B, 	B,  	N },
		{ N, 	B,  	B,  	B,  	W,  	W,  	W,  	W,  	B,  	B,  	B, 	N },
		{ N,	N, 	B, 	B,  	B,  	B,  	B,  	B, 	B, 	B, 	N, 	N },
		{ N, 	N, 	N, 	N, 	B,  	B,  	B, 	B,  	N, 	N, 	N, 	N }
	},
	//	ON Pushed - B&W			radioRendering_PushedBWOn
	{
		{ N, 	N, 	N,  	N, 	B,  	B,  	B, 	B,  	N, 	N, 	N, 	N },
		{ N, 	N, 	B,  	B, 	B, 	B, 	B, 	B, 	B,  	B, 	N, 	N },
		{ N, 	B, 	B, 	B, 	W, 	W,  	W,  	W,  	B,  	B,  	B, 	N },
		{ N,  B, 	B, 	W,  	B,  	B,  	B,  	B,  	W,  	B, 	B,  	N },
		{ B, 	B, 	W, 	B,  	B,  	B,  	B,  	B,  	B,  	W,  	B, 	B },
		{ B, 	B, 	W,  	B,  	B,  	B,  	B,  	B,  	B,  	W,  	B,  	B },
		{ B, 	B,  	W,  	B, 	B,  	B,  	B,  	B,  	B,  	W,  	B,  	B },
		{ B, 	B,  	W,  	B,  	B,  	B,  	B,  	B,  	B,  	W,  	B, 	B },
		{ N, 	B,  	B,  	W,  	B,  	B,  	B,  	B,  	W,  	B, 	B,  	N },
		{ N, 	B,  	B,  	B,  	W,  	W,  	W,  	W,  	B,  	B,  	B, 	N },
		{ N, 	N, 	B, 	B,  	B,  	B,  	B,  	B, 	B,  	B, 	N, 	N },
		{ N, 	N, 	N,  	N, 	B,  	B,  	B, 	B,  	N, 	N, 	N, 	N }
	},
	//	MIXED Pushed - B&W		radioRendering_PushedBWMixed
	{
		{ N, 	N, 	N,  	N, 	B,  	B,  	B, 	B,  	N, 	N, 	N, 	N },
		{ N, 	N, 	B,  	B, 	B, 	B, 	B, 	B, 	B,  	B, 	N, 	N },
		{ N, 	B, 	B, 	B, 	W, 	W,  	W,  	W,  	B,  	B,  	B, 	N },
		{ N, 	B, 	B, 	W, 	W,  	W,  	W,  	W,  	W,  	B, 	B,  	N },
		{ B, 	B, 	W, 	W,  	W,  	W,  	W,  	W,  	W,  	W,  	B,  	B },
		{ B, 	B, 	W,  	B,  	B,  	B,  	B,  	B,  	B,  	W,  	B,  	B },
		{ B, 	B,  	W,  	B,  	B,  	B,  	B, 	B,  	B,  	W,  	B,  	B },
		{ B, 	B,  	W,  	W,  	W,  	W,  	W,  	W,  	W,  	W,  	B, 	B },
		{ N, 	B,  	B,  	W,  	W,  	W,  	W,  	W,  	W,  	B, 	B,  	N },
		{ N, 	B, 	B,  	B,  	W,  	W,  	W,  	W,  	B,  	B,  	B, 	N },
		{ N, 	N, 	B, 	B,  	B,  	B,  	B,  	B, 	B,  	B, 	N, 	N },
		{ N, 	N, 	N,  	N, 	B, 	B,  	B, 	B,  	N, 	N, 	N, 	N }
	}
};


#pragma mark === CLASS: LGARadioButton

//=====================================================================================
// CLASS:	LGARadioButton
//=====================================================================================

#pragma mark -
#pragma mark === INITIALIZATION

//=====================================================================================
// ее INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LGARadioButton::LGARadioButton					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
// Default constructor

LGARadioButton::LGARadioButton ()
{	

	mHilited = false;
	
}	//	LGARadioButton::LGARadioButton


//-------------------------------------------------------------------------------------
// LGARadioButton::LGARadioButton					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
// Copy constructor

LGARadioButton::LGARadioButton (	const LGARadioButton &inOriginal )
								:	LControl ( inOriginal ),
									LGATitleMixin ( inOriginal )
{	

	mHilited = inOriginal.mHilited;
	
}	//	LGARadioButton::LGARadioButton


//-------------------------------------------------------------------------------------
// LGARadioButton::LGARadioButton					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
// Stream constructor

LGARadioButton::LGARadioButton (	LStream	*inStream ) 
							: LControl ( inStream )
{

	SInt16	controlKind;
	SInt32	macRefCon;
	
	// е We read this but don't do anything with it
	*inStream >> controlKind;

	// е Read in the text trait ID
	*inStream >> mTextTraitsID;
	
	// е Read in the title string
	inStream->ReadPString ( mTitle );
	
	// е We read this but don't do anything with it
	*inStream >> macRefCon;
	
	// е Setup the hilite flag
	mHilited = false;

	// е Make sure we setup the max value for the control in case the user
	// did not do it in the PPob
	if ( GetMaxValue () < 2 )
		SetMaxValue ( 2 );
	
}	//	LGARadioButton::LGARadioButton


//-------------------------------------------------------------------------------------
// LGARadioButton::LGARadioButton					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
// Parameterized constructor

LGARadioButton::LGARadioButton (	const SPaneInfo 			&inPaneInfo,
											const SControlInfo 		&inControlInfo,
											Str255						inTitle,
											ResIDT						inTextTraitsID )
									: LControl ( 	inPaneInfo,
														inControlInfo.valueMessage,
														inControlInfo.value,
														inControlInfo.minValue,
														inControlInfo.maxValue )
{	

	// е Setup our fields
	mTitle = inTitle;
	mTextTraitsID = inTextTraitsID;
	
	// е Setup the hilite flag
	mHilited = false;

	// е Make sure we setup the max value for the control in case the user
	// did not do it in the PPob
	if ( GetMaxValue () < 2 )
		SetMaxValue ( 2 );
	
	
}	//	LGARadioButton::LGARadioButton


//-------------------------------------------------------------------------------------
// LGARadioButton::~LGARadioButton				---<<е DESTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LGARadioButton::~LGARadioButton ()
{
}	//	LGARadioButton::~LGARadioButton


#pragma mark -
#pragma mark === ACCESSORS

//=====================================================================================
// ее ACCESSORS
//-------------------------------------------------------------------------------------
// LGARadioButton::GetDescriptor
//-------------------------------------------------------------------------------------

StringPtr
LGARadioButton::GetDescriptor ( Str255	outDescriptor ) const
{

	return LString::CopyPStr ( mTitle, outDescriptor );

}	//	LGARadioButton::GetDescriptor


//-------------------------------------------------------------------------------------
// LGARadioButton::SetDescriptor
//-------------------------------------------------------------------------------------

void
LGARadioButton::SetDescriptor (	ConstStr255Param	 inDescriptor )
{

	mTitle = inDescriptor;
	Refresh ();

}	//	LGARadioButton::SetDescriptor


//-------------------------------------------------------------------------------------
// LGARadioButton::SetHiliteState
//-------------------------------------------------------------------------------------

void			
LGARadioButton::SetHiliteState	(	Boolean	inHiliteState )
{

	// е We only change the state if we have to
	if ( mHilited != inHiliteState )
	{
		mHilited = inHiliteState;
		
		// е Because the state has changed we need to make sure 
		// that things get redrawn so that the change can take effect
		Draw ( nil );
	}
			
}	//	LGARadioButton::SetHiliteState


//-------------------------------------------------------------------------------------
// LGARadioButton::SetValue
//-------------------------------------------------------------------------------------

void			
LGARadioButton::SetValue	( SInt32	inValue )
{

	// е We only do something if the value is changing
	if ( inValue != GetValue ())
	{
		// е Call our parent to handle the setting of the value
		LControl::SetValue ( inValue );
		
		// е And make sure we get the hilite state changed, if it is not hilited
		// thenwe simply get things redrawn
		if ( IsHilited ())
			SetHiliteState ( false );
		else
			Draw ( nil );
			
		// е Broadcast the fact that we have turned the button on if that is the case
		if ( inValue == Button_On ) 
			BroadcastMessage ( msg_ControlClicked, (void*) this );
	}
	
}	//	LGARadioButton::SetValue


//-------------------------------------------------------------------------------------
// LGARadioButton::CalcTitleRect
//-------------------------------------------------------------------------------------

void
LGARadioButton::CalcTitleRect	( Rect	&outRect )
{

	// е Get the text traits ID for the control
	ResIDT	textTID = GetTextTraitsID ();

	// е Get the port setup with the text traits
	UTextTraits::SetPortTextTraits ( textTID );
	
	// е Figure out the height of the text for the selected font
	FontInfo fi;
	::GetFontInfo ( &fi );
	SInt16		textHeight = fi.ascent + fi.descent;
		
	// е Get the local inset frame rectangle
	CalcLocalFrameRect ( outRect );
	SInt16		frameHeight = UGraphicsUtilities::RectHeight ( outRect );
	::InsetRect ( &outRect, 4, 2 );
	
	// е If the text height is greater than the frame height the
	// constrain it to the frame height
	if ( frameHeight < textHeight )
		textHeight = frameHeight;
		
	// е Now we modify the title rect so that it is to the right of the radio
	// control itself
	outRect.top += ((frameHeight - textHeight) / 2) - radioButton_TextOffset;
	outRect.bottom = outRect.top + textHeight;
	outRect.left += radioButton_RadioWidth + radioButton_TextOffset;

}	//	LGARadioButton::CalcTitleRect


//-------------------------------------------------------------------------------------
// LGARadioButton::CalcLocalControlRect
//-------------------------------------------------------------------------------------

void
LGARadioButton::CalcLocalControlRect ( Rect &outRect )
{
	// е Figure out the frame rect for the radio button
	CalcLocalFrameRect ( outRect );
	SInt16		frameHeight = UGraphicsUtilities::RectHeight ( outRect );

	// е Adjust the rect such that it is always centered vertically within
	// the radio buttons frame rect
	outRect.left += 1;
	outRect.right = outRect.left + radioButton_RadioWidth;
	outRect.top += ((frameHeight - radioButton_RadioWidth) / 2) + 1;
	outRect.bottom = outRect.top + radioButton_RadioWidth;
	
}	//	LGARadioButton::CalcLocalControlRect


#pragma mark -
#pragma mark === ENABLING & DISABLING

//=====================================================================================
// ее ENABLING & DISABLING
//-------------------------------------------------------------------------------------
// LGARadioButton::EnableSelf
//-------------------------------------------------------------------------------------
//

void
LGARadioButton::EnableSelf ()
{

	// е Get things redrawn so that we can see the state change
	Draw ( nil );

}	//	LGARadioButton::EnableSelf


//-------------------------------------------------------------------------------------
// LGARadioButton::DisableSelf
//-------------------------------------------------------------------------------------
//

void
LGARadioButton::DisableSelf ()
{

	// е Get things redrawn so that we can see the state change
	Draw ( nil );

}	//	LGARadioButton::DisableSelf


#pragma mark -
#pragma mark === ACTIVATION

//=====================================================================================
// ее ACTIVATION
//-------------------------------------------------------------------------------------
// LGARadioButton::ActivateSelf
//-------------------------------------------------------------------------------------

void
LGARadioButton::ActivateSelf ()
{

	// е Get everything redrawn so the changes take effect
	if ( mActive == triState_On )
		Refresh ();
					
}	//	LGARadioButton::ActivateSelf


//-------------------------------------------------------------------------------------
// LGARadioButton::DeactivateSelf
//-------------------------------------------------------------------------------------

void
LGARadioButton::DeactivateSelf ()
{

	// е Get everything redrawn so the changes take effect
	if ( mActive == triState_Off || mActive == triState_Latent )
		Refresh ();
				
}	//	LGARadioButton::DeactivateSelf


#pragma mark -
#pragma mark === DRAWING

//=====================================================================================
// ее DRAWING
//-------------------------------------------------------------------------------------
// LGARadioButton::DrawSelf
//-------------------------------------------------------------------------------------

void
LGARadioButton::DrawSelf	()
{

	StColorPenState::Normalize ();
	
	// е Get the radio drawn in its various states
	if ( IsEnabled () && IsActive ())
	{
		if ( IsHilited ())
			DrawRadioHilited ();
		else
			DrawRadioNormal ();
	}
	else
		DrawRadioDimmed ();

	// е Now get the title for the radio button drawn
	if ( HasTitle ())
		DrawRadioTitle ();
	
}	//	LGARadioButton::DrawSelf


//-------------------------------------------------------------------------------------
// LGARadioButton::DrawRadioTitle
//-------------------------------------------------------------------------------------

void
LGARadioButton::DrawRadioTitle	()
{
	
	StColorPenState	theColorPenState;
	StColorPenState::Normalize ();
	StTextState			theTextState;
		
	// е Get some loal variables setup including the rect for the title
	ResIDT	textTID = GetTextTraitsID ();
	Rect	titleRect;
	
	// е Setup the port text style fomr the text traits
	UTextTraits::SetPortTextTraits ( textTID );
	
	// е Setup the justification which is always left justified
	SInt16	titleJust = teFlushDefault;
	
	// е Calculate the title rect
	CalcTitleRect ( titleRect );
	
	// е Get the control's title
	Str255 controlTitle;
	GetDescriptor ( controlTitle );

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
			if ( !IsHilited ())
			{
				// е If the control is dimmed then we use the grayishTextOr 
				// transfer mode to draw the text
				if ( !IsEnabled ())
					::TextMode ( grayishTextOr );
				
				// е Get the actual text drawn with the appropriate justification
			UStringDrawing::DrawJustifiedString ( 	controlTitle,
																titleRect,
																titleJust );
			}
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
				
			// е Get the actual text drawn with the appropriate justification
			UStringDrawing::DrawJustifiedString ( 	controlTitle,
																titleRect,
																titleJust );
		}	
	}

}	//	LGARadioButton::DrawRadioTitle


//-------------------------------------------------------------------------------------
// LGARadioButton::DrawRadioNormal
//-------------------------------------------------------------------------------------

void
LGARadioButton::DrawRadioNormal	()
{
	
	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalControlRect ( localFrame );

	// е Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( localFrame );
	SInt16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		// е BLACK & WHITE
		if ( depth < 4 )
		{
			if ( IsMixedState ())
				DrawRadioChoice ( radioRendering_NormalBWMixed, localFrame );
			else if ( IsSelected ())
				DrawRadioChoice ( radioRendering_NormalBWOn, localFrame );
			else
				DrawRadioChoice ( radioRendering_NormalBWOff, localFrame );	
		}
		// е COLOR
		else
		{
			if ( IsMixedState ())
				DrawRadioChoice ( radioRendering_NormalColorMixed, localFrame );
			else if ( IsSelected ())
				DrawRadioChoice ( radioRendering_NormalColorOn, localFrame );
			else
				DrawRadioChoice ( radioRendering_NormalColorOff, localFrame );	
		}	
	}
	
}	//	LGARadioButton::DrawRadioNormal


//-------------------------------------------------------------------------------------
// LGARadioButton::DrawRadioHilited
//-------------------------------------------------------------------------------------

void
LGARadioButton::DrawRadioHilited	()
{
	
	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalControlRect ( localFrame );

	// е Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( localFrame );
	SInt16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		// е BLACK & WHITE
		if ( depth < 4 )
		{
			if ( IsHilited ())
			{
				if ( IsMixedState ())
					DrawRadioChoice ( radioRendering_PushedBWMixed, localFrame );
				else if ( IsSelected ())
					DrawRadioChoice ( radioRendering_PushedBWOn, localFrame );
				else
					DrawRadioChoice ( radioRendering_PushedBWOff, localFrame );
			}
		}
		// е COLOR
		else
		{
			if ( IsHilited ())
			{
				if ( IsMixedState ())
					DrawRadioChoice ( radioRendering_PushedColorMixed, localFrame );
				else if ( IsSelected ())
					DrawRadioChoice ( radioRendering_PushedColorOn, localFrame );
				else
					DrawRadioChoice ( radioRendering_PushedColorOff, localFrame );
			}
		}	
	}
	
}	//	LGARadioButton::DrawRadioHilited


//-------------------------------------------------------------------------------------
// LGARadioButton::DrawRadioDimmed
//-------------------------------------------------------------------------------------

void
LGARadioButton::DrawRadioDimmed	()
{
	
	// е Get the frame for the control
	Rect	localFrame;
	CalcLocalControlRect ( localFrame );

	// е Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( localFrame );
	SInt16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		// е BLACK & WHITE
		if ( depth < 4 )
		{
			if ( !IsEnabled () || !IsActive ())
			{
				// е For the dimmed cases we need to draw using a gray pattern
				// provided by the toolbox
				if ( IsMixedState ())
					DrawRadioChoice ( radioRendering_NormalBWMixed, localFrame );
				else if ( IsSelected ())
					DrawRadioChoice ( radioRendering_NormalBWOn, localFrame );
				else
					DrawRadioChoice ( radioRendering_NormalBWOff, localFrame );
			}
		}
		// е COLOR
		else
		{
			if ( !IsEnabled () || !IsActive ())
			{
				if ( IsMixedState ())
					DrawRadioChoice ( radioRendering_DimmedColorMixed, localFrame );
				else if ( IsSelected ())
					DrawRadioChoice ( radioRendering_DimmedColorOn, localFrame );
				else
					DrawRadioChoice ( radioRendering_DimmedColorOff, localFrame );
			}
		}	
	}
	
}	//	LGARadioButton::DrawRadioDimmed


//-------------------------------------------------------------------------------------
// LGARadioButton::DrawRadioChoice
//-------------------------------------------------------------------------------------

void
LGARadioButton::DrawRadioChoice	( SInt16	inChoice,
												Rect	&inFrame )
{
	
		SInt16 colorindex;
		for ( register int i = 0; i < radio_Rows; i++ )
			for ( register int j = 0; j < radio_Columns; j++ )
				{
					colorindex = sRadioRendering[inChoice][i][j];
					if ( colorindex != N )
					{
						RGBColor theColor = UGAColorRamp::GetColor(colorindex);
						UGraphicsUtilities::PaintColorPixel ( inFrame.left + j, 
														inFrame.top + i, 
														&theColor);
					}
				}
		
}	//	LGARadioButton::DrawRadioChoice


#pragma mark -
#pragma mark === MOUSE TRACKING

//=====================================================================================
// ее MOUSE TRACKING
//-------------------------------------------------------------------------------------
// LGARadioButton::HotSpotAction
//-------------------------------------------------------------------------------------

void
LGARadioButton::HotSpotAction	(	SInt16		/* inHotSpot */,
											Boolean	inCurrInside,
											Boolean	inPrevInside )
{

	// е If the mouse moved in or out of the hot spot handle the
	// hiliting of the control which is slightly different for
	// each of the modes supported by the control
	if ( inCurrInside != inPrevInside )
	{

		// е Things are a little trickier if we ar a radio button
		// as we only need to toggle the state if the control is
		// not already selected, we also want to avoid doing any
		// unnecessary drawing
		if ( inCurrInside )
		{
			if ( !IsHilited ())
				SetHiliteState ( true );
		}
		else
			SetHiliteState ( false );
	}
		
}	//	LGARadioButton::HotSpotAction


//-------------------------------------------------------------------------------------
// LGARadioButton::HotSpotResult
//-------------------------------------------------------------------------------------

void
LGARadioButton::HotSpotResult	(	SInt16 inHotSpot )
{
	
	// е Make sure that by default we turn the hiliting off
	HotSpotAction ( inHotSpot, true, true );
		
	// е Do the right thing about getting the value set after the radio has been
	// clicked in its hotspot, NOTE: this method assumes the entire control is
	// the hotspot
	// е If the radio is not currently selected then we need to
	// ensure that the radio is 'on', once it is on we never
	// change it back as it is a radio button and they only get
	// turned off by someone else when another radio button in
	// a group is selected
	if ( !IsSelected ())
	{
		SetValue ( Button_On );
	}
	
	// е Make sure the transform gets restored and everything gets redrawn
	SetHiliteState ( false );
	
}	//	LGARadioButton::HotSpotResult


