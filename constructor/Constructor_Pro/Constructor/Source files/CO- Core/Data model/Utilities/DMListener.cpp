// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMListener.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 05/10/96
//	   $Date: 2006/01/18 01:32:41 $
//	$History: DMListener.cpp $
//	
//	*****************  Version 8  *****************
//	User: scouten      Date: 2/28/97    Time: 2:06p
//	Updated in $/ConstructorWin/Core/Source
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 02/28/97   Time: 11:18
//	Updated in $/Constructor/Source files/CO- Core/Data model/Utilities
//	Added support for PreValueChange.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:14
//	Updated in $/Constructor/Source files/CO- Core/Data model/Utilities
//	Improved commenting.
//	
//	*****************  Version 5   *****************
//	User: Andrew       Date: 02/04/97   Time: 21:01
//	Checked in '$/ConstructorWin/Sources'
//	Comment: Initial merge with MSVC build
//	
//	*****************  Version 4   *****************
//	User: scouten      Date: 01/24/97   Time: 17:23
//	Checked in '$/Constructor/Source files/CO- Core/Data model/Utilities'
//	Comment: Fixed CR/LF problem
//	
//	*****************  Version 3   *****************
//	User: scouten      Date: 10/17/96   Time: 17:50
//	Checked in '$/Constructor/Source files/Data model/Utilities'
//	Comment: Replaced DMElement::CanMakeChanges with IsEnabled.
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/17/96   Time: 14:40
//	Checked in '$/Constructor/Source files/Data model/Utilities'
//	Comment: Added enabled flag to attributes.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:43
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "DMListener.h"


// ===========================================================================

#pragma mark *** DMListener ***

// ---------------------------------------------------------------------------
//		* DMListener()
// ---------------------------------------------------------------------------
//	Default constructor

DMListener::DMListener()
{
}


// ---------------------------------------------------------------------------
//		* ~DMListener
// ---------------------------------------------------------------------------
//	Destructor

DMListener::~DMListener()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** update notice parsing

// ---------------------------------------------------------------------------
//		* ListenToMessage
// ---------------------------------------------------------------------------
//	Accepts the common messages from data model classes and distributes
//	them to the methods listed below as "override hooks."

void
DMListener::ListenToMessage(
	MessageT	inMessage,
	void*		ioParam)
{
	DMElement* element = nil;
	
	switch (inMessage) {

		// Going away message: probably should remove this view	

		case Element_GoingAway:
			element = (DMElement*) ioParam;
			ValidateObject_(element);
			GoingAway(element);
			break;
		
		// Collect changes message
		
		case Element_CollectChanges:
			element = (DMElement*) ioParam;
			ValidateObject_(element);
			CollectChanges(element);
			break;
		
		// Collect tentative changes message:
		
		case Element_CollectTentativeChanges:
			DM_CollectTentativeChanges* ctcMsg;
			ctcMsg = (DM_CollectTentativeChanges*) ioParam;
			CollectTentativeChanges(ctcMsg);
			break;
			
		// Find UI object message:

		case Element_FindUIObject:
			DM_FindUIObject* fuiMsg;
			fuiMsg = (DM_FindUIObject*) ioParam;
			FindUIObject(fuiMsg);
			break;

		// Modified flag changed message:
		
		case Element_ModifiedFlagChanged:
			element = (DMElement*) ioParam;
			ValidateObject_(element);
			ModifiedFlagChanged(element);
			break;
		
		// Attribute enabled flag changed:
		
		case Element_EnabledChange:
			element = (DMElement*) ioParam;
			ValidateObject_(element);
			EnabledChanged(element);
			break;

		// Container list changed message:

		case Container_ListChange:
			DM_ListChange* listMsg;
			listMsg = (DM_ListChange*) ioParam;
			ContainerListChanged(listMsg);
			break;

		// Selection changed message:
		
		case Selection_Changed:
			DM_SelectionChanged* selMsg;
			selMsg = (DM_SelectionChanged*) ioParam;
			SelectionChanged(selMsg);
			break;

		// Attribute value changed:
		
		case Attribute_ValueChange: {
			DM_ValueChange* valueMsg = (DM_ValueChange*) ioParam;
			ValueChanged(valueMsg);
			break;
		}

		// Pre Attribute value changed:
		
		case Attribute_PreValueChange: {
			DM_PreValueChange* preValueMsg = (DM_PreValueChange*) ioParam;
			PreValueChanged(preValueMsg);
			break;
		}
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** override hooks for update notices

// ---------------------------------------------------------------------------
//		* GoingAway
// ---------------------------------------------------------------------------

void
DMListener::GoingAway(
	DMElement* /* inElement */)
{
	// override hook
}


// ---------------------------------------------------------------------------
//		* CollectChanges
// ---------------------------------------------------------------------------

void
DMListener::CollectChanges(
	DMElement* /* inElement */)
{
	// override hook
}


// ---------------------------------------------------------------------------
//		* CollectTentativeChanges
// ---------------------------------------------------------------------------

void
DMListener::CollectTentativeChanges(
	DM_CollectTentativeChanges* /* inMessage */)
{
	// override hook
}


// ---------------------------------------------------------------------------
//		* FindUIObject
// ---------------------------------------------------------------------------

void
DMListener::FindUIObject(
	DM_FindUIObject* /* inFindUIMsg */)
{
	// override hook
}


// ---------------------------------------------------------------------------
//		* ModifiedFlagChanged
// ---------------------------------------------------------------------------

void
DMListener::ModifiedFlagChanged(
	DMElement* /* inElement */)
{
	// override hook
}


// ---------------------------------------------------------------------------
//		* EnabledChanged
// ---------------------------------------------------------------------------

void
DMListener::EnabledChanged(
	DMElement* /* inElement */)
{
	// override hook
}


// ---------------------------------------------------------------------------
//		* ContainerListChanged
// ---------------------------------------------------------------------------

void
DMListener::ContainerListChanged(
	DM_ListChange* /* inMessage */)
{
	// override hook
}


// ---------------------------------------------------------------------------
//		* SelectionChanged
// ---------------------------------------------------------------------------

void
DMListener::SelectionChanged(
	DM_SelectionChanged* /* inMessage */)
{
	// override hook
}


// ---------------------------------------------------------------------------
//		* PreValueChanged
// ---------------------------------------------------------------------------

void
DMListener::PreValueChanged(
	DM_PreValueChange* /* inMessage */)
{
	// override hook
}


// ---------------------------------------------------------------------------
//		* ValueChanged
// ---------------------------------------------------------------------------

void
DMListener::ValueChanged(
	DM_ValueChange* /* inMessage */)
{
	// override hook
}
