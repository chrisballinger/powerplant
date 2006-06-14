// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxProgressBar.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:43 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxProgressBar.h>
#include <PPxSerializer.h>
#include <SysCreateView.h>

namespace PPx {

namespace {		// unnamed namespace 

	// -----------------------------------------------------------------------
	//	Key names for persistent data

	const CFStringRef	key_IsIndeterminate	= CFSTR("is indeterminate");
}


// ---------------------------------------------------------------------------
//	ProgressBar														  [public]
/**
	Default constructor */

ProgressBar::ProgressBar()
{
}


// ---------------------------------------------------------------------------
//	~ProgressBar													  [public]
/**
	Destructor */

ProgressBar::~ProgressBar()
{
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initialize from popup button creation parameters
	
	@param	inSuperView			Parent view
	@param	inFrame				Bounds for view, in local coords of parent
	@param	inVisible			Whether the view is visible
	@param	inEnabled			Whether the view is enabled
	@param	inInitialValue		Initial value of control
	@param	inMinValue			Minimum value of control
	@param	inMaxValue			Maximum value fo control
	@param	inIsIndeterminate	Whether progress bar is indeterminate		*/

void
ProgressBar::Initialize(
	View*			inSuperView,
	const HIRect&	inFrame,
	bool			inVisible,
	bool			inEnabled,
	SInt32			inInitialValue,
	SInt32			inMinValue,
	SInt32			inMaxValue,
	bool			inIsIndeterminate)
{
	HIViewRef	viewRef = SysCreateView::ProgressBar(
								inInitialValue, inMinValue, inMaxValue,
								inIsIndeterminate);

	View::Initialize(viewRef, inSuperView, inFrame, inVisible, inEnabled);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
ProgressBar::ClassName() const
{
	return CFSTR("PPx::ProgressBar");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
ProgressBar::InitState(
	const DataReader&	inReader)
{
	SInt32		initialValue	= 0;		// Data for Progress Bar
	SInt32		minValue		= 0;
	SInt32		maxValue		= 0;
	bool		isIndeterminate	= true;
	
	inReader.ReadOptional(key_InitValue, initialValue);
	inReader.ReadOptional(key_MinValue, minValue);
	inReader.ReadOptional(key_MaxValue, maxValue);
	inReader.ReadOptional(key_IsIndeterminate, isIndeterminate);
	
	HIViewRef	viewRef = SysCreateView::ProgressBar(
								initialValue, minValue, maxValue,
								isIndeterminate);
	
	View::InitViewState(viewRef, inReader);	// Data for all Views
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
ProgressBar::WriteState(
	DataWriter&	ioWriter) const
{
											// Data for Progress Bar
	ioWriter.WriteValue(key_InitValue, GetValue());
	ioWriter.WriteValue(key_MinValue, GetMinValue());
	ioWriter.WriteValue(key_MaxValue, GetMaxValue());
	ioWriter.WriteValue(key_IsIndeterminate, IsIndeterminate());
	
	View::WriteState(ioWriter);				// Data for all Views
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SetIndeterminate												  [public]
/**
	Sets whether the progress bar is indeterminate
	
	@param	inIsIndeterminate	Wwhether the progress bar is indeterminate	*/

void
ProgressBar::SetIndeterminate(
	bool	inIsIndeterminate)
{
	Boolean		indeterminate = inIsIndeterminate;
	
	SetDataTag(kControlEntireControl, kControlProgressBarIndeterminateTag,
				sizeof(Boolean), &indeterminate);
}


// ---------------------------------------------------------------------------
//	IsIndeterminate												  [public]
/**
	Returns whether the progress bar is indeterminate
	
	@return Whether the progress bar is indeterminate						*/

bool
ProgressBar::IsIndeterminate() const
{
	Boolean		indeterminate;
	
	GetDataTag(kControlEntireControl, kControlProgressBarIndeterminateTag,
				sizeof(Boolean), &indeterminate);
				
	return indeterminate;
}


// ---------------------------------------------------------------------------
//	SetAnimating													  [public]
/**
	Sets the option for animating the progress bar
	
	@param	inIsAnimating	Whether the progress bar should be animating	*/

void
ProgressBar::SetAnimating(
	bool	inIsAnimating)
{
	Boolean		animating = inIsAnimating;
	
	SetDataTag(kControlEntireControl, kControlProgressBarAnimatingTag,
				sizeof(Boolean), &animating);
}


// ---------------------------------------------------------------------------
//	IsAnimating													  [public]
/**
	Returns whether the progress bar is animating
	
	@return Whether the progress bar is animating							*/

bool
ProgressBar::IsAnimating() const
{
	Boolean		animating;
	
	GetDataTag(kControlEntireControl, kControlProgressBarAnimatingTag,
				sizeof(Boolean), &animating);
				
	return animating;
}


} // namespace PPx
