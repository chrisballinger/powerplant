// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxSheetWindow.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:48 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxSheetWindow.h>
#include <PPxEventUtils.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	SheetWindow
/**
	Default constructor */

SheetWindow::SheetWindow()
{
}


// ---------------------------------------------------------------------------
//	Initialize
/**
	Initializes from parameters and shows the sheet
	
	@param	inWindAttrs		Toolbox window attributes
	@param	inBounds		Bounds of sheet window
	@param	inParent		Window to which drawer is attached
	
	See <MacWindows.h> for information about window attributes				*/

void
SheetWindow::Initialize(
	WindowAttributes	inWindAttrs,
	const Rect&			inBounds,
	Window*				inParent)
{
	Window::Initialize(kSheetWindowClass, inWindAttrs, inBounds, nil);
	
	Show(inParent);
}


// ---------------------------------------------------------------------------
//	Show
/**
	Display the sheet window in a parent window
	
	@param	inParent	Parent window in which to display sheet				*/

void
SheetWindow::Show(
	Window*	inParent)
{
	if (inParent != nil) {
		::ShowSheetWindow(GetSysWindow(), inParent->GetSysWindow());
	}
}


// ---------------------------------------------------------------------------
//	Hide
/**
	Hide the sheet window													*/

void
SheetWindow::Hide()
{
	::HideSheetWindow(GetSysWindow());
}


// ---------------------------------------------------------------------------
//	GetParentWindow
/**
	Returns the parent window of the sheet									*/

WindowRef
SheetWindow::GetParentWindow() const
{
	WindowRef	parent;
	OSStatus	err =  ::GetSheetWindowParent(GetSysWindow(), &parent);
	
	PPx_ThrowIfOSError_(err, "GetSheetWindowParent failed");
	
	return parent;
}


