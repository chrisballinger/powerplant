// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMAttributeSubAction.cpp	© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 04/27/96
//	   $Date: 2006/01/18 01:32:37 $
//	$History: DMAttributeSubAction.cpp $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:49
//	Updated in $/Constructor/Source files/CO- Core/Data model/Undo-redo
//	Improved commenting.
//	
//	*****************  Version 3   *****************
//	User: Andrew       Date: 02/04/97   Time: 21:00
//	Checked in '$/ConstructorWin/Sources'
//	Comment: Initial merge with MSVC build
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 01/24/97   Time: 17:21
//	Checked in '$/Constructor/Source files/CO- Core/Data model/Undo-redo'
//	Comment: Fixed CR/LF problem
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:32
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "DMAttributeSubAction.h"

	// Core : Data model : Attributes
#include "DMAttribute.h"


// ===========================================================================

#pragma mark *** DMAttributeSubAction ***

// ---------------------------------------------------------------------------
//		* DMAttributeSubAction(DMAttribute*)
// ---------------------------------------------------------------------------
//	Constructor. Records the "before" value of the attribute by cloning it.

DMAttributeSubAction::DMAttributeSubAction(
	DMAttribute* inAttribute)
{
	ValidateObject_(inAttribute);

	mAttribute = inAttribute;

	mBeforeValue = (DMAttribute*) inAttribute->Clone();
	ValidateObject_(mBeforeValue.GetObject());
	
	mAffectedObject = mAttribute->GetAffectedObject();
	ValidateObject_(mAffectedObject.GetObject());
}


// ---------------------------------------------------------------------------
//		* ~DMAttributeSubAction
// ---------------------------------------------------------------------------
//	Destructor

DMAttributeSubAction::~DMAttributeSubAction()
{
	ValidateSimpleObject_(this);
}


// ---------------------------------------------------------------------------
//		* Commit
// ---------------------------------------------------------------------------
//	Records the "after" value of the attribute by cloning the attribute.

void
DMAttributeSubAction::Commit()
{
	ValidateThis_();
	ValidateObject_(mAttribute.GetObject());
	
	mAfterValue = (DMAttribute*) mAttribute->Clone();
	ValidateObject_(mAfterValue.GetObject());
}


// ---------------------------------------------------------------------------
//		* RedoSelf
// ---------------------------------------------------------------------------
//	Sets the attribute value to the "after" value.

void
DMAttributeSubAction::RedoSelf()
{
	ValidateThis_();
	ValidateObject_(mAttribute.GetObject());
	ValidateObject_(mAfterValue.GetObject());
	
	mAttribute->SetValueFromClone(mAfterValue);
}


// ---------------------------------------------------------------------------
//		* UndoSelf
// ---------------------------------------------------------------------------
//	Sets the attribute value to the "before" value.

void
DMAttributeSubAction::UndoSelf()
{
	ValidateThis_();
	ValidateObject_(mAttribute.GetObject());
	ValidateObject_(mBeforeValue.GetObject());
	
	mAttribute->SetValueFromClone(mBeforeValue);
}


// ---------------------------------------------------------------------------
//		* GetAffectedObject
// ---------------------------------------------------------------------------
//	Returns the object to which this attribute belongs.

DMObject*
DMAttributeSubAction::GetAffectedObject() const
{
	ValidateThis_();
	ValidateObject_(mAffectedObject.GetObject());
	
	return mAffectedObject;
}
