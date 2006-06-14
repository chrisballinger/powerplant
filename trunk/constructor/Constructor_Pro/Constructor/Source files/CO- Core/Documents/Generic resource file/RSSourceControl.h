// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RSSourceControl.h			© 1995-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 03/12/95
//	   $Date: 2006/01/18 01:33:02 $
//	$History: RSSourceControl.h $
//	
//	*****************  Version 5  *****************
//	User: Andrew       Date: 2/05/97    Time: 5:13p
//	Updated in $/ConstructorWin/Includes
//	changed reference to RMMap_ChangeMessage from class to struct
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:45
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Refactored RS* classes into PD* (generic) and RS* (MacOS resource
//	manager) classes.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 15:04
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Update for the refactored RFMap classes.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/05/96   Time: 14:59
//	Updated in $/Constructor/Source files/Documents/+ Generic resource file
//	Resource editor shell overhaul.
//
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:55
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// Documents : Project document
#include "PDSourceControl.h"


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class RFMap;
struct RMMap_ChangeMessage;


// ===========================================================================
//		* MPW Projector settings record
// ===========================================================================

struct CKIDRec {
	unsigned long	checkSum;
	long			LOC;
	short			version;
	short			readOnly;
	char			branch;
	Boolean			modifyReadOnly;
};
typedef CKIDRec *CKIDPtr, **CKIDHandle;


// ===========================================================================
//		* RSSourceControl
// ===========================================================================
//
//	A pretty good API for recognizing source code control resources and
//	verifying access privileges. Currently only recognizes MPW Projector
//	'ckid' resources, but could easily be extended for other formats.
//
// ===========================================================================

class RSSourceControl :	public PDSourceControl,
						public LListener {

public:
							RSSourceControl(RFMap& inRFMap);
	virtual					~RSSourceControl();

	// file I/O

	virtual void			ReadSCCInfo();
	
	// access control

	virtual void			ModifyReadOnly();

	// accessors

	virtual Boolean			HasSCCInfo() const;
	virtual Boolean			IsReadOnly() const;
	virtual Boolean			IsModifyReadOnly() const;
	
	// resource file implementation

protected:
	virtual void			ListenToMessage(
									MessageT			inMessage,
									void*				ioParam);
	virtual void			HandleResourceMessage(
									const RMMap_ChangeMessage* inMessage);

	UInt32					HandleCheckSum(Handle inHandle);

	
	// data members

protected:
	RFMap&					mRFMap;						// resource map for file
	CKIDRec					mCKIDRecord;				// the CKID record from the file
	Boolean					mFileHasCKID;				// true if this resource exists in the file

};
