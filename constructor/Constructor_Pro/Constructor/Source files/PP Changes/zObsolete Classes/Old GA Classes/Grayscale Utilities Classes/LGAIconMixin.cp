// Copyright й2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyrightй 1995 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< е >>-----------------------------------------

FILE:					LGAIconMixin.cp

CLASSES:				LGAIconMixin

DESCRIPTION:		Implementation file for Object

AUTHOR:				Robin Mair

CREATION DATE :	96.01.22

CHANGE HISTORY :

		97.04.04		rtm	Added a call to dispose of the icon suite in the destructor.
								Also changed the call to DisposeIconSuite in LoadIconSuite so
								that it now passes false for the disposeData parameter.
		97.02.13		rtm	Changed the test in the LoadIconHandle method so that it now
								explicitly checks for either a 0 or resID_Undefined value for
								the icon ID and only does nothing in those two cases, this now
								effectively allows negative icon IDs.
		97.01.24		rtm	Added a copy constructor.
		96.09.05		rtm	Added a new method for loading the icon suite handle, this is
								called from all the contructors that use this mixin, but it
								is also used when the icon suite resource ID is changed so that
								the cache can be flushed and reloaded. NOTE: it only loads
								if the resource ID is greater than 0!!
		96.09.04		rtm	PowerPlant conformance changes.
		96.01.22		rtm	Initial creation of file

---------------------------------------<< е >>-----------------------------------------
*/

#ifdef PowerPlant_PCH
	#include PowerPlant_PCH
#endif

// е GRAYSCALE HEADERS
#include <LGAIconMixin.h>
#include <UGraphicsUtilities.h>

// е TOOLBOX HEADERS
#include <Icons.h>


//=====================================================================================
// CONSTANTS
//=====================================================================================

//=====================================================================================
// DEFINES
//=====================================================================================

#pragma mark === CLASS: LGAIconMixin

//=====================================================================================
// CLASS:	LGAIconMixin
//=====================================================================================

#pragma mark -
#pragma mark === INITIALIZATION

//=====================================================================================
// ее INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LGAIconMixin::LGAIconMixin					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LGAIconMixin::LGAIconMixin ()
{	

	mIconSuite = nil;
	mIconSuiteID = 0;
	mTransform = ttNone;
	mSizeSelector = sizeSelector_LargeIconSize;
	mIconPlacement = iconPosition_Center;
	mIconLocation = gEmptyPoint;
	mEdgeOffset = 4;
	mHiliteIcon = false;
	mOffsetIconOnHilite = true;
	
	// е Get the icon location calculated
	CalcIconLocation ();
	
}	//	LGAIconMixin::LGAIconMixin

//-------------------------------------------------------------------------------------
// LGAIconMixin::LGAIconMixin					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------
//
//	Copy Constructor

LGAIconMixin::LGAIconMixin (	const LGAIconMixin &inOriginal )
{	

	mIconSuite = inOriginal.mIconSuite;
	mIconSuiteID = inOriginal.mIconSuiteID;
	mTransform = inOriginal.mTransform;
	mSizeSelector = inOriginal.mSizeSelector;
	mIconPlacement = inOriginal.mIconPlacement;
	mIconLocation = inOriginal.mIconLocation;
	mEdgeOffset = inOriginal.mEdgeOffset;
	mHiliteIcon = inOriginal.mHiliteIcon;
	mOffsetIconOnHilite = inOriginal.mOffsetIconOnHilite;
	
}	//	LGAIconMixin::LGAIconMixin


//-------------------------------------------------------------------------------------
// LGAIconMixin::~LGAIconMixin				---<<е DESTRUCTOR е>>---
//-------------------------------------------------------------------------------------

LGAIconMixin::~LGAIconMixin ()
{

	// е Get rid of the cached icon suite
	if ( mIconSuite )
		::DisposeIconSuite ( mIconSuite, true );
		
}	//	LGAIconMixin::~LGAIconMixin


#pragma mark -
#pragma mark === ACCESSORS

//=====================================================================================
// ее ACCESSORS
//-------------------------------------------------------------------------------------
// LGAIconMixin::LoadIconSuiteHandle
//-------------------------------------------------------------------------------------

void
LGAIconMixin::LoadIconSuiteHandle ()
{

	// е Only make the change if it is different
	if ( mIconSuiteID == 0 || mIconSuiteID == resID_Undefined )
	{
		mIconSuite = nil;
	}
	else
	{
	
		// е Before we load the handle we need to make sure that we get rid
		// of any cached handle that we might have
		if ( mIconSuite )
		{
			::DisposeIconSuite ( mIconSuite, true );
			mIconSuite = nil;
		}
		
		// е Load and cache the icon suite
		Handle				theIconSuite;
		IconSelectorValue	selector = svAllAvailableData;
		
		// е Now handle the actual loading of the icon suite
		::GetIconSuite ( &theIconSuite, mIconSuiteID, selector );
		ThrowIfNil_ ( theIconSuite );
		if ( theIconSuite )
			mIconSuite = theIconSuite;
	}
	
}	//	LGAIconMixin::LoadIconSuiteHandle


