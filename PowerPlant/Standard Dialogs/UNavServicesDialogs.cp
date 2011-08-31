// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	UNavServicesDialogs.cp		PowerPlant 2.2.2	©1998-2005 Metrowerks Inc.
// ===========================================================================
//
//	Classes which use Navigation Services dialogs to prompt the user to
//	open and save files

/* ============================================================================
	08/20/2011	RL Aurbach
	
	We assume OSX 10.4 or greater. This means we use (without checking for
	availability) the NavCreate***Dialog APIs, etc.
   ==========================================================================*/

#ifdef PowerPlant_PCH
	#include PowerPlant_PCH
#endif

#include <UNavServicesDialogs.h>

#include <LFileTypeList.h>
#include <LString.h>
#include <PP_Resources.h>
#include <UDesktop.h>
#include <UExtractFromAEDesc.h>
#include <UMemoryMgr.h>
#include <UModalDialogs.h>

#ifndef __MACH__
#include <Script.h>
#endif

PP_Begin_Namespace_PowerPlant


// ---------------------------------------------------------------------------
//	¥ StNavReplyRecord						Default Constructor		  [public]
// ---------------------------------------------------------------------------

UNavServicesDialogs::StNavReplyRecord::StNavReplyRecord()
{
	mNavReply.validRecord = false;
	mAllocated = false;

	SetDefaultValues();
}


// ---------------------------------------------------------------------------
//	¥ ~StNavReplyRecord						Destructor				  [public]
// ---------------------------------------------------------------------------

UNavServicesDialogs::StNavReplyRecord::~StNavReplyRecord()
{
	if (mAllocated) {
		::NavDisposeReply(&mNavReply);
		mAllocated = false;
	}
}


// ---------------------------------------------------------------------------
//	¥ SetDefaultValues												  [public]
// ---------------------------------------------------------------------------

void
UNavServicesDialogs::StNavReplyRecord::SetDefaultValues()
{
									// Clean up record if it is valid
	if (mAllocated) {
		::NavDisposeReply(&mNavReply);
		mAllocated = false;
	}

		// The Nav Services documentation states that the other fields
		// are invalid if validRecord is false. However, we put sensible
		// defaults in each field so that we can access them without
		// having to throw an error if validRecord is false.

	mNavReply.validRecord				= false;
	mNavReply.replacing					= false;
	mNavReply.isStationery				= false;
	mNavReply.translationNeeded			= false;
	mNavReply.selection.descriptorType = typeNull;
	mNavReply.selection.dataHandle		= nil;
	mNavReply.keyScript					= smSystemScript;
	mNavReply.fileTranslation			= nil;
}


// ---------------------------------------------------------------------------
//	¥ UpdateReply													  [public]
// ---------------------------------------------------------------------------
//	Allocate a NavReplyRecord and let the current dialog fill it in.

OSStatus
UNavServicesDialogs::StNavReplyRecord::UpdateReply (
	NavDialogRef	inDialogRef )
{
	if (mAllocated) {
		::NavDisposeReply(&mNavReply);
		mAllocated = false;
	}
	
	OSStatus		status = ::NavDialogGetReply(inDialogRef, &mNavReply);
	if (status == noErr) {
		mAllocated = true;
	}
	return status;
}


// ---------------------------------------------------------------------------
//	¥ GetFileSpec													  [public]
// ---------------------------------------------------------------------------
//	Pass back the FSSpec for the underlying file. Accesses the first file
//	if there is more than one file.

void
UNavServicesDialogs::StNavReplyRecord::GetFileSpec(
	FSSpec&		outFileSpec) const
{
	UExtractFromAEDesc::TheFSSpec(Selection(), outFileSpec);
}


// ---------------------------------------------------------------------------
//	¥ GetFileSpec													  [public]
// ---------------------------------------------------------------------------
//	Pass back the FSRef for the underlying file (or parent folder, in the case
//	of a modern Put-File operation). Accesses the first file if there is more
//	than one file.

void
UNavServicesDialogs::StNavReplyRecord::GetFileSpec(
	FSRef *		outFileSpec) const
{
	AEKeyword	theKey;
	DescType	typeCode;
	Size 		theSize = 0;
	
	OSErr		err = ::AEGetNthPtr(&mNavReply.selection, 1, typeFSRef, &theKey, 
					&typeCode, outFileSpec, sizeof(FSRef), &theSize);
	ThrowIfOSErr_(err);
}


// ---------------------------------------------------------------------------
//	¥ CopySaveFileName												  [public]
// ---------------------------------------------------------------------------
//	Pass back a copy of the saved file name. User must release it.

CFStringRef
UNavServicesDialogs::StNavReplyRecord::CopySaveFileName() const
{
	return ::CFStringCreateCopy(kCFAllocatorDefault, mNavReply.saveFileName);
}


#pragma mark -

// ---------------------------------------------------------------------------
//	¥ Load															  [public]
// ---------------------------------------------------------------------------

void
UNavServicesDialogs::Load()
{
//	::NavLoad();			// Deprecated
}


// ---------------------------------------------------------------------------
//	¥ Unload														  [public]
// ---------------------------------------------------------------------------

void
UNavServicesDialogs::Unload()
{
//	::NavUnload();			// Deprecated
}

#pragma mark -

// ---------------------------------------------------------------------------
//	¥ AskSaveChanges												  [public]
// ---------------------------------------------------------------------------

