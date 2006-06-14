// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxSlider.cp
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:45 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxSlider.h>
#include <PPxSerializer.h>
#include <SysCreateView.h>

namespace PPx {

namespace {		// unnamed namespace 

	// -----------------------------------------------------------------------
	//	Key names for persistent data

	const CFStringRef	key_Orientation		= CFSTR("orientation");
	const CFStringRef	key_TickMarksCount	= CFSTR("tick marks count");
	const CFStringRef	key_HasLiveTracking	= CFSTR("has live tracking");
}


// ---------------------------------------------------------------------------
//	Slider														  [public]
/**
	Default constructor */

Slider::Slider()
{
	mOrientation	= kControlSliderDoesNotPoint;
	mTickMarksCount	= 0;
}


// ---------------------------------------------------------------------------
//	~Slider														  [public]
/**
	Destructor */

Slider::~Slider()
{
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initialize from slider creation parameters
	
	@param	inSuperView			Parent view
	@param	inFrame				Bounds for view, in local coords of parent
	@param	inVisible			Whether the view is visible
	@param	inEnabled			Whether the view is enabled
	@param	inInitialValue		Initial value of control
	@param	inMinValue			Minimum value of control
	@param	inMaxValue			Maximum value fo control
	@param	inOrientation		Orientation of thumb
	@param	inTickMarksCount	Number of tick marks to draw
	@param	inHasLiveTracking	Whether slider live tracks the thumb		*/

void
Slider::Initialize(
	View*						inSuperView,
	const HIRect&				inFrame,
	bool						inVisible,
	bool						inEnabled,
	SInt32						inInitialValue,
	SInt32						inMinValue,
	SInt32						inMaxValue,
	ControlSliderOrientation	inOrientation,
	UInt16						inTickMarksCount,
	bool						inHasLiveTracking)
{
	mOrientation	= inOrientation;
	mTickMarksCount	= inTickMarksCount;
	
		// ### Need a way to specify live tracking action
		// Do we need a ControlActionUPP? Or can we use a
		// a CarbonEvent handler?
	
	HIViewRef	viewRef = SysCreateView::Slider(
								inInitialValue, inMinValue, inMaxValue,
								inOrientation, inTickMarksCount,
								inHasLiveTracking, nil);

	View::Initialize(viewRef, inSuperView, inFrame, inVisible, inEnabled);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
Slider::ClassName() const
{
	return CFSTR("PPx::Slider");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
Slider::InitState(
	const DataReader&	inReader)
{
	SInt32		initialValue	= 0;		// Data for Slider
	SInt32		minValue		= 0;
	SInt32		maxValue		= 0;
	bool		hasLiveTracking = false;
	
	inReader.ReadOptional(key_InitValue, initialValue);
	inReader.ReadOptional(key_MinValue, minValue);
	inReader.ReadOptional(key_MaxValue, maxValue);
	inReader.ReadOptional(key_Orientation, mOrientation);
	inReader.ReadOptional(key_TickMarksCount, mTickMarksCount);
	inReader.ReadOptional(key_HasLiveTracking, hasLiveTracking);
	
		// ### Need a way to specify live tracking action
	
	HIViewRef	viewRef = SysCreateView::Slider(
								initialValue, minValue, maxValue,
								mOrientation, mTickMarksCount,
								hasLiveTracking, nil);
	
	View::InitViewState(viewRef, inReader);	// Data for all Views
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
Slider::WriteState(
	DataWriter&	ioWriter) const
{
											// Data for Slider
	ioWriter.WriteValue(key_InitValue, GetValue());
	ioWriter.WriteValue(key_MinValue, GetMinValue());
	ioWriter.WriteValue(key_MaxValue, GetMaxValue());
	ioWriter.WriteValue(key_Orientation, mOrientation);
	ioWriter.WriteValue(key_TickMarksCount, mTickMarksCount);
	
	View::WriteState(ioWriter);				// Data for all Views
}


} // namespace PPx
