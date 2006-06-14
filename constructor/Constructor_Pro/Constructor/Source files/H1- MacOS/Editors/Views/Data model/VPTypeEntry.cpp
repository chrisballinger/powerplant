// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPTypeEntry.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 06/12/96
//	   $Date: 2006/01/18 01:33:56 $
//	$History: VPTypeEntry.cpp $
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:40
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Improved commenting.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:45
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Fixed CR/LF problem
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/21/97   Time: 14:25
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Added override of CreateInstance to ensure that class ID field gets
//	filled in properly. (Bug fix #1158.)
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 12/17/96   Time: 16:59
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Silenced signal if default PPob doesn't exist. (Bug fix #1151.)
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 12/08/96   Time: 11:27
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added a bug fix from Clint's 12/07/96 drop: Took out initialization of
//	VETypeEntry fields in InitVPTypeEntry.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/17/96   Time: 11:08
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Improved commenting.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/31/96   Time: 13:59
//	Created
//	Comment: 
//	
// ===========================================================================

#include "VPTypeEntry.h"

	// MacOS : Editors : Views : Data model
#include "VPModelObject.h"


// ===========================================================================

#pragma mark *** VPTypeEntry ***

// ---------------------------------------------------------------------------
//		* VPTypeEntry()
// ---------------------------------------------------------------------------
//	Default constructor

VPTypeEntry::VPTypeEntry()
{
	InitVPTypeEntry();
}


// ---------------------------------------------------------------------------
//		* VPTypeEntry(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. Stream data is:
//		[VETypeEntry data]
//		Int16:		Resource ID for default PPob resource

VPTypeEntry::VPTypeEntry(
	LStream*	inStream)

: VETypeEntry(inStream)

{

	// Clear fields.
	
	InitVPTypeEntry();

	// Read stream data.

	ResIDT ppobID = 0;
	(*inStream)	>> ppobID;

	// Read default PPob (if any).
	
	if (ppobID != 0) {
		mDefaultPPob = ::Get1Resource('PPob', ppobID);
		if (mDefaultPPob != nil) {
			ValidateHandle_(mDefaultPPob);
			::HNoPurge(mDefaultPPob);
			::DetachResource(mDefaultPPob);
		}
	}
}


// ---------------------------------------------------------------------------
//		* VPTypeEntry(VPTypeEntry&)
// ---------------------------------------------------------------------------
//	Copy constructor

VPTypeEntry::VPTypeEntry(
	const VPTypeEntry&	inOriginal)

: VETypeEntry(inOriginal)

{
	InitVPTypeEntry();
}


// ---------------------------------------------------------------------------
//		* ~VPTypeEntry
// ---------------------------------------------------------------------------
//	Destructor

VPTypeEntry::~VPTypeEntry()
{
	if (mDefaultPPob != nil) {
		ValidateHandle_(mDefaultPPob);
		::DisposeHandle(mDefaultPPob);
	}
}


// ---------------------------------------------------------------------------
//		* CreateInstance
// ---------------------------------------------------------------------------
//	Overridden to ensure that class ID field is properly filled in.
//	(2.4.1b1: BUG FIX #1158.)

void
VPTypeEntry::CreateInstance(
	VEModelObjectPtr&	outInstance) const
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mPrototype.GetObject());
	
	// Create an instance.

	VETypeEntry::CreateInstance (outInstance);

	// Set the PowerPlant class ID of the instance.
	//* 2.4.1b1: BUG FIX #1158
	
	VPModelObject* instVP = (dynamic_cast<VPModelObject*>(outInstance.GetObject()));
	if (instVP != nil) {
		ValidateObject_(instVP);
		instVP->SetClassAlias(GetClassID());
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** common initialization

// ---------------------------------------------------------------------------
//		* InitVPTypeEntry										[private]
// ---------------------------------------------------------------------------
//	Common initialization for VPTypeEntry constructors.

void
VPTypeEntry::InitVPTypeEntry()
{
	mDefaultPPob = nil;
}
