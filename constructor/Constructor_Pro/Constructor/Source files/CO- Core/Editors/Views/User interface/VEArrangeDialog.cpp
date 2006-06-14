// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VEArrangeDialog.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 03/22/96
//     $Date: 2006/04/12 08:48:13 $
//	$History: VEArrangeDialog.cpp $
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 02/20/97   Time: 18:26
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Improved commenting.
//	
//	*****************  Version 9  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:07p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:30
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Fixed CR/LF problem
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 12/12/96   Time: 14:59
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Fixed stray preferences resource. (Bug fix #1072.) Improved commenting.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 11/24/96   Time: 22:50
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Fixed a bug in FinishCreateSelf. It wasn't calling
//	inherited::FinishCreateSelf, which meant that the Apply button didn't
//	get the default button indicator.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 11/19/96   Time: 14:11
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Fixed some bugs with edit fields and preview drawing.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/16/96   Time: 23:02
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Moved VE menu command numbers to VEMenuResources.h.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/13/96   Time: 13:36
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor
//	Rearranged view editor classes.
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/31/96   Time: 18:00
//	Checked in '$/Constructor/Source files/Editors/Views/+ Generic view editor'
//	Comment: Updated for latest GA classes.
//
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:59
//	Created
//	Comment: Baseline source 15 October 1996.
//	
//	96.08.05	rtm		Changed superclass to GADialogBox so that we get the correct
//						behavior with the new pushbuttons, also needed to tweak some
//						places where certain claases were being referenced that were
//						no longer being used.
//
//	96.04.03	rtm		Added support for the querying for distribute and equidistant
//						centers values to ve used for the preview. Thanks Eric for
//						once again solving the mysteries of void* parameters.
//
//	96.04.03	rtm		Added support for a distribute on equidistant centers option
//						and the associated preview of this option.
//
//	96.04.01	rtm		Integrated Eric's GAEditField for use as the spread
//						value fields.
//
//	96.03.29	rtm		Got rid of the fields that held the values for the spreading,
//						we now just use the edit field as the storage and set and
//						extract directly from there.
//
//	96.03.26	rtm		Starting implementing the handling of the preview and its
//						rendering.
//
//	96.03.22	rtm		Initial creation of file
//
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "VEArrangeDialog.h"

	// Core : Application
#include "CAPreferencesFile.h"

	// Core : Editors : Views : User interface
#include "VEMenuResources.h"

	// PowerPlant : Utility classes
#include <UResourceMgr.h>

	// PowerPlant : GA : General utility classes
#include <UGraphicsUtilities.h>

#include <LBevelButton.h>
#include <LEditText.h>


// ===========================================================================
//		* Pane IDs
// ===========================================================================

const PaneIDT	PaneID_Preview				= 'prev';
const PaneIDT	PaneID_Sample1				= 'smp1';
const PaneIDT	PaneID_Sample2				= 'smp2';
const PaneIDT	PaneID_Sample3				= 'smp3';

const ResType	ResType_ArrangementData		= 'Arrg'; // GHD: Changed from 'arrg'
const ResIDT	ResID_ArrangementData		= 1000;


// ===========================================================================

#pragma mark *** VEArrangeDialog ***

// ---------------------------------------------------------------------------
//		* VEArrangeDialog(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor

VEArrangeDialog::VEArrangeDialog(
	LStream*	inStream)

: LDialogBox(inStream)

{

	// Read data.
	SInt32 temp;
	*inStream >> temp;
	temp = mHorizontalAlignment;
	*inStream >> temp;
	temp = mVerticalAlignment;
	
	// Clear variables. Most private data is set up in FinishCreateSelf.

	mPreview = nil;					
	mHorizSpreadValue = nil;		
	mVertSpreadValue = nil;			
	mSample1 = nil;
	mSample2 = nil;
	mSample3 = nil;

}


// ---------------------------------------------------------------------------
//		* ~VEArrangeDialog
// ---------------------------------------------------------------------------
//	Destructor

VEArrangeDialog::~VEArrangeDialog()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** accessors

// ---------------------------------------------------------------------------
//		* SetHorizontalAlignment
// ---------------------------------------------------------------------------
//	Change the horizontal alignment setting.

void		
VEArrangeDialog::SetHorizontalAlignment(
	EAlignHorizontal 	inAlign)
{
	mHorizontalAlignment = inAlign;
}


// ---------------------------------------------------------------------------
//		* SetVerticalAlignment
// ---------------------------------------------------------------------------
//	Change the vertical alignment setting.

void		
VEArrangeDialog::SetVerticalAlignment(
	EAlignVertical		inAlign)
{
	mVerticalAlignment = inAlign;
}


// ---------------------------------------------------------------------------
//		* GetHorizontalSpread
// ---------------------------------------------------------------------------
//	Get the horizontal distance to be established between objects.
//	This is done by asking the edit field for its value.

SInt16		
VEArrangeDialog::GetHorizontalSpread() const
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_((const_cast<VEArrangeDialog*>(this))->mHorizSpreadValue);
	
	// Get the value from the edit field.

	return mHorizSpreadValue->GetValue();

}

	
// ---------------------------------------------------------------------------
//		* SetHorizontalSpread
// ---------------------------------------------------------------------------
//	Set the horizontal distance to be established between objects.
//	This is done by telling the edit field to change its value.

