// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxWindow.cp
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:48 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxWindow.h>
#include <PPxEventUtils.h>
#include <PPxSerializer.h>
#include <PPxSignature.h>
#include <PPxWindowContentView.h>

namespace PPx {

namespace {	// unnamed

	// -----------------------------------------------------------------------
	//	Key names for persistent data

	const CFStringRef	key_WindowClass		= CFSTR("window class");
	const CFStringRef	key_WindowAttrs		= CFSTR("window attributes");
	const CFStringRef	key_WindowTitle		= CFSTR("window title");
	const CFStringRef	key_ContentBounds	= CFSTR("content bounds");
	const CFStringRef	key_ContentView		= CFSTR("content view");
	
										// Property tag for the pointer to
										//	a PPx::Window object
	const OSType		property_PPxWindow	= 'wind';
	
	
	// -----------------------------------------------------------------------
	//	Default window attributes used for all Toolbox windows
	
		// PPx views are based on HIView, and work best using compositing.
		// If you turn off compositing, you may need to handle your own
		// clipping when drawing.
		
		// PPx windows rely on the standard handler to process most Carbon
		// events. If you don't use the standard handler, you will need to
		// install your own event handlers.
	
	WindowAttributes	sDefaultAttributes = kWindowCompositingAttribute +
											 kWindowStandardHandlerAttribute;
} // unnamed namespace


// ---------------------------------------------------------------------------
//	Window															  [public]
/**
	Default constructor */

Window::Window()
{
}


// ---------------------------------------------------------------------------
//	~Window															  [public]
/**
	Destructor */

Window::~Window()
{
								// Send notification of our impending demise
	EventUtils::SendTargetDeletedEvent(GetSysEventTarget());
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initializes from parameters
	
	@param	inWindClass			Toolbox window class
	@param	inWindAttrs			Toolbox window attributes
	@param	inContentBounds		Bounding box of content area of window in
									global coordinates
	@param	inTitle				Title for the window
	
	See <MacWindow.h> for information about Toolbox window classes and
	attributes.
	
	The default attributes for PPx::Window are automatically added.
	Call PPx::Window::SetDefaultAttributes to change the default attributes
	which apply to all PPx Windows.											*/
	
void
Window::Initialize(
	WindowClass			inWindClass,
	WindowAttributes	inWindAttrs,
	const Rect&			inContentBounds,
	CFStringRef			inTitle)
{
	inWindAttrs |= sDefaultAttributes;	// Add default attributes
	
										// Only use available attributes
	inWindAttrs &= ::GetAvailableWindowAttributes(inWindClass);

	mSysWindow.MakeWindow(inWindClass, inWindAttrs, inContentBounds);
	
	Window*		thisWindow = this;		// Store pointer to this Window object
										//   as a property so we can get it
										//   from the WindowRef
	mSysWindow.SetProperty( Signature::Get(), property_PPxWindow,
							sizeof(thisWindow), &thisWindow );
	
	mSysWindow.SetTitle(inTitle);
	
		// Toolbox automatically creates a HIView for the content view.
		// We need to create our own content view object, if it hasn't
		// already been created from persistent data, and initialize it.
		
	if (mContentView.get() == nil) {
		mContentView.reset( new WindowContentView );
	}
	mContentView->Initialize(mSysWindow.GetWindowRef());
	
		// Install our default close handler, which just deletes
		// the Window object. Install your own close handler (which
		// will get called before this default one) to implement
		// custom behavior.
	
	WindowCloseDoer::Install(GetSysEventTarget());
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
Window::ClassName() const
{
	return CFSTR("PPx::Window");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
Window::InitState(
	const DataReader&	inReader)
{
	ReadAttachments(inReader);

	WindowClass	windClass;
	inReader.ReadRequired(key_WindowClass, windClass);
	
	WindowAttributes	windAttrs = kWindowNoAttributes;
	Rect				bounds	  = { 0, 0, 0, 0};
	CFString			title;
	
	inReader.ReadOptional(key_WindowAttrs, windAttrs);
	inReader.ReadOptional(key_ContentBounds, bounds);
	inReader.ReadOptional(key_WindowTitle, title);
	
	WindowContentView*	content;
	inReader.ReadObjectValue(key_ContentView, content);
	mContentView.reset(content);
	
	Initialize(windClass, windAttrs, bounds, title);
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
Window::WriteState(
	DataWriter&	ioWriter) const
{
	WriteAttachments(ioWriter);
	
	ioWriter.WriteValue(key_WindowClass, mSysWindow.GetWindowClass());
	ioWriter.WriteValue(key_WindowAttrs, mSysWindow.GetWindowAttributes());
	ioWriter.WriteValue(key_ContentBounds, mSysWindow.GetBounds());
	
	CFString	title = mSysWindow.GetTitle();
	ioWriter.WriteValue(key_WindowTitle, title);
	
	ioWriter.WriteObjectValue(key_ContentView, mContentView.get());
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SysEventTarget													 [private]
/**
	Returns the event target for the Window
	
	@return Event target for the Window										*/

EventTargetRef
Window::SysEventTarget() const
{
	return ::GetWindowEventTarget(mSysWindow.GetWindowRef());
}


// ---------------------------------------------------------------------------
//	GetSysWindow													  [public]
/**
	Returns the System WindowRef associated with the Window
	
	@return System WindowRef associated with the Window						*/

WindowRef
Window::GetSysWindow() const
{
	return mSysWindow.GetWindowRef();
}


// ---------------------------------------------------------------------------
//	GetContentView													  [public]
/**
	Returns the content view of the Window
	
	@return Content view of the Window
	
	A Window is not a View. The content view is the top-level View within
	a Window. Add subviews to the content view if you want to have them
	displayed in a Window.													*/

View*
Window::GetContentView() const
{
	return mContentView.get();
}


// ---------------------------------------------------------------------------
//	AddSubView														  [public]
/**
	Adds a subview to the content view of a Window
	
	@param inSubView	SubView to add										*/

void
Window::AddSubView(
	View*	inSubView)
{
	mContentView->AddSubView(inSubView);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	Select															  [public]
/**
	Brings the Window to the front of its layer and activates it			*/

void
Window::Select()
{
	mSysWindow.Select();
}


// ---------------------------------------------------------------------------
//	IsVisible														  [public]
/**
	Returns whether the Window is visible
	
	@return Whether the Window is visible									*/

bool
Window::IsVisible() const
{
	return mSysWindow.IsVisible();
}


// ---------------------------------------------------------------------------
//	Show															  [public]
/**
	Makes the Window visible												*/

void
Window::Show()
{
	mSysWindow.Show();
}


// ---------------------------------------------------------------------------
//	Hide															  [public]
/**
	Makes the Window invisible												*/

void
Window::Hide()
{
	mSysWindow.Hide();
}


// ---------------------------------------------------------------------------
//	SetTitle														  [public]
/**
	Sets the title of the Window
	
	@param	inTitle		New title for the Window							*/

void
Window::SetTitle(
	CFStringRef	inTitle)
{
	mSysWindow.SetTitle(inTitle);
}


// ---------------------------------------------------------------------------
//	GetTitle														  [public]
/**
	Returns the title of the Window
	
	@return Title fo the Window												*/

CFString
Window::GetTitle() const
{
	return mSysWindow.GetTitle();
}

#pragma mark -

// ---------------------------------------------------------------------------
//	Close															  [public]
/**
	Closes the window
	
	Posts a "close" CarbonEvent for the Window								*/

void
Window::Close()
{
	SysCarbonEvent	closeEvent(kEventClassWindow, kEventWindowClose);
	
	closeEvent.PostTo(GetSysEventTarget());
}


// ---------------------------------------------------------------------------
//	DoWindowClose													  [public]
/**
	Handles window close event
	
	@param	ioEvent		CarbonEvent for window close
	@param	inWindow	Window to close
	
	@return	Status of event handling. Alwyas returns noErr.					*/

OSStatus
Window::DoWindowClose(
	SysCarbonEvent&	/* ioEvent */,
	WindowRef		/* inWindow */)
{
	delete this;
	
	return noErr;
}

#pragma mark -

// ---------------------------------------------------------------------------
//	GetWindowObject											 [public] {static]
/**
	Returns the Window object associated with a WindowRef
	
	@param	inWindowRef		System window reference
	
	@return	PPx Window object
	
	Returns nil if the WindowRef does not belong to a Window created by
	PPx::Window class of the current program.
	
	PPx adds a property to each WindowRef with a pointer to associated
	Window object, and tags that property with the signature (four-character
	creator code) of the program.
	
	Therefore, a host program and plug-in modules or other external code
	call all use PPx and their Windows won't get confused as long as they
	have different signatures.												*/

Window*
Window::GetWindowObject(
	WindowRef	inWindowRef)
{
	Window*		theWindow = nil;
	
	if (inWindowRef != nil) {
		::GetWindowProperty( inWindowRef, Signature::Get(), property_PPxWindow,
							 sizeof(theWindow), nil, &theWindow );
	}
	
	return theWindow;
}


// ---------------------------------------------------------------------------
//	SetDefaultAttributes									 [public] {static]
/**
	Sets the window attributes used for all Toolbox windows
	
	@param	inWindAttrs		Toolbox window attributes bit flags				*/

void
Window::SetDefaultAttributes(
	WindowAttributes	inWindAttrs)
{
	sDefaultAttributes = inWindAttrs;
}


// ---------------------------------------------------------------------------
//	GetDefaultAttributes									 [public] {static]
/**
	Returns the window attributes used for all Toolbox window
	
	@return	Toolbox window attributes bit flags								*/

WindowAttributes
Window::GetDefaultAttributes()
{
	return sDefaultAttributes;
}


} // namespace PPx
