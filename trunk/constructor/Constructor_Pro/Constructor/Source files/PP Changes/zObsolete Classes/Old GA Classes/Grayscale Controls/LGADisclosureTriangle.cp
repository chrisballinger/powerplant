// Copyright й2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyrightй 1996 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< е >>-----------------------------------------

FILE:					LGADisclosureTriangle.cp

CLASSES:				LGADisclosureTriangle

DESCRIPTION:		Implementation file for LGADisclosureTriangle

AUTHOR:				Robin Mair

CREATION DATE :	96.06.18

CHANGE HISTORY :

		97.04.20		rtm	Removed the CreateFromStream method.
		97.02.08		rtm	Changed the rendering of the control such that is it drawn as
								disabled when it is deactivated, this is matches the approach
								Apple is taking with the Apearance Manager.
		97.01.27		rtm	Fixed a few places where drawing could potentially take place
								whenthe control was not visible.
		97.01.24		rtm	Added a copy constructor.
		96.01.20		rtm	Added a parameterized constructor so that the pane can be built
								procedurally.
		96.12.06		rtm	Tweaked some of the color constants as the indices have changed
								a little in the color ramp.
		96.12.03		rtm	Moved the color constants defined in the header file for the
								UGAColorRamp, to this source file so that they can be
								removed from the header file.
		96.10.13		rtm	Skanky fix used to figure out the background color when the
								triangle is across multiple devices, this is used to wipe the
								background behind the control during the animation transitions
								in order to make the drawing smoother.
		96.10.12		rtm	Lot of refinements to make this class draw better in B & W in
								the dimmed state, it now actually uses the pressed rendering
								with the PenPat set to gray, otherwise in certain positions
								some of the edges for the triangle disapearred which looked
								really bad.
		96.10.08		rtm	Got rid of DrawDisclosureTriangle by moving its contents into
								DrawSelf.
		96.10.04		rtm	Replaced all SetCPixel calls with a new PaintColorPixel call
								that has been added to UGraphicsUtilities. This should get
								around the problems with using SetCPixel.
		96.09.04		rtm	PowerPlant conformance changes.
		96.09.02		rtm	Tweaked the way the SetValue method works so that it does the
								right thing when the controls value is being set procedurally
								it now only makes the change if the value in fact changes and
								it always does a redraw when the value does change.
		96.09.02		rtm	Added overrides of EnableSelf and DisableSelf so that we can
								handle getting the control redrawn when the state changes.
		96.06.25		rtm	Added a whole new set of pixmaps for the various black & white
								states of the triangle - what a hassle when will we ever be
								free of the tyranny of B&W!!!
		96.06.18		rtm	Initial creation of file

---------------------------------------<< е >>-----------------------------------------
*/

#ifdef PowerPlant_PCH
	#include PowerPlant_PCH
#endif

// е GRAYSCALE HEADERS
#include <LGADisclosureTriangle.h>
#include <UGraphicsUtilities.h>
#include <UGAColorRamp.h>

// е POWERPLANT HEADERS
#include <LView.h>
#include <LStream.h>
#include <UDrawingState.h>
#include <UDrawingUtils.h>
#include <PP_Messages.h>


//=====================================================================================
// DEFINES
//=====================================================================================

//=====================================================================================
// CONSTANTS
//=====================================================================================

enum
{
	disclosure_EnabledRight				=	0,
	disclosure_PressedRight				=	1,
	disclosure_IntermediateRight		=	2,
	disclosure_PressedDown				=	3,
	disclosure_EnabledDown				=	4,
	disclosure_EnabledLeft				=	5,
	disclosure_PressedLeft				=	6,
	disclosure_IntermediateLeft		=	7,
	disclosure_DisabledRight			=	8,
	disclosure_DisabledDown				=	9,
	disclosure_DisabledLeft				=	10,
	disclosure_EnabledRightBW			=	11,
	disclosure_PressedRightBW			=	12,
	disclosure_IntermediateRightBW	=	13,
	disclosure_PressedDownBW			=	14,
	disclosure_EnabledDownBW			=	15,
	disclosure_EnabledLeftBW			=	16,
	disclosure_PressedLeftBW			=	17,
	disclosure_IntermediateLeftBW		=	18
};

enum
{
	disclosure_Width						=	12
};

enum
{
	disclosure_Choice						=	19,
	disclosure_Rows						=	12,
	disclosure_Columns					=	12
};


// е COLOR CONSTANTS

const int 	B	=	colorRamp_Black;		//	Index for black
const int	W	=	colorRamp_White;		//	Index for white
const int	N	=	-1;						//	Index for no color which means draw nothing
const int	A1	=	colorRamp_GrayA1;		//	Additional shade of gray
const int	A2	=	colorRamp_GrayA2;		//	Additional shade of gray
const int	P1	=	colorRamp_Purple1;	//	Shade of purple for the disclosure triangle
const int	P2	=	colorRamp_Purple2;	//	Shade of purple for the disclosure triangle
const int	P3	=	colorRamp_Purple3;	//	Shade of purple for the disclosure triangle
const int	P4	=	colorRamp_Purple4;	//	Shade of purple for the disclosure triangle


//=====================================================================================
// DEFINES
//=====================================================================================

