// Copyright й2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyrightй 1995 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< е >>-----------------------------------------

FILE:					LGAIconSuite.cp

CLASSES:				LGAIconSuite

DESCRIPTION:		Implementation file for LGAIconSuite

AUTHOR:				Robin Mair

CREATION DATE :	96.06.07

CHANGE HISTORY :

		97.04.20		rtm	Removed the CreateFromStream method.
		97.04.04		rtm	Removed the call to DisposeIconSuite from the destructor as
								this has been moved into the LGAIconMixin class.
		97.02.15		rtm	Added support for the icon to be rendered in a disabled state
								when it is deactivated.
		97.01.24		rtm	Added a copy constructor.
		96.01.20		rtm	Added a parameterized constructor so that the pane can be built
								procedurally.
		96.10.21		rtm	Removed override of AdaptToSuperFrameSize.
		96.10.19		rtm	Optimized the CalcIconLocation method so that we are only
								calculating the icon width once. Also changed the stream
								constructor so that the loading, adjusting of transform and
								calculation of the icons location is now handled in
								FinishCreateSelf.
		96.10.08		rtm	Tweaked CalcIconLocation and CalcLocalIconRect so that they
								correctly handle the odd width for the mini icons. Also got
								rid of DrawiconSuite by moving the contents to DrawSelf.
		96.10.01		rtm	Added an override to MoveBy so that we can recalulate the icons
								location.
		96.09.05		rtm	Changed the constructors so that they now use the function
								provided by the icon mixin for loading and icon suite handle.
		96.09.04		rtm	PowerPlant conformance changes.
		96.06.26		rtm	Added override of ResizeFrameBy so that we can call 
								CalcIconLocation() whenever the zize changes.
		96.06.17		rtm	Changed the drawing of the icons so that they are now drawn
								from a cached icon suite handle instead of calling PlotIconID
								which was very slow.  By default we are jsut oading all of the
								icon data.
		96.06.07		rtm	Initial creation of file

---------------------------------------<< е >>-----------------------------------------
*/

#ifdef PowerPlant_PCH
	#include PowerPlant_PCH
#endif

// е GRAYSCALE HEADERS
#include <LGAIconSuite.h>
#include <UGraphicsUtilities.h>

// е POWERPLANT HEADERS
#include <LStream.h>
#include <UDrawingUtils.h>

// е TOOLBOX HEADERS
#include <Icons.h>


//=====================================================================================
// CONSTANTS
//=====================================================================================

//=====================================================================================
// DEFINES
//=====================================================================================

#pragma mark === CLASS: LGAIconSuite

//=====================================================================================
// CLASS:	LGAIconSuite
//=====================================================================================

#pragma mark -
#pragma mark === INITIALIZATION

//=====================================================================================
// ее INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LGAIconSuite::LGAIconSuite					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Default constructor

LGAIconSuite::LGAIconSuite ()
{	
}	//	LGAIconSuite::LGAIconSuite


//-------------------------------------------------------------------------------------
// LGAIconSuite::LGAIconSuite					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Copy constructor

LGAIconSuite::LGAIconSuite (	const LGAIconSuite &inOriginal )
							:	LPane ( inOriginal ),
								LGAIconMixin ( inOriginal )
{	
}	//	LGAIconSuite::LGAIconSuite


//-------------------------------------------------------------------------------------
// LGAIconSuite::LGAIconSuite					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Stream constructor

LGAIconSuite::LGAIconSuite (	LStream	*inStream ) : LPane ( inStream )
{
	SInt16 sizeSelector;
	SInt16	position;
	
	// е Read in the data from the stream
	*inStream >> mIconSuiteID;
	*inStream >> sizeSelector;
	mSizeSelector = (ESizeSelector)sizeSelector;
	*inStream >> position;
	mIconPlacement = (EIconPosition)position;
	
	// е We set the edge offset to be 0
	mEdgeOffset = 0;
	
}	//	LGAIconSuite::LGAIconSuite


//-------------------------------------------------------------------------------------
// LGAIconSuite::LGAIconSuite					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Parameterized constructor

LGAIconSuite::LGAIconSuite (	const SPaneInfo	&inPaneInfo,
										ResIDT				inIconSuiteResID,
										SInt16					inSizeSelector,
										SInt16					inIconPosition ) 
								: LPane ( inPaneInfo )
{

	// е Setup the fields of the class
	mIconSuiteID = inIconSuiteResID;
	mSizeSelector = (ESizeSelector)inSizeSelector;
	mIconPlacement = (EIconPosition)inIconPosition;
	
	// е We set the edge offset to be 0
	mEdgeOffset = 0;
	
}	//	LGAIconSuite::LGAIconSuite