void		
VEArrangeDialog::SetHorizontalSpread(
	SInt16	inSpread)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mHorizSpreadValue);
	
	// Set edit field's value.
	
	if (mHorizSpreadValue->GetValue() != inSpread)
		mHorizSpreadValue->SetValue(inSpread);
	
}

	
// ---------------------------------------------------------------------------
//		* GetVerticalSpread
// ---------------------------------------------------------------------------
//	Get the vertical distance to be established between objects.
//	This is done by asking the edit field for its value.

SInt16		
VEArrangeDialog::GetVerticalSpread() const
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_((const_cast<VEArrangeDialog*>(this))->mVertSpreadValue);
	
	// Get the value from the edit field.

	return mVertSpreadValue->GetValue();

}

	
// ---------------------------------------------------------------------------
//		* SetVerticalSpread
// ---------------------------------------------------------------------------
//	Set the vertical distance to be established between objects.
//	This is done by telling the edit field to change its value.

void		
VEArrangeDialog::SetVerticalSpread(
	SInt16	inSpread)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mVertSpreadValue);
	
	// Set edit field's value.
	
	if (mVertSpreadValue->GetValue() != inSpread)
		mVertSpreadValue->SetValue(inSpread);
	
}
	

// ---------------------------------------------------------------------------
//		* GetArrangementData
// ---------------------------------------------------------------------------
//	Return a complete description of the proposed alignment changes.

void		
VEArrangeDialog::GetArrangementData(
	SAlignmentData&		outAlignData) const
{

	// Validate pointers.
	
	ValidateThis_();
	ThrowIfNil_(&outAlignData);
	
	// Fill in the structure.

	outAlignData.alignHorizontal = GetHorizontalAlignment();
	outAlignData.horizSpread = GetHorizontalSpread();
	outAlignData.alignVertical = GetVerticalAlignment();
	outAlignData.vertSpread = GetVerticalSpread();

}


// ---------------------------------------------------------------------------
//		* SetArrangementData
// ---------------------------------------------------------------------------
//	Configure the dialog box so that it looks like the alignment data
//	contained in this structure.

void		
VEArrangeDialog::SetArrangementData(
	const SAlignmentData&	inAlignData)
{

	// Validate pointers.
	
	ValidateThis_();

	// Set the spread value if there is one.

	SetHorizontalSpread(inAlignData.horizSpread);

	// Set the vertical spread value.
	
	SetVerticalSpread(inAlignData.vertSpread);
	
	// Set up the horizontal alignment and get the
	// associated button turned on.

	LBevelButton* theButton = nil;

	SetHorizontalAlignment(inAlignData.alignHorizontal);

	theButton = (dynamic_cast<LBevelButton*>(FindPaneByID(inAlignData.alignHorizontal)));
	ValidateObject_(theButton);
	theButton->SetValue(0);
	theButton->SetValue(1);
	
	// Set up the vertical alignment and get the
	// associated button turned on.
	
	SetVerticalAlignment(inAlignData.alignVertical);
	
	theButton = (dynamic_cast<LBevelButton*>(FindPaneByID(inAlignData.alignVertical)));
	ValidateObject_(theButton);
	theButton->SetValue(0);
	theButton->SetValue(1);
	
}