signed char LGADisclosureTriangle::sDisclosurePixMap[disclosure_Choice][disclosure_Rows][disclosure_Columns] = 
{
	// ENABLED RIGHT - color	disclosure_EnabledRight
	{
		{ N,	N, 	N, 	N, 	B,		N, 	N,  	N,  	N, 	N,  	N, 	N },		//	0
		{ N,	N, 	N, 	N, 	B,		B, 	N,  	N,  	N, 	N,  	N, 	N },		//	1
		{ N,	N, 	N, 	N, 	B,		P1, 	B,  	N,  	N, 	N,  	N, 	N },		//	2
		{ N,	N, 	N, 	N, 	B,		P1, 	P2,  	B,  	N, 	N,  	N, 	N },		//	3
		{ N,	N, 	N, 	N, 	B,		P1, 	P2,  	P2,  	B, 	N,  	N, 	N },		//	4
		{ N,	N, 	N, 	N, 	B,		P1, 	P2,  	P2,  	P3, 	B,  	N, 	N },		//	5
		{ N,	N, 	N, 	N, 	B,		P1, 	P2,  	P3,  	B, 	7,  	4, 	N },		//	6
		{ N,	N, 	N, 	N, 	B,		P1, 	P3,  	B,  	7, 	4,  	N, 	N },		//	7
		{ N,	N, 	N, 	N, 	B,		P2, 	B,  	7,  	4, 	N,  	N, 	N },		//	8
		{ N,	N, 	N, 	N, 	B,		B, 	7,  	4,  	N, 	N,  	N, 	N },		//	9
		{ N,	N, 	N, 	N, 	B,		7, 	4,  	N,  	N, 	N,  	N, 	N },		//	10
		{ N,	N, 	N, 	N, 	N,		4, 	N,  	N,  	N, 	N,  	N, 	N }		//	11
	},
	//	PRESSED RIGHT - color		disclosure_PressedRight
	{
		{ N,	N, 	N, 	N, 	B,		N, 	N,  	N,  	N, 	N,  	N, 	N },		//	0
		{ N,	N, 	N, 	N, 	B,		B, 	N,  	N,  	N, 	N,  	N, 	N },		//	1
		{ N,	N, 	N, 	N, 	B,		P4, 	B,  	N,  	N, 	N,  	N, 	N },		//	2
		{ N,	N, 	N, 	N, 	B,		P4, 	P4,  	B,  	N, 	N,  	N, 	N },		//	3
		{ N,	N, 	N, 	N, 	B,		P4, 	P4,  	P4,  	B, 	N,  	N, 	N },		//	4
		{ N,	N, 	N, 	N, 	B,		P4, 	P4,  	P4,  	P4, 	B,  	N, 	N },		//	5
		{ N,	N, 	N, 	N, 	B,		P4, 	P4,  	P4,  	B, 	N,  	N, 	N },		//	6
		{ N,	N, 	N, 	N, 	B,		P4, 	P4,  	B,  	N, 	N,  	N, 	N },		//	7
		{ N,	N, 	N, 	N, 	B,		P4, 	B,  	N,  	N, 	N,  	N, 	N },		//	8
		{ N,	N, 	N, 	N, 	B,		B, 	N,  	N,  	N, 	N,  	N, 	N },		//	9
		{ N,	N, 	N, 	N, 	B,		N, 	N,  	N,  	N, 	N,  	N, 	N },		//	10
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N }		//	11
	},
	//	INTERMEDIATE RIGHT - color		disclosure_IntermediateRight
	{
		{ N,	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N, 	N },		//	0
		{ N,	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	B, 	N, 	N },		//	1
		{ N,	N,		N, 	N,  	N,  	N, 	N,  	N, 	B, 	B, 	N, 	N },		//	2
		{ N,	N,		N, 	N,  	N,  	N, 	N,  	B, 	P4, 	B, 	N, 	N },		//	3
		{ N,	N,		N, 	N,  	N,  	N, 	B,  	P4, 	P4, 	B, 	N, 	N },		//	4
		{ N,	N,		N, 	N,  	N,  	B, 	P4,  	P4, 	P4, 	B, 	N, 	N },		//	5
		{ N,	N,		N, 	N,  	B,  	P4, 	P4,  	P4, 	P4, 	B, 	N, 	N },		//	6
		{ N,	N,		N, 	B,  	P4,  	P4, 	P4,  	P4, 	P4, 	B, 	N, 	N },		//	7
		{ N,	N,		B, 	P4,  	P4,  	P4, 	P4,  	P4, 	P4, 	B, 	N, 	N },		//	8
		{ N, 	B,		B, 	B,  	B,  	B, 	B,  	B, 	B, 	B, 	N, 	N },		//	9
		{ N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N, 	N },		//	10
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N }		//	11
	},
	//	PRESSED DOWN - color		disclosure_PressedDown
	{
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N },		//	0
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N },		//	1
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N },		//	2
		{ B,	B, 	B,		B, 	B,  	B,  	B, 	B,  	B, 	B, 	B, 	N },		//	3
		{ N,	B, 	P4,	P4, 	P4,  	P4,  	P4, 	P4,  	P4, 	B, 	N, 	N },		//	4
		{ N,	N, 	B,		P4, 	P4,  	P4,  	P4, 	P4,  	B, 	N, 	N, 	N },		//	5
		{ N,	N, 	N,		B, 	P4,  	P4,  	P4, 	B,  	N, 	N, 	N, 	N },		//	6
		{ N,	N, 	N,		N, 	B,  	P4,  	B, 	N,  	N, 	N, 	N, 	N },		//	7
		{ N,	N, 	N,		N, 	N,  	B,  	N, 	N,  	N, 	N, 	N, 	N },		//	8
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N },		//	9
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N },		//	10
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N }		//	11
	},
	//	ENABLED DOWN - color			disclosure_EnabledDown
	{
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N },		//	0
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N },		//	1
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N },		//	2
		{ B,	B, 	B,		B, 	B,  	B,  	B, 	B,  	B, 	B, 	B, 	N },		//	3
		{ N,	B, 	P1,	P1, 	P1,  	P1,  	P1, 	P1,  	P2, 	B, 	7, 	4 },		//	4
		{ N,	N, 	B,		P2, 	P2,  	P2,  	P2, 	P3,  	B, 	7, 	4, 	N },		//	5
		{ N,	N, 	N,		B, 	P2,  	P2,  	P3, 	B,  	7, 	4, 	N, 	N },		//	6
		{ N,	N, 	N,		N, 	B,  	P3,  	B, 	7,  	4, 	N, 	N, 	N },		//	7
		{ N,	N, 	N,		N, 	N,  	B,  	7, 	4,  	N, 	N, 	N, 	N },		//	8
		{ N,	N, 	N,		N, 	N,  	N,  	4, 	N,  	N, 	N, 	N, 	N },		//	9
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N },		//	10
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N }		//	11
	},
	//	ENABLED LEFT - color			disclosure_EnabledLeft
	{
		{ N,	N, 	N,  	N,  	N, 	N,  	N, 	B, 	N, 	N,  	N,  	N },		//	0
		{ N,	N, 	N,  	N,  	N, 	N,  	B, 	B, 	7, 	N,  	N, 	N },		//	1
		{ N,	N, 	N,  	N,  	N, 	B,  	P1, 	B, 	7, 	N,  	N, 	N },		//	2
		{ N,	N, 	N,  	N,  	B, 	P2,  	P1, 	B, 	7, 	N,  	N, 	N },		//	3
		{ N,	N, 	N,  	B,  	P2, 	P2,  	P1, 	B, 	7, 	N,  	N, 	N },		//	4
		{ N,	N, 	B,  	P3,  	P2, 	P2,  	P1, 	B, 	7, 	N,  	N, 	N },		//	5
		{ N,	N, 	N,  	B,  	P3, 	P2,  	P1, 	B, 	7, 	N,  	N, 	N },		//	6
		{ N,	N, 	N,  	N,  	B, 	P3,  	P1, 	B, 	7, 	N,  	N, 	N },		//	7
		{ N,	N, 	N,  	N,  	N, 	B,  	P2, 	B, 	7, 	N,  	N, 	N },		//	8
		{ N,	N, 	N,  	N,  	N, 	N,  	B, 	B, 	7, 	N,  	N, 	N },		//	9
		{ N,	N, 	N,  	N,  	N, 	N,  	N, 	B, 	7, 	N,  	N, 	N },		//	10
		{ N,	N, 	N,  	N,  	N, 	N,  	N, 	N, 	7, 	N,  	N, 	N }		//	11
	},
	//	PRESSES LEFT - color			disclosure_PressedLeft
	{
		{ N,	N, 	N,  	N,  	N, 	N,  	N, 	B, 	N, 	N,  	N, 	N },		//	0
		{ N,	N, 	N,  	N,  	N, 	N,  	B, 	B, 	N, 	N,  	N, 	N },		//	1
		{ N,	N, 	N,  	N,  	N, 	B,  	P4, 	B, 	N, 	N,  	N, 	N },		//	2
		{ N,	N, 	N,  	N,  	B, 	P4,  	P4, 	B, 	N, 	N,  	N, 	N },		//	3
		{ N,	N, 	N,  	B,  	P4, 	P4,  	P4, 	B, 	N, 	N,  	N, 	N },		//	4
		{ N,	N, 	B,  	P4,  	P4, 	P4,  	P4, 	B, 	N, 	N,  	N, 	N },		//	5
		{ N,	N, 	N,  	B,  	P4, 	P4,  	P4, 	B, 	N, 	N,  	N, 	N },		//	6
		{ N,	N, 	N,  	N,  	B, 	P4,  	P4, 	B, 	N, 	N,  	N, 	N },		//	7
		{ N,	N, 	N,  	N,  	N, 	B,  	P4, 	B, 	N, 	N,  	N, 	N },		//	8
		{ N,	N, 	N,  	N,  	N, 	N,  	B, 	B, 	N, 	N,  	N, 	N },		//	9
		{ N,	N, 	N,  	N,  	N, 	N,  	N, 	B, 	N, 	N,  	N, 	N },		//	10
		{ N,	N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N,  	N,  	N }		//	11
	},
	//	INTERMEDIATE LEFT - color		disclosure_IntermediateLeft
	{
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N },		//	0
		{ N, 	N,		B, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N },		//	1
		{ N, 	N,		B, 	B,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N },		//	2
		{ N, 	N,		B, 	P4,	B, 	N,  	N,  	N, 	N,  	N, 	N, 	N },		//	3
		{ N, 	N,		B, 	P4,	P4, 	B,  	N,  	N, 	N,  	N, 	N, 	N },		//	4
		{ N, 	N,		B, 	P4,	P4, 	P4,  	B,  	N, 	N,  	N, 	N, 	N },		//	5
		{ N, 	N,		B, 	P4,	P4, 	P4,  	P4,  	B, 	N,  	N, 	N, 	N },		//	6
		{ N, 	N,		B, 	P4,	P4, 	P4,  	P4,  	P4, 	B,  	N, 	N, 	N },		//	7
		{ N, 	N,		B, 	P4,	P4, 	P4,  	P4,  	P4, 	P4,  	B, 	N, 	N },		//	8
		{ N, 	N,		B, 	B,		B, 	B,  	B,  	B, 	B,  	B, 	B, 	N },		//	9
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N },		//	10
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N }		//	11
	},
	//	DISABLED RIGHT - color		disclosure_DisabledRight
	{
		{ N,	N, 	N, 	N, 	5,		N, 	N,  	N,  	N, 	N,  	N, 	N },		//	0
		{ N,	N, 	N, 	N, 	5,		5, 	N,  	N,  	N, 	N,  	N, 	N },		//	1
		{ N,	N, 	N, 	N, 	5,		5, 	5,  	N,  	N, 	N,  	N, 	N },		//	2
		{ N,	N, 	N, 	N, 	5,		5, 	5,  	5,  	N, 	N,  	N, 	N },		//	3
		{ N,	N, 	N, 	N, 	5,		5, 	5,  	5,  	5, 	N,  	N, 	N },		//	4
		{ N,	N, 	N, 	N, 	5,		5, 	5,  	5,  	5, 	5,  	N, 	N },		//	5
		{ N,	N, 	N, 	N, 	5,		5, 	5,  	5,  	5, 	N,  	N, 	N },		//	6
		{ N,	N, 	N, 	N, 	5,		5, 	5,  	5,  	N, 	N,  	N, 	N },		//	7
		{ N,	N, 	N, 	N, 	5,		5, 	5,  	N,  	N, 	N,  	N, 	N },		//	8
		{ N,	N, 	N, 	N, 	5,		5, 	N,  	N,  	N, 	N,  	N, 	N },		//	9
		{ N,	N, 	N, 	N, 	5,		N, 	N,  	N,  	N, 	N,  	N, 	N },		//	10
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N }		//	11
	},
	//	DISABLED DOWN - color		disclosure_DisabledDown
	{
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N },		//	0
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N },		//	1
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N },		//	2
		{ 5,	5, 	5,		5, 	5,  	5,  	5, 	5,  	5, 	5, 	5, 	N },		//	3
		{ N,	5, 	5,		5, 	5,  	5,  	5, 	5,  	5, 	5, 	N, 	N },		//	4
		{ N,	N, 	5,		5, 	5,  	5,  	5, 	5,  	5, 	N, 	N, 	N },		//	5
		{ N,	N, 	N,		5, 	5,  	5,  	5, 	5,  	N, 	N, 	N, 	N },		//	6
		{ N,	N, 	N,		N, 	5,  	5,  	5, 	N,  	N, 	N, 	N, 	N },		//	7
		{ N,	N, 	N,		N, 	N,  	5,  	N, 	N,  	N, 	N, 	N, 	N },		//	8
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N },		//	9
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N },		//	10
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N }		//	11
	},
	//	DISABLED LEFT - color			disclosure_DisabledLeft
	{
		{ N,	N, 	N,  	N,  	N, 	N,  	N, 	5, 	N, 	N,  	N, 	N },		//	0
		{ N,	N, 	N,  	N,  	N, 	N,  	5, 	5, 	N, 	N,  	N, 	N },		//	1
		{ N,	N, 	N,  	N,  	N, 	5,  	5, 	5, 	N, 	N,  	N, 	N },		//	2
		{ N,	N, 	N,  	N,  	5, 	5,  	5, 	5, 	N, 	N,  	N, 	N },		//	3
		{ N,	N, 	N,  	5,  	5, 	5,  	5, 	5, 	N, 	N,  	N, 	N },		//	4
		{ N,	N, 	5,  	5,  	5, 	5,  	5, 	5, 	N, 	N,  	N, 	N },		//	5
		{ N,	N, 	N,  	5,  	5, 	5,  	5, 	5, 	N, 	N,  	N, 	N },		//	6
		{ N,	N, 	N,  	N,  	5, 	5,  	5, 	5, 	N, 	N,  	N, 	N },		//	7
		{ N,	N, 	N,  	N,  	N, 	5,  	5, 	5, 	N, 	N,  	N, 	N },		//	8
		{ N,	N, 	N,  	N,  	N, 	N,  	5, 	5, 	N, 	N,  	N, 	N },		//	9
		{ N,	N, 	N,  	N,  	N, 	N,  	N, 	5, 	N, 	N,  	N, 	N },		//	10
		{ N,	N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N,  	N,  	N }		//	11
	},
	// ENABLED RIGHT - B&W	disclosure_EnabledRightBW
	{
		{ N,	N, 	N, 	N, 	B,		N, 	N,  	N,  	N, 	N,  	N, 	N },		//	0
		{ N,	N, 	N, 	N, 	B,		B, 	N,  	N,  	N, 	N,  	N, 	N },		//	1
		{ N,	N, 	N, 	N, 	B,		W, 	B,  	N,  	N, 	N,  	N, 	N },		//	2
		{ N,	N, 	N, 	N, 	B,		W, 	W,  	B,  	N, 	N,  	N, 	N },		//	3
		{ N,	N, 	N, 	N, 	B,		W, 	W,  	W,  	B, 	N,  	N, 	N },		//	4
		{ N,	N, 	N, 	N, 	B,		W, 	W,  	W,  	W, 	B,  	N, 	N },		//	5
		{ N,	N, 	N, 	N, 	B,		W, 	W,  	W,  	B, 	N,  	N, 	N },		//	6
		{ N,	N, 	N, 	N, 	B,		W, 	W,  	B,  	N, 	N,  	N, 	N },		//	7
		{ N,	N, 	N, 	N, 	B,		W, 	B,  	N,  	N, 	N,  	N, 	N },		//	8
		{ N,	N, 	N, 	N, 	B,		B, 	N,  	N,  	N, 	N,  	N, 	N },		//	9
		{ N,	N, 	N, 	N, 	B,		N, 	N,  	N,  	N, 	N,  	N, 	N },		//	10
		{ N,	N, 	N, 	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N }		//	11
	},
	//	PRESSED RIGHT - B&W		disclosure_PressedRightBW
	{
		{ N,	N, 	N, 	N, 	B,		N, 	N,  	N,  	N, 	N,  	N, 	N },		//	0
		{ N,	N, 	N, 	N, 	B,		B, 	N,  	N,  	N, 	N,  	N, 	N },		//	1
		{ N,	N, 	N, 	N, 	B,		B, 	B,  	N,  	N, 	N,  	N, 	N },		//	2
		{ N,	N, 	N, 	N, 	B,		B, 	B,  	B,  	N, 	N,  	N, 	N },		//	3
		{ N,	N, 	N, 	N, 	B,		B, 	B,  	B,  	B, 	N,  	N, 	N },		//	4
		{ N,	N, 	N, 	N, 	B,		B, 	B,  	B,  	B, 	B,  	N, 	N },		//	5
		{ N,	N, 	N, 	N, 	B,		B, 	B,  	B,  	B, 	N,  	N, 	N },		//	6
		{ N,	N, 	N, 	N, 	B,		B, 	B,  	B,  	N, 	N,  	N, 	N },		//	7
		{ N,	N, 	N, 	N, 	B,		B, 	B,  	N,  	N, 	N,  	N, 	N },		//	8
		{ N,	N, 	N, 	N, 	B,		B, 	N,  	N,  	N, 	N,  	N, 	N },		//	9
		{ N,	N, 	N, 	N, 	B,		N, 	N,  	N,  	N, 	N,  	N, 	N },		//	10
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N }		//	11
	},
	//	INTERMEDIATE RIGHT - B&W		disclosure_IntermediateRightBW
	{
		{ N,	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N, 	N },		//	0
		{ N,	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	B, 	N, 	N },		//	1
		{ N,	N,		N, 	N,  	N,  	N, 	N,  	N, 	B, 	B, 	N, 	N },		//	2
		{ N,	N,		N, 	N,  	N,  	N, 	N,  	B, 	B, 	B, 	N, 	N },		//	3
		{ N,	N,		N, 	N,  	N,  	N, 	B,  	B, 	B, 	B, 	N, 	N },		//	4
		{ N,	N,		N, 	N,  	N,  	B, 	B,  	B, 	B, 	B, 	N, 	N },		//	5
		{ N,	N,		N, 	N,  	B,  	B, 	B,  	B, 	B, 	B, 	N, 	N },		//	6
		{ N,	N,		N, 	B,  	B,  	B, 	B,  	B, 	B, 	B, 	N, 	N },		//	7
		{ N,	N,		B, 	B,  	B,  	B, 	B,  	B, 	B, 	B, 	N, 	N },		//	8
		{ N, 	B,		B, 	B,  	B,  	B, 	B,  	B, 	B, 	B, 	N, 	N },		//	9
		{ N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N, 	N },		//	10
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N }		//	11
	},
	//	PRESSED DOWN - B&W		disclosure_PressedDownBW
	{
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N },		//	0
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N },		//	1
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N },		//	2
		{ B,	B, 	B,		B, 	B,  	B,  	B, 	B,  	B, 	B, 	B, 	N },		//	3
		{ N,	B, 	B,		B, 	B,  	B,  	B, 	B,  	B, 	B, 	N, 	N },		//	4
		{ N,	N, 	B,		B, 	B,  	B,  	B, 	B,  	B, 	N, 	N, 	N },		//	5
		{ N,	N, 	N,		B, 	B,  	B,  	B, 	B,  	N, 	N, 	N, 	N },		//	6
		{ N,	N, 	N,		N, 	B,  	B,  	B, 	N,  	N, 	N, 	N, 	N },		//	7
		{ N,	N, 	N,		N, 	N,  	B,  	N, 	N,  	N, 	N, 	N, 	N },		//	8
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N },		//	9
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N },		//	10
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N }		//	11
	},
	//	ENABLED DOWN - B&W			disclosure_EnabledDownBW
	{
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N },		//	0
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N },		//	1
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N },		//	2
		{ B,	B, 	B,		B, 	B,  	B,  	B, 	B,  	B, 	B, 	B, 	N },		//	3
		{ N,	B, 	W,		W, 	W,  	W,  	W, 	W,  	W, 	B, 	N, 	N },		//	4
		{ N,	N, 	B,		W, 	W,  	W,  	W, 	W,  	B, 	N, 	N, 	N },		//	5
		{ N,	N, 	N,		B, 	W,  	W,  	W, 	B,  	N, 	N, 	N, 	N },		//	6
		{ N,	N, 	N,		N, 	B,  	W,  	B, 	N,  	N, 	N, 	N, 	N },		//	7
		{ N,	N, 	N,		N, 	N,  	B,  	N, 	N,  	N, 	N, 	N, 	N },		//	8
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N },		//	9
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N },		//	10
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N }		//	11
	},
	//	ENABLED LEFT - B&W			disclosure_EnabledLeftBW
	{
		{ N,	N, 	N,  	N,  	N, 	N,  	N, 	B, 	N, 	N,  	N,  	N },		//	0
		{ N,	N, 	N,  	N,  	N, 	N,  	B, 	B, 	N, 	N,  	N, 	N },		//	1
		{ N,	N, 	N,  	N,  	N, 	B,  	W, 	B, 	N, 	N,  	N, 	N },		//	2
		{ N,	N, 	N,  	N,  	B, 	W,  	W, 	B, 	N, 	N,  	N, 	N },		//	3
		{ N,	N, 	N,  	B,  	W, 	W,  	W, 	B, 	N, 	N,  	N, 	N },		//	4
		{ N,	N, 	B,  	W,  	W, 	W,  	W, 	B, 	N, 	N,  	N, 	N },		//	5
		{ N,	N, 	N,  	B,  	W, 	W,  	W, 	B, 	N, 	N,  	N, 	N },		//	6
		{ N,	N, 	N,  	N,  	B, 	W,  	W, 	B, 	N, 	N,  	N, 	N },		//	7
		{ N,	N, 	N,  	N,  	N, 	B,  	W, 	B, 	N, 	N,  	N, 	N },		//	8
		{ N,	N, 	N,  	N,  	N, 	N,  	B, 	B, 	N, 	N,  	N, 	N },		//	9
		{ N,	N, 	N,  	N,  	N, 	N,  	N, 	B, 	N, 	N,  	N, 	N },		//	10
		{ N,	N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N,  	N, 	N }		//	11
	},
	//	PRESSES LEFT - B&W			disclosure_PressedLeftBW
	{
		{ N,	N, 	N,  	N,  	N, 	N,  	N, 	B, 	N, 	N,  	N, 	N },		//	0
		{ N,	N, 	N,  	N,  	N, 	N,  	B, 	B, 	N, 	N,  	N, 	N },		//	1
		{ N,	N, 	N,  	N,  	N, 	B,  	B, 	B, 	N, 	N,  	N, 	N },		//	2
		{ N,	N, 	N,  	N,  	B, 	B,  	B, 	B, 	N, 	N,  	N, 	N },		//	3
		{ N,	N, 	N,  	B,  	B, 	B,  	B, 	B, 	N, 	N,  	N, 	N },		//	4
		{ N,	N, 	B,  	B,  	B, 	B,  	B, 	B, 	N, 	N,  	N, 	N },		//	5
		{ N,	N, 	N,  	B,  	B, 	B,  	B, 	B, 	N, 	N,  	N, 	N },		//	6
		{ N,	N, 	N,  	N,  	B, 	B,  	B, 	B, 	N, 	N,  	N, 	N },		//	7
		{ N,	N, 	N,  	N,  	N, 	B,  	B, 	B, 	N, 	N,  	N, 	N },		//	8
		{ N,	N, 	N,  	N,  	N, 	N,  	B, 	B, 	N, 	N,  	N, 	N },		//	9
		{ N,	N, 	N,  	N,  	N, 	N,  	N, 	B, 	N, 	N,  	N, 	N },		//	10
		{ N,	N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N,  	N,  	N }		//	11
	},
	//	INTERMEDIATE LEFT - B&W		disclosure_IntermediateLeftBW
	{
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N },		//	0
		{ N, 	N,		B, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N },		//	1
		{ N, 	N,		B, 	B,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N },		//	2
		{ N, 	N,		B, 	B,		B, 	N,  	N,  	N, 	N,  	N, 	N, 	N },		//	3
		{ N, 	N,		B, 	B,		B, 	B,  	N,  	N, 	N,  	N, 	N, 	N },		//	4
		{ N, 	N,		B, 	B,		B, 	B,  	B,  	N, 	N,  	N, 	N, 	N },		//	5
		{ N, 	N,		B, 	B,		B, 	B,  	B,  	B, 	N,  	N, 	N, 	N },		//	6
		{ N, 	N,		B, 	B,		B, 	B,  	B,  	B, 	B,  	N, 	N, 	N },		//	7
		{ N, 	N,		B, 	B,		B, 	B,  	B,  	B, 	B,  	B, 	N, 	N },		//	8
		{ N, 	N,		B, 	B,		B, 	B,  	B,  	B, 	B,  	B, 	B, 	N },		//	9
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N },		//	10
		{ N,	N, 	N,		N, 	N,  	N,  	N, 	N,  	N, 	N, 	N, 	N }		//	11
	}
};