SInt16
UNavServicesDialogs::AskSaveChanges(
	ConstStringPtr	inDocumentName,
	ConstStringPtr	inAppName,
	bool			inQuitting)
{
/*
	NavDialogOptions	options;
	::NavGetDefaultDialogOptions(&options);

	LString::CopyPStr(inDocumentName, options.savedFileName);
	LString::CopyPStr(inAppName, options.clientName);

	StNavEventUPP				eventUPP(NavEventProc);
	NavAskSaveChangesResult		reply = kNavAskSaveChangesCancel;

	NavAskSaveChangesAction		action = kNavSaveChangesClosingDocument;
	if (inQuitting) {
		action = kNavSaveChangesQuittingApplication;
	}

	UDesktop::Deactivate();

	OSErr	err = ::NavAskSaveChanges(&options, action, &reply, eventUPP, nil);

	UDesktop::Activate();

	ThrowIfOSErr_(err);

	return (SInt16) reply;
/*/
	OSStatus					status = noErr;
	NavDialogCreationOptions	options;
	NavUserAction				result = kNavUserActionNone;

	status = ::NavGetDefaultDialogCreationOptions(&options);
	if (status == noErr) {
		options.location.h = options.location.v = -1;
		options.clientName = ::CFStringCreateWithPascalString(kCFAllocatorDefault,
										inAppName, kCFStringEncodingMacRoman);
		options.saveFileName = ::CFStringCreateWithPascalString(kCFAllocatorDefault,
										inDocumentName, kCFStringEncodingMacRoman);
		NavAskSaveChangesAction		action = kNavSaveChangesClosingDocument;
		if (inQuitting) {
			action = kNavSaveChangesQuittingApplication;
		}
		
		StNavEventUPP		eventUPP(NavEventProc);
		NavDialogRef		dlgRef = nil;
		
		UDesktop::Deactivate();		
		status = ::NavCreateAskSaveChangesDialog (&options, action, eventUPP, nil, &dlgRef);
		if (status == noErr) {
			status = ::NavDialogRun(dlgRef);
			if (status == noErr) {
				result = ::NavDialogGetUserAction(dlgRef);
			}
		}		
		UDesktop::Activate();
		
		if (dlgRef != nil) {
			::NavDialogDispose(dlgRef);
		}
		if (options.clientName != nil) {
			::CFRelease(options.clientName);
		}
		if (options.saveFileName != nil) {
			::CFRelease(options.saveFileName);
		}
	}
	return (SInt16) result;
/**/
}


// ---------------------------------------------------------------------------
//	¥ AskConfirmRevert												  [public]
// ---------------------------------------------------------------------------

bool
UNavServicesDialogs::AskConfirmRevert(
	ConstStringPtr	inDocumentName)
{
/*
	NavDialogOptions	options;
	::NavGetDefaultDialogOptions(&options);

	LString::CopyPStr(inDocumentName, options.savedFileName);

	StNavEventUPP				eventUPP(NavEventProc);
	NavAskDiscardChangesResult	reply = kNavAskDiscardChangesCancel;

	UDesktop::Deactivate();

	OSErr	err = ::NavAskDiscardChanges(&options, &reply, eventUPP, nil);

	UDesktop::Activate();

	ThrowIfOSErr_(err);

	return (reply == kNavAskDiscardChanges);
/*/
	OSStatus		status = noErr;
	NavUserAction	result = kNavUserActionNone;
	NavDialogCreationOptions	options;
	
	status = ::NavGetDefaultDialogCreationOptions(&options);
	if (status == noErr) {
		options.location.h = options.location.v = -1;
		options.saveFileName = ::CFStringCreateWithPascalString(kCFAllocatorDefault,
								inDocumentName, kCFStringEncodingMacRoman);
		StNavEventUPP	eventUPP(NavEventProc);
		NavDialogRef	dlgRef = nil;
		
		UDesktop::Deactivate();
		status = ::NavCreateAskDiscardChangesDialog(&options, eventUPP, nil, &dlgRef);
		if (status == noErr) {
			status = ::NavDialogRun(dlgRef);
			if (status == noErr) {
				result = ::NavDialogGetUserAction(dlgRef);
			}
		}
		UDesktop::Activate();
		
		if (dlgRef != nil) {
			::NavDialogDispose(dlgRef);
		}
		if (options.saveFileName != nil) {
			::CFRelease(options.saveFileName);
		}
	}
	return (result == kNavUserActionDiscardChanges);
/**/
}

#pragma mark -

// ---------------------------------------------------------------------------
//	¥ AskOpenOneFile												  [public]
// ---------------------------------------------------------------------------
//	Simple wrapper for NavGetFile that lets a user select one file using
//	the default options

bool
UNavServicesDialogs::AskOpenOneFile(
	OSType					inFileType,
	FSSpec&					outFileSpec,
	NavDialogOptionFlags	inFlags)
{
	LFileTypeList	fileTypes(inFileType);
	LFileChooser	chooser;

	inFlags &= ~kNavAllowMultipleFiles;		// Can't select multiple files
//	NavDialogOptions*	options = chooser.GetDialogOptions();
	NavDialogCreationOptions *	options = chooser.GetDialogOptions();
	options->optionFlags = inFlags;

	bool	askOK = chooser.AskOpenFile(fileTypes);

	if (askOK) {
		chooser.GetFileSpec(1, outFileSpec);
	}

	return askOK;
}


// ---------------------------------------------------------------------------
//	¥ AskChooseOneFile												  [public]
// ---------------------------------------------------------------------------

bool
UNavServicesDialogs::AskChooseOneFile(
	OSType					inFileType,
	FSSpec&					outFileSpec,
	NavDialogOptionFlags	inFlags)
{
	LFileTypeList	fileTypes(inFileType);
	LFileChooser	chooser;

//	NavDialogOptions*	options = chooser.GetDialogOptions();
	NavDialogCreationOptions *	options = chooser.GetDialogOptions();
	options->optionFlags = inFlags;

	return chooser.AskChooseOneFile(fileTypes, outFileSpec);
}


// ---------------------------------------------------------------------------
//	¥ AskChooseFolder												  [public]
// ---------------------------------------------------------------------------

bool
UNavServicesDialogs::AskChooseFolder(
	FSSpec&					outFileSpec,
	SInt32&					outFolderDirID)
{
	LFileChooser	chooser;

	return chooser.AskChooseFolder(outFileSpec, outFolderDirID);
}


// ---------------------------------------------------------------------------
//	¥ AskChooseVolume												  [public]
// ---------------------------------------------------------------------------

bool
UNavServicesDialogs::AskChooseVolume(
	FSSpec&					outFileSpec)
{
	LFileChooser	chooser;

	return chooser.AskChooseVolume(outFileSpec);
}


// ---------------------------------------------------------------------------
//	¥ AskSaveFile													  [public]
// ---------------------------------------------------------------------------

