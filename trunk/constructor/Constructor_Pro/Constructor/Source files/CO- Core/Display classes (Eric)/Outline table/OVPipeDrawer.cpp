// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	OVPipeDrawer.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/21/96
//     $Date: 2006/01/18 01:32:44 $
//  $History: OVPipeDrawer.cpp $
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:36
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Outline table
//	Improved commenting.
//	
//	*****************  Version 6  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:05p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:24
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Outline table
//	Fixed CR/LF problem
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 12/07/96   Time: 11:20
//	Updated in $/Constructor/Source files/Display classes (Eric)/Outline table
//	Updated toRobin's drop 12/06/96.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/24/96   Time: 21:45
//	Updated in $/Constructor/Source files/Display classes (Eric)/Outline table
//	Finished implementation of pipe drawing.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/22/96   Time: 08:59
//	Updated in $/Constructor/Source files/Display classes (Eric)/Outline table
//	Fixed a missing class declaration.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/22/96   Time: 08:57
//	Created in $/Constructor/Source files/Display classes (Eric)/Outline table
//	Added class.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "OVPipeDrawer.h"

	// Core : Display classes (Eric) : Outline table
#include "OVTable.h"

	// PowerPlant : CA : Array classes
#include <LFastArrayIterator.h>

	// PowerPlant : CA : Outline table
#include <LOutlineItem.h>

	// PowerPlant : GA : Grayscale utilities
#include <UGAColorRamp.h>


// ===========================================================================

#pragma mark *** OVPipeDrawer ***

// ---------------------------------------------------------------------------
//		* OVPipeDrawer(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor

OVPipeDrawer::OVPipeDrawer(
	LStream*	inStream)

: LPane(inStream)

{
	mOVTable = nil;
}


// ---------------------------------------------------------------------------
//		* ~OVPipeDrawer
// ---------------------------------------------------------------------------
//	Destructor

OVPipeDrawer::~OVPipeDrawer()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** drawing behaviors

// ---------------------------------------------------------------------------
//		* DrawSelf												[protected]
// ---------------------------------------------------------------------------

void
OVPipeDrawer::DrawSelf()
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mOVTable);
	
	// Find the selection for this table.

	if (mSelection == nil) {
		mSelection = mOVTable->GetSelection();
		if (mSelection != nil) {
			ValidateObject_(mSelection.GetObject());
			mSelection->AddListener(this);
		}
	}

	// Set up a device loop to draw the pipes.
	
	try {
		Rect frame;
		CalcLocalFrameRect(frame);
	
		StDeviceLoop device(frame);
		SInt16 depth;
	
		while (device.NextDepth(depth)) {
			StColorPenState::Normalize();
			DrawPipes(depth, true);
		}
	}
	catch(...) {
	}

}


// ---------------------------------------------------------------------------
//		* DrawPipes												[protected]
// ---------------------------------------------------------------------------
//	Called from the inner loop of DrawSelf. Draws the inheritance pipes
//	for a given pixel depth.