#pragma mark === CLASS: LGADisclosureTriangle
#pragma mark -
#pragma mark === INITIALIZATION

//=====================================================================================
// CLASS:	LGADisclosureTriangle
//=====================================================================================

//=====================================================================================
// ее INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LGADisclosureTriangle::LGADisclosureTriangle					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Default constructor

LGADisclosureTriangle::LGADisclosureTriangle ()
{	

	mHilited = false;
	mLeftDisclosure = false;
	
}	//	LGADisclosureTriangle::LGADisclosureTriangle


//-------------------------------------------------------------------------------------
// LGADisclosureTriangle::LGADisclosureTriangle					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Copy constructor

LGADisclosureTriangle::LGADisclosureTriangle (	const LGADisclosureTriangle &inOriginal )
									:	LControl ( inOriginal )
{	

	mHilited = inOriginal.mHilited;
	mLeftDisclosure = inOriginal.mLeftDisclosure;
	
}	//	LGADisclosureTriangle::LGADisclosureTriangle


//-------------------------------------------------------------------------------------
// LGADisclosureTriangle::LGADisclosureTriangle					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Stream constructor

LGADisclosureTriangle::LGADisclosureTriangle (	LStream	*inStream ) 
											: LControl ( inStream )
{

	// е Read in the orientation flag
	*inStream >> mLeftDisclosure;
	
	// е Setup the hilite flag
	mHilited = false;

	// е Make sure we setup the max value for the control in case the user
	// did not do it in the PPob
	if ( GetMaxValue () < 1 )
		SetMaxValue ( 1 );
		
}	//	LGADisclosureTriangle::LGADisclosureTriangle


