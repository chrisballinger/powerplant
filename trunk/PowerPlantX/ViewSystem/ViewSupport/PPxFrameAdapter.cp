// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxFrameAdapter.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:47 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxFrameAdapter.h>
#include <PPxSerializer.h>

namespace PPx {

namespace {	// unnamed

	// -----------------------------------------------------------------------
	//	Key names for persistent data

	const CFStringRef	key_BindLeft	= CFSTR("bind left");
	const CFStringRef	key_BindTop		= CFSTR("bind top");
	const CFStringRef	key_BindRight	= CFSTR("bind right");
	const CFStringRef	key_BindBottom	= CFSTR("bind bottom");
}


// ---------------------------------------------------------------------------
//	BindingsFrameAdapter											  [public]
/**
	Default constructor */

BindingsFrameAdapter::BindingsFrameAdapter()
{
	mBindLeft	= false;
	mBindTop	= false;
	mBindRight	= false;
	mBindBottom	= false;
}


// ---------------------------------------------------------------------------
//	BindingsFrameAdapter											  [public]
/**
	Destructor */

BindingsFrameAdapter::~BindingsFrameAdapter()
{
}


// ---------------------------------------------------------------------------
//	SetBindings														  [public]
/**
	Sets the bindings for each side of the frame
	
	@param	inBindLeft		Binding for left of frame
	@param	inBindTop		Binding for top of frame
	@param	inBindRight		Binding for right of frame
	@param	inBindBottom	Binding for bottom of frame						*/

void
BindingsFrameAdapter::SetBindings(
	bool	inBindLeft,
	bool	inBindTop,
	bool	inBindRight,
	bool	inBindBottom)
{
	mBindLeft	= inBindLeft;
	mBindTop	= inBindTop;
	mBindRight	= inBindRight;
	mBindBottom	= inBindBottom;
}


// ---------------------------------------------------------------------------
//	DoAdaptFrame													 [private]
/**
	Determines size of frame based on old and new sizes of the superframe
	
	@param	inOldSuperFrame		Old bounds of container frame
	@param	inNewSuperFrame		New bounds of container frame
	@param	ioFrame				On entry, bounds of child frame
								On exit, new bounds for child frame			*/

void
BindingsFrameAdapter::DoAdaptFrame(
	const HIRect&	inOldSuperFrame,
	const HIRect&	inNewSuperFrame,
	HIRect&			ioFrame) const
{
	float	widthDelta	= inNewSuperFrame.size.width
							- inOldSuperFrame.size.width;
	float	heightDelta	= inNewSuperFrame.size.height
							- inOldSuperFrame.size.height;
							
		// A bound side remains a fixed distance from the corresponding
		// side of its superframe

	if (mBindRight and not mBindLeft ) {	// X origin moves
		ioFrame.origin.x += widthDelta;
	}
	
	if (mBindBottom and not mBindTop) {		// Y origin moves
		ioFrame.origin.y += heightDelta;
	}
	
	if (mBindLeft and mBindRight) {			// Width changes
		ioFrame.size.width += widthDelta;
	}
	
	if (mBindTop and mBindBottom) {			// Height changes
		ioFrame.size.height += heightDelta;
	}
}


// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
BindingsFrameAdapter::ClassName() const
{
	return CFSTR("PPx::BindingsFrameAdapter");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
BindingsFrameAdapter::InitState(
	const DataReader&	inReader)
{
	inReader.ReadOptional(key_BindLeft,		mBindLeft);
	inReader.ReadOptional(key_BindTop,		mBindTop);
	inReader.ReadOptional(key_BindRight,	mBindRight);
	inReader.ReadOptional(key_BindBottom,	mBindBottom);
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
BindingsFrameAdapter::WriteState(
	DataWriter&	ioWriter) const
{
	ioWriter.WriteValue(key_BindLeft,	mBindLeft);
	ioWriter.WriteValue(key_BindTop,	mBindTop);
	ioWriter.WriteValue(key_BindRight,	mBindRight);
	ioWriter.WriteValue(key_BindBottom,	mBindBottom);
}

} // namespace PPx
