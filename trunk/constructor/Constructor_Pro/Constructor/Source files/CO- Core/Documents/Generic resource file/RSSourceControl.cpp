// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RSSourceControl.cpp			© 1995-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 03/12/95
//	   $Date: 2006/01/18 01:33:01 $
//	$History: RSSourceControl.cpp $
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 02/20/97   Time: 16:39
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Improved commenting.
//	
//	*****************  Version 6  *****************
//	User: Andrew       Date: 2/05/97    Time: 5:21p
//	Updated in $/ConstructorWin/Sources
//	added prefix file for MSVC build
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:45
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Refactored RS* classes into PD* (generic) and RS* (MacOS resource
//	manager) classes.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 15:04
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Update for the refactored RFMap classes.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/05/96   Time: 15:26
//	Updated in $/Constructor/Source files/Documents/+ Generic resource file
//	Fixed static initializers for cursor resources.
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

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "RSSourceControl.h"

	// MacOS : Resource manager
#include "RFMap.h"
#include "RFResource.h"
#include "RFType.h"

	// PowerPlant : Utility classes
#include <UMemoryMgr.h>
#include <UResourceMgr.h>


// ===============================================================
//		* Resource IDs
// ===============================================================

const ResIDT	CKID_ResourceID				= 128;


// ===========================================================================

#pragma mark *** RSSourceControl ***

// ---------------------------------------------------------------------------
//		* RSSourceControl(RFMap&)
// ---------------------------------------------------------------------------
//	Constructor, specifying resource map to attach to

RSSourceControl::RSSourceControl(
	RFMap&	inRFMap)

: mRFMap(inRFMap)

{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(&mRFMap);
	
	// Clear variables.

	mFileHasCKID = false;

	// Listen to change message from resource map.

	mRFMap.AddListener(this);

}


// ---------------------------------------------------------------------------
//		* ~RSSourceControl
// ---------------------------------------------------------------------------
//	Destructor

RSSourceControl::~RSSourceControl()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** file I/O

// ---------------------------------------------------------------------------
//		* ReadSCCInfo
// ---------------------------------------------------------------------------
//	Reads the CKID resource from the document (if it exists).

void
RSSourceControl::ReadSCCInfo()
{

	// Validate pointers.

	ValidateThis_();
	
	// Grab the resource.
	
	RFResource* ckidRsrc = mRFMap.FindResource('ckid', CKID_ResourceID, false);
	if (ckidRsrc == nil) {
	
		// There's no CKID resource. Bail out now.
	
		mFileHasCKID = false;
		BroadcastMessage(msg_SCC_Changed, nil);
		return;
		
	}

	ValidateObject_(ckidRsrc);
	
	// There is a CKID resource. Grab its data.
	
	CKIDHandle ckidRsrcH = (CKIDHandle) ckidRsrc->GetResData();
	ValidateHandle_((Handle) ckidRsrcH);
	
	// Make sure it's the right version.
	
	if ((**ckidRsrcH).version != 4) {
		::DisposeHandle((Handle) ckidRsrcH);
		mFileHasCKID = false;
		BroadcastMessage(msg_SCC_Changed, nil);
		return;
	}

	// Copy resource to our local buffer & dispose it.
	
	mCKIDRecord = **ckidRsrcH;
	::DisposeHandle((Handle) ckidRsrcH);
	mFileHasCKID = true;

	// Broadcast value-changed message.

	BroadcastMessage(msg_SCC_Changed, nil);

}


// ===========================================================================

#pragma mark -
#pragma mark ** access control

// ---------------------------------------------------------------------------
//		* ModifyReadOnly
// ---------------------------------------------------------------------------
//	Change this document from read-only to modified read-only.

void
RSSourceControl::ModifyReadOnly()
{

	// Validate pointers.

	ValidateThis_();

	// Don't bother unless document is currently read-only and unlocked.

	if ((!IsReadOnly()) || mFileIsLocked)
		return;

	// Flip the magic bit.
	
	mCKIDRecord.modifyReadOnly = true;

	// Flip the magic bit in the CKID resource.

										//* 2.1.2: BUG FIX #392: added this section
										//			and removed method WriteSCCInfo()
	RFResource* ckidRsrc = mRFMap.FindResource('ckid', CKID_ResourceID, true);
	ValidateObject_(ckidRsrc);
	
	Handle ckidRsrcH = ckidRsrc->GetResData();
	ValidateHandle_(ckidRsrcH);
	
	try {
										//* 2.1.2: BUG FIX #417: replaced section with
										//			better version from Apple TN PT 07
		CKIDHandle ckidH = (CKIDHandle) ckidRsrcH;
		(**ckidH).modifyReadOnly = true;
		(**ckidH).checkSum = HandleCheckSum(ckidRsrcH);
		ckidRsrc->SetResData(ckidRsrcH);
	}
	catch(...) {
		// ignore errors
	}
	::DisposeHandle(ckidRsrcH);

	// Broadcast value-changed message.

	BroadcastMessage(msg_SCC_Changed, nil);

}


// ===========================================================================

#pragma mark -
#pragma mark ** accessors

// ---------------------------------------------------------------------------
//		* HasSCCInfo
// ---------------------------------------------------------------------------
//	Return true if we have a valid CKID resource or the file is locked.

Boolean
RSSourceControl::HasSCCInfo() const
{

	// Validate pointers.

	ValidateThis_();
	
	// Return true if CKID exists or file is locked.
	
	return mFileHasCKID || mFileIsLocked;

}


// ---------------------------------------------------------------------------
//		* IsReadOnly
// ---------------------------------------------------------------------------
//	Return true if CKID says this document is read-only.

Boolean
RSSourceControl::IsReadOnly() const
{

	// Validate pointers.

	ValidateThis_();
	
	// Return true if CKID says document is locked.
	
	return mFileHasCKID && ((mCKIDRecord.readOnly == 0) && !mCKIDRecord.modifyReadOnly);

}


// ---------------------------------------------------------------------------
//		* IsModifyReadOnly
// ---------------------------------------------------------------------------
//	Return true if CKID says this document is modified read-only.

Boolean
RSSourceControl::IsModifyReadOnly() const
{

	// Validate pointers.

	ValidateThis_();
	
	// Look at CKID resource data.
	
	return mFileHasCKID && ((mCKIDRecord.readOnly == 0) && mCKIDRecord.modifyReadOnly);

}


// ===========================================================================

#pragma mark -
#pragma mark ** resource file implementation

// ---------------------------------------------------------------------------
//		* ListenToMessage										[protected]
// ---------------------------------------------------------------------------
//	Listen for messages from the resource file map. WeÕre interested to
//	know if the source code control resource exists, goes away, changes, etc.

void
RSSourceControl::ListenToMessage(
	MessageT	inMessage,
	void*		ioParam) 
{

	// Validate pointers.

	ValidateThis_();
	
	// Listen for resource change messages.
	
	switch (inMessage) {
		case Resource_Added:
		case Resource_Removed:
		case Resource_DataChanged:
			HandleResourceMessage((RMMap_ChangeMessage*) ioParam);
			break;
	}
}


// ---------------------------------------------------------------------------
//		* HandleResourceMessage									[protected]
// ---------------------------------------------------------------------------
//	A resource change message was received. If it affects the CKID resource,
//	we update the SCC info.

void
RSSourceControl::HandleResourceMessage(
	const RMMap_ChangeMessage* inMessage)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inMessage->mRsrcType);
	ValidateObject_(inMessage->mResource);
	
	// If CKID resource data is changing, re-read CKID.
	
	RFResource* changedRF = (dynamic_cast<RFResource*>(inMessage->mResource));
	ValidateObject_(changedRF);

	if ((changedRF->GetResType() == 'ckid') && (changedRF->GetResID() == CKID_ResourceID))
	  	ReadSCCInfo();

}


// ---------------------------------------------------------------------------
//		* HandleCheckSum										[protected]
// ---------------------------------------------------------------------------
//	Calculate the checksum for MPW ckid resource format.
//
//* 2.1.2: BUG FIX #417: added method

UInt32
RSSourceControl::HandleCheckSum(
	Handle	inHandle)
{

	// Validate pointers.
	
	ValidateHandle_(inHandle);
	
	// Calculate checksum for handle data.

	SInt32 size = (::GetHandleSize(inHandle) / sizeof (long)) - 1;
	UInt32 sum = 0;
	UInt32* p = (UInt32*) *inHandle;

	p++;
	while (size-- > 0)
		sum += *p++;

	return sum;

}