bool
UNavServicesDialogs::AskSaveFile(
	ConstStringPtr			inDefaultName,
	OSType					inFileType,
	FSSpec&					outFileSpec,
	bool&					outReplacing,
	NavDialogOptionFlags	inFlags)
{
	LFileDesignator		designator;

	designator.SetFileType(inFileType);

	if (not (inFlags & kNavAllowStationery)) {
									// Turn off type popup. The popup
									//   contains file translation and
									//   stationery options. By design
									//   this function doesn't support
									//   file translation.
		inFlags |= kNavNoTypePopup;
	}

//	NavDialogOptions*	options = designator.GetDialogOptions();
	NavDialogCreationOptions *	options = designator.GetDialogOptions();
	options->optionFlags = inFlags;

	bool	askOK = designator.AskDesignateFile(inDefaultName);

	if (askOK) {
		designator.GetFileSpec(outFileSpec);
		outReplacing = designator.IsReplacing();
	}

	return askOK;
}

#pragma mark -

// ===========================================================================
//	LFileChooser Class
// ===========================================================================
//	Uses Navigation Services to ask user to open a file

// ---------------------------------------------------------------------------
//	¥ LFileChooser::LFileChooser			Constructor				  [public]
// ---------------------------------------------------------------------------

UNavServicesDialogs::LFileChooser::LFileChooser()
{
/*
	::NavGetDefaultDialogOptions(&mNavOptions);

	::GetIndString( mNavOptions.windowTitle,
					STRx_Standards, str_OpenDialogTitle);
/*/
	::NavGetDefaultDialogCreationOptions(&mNavOptions);
	Str255				windowTitle;
	::GetIndString(windowTitle, STRx_Standards, str_OpenDialogTitle);
	mNavOptions.windowTitle = ::CFStringCreateWithPascalString(kCFAllocatorDefault, windowTitle, kCFStringEncodingMacRoman);
/**/

	mNavFilterProc	= nil;
	mNavPreviewProc	= nil;
	mSelectDefault	= false;
	mUTIList = nil;
}


// ---------------------------------------------------------------------------
//	¥ LFileChooser::~LFileChooser			Destructor				  [public]
// ---------------------------------------------------------------------------

UNavServicesDialogs::LFileChooser::~LFileChooser()
{
	if (mNavOptions.windowTitle != nil) {
		::CFRelease(mNavOptions.windowTitle);
	}
	if (mUTIList != nil) {
		::CFRelease(mUTIList);
	}
}


// ---------------------------------------------------------------------------
//	¥ LFileChooser::GetDialogOptions								  [public]
// ---------------------------------------------------------------------------

NavDialogCreationOptions*
UNavServicesDialogs::LFileChooser::GetDialogOptions()
{
	return &mNavOptions;
}


// ---------------------------------------------------------------------------
//	¥ LFileChooser::SetDefaultLocation								  [public]
// ---------------------------------------------------------------------------
//	Specify the item that is selected when the "choose" dialog is displayed
//
//	If the inFileSpec refers to a directory, the inSelectIt parameter
//	specifies whether to select the directory itself (true), or to select
//	the first item within the directory (false).

void
UNavServicesDialogs::LFileChooser::SetDefaultLocation(
	const FSSpec&	inFileSpec,
	bool			inSelectIt)
{
	mDefaultLocation = inFileSpec;
	mSelectDefault	 = inSelectIt;
}


// ---------------------------------------------------------------------------
//	¥ LFileChooser::SetObjectFilterProc								  [public]
// ---------------------------------------------------------------------------
//	Specify the object filter callback function
//
//	See the comments for UClassicDialogs::LFileChooser::SetObjectFilterProc()
//	if you wish to use the same callback function for both NavServices
//	and StandardFile.

void
UNavServicesDialogs::LFileChooser::SetObjectFilterProc(
	NavObjectFilterProcPtr	inFilterProc)
{
	mNavFilterProc = inFilterProc;
}


// ---------------------------------------------------------------------------
//	¥ LFileChooser::SetPreviewProc									  [public]
// ---------------------------------------------------------------------------

void
UNavServicesDialogs::LFileChooser::SetPreviewProc(
	NavPreviewProcPtr	inPreviewProc)
{
	mNavPreviewProc = inPreviewProc;
}


// ---------------------------------------------------------------------------
//	¥ LFileChooser::AskOpenFile										  [public]
// ---------------------------------------------------------------------------

bool
UNavServicesDialogs::LFileChooser::AskOpenFile(
	const LFileTypeList&	inFileTypes)
{
	bool					result = false;
									// Create UPPs for callback functions
	StNavEventUPP			eventUPP(NavEventProc);
	StNavObjectFilterUPP	objectFilterUPP(mNavFilterProc);
	StNavPreviewUPP			previewUPP(mNavPreviewProc);

	mNavReply.SetDefaultValues();	// Initialize Reply record
/*
									// Set default location, the location
									//   that's displayed when the dialog
									//   first appears
	AEDesc*		defaultLocationDesc = nil;
	if (not mDefaultLocation.IsNull()) {
		defaultLocationDesc = mDefaultLocation;

		if (mSelectDefault) {
			mNavOptions.dialogOptionFlags |= kNavSelectDefaultLocation;
		} else {
			mNavOptions.dialogOptionFlags &= ~kNavSelectDefaultLocation;
		}
	}

	UDesktop::Deactivate();

	OSErr err = ::NavGetFile(
						defaultLocationDesc,
						mNavReply,
						&mNavOptions,
						eventUPP,
						previewUPP,
						objectFilterUPP,
						inFileTypes.TypeListHandle(),
						0L);							// User Data

	UDesktop::Activate();

	if ( (err != noErr) && (err != userCanceledErr) ) {
		Throw_(err);
	}

	return mNavReply.IsValid();
/*/
	OSStatus					status = noErr;
	NavUserAction				action = kNavUserActionNone;
	NavDialogRef				dlgRef = nil;

	mNavOptions.location.h = mNavOptions.location.v = -1;
	AEDesc *					defLoc = (mDefaultLocation.IsNull() ? nil : (AEDesc*) mDefaultLocation);
	if (mSelectDefault) {
		mNavOptions.optionFlags |= kNavSelectDefaultLocation;
	} else {
		mNavOptions.optionFlags &= ~kNavSelectDefaultLocation;
	}
	if (inFileTypes.NumberOfTypes() > 0 && mUTIList == nil) {
		DeriveUTIList(inFileTypes);
	}
	
	UDesktop::Deactivate();
	status = ::NavCreateGetFileDialog(&mNavOptions, inFileTypes.TypeListHandle(),
								eventUPP, previewUPP, objectFilterUPP, defLoc, &dlgRef);
	if (status == noErr) {
		::NavDialogSetFilterTypeIdentifiers(dlgRef, mUTIList);
		status = ::NavDialogRun(dlgRef);
		if (status == noErr) {
			action = ::NavDialogGetUserAction(dlgRef);
			result = (action == kNavUserActionOpen) || (action == kNavUserActionChoose);
			if (result) {
				mNavReply.UpdateReply(dlgRef);
			}
		}
	}
	UDesktop::Activate();
	if (dlgRef != nil) {
		::NavDialogDispose(dlgRef);
	}
	
	ThrowIfOSStatus_(status);
	
	return result;
/**/
}


