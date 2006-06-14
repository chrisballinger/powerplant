// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxScrollBar.cp
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:44 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxScrollBar.h>
#include <PPxSerializer.h>
#include <SysCreateView.h>

namespace PPx {

namespace {		// unnamed namespace 

	// -----------------------------------------------------------------------
	//	Key names for persistent data

	const CFStringRef	key_ViewSize		= CFSTR("view size");
	const CFStringRef	key_HasLiveTracking	= CFSTR("has live tracking");
}

// ---------------------------------------------------------------------------
//	ScrollBar														  [public]
/**
	Default constructor */

ScrollBar::ScrollBar()
{
}


// ---------------------------------------------------------------------------
//	~ScrollBar														  [public]
/**
	Destructor */

ScrollBar::~ScrollBar()
{
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initialize from icon control creation parameters
	
	@param	inSuperView			Parent view
	@param	inFrame				Bounds for view, in local coords of parent
	@param	inVisible			Whether the view is visible
	@param	inEnabled			Whether the view is enabled
	@param	inInitialValue		Initial value of control
	@param	inMinValue			Minimum value of control
	@param	inMaxValue			Maximum value fo control
	@param	inViewSize			Size of view being scrolled
	@param	inHasLiveTracking	Whether content scrolls as thumb is dragged
*/

void
ScrollBar::Initialize(
	View*				inSuperView,
	const HIRect&		inFrame,
	bool				inVisible,
	bool				inEnabled,
	SInt32				inInitialValue,
	SInt32				inMinValue,
	SInt32				inMaxValue,
	SInt32				inViewSize,
	bool				inHasLiveTracking)
{
	
		// ### Need a way to specify live tracking action
		// Do we need a ControlActionUPP? Or can we use a
		// a CarbonEvent handler?
	
	HIViewRef	viewRef = SysCreateView::ScrollBar(
								inInitialValue, inMinValue, inMaxValue,
								inViewSize, inHasLiveTracking,
								nil);
	
	View::Initialize(viewRef, inSuperView, inFrame, inVisible, inEnabled);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
ScrollBar::ClassName() const
{
	return CFSTR("PPx::ScrollBar");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
ScrollBar::InitState(
	const DataReader&	inReader)
{
	SInt32		initialValue	= 0;		// Data for Scroll Bar
	SInt32		minValue		= 0;
	SInt32		maxValue		= 0;
	SInt32		viewSize		= 0;
	bool		hasLiveTracking = false;
	
	inReader.ReadOptional(key_InitValue, initialValue);
	inReader.ReadOptional(key_MinValue, minValue);
	inReader.ReadOptional(key_MaxValue, maxValue);
	inReader.ReadOptional(key_ViewSize, viewSize);
	inReader.ReadOptional(key_HasLiveTracking, hasLiveTracking);
	
		// ### Need a way to specify live tracking action
		// Do we need a ControlActionUPP? Or can we use a
		// a CarbonEvent handler?
	
	HIViewRef	viewRef = SysCreateView::ScrollBar(
								initialValue, minValue, maxValue,
								viewSize, hasLiveTracking,
								nil);
								
		// +++ Read and set option for "shows arrows"
	
	View::InitViewState(viewRef, inReader);	// Data for all Views
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
ScrollBar::WriteState(
	DataWriter&	ioWriter) const
{
											// Data for Scroll Bar
	ioWriter.WriteValue(key_InitValue, GetValue());
	ioWriter.WriteValue(key_MinValue, GetMinValue());
	ioWriter.WriteValue(key_MaxValue, GetMaxValue());
	ioWriter.WriteValue(key_ViewSize, GetViewSize());

	View::WriteState(ioWriter);				// Data for all Views
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SetViewSize														  [public]
/**
	Sets the view size for the view being scrolled
	
	@param	inViewSize		View size for the view being scrolled			*/

void
ScrollBar::SetViewSize(
	SInt32	inViewSize)
{
	SysHIView	sysView( GetSysView() );
	sysView.SetViewSize(inViewSize);
}


// ---------------------------------------------------------------------------
//	GetViewSize														  [public]
/**
	Returns the view size for the view being scrolled
	
	@return View size for the view being scrolled							*/

SInt32
ScrollBar::GetViewSize() const
{
	SysHIView	sysView( GetSysView() );
	return sysView.GetViewSize();
}


// ---------------------------------------------------------------------------
//	SetShowsArrowsFlag												  [public]
/**
	Sets whether to show the scroll bar arrows
	
	@param	inShowsArrows	Whether to show the scroll bar arrows			*/

void
ScrollBar::SetShowsArrowsFlag(
	bool	inShowsArrows)
{
	Boolean		showsArrows = inShowsArrows;
	
	SetDataTag(kControlEntireControl, kControlScrollBarShowsArrowsTag,
				sizeof(Boolean), &showsArrows);
}


// ---------------------------------------------------------------------------
//	GetShowsArrowsFlag												  [public]
/**
	Returns whether to show the scroll bar arrows
	
	@return Whether to show the scroll bar arrows							*/

bool
ScrollBar::GetShowsArrowsFlag() const
{
	Boolean		showsArrows;
	
	GetDataTag(kControlEntireControl, kControlScrollBarShowsArrowsTag,
				sizeof(Boolean), &showsArrows);
				
	return showsArrows;
}


} // namespace PPx
