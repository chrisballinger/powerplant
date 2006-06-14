// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMDragTask.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 06/07/96
//	   $Date: 2006/01/18 01:32:39 $
//	$History: DMDragTask.cpp $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:14
//	Updated in $/Constructor/Source files/CO- Core/Data model/Utilities
//	Improved commenting.
//
//	*****************  Version 4   *****************
//	User: Andrew       Date: 02/04/97   Time: 21:00
//	Checked in '$/ConstructorWin/Sources'
//	Comment: Initial merge with MSVC build
//	
//	*****************  Version 3   *****************
//	User: scouten      Date: 01/24/97   Time: 17:22
//	Checked in '$/Constructor/Source files/CO- Core/Data model/Utilities'
//	Comment: Fixed CR/LF problem
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/23/96   Time: 23:26
//	Checked in '$/Constructor/Source files/Data model/Utilities'
//	Comment: Added WasDraggedToTrash method.
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

#include "DMDragTask.h"

	// Core : Data model : Core objects
#include "DMElement.h"


// ===========================================================================

#pragma mark *** DMDragTask ***

// ---------------------------------------------------------------------------
//		* DMDragTask(EventRecord&)
// ---------------------------------------------------------------------------
//	Constructor

DMDragTask::DMDragTask(
	const EventRecord&	inEventRecord)

: LDragTask(inEventRecord)

{
}


// ---------------------------------------------------------------------------
//		* ~DMDragTask
// ---------------------------------------------------------------------------
//	Destructor

DMDragTask::~DMDragTask()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** drag setup

// ---------------------------------------------------------------------------
//		* AddDragElement
// ---------------------------------------------------------------------------
//	Call to add a new element to the drag. Converts the data model element
// 	into a simple, internal drag flavor.

ItemReference
DMDragTask::AddDragElement(
	DMElement*		inElement,
	RgnHandle		inGlobalDragRgn,
	const Rect*		inGlobalItemBounds)
{

	ValidateObject_(inElement);
#if PP_Target_Carbon
	ThrowIfNil_(inGlobalDragRgn);
#else
	ValidateHandle_((Handle) inGlobalDragRgn);
#endif

	// Creating the default drag item is pretty simple, really.
	// The item reference parameter is simply a DMElement*.
	// No data is generated when the drag is originated.
	
	ThrowIfOSErr_(::AddDragItemFlavor(
					mDragRef,						// theDragRef
					(ItemReference) inElement,		// theItemRef
					DM_DragFlavor,					// theType
					nil,							// dataPtr
					0,								// dataSize
					0));							// theFlags

	// If a region handle was passed in, we add it to the drag region.
	// This region is assumed to be in global coordinates, but not carved out.
		
	if (inGlobalDragRgn != nil) {
	
		// Carve out a 1-pixel outline of the drag region.
		
		RgnHandle tempRgn = ::NewRgn();
#if PP_Target_Carbon
		ThrowIfNil_(tempRgn);
#else
		ValidateHandle_((Handle) tempRgn);
#endif

		::CopyRgn(inGlobalDragRgn, tempRgn);
		::InsetRgn(tempRgn, 1, 1);
		::DiffRgn(inGlobalDragRgn, tempRgn, tempRgn);
		
		// Add it to the global region.
		
		::UnionRgn(mDragRegion, tempRgn, mDragRegion);
		::DisposeRgn(tempRgn);
	
	}

	// If an item bounds rect was passed in, we set the item bounds now.
	
	if (inGlobalItemBounds != nil) {
		ThrowIfOSErr_(::SetDragItemBounds(
					mDragRef,						// theDragRef
					(ItemReference) inElement,		// theItemRef
					inGlobalItemBounds));			// itemBounds
	}

	// Return the item reference we generated.
	
	return (ItemReference) inElement;

}


// ===========================================================================

#pragma mark -
#pragma mark ** drag information accessors

// ---------------------------------------------------------------------------
//		* WasDraggedToTrash
// ---------------------------------------------------------------------------
//	Returns true if the dragged items were dropped in the trash can.

Boolean
DMDragTask::WasDraggedToTrash()
{
	try {

		// Find out where the drag went.

		StAEDescriptor dropLoc;
		ThrowIfOSErr_(::GetDropLocation(GetDragReference(), &dropLoc.mDesc));

		if (dropLoc.mDesc.descriptorType != typeAlias)
			return false;
	
		// Dragged to Finder. See if it went in the trash.
	
		Boolean aliasWasChanged;
		FSSpec theDestinationFSSpec;
		FSSpec theTrashFSSpec;
		short theTrashVRefNum;
		long theTrashDirID;

		// Build FSSpec for the drag destination.

#if PP_Target_Carbon
		Handle theHandle;
		Size theSize;
		
		theSize = ::AEGetDescDataSize(dropLoc);
		theHandle = ::NewHandle(theSize);
		ThrowIfNil_(theHandle);
		::HLock(theHandle);
		::AEGetDescData(dropLoc, *theHandle, theSize);
		OSErr theErr = ::ResolveAlias(nil, (AliasHandle) theHandle,
							&theDestinationFSSpec, &aliasWasChanged);
		::DisposeHandle(theHandle);
		ThrowIfOSErr_(theErr);
#else
		StHandleLocker(dropLoc.mDesc.dataHandle);
		ThrowIfOSErr_(::ResolveAlias(nil, (AliasHandle) dropLoc.mDesc.dataHandle,
								&theDestinationFSSpec, &aliasWasChanged));
#endif

		// Build FSSpec for the trash can.

		// icw -- You should always use a known vRefNum when finding the trash instead
		// of using kOnSystemDisk.
		ThrowIfOSErr_(::FindFolder(theDestinationFSSpec.vRefNum, kTrashFolderType, kDontCreateFolder,
								&theTrashVRefNum, &theTrashDirID));
		ThrowIfOSErr_(::FSMakeFSSpec( theTrashVRefNum, theTrashDirID, nil, &theTrashFSSpec));

		// Compare the two FSSpecs.

		return ((theDestinationFSSpec.vRefNum == theTrashFSSpec.vRefNum)
				&& (theDestinationFSSpec.parID == theTrashFSSpec.parID)
				&& (::EqualString(theDestinationFSSpec.name, theTrashFSSpec.name, false, true)));

	}
	catch(...) {
		return false;
	}
}
