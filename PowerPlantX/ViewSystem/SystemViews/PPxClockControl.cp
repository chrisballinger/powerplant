// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxClockControl.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:40 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxClockControl.h>
#include <PPxSerializer.h>
#include <PPxViewUtils.h>
#include <SysCreateView.h>

namespace PPx {

namespace {	// unnamed namespace

	// -----------------------------------------------------------------------
	//	Key names for persistent data

	const CFStringRef	key_ClockType		= CFSTR("clock type");
	const CFStringRef	key_ClockFlags		= CFSTR("clock flags");
}


// ---------------------------------------------------------------------------
//	ClockControl													  [public]
/**
	Default constructor */

ClockControl::ClockControl()
{
	mClockType	= kControlClockTypeHourMinute;
	mClockFlags	= kControlClockFlagStandard;
}


// ---------------------------------------------------------------------------
//	~ClockControl													  [public]
/**
	Destructor */

ClockControl::~ClockControl()
{
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initialize from clock creation parameters
	
	@param	inSuperView		Parent view
	@param	inFrame			Bounds for view, in local coordinates of parent
	@param	inVisible		Whether the view is visible
	@param	inEnabled		Whether the view is enabled
	@param	inClockType		Kind of clock (time or date)
	@param	inClockFlags	Clock options									*/

void
ClockControl::Initialize(
	View*				inSuperView,
	const HIRect&		inFrame,
	bool				inVisible,
	bool				inEnabled,
	ControlClockType	inClockType,
	ControlClockFlags	inClockFlags)
{
	mClockType	= inClockType;
	mClockFlags	= inClockFlags;

	HIViewRef	viewRef = SysCreateView::ClockControl(
								inClockType, inClockFlags);

	View::Initialize(viewRef, inSuperView, inFrame, inVisible, inEnabled);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
ClockControl::ClassName() const
{
	return CFSTR("PPx::ClockControl");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
ClockControl::InitState(
	const DataReader&	inReader)
{
											// Data for Clock
	inReader.ReadOptional(key_ClockType, mClockType);
	inReader.ReadOptional(key_ClockFlags, mClockFlags);

	HIViewRef	viewRef = SysCreateView::ClockControl(
								mClockType, mClockFlags);
	
	View::InitViewState(viewRef, inReader);	// Data for all Views
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
ClockControl::WriteState(
	DataWriter&	ioWriter) const
{
											// Data for Clock
	ioWriter.WriteValue(key_ClockType, mClockType);
	ioWriter.WriteValue(key_ClockFlags, mClockFlags);
	
	View::WriteState(ioWriter);				// Data for all Views
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SetLongDate													  [public]
/**
	Sets the long date for the clock
	
	@param	inLongDate	Long data to display in clock						*/

void
ClockControl::SetLongDate(
	const LongDateRec&	inLongDate)
{
	SetDataTag(kControlClockPart, kControlClockLongDateTag,
				sizeof(LongDateRec), &inLongDate);
}


// ---------------------------------------------------------------------------
//	GetLongDate													  [public]
/**
	Passes back long date stored in the clock
	
	@param	outLongDate		Long date from clock							*/

void
ClockControl::GetLongDate(
	LongDateRec&	outLongDate)
{
	GetDataTag(kControlClockPart, kControlClockLongDateTag,
				sizeof(LongDateRec), &outLongDate);
}


// ---------------------------------------------------------------------------
//	SetThemeFontID												  [public]
/**
	Sets the theme font ID for the clock
	
	@param	inFont		Theme font ID to use for text						*/

void
ClockControl::SetThemeFontID(
	ThemeFontID		inFont)
{
	ViewUtils::SetControlThemeFontID(this, inFont);
}


// ---------------------------------------------------------------------------
//	SetAnimating													  [public]
/**
	Sets the option for animating the clock
	
	@param	inIsAnimating	Whether the clock should be animating			*/

void
ClockControl::SetAnimating(
	bool	inIsAnimating)
{
	Boolean		animating = inIsAnimating;
	
	SetDataTag(kControlClockPart, kControlClockAnimatingTag,
				sizeof(Boolean), &animating);
}


// ---------------------------------------------------------------------------
//	IsAnimating													  [public]
/**
	Returns whether the clock is animating
	
	@return Whether the clock is animating									*/

bool
ClockControl::IsAnimating() const
{
	Boolean		animating;
	
	GetDataTag(kControlClockPart, kControlClockAnimatingTag,
				sizeof(Boolean), &animating);
				
	return animating;
}


} // namespace PPx
