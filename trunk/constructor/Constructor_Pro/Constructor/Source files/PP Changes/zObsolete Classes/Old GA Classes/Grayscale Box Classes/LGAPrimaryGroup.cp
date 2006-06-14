// Copyright �2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright� 1996 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< � >>-----------------------------------------

FILE:					LGAPrimaryGroup.cp

CLASSES:				LGAPrimaryGroup

DESCRIPTION:		Implementation file for LGAPrimaryGroup

AUTHOR:				Robin Mair

CREATION DATE :	96.05.26

CHANGE HISTORY :

		97.04.20		rtm	Removed the CreateFromStream method.
		97.01.24		rtm	Added a copy constructor.
		97.01.22		rtm	Added a parameterized constructor so that instances can be 
								created procedurally.
		96.10.21		rtm	Added a call to LGAPrimaryBox::FinishCreateSelf in the
								FinishCreateSelf method.
		96.09.04		rtm	PowerPlant conformance changes.
		96.08.11		rtm	Changed ListenToMessage so that it no longer makes a call to
								Draw after the SetValue call, SetValue now handles the Draw.
		96.05.26		rtm	Initial creation of file

---------------------------------------<< � >>-----------------------------------------
*/

#ifdef PowerPlant_PCH
	#include PowerPlant_PCH
#endif

// � GRAYSCALE HEADERS
#include <LGAPrimaryGroup.h>
#include <LGAPrimaryBox.h>
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
// CLASS:	LGAPrimaryGroup
//=====================================================================================

#pragma mark === CLASS: LGAPrimaryGroup
#pragma mark -
#pragma mark === INITIALIZATION

//=====================================================================================
// �� INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LGAPrimaryGroup::LGAPrimaryGroup					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------
//
//	Default constructor

LGAPrimaryGroup::LGAPrimaryGroup ()
{

	mCurrentControl = nil;
	
}	//	LGAPrimaryGroup::LGAPrimaryGroup


//-------------------------------------------------------------------------------------
// LGAPrimaryGroup::LGAPrimaryGroup					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------
//
//	Copy constructor

LGAPrimaryGroup::LGAPrimaryGroup (	const LGAPrimaryGroup &inOriginal )
								:	LGAPrimaryBox ( inOriginal )
{

	mCurrentControl = inOriginal.mCurrentControl;
	
}	//	LGAPrimaryGroup::LGAPrimaryGroup


//-------------------------------------------------------------------------------------
// LGAPrimaryGroup::LGAPrimaryGroup					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------
//
//	Stream constructor

LGAPrimaryGroup::LGAPrimaryGroup (	LStream*	inStream ) : LGAPrimaryBox ( inStream )
{

	mCurrentControl = nil;
	
}	//	LGAPrimaryGroup::LGAPrimaryGroup


//-------------------------------------------------------------------------------------
// LGAPrimaryGroup::LGAPrimaryGroup					---<<� CONSTRUCTOR �>>---
//-------------------------------------------------------------------------------------
//
//	Parameterized constructor

LGAPrimaryGroup::LGAPrimaryGroup	(	const SPaneInfo	&inPaneInfo,
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

	// � Setup the classes fields
	mCurrentControl = nil;

}	//	LGAPrimaryGroup::LGAPrimaryGroup


//-------------------------------------------------------------------------------------
// LGAPrimaryGroup::~LGAPrimaryGroup				---<<� DESTRUCTOR �>>---
//-------------------------------------------------------------------------------------

LGAPrimaryGroup::~LGAPrimaryGroup ()
{
}	//	LGAPrimaryGroup::~LGAPrimaryGroup


//-------------------------------------------------------------------------------------
// LGAPrimaryGroup::FinishCreateSelf
//-------------------------------------------------------------------------------------

void
LGAPrimaryGroup::FinishCreateSelf ()
{

	// � Call the superclass so that we correctly get the box rects calculated
	LGAPrimaryBox::FinishCreateSelf ();

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
	
}	//	LGAPrimaryGroup::FinishCreateSelf


#pragma mark -
#pragma mark === ACCESSORS

//=====================================================================================
// �� ACCESSORS
//-------------------------------------------------------------------------------------
// LGAPrimaryGroup::GetCurrentControlID
//-------------------------------------------------------------------------------------

PaneIDT
LGAPrimaryGroup::GetCurrentControlID ()
{
	PaneIDT	currentID = 0;
	if ( mCurrentControl != nil ) 
	{
		currentID = mCurrentControl->GetPaneID ();
	}
	
	return currentID;
	 
}	//	LGAPrimaryGroup::GetCurrentControlID


#pragma mark -
#pragma mark === LISTENING

//=====================================================================================
// �� LISTENING
//-------------------------------------------------------------------------------------
// LGAPrimaryGroup::ListenToMessage
//-------------------------------------------------------------------------------------

void		
LGAPrimaryGroup::ListenToMessage (	MessageT inMessage, 
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
			
}	//	LGAPrimaryGroup::ListenToMessage


#pragma mark -
#pragma mark === ADDING CONTROLS

//=====================================================================================
// �� ADDING CONTROLS
//-------------------------------------------------------------------------------------
// LGAPrimaryGroup::AddControl
//-------------------------------------------------------------------------------------

void
LGAPrimaryGroup::AddControl ( LControl*	inControl )
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
	
}	//	LGAPrimaryGroup::AddControl

