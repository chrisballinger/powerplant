// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxPopupArrow.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:43 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxPopupArrow.h>
#include <PPxSerializer.h>
#include <SysCreateView.h>

namespace PPx {

namespace {		// unnamed namespace 

	// -----------------------------------------------------------------------
	//	Key names for persistent data

	const CFStringRef	key_Orientation		= CFSTR("orientation");
	const CFStringRef	key_ArrowSize		= CFSTR("arrow size");
}


// ---------------------------------------------------------------------------
//	PopupArrow													  [public]
/**
	Default constructor */

PopupArrow::PopupArrow()
{
	mOrientation = kControlPopupArrowOrientationEast;
	mArrowSize	 = kControlPopupArrowSizeNormal;
}


// ---------------------------------------------------------------------------
//	~PopupArrow													  [public]
/**
	Destructor */

PopupArrow::~PopupArrow()
{
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initialize from popup arrow creation parameters
	
	@param	inSuperView		Parent view
	@param	inFrame			Bounds for view, in local coordinates of parent
	@param	inVisible		Whether the view is visible
	@param	inEnabled		Whether the view is enabled	
	@param	inOrientation	Direction arrow points (north, east, south, west)
	@param	inArrowSize		Size of arrow (normal or small)					*/

void
PopupArrow::Initialize(
	View*							inSuperView,
	const HIRect&					inFrame,
	bool							inVisible,
	bool							inEnabled,
	ControlPopupArrowOrientation	inOrientation,
	ControlPopupArrowSize			inArrowSize)
{
	mOrientation = inOrientation;
	mArrowSize	 = inArrowSize;
	
	HIViewRef	viewRef = SysCreateView::PopupArrow(
								inOrientation, inArrowSize);

	View::Initialize(viewRef, inSuperView, inFrame, inVisible, inEnabled);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
PopupArrow::ClassName() const
{
	return CFSTR("PPx::PopupArrow");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
PopupArrow::InitState(
	const DataReader&	inReader)
{
											// Data for Popup Arrow
	inReader.ReadOptional(key_Orientation, mOrientation);
	inReader.ReadOptional(key_ArrowSize, mArrowSize);
	
	HIViewRef	viewRef = SysCreateView::PopupArrow(
								mOrientation, mArrowSize);
	
	View::InitViewState(viewRef, inReader);	// Data for all Views
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
PopupArrow::WriteState(
	DataWriter&	ioWriter) const
{
											// Data for Popup Arrow
	ioWriter.WriteValue(key_Orientation, mOrientation);
	ioWriter.WriteValue(key_ArrowSize, mArrowSize);
	
	View::WriteState(ioWriter);				// Data for all Views
}


} // namespace PPx
