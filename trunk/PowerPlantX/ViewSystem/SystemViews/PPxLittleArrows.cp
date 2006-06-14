// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxLittleArrows.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:42 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxLittleArrows.h>
#include <PPxSerializer.h>
#include <SysCreateView.h>

namespace PPx {

namespace {		// unnamed namespace 

	// -----------------------------------------------------------------------
	//	Key names for persistent data

	const CFStringRef	key_Increment		= CFSTR("increment");
}


// ---------------------------------------------------------------------------
//	LittleArrows													  [public]
/**
	Default constructor */

LittleArrows::LittleArrows()
{
	mIncrement = 1;
}


// ---------------------------------------------------------------------------
//	~LittleArrows													  [public]
/**
	Destructor */

LittleArrows::~LittleArrows()
{
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initialize from little arrows creation parameters
	
	@param	inSuperView		Parent view
	@param	inFrame			Bounds for view, in local coordinates of parent
	@param	inVisible		Whether the view is visible
	@param	inEnabled		Whether the view is enabled
	@param	inInitialValue	Initial value of control
	@param	inMinValue		Minimum value of control
	@param	inMaxValue		Maximum value fo control
	@param	inIncrement		Amout to increment/decrement value when clicked	*/

void
LittleArrows::Initialize(
	View*			inSuperView,
	const HIRect&	inFrame,
	bool			inVisible,
	bool			inEnabled,
	SInt32			inInitialValue,
	SInt32			inMinValue,
	SInt32			inMaxValue,
	SInt32			inIncrement)
{
	mIncrement = inIncrement;
	
	HIViewRef	viewRef = SysCreateView::LittleArrows(
								inInitialValue, inMinValue, inMaxValue,
								inIncrement);

	View::Initialize(viewRef, inSuperView, inFrame, inVisible, inEnabled);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
LittleArrows::ClassName() const
{
	return CFSTR("PPx::LittleArrows");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
LittleArrows::InitState(
	const DataReader&	inReader)
{
	SInt32		initialValue = 0;			// Data for Little Arrows
	SInt32		minValue	 = 0;
	SInt32		maxValue	 = 0;
	SInt32		increment	 = 1;
	
	inReader.ReadOptional(key_InitValue, initialValue);
	inReader.ReadOptional(key_MinValue, minValue);
	inReader.ReadOptional(key_MaxValue, initialValue);
	inReader.ReadOptional(key_Increment, increment);
	
	HIViewRef	viewRef = SysCreateView::LittleArrows(
								initialValue, minValue, maxValue,
								increment);
	
	View::InitViewState(viewRef, inReader);	// Data for all Views
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
LittleArrows::WriteState(
	DataWriter&	ioWriter) const
{
											// Data for Little Arrows
	ioWriter.WriteValue(key_InitValue, GetValue());
	ioWriter.WriteValue(key_MinValue, GetMinValue());
	ioWriter.WriteValue(key_MaxValue, GetMaxValue());
	ioWriter.WriteValue(key_Increment, mIncrement);
	
	View::WriteState(ioWriter);				// Data for all Views
}


} // namespace PPx
