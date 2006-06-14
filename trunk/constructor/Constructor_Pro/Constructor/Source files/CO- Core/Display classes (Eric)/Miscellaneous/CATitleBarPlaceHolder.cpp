// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	CATitleBarPlaceHolder.cpp	   © 1996 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 12/16/96
//     $Date: 2006/01/18 01:32:42 $
//  $History: CATitleBarPlaceHolder.cpp $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:25
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Miscellaneous
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: Andrew       Date: 2/04/97    Time: 2:48p
//	Updated in $/ConstructorWin/Sources
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:23
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Miscellaneous
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 12/17/96   Time: 00:14
//	Created in $/Constructor/Source files/Display classes (Eric)/Miscellaneous
//	Added class.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "CATitleBarPlaceHolder.h"

	// PowerPlant : CA : Utilities
#include "UPrintingUtils.h"


// ===========================================================================

#pragma mark *** CATitleBarPlaceHolder ***

// ---------------------------------------------------------------------------
//		* CATitleBarPlaceHolder(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor

CATitleBarPlaceHolder::CATitleBarPlaceHolder(
	LStream*	inStream)

: LPlaceHolder(inStream)

{
}


// ---------------------------------------------------------------------------
//		* ~CATitleBarPlaceHolder
// ---------------------------------------------------------------------------
//	Destructor

CATitleBarPlaceHolder::~CATitleBarPlaceHolder()
{
}


// ---------------------------------------------------------------------------
//		* DrawSelf
// ---------------------------------------------------------------------------
//	Overridden to draw a thin hairline along the bottom of the title bar.

void
CATitleBarPlaceHolder::DrawSelf()
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Draw a thin line along bottom of frame.

	Rect frame;
	CalcLocalFrameRect(frame);
	
	StFractionalPenWidth hairline(1, 6);
	
	::MoveTo(frame.left, frame.bottom - 1);
	::LineTo(frame.right, frame.bottom - 1);

}


// ---------------------------------------------------------------------------
//		* ScrollToPanel
// ---------------------------------------------------------------------------
//	Overridden to prevent vertical scrolling.

Boolean
CATitleBarPlaceHolder::ScrollToPanel(
	const PanelSpec&	inPanel)
{
	PanelSpec tempSpec = inPanel;
	tempSpec.vertIndex = 1;
	
	return LPlaceHolder::ScrollToPanel(tempSpec);
}