//-------------------------------------------------------------------------------------
// LGADisclosureTriangle::LGADisclosureTriangle					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Parameterized constructor

LGADisclosureTriangle::LGADisclosureTriangle (	const SPaneInfo		&inPaneInfo,
																const SControlInfo	&inControlInfo,
																Boolean					inLeftDisclosure ) 
									: LControl ( 	inPaneInfo,
														inControlInfo.valueMessage,
														inControlInfo.value,
														inControlInfo.minValue,
														inControlInfo.maxValue )
{

	// е Setup some initial values
	mHilited = mValue != 0;
	mLeftDisclosure = inLeftDisclosure;
	
	// е Make sure we setup the max value for the control in case the user
	// did not do it in the PPob
	if ( GetMaxValue () < 1 )
		SetMaxValue ( 1 );
		
}	//	LGAIconButton::LGAIconButton


//-------------------------------------------------------------------------------------
// LGADisclosureTriangle::~LGADisclosureTriangle				---<<е DESTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LGADisclosureTriangle::~LGADisclosureTriangle ()
{
}	//	LGADisclosureTriangle::~LGADisclosureTriangle


#pragma mark -
#pragma mark === ACCESSORS

//=====================================================================================
// ее ACCESSORS
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
// LGADisclosureTriangle::SetHiliteState
//-------------------------------------------------------------------------------------