// ---------------------------------------------------------------------------
//	¥ LFileChooser::AskChooseOneFile								  [public]
// ---------------------------------------------------------------------------

bool
UNavServicesDialogs::LFileChooser::AskChooseOneFile(
	const LFileTypeList&	inFileTypes,
	FSSpec&					outFileSpec)
{
	bool 					result = false;
									// Create UPPs for callback functions
	StNavEventUPP			eventUPP(NavEventProc);
	StNavObjectFilterUPP	objectFilterUPP(mNavFilterProc);
	StNavPreviewUPP			previewUPP(mNavPreviewProc);

	mNavReply.SetDefaultValues();
/*
									// Can choose only one file
	mNavOptions.optionFlags &= ~kNavAllowMultipleFiles;

									// Set default location, the location
									//   that's displayed when the dialog
									//   first appears
	AEDesc*		defaultLocationDesc = nil;
	if (not mDefaultLocation.IsNull()) {
		defaultLocationDesc = mDefaultLocation;

		if (mSelectDefault) {
			mNavOptions.optionFlags |= kNavSelectDefaultLocation;
		} else {
			mNavOptions.optionFlags &= ~kNavSelectDefaultLocation;
		}
	}

	UDesktop::Deactivate();

	OSErr err = ::NavChooseFile(
						defaultLocationDesc,
						mNavReply,
						&mNavOptions,
						eventUPP,
						previewUPP,
						objectFilterUPP,
						inFileTypes.TypeListHandle(),
						0L);							// User Data

	UDesktop::Activate();

	if ( (err != noErr) && (err != userCanceledErr) ) {
		Throw_(err);
	}

	if (mNavReply.IsValid()) {
		mNavReply.GetFileSpec(outFileSpec);
	}

	return mNavReply.IsValid();
/*/
	OSStatus					status = noErr;
	NavUserAction				action = kNavUserActionNone;
	NavDialogRef				dlgRef = nil;
	
	mNavOptions.optionFlags &= ~kNavAllowMultipleFiles;
	mNavOptions.location.h = mNavOptions.location.v = -1;
	AEDesc *					defLoc = (mDefaultLocation.IsNull() ? nil : (AEDesc*) mDefaultLocation);
	if (mSelectDefault) {
		mNavOptions.optionFlags |= kNavSelectDefaultLocation;
	} else {
		mNavOptions.optionFlags &= ~kNavSelectDefaultLocation;
	}
	if (inFileTypes.NumberOfTypes() > 0 && mUTIList == nil) {
		DeriveUTIList(inFileTypes);
	}
	
	UDesktop::Deactivate();
	status = ::NavCreateChooseFileDialog(&mNavOptions, inFileTypes.TypeListHandle(),
								eventUPP, previewUPP, objectFilterUPP, defLoc, &dlgRef);
	if (status == noErr) {
		::NavDialogSetFilterTypeIdentifiers(dlgRef, mUTIList);
		status = ::NavDialogRun(dlgRef);
		if (status == noErr) {
			action = ::NavDialogGetUserAction(dlgRef);
			result = (action == kNavUserActionOpen) || (action == kNavUserActionChoose);
			if (result) {
				mNavReply.UpdateReply(dlgRef);
				mNavReply.GetFileSpec(outFileSpec);
			}
		}
	}
	UDesktop::Activate();
	
	if (dlgRef != nil) {
		::NavDialogDispose(dlgRef);
	}
	
	ThrowIfOSStatus_(status);
	
	return result;
/**/
}


// ---------------------------------------------------------------------------
//	¥ LFileChooser::AskChooseFolder									  [public]
// ---------------------------------------------------------------------------

bool
UNavServicesDialogs::LFileChooser::AskChooseFolder(
	FSSpec&			outFileSpec,
	SInt32&			outFolderDirID)
{
	bool					result = false;
									// Create UPPs for callback functions
	StNavEventUPP			eventUPP(NavEventProc);
	StNavObjectFilterUPP	objectFilterUPP(mNavFilterProc);

	mNavReply.SetDefaultValues();
/*
									// Set default location, the location
									//   that's displayed when the dialog
									//   first appears
	AEDesc*		defaultLocationDesc = nil;
	if (not mDefaultLocation.IsNull()) {
		defaultLocationDesc = mDefaultLocation;

		if (mSelectDefault) {
			mNavOptions.optionFlags |= kNavSelectDefaultLocation;
		} else {
			mNavOptions.optionFlags &= ~kNavSelectDefaultLocation;
		}
	}

	UDesktop::Deactivate();

	OSErr err = ::NavChooseFolder(
						defaultLocationDesc,
						mNavReply,
						&mNavOptions,
						eventUPP,
						objectFilterUPP,
						0L);							// User Data

	UDesktop::Activate();

	if ( (err != noErr) && (err != userCanceledErr) ) {
		Throw_(err);
	}

	if (mNavReply.IsValid()) {		// User chose a folder

		FSSpec	folderInfo;
		mNavReply.GetFileSpec(folderInfo);

			// The FSSpec from NavChooseFolder is NOT the file spec
			// for the folder. The parID field is actually the DirID
			// of the folder itself, not the folder's parent, and
			// the name field is empty. We call FSMakeFSSpec() using those
			// value to create an FSSpec for the folder itself.

		outFolderDirID = folderInfo.parID;

		::FSMakeFSSpec(folderInfo.vRefNum, folderInfo.parID, folderInfo.name,
						&outFileSpec);
	}

	return mNavReply.IsValid();
/*/
	OSStatus					status = noErr;
	NavUserAction				action = kNavUserActionNone;
	NavDialogRef				dlgRef = nil;

	mNavOptions.location.h = mNavOptions.location.v = -1;
	AEDesc *					defLoc = (mDefaultLocation.IsNull() ? nil : (AEDesc*) mDefaultLocation);
	if (mSelectDefault) {
		mNavOptions.optionFlags |= kNavSelectDefaultLocation;
	} else {
		mNavOptions.optionFlags &= ~kNavSelectDefaultLocation;
	}
	
	UDesktop::Deactivate();
	status = ::NavCreateChooseFolderDialog(&mNavOptions, eventUPP, objectFilterUPP, defLoc, &dlgRef);
	if (status == noErr) {
		status = ::NavDialogRun(dlgRef);
		if (status == noErr) {
			action = ::NavDialogGetUserAction(dlgRef);
			result = (action == kNavUserActionOpen) || (action == kNavUserActionChoose);
			if (result) {
				mNavReply.UpdateReply(dlgRef);
				FSSpec			folderInfo;
				mNavReply.GetFileSpec(folderInfo);
				outFolderDirID = folderInfo.parID;
				::FSMakeFSSpec(folderInfo.vRefNum, folderInfo.parID, folderInfo.name, &outFileSpec);
			}
		}
	}
	UDesktop::Activate();
	if (dlgRef != nil) {
		::NavDialogDispose(dlgRef);
	}
	
	ThrowIfOSStatus_(status);
	
	return result;
/**/
}