//-------------------------------------------------------------------------------------
// LGAIconMixin::SetIconResourceID
//-------------------------------------------------------------------------------------

void
LGAIconMixin::SetIconResourceID (	ResIDT	inResID )
{

	// е Only make the change if it is different, if the ID did in fact change
	// then we also need to make sure that we get the icon suite cached
	if ( mIconSuiteID != inResID )
	{
		mIconSuiteID = inResID;

		// е Load and cache the icon suite
		LoadIconSuiteHandle ();
		
	}
	
}	//	LGAIconMixin::SetIconResourceID


//-------------------------------------------------------------------------------------
// LGAIconMixin::SetIconTransform
//-------------------------------------------------------------------------------------

void
LGAIconMixin::SetIconTransform (	SInt16		inTransform )
{

	// е Only make the change if it is different
	if ( mTransform != inTransform )
	{
		mTransform = inTransform;
	}
	
}	//	LGAIconMixin::SetIconTransform


//-------------------------------------------------------------------------------------
// LGAIconMixin::SetIconSize
//-------------------------------------------------------------------------------------

void
LGAIconMixin::SetIconSize (	ESizeSelector	inSizeSelector )
{

	// е Only make the change if it is different
	if ( mSizeSelector != inSizeSelector )
	{
		mSizeSelector = inSizeSelector;
		
		// е We also need to get the icons location recalculated
		CalcIconLocation ();
	}
	
}	//	LGAIconMixin::SetIconSize


//-------------------------------------------------------------------------------------
// LGAIconMixin::SetIconPosition
//-------------------------------------------------------------------------------------

void
LGAIconMixin::SetIconPosition (	EIconPosition	inPosition )
{

	// е Only make the change if it is different
	if ( mIconPlacement != inPosition )
	{
		mIconPlacement = inPosition;
		
		// е We also need to get the icons location recalculated
		CalcIconLocation ();
	}
	
}	//	LGAIconMixin::SetIconPosition


//-------------------------------------------------------------------------------------
// LGAIconMixin::SetEdgeOffset
//-------------------------------------------------------------------------------------

void
LGAIconMixin::SetEdgeOffset (	SInt16		inOffset )
{

	// е Only make the change if it is different
	if ( mEdgeOffset != inOffset )
	{
		mEdgeOffset = inOffset;
		
		// е We also need to get the icons location recalculated
		// as this change may alter where it is drawn
		CalcIconLocation ();
	}
	
}	//	LGAIconMixin::SetEdgeOffset


//-------------------------------------------------------------------------------------
// LGAIconMixin::SetHiliteIcon
//-------------------------------------------------------------------------------------

void
LGAIconMixin::SetHiliteIcon (	Boolean	inHiliteIcon )
{

	// е Only make the change if it is different
	if ( mHiliteIcon != inHiliteIcon )
	{
		mHiliteIcon = inHiliteIcon;
	}
	
}	//	LGAIconMixin::SetHiliteIcon


//-------------------------------------------------------------------------------------
// LGAIconMixin::SetOffsetIconOnHilite
//-------------------------------------------------------------------------------------

void
LGAIconMixin::SetOffsetIconOnHilite (	Boolean	inOffsetIcon )
{

	// е Only make the change if it is different
	if ( mOffsetIconOnHilite != inOffsetIcon )
	{
		mOffsetIconOnHilite = inOffsetIcon;
	}
	
}	//	LGAIconMixin::SetOffsetIconOnHilite


//-------------------------------------------------------------------------------------
// LGAIconMixin::SetClickInIcon
//-------------------------------------------------------------------------------------

void
LGAIconMixin::SetClickInIcon (	Boolean	inClickInIcon )
{

	// е Only make the change if it is different
	if ( mClickInIcon != inClickInIcon )
	{
		mClickInIcon = inClickInIcon;
	}
	
}	//	LGAIconMixin::SetClickInIcon


//-------------------------------------------------------------------------------------
// LGAIconMixin::CalcLocalIconRect
//-------------------------------------------------------------------------------------

void
LGAIconMixin::CalcLocalIconRect (	Rect	&/*outRect*/ ) const
{
}	//	LGAIconMixin::CalcLocalIconRect


#pragma mark -
#pragma mark === MISCELLANEOUS

//=====================================================================================
// ее MISCELLANEOUS
//-------------------------------------------------------------------------------------
// LGAIconMixin::CalcIconLocation
//-------------------------------------------------------------------------------------

void
LGAIconMixin::CalcIconLocation ()
{
}	//	LGAIconMixin::CalcIconLocation


