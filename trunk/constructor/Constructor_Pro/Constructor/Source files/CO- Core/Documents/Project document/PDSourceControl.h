// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PDSourceControl.h			   © 1997 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 02/03/97
//     $Date: 2006/01/18 01:33:04 $
//  $History: PDSourceControl.h $
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:31
//	Created in $/Constructor/Source files/CO- Core/Documents/Project document
//	Refactored RS* classes into PD* (generic) and RS* (MacOS resource
//	manager) classes.
//	
// ===========================================================================

#pragma once


// ===========================================================================
//		* SCC change messages
// ===========================================================================

const MessageT msg_SCC_Changed			= 'PROJ';
const MessageT msg_ReadOnlyViolation	= 'RONL';


// ===========================================================================
//		* PDSourceControl
// ===========================================================================
//
//	A minimal API for recognizing source code control information and
//	verifying access privileges. Base class only recognizes file locks.
//
// ===========================================================================

class PDSourceControl : public LBroadcaster {

public:
							PDSourceControl();
	virtual					~PDSourceControl();

	// access control

	virtual Boolean			CanMakeChanges(Boolean inSilent = false) const;
	virtual void			SetFileIsLocked(Boolean inLocked);
	virtual void			ModifyReadOnly();

	// accessors

	virtual Boolean			HasSCCInfo() const;
	virtual Boolean			IsReadOnly() const;
	virtual Boolean			IsModifyReadOnly() const;
	virtual Boolean			IsLocked() const;

	// access control violation display

protected:
	virtual void			ShowLockedCursor() const;

	static Cursor&			FetchCursor(
									Cursor&			inCursor,
									ResIDT			inCursorID);

	
	// data members

protected:
	Boolean					mFileIsLocked;				// true if the file is locked


	// class variables

protected:	
	static Cursor			sCKIDReadOnlyCursor;		// cursor when user tries to modify a CKID read only document
	static Cursor			sFileReadOnlyCursor;		// cursor when user tries to modify a read only document
	static Cursor			sFileLockedCursor;			// cursor when user tries to modify a locked document

};