// ---------------------------------------------------------------------------
//	¥ LFileChooser::AskChooseVolume									  [public]
// ---------------------------------------------------------------------------

bool
UNavServicesDialogs::LFileChooser::AskChooseVolume(
	FSSpec&		outFileSpec)
{
	bool					result = false;
									// Create UPPs for callback functions
	StNavEventUPP			eventUPP(NavEventProc);
	StNavObjectFilterUPP	objectFilterUPP(mNavFilterProc);

	mNavReply.SetDefaultValues();
/*
	AEDesc*		defaultLocationDesc = nil;
	if (not mDefaultLocation.IsNull()) {
		defaultLocationDesc = mDefaultLocation;

			// mSelectDefault should always be true when selecting
			// volumes since we can't navigate into anything

		mNavOptions.optionFlags |= kNavSelectDefaultLocation;
	}

	UDesktop::Deactivate();

	OSErr err = ::NavChooseVolume(
						defaultLocationDesc,
						mNavReply,
						&mNavOptions,
						eventUPP,
						objectFilterUPP,
						0L);							// User Data

	UDesktop::Activate();

	if (mNavReply.IsValid()) {		// User chose a volume

		FSSpec	volumeInfo;
		mNavReply.GetFileSpec(volumeInfo);

			// The FSSpec from NavChooseFolder is NOT the file spec
			// for the volume. The parID field is actually the DirID
			// of the volume itself, not the volumes's parent, and
			// the name field is empty. We must call PBGetCatInfo
			// to get the parent DirID and volume name

		Str255		name;
		CInfoPBRec	thePB;			// Directory Info Parameter Block
		thePB.dirInfo.ioCompletion	= nil;
		thePB.dirInfo.ioVRefNum		= volumeInfo.vRefNum;	// Volume is right
		thePB.dirInfo.ioDrDirID		= volumeInfo.parID;		// Volumes's DirID
		thePB.dirInfo.ioNamePtr		= name;
		thePB.dirInfo.ioFDirIndex	= -1;	// Lookup using Volume and DirID

		err = ::PBGetCatInfoSync(&thePB);
		ThrowIfOSErr_(err);

											// Create cannonical FSSpec
		::FSMakeFSSpec(thePB.dirInfo.ioVRefNum, thePB.dirInfo.ioDrParID,
					   name, &outFileSpec);
	}

	return mNavReply.IsValid();
/*/
	OSStatus					status = noErr;
	NavUserAction				action = kNavUserActionNone;
	NavDialogRef				dlgRef = nil;

	mNavOptions.location.h = mNavOptions.location.v = -1;
	AEDesc *					defLoc = (mDefaultLocation.IsNull() ? nil : (AEDesc*) mDefaultLocation);
	mNavOptions.optionFlags != kNavSelectDefaultLocation;
	
	UDesktop::Deactivate();
	status = ::NavCreateChooseVolumeDialog(&mNavOptions, eventUPP, objectFilterUPP, defLoc, &dlgRef);
	if (status == noErr) {
		status = ::NavDialogRun(dlgRef);
		if (status == noErr) {
			action = ::NavDialogGetUserAction(dlgRef);
			result = (action == kNavUserActionOpen) || (action == kNavUserActionChoose);
			if (result) {
				mNavReply.UpdateReply(dlgRef);
				FSSpec			volumeInfo;
				mNavReply.GetFileSpec(volumeInfo);

// The FSSpec from NavChooseFolder is NOT the file spec for the volume. 
// The parID field is actually the DirID of the volume itself, not the 
// volumes's parent, and the name field is empty. We must call PBGetCatInfo
// to get the parent DirID and volume name

				Str255			name;
				CInfoPBRec		thePB;			// Directory Info Parameter Block
				thePB.dirInfo.ioCompletion	= nil;
				thePB.dirInfo.ioVRefNum		= volumeInfo.vRefNum;	// Volume is right
				thePB.dirInfo.ioDrDirID		= volumeInfo.parID;		// Volumes's DirID
				thePB.dirInfo.ioNamePtr		= name;
				thePB.dirInfo.ioFDirIndex	= -1;	// Lookup using Volume and DirID

				status = ::PBGetCatInfoSync(&thePB);
				if (status == noErr) {
					::FSMakeFSSpec(thePB.dirInfo.ioVRefNum, thePB.dirInfo.ioDrParID,
							   name, &outFileSpec);
				}
			}
		}
	}
	UDesktop::Activate();
	if (dlgRef != nil) {
		::NavDialogDispose(dlgRef);
	}
	
	ThrowIfOSStatus_(status);
	
	return result;
/**/
}


// ---------------------------------------------------------------------------
//	¥ LFileChooser::IsValid											  [public]
// ---------------------------------------------------------------------------

bool
UNavServicesDialogs::LFileChooser::IsValid() const
{
	return mNavReply.IsValid();
}


// ---------------------------------------------------------------------------
//	¥ LFileChooser::GetNumberOfFiles								  [public]
// ---------------------------------------------------------------------------

