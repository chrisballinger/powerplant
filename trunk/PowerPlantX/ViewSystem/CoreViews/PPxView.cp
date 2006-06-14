// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxView.cp
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:38 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxView.h>
#include <PPxEventUtils.h>
#include <PPxFrameAdapter.h>
#include <PPxSerializer.h>
#include <PPxSignature.h>
#include <SysWindow.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	External key names for commonly used persistent data

const CFStringRef	key_InitValue		= CFSTR("initial value");
const CFStringRef	key_MinValue		= CFSTR("minimum value");
const CFStringRef	key_MaxValue		= CFSTR("maximum value");

const CFStringRef	key_ViewTitle		= CFSTR("view title");
const CFStringRef	key_AutoToggle		= CFSTR("auto toggle");
const CFStringRef	key_IsPrimaryGroup	= CFSTR("is primary group");

const CFStringRef	key_TextContent		= CFSTR("text content");
const CFStringRef	key_TextJustification
										= CFSTR("text justification");
										
const CFStringRef	key_ContentType		= CFSTR("content type");
const CFStringRef	key_ContentResID	= CFSTR("content resource id");


namespace {	// unnamed

	// -----------------------------------------------------------------------
	//	Key names for persistent data local to View

	const CFStringRef	key_ViewID			= CFSTR("view id");
	const CFStringRef	key_SuperView		= CFSTR("superview");
	const CFStringRef	key_FrameAdapter	= CFSTR("frame adapter");
	const CFStringRef	key_ViewFrame		= CFSTR("view frame");
	const CFStringRef	key_IsVisible		= CFSTR("is visible");
	const CFStringRef	key_IsEnabled		= CFSTR("is enabled");

	//	Property tag for the pointer to a PPx::View object
	const OSType		property_PPxView	= 'view';
}


// ---------------------------------------------------------------------------
//	View														   [protected]
/**
	Default constructor */

View::View()
{
	mSuperView		= nil;
	mSubViews		= nil;
}


// ---------------------------------------------------------------------------
//	~View															  [public]
/**
	Destructor */

View::~View()
{
								// Send notification of our impending demise
	if (GetSysView() != nil) {
		EventUtils::SendTargetDeletedEvent(GetSysEventTarget());
	}

	mFrameAdapter.reset();

	if (mSubViews != nil) {
	
			// Delete objects from back to front of SubView list.
			// Deleting SubView will cause it to be removed from
			// our mSubViews list, reducing its size by one each
			// time through the loop.

		while (mSubViews->size() > 0) {
			View*	subView = mSubViews->back();
			delete subView;
		}
		
		delete mSubViews;
		mSubViews = nil;
	}
	
	RemoveFromSuperView();
}


// ---------------------------------------------------------------------------
//	Initialize													   [protected]
/**
	Initializes from parameters
	
	@param	inViewRef	System HIViewRef for this view
	@param	inFrame		Bounds for view, in local coordinates of parent
	
	Creates View with no superview											*/

void
View::Initialize(
	HIViewRef		inViewRef,
	const HIRect&	inFrame)
{
	mSysHIView.Adopt(inViewRef);
	::CFRelease(inViewRef);
	
	View*	thisView = this;			// Store pointer to this View object
										//   as a property so we can get it
										//   from the HIViewRef
	mSysHIView.SetProperty( Signature::Get(), property_PPxView,
							sizeof(thisView), &thisView );
	
	SetFrame(inFrame);
}


// ---------------------------------------------------------------------------
//	Initialize													   [protected]
/**
	Initializes from parameters
	
	@param	inViewRef		System HIViewRef for this view
	@param	inSuperView		Parent view
	@param	inFrame			Bounds for view, in local coordinates of parent
	@param	inVisible		Whether the view is visible
	@param	inEnabled		Whether the view is enabled						*/

