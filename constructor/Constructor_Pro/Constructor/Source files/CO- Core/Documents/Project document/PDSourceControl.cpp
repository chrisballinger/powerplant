// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PDSourceControl.cpp			   © 1997 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 02/03/97
//     $Date: 2006/01/18 01:33:04 $
//  $History: PDSourceControl.cpp $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/20/97   Time: 16:40
//	Updated in $/Constructor/Source files/CO- Core/Documents/Project document
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: Andrew       Date: 2/05/97    Time: 2:40p
//	Updated in $/ConstructorWin/Sources
//	Initial edit for MSVC build (prefix file, /p, and dynamic casts)
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:31
//	Created in $/Constructor/Source files/CO- Core/Documents/Project document
//	Refactored RS* classes into PD* (generic) and RS* (MacOS resource
//	manager) classes.
//	
// ===========================================================================

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "PDSourceControl.h"


// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	CKID_ResourceID				= 128;
const ResIDT	CURS_CKIDReadOnlyCursor		= 15151;
const ResIDT	CURS_FileLockedCursor		= 15154;
const ResIDT	CURS_FileReadOnlyCursor		= 15155;

Cursor PDSourceControl::sCKIDReadOnlyCursor	= FetchCursor(sCKIDReadOnlyCursor, CURS_CKIDReadOnlyCursor);
Cursor PDSourceControl::sFileReadOnlyCursor	= FetchCursor(sFileReadOnlyCursor, CURS_FileReadOnlyCursor);
Cursor PDSourceControl::sFileLockedCursor	= FetchCursor(sFileLockedCursor, CURS_FileLockedCursor);


// ===========================================================================

#pragma mark *** PDSourceControl ***

// ---------------------------------------------------------------------------
//		* PDSourceControl()
// ---------------------------------------------------------------------------
//	Default constructor

PDSourceControl::PDSourceControl()
{
	mFileIsLocked = false;
}


// ---------------------------------------------------------------------------
//		* ~PDSourceControl
// ---------------------------------------------------------------------------
//	Destructor

PDSourceControl::~PDSourceControl()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** access control

// ---------------------------------------------------------------------------
//		* CanMakeChanges
// ---------------------------------------------------------------------------
//	Return true if it's OK to make changes to this document. This routine
//	should be called before making any editing changes. Call with
//	inSilent == true to suppress flashing cursor, etc. when changes
//	are not permitted.

Boolean
PDSourceControl::CanMakeChanges(
	Boolean		inSilent) const
{
	
	// Validate pointers.

	ValidateThis_();

	// See if file is locked.
	
	if (IsLocked()) {
		if (!inSilent)
			ShowLockedCursor();
		return false;
	}

	// Check read-only status.
	
	if (IsReadOnly()) {
		if (!inSilent)
			ShowLockedCursor();
		return false;
	}
	
	// Not locked & not read-only, changes are OK.

	return true;

}


// ---------------------------------------------------------------------------
//		* SetFileIsLocked
// ---------------------------------------------------------------------------
//	Call when opening a document to inform the source control object
//	that the file is locked and cannot be changed.

void
PDSourceControl::SetFileIsLocked(
	Boolean		inLocked)
{

	// Validate pointers.

	ValidateThis_();
	
	// Flag that file is locked.
	
	mFileIsLocked = inLocked;
	
	// Send SCC update message.
	
	BroadcastMessage(msg_SCC_Changed, nil);

}


// ---------------------------------------------------------------------------
//		* ModifyReadOnly
// ---------------------------------------------------------------------------
//	Change this document from read-only to modified read-only. Base class
//	does nothing since it does not support any SCC system.

void
PDSourceControl::ModifyReadOnly()
{
	// override hook
}


// ===========================================================================

#pragma mark -
#pragma mark ** accessors

// ---------------------------------------------------------------------------
//		* HasSCCInfo
// ---------------------------------------------------------------------------
//	Return true if we have a valid SCCS or the file is locked. Default
//	only checks for file locked.

Boolean
PDSourceControl::HasSCCInfo() const
{
	return IsLocked();
}


// ---------------------------------------------------------------------------
//		* IsReadOnly
// ---------------------------------------------------------------------------
//	Return true if the SCCS says this document is read-only. Default
//	always says no since we don't support any SCCS.

Boolean
PDSourceControl::IsReadOnly() const
{
	return false;
}


// ---------------------------------------------------------------------------
//		* IsModifyReadOnly
// ---------------------------------------------------------------------------
//	Return true if the SCCS says this document is modified read-only.
//	Default always says no since we don't support any SCCS.

Boolean
PDSourceControl::IsModifyReadOnly() const
{
	return false;
}


// ---------------------------------------------------------------------------
//		* IsLocked
// ---------------------------------------------------------------------------
//	Return true if the file is locked.

Boolean
PDSourceControl::IsLocked() const
{
	return mFileIsLocked;
}


// ===========================================================================

#pragma mark -
#pragma mark ** accessors

// ---------------------------------------------------------------------------
//		* ShowLockedCursor										[protected]
// ---------------------------------------------------------------------------
//	The user has attempted a change on a document that is read-only or locked.
//	Show a mouse cursor that reflects this error for a few ticks.

void
PDSourceControl::ShowLockedCursor() const
{

	// Validate pointers.

	ValidateThis_();

	// Choose the appropriate mouse cursor.

	Cursor* cursp = nil;
	if (IsReadOnly())
		cursp = &sCKIDReadOnlyCursor;
	else if (IsLocked())
		cursp = &sFileLockedCursor;
	
	// Display this mouse.
	
	if (cursp != nil) {
		::ShowCursor();
		::SetCursor(cursp);
	}
	
	// Blink the file status indicator if there is one.

	(const_cast<PDSourceControl*>(this))->BroadcastMessage(msg_ReadOnlyViolation, nil);
	
	UInt32 junk;
	::Delay(12, &junk);				// sorry, Jon!
	
	// Restore normal mouse.
	
	::InitCursor();

}


// ---------------------------------------------------------------------------
//		* FetchCursor									[static, protected]
// ---------------------------------------------------------------------------
//	Called only at application startup time. Read a mouse cursor.

Cursor&
PDSourceControl::FetchCursor(
	Cursor&		inCursor,
	ResIDT		inCursorID)
{

	// Fetch the cursor resource.

	CursHandle hCursor = ::GetCursor(inCursorID);
	ValidateHandle_((Handle) hCursor);

	// Copy resource data into local buffer.

	inCursor = **hCursor;
	
	// Release the resource.
	
	::ReleaseResource((Handle) hCursor);
	return inCursor;

}