//-------------------------------------------------------------------------------------
// LGAIconSuite::~LGAIconSuite				---<<е DESTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LGAIconSuite::~LGAIconSuite ()
{
}	//	LGAIconSuite::~LGAIconSuite


//-------------------------------------------------------------------------------------
// LGAIconSuite::FinishCreateSelf
//-------------------------------------------------------------------------------------

void
LGAIconSuite::FinishCreateSelf ()
{

	// е Load and cache the icon suite
	LoadIconSuiteHandle ();
		
	// е If the icon is not enabled we need to make sure that
	// the transform reflects that
	mTransform = mEnabled == triState_Off ? ttDisabled : ttNone;
			
	// е Finally, we need to get the icon location calculated
	CalcIconLocation ();
	
}	//	LGAIconSuite::FinishCreateSelf

	
#pragma mark -
#pragma mark === ACCESSORS

//=====================================================================================
// ее ACCESSORS
//-------------------------------------------------------------------------------------
// LGAIconSuite::CalcLocalIconRect
//-------------------------------------------------------------------------------------

void
LGAIconSuite::CalcLocalIconRect ( Rect &outRect ) const
{
	
	// е Setup the rectangle based on the location and size of the icon
	outRect.top = mIconLocation.v;
	outRect.left = mIconLocation.h;
	outRect.bottom = mIconLocation.v + mSizeSelector;
	outRect.right = mIconLocation.h + (mSizeSelector == 12 ? 16 : mSizeSelector);
	
}	//	LGAIconSuite::CalcLocalIconRect


#pragma mark -
#pragma mark === ENABLING & DISABLING

//=====================================================================================
// ее ENABLING & DISABLING
//-------------------------------------------------------------------------------------
// LGAIconSuite::EnableSelf
//-------------------------------------------------------------------------------------
//
//	This has been overridden so that we can do the correct thing about getting the
//		transform to match the state of the control

void
LGAIconSuite::EnableSelf ()
{
	
 	if ( mEnabled == triState_On )
	{
		SInt16	oldTransform = GetIconTransform ();
		
		mTransform = ttNone;

		// е If the transform actually changed then make sure that
		// we get the button redrawn
		if ( oldTransform != mTransform )
			Draw ( nil );
	}

}	//	LGAIconSuite::EnableSelf


//-------------------------------------------------------------------------------------
// LGAIconSuite::DisableSelf
//-------------------------------------------------------------------------------------
//
//	This has been overridden so that we can do the correct thing about getting the
//		transform to match the state of the control

void
LGAIconSuite::DisableSelf ()
{

	// е Always just set the transform to be disabled
	if ( mEnabled == triState_Off || mEnabled == triState_Latent )
	{
		SInt16	oldTransform = GetIconTransform ();
		
		mTransform = ttDisabled;

		// е If the transform actually changed then make sure that
		// we get the button redrawn
		if ( oldTransform != mTransform )
			Draw ( nil );
	}

}	//	LGAIconSuite::DisableSelf


#pragma mark -
#pragma mark === ACTIVATION

//=====================================================================================
// ее ACTIVATION
//-------------------------------------------------------------------------------------
// LGAIconSuite::ActivateSelf
//-------------------------------------------------------------------------------------
//

void
LGAIconSuite::ActivateSelf ()
{
	
	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );
	
	// е Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( localFrame );
	SInt16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		if ( depth > 4 )
		{
			// е If we are activating the icon then change its transform
		 	if ( mActive == triState_On )
			{
				SInt16	oldTransform = GetIconTransform ();
				
				if ( mEnabled == triState_On )
					mTransform = ttNone;
				else
					mTransform = ttDisabled;

				// е If the transform actually changed then make sure that
				// we get the button redrawn
				if ( oldTransform != mTransform )
					Refresh ();
			}
		}
	}
	
}	//	LGAIconSuite::ActivateSelf


//-------------------------------------------------------------------------------------
// LGAIconSuite::DeactivateSelf
//-------------------------------------------------------------------------------------
//

void
LGAIconSuite::DeactivateSelf ()
{

	Rect	localFrame;
	CalcLocalFrameRect ( localFrame );
	
	// е Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( localFrame );
	SInt16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		if ( depth > 4 )
		{
			// е Always just set the transform to be disabled
			if ( mActive == triState_Off || mActive == triState_Latent )
			{
				SInt16	oldTransform = GetIconTransform ();
				
				mTransform = ttDisabled;

				// е If the transform actually changed then make sure that
				// we get the item redrawn
				if ( oldTransform != mTransform )
					Refresh ();
			}
		}
	}
	
}	//	LGAIconSuite::DeactivateSelf


#pragma mark -
#pragma mark === DRAWING

//=====================================================================================
// ее DRAWING
//-------------------------------------------------------------------------------------
// LGAIconSuite::DrawSelf
//-------------------------------------------------------------------------------------

