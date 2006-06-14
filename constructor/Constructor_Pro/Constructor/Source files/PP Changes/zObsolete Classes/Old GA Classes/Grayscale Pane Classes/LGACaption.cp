// Copyright �2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright� 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< � >>-----------------------------------------

FILE:					LGACaption.cp

CLASSES:				LGACaption

DESCRIPTION:		Implementation file for LGACaption

AUTHOR:				Robin Mair

CREATION DATE :	97.02.11

CHANGE HISTORY :

		97.02.11		rtm	Initial creation of file
								
---------------------------------------<< � >>-----------------------------------------
*/


#ifdef PowerPlant_PCH
	#include PowerPlant_PCH
#endif

#include <LGACaption.h>

// � GRAYSCALE HEADERS
#include <UGraphicsUtilities.h>
#include	<UGAColorRamp.h>

// � POWERPLANT HEADERS
#include <PP_Messages.h>
#include <UDrawingUtils.h>
#include <UDrawingState.h>
#include	<UTextTraits.h>


//=====================================================================================
// CONSTANTS
//=====================================================================================

//=====================================================================================
// DEFINES
//=====================================================================================

#pragma mark === CLASS: LGACaption

//=====================================================================================
// CLASS:	LGACaption
//=====================================================================================

#pragma mark -
#pragma mark === INITIALIZATION & DESTRUCTION

//=====================================================================================
// �� INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LGACaption::LGACaption					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------

LGACaption::LGACaption ()
{
}	//	LGACaption::LGACaption


//-------------------------------------------------------------------------------------
// LGACaption::LGACaption					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------

LGACaption::LGACaption (	const LGACaption	&inCaption ) : LCaption ( inCaption )
{
}	//	LGACaption::LGACaption


//-------------------------------------------------------------------------------------
// LGACaption::LGACaption					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------

LGACaption::LGACaption (	LStream*	inStream ) : LCaption ( inStream )
{
}	//	LGACaption::LGACaption


//-------------------------------------------------------------------------------------
// LGACaption::LGACaption					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------

LGACaption::LGACaption ( 	const SPaneInfo	&inPaneInfo,
									ConstStringPtr	inString,
									ResIDT			inTextTraitsID)
						: LCaption (	inPaneInfo, 
											inString,
											inTextTraitsID )
{
}	//	LGACaption::LGACaption


//-------------------------------------------------------------------------------------
// LGACaption::~LGACaption					---<<� DESTRUCTOR �>>---
//-------------------------------------------------------------------------------------

LGACaption::~LGACaption ()
{
}	//	LGACaption::~LGACaption


#pragma mark -
#pragma mark === ACTIVATION

//=====================================================================================
// �� ACTIVATION
//-------------------------------------------------------------------------------------
// LGACaption::ActivateSelf
//-------------------------------------------------------------------------------------

void
LGACaption::ActivateSelf ()
{

	// � Get everything redrawn so the changes take effect
	if ( mActive == triState_On )
		Refresh ();
					
}	//	LGACaption::ActivateSelf


//-------------------------------------------------------------------------------------
// LGACaption::DeactivateSelf
//-------------------------------------------------------------------------------------

void
LGACaption::DeactivateSelf ()
{

	// � Get everything redrawn so the changes take effect
	if ( mActive == triState_Off || mActive == triState_Latent )
		Refresh ();
				
}	//	LGACaption::DeactivateSelf


#pragma mark -
#pragma mark === DRAWING

//=====================================================================================
// �� DRAWING
//-------------------------------------------------------------------------------------
// LGACaption::DrawSelf
//-------------------------------------------------------------------------------------

void
LGACaption::DrawSelf ()
{

	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );
	
	// � Setup the text traits and get the justification
	SInt16	just = UTextTraits::SetPortTextTraits ( mTxtrID );
	
	// � Get the text color which was setup by the text traits setup
	RGBColor	textColor;
	::GetForeColor ( &textColor );
	
	// � Get the fore and back colors applied
	ApplyForeAndBackColors ();
	
	// � Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( localFrame );
	SInt16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		// � If we are drawing to a color screen then we are going to lighten
		// the color of the text when we are disabled
		if ( depth > 4 && !IsActive ())
			textColor = UGraphicsUtilities::Lighten ( &textColor );
	
		// � Set the foreground color
		::RGBForeColor ( &textColor );
		
		// � Now we can finally get the text drawn
		UTextDrawing::DrawWithJustification ( (Ptr)&mText[1], mText[0], localFrame, just );
	
	}
	
}	//	LGACaption::DrawSelf
