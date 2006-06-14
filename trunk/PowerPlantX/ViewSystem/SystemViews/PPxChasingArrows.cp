// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxChasingArrows.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:39 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxChasingArrows.h>
#include <PPxSerializer.h>
#include <SysCreateView.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	ChasingArrows													  [public]
/**
	Default constructor */

ChasingArrows::ChasingArrows()
{
}


// ---------------------------------------------------------------------------
//	~ChasingArrows													  [public]
/**
	Destructor */

ChasingArrows::~ChasingArrows()
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
ChasingArrows::Initialize(
	View*			inSuperView,
	const HIRect&	inFrame,
	bool			inVisible,
	bool			inEnabled)
{
	HIViewRef	viewRef = SysCreateView::ChasingArrows();
	
	View::Initialize(viewRef, inSuperView, inFrame, inVisible, inEnabled);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
ChasingArrows::ClassName() const
{
	return CFSTR("PPx::ChasingArrows");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
ChasingArrows::InitState(
	const DataReader&	inReader)
{
	HIViewRef	viewRef = SysCreateView::ChasingArrows();
	
	View::InitViewState(viewRef, inReader);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SetAnimating													  [public]
/**
	Sets the option for animating the chasing arrows
	
	@param	inIsAnimating	Whether the arrows should be animating			*/

void
ChasingArrows::SetAnimating(
	bool	inIsAnimating)
{
	Boolean		animating = inIsAnimating;
	
	SetDataTag(kControlEntireControl, kControlChasingArrowsAnimatingTag,
				sizeof(Boolean), &animating);
}


// ---------------------------------------------------------------------------
//	IsAnimating														  [public]
/**
	Returns whether the chasing arrows are animating
	
	@return Whether the chasing arrows are animating						*/

bool
ChasingArrows::IsAnimating() const
{
	Boolean		animating;
	
	GetDataTag(kControlEntireControl, kControlChasingArrowsAnimatingTag,
				sizeof(Boolean), &animating);
				
	return animating;
}


} // namespace PPx