void
View::Initialize(
	HIViewRef		inViewRef,
	View*			inSuperView,
	const HIRect&	inFrame,
	bool			inVisible,
	bool			inEnabled)
{
	mSysHIView.Adopt(inViewRef);
	::CFRelease(inViewRef);
	
	View*	thisView = this;			// Store pointer to this View object
										//   as a property so we can get it
										//   from the HIViewRef
	mSysHIView.SetProperty( Signature::Get(), property_PPxView,
							sizeof(thisView), &thisView );
	
	SetFrame(inFrame);
	
	mSysHIView.SetEnabled(inEnabled);	// Enable first in case making
	mSysHIView.SetVisible(inVisible);	//   visible draws
	
	if (inSuperView != nil) {
		inSuperView->AddSubView(this);
	}
}

#pragma mark -

// ---------------------------------------------------------------------------
//	GetSysView														  [public]
/**
	Returns the system HIViewRef for the View
	
	@return System HIViewRef for the View */

HIViewRef
View::GetSysView() const
{
	return mSysHIView.GetSysView();
}


// ---------------------------------------------------------------------------
//	AdoptSysView												   [protected]
/**
	Uses the input HIViewRef
	
	@param	inViewRef	System HIViewRef to adopt							*/

void
View::AdoptSysView(
	HIViewRef	inViewRef)
{
	PPx_BadParamIfNil_(inViewRef);

	mSysHIView.Adopt(inViewRef);
}


// ---------------------------------------------------------------------------
//	SysEventTarget													 [private]
/**
	Returns the event target for the View
	
	@return Event target for the View										*/

EventTargetRef
View::SysEventTarget() const
{
	return mSysHIView.GetSysEventTarget();
}

#pragma mark -


// ---------------------------------------------------------------------------
//	ReadViewState												   [protected]
/**
	Reads persistent data for a View
	
	@param	inReader		Data dictionary from which to read
	@param	outSuperView	Parent view
	@param	outFrame		Size and location of View within SuperView
	@param	outVisible		Whether the View should be visible
	@param	outEnabled		Whether the View should be enabled
	
	You should not normally need to call this function. It's provided for
	subclasses which need to know their view state in order to properly
	initialize themselves.													*/

void
View::ReadViewState(
	const DataReader&	inReader,
	View*&				outSuperView,
	HIRect&				outFrame,
	bool&				outVisible,
	bool&				outEnabled)
{
	inReader.ReadObjectValue(key_SuperView, outSuperView);
	
	inReader.ReadOptional(key_ViewFrame, outFrame);
	inReader.ReadOptional(key_IsVisible, outVisible);
	inReader.ReadOptional(key_IsEnabled, outEnabled);
}


// ---------------------------------------------------------------------------
//	InitViewState												   [protected]
/**
	Initializes view from persistent data
	
	@param	inViewRef	System HIViewRef for this View
	@param	inReader	Data dictionary from which to read persistent data

	View does not use an override of Peristent::InitState() because it
	requires a valid HIViewRef before it can initialize itself.

	A View subclass should call this function from its override of InitState()
	after creating its HIViewRef.											*/

