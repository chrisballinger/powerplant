// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxNavServices.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:27 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxNavServices.h>
#include <PPxPrimaryBundle.h>
#include <PPxSignature.h>
#include <SysCFString.h>

namespace PPx {


namespace {

	// -----------------------------------------------------------------------
	//	Toolbox callback function for handling events while a Navigation
	//	Services dialog is active

	pascal void
	NavEventCallback(
		NavEventCallbackMessage	inMessage,
		NavCBRecPtr				inParams,
		void*					inUserData)
	{
		NavEventResponder*	theResponder =
								static_cast<NavEventResponder*>(inUserData);
								
			// Rather than a single callback, we have special callbacks
			// for "user action" and "terminate", and a generic one for
			// all other messages.
								
		try {
			if (inMessage == kNavCBUserAction) {
			
				theResponder->InvokeNavUserAction(inParams);
				
			} else if (inMessage == kNavCBTerminate) {
			
				theResponder->InvokeNavTerminate(inParams);
				::NavDialogDispose(inParams->context);
				
			} else {
			
				theResponder->InvokeNavEventCallback(inMessage, inParams);
			}
		}
			
		catch (...) { }
	}
	
	
	// -----------------------------------------------------------------------
	//	Returns UPP to event handler callback function

	NavEventUPP
	GetNavEventUPP()
	{
										// Local static is constructed once
										//   first time we are called
		static SysNavEventUPP	sNavEventUPP(NavEventCallback);
		
		return sNavEventUPP.Get();
	}


	// -----------------------------------------------------------------------
	//	Toolbox callback function for filtering objects while a Navigation
	//	Services dialog is active

	pascal Boolean
	NavObjectFilterCallback(
		AEDesc*			inItem,
		void*			inInfo,
		void*			inUserData,
		NavFilterModes	inFilterMode)
	{
		bool displayItem = true;

		NavEventResponder*	theResponder =
								static_cast<NavEventResponder*>(inUserData);

		try
		{
			AutoAEDesc					theItem(*inItem);
			const NavFileOrFolderInfo*	fileOrFolderInfo = NULL;
			
			if ((theItem.GetDescType() == typeFSRef) or
				(theItem.GetDescType() == typeFSS))
			{
				fileOrFolderInfo = static_cast<const NavFileOrFolderInfo*>(inInfo);
			}

			displayItem = theResponder->InvokeNavObjectFilterCallback(theItem,
				fileOrFolderInfo, inFilterMode);
		}
		catch (...)
		{
		}

		return displayItem;
	}
	
	
	// -----------------------------------------------------------------------
	//	Returns UPP to object filter callback function

	NavObjectFilterUPP
	GetNavObjectFilterUPP()
	{
										// Local static is constructed once
										//   first time we are called
		static SysNavObjectFilterUPP	sNavObjectFilterUPP(NavObjectFilterCallback);
		
		return sNavObjectFilterUPP.Get();
	}


	// -----------------------------------------------------------------------
	//	Toolbox callback function for previewing while a Navigation
	//	Services dialog is active

	pascal Boolean
	NavPreviewCallback(
		NavCBRecPtr	inParams,
		void*		inUserData)
	{
		bool previewDrawn = false;

		NavEventResponder*	theResponder =
								static_cast<NavEventResponder*>(inUserData);

		try
		{
			previewDrawn = theResponder->InvokeNavPreviewCallback(inParams);
		}
		catch (...)
		{
		}

		return previewDrawn;
	}
	
	
	// -----------------------------------------------------------------------
	//	Returns UPP to preview callback function

