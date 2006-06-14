// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxUserPane.cp
//
//	Copyright © 2004-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:46 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxUserPane.h>
#include <PPxSerializer.h>
#include <PPxViewUtils.h>
#include <SysCreateView.h>

namespace PPx {


// ---------------------------------------------------------------------------
//	UserPane														  [public]
/**
	Default constructor */

UserPane::UserPane()
{
}


// ---------------------------------------------------------------------------
//	~UserPane														  [public]
/**
	Destructor */

UserPane::~UserPane()
{
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initialize from user pane creation parameters
	
	@param	inSuperView			Parent view
	@param	inFrame				Bounds for view, in local coords of parent
	@param	inVisible			Whether the view is visible
	@param	inEnabled			Whether the view is enabled
	@param	inFeatures			Control feature bits						*/

void
UserPane::Initialize(
	View*			inSuperView,
	const HIRect&	inFrame,
	bool			inVisible,
	bool			inEnabled,
	UInt32			inFeatures)
{
	HIViewRef	viewRef = SysCreateView::UserPane(inFeatures);

	View::Initialize(viewRef, inSuperView, inFrame, inVisible, inEnabled);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
UserPane::ClassName() const
{
	return CFSTR("PPx::UserPane");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
UserPane::InitState(
	const DataReader&	inReader)
{
	// ### Not Finished
	UInt32	features = 0;

	HIViewRef	viewRef = SysCreateView::UserPane(features);
	
	View::InitViewState(viewRef, inReader);	// Data for all Views
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
UserPane::WriteState(
	DataWriter&	ioWriter) const
{
	// ### Not Finished
	View::WriteState(ioWriter);				// Data for all Views
}


} // namespace PPx
