// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxWindowContentView.cp
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:38 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxWindowContentView.h>
#include <PPxEnvironment.h>
#include <SysEventParam.h>
#include <SysWindow.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	WindowContentView												  [public]
/**
	Default constructor */

WindowContentView::WindowContentView()
{
}


// ---------------------------------------------------------------------------
//	~WindowContentView												  [public]
/**
	Destructor */

WindowContentView::~WindowContentView()
{
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initialize from a WindowRef
	
	@param	inSysWindow		Window reference								*/

void
WindowContentView::Initialize(
	WindowRef	inSysWindow)
{
		// We do not call View::Initialize(). We don't have to create
		// a HIView and put it inside the window because the Toolbox
		// does that automatically. We just need to get the proper
		// HIViewRef and associate it with our wrapper object.
		
		// GetRootControl returns the control we want, whether or not
		// compositing is on, and for all window kinds. See Apple's
		// Technical Q&A QA1214 and QA1215 for details.

	HIViewRef	contentViewRef;
	OSStatus	status = ::GetRootControl(inSysWindow,  &contentViewRef);
	
	if (status == errNoRootControl) {
		status = ::CreateRootControl(inSysWindow, &contentViewRef);
	}
										   
	PPx_ThrowIfOSError_(status, "GetRootControl failed to find window content view");
	
	AdoptSysView(contentViewRef);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
WindowContentView::InitState(
	const DataReader&	inReader)
{
		// We do not call InitViewState(). The system window owns the
		// content view and will set its state upon creation.

	ReadAttachments(inReader);
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
WindowContentView::WriteState(
	DataWriter&	ioWriter) const
{
		// We do not call View::WriteState. The system window owns the
		// content view and will set its state upon creation.

	WriteAttachments(ioWriter);
	WriteViewHierarchy(ioWriter);
}


// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
WindowContentView::ClassName() const
{
	return CFSTR("PPx::WindowContentView");
}


// ---------------------------------------------------------------------------
//	DoControlBoundsChanged										   [protected]
//
//	$$ OS Bug Workaround $$	Needed for OS 10.2.x
//
//	If certain system controls (push button for example) are moved or resized
//	while handling a kControlBoundsChanged event, the "traffic light" buttons
//	at the left side of a window's title bar have their bounds changed as
//	well. The button become very big, extending into the content area of the
//	window. Apple engineers have confirmed that this is an OS bug related
//	to using stale cached data.
//
//	The OS sends this event after a window resizes because the window's
//	content view automatically changes when the window's content region
//	changes. As a workaround, we install a handler on WindowContentView, which
//	is the top level of the PPx view hierarchy. When this handler gets called
//	for the event sent by the OS, it posts a copy of the event directly to
//	the target for the WindowContentView with a special Boolean flag added,
//	and then returns noErr so the event is not propagated.
//
//	Because the copy of the event is posted to the event queue (and not sent
//	immediately), this handler returns control back to the system without
//	doing anything, and the traffic light buttons are fine. Then, when the
//	system pulls the event we posted from the event queue, the system
//	dispatches it directly to this handler (because we specified this view
//	as the target for the event),
//
//	This time, we detect the special flag parameter that we added, and call
//	through to PPx::View's handler. That handler does what we wanted to do in
//	the first place, which is adapt the sizes and locations of PPx Views to
//	the new window size.
	
OSStatus
WindowContentView::DoControlBoundsChanged(
	SysCarbonEvent&	ioEvent,
	ControlRef		inControl,
	UInt32			inChangeAttributes,
	const HIRect&	inOriginalBounds,
	const HIRect&	inCurrentBounds)
{
	const	EventParamName	event_BoundsChanged	= 'BnCh';

	Boolean	callInheritedHandler = true;
	
		// Workaround not required for OS 10.3 or later
		// Workaround does not work for live resizing

	if ( (Environment::GetOSVersion() < 0x1030) and
		 (not (SysWindow(GetSysWindow()).GetWindowAttributes()
		 		& kWindowLiveResizeAttribute)) ) {
									// Check for our special flag
		callInheritedHandler = false;
		SysEventParam::GetOptional(ioEvent, event_BoundsChanged,
													callInheritedHandler);
	}
	
	if (callInheritedHandler) {
		View::DoControlBoundsChanged(ioEvent, inControl,
			inChangeAttributes, inOriginalBounds, inCurrentBounds);
			
	} else {						// Make a copy, add our flag,
									//   and post new event
		SysCarbonEvent myBoundsChanged(ioEvent);
		SysEventParam::Set(myBoundsChanged, event_BoundsChanged, Boolean(true));
		myBoundsChanged.PostTo(GetSysEventTarget(), nil, kEventPriorityHigh);
	}
	
	return noErr;
}


} // namespace PPx