// ---------------------------------------------------------------------------
//		* SaveArrangementData
// ---------------------------------------------------------------------------
//	Write the arrangement configuration to the preferences file.

void
VEArrangeDialog::SaveArrangementData()
{

	// Validate pointers.
	
	ValidateThis_();

	// Read the current alignment configuration.
	
	SAlignmentData arrangeData;
	GetArrangementData(arrangeData);
	
	// Create the new resource which will contain the arrangement info.
	
	StPreferencesContext prefsContext;
	if (prefsContext.IsValid()) {					//* 2.4a2: BUG FIX #1072: added if statement

		StNewResource newResource(	ResType_ArrangementData,
									ResID_ArrangementData,
									sizeof (SAlignmentData),
									true);
	
		// Copy the resource data.
		
		::BlockMoveData(&arrangeData, *((Handle) newResource), sizeof (SAlignmentData));
	
		// Now write the resource out to the preferences file.
	
		newResource.Write(true);
	
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** drawing behaviors

// ---------------------------------------------------------------------------
//		* DrawSelf												[protected]
// ---------------------------------------------------------------------------
//	Make sure the preview elements are properly placed.

void		
VEArrangeDialog::DrawSelf ()
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mPreview);
	ValidateObject_(mSample1);
	ValidateObject_(mSample2);
	ValidateObject_(mSample3);

	// Make sure the window still gets to do its thing.

	LDialogBox::DrawSelf();

	// See if we have any settings at all.
	// If we don't, we hide the samples; otherwise, we show them.
	
	if ((mHorizontalAlignment == alignHoriz_None) && (mVerticalAlignment == alignVert_None)) {
		mSample1->Hide();
		mSample2->Hide();
		mSample3->Hide();
	}
	else {
		mSample1->Show();
		mSample2->Show();
		mSample3->Show();
	}
	
	// Reset drawing system.

	StColorPenState::Normalize();
	
	// Handle the horizontal alignment.

	DrawHorizontalArrangement();
	
	// Handle the vertical alignment.

	DrawVerticalArrangement();
	
	// Flush the buffer so the red alignment lines are shown.
	FlushPortBuffer();
}


// ---------------------------------------------------------------------------
//		* DrawHorizontalArrangement								[protected]
// ---------------------------------------------------------------------------
//	Be sure sample panes are properly placed for horizontal alignment.
//	Draw guide lines if appropriate.

