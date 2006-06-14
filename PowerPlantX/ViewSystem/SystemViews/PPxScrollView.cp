// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxScrollView.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:44 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxScrollView.h>
#include <PPxSerializer.h>
#include <SysCreateView.h>

namespace PPx {

namespace {		// unnamed namespace 

	// -----------------------------------------------------------------------
	//	Key names for persistent data

	const CFStringRef	key_ScrollViewOptions	= CFSTR("scroll options");
	const CFStringRef	key_AutoHideScrollbars	= CFSTR("autohide scrollbars");
}


// ---------------------------------------------------------------------------
//	ScrollView													  [public]
/**
	Default constructor */

ScrollView::ScrollView()
{
	mScrollOptions = kHIScrollViewValidOptions;
}


// ---------------------------------------------------------------------------
//	~ScrollView													  [public]
/**
	Destructor */

ScrollView::~ScrollView()
{
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initialize from screoll view creation parameters
	
	@param	inSuperView		Parent view
	@param	inFrame			Bounds for view, in local coordinates of parent
	@param	inVisible		Whether the view is visible
	@param	inEnabled		Whether the view is enabled
	@param	inOptions		Options for scroll view							*/

void
ScrollView::Initialize(
	View*			inSuperView,
	const HIRect&	inFrame,
	bool			inVisible,
	bool			inEnabled,
	OptionBits		inOptions)
{
	mScrollOptions = inOptions;

	HIViewRef	viewRef = SysCreateView::ScrollView(inOptions);
	
	View::Initialize(viewRef, inSuperView, inFrame, inVisible, inEnabled);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
ScrollView::ClassName() const
{
	return CFSTR("PPx::ScrollView");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
ScrollView::InitState(
	const DataReader&	inReader)
{
	bool		autoHide = true;			// Data for Scroll View
	
	inReader.ReadOptional(key_ScrollViewOptions, mScrollOptions);
	inReader.ReadOptional(key_AutoHideScrollbars, autoHide);
	
	HIViewRef	viewRef = SysCreateView::ScrollView(mScrollOptions);
	
	View::InitViewState(viewRef, inReader);	// Data for all Views
	
	SetAutoHideScrollBars(autoHide);
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
ScrollView::WriteState(
	DataWriter&	ioWriter) const
{
											// Data for Scroll View
	ioWriter.WriteValue(key_ScrollViewOptions, mScrollOptions);
	ioWriter.WriteValue(key_AutoHideScrollbars, GetAutoHideScrollBars());
	
	View::WriteState(ioWriter);				// Data for all Views
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SetAutoHideScrollBars											  [public]
/**
	Sets whether the automatically hide the scroll bars
	
	@param	inAutoHide	Whether the automatically hide the scroll bars		*/

void
ScrollView::SetAutoHideScrollBars(
	bool	inAutoHide)
{
	OSStatus	status = ::HIScrollViewSetScrollBarAutoHide(
									GetSysView(), inAutoHide);
									
	PPx_ThrowIfOSError_(status, "HIScrollViewSetScrollBarAutoHide failed");
}


// ---------------------------------------------------------------------------
//	GetAutoHideScrollBars											  [public]
/**
	Returns whether the automatically hide the scroll bars
	
	@return Whether the automatically hide the scroll bars					*/

bool
ScrollView::GetAutoHideScrollBars() const
{
	return ::HIScrollViewGetScrollBarAutoHide(GetSysView());
}


} // namespace PPx