SInt32
UNavServicesDialogs::LFileChooser::GetNumberOfFiles() const
{
	SInt32	numFiles = 0;

	if (mNavReply.IsValid()) {
		AEDescList	selectedItems = mNavReply.Selection();
		OSErr	err = ::AECountItems(&selectedItems, &numFiles);

		ThrowIfOSErr_(err);
	}

	return numFiles;
}


// ---------------------------------------------------------------------------
//	¥ LFileChooser::GetFileSpec										  [public]
// ---------------------------------------------------------------------------

void
UNavServicesDialogs::LFileChooser::GetFileSpec(
	SInt32		inIndex,
	FSSpec&		outFileSpec) const
{
	AEKeyword	theKey;
	DescType	theType;
	Size		theSize;

	AEDescList	selectedItems = mNavReply.Selection();
	OSErr err = ::AEGetNthPtr(&selectedItems, inIndex, typeFSS,
						&theKey, &theType, (Ptr) &outFileSpec,
						sizeof(FSSpec), &theSize);

	ThrowIfOSErr_(err);
}

void
UNavServicesDialogs::LFileChooser::GetFileSpec (
	SInt32			inIndex,
	FSRef *			outFSRef ) const
{
    AEKeyword 		theKey;
    DescType 		typeCode;
    Size 			theSize = 0;
    
	AEDescList		selectedItems = mNavReply.Selection();
	OSErr			err = ::AEGetNthPtr(&selectedItems, inIndex, typeFSRef,
						&theKey, &typeCode, outFSRef, sizeof(FSRef), &theSize);
	
	ThrowIfOSErr_(err);
}


void
UNavServicesDialogs::LFileChooser::GetFileSpec (
	SInt32			inIndex,
	CFURLRef *		outURLRef ) const
{
	FSRef			fileRef;
	GetFileSpec(inIndex, &fileRef);
	*outURLRef = ::CFURLCreateFromFSRef(kCFAllocatorDefault, &fileRef);
}


// ---------------------------------------------------------------------------
//	¥ LFileChooser::GetFileDescList									  [public]
// ---------------------------------------------------------------------------

void
UNavServicesDialogs::LFileChooser::GetFileDescList(
	AEDescList&		outDescList) const
{
	outDescList = mNavReply.Selection();
}


// ---------------------------------------------------------------------------
//	¥ LFileChooser::GetScriptCode									  [public]
// ---------------------------------------------------------------------------

ScriptCode
UNavServicesDialogs::LFileChooser::GetScriptCode() const
{
	return mNavReply.KeyScript();
}




// ---------------------------------------------------------------------------
//	¥ GetUTIList													  [public]
// ---------------------------------------------------------------------------
//	Return the list of UTIs associated with this File Type List

CFArrayRef
UNavServicesDialogs::LFileChooser::GetUTIList () const
{
	return (CFArrayRef) mUTIList;
}


// ---------------------------------------------------------------------------
//	¥ CopyUTIList													  [public]
// ---------------------------------------------------------------------------
//	Create a new mutable array, using the contents of the specified array.

void
UNavServicesDialogs::LFileChooser::CopyUTIList (
	CFArrayRef				inArray )
{
	if (mUTIList != nil) {
		::CFRelease(mUTIList);
	}
	mUTIList = ::CFArrayCreateMutableCopy(kCFAllocatorDefault, 0, inArray);
}


// ---------------------------------------------------------------------------
//	¥ AddUTIToList													  [public]
// ---------------------------------------------------------------------------
//	Add the specified UTI to the UTI-List. Create the UTI list if necessary.

void
UNavServicesDialogs::LFileChooser::AddUTIToList (
	CFStringRef				inUTI )
{
	if (mUTIList == nil) {
		mUTIList = ::CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);
	}
	::CFArrayAppendValue(mUTIList, inUTI);
}


// ---------------------------------------------------------------------------
//	¥ AddUTIListToList												  [public]
// ---------------------------------------------------------------------------
//	Add an array of UTIs to the current UTI list. Create the UTI list if
//	necessary.

void
UNavServicesDialogs::LFileChooser::AddUTIListToList (
	CFArrayRef				inUTIList )
{
	if (mUTIList == nil) {
		mUTIList = ::CFArrayCreateMutable(kCFAllocatorDefault, 0, &kCFTypeArrayCallBacks);
	}
	CFRange					theRange = ::CFRangeMake(0, ::CFArrayGetCount(inUTIList));
	::CFArrayAppendArray(mUTIList, inUTIList, theRange);
}


// ---------------------------------------------------------------------------
//	¥ DeriveUTIList													  [public]
// ---------------------------------------------------------------------------
//	Build the array of UTIs by examining each file type in the file type list
//	and appending the associated list of UTIs to our mUTIList.

void
UNavServicesDialogs::LFileChooser::DeriveUTIList (
	const LFileTypeList&	inFileTypes )
{
	SInt16					typeCt = inFileTypes.NumberOfTypes();
	const OSType *			typePtr = inFileTypes.TypeListPtr();
	for (SInt16 i = 0; i < typeCt; i++) {
		CFStringRef			osTypeRef = ::UTCreateStringForOSType(typePtr[i]);
		CFArrayRef			tagArray = ::UTTypeCreateAllIdentifiersForTag(kUTTagClassOSType,
											osTypeRef, nil);
		AddUTIListToList(tagArray);
		::CFRelease(tagArray);
		::CFRelease(osTypeRef);
	}
}


#pragma mark -

// ===========================================================================
//	LFileDesignator Class
// ===========================================================================
//	Uses Navigation Services to ask user to save a file

// ---------------------------------------------------------------------------
//	¥ LFileDesignator::LFileDesignator								  [public]
// ---------------------------------------------------------------------------

UNavServicesDialogs::LFileDesignator::LFileDesignator()
{
/*
	::NavGetDefaultDialogOptions(&mNavOptions);

	::GetIndString( mNavOptions.windowTitle,
					STRx_Standards, str_SaveDialogTitle);
/*/
	::NavGetDefaultDialogCreationOptions(&mNavOptions);
	Str255				windowTitle;
	::GetIndString(windowTitle, STRx_Standards, str_OpenDialogTitle);
	mNavOptions.windowTitle = ::CFStringCreateWithPascalString(kCFAllocatorDefault, windowTitle, kCFStringEncodingMacRoman);
/**/
	mFileType	 = FOUR_CHAR_CODE('\?\?\?\?');
	mFileCreator = LFileTypeList::GetProcessSignature();
}


