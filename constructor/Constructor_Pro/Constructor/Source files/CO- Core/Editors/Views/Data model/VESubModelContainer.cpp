// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VESubModelContainer.cpp		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/25/96
//     $Date: 2006/01/18 01:33:12 $
//  $History: VESubModelContainer.cpp $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/20/97   Time: 18:26
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/Data model
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:09p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/30/96   Time: 18:18
//	Created in $/Constructor/Source files/Editors/Views/+ Generic view editor/Data model
//	Added file again. It was checked in as an ObjectMaster file before.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/25/96   Time: 00:37
//	Created in $/Constructor/Source files/Editors/Views/+ Generic view editor/Data model
//	Added class.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "VESubModelContainer.h"

	// Core : Editors : Views : Data model
#include "VEModelObject.h"
#include "VETypeEntry.h"


// ===========================================================================

#pragma mark *** VESubModelContainer ***

// ---------------------------------------------------------------------------
//		* VESubModelContainer()
// ---------------------------------------------------------------------------
//	Default constructor

VESubModelContainer::VESubModelContainer()
{
}


// ---------------------------------------------------------------------------
//		* VESubModelContainer(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. No additional stream data is read.

VESubModelContainer::VESubModelContainer(
	LStream*	inStream)

: DMContainerAttribute(inStream)

{
}


// ---------------------------------------------------------------------------
//		* VESubModelContainer(VESubModelContainer&)
// ---------------------------------------------------------------------------
//	Copy constructor

VESubModelContainer::VESubModelContainer(
	const VESubModelContainer&	inOriginal)

: DMContainerAttribute(inOriginal)

{
}


// ---------------------------------------------------------------------------
//		* ~VESubModelContainer
// ---------------------------------------------------------------------------
//	Destructor

VESubModelContainer::~VESubModelContainer()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** clipboard/drag-and-drop primitives

// ---------------------------------------------------------------------------
//		* CanHostElementSelf									[protected]
// ---------------------------------------------------------------------------
//	Overriden to enforce the "must be root object" flag.

Boolean
VESubModelContainer::CanHostElementSelf(
	const DMElement*	inNewElement,
	DM_DragPlacement*	ioPlacement)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inNewElement);
	
	// Enforce the "must be root object" flag.
	
	const VEModelObject* elementVE = dynamic_const_cast(inNewElement,DMElement*,VEModelObject*);
	if (elementVE != nil) {

		ValidateObject_(elementVE);
		VETypeEntry* typeEntry = elementVE->GetTypeEntry();
		ValidateObject_(typeEntry);
		
		if (typeEntry->MustBeRoot())
			return false;
		
	}
	
	// It's okay on that score, see if it fits otherwise...
	
	return DMContainerAttribute::CanHostElementSelf(inNewElement, ioPlacement);

}
