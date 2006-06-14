// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	StDMSetup.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 04/27/96
//	   $Date: 2006/01/18 01:32:37 $
//	$History: StDMSetup.cpp $
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 03/06/97   Time: 11:09
//	Updated in $/Constructor/Source files/CO- Core/Data model/Core objects
//	Added DMNameOrOrdinalAttribute (for Windows target).
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:24
//	Updated in $/Constructor/Source files/CO- Core/Data model/Core objects
//	Improved commenting.
//	
//	*****************  Version 7  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:06p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 01/31/97   Time: 11:58
//	Updated in $/Constructor/Source files/CO- Core/Data model/Core objects
//	Added registration for DMResourceRefAttribute.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:21
//	Updated in $/Constructor/Source files/CO- Core/Data model/Core objects
//	Fixed CR/LF problem
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 12/13/96   Time: 13:52
//	Updated in $/Constructor/Source files/Data model/Core objects
//	Added support for DMMinMaxIntegerAttribute and DMResListAttribute.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 12/08/96   Time: 11:21
//	Updated in $/Constructor/Source files/Data model/Core objects
//	Upgraded to Clint's drop 12/07/96. Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 10/31/96   Time: 17:52
//	Updated in $/Constructor/Source files/Data model/Core objects
//	Added registration for DMFontStringAttribute.
//
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:30
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "StDMSetup.h"

	// Core : Data model : Core classes
#include "DMObject.h"
#include "DMReanimator.h"

	// Core : Data model : Attributes
#include "DMBitfieldAttribute.h"
#include "DMContainerAttribute.h"
#include "DMFontStringAttribute.h"
#include "DMIntegerAttribute.h"
#include "DMLabelledIntegerAttribute.h"
#include "DMListAttribute.h"
#include "DMMinMaxIntegerAttribute.h"
#include "DMResListAttribute.h"
#include "DMRGBColorAttribute.h"
#include "DMStringAttribute.h"
#include "DMToolboxBooleanAttribute.h"
#include "DMValueLabel.h"
#include "DMResourceRefAttribute.h"

#if Constructor_ForWindows
	#include "DMNameOrOrdinalAttribute.h"
#endif


// ===========================================================================

#pragma mark *** StDMSetup ***

// ---------------------------------------------------------------------------
//		* StDMSetup()
// ---------------------------------------------------------------------------
//	Constructor

StDMSetup::StDMSetup()
{

	// Create the global reanimator object.

	DMReanimator* reanimator = new DMReanimator;
	ValidateObject_(reanimator);
	
	// Be sure it stays around for a while.
	
	reanimator->AddUser(this);
	
	// Register the common data model classes.
	
	RegisterCommonElements();

}


// ---------------------------------------------------------------------------
//		* ~StDMSetup
// ---------------------------------------------------------------------------
//	Destructor

StDMSetup::~StDMSetup()
{

	// Find the global reanimator object.

	DMReanimator* reanimator = DMReanimator::GetReanimator();
	ValidateObject_(reanimator);
	
	// Release our reference to it. This should cause it to
	// be deleted.
	
	reanimator->RemoveUser(this);

}


// ---------------------------------------------------------------------------
//		* RegisterCommonElements								[protected]
// ---------------------------------------------------------------------------
//	Called by constructor of StDMSetup. Registers the common attribute
//	and object classes for the data modelling framework.

void
StDMSetup::RegisterCommonElements()
{

	// Find the global reanimator object.

	DMReanimator* reanimator = DMReanimator::GetReanimator();
	ValidateObject_(reanimator);

	// Register standard data model classes.

		// Core : Data model : Core objects
	reanimator->RegisterClass(DMObject::class_ID, DMObject::CreateFromStream);

		// Core : Data model : Attributes
	reanimator->RegisterClass(DMBitfieldAttribute::class_ID, DMBitfieldAttribute::CreateFromStream);
	reanimator->RegisterClass(DMContainerAttribute::class_ID, DMContainerAttribute::CreateFromStream);
	reanimator->RegisterClass(DMFontStringAttribute::class_ID, DMFontStringAttribute::CreateFromStream);
	reanimator->RegisterClass(DMIntegerAttribute::class_ID, DMIntegerAttribute::CreateFromStream);
	reanimator->RegisterClass(DMLabelledIntegerAttribute::class_ID, DMLabelledIntegerAttribute::CreateFromStream);
	reanimator->RegisterClass(DMListAttribute::class_ID, DMListAttribute::CreateFromStream);
	reanimator->RegisterClass(DMMinMaxIntegerAttribute::class_ID, DMMinMaxIntegerAttribute::CreateFromStream);
	reanimator->RegisterClass(DMResListAttribute::class_ID, DMResListAttribute::CreateFromStream);
	reanimator->RegisterClass(DMRGBColorAttribute::class_ID, DMRGBColorAttribute::CreateFromStream);
	reanimator->RegisterClass(DMStringAttribute::class_ID, DMStringAttribute::CreateFromStream);
	reanimator->RegisterClass(DMValueLabel::class_ID, DMValueLabel::CreateFromStream);
	reanimator->RegisterClass(DMToolboxBooleanAttribute::class_ID, DMToolboxBooleanAttribute::CreateFromStream);
	reanimator->RegisterClass(DMResourceRefAttribute::class_ID, DMResourceRefAttribute::CreateFromStream);

#if Constructor_ForWindows
	reanimator->RegisterClass(DMNameOrOrdinalAttribute::class_ID, DMNameOrOrdinalAttribute::CreateFromStream);
#endif

}