	NavPreviewUPP
	GetNavPreviewUPP()
	{
										// Local static is constructed once
										//   first time we are called
		static SysNavPreviewUPP	sNavPreviewUPP(NavPreviewCallback);
		
		return sNavPreviewUPP.Get();
	}

} // namespace NavServices

#pragma mark -

// ---------------------------------------------------------------------------
//	~NavEventResponder
/**
	Destructor */

NavEventResponder::~NavEventResponder()
{
}


// ---------------------------------------------------------------------------
//	SetFileTypeList
/**
	Sets the list of file types that NavServices will show from an existing
	NavTypeListHandle.  This function takes ownership of the input handle.
	
	@param	inFileTypeList	A NavTypeListHandle object for the class to take
							ownership of and use for filtering items.  You
							may send nil to clear the list.					*/

void NavEventResponder::SetFileTypeList(
	NavTypeListHandle	inFileTypeList)
{
	mFileTypeList.Reset(reinterpret_cast<Handle>(inFileTypeList));
}


// ---------------------------------------------------------------------------
//	SetFileTypeList
/**
	Sets the list of file types that NavServices will show from a list of
	OSTypes that are laid out in contiguous memory.
	
	@param	inNumFileTypes	The number of file types to use for filtering
							items.  You may send zero to clear the list.
	@param	inFileTypes		A pointer to the list of file types.  This
							pointer must be valid if inNumFileTypes is
							greater than zero.								*/

void NavEventResponder::SetFileTypeList(
	SInt16	inNumFileTypes,
	OSType*	inFileTypes)
{
	if (inNumFileTypes > 0)
	{
		PPx_BadParamIfNil_(inFileTypes);
		
		mFileTypeList.Reset( ::NewHandle(sizeof(NavTypeList) +
			(sizeof(OSType) * inNumFileTypes)) );

		NavTypeListPtr	listPtr = (NavTypeListPtr) *mFileTypeList.Get();

		listPtr->componentSignature	= PPx::Signature::Get();
		listPtr->osTypeCount		= inNumFileTypes;

		::BlockMoveData(inFileTypes, listPtr->osType, (sizeof(OSType) * inNumFileTypes));
	}
	else
		mFileTypeList.Reset();
}


// ---------------------------------------------------------------------------
//	SetFileTypeList
/**
	Gets the NavTypeListHandle for the object.  Ownership of the handle
	does not transfer to the caller.
	
	@return	Returns the NavTypeListHandle for the object which may be nil.	*/

NavTypeListHandle
NavEventResponder::GetFileTypeList() const
{
	return reinterpret_cast<NavTypeListHandle>(mFileTypeList.Get());
}


// ---------------------------------------------------------------------------
//	DoNavTerminate
/**
	Responds to the "terminate" message from a Navigation event callback
	
	@param	inParams		NavServices callback record	
	
	Subclasses should override this function if they need to clean up
	custom items in the dialog or perform any other actions upon closing
	of the navigation dialog.												*/

void
NavEventResponder::DoNavTerminate(
	NavCBRecPtr		/* inParams */)
{
}


// ---------------------------------------------------------------------------
//	DoNavEventCallback
/**
	Responds to all Navigation event callback messages other than
	"user action" and "terminate"
	
	@param	inMessage		NavServices callback message
	@param	inParams		NavServices callback record	
	
	Subclasses should override this function if they need to respond to
	particular callback messages. You'll need to do this to add custom
	items to navigation dialogs.											*/

void
NavEventResponder::DoNavEventCallback(
	NavEventCallbackMessage	/* inMessage */,
	NavCBRecPtr				/* inParams */)
{
}


// ---------------------------------------------------------------------------
//	DoNavObjectFilterCallback
/**
	Responds to all Navigation object filter callback messages
	
	@param	inItem				The object your filter function is being
								invoked for.  Never assume that an object
								is a file specification.
	@param	inFileOrFolderInfo	Pointer to info about the object.  This info
								is only valid for objects of descriptor types
								'typeFSS' or 'typeFSRef', so be sure to check
								for nil before derefencing the pointer.
	@param	inFilterMode		A value representing which list of objects is
								currently being filtered.
	
	@return	Return true if Navigation Services should display the object in
			the dialog.

	Subclasses should override this function if they need to filter
	which items are shown in the dialog.									*/

bool
NavEventResponder::DoNavObjectFilterCallback(
	const AutoAEDesc&			/* inItem */,
	const NavFileOrFolderInfo*	/* inFileOrFolderInfo */,
	NavFilterModes				/* inFilterMode */)
{
	return true;
}


// ---------------------------------------------------------------------------
//	DoNavPreviewCallback
/**
	Responds to all Navigation preview callback messages
	
	@param	inParams		NavServices callback record	
	
	@return	Return true if your preview function successfully draws the
	custom file preview, otherwise return false

	Subclasses should override this function if they need to draw
	a custom file preview in the Navigation Services dialog.				*/

bool
NavEventResponder::DoNavPreviewCallback(
	NavCBRecPtr	/* inParams */)
{
	return false;
}

#pragma mark -

// ---------------------------------------------------------------------------
//	AutoNavReply
/**
	Constructor
	
	@param	inNavDialog		Nav dialog for which to get the reply record	*/

AutoNavReply::AutoNavReply(
	NavDialogRef	inNavDialog)
{
	OSStatus	status = ::NavDialogGetReply(inNavDialog, &mNavReply);
	PPx_ThrowIfOSError_(status, "NavDailogGetReply failed");
}


// ---------------------------------------------------------------------------
//	~AutoNavReply
/**
	Destructor */

AutoNavReply::~AutoNavReply()
{
	::NavDisposeReply(&mNavReply);
}


// ---------------------------------------------------------------------------
//	Get
/**
	Returns a reference to the reply record
	
	@return Reference to the reply record									*/

const NavReplyRecord&
AutoNavReply::Get() const
{
	return mNavReply;
}

#pragma mark -

// ---------------------------------------------------------------------------
//	GetDefaultCreationOptions
/**
	Passes back the default options for creating NavServices dialogs
	
	@param	outOptions	Default options for creating NavServices dialogs	*/

void
NavServices::GetDefaultCreationOptions(
	NavDialogCreationOptions&	outOptions)
{
	OSStatus	status = ::NavGetDefaultDialogCreationOptions(&outOptions);
	PPx_ThrowIfOSError_(status, "NavGetDefaultDialogCreationOptions failed");
}


// ---------------------------------------------------------------------------
//	AskSaveChanges
/**
	Displays dialog asking whether to save changes before closing or
	quitting
	
	@param	inResponder		Object for handling callbacks
	@param	inAction		Action code
	@param	inOptions		Options for making the dialog
	
	Action code is either kNavSaveChangesClosingDocument or
	kNavSaveChangesQuittingApplication										*/


void
NavServices::AskSaveChanges(
	NavEventResponder&				inResponder,
	NavAskSaveChangesAction			inAction,
	const NavDialogCreationOptions&	inOptions)
{
	NavDialogRef	navDialog;

	OSStatus	status = ::NavCreateAskSaveChangesDialog(
										&inOptions, inAction,
										GetNavEventUPP(),
										&inResponder,
										&navDialog);
	PPx_ThrowIfOSError_(status, "NavCreateAskSaveChangesDialog failed");
	
	status = ::NavDialogRun(navDialog);
	
	if (status != noErr) {
		::NavDialogDispose(navDialog);
		PPx_ThrowOSError_(status, "NavDialogRun failed");
	}
}


// ---------------------------------------------------------------------------
//	AskSaveChanges
/**
	Displays dialog asking whether to save changes before closing or
	quitting
	
	@param	inResponder		Object for handling callbacks
	@param	inAction		Action code
	@param	inParentWindow	Parent window if dialog is a sheet
	
	Action code is either kNavSaveChangesClosingDocument or
	kNavSaveChangesQuittingApplication										*/

void
NavServices::AskSaveChanges(
	NavEventResponder&				inResponder,
	NavAskSaveChangesAction			inAction,
	WindowRef						inParentWindow)
{
	NavDialogCreationOptions	creationOptions;
	GetDefaultCreationOptions(creationOptions);
	
	creationOptions.clientName = (CFStringRef) PrimaryBundle::Instance().
									GetValueForInfoDictionaryKey(
														kCFBundleNameKey);
	
	if (inParentWindow != nil) {
		creationOptions.modality	 = kWindowModalityWindowModal;
		creationOptions.parentWindow = inParentWindow;
	}
	
	AskSaveChanges(inResponder, inAction, creationOptions);
}



// ---------------------------------------------------------------------------
//	AskDiscardChanges
/**
	Displays dialog asking user if it is OK to discard changes to a document.
	Usually called before performing a "Revert" operation.
	
	@param	inResponder		Object for handling callbacks
	@param	inOptions		Options for making the dialog

	@note You must fill in the saveFileName field of inOptions				*/

void
NavServices::AskDiscardChanges(
	NavEventResponder&				inResponder,
	const NavDialogCreationOptions&	inOptions)
{
	NavDialogRef	navDialog;

	OSStatus	status = ::NavCreateAskDiscardChangesDialog(
										&inOptions,
										GetNavEventUPP(),
										&inResponder,
										&navDialog);
	PPx_ThrowIfOSError_(status, "NavCreateAskDiscardChangesDialog failed");
	
	status = ::NavDialogRun(navDialog);
	
	if (status != noErr) {
		::NavDialogDispose(navDialog);
		PPx_ThrowOSError_(status, "NavDialogRun failed");
	}
}


// ---------------------------------------------------------------------------
//	AskDiscardChanges
/**
	Displays dialog asking user if it is OK to discard changes to a document.
	Usually called before performing a "Revert" operation.
	
	@param	inResponder		Object for handling callbacks
	@param	inParentWindow	Parent window if dialog is a sheet				*/

void
NavServices::AskDiscardChanges(
	NavEventResponder&				inResponder,
	WindowRef						inParentWindow)
{
	NavDialogCreationOptions	creationOptions;
	GetDefaultCreationOptions(creationOptions);
	
	creationOptions.clientName = (CFStringRef) PrimaryBundle::Instance().
									GetValueForInfoDictionaryKey(
														kCFBundleNameKey);
	
	CFString	documentName;
	
	if (inParentWindow != nil) {
		creationOptions.modality	 = kWindowModalityWindowModal;
		creationOptions.parentWindow = inParentWindow;
		
		CFStringRef	title;
		::CopyWindowTitleAsCFString(inParentWindow, &title);
		documentName.AttachRef(title, retain_No);
	}
	
	creationOptions.saveFileName = documentName.UseRef();
	
	AskDiscardChanges(inResponder, creationOptions);
}


// ---------------------------------------------------------------------------
//	AskReviewDocuments
/**
	Displays dialog asking how to handle multiple unsaved documents when
	quitting an application
	
	@param	inResponder			Object for handling callbacks
	@param	inDocumentCount		Number of unsaved documents needing review
	@param	inOptions			Options for making the dialog				*/

void
NavServices::AskReviewDocuments(
	NavEventResponder&				inResponder,
	UInt32							inDocumentCount,
	const NavDialogCreationOptions&	inOptions)
{
	NavDialogRef	navDialog;
	
	OSStatus	status = ::NavCreateAskReviewDocumentsDialog(
									&inOptions,
									inDocumentCount,
									GetNavEventUPP(),
									&inResponder,
									&navDialog);
	PPx_ThrowIfOSError_(status, "NavCreateAskReviewDocumentsDialog failed");
	
	status = ::NavDialogRun(navDialog);
	
	if (status != noErr) {
		::NavDialogDispose(navDialog);
		PPx_ThrowOSError_(status, "NavDialogRun failed");
	}
}


// ---------------------------------------------------------------------------
//	AskReviewDocuments
/**
	Displays dialog asking how to handle multiple unsaved documents when
	quitting an application
	
	@param	inResponder			Object for handling callbacks
	@param	inDocumentCount		Number of unsaved documents needing review	*/

void
NavServices::AskReviewDocuments(
	NavEventResponder&				inResponder,
	UInt32							inDocumentCount)
{
	NavDialogCreationOptions	creationOptions;
	GetDefaultCreationOptions(creationOptions);
	
	creationOptions.clientName = (CFStringRef) PrimaryBundle::Instance().
									GetValueForInfoDictionaryKey(
														kCFBundleNameKey);
	
	AskReviewDocuments(inResponder, inDocumentCount, creationOptions);
}


// ---------------------------------------------------------------------------
//	AskReviewDocuments
/**
	Displays dialog asking how to handle multiple unsaved documents when
	quitting an application
	
	@param	inDocumentCount		Number of unsaved documents needing review
	
	@return	User choice for reviewing documents
	
	@retval	kNavUserActionCancel			Abort quit
	@retval	kNavUserActionReviewDocuments	Ask to save each unsaved document
	@retval	kNavUserActionDiscardDocuments	Quit without saving documents
	
	An "ask review documents" dialog is application-modal. This function
	does not return until the user dismisses the dialog by making a choice
	on how to handle unsaved documents.										*/

NavUserAction
NavServices::AskReviewDocuments(
	UInt32	inDocumentCount)
{
	NavDialogCreationOptions	creationOptions;
	GetDefaultCreationOptions(creationOptions);
	
	creationOptions.clientName = (CFStringRef) PrimaryBundle::Instance().
									GetValueForInfoDictionaryKey(
														kCFBundleNameKey);
														
		// Unlike other nav dialogs which may be sheets, this dialog
		// is app-modal. NavDialogRun does not return until the user
		// dismisses the dialog. To find out which button the user
		// clicked, we call NavDialogGetUserAction rather than using
		// an event filter.

	NavDialogRef	navDialog;
	OSStatus		status = ::NavCreateAskReviewDocumentsDialog(
										&creationOptions, inDocumentCount,
										nil, nil, &navDialog);
	PPx_ThrowIfOSError_(status, "NavCreateAskReviewDocumentsDialog failed");
	
	status = ::NavDialogRun(navDialog);
	
	if (status != noErr) {
		PPx_ThrowOSError_(status, "NavDialogRun failed");
	}
	
	NavUserAction	reviewAction = ::NavDialogGetUserAction(navDialog);
	
	::NavDialogDispose(navDialog);
	
	return reviewAction;
}

#pragma mark -

// ---------------------------------------------------------------------------
//	AskDesignateFile
/**
	Display NavServices dialog for designating a new file. Generally
	referred to as "Put File"
	
	@param	inResponder		Object for handling callbacks
	@param	inFileType		Type of file
	@param	inFileCreator	Creator code for file
	@param	inOptions		Options for making the dialog					*/

void
NavServices::AskDesignateFile(
	NavEventResponder&				inResponder,
	OSType							inFileType,
	OSType							inFileCreator,
	const NavDialogCreationOptions&	inOptions)
{
	NavDialogRef	navDialog;

	OSStatus	status = ::NavCreatePutFileDialog(
										&inOptions,
										inFileType,
										inFileCreator,
										GetNavEventUPP(),
										&inResponder,
										&navDialog);
	PPx_ThrowIfOSError_(status, "NavCreatePutFileDialog failed");
	
	status = ::NavDialogRun(navDialog);
	
	if (status != noErr) {
		::NavDialogDispose(navDialog);
		PPx_ThrowOSError_(status, "NavDialogRun failed");
	}
}


// ---------------------------------------------------------------------------
//	AskDesignateFile
/**
	Display NavServices dialog for designating a new file. Generally
	referred to as "Put File"
	
	@param	inResponder		Object for handling callbacks
	@param	inFileType		Type of file
	@param	inDefaultName	Default name for file
	@param	inParentWindow	Parent window if dialog is a sheet				*/

void
NavServices::AskDesignateFile(
	NavEventResponder&		inResponder,
	OSType					inFileType,
	CFStringRef				inDefaultName,
	WindowRef				inParentWindow)
{
	NavDialogCreationOptions	creationOptions;
	GetDefaultCreationOptions(creationOptions);
	
	creationOptions.clientName = (CFStringRef) PrimaryBundle::Instance().
									GetValueForInfoDictionaryKey(
														kCFBundleNameKey);
	
	creationOptions.saveFileName = inDefaultName;
	
	if (inParentWindow != nil) {
		creationOptions.modality	 = kWindowModalityWindowModal;
		creationOptions.parentWindow = inParentWindow;
	}

	AskDesignateFile(inResponder, inFileType, PPx::Signature::Get(),
						creationOptions);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	AskGetFile
/**
	Display NavServices dialog for getting a file to open
	
	@param	inResponder		Object for handling callbacks
	@param	inOptions		Options for making the dialog					*/

void
NavServices::AskGetFile(
	NavEventResponder&				inResponder,
	const NavDialogCreationOptions&	inOptions)
{
	NavDialogRef	navDialog;

	OSStatus	status = ::NavCreateGetFileDialog(
									&inOptions,
									inResponder.GetFileTypeList(),
									GetNavEventUPP(),
									GetNavPreviewUPP(),
									GetNavObjectFilterUPP(),
									&inResponder,		// Client data
									&navDialog);
	PPx_ThrowIfOSError_(status, "NavCreateGetFileDialog failed");
	
	status = ::NavDialogRun(navDialog);
	
	if (status != noErr) {
		::NavDialogDispose(navDialog);
		PPx_ThrowOSError_(status, "NavDialogRun failed");
	}
}


// ---------------------------------------------------------------------------
//	AskGetFile
/**
	Display NavServices dialog for getting a file to open
	
	@param	inResponder		Object for handling callbacks					*/

void
NavServices::AskGetFile(
	NavEventResponder&	inResponder)
{
	NavDialogCreationOptions	creationOptions;
	GetDefaultCreationOptions(creationOptions);
	
	creationOptions.clientName = (CFStringRef) PrimaryBundle::Instance().
									GetValueForInfoDictionaryKey(
														kCFBundleNameKey);
	
	AskGetFile(inResponder, creationOptions);
}


// ---------------------------------------------------------------------------
//	AskGetFile
/**
	Display NavServices dialog for getting a file to open
	
	@param	inResponder		Object for handling callbacks
	@param	inTypeList		List of file types for file to choose
	@param	inOptions		Options for making the dialog

	@note This function is deprecated and is provided for backwards
	compatibility.															*/

void
NavServices::AskGetFile(
	NavEventResponder&				inResponder,
	NavTypeListHandle				inTypeList,
	const NavDialogCreationOptions&	inOptions)
{
	NavDialogRef	navDialog;

	OSStatus	status = ::NavCreateGetFileDialog(
									&inOptions,
									inTypeList,
									GetNavEventUPP(),
									nil,				// Preview Proc
									nil,				// Filter Proc,
									&inResponder,		// Client data
									&navDialog);
	PPx_ThrowIfOSError_(status, "NavCreateGetFileDialog failed");
	
	status = ::NavDialogRun(navDialog);
	
	if (status != noErr) {
		::NavDialogDispose(navDialog);
		PPx_ThrowOSError_(status, "NavDialogRun failed");
	}
}


// ---------------------------------------------------------------------------
//	AskGetFile
/**
	Display NavServices dialog for getting a file to open
	
	@param	inResponder		Object for handling callbacks
	@param	inTypeList		List of file types for file to choose

	@note This function is deprecated and is provided for backwards
	compatibility.															*/

void
NavServices::AskGetFile(
	NavEventResponder&		inResponder,
	NavTypeListHandle		inTypeList)
{
	NavDialogCreationOptions	creationOptions;
	GetDefaultCreationOptions(creationOptions);
	
	creationOptions.clientName = (CFStringRef) PrimaryBundle::Instance().
									GetValueForInfoDictionaryKey(
														kCFBundleNameKey);
	
	AskGetFile(inResponder, inTypeList, creationOptions);
}


// ---------------------------------------------------------------------------
//	AskChooseFile
/**
	Display NavServices dialog for choosing a file on which to perform
	an operation.
	
	@param	inResponder		Object for handling callbacks
	@param	inOptions		Options for making the dialog
	
	@note Use AskGetFile for selecting a file to open. The system presents
	different dialogs for "get" and "choose".								*/

void
NavServices::AskChooseFile(
	NavEventResponder&				inResponder,
	const NavDialogCreationOptions&	inOptions)
{
	NavDialogRef	navDialog;

	OSStatus	status = ::NavCreateChooseFileDialog(
									&inOptions,
									inResponder.GetFileTypeList(),
									GetNavEventUPP(),
									GetNavPreviewUPP(),
									GetNavObjectFilterUPP(),
									&inResponder,		// Client data
									&navDialog);
	PPx_ThrowIfOSError_(status, "NavCreateGetFileDialog failed");
	
	status = ::NavDialogRun(navDialog);
	PPx_ThrowIfOSError_(status, "NavDialogRun failed");
}


// ---------------------------------------------------------------------------
//	AskChooseFile
/**
	Display NavServices dialog for choosing a file on which to perform
	an operation.
	
	@param	inResponder		Object for handling callbacks					*/

void
NavServices::AskChooseFile(
	NavEventResponder&	inResponder)
{
	NavDialogCreationOptions	creationOptions;
	GetDefaultCreationOptions(creationOptions);
	
	creationOptions.clientName = (CFStringRef) PrimaryBundle::Instance().
									GetValueForInfoDictionaryKey(
														kCFBundleNameKey);
	
	AskChooseFile(inResponder, creationOptions);
}


// ---------------------------------------------------------------------------
//	AskChooseFile
/**
	Display NavServices dialog for choosing a file on which to perform
	an operation.
	
	@param	inResponder		Object for handling callbacks
	@param	inTypeList		List of file types for file to choose
	@param	inOptions		Options for making the dialog
	
	@note This function is deprecated and is provided for backwards
	compatibility.
	@note Use AskGetFile for selecting a file to open. The system presents
	different dialogs for "get" and "choose".								*/

void
NavServices::AskChooseFile(
	NavEventResponder&				inResponder,
	NavTypeListHandle				inTypeList,
	const NavDialogCreationOptions&	inOptions)
{
	NavDialogRef	navDialog;

	OSStatus	status = ::NavCreateChooseFileDialog(
									&inOptions,
									inTypeList,
									GetNavEventUPP(),
									nil,				// Preview Proc
									nil,				// Filter Proc,
									&inResponder,		// Client data
									&navDialog);
	PPx_ThrowIfOSError_(status, "NavCreateGetFileDialog failed");
	
	status = ::NavDialogRun(navDialog);
	PPx_ThrowIfOSError_(status, "NavDialogRun failed");
}


// ---------------------------------------------------------------------------
//	AskChooseFile
/**
	Display NavServices dialog for choosing a file on which to perform
	an operation.
	
	@param	inResponder		Object for handling callbacks
	@param	inTypeList		List of file types for file to choose

	@note This function is deprecated and is provided for backwards
	compatibility.															*/

void
NavServices::AskChooseFile(
	NavEventResponder&		inResponder,
	NavTypeListHandle		inTypeList)
{
	NavDialogCreationOptions	creationOptions;
	GetDefaultCreationOptions(creationOptions);
	
	creationOptions.clientName = (CFStringRef) PrimaryBundle::Instance().
									GetValueForInfoDictionaryKey(
														kCFBundleNameKey);
	
	AskChooseFile(inResponder, inTypeList, creationOptions);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	AskChooseFolder
/**
	Display NavServices dialog for choosing a folder on which to perform
	an operation.
	
	@param	inResponder		Object for handling callbacks
	@param	inOptions		Options for making the dialog					*/

void
NavServices::AskChooseFolder(
	NavEventResponder&				inResponder,
	const NavDialogCreationOptions&	inOptions)
{
	NavDialogRef	navDialog;

	OSStatus	status = ::NavCreateChooseFolderDialog(
									&inOptions,
									GetNavEventUPP(),
									GetNavObjectFilterUPP(),
									&inResponder,		// Client data
									&navDialog);
	PPx_ThrowIfOSError_(status, "NavCreateChooseFolderDialog failed");
	
	status = ::NavDialogRun(navDialog);
	PPx_ThrowIfOSError_(status, "NavDialogRun failed");
}


// ---------------------------------------------------------------------------
//	AskChooseFolder
/**
	Display NavServices dialog for choosing a folder on which to perform
	an operation.
	
	@param	inResponder		Object for handling callbacks					*/

void
NavServices::AskChooseFolder(
	NavEventResponder&	inResponder)
{
	NavDialogCreationOptions	creationOptions;
	GetDefaultCreationOptions(creationOptions);
	
	creationOptions.clientName = (CFStringRef) PrimaryBundle::Instance().
									GetValueForInfoDictionaryKey(
														kCFBundleNameKey);
	
	AskChooseFolder(inResponder, creationOptions);
}


#pragma mark -

// ---------------------------------------------------------------------------
//	AskChooseVolume
/**
	Display NavServices dialog for choosing a volume on which to perform
	an operation.
	
	@param	inResponder		Object for handling callbacks
	@param	inOptions		Options for making the dialog					*/

void
NavServices::AskChooseVolume(
	NavEventResponder&				inResponder,
	const NavDialogCreationOptions&	inOptions)
{
	NavDialogRef	navDialog;

	OSStatus	status = ::NavCreateChooseVolumeDialog(
									&inOptions,
									GetNavEventUPP(),
									GetNavObjectFilterUPP(),
									&inResponder,		// Client data
									&navDialog);
	PPx_ThrowIfOSError_(status, "NavCreateChooseVolumeDialog failed");
	
	status = ::NavDialogRun(navDialog);
	PPx_ThrowIfOSError_(status, "NavDialogRun failed");
}


// ---------------------------------------------------------------------------
//	AskChooseVolume
/**
	Display NavServices dialog for choosing a volume on which to perform
	an operation.
	
	@param	inResponder		Object for handling callbacks					*/

void
NavServices::AskChooseVolume(
	NavEventResponder&	inResponder)
{
	NavDialogCreationOptions	creationOptions;
	GetDefaultCreationOptions(creationOptions);
	
	creationOptions.clientName = (CFStringRef) PrimaryBundle::Instance().
									GetValueForInfoDictionaryKey(
														kCFBundleNameKey);
	
	AskChooseVolume(inResponder, creationOptions);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	AskChooseObject
/**
	Display NavServices dialog for choosing a file, folder, or volume on
	which to perform an operation.
	
	@param	inResponder		Object for handling callbacks
	@param	inOptions		Options for making the dialog					*/

void
NavServices::AskChooseObject(
	NavEventResponder&				inResponder,
	const NavDialogCreationOptions&	inOptions)
{
	NavDialogRef	navDialog;

	OSStatus	status = ::NavCreateChooseObjectDialog(
									&inOptions,
									GetNavEventUPP(),
									GetNavPreviewUPP(),
									GetNavObjectFilterUPP(),
									&inResponder,		// Client data
									&navDialog);
	PPx_ThrowIfOSError_(status, "NavCreateChooseObjectDialog failed");
	
	status = ::NavDialogRun(navDialog);
	PPx_ThrowIfOSError_(status, "NavDialogRun failed");
}


// ---------------------------------------------------------------------------
//	AskChooseObject
/**
	Display NavServices dialog for choosing a file, folder, or volume on
	which to perform an operation.
	
	@param	inResponder		Object for handling callbacks					*/

void
NavServices::AskChooseObject(
	NavEventResponder&	inResponder)
{
	NavDialogCreationOptions	creationOptions;
	GetDefaultCreationOptions(creationOptions);
	
	creationOptions.clientName = (CFStringRef) PrimaryBundle::Instance().
									GetValueForInfoDictionaryKey(
														kCFBundleNameKey);
	
	AskChooseObject(inResponder, creationOptions);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	AskNewFolder
/**
	Display NavServices dialog for creating a new folder.
	
	@param	inResponder		Object for handling callbacks
	@param	inOptions		Options for making the dialog					*/

void
NavServices::AskNewFolder(
	NavEventResponder&				inResponder,
	const NavDialogCreationOptions&	inOptions)
{
	NavDialogRef	navDialog;

	OSStatus	status = ::NavCreateNewFolderDialog(
									&inOptions,
									GetNavEventUPP(),
									&inResponder,		// Client data
									&navDialog);
	PPx_ThrowIfOSError_(status, "NavCreateNewFolderDialog failed");
	
	status = ::NavDialogRun(navDialog);
	PPx_ThrowIfOSError_(status, "NavDialogRun failed");
}


// ---------------------------------------------------------------------------
//	AskNewFolder
/**
	Display NavServices dialog for creating a new folder.
	
	@param	inResponder		Object for handling callbacks					*/

void
NavServices::AskNewFolder(
	NavEventResponder&	inResponder)
{
	NavDialogCreationOptions	creationOptions;
	GetDefaultCreationOptions(creationOptions);
	
	creationOptions.clientName = (CFStringRef) PrimaryBundle::Instance().
									GetValueForInfoDictionaryKey(
														kCFBundleNameKey);
	
	AskNewFolder(inResponder, creationOptions);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SysNavEventUPP							Constructor

SysNavEventUPP::SysNavEventUPP(
	NavEventProcPtr	inCallbackFunc)
{
	mNavEventUPP = ::NewNavEventUPP(inCallbackFunc);
}


// ---------------------------------------------------------------------------
//	~SysNavEventUPP							Destructor

SysNavEventUPP::~SysNavEventUPP()
{
	::DisposeNavEventUPP(mNavEventUPP);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SysNavObjectFilterUPP					Constructor

SysNavObjectFilterUPP::SysNavObjectFilterUPP(
	NavObjectFilterProcPtr	inCallbackFunc)
{
	mNavObjectFilterUPP = ::NewNavObjectFilterUPP(inCallbackFunc);
}


// ---------------------------------------------------------------------------
//	~SysNavObjectFilterUPP					Destructor

SysNavObjectFilterUPP::~SysNavObjectFilterUPP()
{
	::DisposeNavObjectFilterUPP(mNavObjectFilterUPP);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SysNavPreviewUPP						Constructor

SysNavPreviewUPP::SysNavPreviewUPP(
	NavPreviewProcPtr	inCallbackFunc)
{
	mNavPreviewUPP = ::NewNavPreviewUPP(inCallbackFunc);
}


// ---------------------------------------------------------------------------
//	~SysNavPreviewUPP						Destructor

SysNavPreviewUPP::~SysNavPreviewUPP()
{
	::DisposeNavPreviewUPP(mNavPreviewUPP);
}

} // namespace PPx