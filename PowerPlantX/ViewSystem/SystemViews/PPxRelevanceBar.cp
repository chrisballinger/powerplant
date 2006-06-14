// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxRelevanceBar.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:44 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxRelevanceBar.h>
#include <PPxSerializer.h>
#include <SysCreateView.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	RelevanceBar													  [public]
/**
	Default constructor */

RelevanceBar::RelevanceBar()
{
}


// ---------------------------------------------------------------------------
//	~RelevanceBar													  [public]
/**
	Destructor */

RelevanceBar::~RelevanceBar()
{
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initialize from relevance bar creation parameters
	
	@param	inSuperView			Parent view
	@param	inFrame				Bounds for view, in local coords of parent
	@param	inVisible			Whether the view is visible
	@param	inEnabled			Whether the view is enabled
	@param	inInitialValue		Initial value of control
	@param	inMinValue			Minimum value of control
	@param	inMaxValue			Maximum value fo control					*/

void
RelevanceBar::Initialize(
	View*			inSuperView,
	const HIRect&	inFrame,
	bool			inVisible,
	bool			inEnabled,
	SInt32			inInitialValue,
	SInt32			inMinValue,
	SInt32			inMaxValue)
{
	HIViewRef	viewRef = SysCreateView::RelevanceBar(
								inInitialValue, inMinValue, inMaxValue);

	View::Initialize(viewRef, inSuperView, inFrame, inVisible, inEnabled);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
RelevanceBar::ClassName() const
{
	return CFSTR("PPx::RelevanceBar");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
RelevanceBar::InitState(
	const DataReader&	inReader)
{
	SInt32		initialValue = 0;			// Data for Relevance Bar
	SInt32		minValue	 = 0;
	SInt32		maxValue	 = 0;
	
	inReader.ReadOptional(key_InitValue, initialValue);
	inReader.ReadOptional(key_MinValue, minValue);
	inReader.ReadOptional(key_MaxValue, maxValue);
	
	HIViewRef	viewRef = SysCreateView::RelevanceBar(
								initialValue, minValue, maxValue);
	
	View::InitViewState(viewRef, inReader);	// Data for all Views
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
RelevanceBar::WriteState(
	DataWriter&	ioWriter) const
{
											// Data for Relevance Bar
	ioWriter.WriteValue(key_InitValue, GetValue());
	ioWriter.WriteValue(key_MinValue, GetMinValue());
	ioWriter.WriteValue(key_MaxValue, GetMaxValue());
	
	View::WriteState(ioWriter);				// Data for all Views
}


} // namespace PPx
