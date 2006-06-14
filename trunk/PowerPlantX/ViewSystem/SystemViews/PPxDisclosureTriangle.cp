// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxDisclosureTriangle.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:41 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxDisclosureTriangle.h>
#include <PPxSerializer.h>
#include <SysCreateView.h>

namespace PPx {

namespace {		// unnamed namespace 

	// -----------------------------------------------------------------------
	//	Key names for persistent data

	const CFStringRef	key_Orientation		= CFSTR("orientation");
	const CFStringRef	key_DrawTitle		= CFSTR("draw title");
}


// ---------------------------------------------------------------------------
//	DisclosureTriangle											  [public]
/**
	Default constructor */

DisclosureTriangle::DisclosureTriangle()
{
	mOrientation	= kControlDisclosureTrianglePointDefault;
	mDrawTitle		= false;
	mAutoToggle		= true;
}


// ---------------------------------------------------------------------------
//	~DisclosureTriangle											  [public]
/**
	Destructor */

DisclosureTriangle::~DisclosureTriangle()
{
}

#pragma mark -

// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initialize from disclosure triangle creation parameters
	
	@param	inSuperView		Parent view
	@param	inFrame			Bounds for view, in local coordinates of parent
	@param	inVisible		Whether the view is visible
	@param	inEnabled		Whether the view is enabled	
	@param	inOrientation	Direction triangle points when closed
	@param	inTitle			Title for disclosure triangle
	@param	inInitialValue	0 = closed, 1 = open
	@param	inDrawTitle		Whether to draw the title
	@param	inAutoToggle	Whether the triangle automatically toggles
								between open/closed when clicked			*/

void
DisclosureTriangle::Initialize(
	View*			inSuperView,
	const HIRect&	inFrame,
	bool			inVisible,
	bool			inEnabled,
	ControlDisclosureTriangleOrientation inOrientation,
	CFStringRef		inTitle,
	SInt32			inInitialValue,
	bool			inDrawTitle,
	bool			inAutoToggle)
{
	mOrientation	= inOrientation;
	mDrawTitle		= inDrawTitle;
	mAutoToggle		= inAutoToggle;

	HIViewRef	viewRef = SysCreateView::DisclosureTriangle(
								inOrientation, inTitle, inInitialValue,
								inDrawTitle, inAutoToggle);

	View::Initialize(viewRef, inSuperView, inFrame, inVisible, inEnabled);
}


// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
DisclosureTriangle::ClassName() const
{
	return CFSTR("PPx::DisclosureTriangle");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
DisclosureTriangle::InitState(
	const DataReader&	inReader)
{
											// Data for Disclosure Triangle
	CFString	title;
	SInt32		initialValue = value_Closed;
	
	inReader.ReadOptional(key_Orientation, mOrientation);
	inReader.ReadOptional(key_ViewTitle, title);
	inReader.ReadOptional(key_InitValue, initialValue);
	inReader.ReadOptional(key_DrawTitle, mDrawTitle);
	inReader.ReadOptional(key_AutoToggle, mAutoToggle);

	HIViewRef	viewRef = SysCreateView::DisclosureTriangle(
								mOrientation, title, initialValue,
								mDrawTitle, mAutoToggle);
	
	View::InitViewState(viewRef, inReader);	// Data for all Views
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
DisclosureTriangle::WriteState(
	DataWriter&	ioWriter) const
{
											// Data for Disclosure Triangle
	ioWriter.WriteValue(key_Orientation, mOrientation);

	ioWriter.WriteValue(key_ViewTitle, GetTitle());
	
	ioWriter.WriteValue(key_InitValue, GetValue());
	ioWriter.WriteValue(key_DrawTitle, mDrawTitle);
	ioWriter.WriteValue(key_AutoToggle, mAutoToggle);
	
	View::WriteState(ioWriter);				// Data for all Views
}


} // namespace PPx
