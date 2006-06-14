// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxWindowHeader.cp
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:46 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxWindowHeader.h>
#include <PPxSerializer.h>
#include <SysCreateView.h>

namespace PPx {

namespace {		// unnamed namespace 

	// -----------------------------------------------------------------------
	//	Key names for persistent data

	const CFStringRef	key_IsListHeader	= CFSTR("IsListHeader");
}


// ---------------------------------------------------------------------------
//	WindowHeader													  [public]
/**
	Default constructor */

WindowHeader::WindowHeader()
{
	mIsListHeader = false;
}


// ---------------------------------------------------------------------------
//	~WindowHeader													  [public]
/**
	Destructor */

WindowHeader::~WindowHeader()
{
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initialize from window header creation parameters
	
	@param	inSuperView		Parent view
	@param	inFrame			Bounds for view, in local coords of parent
	@param	inVisible		Whether the view is visible
	@param	inEnabled		Whether the view is enabled
	@param	inIsListHeader	Whether header is for a list view				*/

void
WindowHeader::Initialize(
	View*			inSuperView,
	const HIRect&	inFrame,
	bool			inVisible,
	bool			inEnabled,
	bool			inIsListHeader)		// List header has no bottom line
{
	mIsListHeader = inIsListHeader;
	
	HIViewRef	viewRef = SysCreateView::WindowHeader(inIsListHeader);

	View::Initialize(viewRef, inSuperView, inFrame, inVisible, inEnabled);
}


// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
WindowHeader::ClassName() const
{
	return CFSTR("PPx::WindowHeader");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
WindowHeader::InitState(
	const DataReader&	inReader)
{
											// Data for Window Header
	inReader.ReadOptional(key_IsListHeader, mIsListHeader);
	
	HIViewRef	viewRef = SysCreateView::WindowHeader(mIsListHeader);
	
	View::InitViewState(viewRef, inReader);	// Data for all Views
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
WindowHeader::WriteState(
	DataWriter&	ioWriter) const
{
											// Data for Window Header
	ioWriter.WriteValue(key_IsListHeader, mIsListHeader);
	
	View::WriteState(ioWriter);				// Data for all Views
}


} // namespace PPx
