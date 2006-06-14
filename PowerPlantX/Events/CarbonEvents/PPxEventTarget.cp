// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxEventTarget.cp
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:14 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxEventTarget.h>
#include <PPxSerializer.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	SysEventTarget													 [private]
/**
	Returns the application event target
	
	@return	Event target for the application								*/

EventTargetRef
ApplicationEventTarget::SysEventTarget() const
{
	return ::GetApplicationEventTarget();
}


// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
ApplicationEventTarget::ClassName() const
{
	return CFSTR("PPx::ApplicationEventTarget");
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SysEventTarget													 [private]
/**
	Returns the user focus event target
	
	@return Event target for the user focus									*/

EventTargetRef
UserFocusEventTarget::SysEventTarget() const
{
	return ::GetUserFocusEventTarget();
}


// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
UserFocusEventTarget::ClassName() const
{
	return CFSTR("PPx::UserFocusEventTarget");
}

#pragma mark -

const CFStringRef	key_WindowClass	= CFSTR("window class");


// ---------------------------------------------------------------------------
//	FrontWindowEventTarget											  [public]
/**
	Default constructor */

FrontWindowEventTarget::FrontWindowEventTarget()
{
	mWindowClass = kAllWindowClasses;
}


// ---------------------------------------------------------------------------
//	FrontWindowEventTarget											  [public]
/**
	Constructs from a window class
	
	@param	inWindowClass	Class of Toolbox window							*/

FrontWindowEventTarget::FrontWindowEventTarget(
	WindowClass	inWindowClass)
{
	mWindowClass = inWindowClass;
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
FrontWindowEventTarget::InitState(
	const DataReader&	inReader)
{
	inReader.ReadOptional(key_WindowClass, mWindowClass);
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
FrontWindowEventTarget::WriteState(
	DataWriter&	ioWriter) const
{
	ioWriter.WriteValue(key_WindowClass, mWindowClass);
}


// ---------------------------------------------------------------------------
//	SysEventTarget													 [private]
/**
	Returns the event target for the front window of its class
	
	@return Event target for the front window of its class					*/

EventTargetRef
FrontWindowEventTarget::SysEventTarget() const
{
	EventTargetRef	targetRef = nil;
	
	WindowRef	windRef = ::GetFrontWindowOfClass(mWindowClass, true);
	
	if (windRef != nil) {
		targetRef = ::GetWindowEventTarget(windRef);
	}
	
	return targetRef;
}


// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
FrontWindowEventTarget::ClassName() const
{
	return CFSTR("PPx::FrontWindowEventTarget");
}


} // namespace PPx