void			
LGADisclosureTriangle::SetHiliteState	(	Boolean	inHiliteState )
{
	
	if ( mHilited != inHiliteState )
	{
		mHilited = inHiliteState;
	}
			
}	//	LGADisclosureTriangle::SetHiliteState


//-------------------------------------------------------------------------------------
// LGADisclosureTriangle::SetValue
//-------------------------------------------------------------------------------------

void			
LGADisclosureTriangle::SetValue	( SInt32	inValue )
{

	if ( inValue != GetValue ())
	{
		Refresh ();
		
		// е Call our parent to handle the setting of the value
		LControl::SetValue ( inValue );
		
		// е And make sure we get the hilite state changed
		if ( IsHilited ())
		{
			SetHiliteState ( false );
		}
		
		// е We always need get things redrawn
		DrawIntermediateState ();
		Draw ( nil );
	}

}	//	LGADisclosureTriangle::SetValue


//-------------------------------------------------------------------------------------
// LGADisclosureTriangle::CalcLocalControlRect
//-------------------------------------------------------------------------------------

void
LGADisclosureTriangle::CalcLocalControlRect ( Rect &outRect )
{
	// е Figure out the frame rect for the checkbox button
	CalcLocalFrameRect ( outRect );

	// е Adjust the rect such that it is always centered vertically 
	// and horizontally within the frame rect
	outRect.left += ((UGraphicsUtilities::RectWidth ( outRect ) - disclosure_Width) / 2);
	outRect.right = outRect.left + disclosure_Width;
	outRect.top += ((UGraphicsUtilities::RectHeight ( outRect ) - disclosure_Width) / 2);
	outRect.bottom = outRect.top + disclosure_Width;
	
}	//	LGADisclosureTriangle::CalcLocalControlRect


