// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxSeparatorLine.cp
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:45 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxSeparatorLine.h>
#include <PPxSerializer.h>
#include <SysCreateView.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	SeparatorLine													  [public]
/**
	Default constructor */

SeparatorLine::SeparatorLine()
{
}


// ---------------------------------------------------------------------------
//	~SeparatorLine												  [public]
/**
	Destructor */

SeparatorLine::~SeparatorLine()
{
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initialize from chasing arrows creation parameters
	
	@param	inSuperView		Parent view
	@param	inFrame			Bounds for view, in local coordinates of parent
	@param	inVisible		Whether the view is visible
	@param	inEnabled		Whether the view is enabled						*/

void
SeparatorLine::Initialize(
	View*			inSuperView,
	const HIRect&	inFrame,
	bool			inVisible,
	bool			inEnabled)
{
	HIViewRef	viewRef = SysCreateView::SeparatorLine();

	View::Initialize(viewRef, inSuperView, inFrame, inVisible, inEnabled);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
SeparatorLine::ClassName() const
{
	return CFSTR("PPx::SeparatorLine");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
SeparatorLine::InitState(
	const DataReader&	inReader)
{
	HIViewRef	viewRef = SysCreateView::SeparatorLine();
	
	View::InitViewState(viewRef, inReader);	// Data for all Views
}


} // namespace PPx