// ---------------------------------------------------------------------------
//	¥ LFileDesignator::~LFileDesignator								  [public]
// ---------------------------------------------------------------------------

UNavServicesDialogs::LFileDesignator::~LFileDesignator()
{
}


// ---------------------------------------------------------------------------
//	¥ LFileDesignator::SetFileType									  [public]
// ---------------------------------------------------------------------------

void
UNavServicesDialogs::LFileDesignator::SetFileType(
	OSType		inFileType)
{
	mFileType = inFileType;
}


// ---------------------------------------------------------------------------
//	¥ LFileDesignator::SetFileCreator								  [public]
// ---------------------------------------------------------------------------

void
UNavServicesDialogs::LFileDesignator::SetFileCreator(
	OSType		inFileCreator)
{
	mFileCreator = inFileCreator;
}


// ---------------------------------------------------------------------------
//	¥ LFileDesignator::GetDialogOptions								  [public]
// ---------------------------------------------------------------------------

NavDialogCreationOptions*
UNavServicesDialogs::LFileDesignator::GetDialogOptions()
{
	return &mNavOptions;
}


// ---------------------------------------------------------------------------
//	¥ LFileDesignator::SetDefaultLocation							  [public]
// ---------------------------------------------------------------------------

void
UNavServicesDialogs::LFileDesignator::SetDefaultLocation(
	const FSSpec&	inFileSpec,
	bool			inSelectIt)
{
	mDefaultLocation = inFileSpec;
	mSelectDefault	 = inSelectIt;
}


// ---------------------------------------------------------------------------
//	¥ LFileDesignator::AskDesignateFile								  [public]
// ---------------------------------------------------------------------------

bool
UNavServicesDialogs::LFileDesignator::AskDesignateFile(
	ConstStringPtr	inDefaultName)
{
	bool				result = false;
	StNavEventUPP		eventUPP(NavEventProc);

	mNavReply.SetDefaultValues();
/*
	LString::CopyPStr(inDefaultName, mNavOptions.savedFileName);

	AEDesc*		defaultLocationDesc = nil;
	if (not mDefaultLocation.IsNull()) {
		defaultLocationDesc = mDefaultLocation;

		if (mSelectDefault) {
			mNavOptions.dialogOptionFlags |= kNavSelectDefaultLocation;
		} else {
			mNavOptions.dialogOptionFlags &= ~kNavSelectDefaultLocation;
		}
	}

	UDesktop::Deactivate();

	OSErr err = ::NavPutFile(
						defaultLocationDesc,
						mNavReply,
						&mNavOptions,
						eventUPP,
						mFileType,
						mFileCreator,
						0L);					// User Data

	UDesktop::Activate();

	if ( (err != noErr) && (err != userCanceledErr) ) {
		Throw_(err);
	}

	return mNavReply.IsValid();
/*/
	mNavOptions.saveFileName = ::CFStringCreateWithPascalString(kCFAllocatorDefault,
							inDefaultName, kCFStringEncodingMacRoman);

	OSStatus					status = noErr;
	NavUserAction				action = kNavUserActionNone;
	NavDialogRef				dlgRef = nil;

	mNavOptions.location.h = mNavOptions.location.v = -1;
	AEDesc *					defLoc = (mDefaultLocation.IsNull() ? nil : (AEDesc*) mDefaultLocation);
	if (mSelectDefault) {
		mNavOptions.optionFlags |= kNavSelectDefaultLocation;
	} else {
		mNavOptions.optionFlags &= ~kNavSelectDefaultLocation;
	}
	
	UDesktop::Deactivate();
	status = ::NavCreatePutFileDialog(&mNavOptions, mFileType, mFileCreator, 
							eventUPP, defLoc, &dlgRef);
	if (status == noErr) {
		status = ::NavDialogRun(dlgRef);
		if (status == noErr) {
			action = ::NavDialogGetUserAction(dlgRef);
			result = (action == kNavUserActionSaveAs);
			if (result) {
				mNavReply.UpdateReply(dlgRef);
			}
		}
	}
	UDesktop::Activate();
	
	if (dlgRef != nil) {
		::NavDialogDispose(dlgRef);
	}
	if (mNavOptions.saveFileName != nil) {
		::CFRelease(mNavOptions.saveFileName);
	}
	
	ThrowIfOSStatus_(status);
	
	return result;
/**/
}


// ---------------------------------------------------------------------------
//	¥ LFileDesignator::IsValid										  [public]
// ---------------------------------------------------------------------------

bool
UNavServicesDialogs::LFileDesignator::IsValid() const
{
	return mNavReply.IsValid();
}


// ---------------------------------------------------------------------------
//	¥ LFileDesignator::GetFileSpec									  [public]
// ---------------------------------------------------------------------------
//	The selection field of the NavReplyRecord provides access to the parent folder
//	of the designated file; the saveFileName field contains its name. We need to
//	construct a file representation (in the appropriate format) for this designated
//	file.

void
UNavServicesDialogs::LFileDesignator::GetFileSpec(
	FSSpec&		outFileSpec) const
{
/*
	mNavReply.GetFileSpec(outFileSpec);
/*/
	CFStringRef		name = mNavReply.CopySaveFileName();
	Str255			pName;
	::CFStringGetPascalString(name, (StringPtr) &pName, 256, kCFStringEncodingMacRoman);
	::CFRelease(name);

	FSSpec			parentSpec;
	OSErr			err = noErr;
	SInt32			parID = 0;
	Boolean			isFolder;
	CInfoPBRec 		pb;
	Str31 			tempName;

	mNavReply.GetFileSpec(parentSpec);		// Get the parent FSSpec
	pb.dirInfo.ioVRefNum = parentSpec.vRefNum;
	pb.dirInfo.ioDrDirID = parentSpec.parID;
	if ( (parentSpec.name == NULL) || (parentSpec.name[0] == 0) ) {
		tempName[0] = 0;
		pb.dirInfo.ioNamePtr = tempName;
		pb.dirInfo.ioFDirIndex = -1;		// use ioDirID
	}  else  {
		pb.dirInfo.ioNamePtr = (StringPtr)parentSpec.name;
		pb.dirInfo.ioFDirIndex = 0;			//use ioNamePtr and ioDirID
	}
	err = ::PBGetCatInfoSync(&pb);			// Get it's directory ID
	if ( err == noErr ) {
		isFolder = (pb.hFileInfo.ioFlAttrib & kioFlAttribDirMask) != 0;
		if ( isFolder ) {
			parID = pb.dirInfo.ioDrDirID;
		} else {
			parID = pb.hFileInfo.ioFlParID;
		}
// The output FSSpec is made from the parent directory's vRefNum and parID and
// the specified name.
		::FSMakeFSSpec(parentSpec.vRefNum, parID, pName, &outFileSpec);
	}
/**/
}