void
OVPipeDrawer::DrawPipes(
	SInt16		inDepth,
	Boolean		inShowSelection)
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mOVTable);
	
	// Set up variables for iteration.
	
	UInt32 pipeVector = 0;
	UInt32 darkPipeVector = 0;
	Boolean selectParent = false;
	UInt16 nestingLevel = 0;
	Boolean	hitVisibleArea = false;
	
	// Iterate backwards through items.
	
	LFastArrayIterator iter(mOVTable->GetItems(), LArrayIterator::from_End);
	LOutlineItem* item;
	
	while (iter.Previous(&item)) {

		// See if we're in the update area yet.
		
		STableCell cell(iter.GetCurrentIndex(), 1);
		Rect cellFrame;
		
		if (mOVTable->GetLocalCellRect(cell, cellFrame))
			hitVisibleArea = true;
		else if (hitVisibleArea)
			return;

		// Check nesting level.
	
		UInt16 newNestingLevel = item->GetIndentDepth();
		if (newNestingLevel < nestingLevel) {

			// We're dropping down a level. Clear the pipe and dark-pipe
			// flags for this level

			pipeVector &= ~(1 << nestingLevel);
			if (darkPipeVector & (1 << nestingLevel))
				selectParent = true;
			darkPipeVector &= ~(1 << nestingLevel);

		}
		else if (newNestingLevel > nestingLevel) {
		
			// We're going up a level (or more). Update the pipe horizontal centers array.
			
			UpdatePipeCenters(item, nestingLevel);
		
		}
		
		nestingLevel = newNestingLevel;
		
		// See if this item is selected.
		
		Boolean isSelected = inShowSelection && (selectParent || mOVTable->CellIsSelected(cell));
		selectParent = false;
	
		// Draw pipes for each level.

		if (hitVisibleArea) {
			SInt16 nest = nestingLevel;
			SInt16 left = item->GetLeftEdge();

			while (nest >= 2) {
				if (nest == nestingLevel) {
					DrawPipeSegment(cellFrame,								// inCellFrame
									left,									// inItemLeftEdge
									inDepth,								// inDepth
									nest,									// inNestingDepth,
									true,									// inSideStick
									isSelected,								// inSideStickDark
									(pipeVector & (1 << nest)) != 0,		// inBottomStick
									(darkPipeVector & (1 << nest)) != 0);	// inBottomStickDark
				}
				else {
					DrawPipeSegment(cellFrame,								// inCellFrame
									left,									// inItemLeftEdge
									inDepth,								// inDepth
									nest,									// inNestingDepth,
									false,									// inSideStick
									false,									// inSideStickDark
									(pipeVector & (1 << nest)) != 0,		// inBottomStick
									(darkPipeVector & (1 << nest)) != 0);	// inBottomStickDark
				}
				nest--;
			}
		}

		// Make sure a pipe gets drawn for this level if we go up in the hierarchy later.
		
		pipeVector |= 1 << nestingLevel;
		if (isSelected)
			darkPipeVector |= 1 << nestingLevel;

	}
}


// ---------------------------------------------------------------------------
//		* DrawPipeSegment										[protected]
// ---------------------------------------------------------------------------
//	Called by DrawPipes. Given a cell frame and information about the
//	containment hierarchy at this cell, draw the cell's pipes.

void
OVPipeDrawer::DrawPipeSegment(
	const Rect&		inCellFrame,
	SInt16			inItemLeftEdge,
	SInt16			inDepth,
	SInt16			inNestingDepth,
	Boolean			inSideStick,
	Boolean			inSideStickDark,
	Boolean			inBottomStick,
	Boolean			inBottomStickDark)
{

	// If neither bottom nor side exists, donÕt draw anything.
	
	if (!(inSideStick || inBottomStick))
		return;

	// Calculate horizontal pipe positions.

	SInt16 right = inItemLeftEdge - 4;
	SInt16 centerH = mPipeCenters[inNestingDepth];
	
	// Calculate vertical pipe positions.
	
	SInt16 centerV = (inCellFrame.top + inCellFrame.bottom) >> 1;
	SInt16 top = inCellFrame.top;
	SInt16 bottom = inCellFrame.bottom - 1;

	// Draw bottom segment.

	StColorPenState pen;
	if (inBottomStick) {
		SetPenColor(inDepth, 1, inBottomStickDark);
		::MoveTo(centerH - 1, centerV + 2);
		::LineTo(centerH - 1, bottom);
		SetPenColor(inDepth, 2, inBottomStickDark);
		::MoveTo(centerH, centerV + 2);
		::LineTo(centerH, bottom);
		SetPenColor(inDepth, 3, inBottomStickDark);
		::MoveTo(centerH + 1, centerV + 2);
		::LineTo(centerH + 1, bottom);
	}
	
	// Draw side segment.
	
	if (inSideStick) {
		SetPenColor(inDepth, 1, inSideStickDark);
		::MoveTo(centerH + 2, centerV - 1);
		::LineTo(right, centerV - 1);
		SetPenColor(inDepth, 2, inSideStickDark);
		::MoveTo(centerH + 1, centerV);
		::LineTo(right, centerV);
		SetPenColor(inDepth, 3, inSideStickDark);
		::MoveTo(inBottomStick ? centerH + 2 : centerH - 1, centerV + 1);
		::LineTo(right, centerV + 1);
	}

	// Draw top segment.

	Boolean topStickDark = (inSideStick && inSideStickDark) || (inBottomStick && inBottomStickDark);
	SetPenColor(inDepth, 1, topStickDark);
	::MoveTo(centerH - 1, inBottomStick ? centerV + 1 : centerV);
	::LineTo(centerH - 1, top);
	SetPenColor(inDepth, 2, topStickDark);
	::MoveTo(centerH, inBottomStick ? centerV + 1 : centerV);
	::LineTo(centerH, top);
	SetPenColor(inDepth, 3, topStickDark);
	::MoveTo(centerH + 1, inSideStick ? centerV - 1 : centerV + 1);
	::LineTo(centerH + 1, top);
	if (inSideStick && inBottomStick) {
		::MoveTo(centerH + 1, centerV + 1);
		::LineTo(centerH + 1, centerV + 1);
	}
}


