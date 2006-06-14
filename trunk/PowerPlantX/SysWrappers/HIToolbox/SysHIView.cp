// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysHIView.cp
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:36 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <SysHIView.h>
#include <SysCarbonEvent.h>
#include <SysEventHandler.h>
#include <SysEventParam.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	SysHIView														  [public]
/**
	Default constructor */

SysHIView::SysHIView()
{
	mHIViewRef = nil;
}


// ---------------------------------------------------------------------------
//	SysHIView														  [public]
/**
	Constructs from an existing HIViewRef
	
	@param	inViewRef	HIViewRef for this object to use					*/

SysHIView::SysHIView(
	HIViewRef	inViewRef)
{
	mHIViewRef = inViewRef;
	
	if (inViewRef != nil) {
		::CFRetain(inViewRef);
	}
}


// ---------------------------------------------------------------------------
//	SysHIView														  [public]
/**
	Destructor */

SysHIView::~SysHIView()
{
	if (mHIViewRef != nil) {
		::CFRelease(mHIViewRef);
	}
}

#pragma mark -

// ---------------------------------------------------------------------------
//	Adopt															  [public]
/**
	Adopts an existing HIViewRef
	
	@param	inViewRef	HIViewRef to adopt
	
	Releases current HIViewRef and takes ownership of the input HIViewRef	*/

void
SysHIView::Adopt(
	HIViewRef	inViewRef)
{
	if (mHIViewRef != nil) {
		::CFRelease(mHIViewRef);
	}
	
	mHIViewRef = inViewRef;
	
	if (inViewRef != nil) {
		::CFRetain(inViewRef);
	}
}


// ---------------------------------------------------------------------------
//	GetSysEventTarget												  [public]
/**
	Returns the system event target for the HIView							*/

EventTargetRef
SysHIView::GetSysEventTarget() const
{
	return ::HIObjectGetEventTarget((HIObjectRef) mHIViewRef);
}


// ---------------------------------------------------------------------------
//	GetSysView														  [public]
/**
	Returns the HIViewRef for the view
	
	@return HIViewRef for the view											*/

HIViewRef
SysHIView::GetSysView() const
{
	return mHIViewRef;
}


// ---------------------------------------------------------------------------
//	AddSubView														  [public]
/**
	Adds a subview to this view
	
	@param	inSubview	Subview to add										*/

void
SysHIView::AddSubView(
	HIViewRef	inSubview)
{
	OSStatus	status = ::HIViewAddSubview(mHIViewRef, inSubview);
	
	PPx_ThrowIfOSError_(status, "HIViewAddSubview failed");
}


// ---------------------------------------------------------------------------
//	RemoveFromSuperView												  [public]
/**
	Remove this view from its superview										*/

void
SysHIView::RemoveFromSuperView()
{
	OSStatus	status = ::HIViewRemoveFromSuperview(mHIViewRef);
	
	PPx_ThrowIfOSError_(status, "HIViewRemoveFromSuperview failed");
}


// ---------------------------------------------------------------------------
//	GetSuperView													  [public]
/**
	Returns the HIViewRef for this view's superview
	
	@return HIViewRef for this view's superview								*/

