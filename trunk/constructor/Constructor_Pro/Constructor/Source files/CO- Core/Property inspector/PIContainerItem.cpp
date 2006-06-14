// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PIContainerItem.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 07/19/96
//     $Date: 2006/01/18 01:33:21 $
//	$History: PIContainerItem.cpp $
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 02/28/97   Time: 11:48
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Added #ifs for Pilot appearance.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/21/97   Time: 11:15
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Improved commenting.
//	
//	*****************  Version 4  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:05p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:31
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 10/31/96   Time: 17:24
//	Updated in $/Constructor/Source files/Property inspector
//	Added support for attribute's enabled flag.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "PIContainerItem.h"

	// PowerPlant : GA : Grayscale utilities
#include "UGAColorRamp.h"


// ===========================================================================

#pragma mark *** PIContainerItem ***

// ---------------------------------------------------------------------------
//		* PIContainerItem()
// ---------------------------------------------------------------------------
//	Constructor

PIContainerItem::PIContainerItem()
{
	mExpanded = true;
}


// ---------------------------------------------------------------------------
//		* ~PIContainerItem
// ---------------------------------------------------------------------------
//	Destructor

PIContainerItem::~PIContainerItem()
{
	ValidateThis_();
}


// ---------------------------------------------------------------------------
//		* GetDrawContentsSelf									[protected]
// ---------------------------------------------------------------------------
//	Overriden to make the attribute title bold if this is a top-level
//	container.

void
PIContainerItem::GetDrawContentsSelf(
	const STableCell&		inCell,
	SOutlineDrawContents&	ioDrawContents)
{

	ValidateThis_();

	// Use default behavior for getting value.

	PIAttributeItem::GetDrawContentsSelf(inCell, ioDrawContents);

	// Make the attribute title bold if this is a top-level container.

	if ((inCell.col == 1) && (mSuperItem == nil))
		ioDrawContents.outTextTraits.style |= bold;

}


// ---------------------------------------------------------------------------
//		* DrawRowAdornments										[protected]
// ---------------------------------------------------------------------------
//	If this is the top level container, shade row.
//	Otherwise use typical adornment.

void
PIContainerItem::DrawRowAdornments(
	const Rect&	inLocalRowRect)
{

	ValidateThis_();

	if (mSuperItem == nil) {
		Rect rowRect = inLocalRowRect;
		rowRect.bottom++;
#if Constructor_ForPilot
		ShadeRow(UGAColorRamp::GetColor(3), rowRect);
#else
		ShadeRow(UGAColorRamp::GetColor(2), rowRect);
#endif
		OVItem::DrawRowAdornments(inLocalRowRect);
	}
	else
		PIAttributeItem::DrawRowAdornments(inLocalRowRect);
}


// ---------------------------------------------------------------------------
//		* SetElementSelf										[protected]
// ---------------------------------------------------------------------------

void
PIContainerItem::SetElementSelf(
	DMElement*	inElement)
{
	ValidateThis_();
	ValidateObject_(inElement);

	PIAttributeItem::SetElementSelf(inElement);
	SetContainer(inElement);
}


// ---------------------------------------------------------------------------
//		* DoubleClick											[protected]
// ---------------------------------------------------------------------------
//	Expand or collapse the container.

void
PIContainerItem::DoubleClick(
	const STableCell&			/* inCell */,
	const SMouseDownEvent&		/* inMouseDown */,
	const SOutlineDrawContents&	/* inDrawContents */,
	Boolean						/* inHitText */)
{
	ValidateThis_();
	
	if (IsExpanded())
		Collapse();
	else
		Expand();
}