// ---------------------------------------------------------------------------
//		* SetPenColor											[protected]
// ---------------------------------------------------------------------------
//	Set up the pen to draw a single pipe segment. The particular color and
//	pen pattern chosen depend on the pixel depth, color selector, and whether
//	the pipe segment is selected.

void
OVPipeDrawer::SetPenColor(
	SInt32		inPixelDepth,
	SInt32		inColorSelector,
	Boolean		inDarken)
{

	if (inPixelDepth >= 4) {

		// We have colors. Use them.
		
		RGBColor theColor;
		switch (inColorSelector) {
			case 1:
				inDarken ? theColor = UGAColorRamp::GetColor(10) : theColor = UGAColorRamp::GetColor(4);
				::RGBForeColor(&theColor);
				break;
			case 2:
				inDarken ? theColor = UGAColorRamp::GetColor(12) : theColor = UGAColorRamp::GetColor(7);
				::RGBForeColor(&theColor);
				break;
			case 3:
				inDarken ? theColor = UGAColorRamp::GetBlackColor() : theColor = UGAColorRamp::GetColor(10);
				::RGBForeColor(&theColor);
				break;
		}

	}
	else {
	
		// Black & white or 4-color display. Use pen patterns rather than colors.
		
		Pattern thePattern;
		
		if (inDarken)
			UQDGlobals::GetBlackPat(&thePattern);
		else
			UQDGlobals::GetGrayPat(&thePattern);
		
		::PenPat(&thePattern);
	}
}


// ---------------------------------------------------------------------------
//		* UpdatePipeCenters										[protected]
// ---------------------------------------------------------------------------
//	Called by DrawPipes when it discovers that the pixel depth has changed.
//	Recalculate the horizontal center of the pipes at each nesting depth.

void
OVPipeDrawer::UpdatePipeCenters(
	LOutlineItem*	inItem,
	SInt16			inStopDepth)
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inItem);
	
	// Loop through item's superitems.
	
	LOutlineItem* item = inItem->GetSuperItem();
	while (item != nil) {
	
		// Make sure we're pointing to a valid item.
		
		ValidateObject_(item);
		
		// Stop if we've reached the previously-known depth.

		SInt16 depth = item->GetIndentDepth();
		if (depth < inStopDepth)
			break;
	
		// Record the horizontal center of the icon at this depth.
	
		mPipeCenters[depth + 1] = item->GetLeftEdge() + 8;
		
		// Move to item's superitem.
		
		item = item->GetSuperItem();
	
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** selection refresh

// ---------------------------------------------------------------------------
//		* SelectionChanged										[protected]
// ---------------------------------------------------------------------------
//	When the selection changes, trigger a latent redraw. This is kind of
//	the lazy way out of this situation, but it seemed easier than calculating
//	the region which needed to be redrawn.

void
OVPipeDrawer::SelectionChanged(
	DM_SelectionChanged*	inMessage)
{
	
	// Validate pointers.
	
	ValidateThis_();
	
	// Make sure it's our selection that's changing.
	
	if (mSelection != inMessage->mSelection)
		return;
	
	// Trigger a redraw later.
	
	StartRepeating();

}


// ---------------------------------------------------------------------------
//		* SpendTime												[protected]
// ---------------------------------------------------------------------------
//	Used by SelectionChanged. Redraw the pipes, then turn off repeating.

void
OVPipeDrawer::SpendTime(
	const EventRecord&	/* inMacEvent */)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Stop repeating. Once through is enough.
	
	StopRepeating();
	
	// Redraw selection. This is the simple, brute-force approach.
	
	Draw(nil);

}


// ===========================================================================

#pragma mark -
#pragma mark ** configuration

// ---------------------------------------------------------------------------
//		* FinishCreateSelf										[protected]
// ---------------------------------------------------------------------------
//	Find the outline table that we're drawing inside.

void
OVPipeDrawer::FinishCreateSelf()
{

	// Validate pointers.
	
	ValidateThis_();

	// Find the outline table we're drawing with. It should be our superview.
	
	mOVTable = (dynamic_cast<OVTable*>(GetSuperView()));
	ValidateObject_(mOVTable);
	
}