#pragma mark -
#pragma mark === ENABLING & DISABLING

//=====================================================================================
// ее ENABLING & DISABLING
//-------------------------------------------------------------------------------------
// LGADisclosureTriangle::EnableSelf
//-------------------------------------------------------------------------------------
//

void
LGADisclosureTriangle::EnableSelf ()
{

	// е Get things redrawn so that we can see the state change
	Draw ( nil );
	
}	//	LGADisclosureTriangle::EnableSelf


//-------------------------------------------------------------------------------------
// LGADisclosureTriangle::DisableSelf
//-------------------------------------------------------------------------------------
//

void
LGADisclosureTriangle::DisableSelf ()
{

	// е Get things redrawn so that we can see the state change
	Draw ( nil );

}	//	LGADisclosureTriangle::DisableSelf


#pragma mark -
#pragma mark === ACTIVATION

//=====================================================================================
// ее ACTIVATION
//-------------------------------------------------------------------------------------
// LGADisclosureTriangle::ActivateSelf
//-------------------------------------------------------------------------------------

void
LGADisclosureTriangle::ActivateSelf ()
{

	// е Get everything redrawn so the changes take effect
	if ( mActive == triState_On )
		Refresh ();
					
}	//	LGADisclosureTriangle::ActivateSelf


//-------------------------------------------------------------------------------------
// LGADisclosureTriangle::DeactivateSelf
//-------------------------------------------------------------------------------------

