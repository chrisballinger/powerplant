// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	UMemoryUtils.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/29/96
//     $Date: 2006/01/18 01:33:28 $
//  $History: UMemoryUtils.cpp $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/07/97   Time: 15:15
//	Updated in $/Constructor/Source files/CO- Core/Utility classes
//	Removed #include <stdexcept> (again).
//	
//	*****************  Version 3  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:07p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/22/97   Time: 20:12
//	Updated in $/Constructor/Source files/CO- Core/Utility classes
//	Removed #include <stdexcept>. No longer needed with MSL.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/30/96   Time: 17:14
//	Created in $/Constructor/Source files/Utility classes
//	Added class.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "UMemoryUtils.h"

	// Core : Resource manager
//#include "StResourceContext.h"
#include <UResourceMgr.h>

	// PowerPlant : Utility classes
#include <UModalDialogs.h>

	// ANSI headers
#ifdef WINVER
	#include <new>
#endif


// ===========================================================================
//		* Class variables
// ===========================================================================

bool UMemoryUtils::sShowingAlert			= false;
bool StCriticalSection::sCriticalSection	= false;
bool StInterruptSection::sInterruptSection	= false;


// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	PPob_OutOfMemory	= 25000;
const ResIDT	PPob_LowMemory		= 25010;


// ===========================================================================

#pragma mark *** UMemoryUtils ***

// ---------------------------------------------------------------------------
//		* MemoryIsLow											[static]
// ---------------------------------------------------------------------------
//	Returns true if the application is close to running out of memory.
//	This function is used by CAGrowZone to trigger the low-memory alert,
//	and is used by most of Constructor to disable non-essential menu
//	commands.

bool
UMemoryUtils::MemoryIsLow()
{
	
	// Find the grow zone object. Can't validate the pointer
	// since it is typically allocated as a stack-abased object.
	
	LGrowZone* growZone = LGrowZone::GetGrowZone();
	if (growZone == nil)
		return false;
	
	// Ask the grow zone if memory is low.
	
	return growZone->MemoryIsLow();
	
}


// ---------------------------------------------------------------------------
//		* ShowOutOfMemoryAlert									[static]
// ---------------------------------------------------------------------------
//	Show an alert stating that a command couldn't be completed because there
//	is no more memory.

void
UMemoryUtils::ShowOutOfMemoryAlert()
{
	
	if (sShowingAlert)
		return;
	
	sShowingAlert = true;
	
	try {
	
		// Consider this a critical section, but make sure
		// there's a reasonable amount of memory left.
		
		StCriticalSection crit;

		// Show the can't open dialog.
		
		StApplicationContext appContext;
		StDialogHandler dialog(PPob_OutOfMemory, LCommander::GetTopCommander());
	
		LWindow* window = dialog.GetDialog();
		ValidateObject_(dialog.GetDialog());
		window->Show();
	
		// Wait until OK is hit.
	
		MessageT message;
		do {
			message = dialog.DoDialog();
		} while (message == msg_Nothing);
	}
	catch(...) { }

	sShowingAlert = false;

}


// ---------------------------------------------------------------------------
//		* ShowLowMemoryAlert									[static]
// ---------------------------------------------------------------------------
//	Show an alert stating that some commands are disabled because memory
//	is running low.

void
UMemoryUtils::ShowLowMemoryAlert()
{

	if (sShowingAlert)
		return;
	
	sShowingAlert = true;
	
	try {
	
		// Consider this a critical section, but make sure
		// there's a reasonable amount of memory left.
		
		StCriticalSection crit;

		// Show the can't open dialog.
		
		StApplicationContext appContext;
		StDialogHandler dialog(PPob_LowMemory, LCommander::GetTopCommander());
	
		LWindow* window = dialog.GetDialog();
		ValidateObject_(dialog.GetDialog());
		window->Show();
	
		// Wait until OK is hit.
	
		MessageT message;
		do {
			message = dialog.DoDialog();
		} while (message == msg_Nothing);
	}
	catch(...) { }

	sShowingAlert = false;

}