void
VEArrangeDialog::DrawHorizontalArrangement()
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mPreview);
	ValidateObject_(mSample1);
	ValidateObject_(mSample2);
	ValidateObject_(mSample3);

	// Find the content rectangle within which we will do all the drawing.

	Rect drawRect;
	mPreview->CalcLocalFrameRect(drawRect);
	::InsetRect(&drawRect, 6, 0);

	// Find the location and size of each sample pane.

	Rect sampleFrame1;
	Rect sampleFrame2;
	Rect sampleFrame3;
	SPoint32 localPoint1, localPoint2;
	
	mSample1->CalcLocalFrameRect(sampleFrame1);
	mSample1->GetFrameLocation(localPoint1);
	(mSample1->GetSuperView())->GetFrameLocation(localPoint2);
	sampleFrame1.left = localPoint1.h - localPoint2.h;
	sampleFrame1.top = localPoint1.v - localPoint2.v;
	sampleFrame1.right += sampleFrame1.left;
	sampleFrame1.bottom += sampleFrame1.top;
	
	mSample2->CalcLocalFrameRect(sampleFrame2);
	mSample2->GetFrameLocation(localPoint1);
	(mSample2->GetSuperView())->GetFrameLocation(localPoint2);
	sampleFrame2.left = localPoint1.h - localPoint2.h;
	sampleFrame2.top = localPoint1.v - localPoint2.v;
	sampleFrame2.right += sampleFrame2.left;
	sampleFrame2.bottom += sampleFrame2.top;
	
	mSample3->CalcLocalFrameRect(sampleFrame3);
	mSample3->GetFrameLocation(localPoint1);
	(mSample3->GetSuperView())->GetFrameLocation(localPoint2);
	sampleFrame3.left = localPoint1.h - localPoint2.h;
	sampleFrame3.top = localPoint1.v - localPoint2.v;
	sampleFrame3.right += sampleFrame3.left;
	sampleFrame3.bottom += sampleFrame3.top;
	
	SDimension16 sampleSize1;
	SDimension16 sampleSize2;
	SDimension16 sampleSize3;

	mSample1->GetFrameSize(sampleSize1);
	mSample2->GetFrameSize(sampleSize2);
	mSample3->GetFrameSize(sampleSize3);

	// Select the way we draw horizontal alignment.

	const RGBColor redLine = { 65535, 0, 0 };

	switch (mHorizontalAlignment) {
		
		// No alignment: Restore original position of sample panes.
	
		case alignHoriz_None:
			mSample1->PlaceInSuperFrameAt(mSample1Loc.h, sampleFrame1.top, false);
			mSample2->PlaceInSuperFrameAt(mSample2Loc.h, sampleFrame2.top, false);
			mSample3->PlaceInSuperFrameAt(mSample3Loc.h, sampleFrame3.top, false);
			break;
		
		// Left align: Move sample panes to left edge & draw guide line.

		case alignHoriz_AlignLeft:
			mSample1->PlaceInSuperFrameAt(drawRect.left, sampleFrame1.top, false);
			mSample2->PlaceInSuperFrameAt(drawRect.left, sampleFrame2.top, false);
			mSample3->PlaceInSuperFrameAt(drawRect.left, sampleFrame3.top, false);
			
			mPreview->FocusDraw();
			::RGBForeColor(&redLine);
			::MoveTo(drawRect.left, drawRect.top);
			::LineTo(drawRect.left, drawRect.bottom - 1);

			break;
			
		// Center align: Move sample panes to middle & draw guide line.
		
		case alignHoriz_AlignVerticalCenters: {
			SInt32 center = (drawRect.right + drawRect.left) / 2;
			
			mSample1->PlaceInSuperFrameAt(center - (sampleSize1.width / 2), sampleFrame1.top, false);
			mSample2->PlaceInSuperFrameAt(center - (sampleSize2.width / 2), sampleFrame2.top, false);
			mSample3->PlaceInSuperFrameAt(center - (sampleSize3.width / 2), sampleFrame3.top, false);
			
			mPreview->FocusDraw();
			::RGBForeColor(&redLine);
			::MoveTo(center, drawRect.top);
			::LineTo(center, drawRect.bottom - 1);

			break;
		}

		// Right align: Move sample panes to right edge & draw guide line.
		
		case alignHoriz_AlignRight:
			mSample1->PlaceInSuperFrameAt(drawRect.right - sampleSize1.width, sampleFrame1.top, false);
			mSample2->PlaceInSuperFrameAt(drawRect.right - sampleSize2.width, sampleFrame2.top, false);
			mSample3->PlaceInSuperFrameAt(drawRect.right - sampleSize3.width, sampleFrame3.top, false);
		
			mPreview->FocusDraw();
			::RGBForeColor(&redLine);
			::MoveTo(drawRect.right - 1, drawRect.top);
			::LineTo(drawRect.right - 1, drawRect.bottom - 1);

			break;
		
		// Distribute panes: Move panes as indicated.
		
		case alignHoriz_DistributeHorizontal:
		case alignHoriz_DistributeHorizInContainer:
		case alignHoriz_UserDefinedSpread: {
			
			// Find the spread amount.
			
			SInt32 spread = GetHorizontalSpread();
			
			// Find the maximum spread amount that we can display.
			
			SInt32 maxSpread = (drawRect.right - drawRect.left);
			maxSpread -= sampleSize1.width;
			maxSpread -= sampleSize2.width;
			maxSpread -= sampleSize3.width;
			maxSpread /= 2;
			
			// Truncate spread display to fit on screen.
			
			if (spread > maxSpread)
				spread = maxSpread;			
			
			// Spread in container should always fill container.
			
			if (mHorizontalAlignment == alignHoriz_DistributeHorizInContainer)
				spread = maxSpread;

			// Set placement of panes.

			SInt32 leftEdge = drawRect.left;

			mSample1->PlaceInSuperFrameAt(leftEdge, sampleFrame1.top, false);
			leftEdge += sampleSize1.width + spread;
			
			mSample2->PlaceInSuperFrameAt(leftEdge, sampleFrame2.top, false);
			leftEdge += sampleSize2.width + spread;

			mSample3->PlaceInSuperFrameAt(leftEdge, sampleFrame3.top, false);
						
			// Draw both edges to signify spread in container.
			
			if (mHorizontalAlignment == alignHoriz_DistributeHorizInContainer) {
				::RGBForeColor(&redLine);
				mPreview->FocusDraw();

				::MoveTo(drawRect.left, drawRect.top);
				::LineTo(drawRect.left, drawRect.bottom - 1);
				::MoveTo(drawRect.right, drawRect.top);
				::LineTo(drawRect.right, drawRect.bottom - 1);
			}

			break;
		
		default:
			break;
		}
	}
}


