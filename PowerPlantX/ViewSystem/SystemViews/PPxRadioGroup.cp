// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxRadioGroup.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:43 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxRadioGroup.h>
#include <PPxSerializer.h>
#include <SysCreateView.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	RadioGroup													  [public]
/**
	Default constructor */

RadioGroup::RadioGroup()
{
}


// ---------------------------------------------------------------------------
//	~RadioGroup													  [public]
/**
	Destructor */

RadioGroup::~RadioGroup()
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
RadioGroup::Initialize(
	View*			inSuperView,
	const HIRect&	inFrame,
	bool			inVisible,
	bool			inEnabled)
{
	HIViewRef	viewRef = SysCreateView::RadioGroup();

	View::Initialize(viewRef, inSuperView, inFrame, inVisible, inEnabled);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
RadioGroup::ClassName() const
{
	return CFSTR("PPx::RadioGroup");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
RadioGroup::InitState(
	const DataReader&	inReader)
{
	HIViewRef	viewRef = SysCreateView::RadioGroup();
	
	View::InitViewState(viewRef, inReader);	// Data for all Views
}

#pragma mark -

// ---------------------------------------------------------------------------
//	GetCurrentButton												  [public]
/**
	Returns the current radio button
	
	@return	Current radio button											*/

View*
RadioGroup::GetCurrentButton() const
{
	View*		currentButton = nil;
	
	SInt32		currentIndex = GetValue();
	ControlRef	currentRef;
	
	OSStatus	status = ::GetIndexedSubControl( GetSysView(),
												 GetValue(),
												 &currentRef );
												 
	if (status == noErr) {
		currentButton = GetViewObject(currentRef);
	}
	
	return currentButton;
}

} // namespace PPx