HIViewRef
SysHIView::GetSuperView() const
{
	return ::HIViewGetSuperview(mHIViewRef);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	IsVisible														  [public]
/**
	Returns whether the view is visible
	
	@return Whether the view is visible										*/

bool
SysHIView::IsVisible() const
{
	return ::HIViewIsVisible(mHIViewRef);
}


// ---------------------------------------------------------------------------
//	SetVisible														  [public]
/**
	Sets whether view is visible or invisible
	
	@param	inMakeVisible	Whether to make the view visible or invisible	*/

void
SysHIView::SetVisible(
	bool	inMakeVisible)
{
	OSStatus	status = ::HIViewSetVisible(mHIViewRef, inMakeVisible);
	
	PPx_ThrowIfOSError_(status, "HIViewSetVisible failed");
}


// ---------------------------------------------------------------------------
//	IsActive														  [public]
/**
	Returns whether the view is active
	
	@return Whether the view is active										*/

bool
SysHIView::IsActive() const
{
	return ::IsControlActive(mHIViewRef);
}


// ---------------------------------------------------------------------------
//	SetActive														  [public]
/**
	Sets whether view is active or inactive
	
	@param	inActivate		Whether to make the view active or inactive		*/

void
SysHIView::SetActive(
	bool	inActivate)
{
	if (inActivate) {
		::ActivateControl(mHIViewRef);
	} else {
		::DeactivateControl(mHIViewRef);
	}
}


// ---------------------------------------------------------------------------
//	IsEnabled														  [public]
/**
	Returns whether the view is enabled
	
	@return Whether the view is enabled										*/

bool
SysHIView::IsEnabled() const
{
	return ::IsControlEnabled(mHIViewRef);
}


// ---------------------------------------------------------------------------
//	SetEnabled														  [public]
/**
	Sets whether view is enabled or disabled
	
	@param	inEnable		Whether to make the view enabled or disabled	*/

void
SysHIView::SetEnabled(
	bool	inEnable)
{
	if (inEnable) {
		::EnableControl(mHIViewRef);
	} else {
		::DisableControl(mHIViewRef);
	}
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SetValue														  [public]
/**
	Sets the value for the view
	
	@param	inValue		New value for the view								*/

void
SysHIView::SetValue(
	SInt32	inValue)
{
	::SetControl32BitValue(mHIViewRef, inValue);
}


// ---------------------------------------------------------------------------
//	GetValue														  [public]
/**
	Returns the value for the view
	
	@return Value for the view												*/

SInt32
SysHIView::GetValue() const
{
	return ::GetControl32BitValue(mHIViewRef);
}


// ---------------------------------------------------------------------------
//	SetMinValue														  [public]
/**
	Sets the minimum value for the view
	
	@param	inMinValue	New minimum value for the view						*/

void
SysHIView::SetMinValue(
	SInt32	inMinValue)
{
	::SetControl32BitMinimum(mHIViewRef, inMinValue);
}


// ---------------------------------------------------------------------------
//	GetMinValue														  [public]
/**
	Returns the minimum value for the view
	
	@return Minimum value for the view										*/

SInt32
SysHIView::GetMinValue() const
{
	return ::GetControl32BitMinimum(mHIViewRef);
}


// ---------------------------------------------------------------------------
//	SetMaxValue														  [public]
/**
	Sets the maximum value for the view
	
	@param	inMaxValue	New maximum value for the view						*/

void
SysHIView::SetMaxValue(
	SInt32	inMaxValue)
{
	::SetControl32BitMaximum(mHIViewRef, inMaxValue);
}


// ---------------------------------------------------------------------------
//	GetMaxValue														  [public]
/**
	Returns the maximum value for the view
	
	@return Maximum value for the view										*/

SInt32
SysHIView::GetMaxValue() const
{
	return ::GetControl32BitMaximum(mHIViewRef);
}


// ---------------------------------------------------------------------------
//	SetViewSize														  [public]
/**
	Set the view size used to determine scrolling
	
	@param	inViewSize		New view size for view							*/

void
SysHIView::SetViewSize(
	SInt32	inViewSize)
{
	::SetControlViewSize(mHIViewRef, inViewSize);
}


// ---------------------------------------------------------------------------
//	GetViewSize														  [public]
/**
	Returns the view size of the view
	
	@return View size of the view											*/

SInt32
SysHIView::GetViewSize() const
{
	return ::GetControlViewSize(mHIViewRef);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SetTitle														  [public]
/**
	Sets the title of the view
	
	@param	inTitle		New title for the view								*/

void
SysHIView::SetTitle(
	CFStringRef	inTitle)
{
	OSStatus	status = ::SetControlTitleWithCFString(mHIViewRef, inTitle);
	
	PPx_ThrowIfOSError_(status, "SetControlTitleWithCFString failed");
}


// ---------------------------------------------------------------------------
//	GetTitle														  [public]
/**
	Returns the title of the view
	
	@return Title of the view												*/

CFString
SysHIView::GetTitle() const
{
	CFStringRef	title;
	
	OSStatus	status = ::CopyControlTitleAsCFString(mHIViewRef, &title);
	
	PPx_ThrowIfOSError_(status, "SetControlTitleWithCFString failed");
	
	return CFString(title, retain_No);
}


// ---------------------------------------------------------------------------
//	SetCommandID													  [public]
/**
	Sets the Command ID sent when the view is clicked
	
	@param	inCommandID		New command ID for view							*/

void
SysHIView::SetCommandID(
	UInt32	inCommandID)
{
	OSStatus	status = ::SetControlCommandID(mHIViewRef, inCommandID);
	
	PPx_ThrowIfOSError_(status, "SetControlCommandID failed");
}


// ---------------------------------------------------------------------------
//	GetCommandID													  [public]
/**
	Returns the command ID for the view
	
	@return Command ID for the view											*/

UInt32
SysHIView::GetCommandID() const
{
	UInt32		commandID;
	
	OSStatus	status = ::GetControlCommandID(mHIViewRef, &commandID);
	
	PPx_ThrowIfOSError_(status, "GetControlCommandID failed");
	
	return commandID;
}


// ---------------------------------------------------------------------------
//	SetDataTag														  [public]
/**
	Sets a tagged data value for the view
	
	@param	inPartCode	Part of the view to which the data applies
	@param	inTag		Tag name of daa value
	@param	inDataSize	Byte length of data
	@param	inDataPtr	Pointer to data buffer								*/

void
SysHIView::SetDataTag(
	SInt16				inPartCode,
	FourCharCode		inTag,
	Size				inDataSize,
	const void*			inDataPtr)
{
	OSStatus	status = ::SetControlData( mHIViewRef, inPartCode, inTag,
										   inDataSize, inDataPtr );
										   
	PPx_ThrowIfOSError_(status, "SetControlData failed");
}


// ---------------------------------------------------------------------------
//	GetDataTag														  [public]
/**
	Gets a tggaed data value for the view
	
	@param	inPartCode		Part of the view to which the data applies
	@param	inTag			Tag name of daa value
	@param	inBufferSize	Length of data buffer
	@param	inBuffer		Poitner to data buffer
	@param	outDataSize		Actual size of data value						*/

OSStatus
SysHIView::GetDataTag(
	SInt16			inPartCode,
	FourCharCode	inTag,
	Size			inBufferSize,
	void*			inBuffer,
	Size			*outDataSize) const
{
	return ::GetControlData( mHIViewRef, inPartCode, inTag,
							 inBufferSize, inBuffer, outDataSize );
}


// ---------------------------------------------------------------------------
//	SetProperty														  [public]
/**
	Sets a property for the view
	
	@param	inCreator		Creator code for identifying the property
	@param	inTag			Tag for identifying the property
	@param	inSize			Size of the property data
	@param	inPropertyPtr	Pointer to buffer of property data
	
	A creator code and tag identify a property								*/

void
SysHIView::SetProperty(
	OSType			inCreator,
	OSType			inTag,
	UInt32			inSize,
	const void*		inPropertyPtr)
{
	OSStatus	status = ::SetControlProperty( mHIViewRef, inCreator, inTag,
											   inSize, inPropertyPtr );
										   
	PPx_ThrowIfOSError_(status, "SetControlProperty failed");
}


// ---------------------------------------------------------------------------
//	GetProperty														  [public]
/**
	Gets a property for the view
	
	@param	inCreator		Creator code for identifying the property
	@param	inTag			Tag for identifying the property
	@param	inBufferSize	Size of the property data
	@param	inBuffer		Pointer to buffer for property data
	@param	outSize			Actual number of bytes retrieved				*/

OSStatus
SysHIView::GetProperty(
	OSType			inCreator,
	OSType			inTag,
	UInt32			inBufferSize,
	void*			inBuffer,
	UInt32*			outSize) const
{
	return ::GetControlProperty( mHIViewRef, inCreator, inTag,
								 inBufferSize, outSize, inBuffer );
}

#pragma mark -

// ---------------------------------------------------------------------------
//	GetBounds														  [public]
/**
	Gets the bounds of the view
	
	@param	outBounds	View's bounds
	
	The bounds define the local coordinates of the View. When compositing
	is on, the top left is at (0, 0).										*/

void
SysHIView::GetBounds(
	HIRect&		outBounds) const
{
	OSStatus	status = ::HIViewGetBounds(mHIViewRef, &outBounds);
	
	PPx_ThrowIfOSError_(status, "HIViewGetBounds failed");
}


// ---------------------------------------------------------------------------
//	GetFrame														  [public]
/**
	Gets the frame of the view
	
	@param	outFrame	View's frame
	
	The frame is in the coordinate system of the View's parent				*/

void
SysHIView::GetFrame(
	HIRect&		outFrame) const
{
	OSStatus	status = ::HIViewGetFrame(mHIViewRef, &outFrame);
	
	PPx_ThrowIfOSError_(status, "HIViewGetFrame failed");
}


// ---------------------------------------------------------------------------
//	SetFrame														  [public]
/**
	Sets the frame of a view
	
	@param	inFrame		New frame for view
	
	The frame specifies a View's location within its parent					*/

void
SysHIView::SetFrame(
	const HIRect&	inFrame)
{
	OSStatus	status = ::HIViewSetFrame(mHIViewRef, &inFrame);
	
	PPx_ThrowIfOSError_(status, "HIViewSetFrame failed");
}


// ---------------------------------------------------------------------------
//	MoveFrameBy														  [public]
/**
	Move the view's frame the specified distance
	
	@param	inDeltaX	Horizontal offset
	@param	inDeltaY	Vertical offset										*/

void
SysHIView::MoveFrameBy(
	float	inDeltaX,
	float	inDeltaY)
{
	OSStatus	status = ::HIViewMoveBy(mHIViewRef, inDeltaX, inDeltaY);
	
	PPx_ThrowIfOSError_(status, "HIViewMoveBy failed");
}


// ---------------------------------------------------------------------------
//	PlaceFrameAt													  [public]
/**
	Places the view's frame at a particular location in its superview
	
	@param	inLeftX		Left location
	@param	inTopY		Top location										*/

void
SysHIView::PlaceFrameAt(
	float	inLeftX,
	float	inTopY)
{
	OSStatus	status = ::HIViewPlaceInSuperviewAt( mHIViewRef, inLeftX,
																 inTopY );
	
	PPx_ThrowIfOSError_(status, "HIViewPlaceInSuperviewAt failed");
}

#pragma mark -

// ---------------------------------------------------------------------------
//	GetNeedsDisplay
/**
	Returns whether the view or any subviews requires redrawing
	
	@return Whether the view or any subviews requires redrawing				*/

bool
SysHIView::GetNeedsDisplay() const
{
	return ::HIViewGetNeedsDisplay(mHIViewRef);
}


// ---------------------------------------------------------------------------
//	SetNeedsDisplay
/*
	Specifies whether the view needs to be completely redrawn or validated

	@param	inNeedsDisplay		Whether to invalidate or validate the view	*/

void
SysHIView::SetNeedsDisplay(
	bool	inNeedsDisplay)
{
	OSStatus	status = ::HIViewSetNeedsDisplay(mHIViewRef, inNeedsDisplay);
	
	PPx_ThrowIfOSError_(status, "HIViewSetNeedsDisplay failed");
}


// ---------------------------------------------------------------------------
//	SetNeedsDisplayInRegion
/*
	Specifies whether a reqion within a view needs to be redrawn or not
	
	@param	inRegion			Region within the view
	@param	inNeedsDisplay		Whether to validate or invalidate the region
*/

void
SysHIView::SetNeedsDisplayInRegion(
	RgnHandle	inRegion,
	bool		inNeedsDisplay)
{
	OSStatus	status = ::HIViewSetNeedsDisplayInRegion(
								mHIViewRef, inRegion, inNeedsDisplay);
	
	PPx_ThrowIfOSError_(status, "HIViewSetNeedsDisplayInRegion failed");
}


// ---------------------------------------------------------------------------
//	CreateOffscreenImage											  [public]
/**
	Creates an offscreen image for the view
	
	@param	outFrame		Frame of the offsceen image
	@param	outImage		CFImageRef of the offscreen image				*/

void
SysHIView::CreateOffscreenImage(
	HIRect&			outFrame,
	CGImageRef&		outImage)
{
	OSStatus	status = ::HIViewCreateOffscreenImage(
								mHIViewRef, options_None,
								&outFrame, &outImage);
								
	PPx_ThrowIfOSError_(status, "HIViewCreateOffscreenImage failed");
}

#pragma mark -

namespace {	// unnamed 

	// -----------------------------------------------------------------------
	//	Parameter for HIObject initialization event

	const EventParamName		param_ControlFeatures = 'Feat';


	// -----------------------------------------------------------------------
	//	Toolbox callback function for HIObject construction, initialization,
	//	and destruction

	pascal OSStatus
	ObjectLifetimeEventCallback(
		EventHandlerCallRef		inCallRef,
		EventRef				inEventRef,
		void*					inUserData)
	{
		OSStatus	status = eventNotHandledErr;
	
		try {
			UInt32	eventClass = ::GetEventClass(inEventRef);
			UInt32	eventKind  = ::GetEventKind(inEventRef);
		
			if ( (eventClass == kEventClassHIObject)  &&
				 (eventKind  == kEventHIObjectConstruct) ) {
			
					// HIObject construction
					
					// We need to know the HIObjectRef during the
					// initialization, but that event doesn't get passed that
					// information. This event does contain the HIObjectRef,
					// so we extract it and store it in the special output
					// parameter. The system takes this output parameter and
					// passes it as the user data to the initialization event.
			
				HIObjectRef	objectRef;
				SysEventParam::Get( inEventRef,
											kEventParamHIObjectInstance,
											objectRef );
				
				status = ::SetEventParameter( inEventRef,
											  kEventParamHIObjectInstance,
											  typeVoidPtr, sizeof(void*),
											  &objectRef );
										
			} else if ( (eventClass == kEventClassHIObject)  &&
						(eventKind  == kEventHIObjectInitialize) ) {
			
					// HIObject initialization
					
					// The user data is the HIObjectRef that we put in the
					// output parameter during the construct event. Since
					// this callback is only used for HIViews, we know the
					// HIObjectRef is also a ControlRef.
			
				ControlRef	control = (ControlRef) inUserData;
				
					// When calling HIObjectCreate in our CreateSysView
					// function, we passed the control features as
					// a custom parameter in the data event.

				OptionBits	features;
				SysEventParam::Get(inEventRef, param_ControlFeatures,
									features);
									
					// We need the control features from within the
					// control initialization event handler. That event
					// gets passed the ControlRef. We store the features
					// in the refcon of the control so that we can
					// retrieve that data by just knowing the ControlRef.
				
				::SetControlReference(control, features);
				
					// Calling the next handler will eventually
					// call the control initialization event handler
			
				status = ::CallNextEventHandler(inCallRef, inEventRef);
				
			} else if ( (eventClass == kEventClassControl)  &&
						(eventKind  == kEventControlInitialize) ) {
						
					// Control Initialization
					
					// From within the HIObject initialization event handler
					// above, we stored the control features in the refcon for
					// the ControlRef. Get the features, then reset the
					// refcon back to the default value of zero.
				
				ControlRef	control;
				SysEventParam::Get(inEventRef, kEventParamDirectObject,
										control);
				OptionBits	features = ::GetControlReference(control);
				::SetControlReference(control, 0);
				
					// Store features as an output parameter in the event
			
				SysEventParam::Set(inEventRef, kEventParamControlFeatures,
									features);

				status = noErr;
			}
		}
		
		catch (...) { }
		
		return status;
	}
	
	
	// -----------------------------------------------------------------------
	//	Returns UPP to Toolbox callback function for HIObject lifetime events
	
	EventHandlerUPP
	GetObjectLifetimeEventUPP()
	{
										// Local static is constructed once
										//   first time we are called
		static SysEventHandlerUPP
					sObjectLifetimeEventUPP(ObjectLifetimeEventCallback);
		
		return sObjectLifetimeEventUPP.Get();
	}
	
	
} // unnamed namespace

#pragma mark -

// ---------------------------------------------------------------------------
//	RegisterSysViewClass									 [static] {public]
/**
	Registers a class with the system

	Before you can create a HIView, you must register its class name		*/

void
SysHIView::RegisterSysViewClass(
	CFStringRef		inClassID,
	CFStringRef		inBaseClassID)
{
		// The system requires that we handle the construct and destruct
		// events. We need to handle control initialize to specify the
		// features supported by the view. In order to get the data we need
		// passed into the control initialize event callback, we need
		// to handle the HIObject initialize event.

	EventTypeSpec	eventTypes[] = {
						{kEventClassHIObject, kEventHIObjectConstruct},
						{kEventClassHIObject, kEventHIObjectInitialize},
						{kEventClassHIObject, kEventHIObjectDestruct},
						{kEventClassControl , kEventControlInitialize} };

	OSStatus	status = ::HIObjectRegisterSubclass(
								inClassID, inBaseClassID, options_None,
								GetObjectLifetimeEventUPP(),
								GetEventTypeCount(eventTypes), eventTypes,
								nil, nil);
	
	PPx_ThrowIfOSError_(status, "HIObjectRegisterSubclass failed");
}


// ---------------------------------------------------------------------------
//	CreateSysView											 [static] {public]
/**
	Create a new HIView
	
	@param	inClassID	Class ID for the HIView
	@param	inFeatures	Control features supported by the HIView

	You must call RegisterSysViewClass() for the class ID before calling
	this function. inFeatures are the control featurees supported by
	the view. See <Controls.h> for a list of features.						*/

HIViewRef
SysHIView::CreateSysView(
	CFStringRef		inClassID,
	OptionBits		inFeatures)
{
		// We need to know the features when the Toolbox sends the
		// newly created HIView an initialize event. So, we store
		// the features as a parameter in the data event passed to
		// HIObjectCreate. The system will pass this event to our
		// callback function when it sends the initialize event.

	SysCarbonEvent	dataEvent(kEventClassHIObject, kEventHIObjectInitialize);
	
	SysEventParam::Set(dataEvent, param_ControlFeatures, inFeatures);
	
	HIObjectRef	objectRef;
	OSStatus	status = ::HIObjectCreate(inClassID, dataEvent, &objectRef);
	
	PPx_ThrowIfOSError_(status, "HIObjectCreate failed");
	
	return (HIViewRef) objectRef;
}


} // namespace PPx
