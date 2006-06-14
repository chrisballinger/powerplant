// Copyright �2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright� 1996 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< � >>-----------------------------------------

FILE:					LGABoxGroup.cp

CLASSES:				LGABoxGroup

DESCRIPTION:		Implementation file for LGABoxGroup

AUTHOR:				Robin Mair

CREATION DATE :	96.10.21

CHANGE HISTORY :

		97.04.20		rtm	Removed the CreateFromStream method.
		97.01.24		rtm	Added a copy constructor.
		97.01.22		rtm	Added a parameterized conbstructor so that instances can be 
								created procedurally.
		96.10.21		rtm	Initial creation of file

---------------------------------------<< � >>-----------------------------------------
*/

#ifdef PowerPlant_PCH
	#include PowerPlant_PCH
#endif

// � GRAYSCALE HEADERS
#include <LGABoxGroup.h>
#include <LGABox.h>
#include <UGraphicsUtilities.h>
#include <UGAColorRamp.h>

// � POWERPLANT HEADERS
#include <LStream.h>
#include <PP_Messages.h>
#include <LArrayIterator.h>


//=====================================================================================
// CONSTANTS
//=====================================================================================

//=====================================================================================
// DEFINES
//=====================================================================================

//=====================================================================================
// CLASS:	LGABoxGroup
//=====================================================================================

#pragma mark === CLASS: LGABoxGroup
#pragma mark -
#pragma mark === INITIALIZATION

//=====================================================================================
// �� INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LGABoxGroup::LGABoxGroup					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------
//
//	Default constructor

LGABoxGroup::LGABoxGroup ()
{

	mCurrentControl = nil;
	
}	//	LGABoxGroup::LGABoxGroup


//-------------------------------------------------------------------------------------
// LGABoxGroup::LGABoxGroup					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------
//
//	Copy constructor

LGABoxGroup::LGABoxGroup 	(	const LGABoxGroup &inOriginal )
							:	LGABox ( inOriginal )
{

	mCurrentControl = inOriginal.mCurrentControl;
	
}	//	LGABoxGroup::LGABoxGroup


//-------------------------------------------------------------------------------------
// LGABoxGroup::LGABoxGroup					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------
//
//	Stream constructor

LGABoxGroup::LGABoxGroup (	LStream*	inStream ) : LGABox ( inStream )
{

	mCurrentControl = nil;
	
}	//	LGABoxGroup::LGABoxGroup


//-------------------------------------------------------------------------------------
// LGABoxGroup::LGABoxGroup					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------
//
//	Parameterized constructor

LGABoxGroup::LGABoxGroup (	const SPaneInfo	&inPaneInfo,
									const SViewInfo	&inViewInfo,
									Str255				inTitle,
									ResIDT				inTextTraitsID,
									Boolean				inHasBorder,
									Int16					inBorderStyle,
									Int16					inTitlePosition,
									Rect					&inContentOffset )
					: LGABox ( inPaneInfo, 
									inViewInfo,
									inTitle,
									inTextTraitsID,
									inHasBorder,
									inBorderStyle,
									inTitlePosition,
									inContentOffset )
{

	// � Setup the classes fields
	mCurrentControl = nil;

}	//	LGABoxGroup::LGABoxGroup


//-------------------------------------------------------------------------------------
// LGABoxGroup::~LGABoxGroup				---<<� DESTRUCTOR �>>---
//-------------------------------------------------------------------------------------

LGABoxGroup::~LGABoxGroup ()
{
}	//	LGABoxGroup::~LGABoxGroup


//-------------------------------------------------------------------------------------
// LGABoxGroup::FinishCreateSelf
//-------------------------------------------------------------------------------------

void
LGABoxGroup::FinishCreateSelf ()
{

	// � Call the superclass so that we correctly get the box rects calculated
	LGABox::FinishCreateSelf ();

	// � Now we need to iterate over all of the groups subpanes adding
	// any controls to the group as listeners
	if ( mSubPanes.GetCount() > 0 )
	{
		LArrayIterator	iterator ( mSubPanes, LArrayIterator::from_Start );
		LPane*	thePane;
		while ( iterator.Next ( &thePane ))
		{
			LControl*	theControl = dynamic_cast<LControl*>( thePane );
			if ( theControl )
			{
				AddControl ( theControl );
			}
		}
	}
	
}	//	LGABoxGroup::FinishCreateSelf


#pragma mark -
#pragma mark === ACCESSORS

//=====================================================================================
// �� ACCESSORS
//-------------------------------------------------------------------------------------
// LGABoxGroup::GetCurrentControlID
//-------------------------------------------------------------------------------------

PaneIDT
LGABoxGroup::GetCurrentControlID ()
{
	PaneIDT	currentID = 0;
	if ( mCurrentControl != nil ) 
	{
		currentID = mCurrentControl->GetPaneID ();
	}
	
	return currentID;
	 
}	//	LGABoxGroup::GetCurrentControlID


#pragma mark -
#pragma mark === LISTENING

//=====================================================================================
// �� LISTENING
//-------------------------------------------------------------------------------------
// LGABoxGroup::ListenToMessage
//-------------------------------------------------------------------------------------

void		
LGABoxGroup::ListenToMessage (	MessageT inMessage, 
											void*		ioParam	)
{
	
	if ( inMessage == msg_ControlClicked )
	{		
		LControl* theControl = (LControl*) ioParam;
		ThrowIfNil_ ( theControl );
		if ( theControl != mCurrentControl ) 
		{
			// � Turn off current On button
			if ( mCurrentControl != nil ) 
			{
				mCurrentControl->SetValue ( Button_Off );
			}
			
			// � Setup the reference to the current control
			mCurrentControl = theControl;
		}
	}
			
}	//	LGABoxGroup::ListenToMessage


#pragma mark -
#pragma mark === ADDING CONTROLS

//=====================================================================================
// �� ADDING CONTROLS
//-------------------------------------------------------------------------------------
// LGABoxGroup::AddControl
//-------------------------------------------------------------------------------------

void
LGABoxGroup::AddControl ( LControl*	inControl )
{
	if ( inControl == nil ) 
	{
		return;
	}

	inControl->AddListener ( this );
	
	if ( mCurrentControl == nil ) 
	{			
		inControl->SetValue ( Button_On );
		mCurrentControl = inControl;
		
	} 
	else if ( inControl->GetValue () == Button_On ) 
	{
		// � If any subsequent control is on then simply handle it
		// as if the control was clicked
		ListenToMessage ( msg_ControlClicked, inControl );
	}
	
}	//	LGABoxGroup::AddControl