void
LGAIconSuite::DrawSelf	()
{

	StColorPenState::Normalize ();

	// е Get the icon rectangle
	Rect iconRect;
	CalcLocalIconRect ( iconRect );
	
	// е  Now we plot the icon by calling the toolbox passing it our cached
	// handle to the icon suite
	if ( GetIconSuiteH ())
		::PlotIconSuite ( &iconRect, atNone, mTransform, GetIconSuiteH () );
		
}	//	LGAIconSuite::DrawSelf


#pragma mark -
#pragma mark === RESIZING

//=====================================================================================
// ее RESIZING
// ------------------------------------------------------------------------------------
//		LGAIconSuite::ResizeFrameBy
// ------------------------------------------------------------------------------------
//

void
LGAIconSuite::ResizeFrameBy ( 	SInt16		inWidthDelta,
										SInt16		inHeightDelta,
										Boolean		inRefresh )
{
	
	// е Pas the call on to our superclass
	LPane::ResizeFrameBy ( inWidthDelta, inHeightDelta, inRefresh );
	
	// е Any time we might undergo a size change we need to make sure that
	// we re-calculate the icon location
	CalcIconLocation ();
	
}	//	LGAIconSuite::ResizeFrameBy


//-------------------------------------------------------------------------------------
// LGAIconSuite::MoveBy
//-------------------------------------------------------------------------------------

void
LGAIconSuite::MoveBy ( 	SInt32		inHorizDelta,
									SInt32		inVertDelta,
									Boolean	inRefresh )
{
	
	// е Pas the call on to our superclass
	LPane::MoveBy ( inHorizDelta, inVertDelta, inRefresh );
	
	// е Any time we might undergo a size change we need to make sure that
	// we re-calculate the icon location
	CalcIconLocation ();
	
}	//	LGAIconSuite::MoveBy


#pragma mark -
#pragma mark === MISCELLANEOUS

//=====================================================================================
// ее MISCELLANEOUS
//-------------------------------------------------------------------------------------
// LGAIconSuite::CalcIconLocation
//-------------------------------------------------------------------------------------

void
LGAIconSuite::CalcIconLocation ()
{

	Rect	localFrame;
	SInt16	width, height;
	
	// е Get the local inset frame rect
	CalcLocalFrameRect ( localFrame );
	mIconLocation.h = localFrame.left;
	mIconLocation.v = localFrame.top;
	
	// е Setup the width and height values
	width = localFrame.right - localFrame.left;
	height = localFrame.bottom - localFrame.top;
	
	// е Calculate the icon width based on the size selector
	SInt16		iconWidth = mSizeSelector == 12 ? 16 : mSizeSelector;
	
	// е Now we calculate the location of the icon based on the icon placement enum
	switch ( mIconPlacement )
	{
		case iconPosition_None:
			mIconLocation.h += ( width / 2 ) - ( iconWidth / 2 );
			mIconLocation.v += ( height / 2 ) - ( mSizeSelector / 2 );
		break;
		
		case iconPosition_TopLeft:
			mIconLocation.h += mEdgeOffset;
			mIconLocation.v += mEdgeOffset;
		break;
		
		case iconPosition_TopCenter:
			mIconLocation.h += ( width / 2 ) - ( iconWidth / 2 );
			mIconLocation.v += mEdgeOffset;
		break;
		
		case iconPosition_TopRight:
			mIconLocation.h += width - ( iconWidth + mEdgeOffset );
			mIconLocation.v += mEdgeOffset;
		break;
		
		case iconPosition_RightCenter:
			mIconLocation.h += width - ( iconWidth + mEdgeOffset );
			mIconLocation.v += ( height / 2 ) - ( mSizeSelector / 2 );
		break;
		
		case iconPosition_BottomRight:
			mIconLocation.h += width - ( iconWidth + mEdgeOffset );
			mIconLocation.v += height - ( mSizeSelector + mEdgeOffset );
		break;
		
		case iconPosition_BottomCenter:
			mIconLocation.h += ( width / 2 ) - ( iconWidth / 2 );
			mIconLocation.v += height - ( mSizeSelector + mEdgeOffset );
		break;
		
		case iconPosition_BottomLeft:
			mIconLocation.h += mEdgeOffset;
			mIconLocation.v += height - ( mSizeSelector + mEdgeOffset );
		break;
		
		case iconPosition_LeftCenter:
			mIconLocation.h += mEdgeOffset;
			mIconLocation.v += ( height / 2 ) - ( mSizeSelector / 2 );
		break;
		
		case iconPosition_Center:
			mIconLocation.h += ( width / 2 ) - ( iconWidth / 2 );
			mIconLocation.v += ( height / 2 ) - ( mSizeSelector / 2 );
		break;
		
	};
	
}	//	LGAIconSuite::CalcIconLocation