// ---------------------------------------------------------------------------
//	GetClassName
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
SheetWindow::ClassName() const
{
	return CFSTR("PPx::SheetWindow");
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SheetAlert
/**
	Default constructor */

SheetAlert::SheetAlert()
{
	mSysDialogRef	= nil;
	mNotifyTarget	= nil;
	mOKCommand		= commandID_None;
	mCancelCommand	= commandID_None;
	mOtherCommand	= commandID_None;
			
	CommandProcessDoer::Install(GetSysEventTarget());
}


// ---------------------------------------------------------------------------
//	~SheetAlert
/**
	Destructor */

SheetAlert::~SheetAlert()
{
	Close();
}


// ---------------------------------------------------------------------------
//	Initialize
/**
	Initialize from parameters
	
	@param	inErrorMessage		Error message for alert
	@param	inExplanation		Explanation message for alert
	@param	inAlertType			Kind of alert
	@param	inParams			Toolbox Alert settings
	
	The AlertStdCFStringAlertParamRec contains several fields where you
	can set options for the alert. See <Dialogs.h> for details.				*/

void
SheetAlert::Initialize(
	CFStringRef		inErrorMessage,
	CFStringRef		inExplanation,
	AlertType		inAlertType,
	const AlertStdCFStringAlertParamRec*	inParams)
{
	OSStatus	status = ::CreateStandardSheet( inAlertType,
												inErrorMessage,
												inExplanation,
												inParams,
												GetSysEventTarget(),
												&mSysDialogRef);
	PPx_ThrowIfOSError_(status, "CreateStandardSheet failed");
}


// ---------------------------------------------------------------------------
//	Initialize
/**
	Initialize from parameters
	
	@param	inErrorMessage		Error message for alert
	@param	inExplanation		Explanation message for alert
	@param	inAlertType			Kind of alert
	@param	inOKLabel			Title for the OK button
	@param	inCancelLabel		Title for the cancel button
	@param	inOtherLabel		Title for the other button
	
	The button labels are the most common options to specify for
	an alert. Call the version of Initialize that takes a
	AlertStdCFStringAlertParamRec paraemter if you need to set
	other options.
	
	Pass nil for the title of a button if you don't want the button
	to appear in the alert.													*/

void
SheetAlert::Initialize(
	CFStringRef		inErrorMessage,
	CFStringRef		inExplanation,
	AlertType		inAlertType,
	CFStringRef		inOKLabel,
	CFStringRef		inCancelLabel,
	CFStringRef		inOtherLabel)
{
	AlertStdCFStringAlertParamRec	params;
	OSStatus	status =
		::GetStandardAlertDefaultParams(&params, kStdCFStringAlertVersionOne);
	PPx_ThrowIfOSError_(status, "GetStandardAlertDefaultParams failed");
	
	params.defaultText = inOKLabel;
	params.cancelText = inCancelLabel;
	params.otherText = inOtherLabel;
	params.defaultButton = kAlertStdAlertOKButton;
	params.cancelButton = kAlertStdAlertCancelButton;
	
	Initialize(inErrorMessage, inExplanation, inAlertType, &params);
}


// ---------------------------------------------------------------------------
//	GetClassName
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
SheetAlert::ClassName() const
{
	return CFSTR("PPx::SheetAlert");
}


// ---------------------------------------------------------------------------
//	InitState
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
SheetAlert::InitState(
	const DataReader&	inReader)
{
	Correspondent::InitState(inReader);
}


// ---------------------------------------------------------------------------
//	WriteState
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
SheetAlert::WriteState(
	DataWriter& ioWriter) const
{
	Correspondent::WriteState(ioWriter);
}


// ---------------------------------------------------------------------------
//	Show
/**
	Displays a sheet alert in a parent window
	
	@param	inParent		Parent window in which to display sheet alert
	@param	inNotifyTarget	Event target for button commands
	@param	inOKCommand		Command sent after clicking the OK button
	@param	inCancelCommand	Command sent after clicking the cancel button
	@param	inOtherCommand	Cammand sent after clicking the other button
	
	Pass in an event target and command ID number for the buttons if you
	wish to be notified that a particular button was clicked to dismiss
	the alert.
	
	The system will qutomatically close the alert no matter what button
	is clicked.																*/

void
SheetAlert::Show(
	WindowRef		inParent,
	EventTargetRef	inNotifyTarget,
	CommandIDT		inOKCommand,
	CommandIDT		inCancelCommand,
	CommandIDT		inOtherCommand)
{
	mNotifyTarget	= inNotifyTarget;
	mOKCommand		= inOKCommand;
	mCancelCommand	= inCancelCommand;
	mOtherCommand	= inOtherCommand;
	
	OSStatus	status =
			::ShowSheetWindow(::GetDialogWindow(mSysDialogRef), inParent);
	PPx_ThrowIfOSError_(status, "ShowSheetWindow failed");
}


// ---------------------------------------------------------------------------
//	Close
/**
	Closes the sheet alert */

void
SheetAlert::Close()
{
	if (mSysDialogRef != nil) {
		::CloseStandardSheet(mSysDialogRef, 0);
	}
}


// ---------------------------------------------------------------------------
//	DoCommandProcess
/**
	Handles CarbonEvent for command process
	
	@param	ioEvent			CarbonEvent for command process
	@param	inCommand		Command information
	@param	inKeyModifiers	State of modifiers keys
	@param	inMenuContext	Menu context for command
	
	Relays the event as an event for a specific command ID					*/

OSStatus
SheetAlert::DoCommandProcess(
	SysCarbonEvent&	/* ioEvent */,
	HICommand		inCommand,
	UInt32			/* inKeyModifiers */,
	UInt32			/* inMenuContext */)
{
	OSStatus	status = noErr;
	
	switch (inCommand.commandID) {
	
		case kHICommandOK:
			if ((mNotifyTarget != nil) and (mOKCommand != commandID_None)) {
				EventUtils::PostCommandID(mOKCommand, mNotifyTarget);
			}
			break;
			
		case kHICommandCancel:
			if ((mNotifyTarget != nil) and (mCancelCommand != commandID_None)) {
				EventUtils::PostCommandID(mCancelCommand, mNotifyTarget);
			}
			break;
			
		case kHICommandOther:
			if ((mNotifyTarget != nil) and (mOtherCommand != commandID_None)) {
				EventUtils::PostCommandID(mOtherCommand, mNotifyTarget);
			}
			break;
			
		default:
			status = eventNotHandledErr;
			break;
	}
	
	return status;
}


} // namespace PPx