void
View::InitViewState(
	HIViewRef			inViewRef,
	const DataReader&	inReader)
{
	ObjectIDT	theID = objectID_None;
	inReader.ReadOptional(key_ViewID, theID);
	SetID(theID);
	
	View*		superView	= nil;
	HIRect		frame		= ::CGRectZero;
	bool		visible		= false;
	bool		enabled		= true;
	
	ReadViewState(inReader, superView, frame, visible, enabled);
	
	FrameAdapter*	adapter;
	inReader.ReadObjectValue(key_FrameAdapter, adapter);
	mFrameAdapter.reset(adapter);
	
	Initialize(inViewRef, frame);
	
	mSysHIView.SetEnabled(enabled);		// Enable first in case making
	mSysHIView.SetVisible(visible);		//   visible draws
	
	if (superView != nil) {
		superView->AddSubView(this);
	}
	
	ReadAttachments(inReader);
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
View::WriteState(
	DataWriter&	ioWriter) const
{
	ObjectIDT	theID = GetID();
	if (theID != objectID_None) {
		ioWriter.WriteValue(key_ViewID, GetID());
	}

	WriteAttachments(ioWriter);
	
	ioWriter.WriteObjectValue(key_FrameAdapter, mFrameAdapter.get());
	
	WriteViewHierarchy(ioWriter);

	HIRect	frame;
	mSysHIView.GetFrame(frame);
	ioWriter.WriteValue(key_ViewFrame, frame);
	
	ioWriter.WriteValue(key_IsVisible, mSysHIView.IsVisible());
	ioWriter.WriteValue(key_IsEnabled, mSysHIView.IsEnabled());
}


// ---------------------------------------------------------------------------
//	WriteViewHierarchy											   [protected]
/**
	Write subviews to a data dictionary
	
	@param	ioWriter	Data dictionary to which to write persistent data
	
	@note You should not call this function. Call WriteState instead.		*/

void
View::WriteViewHierarchy(
	DataWriter&	ioWriter) const
{
	ioWriter.WriteObjectValue(key_SuperView, mSuperView);
	
	if (mSubViews != nil) {
	
		for (std::vector<View*>::const_iterator iter(mSubViews->begin()),
												last(mSubViews->end());
			 iter != last; ++iter) {

			ioWriter.WriteObject(*iter);
		}
	}
}

#pragma mark -

// ---------------------------------------------------------------------------
//	AddSubView														  [public]
/**
	Adds a view as a subview of this view */

void
View::AddSubView(
	View*	inSubView)
{
	if (inSubView->GetSuperView() != nil) {
		inSubView->RemoveFromSuperView();
	}

	if (mSubViews == nil) {					// This is the first subview
		mSubViews = new std::vector<View*>;
		
			// Install handler for bounds changed so we can notify
			// subviews when our frame size changes
			
		ControlBoundsChangedDoer::Install(GetSysEventTarget());
	}

	mSysHIView.AddSubView(inSubView->GetSysView());
	
	mSubViews->push_back(inSubView);
	inSubView->SetSuperView(this);
}


// ---------------------------------------------------------------------------
//	RemoveFromSuperView												  [public]
/**
	Removes view from its superview */

void
View::RemoveFromSuperView()
{
	if (mSuperView != nil) {
		mSysHIView.RemoveFromSuperView();
		mSuperView->RemoveSubView(this);
		SetSuperView(nil);
	}
}


// ---------------------------------------------------------------------------
//	RemoveSubView													 [private]
/**
	Removes a subview from a View
	
	@param	inSubView	SubView to remove									*/

void
View::RemoveSubView(
	View*	inSubView)
{
											// STL remove/erase idiom
	mSubViews->erase( remove(mSubViews->begin(), mSubViews->end(), inSubView),
					  mSubViews->end() );
}


// ---------------------------------------------------------------------------
//	GetSuperView													  [public]
/**
	Returns the SuperView of the View
	
	@return SuperView of the View											*/

View*
View::GetSuperView() const
{
	return mSuperView;
}


// ---------------------------------------------------------------------------
//	SetSuperView													 [private]
/**
	Sets the superview of the View
	
	@param	inSuperView		SuperView for the View							*/

void
View::SetSuperView(
	View*	inSuperView)
{
	mSuperView = inSuperView;
}


// ---------------------------------------------------------------------------
//	GetSysWindow													  [public]
/**
	Returns the system WindowRef for the Window containing the View
	
	@return	System WindowRef containing the View							*/

WindowRef
View::GetSysWindow() const
{
	return ::GetControlOwner(GetSysView());
}


// ---------------------------------------------------------------------------
//	SubViewCount													  [public]
/**
	Returns the number of subviews of the View								*/

SInt32
View::SubViewCount() const
{
	return (mSubViews == nil) ? 0 : mSubViews->size();
}


// ---------------------------------------------------------------------------
//	GetSubViewByIndex												  [public]
/**
	Returns subview specified by a zero-based index
	
	@param	inIndex		Zero-based index
	
	@return	SubView at the specified index in subview list of View

	Returns nil if index is out of range									*/

View*
View::GetSubViewByIndex(
	SInt32	inIndex) const
{
	View*	subView = nil;
	
	if ( (mSubViews != nil)  &&
		 (inIndex >= 0)  &&
		 (inIndex < mSubViews->size()) ) {
	
		subView = (*mSubViews)[inIndex];
	}
	
	return subView;
}


// ---------------------------------------------------------------------------
//	FindViewByID													  [public]
/**
	Returns the View with the specified Object ID.
	
	@param	inID	Object ID of View to find
	
	@return	View with the specified object ID
	
	Searches the entire hierarchy rooted at the View. View returned
	is the View itself, a descendent View, or nil if not found.				*/

View*
View::FindViewByID(
	ObjectIDT	inID)
{
		// We call the const function to get the View pointer, but cast
		// away the constness. This is safe since this function is not
		// const and it's whole purpose is to return a pointer to a View
		// that can be changed.

	return const_cast<View*>(FindConstViewByID(inID));
}


// ---------------------------------------------------------------------------
//	FindConstViewByID												  [public]
/**
	Returns the View with the specified Object ID. View returned is the
	View itself or one of its subviews.
	
	@param	inID	Object ID of View to find
	
	@return	Const View with the specified object ID
	
	Searches the entire hierarchy rooted at the View. View returned
	is the View itself, a descendent View, or nil if not found.				*/

const View*
View::FindConstViewByID(
	ObjectIDT	inID) const
{
	const View*	theView = nil;
	
	if (inID == GetID()) {
		theView = this;
		
	} else if (mSubViews != nil) {
	
		for (std::vector<View*>::const_iterator	iter(mSubViews->begin()),
												last(mSubViews->end());
			 iter != last; ++iter) {
			 
			theView = (*iter)->FindConstViewByID(inID);
			if (theView != nil) break;
		}
	}
	
	return theView;
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SetFrame														  [public]
/**
	Sets the frame of the View.
	
	@param	inFrame		New frame for View, in coordinates of parent View	*/

void
View::SetFrame(
	const HIRect&	inFrame)
{
	mSysHIView.SetFrame(inFrame);
}


// ---------------------------------------------------------------------------
//	GetFrame														  [public]
/**
	Passes back the View's frame
	
	@param	outFrame	View's frame in coordinates of parent View			*/

void
View::GetFrame(
	HIRect&		outFrame) const
{
	mSysHIView.GetFrame(outFrame);
}


// ---------------------------------------------------------------------------
//	GetLocalFrame													  [public]
/**
	Passes back the View's frame in local coordinates
	
	@param	outFrame	View's frame in local coordinates
	
	Local coordinates for a View. When compositing is on, the top left
	is at (0, 0).															*/

void
View::GetLocalFrame(
	HIRect&		outFrame) const
{
	mSysHIView.GetBounds(outFrame);
}


// ---------------------------------------------------------------------------
//	SetFrameAdapter													  [public]
/**
	Sets the FrameAdapter object for the View
	
	@param	inAdapter	FrameAdapter object
	
	A FrameAdapter controls how a view moves and/or resizes when its
	superview changes size. View takes ownership of the FrameAdapter and
	is responsible for deleting it.											*/

void
View::SetFrameAdapter(
	FrameAdapter*	inAdapter)
{
	mFrameAdapter.reset(inAdapter);
}


// ---------------------------------------------------------------------------
//	DoControlBoundsChanged										   [protected]
/**
	Handles event for the View's bounds being changed
	
	@param	ioEvent				CarbonEvent for control bounds changed
	@param	inControl			ControlRef for View
	@param	inChangeAttributes	Indicates if size and/or location changed
	@param	inOriginalBounds	Bounds before change
	@param	inCurrentBounds		Bounds after change
	
	Informs subviews of the bounds change so that they can adjust their
	size and/or location if necessary										*/
	
OSStatus
View::DoControlBoundsChanged(
	SysCarbonEvent&	/* ioEvent */,
	ControlRef		/* inControl */,
	UInt32			inChangeAttributes,
	const HIRect&	inOriginalBounds,
	const HIRect&	inCurrentBounds)
{
		// If the size (rather than just the location) of the bounds
		// have changed, tell subviews to adapt to the change in
		// size of their superview

	if ( (inChangeAttributes & kControlBoundsChangeSizeChanged)  &&
		 (mSubViews != nil) ) {
		 
		for (std::vector<View*>::iterator iter(mSubViews->begin()),
										  last(mSubViews->end());
			 iter != last; ++iter) {

			(*iter)->AdaptToSuperFrameSize(inOriginalBounds, inCurrentBounds);
		}
	}
	
	return noErr;
}


// ---------------------------------------------------------------------------
//	AdaptToSuperFrameSize										   [protected]
/**
	Adjusts the size and/or location of the View's frame in response to a
	change in the size of the superview's frame
	
	@param	inOldSuperFrame		SuperView frame before change
	@param	inNewSuperFrame		SuperView frame after change
	
	Uses its FrameAdapter object to determine how to adjust its frame		*/

void
View::AdaptToSuperFrameSize(
	const HIRect&	inOldSuperFrame,
	const HIRect&	inNewSuperFrame)
{
	if (mFrameAdapter.get() != nil) {
	
		HIRect	frame;
		GetFrame(frame);
	
		mFrameAdapter->AdaptFrame(inOldSuperFrame, inNewSuperFrame, frame);
		
		SetFrame(frame);
	}
}

#pragma mark -

// ---------------------------------------------------------------------------
//	GetNeedsDisplay													  [public]
/**
	Returns whether the View or any subviews requires redrawing
	
	@return Whether the View or any subviews requires redrawing				*/

bool
View::GetNeedsDisplay() const
{
	return mSysHIView.GetNeedsDisplay();
}


// ---------------------------------------------------------------------------
//	SetNeedsDisplay													  [public]
/*
	Specifies whether the View needs to be completely redrawn or validated

	@param	inNeedsDisplay		Whether to invalidate or validate the View	*/

void
View::SetNeedsDisplay(
	bool	inNeedsDisplay)
{
	mSysHIView.SetNeedsDisplay(inNeedsDisplay);
}


// ---------------------------------------------------------------------------
//	SetNeedsDisplayInRegion											  [public]
/*
	Specifies whether a reqion within a View needs to be redrawn or not
	
	@param	inRegion			Region within the View
	@param	inNeedsDisplay		Whether to validate or invalidate the region
*/

void
View::SetNeedsDisplayInRegion(
	RgnHandle	inRegion,
	bool		inNeedsDisplay)
{
	mSysHIView.SetNeedsDisplayInRegion(inRegion, inNeedsDisplay);
}


// ---------------------------------------------------------------------------
//	GetOffscreenImage												  [public]
/**
	Returns an offscreen image for the View
	
	@param	outFrame		Frame of the offsceen image

	@return CGImage of the offscreen image									*/

CGImage
View::GetOffscreenImage(
	HIRect&	outFrame)
{
	CGImageRef	ref;
	
	mSysHIView.CreateOffscreenImage(outFrame, ref);
	
	return CGImage(ref, retain_No);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	IsVisible														  [public]
/**
	Returns whether the View is visible
	
	@return Whether the View is visible										*/

bool
View::IsVisible() const
{
	return mSysHIView.IsVisible();
}


// ---------------------------------------------------------------------------
//	SetVisible														  [public]
/**
	Makes the View visible or invisible
	
	@param	inMakeVisible	Whether to show or hide the View				*/

void
View::SetVisible(
	bool	inMakeVisible)
{
	mSysHIView.SetVisible(inMakeVisible);
}


// ---------------------------------------------------------------------------
//	IsActive														  [public]
/**
	Returns whether the View is active
	
	@return	Whether the View is active										*/

bool
View::IsActive() const
{
	return mSysHIView.IsActive();
}


// ---------------------------------------------------------------------------
//	SetActive														  [public]
/**
	Makes the View active or inactive
	
	@param	inActivate	Whether to activate or deactivate the View			*/

void
View::SetActive(
	bool	inActivate)
{
	mSysHIView.SetActive(inActivate);
}


// ---------------------------------------------------------------------------
//	IsEnabled														  [public]
/**
	Returns whether the View is enabled
	
	@return	Whether the View is enabled										*/

bool
View::IsEnabled() const
{
	return mSysHIView.IsEnabled();
}


// ---------------------------------------------------------------------------
//	SetEnabled														  [public]
/**
	Makes the View enabled or disabled
	
	@param	inEnable	Whether to enable or diable the View				*/

void
View::SetEnabled(
	bool	inEnable)
{
	mSysHIView.SetEnabled(inEnable);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SetValue														  [public]
/**
	Sets the View's value
	
	@param	inValue		New value for the View								*/

void
View::SetValue(
	SInt32	inValue)
{
	mSysHIView.SetValue(inValue);
}


// ---------------------------------------------------------------------------
//	GetValue														  [public]
/**
	Returns the View's value
	
	@return View's value													*/

SInt32
View::GetValue() const
{
	return mSysHIView.GetValue();
}


// ---------------------------------------------------------------------------
//	SetMinValue														  [public]
/**
	Sets the minimum value for the View
	
	@param	inMinValue		New minimum value for the View					*/

void
View::SetMinValue(
	SInt32	inMinValue)
{
	mSysHIView.SetMinValue(inMinValue);
}


// ---------------------------------------------------------------------------
//	GetMinValue														  [public]
/**
	Gets the minimum value for the View
	
	@return Minimum value for the View										*/

SInt32
View::GetMinValue() const
{
	return mSysHIView.GetMinValue();
}


// ---------------------------------------------------------------------------
//	SetMaxValue														  [public]
/**
	Sets the maximum value for the View
	
	@param	inMaxValue		New maximum value for the View					*/

void
View::SetMaxValue(
	SInt32	inMaxValue)
{
	mSysHIView.SetMaxValue(inMaxValue);
}


// ---------------------------------------------------------------------------
//	GetMaxValue														  [public]
/**
	Gets the maximum value for the View
	
	@return Maximum value for the View										*/

SInt32
View::GetMaxValue() const
{
	return mSysHIView.GetMaxValue();
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SetTitle														  [public]
/**
	Sets the title of the View
	
	@param	inTitle		New title for the View								*/

void
View::SetTitle(
	CFStringRef	inTitle)
{
	mSysHIView.SetTitle(inTitle);
}


// ---------------------------------------------------------------------------
//	GetTitle														  [public]
/**
	Gets the title of the View
	
	@return Title of the View												*/

CFString
View::GetTitle() const
{
	return mSysHIView.GetTitle();
}


// ---------------------------------------------------------------------------
//	SetDataTag														  [public]
/**
	Sets a View property specified by a data tag
	
	@param	inPartCode	Part of View to which the property applies
	@param	inTag		Data tag ID
	@param	inDataSize	Byte size of data
	@param	inDataPtr	Pointer to data buffer								*/

void
View::SetDataTag(
	SInt16				inPartCode,
	FourCharCode		inTag,
	Size				inDataSize,
	const void*			inDataPtr)
{
	mSysHIView.SetDataTag(inPartCode, inTag, inDataSize, inDataPtr);
}


// ---------------------------------------------------------------------------
//	GetDataTag														  [public]
/**
	Gets a View property specified by a data tag
	
	@param	inPartCode		Part of View to which the property applies
	@param	inTag			Data tag ID
	@param	inBufferSize	Byte size of buffer
	@param	inBuffer		Pointer to data buffer
	@param	outDataSize		Actual size of data returned. Pass nil if you
								don't want this information					*/

OSStatus
View::GetDataTag(
	SInt16			inPartCode,
	FourCharCode	inTag,
	Size			inBufferSize,
	void*			inBuffer,
	Size*			outDataSize) const
{
	return mSysHIView.GetDataTag( inPartCode, inTag, inBufferSize,
								   inBuffer, outDataSize );
}

#pragma mark -

// ---------------------------------------------------------------------------
//	GetViewObject											 [public] {static]
// ---------------------------------------------------------------------------
/**
	Returns the View object associated with a HIViewRef
	
	@param	inViewRef	System HIViewRef
	
	@return	View object associated with the HIViewRef
	
	Returns nil if the HIViewRef does not belong to a View created by the
	PPx::View class of the current program.
	
	PPx adds a property to each HIViewRef with a pointer to the associated
	View object, and tags that property with the signature (four-character
	creator code) of the program.
	
	Therefore, a host program and plug-in modules or other external code
	call all use PPx and their Views won't get confused as long as they
	have different signatures.												*/
	
View*
View::GetViewObject(
	HIViewRef	inViewRef)
{
	View*	theView = nil;
	
	if (inViewRef != nil) {
		::GetControlProperty( inViewRef, Signature::Get(), property_PPxView,
							  sizeof(theView), nil, &theView );
	}
	
	return theView;
}


} // namespace PPx