void
LGADisclosureTriangle::DeactivateSelf ()
{

	// е Get everything redrawn so the changes take effect
	if ( mActive == triState_Off || mActive == triState_Latent )
		Refresh ();
				
}	//	LGADisclosureTriangle::DeactivateSelf


#pragma mark -
#pragma mark === DRAWING

//=====================================================================================
// ее DRAWING
//-------------------------------------------------------------------------------------
// GModeControl::DrawSelf
//-------------------------------------------------------------------------------------

void
LGADisclosureTriangle::DrawSelf	()
{

	StColorPenState::Normalize ();

	// е Setup a device loop so that we can handle drawing at the correct bit depth
	Rect	localRect;
	CalcLocalControlRect ( localRect );
	StDeviceLoop	theLoop ( localRect );
	SInt16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
			
		// е Get the disclosure triangle drawn in its various states
		if ( IsHilited ())
		{
			StColorPenState::Normalize ();

			if ( IsSelected ())
				PlotPixelMapChoice ( depth < 4 ? disclosure_PressedDownBW :
																disclosure_PressedDown );
			else
				PlotPixelMapChoice ( IsLeftDisclosure() ? 
					(depth < 4 ? disclosure_PressedLeftBW : disclosure_PressedLeft) :
						(depth < 4 ? disclosure_PressedRightBW : disclosure_PressedRight));
		}
		else if ( !IsEnabled () || !IsActive ())
		{

			if ( IsSelected ())
			{
				if ( depth < 4	)
				{
					Pattern thePattern;
					UQDGlobals::GetGrayPat(&thePattern);
					::PenPat ( &thePattern );
					
					PlotPixelMapChoice ( disclosure_PressedDownBW );
				}
				else
				{
					StColorPenState::Normalize ();
					
					PlotPixelMapChoice ( disclosure_DisabledDown );
				}
			}
			else
			{
				if ( depth < 4	)
				{
					Pattern thePattern;
					UQDGlobals::GetGrayPat(&thePattern);
					::PenPat ( &thePattern );
					
					PlotPixelMapChoice ( IsLeftDisclosure() ? disclosure_PressedLeftBW : 
														disclosure_PressedRightBW );
				}
				else
				{
					StColorPenState::Normalize ();
					
					PlotPixelMapChoice ( IsLeftDisclosure() ? disclosure_DisabledLeft :
													disclosure_DisabledRight );
				}
			}
		}
		else
		{
			if ( IsSelected ())
				PlotPixelMapChoice ( depth < 4 ? disclosure_EnabledDownBW : 
																	disclosure_EnabledDown );
			else
				PlotPixelMapChoice ( IsLeftDisclosure() ? 
					(depth < 4 ? disclosure_EnabledLeftBW : disclosure_EnabledLeft) :
						(depth < 4 ? disclosure_EnabledRightBW : disclosure_EnabledRight));
		}
	}
	
}	//	LGADisclosureTriangle::DrawSelf