void
UNavServicesDialogs::LFileDesignator::GetFileSpec (
	FSRef *			outParentRef,
	CFStringRef *	outFileName ) const
{
/*
    AEKeyword		theKey;
    DescType		typeCode;
    Size			theSize = 0;
    
    AEDescList		selectedItems = mNavReply.Selection();
    OSErr			err = ::AEGetNthPtr (&selectedItems, 1, typeFSRef, &theKey, 
    						&typeCode, outParentRef, sizeof(FSRef), &theSize);
    
    *outFileName = ::CFStringCreateCopy(kCFAllocatorDefault, mNavReply.CopySaveFileName());
    
    ThrowIfOSErr_(err);
*/
	mNavReply.GetFileSpec(outParentRef);
	*outFileName = mNavReply.CopySaveFileName();
}


void
UNavServicesDialogs::LFileDesignator::GetFileSpec (
	CFURLRef *		outFileSpec ) const
{
	*outFileSpec = nil;
	
	FSRef			parentRef;
	CFStringRef		nameRef;
	GetFileSpec(&parentRef, &nameRef);
	
	CFURLRef		parentURLRef = ::CFURLCreateFromFSRef(kCFAllocatorDefault, &parentRef);
	if (parentURLRef != nil) {
		*outFileSpec = ::CFURLCreateCopyAppendingPathComponent(kCFAllocatorDefault,
						parentURLRef, nameRef, false);
		::CFRelease(parentURLRef);
	}
	::CFRelease(nameRef);
}


// ---------------------------------------------------------------------------
//	¥ LFileDesignator::IsReplacing									  [public]
// ---------------------------------------------------------------------------

bool
UNavServicesDialogs::LFileDesignator::IsReplacing() const
{
	return mNavReply.IsReplacing();
}


// ---------------------------------------------------------------------------
//	¥ LFileDesignator::IsStationery									  [public]
// ---------------------------------------------------------------------------

bool
UNavServicesDialogs::LFileDesignator::IsStationery() const
{
	return mNavReply.IsStationery();
}


// ---------------------------------------------------------------------------
//	¥ LFileDesignator::GetScriptCode								  [public]
// ---------------------------------------------------------------------------

ScriptCode
UNavServicesDialogs::LFileDesignator::GetScriptCode() const
{
	return mNavReply.KeyScript();
}


// ---------------------------------------------------------------------------
//	¥ LFileDesignator::CompleteSave									  [public]
// ---------------------------------------------------------------------------

void
UNavServicesDialogs::LFileDesignator::CompleteSave(
	NavTranslationOptions	inOption)
{
	if (mNavReply.IsValid()) {
		::NavCompleteSave(mNavReply, inOption);
	}
}


#pragma mark -

// ---------------------------------------------------------------------------
//	¥ NavEventProc													  [static]
// ---------------------------------------------------------------------------
//	Event filter callback routine for Navigation Services

pascal void
UNavServicesDialogs::NavEventProc(
	NavEventCallbackMessage		inSelector,
	NavCBRecPtr					ioParams,
	NavCallBackUserData			ioUserData)
{
	switch (inSelector) {
		case kNavCBStart: {
			if (ioUserData != nil) {
				AEDesc *		defLoc = (AEDesc*) ioUserData;
				if (defLoc != nil) {
					::NavCustomControl(ioParams->context, kNavCtlSetLocation, defLoc);
				}
			}
			break;
		}
		
		case kNavCBEvent: {
			try {
				UModalAlerts::ProcessModalEvent(*(ioParams->eventData.eventDataParms.event));
			} catch (...) {}
			break;
		}
	}
}


// ===========================================================================
//	StNavEventUPP Class
// ===========================================================================

UNavServicesDialogs::StNavEventUPP::StNavEventUPP(
	NavEventProcPtr		inProcPtr)
{
	mNavEventUPP = nil;

	if (inProcPtr != nil) {
		mNavEventUPP = NewNavEventUPP(inProcPtr);
	}
}


UNavServicesDialogs::StNavEventUPP::~StNavEventUPP()
{
	if (mNavEventUPP != nil) {
		DisposeNavEventUPP(mNavEventUPP);
	}
}

#pragma mark -
// ===========================================================================
//	StNavObjectFilterUPP Class
// ===========================================================================

UNavServicesDialogs::StNavObjectFilterUPP::StNavObjectFilterUPP(
	NavObjectFilterProcPtr		inProcPtr)
{
	mNavObjectFilterUPP = nil;

	if (inProcPtr != nil) {
		mNavObjectFilterUPP = NewNavObjectFilterUPP(inProcPtr);
	}
}


UNavServicesDialogs::StNavObjectFilterUPP::~StNavObjectFilterUPP()
{
	if (mNavObjectFilterUPP != nil) {
		DisposeNavObjectFilterUPP(mNavObjectFilterUPP);
	}
}

#pragma mark -
// ===========================================================================
//	StNavPreviewUPP Class
// ===========================================================================

UNavServicesDialogs::StNavPreviewUPP::StNavPreviewUPP(
	NavPreviewProcPtr	inProcPtr)
{
	mNavPreviewUPP = nil;

	if (inProcPtr != nil) {
		mNavPreviewUPP = NewNavPreviewUPP(inProcPtr);
	}
}


UNavServicesDialogs::StNavPreviewUPP::~StNavPreviewUPP()
{
	if (mNavPreviewUPP != nil) {
		DisposeNavPreviewUPP(mNavPreviewUPP);
	}
}


PP_End_Namespace_PowerPlant