// ---------------------------------------------------------------------------
//		* DrawVerticalArrangement								[protected]
// ---------------------------------------------------------------------------
//	Be sure sample panes are properly placed for vertical alignment.
//	Draw guide lines if appropriate.

void
VEArrangeDialog::DrawVerticalArrangement()
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mPreview);
	ValidateObject_(mSample1);
	ValidateObject_(mSample2);
	ValidateObject_(mSample3);

	// Find the content rectangle within which we will do all the drawing.

	Rect drawRect;
	mPreview->CalcLocalFrameRect(drawRect);
	::InsetRect(&drawRect, 0, 6);
	 
	// Find the location and size of each sample pane.

	Rect sampleFrame1;
	Rect sampleFrame2;
	Rect sampleFrame3;
	SPoint32 localPoint1, localPoint2;
	
	mSample1->CalcLocalFrameRect(sampleFrame1);
	mSample1->GetFrameLocation(localPoint1);
	(mSample1->GetSuperView())->GetFrameLocation(localPoint2);
	sampleFrame1.left = localPoint1.h - localPoint2.h;
	sampleFrame1.top = localPoint1.v - localPoint2.v;
	sampleFrame1.right += sampleFrame1.left;
	sampleFrame1.bottom += sampleFrame1.top;
	
	mSample2->CalcLocalFrameRect(sampleFrame2);
	mSample2->GetFrameLocation(localPoint1);
	(mSample2->GetSuperView())->GetFrameLocation(localPoint2);
	sampleFrame2.left = localPoint1.h - localPoint2.h;
	sampleFrame2.top = localPoint1.v - localPoint2.v;
	sampleFrame2.right += sampleFrame2.left;
	sampleFrame2.bottom += sampleFrame2.top;
	
	mSample3->CalcLocalFrameRect(sampleFrame3);
	mSample3->GetFrameLocation(localPoint1);
	(mSample3->GetSuperView())->GetFrameLocation(localPoint2);
	sampleFrame3.left = localPoint1.h - localPoint2.h;
	sampleFrame3.top = localPoint1.v - localPoint2.v;
	sampleFrame3.right += sampleFrame3.left;
	sampleFrame3.bottom += sampleFrame3.top;
	
	SDimension16 sampleSize1;
	SDimension16 sampleSize2;
	SDimension16 sampleSize3;

	mSample1->GetFrameSize(sampleSize1);
	mSample2->GetFrameSize(sampleSize2);
	mSample3->GetFrameSize(sampleSize3);

	// Select the way we draw vertical alignment.

	const RGBColor redLine = { 65535, 0, 0 };

	switch (mVerticalAlignment) {

		// No alignment: Restore original position of sample panes.

		case alignVert_None:
			mSample1->PlaceInSuperFrameAt(sampleFrame1.left, mSample1Loc.v, false);
			mSample2->PlaceInSuperFrameAt(sampleFrame2.left, mSample2Loc.v, false);
			mSample3->PlaceInSuperFrameAt(sampleFrame3.left, mSample3Loc.v, false);
			break;

		// Top align: Move sample panes to top edge & draw guide line.
		
		case alignVert_AlignTop:
			mSample1->PlaceInSuperFrameAt(sampleFrame1.left, drawRect.top, false);
			mSample2->PlaceInSuperFrameAt(sampleFrame2.left, drawRect.top, false);
			mSample3->PlaceInSuperFrameAt(sampleFrame3.left, drawRect.top, false);
			
			mPreview->FocusDraw();
			::RGBForeColor(&redLine);
			::MoveTo(drawRect.left, drawRect.top);
			::LineTo(drawRect.right - 1, drawRect.top);

			break;
		
		// Center align: Move sample panes to center & draw guide line.
		
		case alignVert_AlignHorizontalCenters:
			SInt32 center;
			center = (drawRect.top + drawRect.bottom) / 2;

			mSample1->PlaceInSuperFrameAt(sampleFrame1.left, center - (sampleSize1.height / 2), false);
			mSample2->PlaceInSuperFrameAt(sampleFrame2.left, center - (sampleSize2.height / 2), false);
			mSample3->PlaceInSuperFrameAt(sampleFrame3.left, center - (sampleSize3.height / 2), false);

			mPreview->FocusDraw();
			::RGBForeColor(&redLine);
			::MoveTo(drawRect.left, center);
			::LineTo(drawRect.right - 1, center);

			break;
		
		// Bottom align: Move sample panes to bottom edge & draw guide line.
		
		case alignVert_AlignBottom:
			mSample1->PlaceInSuperFrameAt(sampleFrame1.left, drawRect.bottom - sampleSize1.height, false);
			mSample2->PlaceInSuperFrameAt(sampleFrame2.left, drawRect.bottom - sampleSize2.height, false);
			mSample3->PlaceInSuperFrameAt(sampleFrame3.left, drawRect.bottom - sampleSize3.height, false);
			
			mPreview->FocusDraw();
			::RGBForeColor(&redLine);
			::MoveTo(drawRect.left, drawRect.bottom - 1);
			::LineTo(drawRect.right - 1, drawRect.bottom - 1);

			break;

		case alignVert_DistributeVertical:
		case alignVert_DistributeVertInContainer:
		case alignVert_UserDefinedSpread: {

			// Find the spread amount.
			
			SInt32 spread = GetVerticalSpread();
			
			// Find the maximum spread amount that we can display.
			
			SInt32 maxSpread = (drawRect.bottom - drawRect.top);
			maxSpread -= sampleSize1.height;
			maxSpread -= sampleSize2.height;
			maxSpread -= sampleSize3.height;
			maxSpread /= 2;
			
			// Truncate spread display to fit on screen.
			
			if (spread > maxSpread)
				spread = maxSpread;			
			
			// Spread in container should always fill container.
			
			if (mVerticalAlignment == alignVert_DistributeVertInContainer)
				spread = maxSpread;

			// Set placement of panes.

			SInt32 topEdge = drawRect.top;

			mSample1->PlaceInSuperFrameAt(sampleFrame1.left, topEdge, false);
			topEdge += sampleSize1.height + spread;
			
			mSample2->PlaceInSuperFrameAt(sampleFrame2.left, topEdge, false);
			topEdge += sampleSize2.height + spread;

			mSample3->PlaceInSuperFrameAt(sampleFrame3.left, topEdge, false);

			// Draw both lines to signify distribute in container.
			
			if (mVerticalAlignment == alignVert_DistributeVertInContainer) {
				::RGBForeColor(&redLine);
				mPreview->FocusDraw();

				::MoveTo(drawRect.left, drawRect.top);
				::LineTo(drawRect.right - 1, drawRect.top);
				::MoveTo(drawRect.left, drawRect.bottom - 1);
				::LineTo(drawRect.right - 1, drawRect.bottom - 1);
			}

			break;
		
		default:
			break;
		}
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** preview setup

// ---------------------------------------------------------------------------
//		* ListenToMessage										[protected]
// ---------------------------------------------------------------------------
//	Messages come from the alignment buttons. Use this to trigger a redraw
//	of the preview.

void
VEArrangeDialog::ListenToMessage(
	MessageT	inMessage,
	void*		ioParam)
{

	// Validate pointers.
	
	ValidateThis_();

	// Decode the message.
	
	SInt32 buttonValue;
	switch (inMessage) {
	
		// Horizontal flush alignment: Disable spread amount field.

		case alignHoriz_None:
		case alignHoriz_AlignLeft:
		case alignHoriz_AlignVerticalCenters:
		case alignHoriz_AlignRight:
			buttonValue = *((SInt32*) ioParam);
			if (buttonValue == 1) {
				mHorizontalAlignment = (EAlignHorizontal) inMessage;
				ValidateObject_(mHorizSpreadValue);
				mHorizSpreadValue->SetDescriptor( (StringPtr) "" );
				mHorizSpreadValue->Disable();
			}
			break;
		
		// Distribute horizontal: Find horizontal spread amount.
		
		case alignHoriz_DistributeHorizontal:
			buttonValue = *((SInt32*) ioParam);
			if (buttonValue == 1) {

				mHorizontalAlignment = alignHoriz_DistributeHorizontal;
			
				// We get the distribute amount by querying the editor.
				// The interface for querying is handled by calling ProcessCommand.

				SInt16 hDistributeValue = 0;
				if (ProcessCommand(cmd_SpreadHorizQuery, (void*) &hDistributeValue))
					SetHorizontalSpread(hDistributeValue);

				// Disable the edit field.

				ValidateObject_(mHorizSpreadValue);
				mHorizSpreadValue->Disable();

			}
			break;
	
		// Distribute horizontal in container: Find horizontal spread amount.
		
		case alignHoriz_DistributeHorizInContainer:
			buttonValue = *((SInt32*) ioParam);
			if (buttonValue == 1) {

				mHorizontalAlignment = alignHoriz_DistributeHorizInContainer;
				
				// We get the distribute amount by querying the editor.
				// The interface for querying is handled by calling ProcessCommand.

				SInt16 hCenterDistance = 0;
				if (ProcessCommand(cmd_SpreadHorizContQuery, (void*) &hCenterDistance))
					SetHorizontalSpread(hCenterDistance);

				// Disable the edit field.

				ValidateObject_(mHorizSpreadValue);
				mHorizSpreadValue->Disable();

			}
			break;

		// User defined horizontal spread: Enable edit field.

		case alignHoriz_UserDefinedSpread:
			buttonValue = *((SInt32*) ioParam);
			if (buttonValue == 1) {
				mHorizontalAlignment = alignHoriz_UserDefinedSpread;
				ValidateObject_(mHorizSpreadValue);
				mHorizSpreadValue->Enable();
				mHorizSpreadValue->SelectAll();
				SwitchTarget(mHorizSpreadValue);
			}
			break;
		
		// Vertical flush alignment: Disable spread amount field.

		case alignVert_None:
		case alignVert_AlignTop:
		case alignVert_AlignHorizontalCenters:
		case alignVert_AlignBottom:
			buttonValue = *((SInt32*) ioParam);
			if (buttonValue == 1) {
				mVerticalAlignment = (EAlignVertical) inMessage;
				ValidateObject_(mVertSpreadValue);
				mVertSpreadValue->SetDescriptor( (StringPtr)"" );
				mVertSpreadValue->Disable();
			}
			break;
		
		// Distribute vertical: Find vertical spread amount.
		
		case alignVert_DistributeVertical:
			buttonValue = *((SInt32*) ioParam);
			if (buttonValue == 1) {

				mVerticalAlignment = alignVert_DistributeVertical;
 				
				// We get the distribute amount by querying the editor.
				// The interface for querying is handled by calling ProcessCommand.

				SInt16 vDistributeValue = 0;
				if (ProcessCommand(cmd_SpreadVertQuery, (void*) &vDistributeValue))
					SetVerticalSpread(vDistributeValue); 

				// Disable the edit field.

				ValidateObject_(mVertSpreadValue);
				mVertSpreadValue->Disable();

			}
			break;

		// Distribute vertical in container: Find vertical spread amount.
		
		case alignVert_DistributeVertInContainer:
			buttonValue = *((SInt32*) ioParam);
			if (buttonValue == 1) {

				mVerticalAlignment = alignVert_DistributeVertInContainer;
 				
				// We get the distribute amount by querying the editor.
				// The interface for querying is handled by calling ProcessCommand.

				SInt16 vDistributeValue = 0;
				if (ProcessCommand(cmd_SpreadVertContQuery, (void*) &vDistributeValue))
					SetVerticalSpread(vDistributeValue);
				
				// Disable the edit field.

				ValidateObject_(mVertSpreadValue);
				mVertSpreadValue->Disable();
			}
			break;

		// User defined horizontal spread: Enable edit field.

		case alignVert_UserDefinedSpread:
			buttonValue = *((SInt32*) ioParam);
			if (buttonValue == 1) {
				mVerticalAlignment = alignVert_UserDefinedSpread;
				ValidateObject_(mVertSpreadValue);
				mVertSpreadValue->Enable();
				mVertSpreadValue->SelectAll();
				SwitchTarget(mVertSpreadValue);
			}
			break;

		// Other messages: Pass along to LDialogBox.
		//	Skip the call to UpdatePreview.
		
		default:
			LDialogBox::ListenToMessage(inMessage, ioParam);
			return;
		
	}

	// Now we need to get the preview display updated to reflect the changes
	// made in the arrangement selections.

	UpdatePreview();
	
}


// ---------------------------------------------------------------------------
//		* UpdatePreview											[protected]
// ---------------------------------------------------------------------------
//	Trigger a redraw of the preview area.

void		
VEArrangeDialog::UpdatePreview()
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mPreview);
	
	// Use the box's calls to redraw its content area (not the same as
	// redrawing the entire box).

	mPreview->FocusDraw();
	Rect drawRect;
	mPreview->CalcLocalFrameRect(drawRect);
 	mPreview->LocalToPortPoint(topLeft(drawRect));
	mPreview->LocalToPortPoint(botRight(drawRect));
	InvalPortRect(&drawRect);
}