//-------------------------------------------------------------------------------------
// LGADisclosureTriangle::DrawIntermediateState
//-------------------------------------------------------------------------------------

void
LGADisclosureTriangle::DrawIntermediateState	()
{
	if ( IsVisible () && FocusDraw ())
	{
		StColorPenState	theColorPenState;
		theColorPenState.Normalize ();
		
		// е Setup a device loop so that we can handle drawing at the correct bit depth
		Rect	localRect;
		CalcLocalControlRect ( localRect );
		StDeviceLoop	theLoop ( localRect );
		SInt16				depth;
		while ( theLoop.NextDepth ( depth )) 
		{
			// е Now we wipe the background before we draw the intermediate state
			WipeBackground ();
			
			// е Now plot the intermediate state for either the left or the
			// right triangle
			if ( depth >= 4 )
			{
				PlotPixelMapChoice ( IsLeftDisclosure () ?  disclosure_IntermediateLeft :
																	disclosure_IntermediateRight );
			}
			else
			{
				PlotPixelMapChoice ( IsLeftDisclosure () ?  disclosure_IntermediateLeftBW :
																	disclosure_IntermediateRightBW );
			}
																					 
			// е Now we wipe the background between states
			WipeBackground ();
			
			// е Now plot the pressed state for the new state which will
			// provide a transition into the final state
			if ( depth >= 4 )
			{
				PlotPixelMapChoice ( IsLeftDisclosure () ? 
												( IsSelected () ? disclosure_PressedDown :
																		disclosure_PressedLeft) :
												( IsSelected () ? disclosure_PressedDown :
																		disclosure_PressedRight ));
			}
			else
			{
				PlotPixelMapChoice ( IsLeftDisclosure () ? 
												( IsSelected () ? disclosure_PressedDownBW :
																		disclosure_PressedLeftBW) :
												( IsSelected () ? disclosure_PressedDownBW :
																		disclosure_PressedRightBW ));
			}
																		 
			// е Now we wipe the background again before the final state is drawn
			WipeBackground ();
		}
	}
	
}	//	LGADisclosureTriangle::DrawIntermediateState


//-------------------------------------------------------------------------------------
// LGADisclosureTriangle::WipeBackground
//-------------------------------------------------------------------------------------

void
LGADisclosureTriangle::WipeBackground	()
{

	if ( IsVisible () && FocusDraw ())
	{
		StColorPenState	theColorPenState;
		theColorPenState.Normalize ();
				
		// е In order to make the transitions between the various states
		// smoother we are going to attempt to figure out what the background
		// color is behind the triangle, by using the current clipping region
		// we will subzequently use this color to wipe the background
		Rect	localFrame;
		CalcLocalFrameRect ( localFrame );
		RgnHandle	currClip = ::NewRgn ();
		::GetClip ( currClip );
		Rect clipRect;
#if PP_Target_Carbon
		::GetRegionBounds(currClip, &clipRect);
#else
		clipRect = (**currClip).rgnBBox;
#endif
		::DisposeRgn ( currClip );
		::SectRect ( &localFrame, &clipRect, &localFrame );
		
		// е Now find the background color
		RGBColor	backColor; 
		::GetCPixel ( localFrame.right - 1, localFrame.top, &backColor );

		// е Now we wipe the background with this color by performing an
		// erase rect
		::RGBBackColor ( &backColor );
		::EraseRect ( &localFrame );
	}

}	//	LGADisclosureTriangle::WipeBackground


//-------------------------------------------------------------------------------------
// LGADisclosureTriangle::PlotPixelMapChoice
//-------------------------------------------------------------------------------------

void
LGADisclosureTriangle::PlotPixelMapChoice	( SInt16	inChoice )
{
	
	// е Get the frame for the control which we will use 
	// to plot the triangle
	Rect	localFrame;
	CalcLocalControlRect ( localFrame );

	// е Plot the triangle by iterating over the array of color
	// choices that specify the control
	SInt16 colorindex;
	for ( register int i = 0; i < disclosure_Rows; i++ )
		for ( register int j = 0; j < disclosure_Columns; j++ )
			{
				colorindex = sDisclosurePixMap[inChoice][i][j];
				if ( colorindex != N )
				{
					RGBColor theColor = UGAColorRamp::GetColor(colorindex);
					UGraphicsUtilities::PaintColorPixel ( 	localFrame.left + j, 
										localFrame.top + i, 
										&theColor);
				}
			}

}	//	LGADisclosureTriangle::PlotPixelMapChoice


#pragma mark -
#pragma mark === MOUSE TRACKING

//=====================================================================================
// ее MOUSE TRACKING
//-------------------------------------------------------------------------------------
// LGADisclosureTriangle::HotSpotAction
//-------------------------------------------------------------------------------------

void
LGADisclosureTriangle::HotSpotAction	(	SInt16		/* inHotSpot */,
													Boolean	inCurrInside,
													Boolean	inPrevInside )
{

	// е If the mouse moved in or out of the hot spot handle the
	// hiliting of the control
	if ( inCurrInside != inPrevInside )
	{
		
		// е Because a disclosure triangle is basically just 
		// a switch we simply toggle the state
		SetHiliteState ( inCurrInside );

		// е Get the control redrawn so that any hiliting changes
		// can take effect, we also will do a wipe before the state
		// change is drawn
		Draw ( nil );
	}
		
}	//	LGADisclosureTriangle::HotSpotAction


//-------------------------------------------------------------------------------------
// LGADisclosureTriangle::HotSpotResult
//-------------------------------------------------------------------------------------

void
LGADisclosureTriangle::HotSpotResult	(	SInt16 /*inHotSpot*/ )
{
	
	// е Becausee a disclosure triangle is basicallya switch we toggle
	// the value field of the control to indicate that
	// it is on or off
	SetValue ( 1 - GetValue ());
	
}	//	LGADisclosureTriangle::HotSpotResult


