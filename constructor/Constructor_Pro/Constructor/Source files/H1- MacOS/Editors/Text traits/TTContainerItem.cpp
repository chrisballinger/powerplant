// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	TTContainerItem.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 10/29/96
//	   $Date: 2006/01/18 01:33:53 $
//	$History: TTContainerItem.cpp $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:18
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Text traits
//	Improved commenting.
//
//	*****************  Version 2   *****************
//	User: scouten      Date: 01/24/97   Time: 17:43
//	Checked in '$/Constructor/Source files/H1- MacOS/Editors/Text traits'
//	Comment: Fixed CR/LF problem
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/30/96   Time: 18:29
//	Created
//	Comment: Baseline Txtr editor.
//	
// ===========================================================================

#include "TTContainerItem.h"


// ===========================================================================

#pragma mark *** TTContainerItem ***

// ---------------------------------------------------------------------------
//		* TTContainerItem()
// ---------------------------------------------------------------------------
//	Constructor

TTContainerItem::TTContainerItem()
{
}


// ---------------------------------------------------------------------------
//		* ~TTContainerItem
// ---------------------------------------------------------------------------
//	Destructor

TTContainerItem::~TTContainerItem()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** disclosure triangle

// ---------------------------------------------------------------------------
//		* Collapse
// ---------------------------------------------------------------------------
//	Overriden to disable collapsing.

void
TTContainerItem::Collapse()
{
	mExpanded = false;
}



// ---------------------------------------------------------------------------
//		* CanExpand
// ---------------------------------------------------------------------------
//	Overriden to disable drawing the disclosure triangle.

Boolean
TTContainerItem::CanExpand() const
{
	return false;
}



// ---------------------------------------------------------------------------
//		* CalcLocalDisclosureTriangleRect
// ---------------------------------------------------------------------------
//	Overriden to disable drawing the disclosure triangle.

Boolean
TTContainerItem::CalcLocalDisclosureTriangleRect(
	Rect&	/* outTriangleRect */)
{
	return false;
}


// ===========================================================================

#pragma mark -
#pragma mark ** disclosure triangle

// ---------------------------------------------------------------------------
//		* GetDrawContentsSelf									[protected]
// ---------------------------------------------------------------------------
//	Overriden to prevent attribute title from being drawn in bold.

void
TTContainerItem::GetDrawContentsSelf(
	const STableCell&		inCell,
	SOutlineDrawContents&	ioDrawContents)
{

	// Use default behavior for getting value.

	PIContainerItem::GetDrawContentsSelf(inCell, ioDrawContents);

	// Cancel bold title behavior.

	if (inCell.col == 1)
		ioDrawContents.outTextTraits.style &= ~bold;

}

// ---------------------------------------------------------------------------
//		* DrawRowAdornments										[protected]
// ---------------------------------------------------------------------------
//	Overriden to disable the shaded row adornment that is typically drawn.

void
TTContainerItem::DrawRowAdornments(
	const Rect&		inLocalRowRect)
{
	PIAttributeItem::DrawRowAdornments(inLocalRowRect);
}