// ===========================================================================

#pragma mark -
#pragma mark ** initialization

// ---------------------------------------------------------------------------
//		* FinishCreateSelf										[protected]
// ---------------------------------------------------------------------------
//	Find references to subpanes.

void
VEArrangeDialog::FinishCreateSelf()
{
	
	// Validate pointers.
	
	ValidateThis_();
	
	// Configure default and cancel buttons.
	
	LDialogBox::FinishCreateSelf();
	
	// Listen to radio buttons, etc.
	
	UReanimator::LinkListenerToControls(this, this, GetPaneID());

	// Find the sample panes.
	
	mSample1 = FindPaneByID(PaneID_Sample1);
	mSample2 = FindPaneByID(PaneID_Sample2);
	mSample3 = FindPaneByID(PaneID_Sample3);
	
	ValidateObject_(mSample1);
	ValidateObject_(mSample2);
	ValidateObject_(mSample3);

	// Find the preview pane.
	
	mPreview = (dynamic_cast<LView*>(FindPaneByID(PaneID_Preview)));
	ValidateObject_(mPreview);
	
	// Find the spread amount fields (horizontal & vertical).
	
	mHorizSpreadValue = (dynamic_cast<LEditText*>(FindPaneByID(alignHoriz_Value)));
	mVertSpreadValue = (dynamic_cast<LEditText*>(FindPaneByID(alignVert_Value)));

	ValidateObject_(mHorizSpreadValue);
	ValidateObject_(mVertSpreadValue);

	// Save the original locations of the sample panes.
	
	SPoint32 localPoint1, localPoint2;

	mSample1->GetFrameLocation(localPoint1);
	(mSample1->GetSuperView())->GetFrameLocation(localPoint2);
	mSample1Loc.h = localPoint1.h - localPoint2.h;
	mSample1Loc.v = localPoint1.v - localPoint2.v;

	mSample2->GetFrameLocation(localPoint1);
	(mSample2->GetSuperView())->GetFrameLocation(localPoint2);
	mSample2Loc.h = localPoint1.h - localPoint2.h;
	mSample2Loc.v = localPoint1.v - localPoint2.v;

	mSample3->GetFrameLocation(localPoint1);
	(mSample3->GetSuperView())->GetFrameLocation(localPoint2);
	mSample3Loc.h = localPoint1.h - localPoint2.h;
	mSample3Loc.v = localPoint1.v - localPoint2.v;

	// Last time through this dialog, we saved the user's preferences for aligning.
	// Now restore this information.
	
	StPreferencesContext prefsContext;
	StResource currArrangementData(ResType_ArrangementData, ResID_ArrangementData, false);

	if ((Handle) currArrangementData != nil) {
		StHandleLocker lockData(currArrangementData);
		SetArrangementData(**(SAlignmentDataH) ((Handle) currArrangementData));
	}
	else {
		SAlignmentData defaultAlign = { alignHoriz_None, 0, alignVert_None, 0 };
		SetArrangementData(defaultAlign);
	}
}