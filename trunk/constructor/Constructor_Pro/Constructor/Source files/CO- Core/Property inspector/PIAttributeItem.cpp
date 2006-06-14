// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PIAttributeItem.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 07/19/96
//     $Date: 2006/01/18 01:33:20 $
//	$History: PIAttributeItem.cpp $
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 02/28/97   Time: 11:48
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Added #ifs for Pilot appearance.
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 02/18/97   Time: 15:28
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Added ValueChanged method to ensure that attribute display keeps up
//	with attribute changes.
//	
//	*****************  Version 8  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:05p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:31
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Fixed CR/LF problem
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 12/12/96   Time: 19:00
//	Updated in $/Constructor/Source files/Property inspector
//	Rolled in Clint's changes for zero-based indexing in property
//	inspector.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 12/05/96   Time: 13:52
//	Updated in $/Constructor/Source files/Property inspector
//	Fixed DrawRowAdornments so it draws consistently on 1-bt displays.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/26/96   Time: 09:42
//	Updated in $/Constructor/Source files/Property inspector
//	Moved ValueChanged method to PIStringItem (reduces flicker for other
//	attribute items). Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/04/96   Time: 15:02
//	Updated in $/Constructor/Source files/Property inspector
//	Updated SetValueFromDisplay to show validation failure alert.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 10/31/96   Time: 17:17
//	Updated in $/Constructor/Source files/Property inspector
//	Added support for attribute's enabled flag.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 02:08
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "PIAttributeItem.h"

	// Core : Data model : Attributes
#include "DMListAttribute.h"

	// Core : Data model : Core objects
#include "DMObject.h"

	// Core : Data model : Exceptions
#include "DMValidationException.h"

	// Core : Display classes (Eric) : Outline table
#include "OVTable.h"

	// Core : Resource manager
//#include "StResourceContext.h"
#include <UResourceMgr.h>

	// PowerPlant : GA : Grayscale utilities
#include <UGAColorRamp.h>


// ===========================================================================

#pragma mark *** PIAttributeItem ***

// ---------------------------------------------------------------------------
//		* PIAttributeItem()
// ---------------------------------------------------------------------------
//	Constructor

PIAttributeItem::PIAttributeItem()
{
	mIndentSize = 8;
}


// ---------------------------------------------------------------------------
//		* ~PIAttributeItem
// ---------------------------------------------------------------------------
//	Destructor

PIAttributeItem::~PIAttributeItem()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** cell accessors

// ---------------------------------------------------------------------------
//		* GetAttributeForCell									[protected]
// ---------------------------------------------------------------------------
//	Overriden to return the attribute if column 2 is requested.

DMAttribute*
PIAttributeItem::GetAttributeForCell(
	TableIndexT		inColumn) const
{

	// Validate pointers.

	ValidateThis_();
	
	// Column two is always the attribute.

	if (inColumn == 2) {
		ValidateObject_(mAttribute.GetObject());
		return mAttribute;
	}
	else
		return OVItem::GetAttributeForCell(inColumn);

}


// ===========================================================================

#pragma mark -
#pragma mark ** update notifications

// ---------------------------------------------------------------------------
//		* ListenToMessage										[protected]
// ---------------------------------------------------------------------------
//	Listen for display changed message.

void
PIAttributeItem::ListenToMessage(
	MessageT	inMessage,
	void*		ioParam)
{

	// Validate pointers.

	ValidateThis_();

	// Listen for display changed message.
	// Update data model if we get it.

	switch (inMessage) {
		case msg_DisplayValueChanged:
			SetValueFromDisplay();
			break;
		
		default:
			OVItem::ListenToMessage(inMessage, ioParam);
	}
}


// ---------------------------------------------------------------------------
//		* EnabledChanged										[protected]
// ---------------------------------------------------------------------------
//	Called when the enabled flag of an element changes. If it is the
//	attribute we are displaying, then update the display.

void
PIAttributeItem::EnabledChanged(
	DMElement*	inElement)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inElement);

	// See if this is our attribute.
	
	if (inElement == mAttribute.GetObject()) {

		// It is...

		ValidateObject_(mAttribute.GetObject());

		// Redraw the row. The title field will be redrawn accordingly.

		RefreshRow();
		
		// Redraw any controls that may be used to show the value.
		
		if (mAttribute->IsEnabled())
			EnableSelf();
		else
			DisableSelf();

	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** drawing

// ---------------------------------------------------------------------------
//		* GetDrawContentsSelf									[protected]
// ---------------------------------------------------------------------------
//	Draw the attribute's title in column 1. This method is typically overriden
//	so that the attribute's value can be shown in column 2.

void
PIAttributeItem::GetDrawContentsSelf(
	const STableCell&		inCell,
	SOutlineDrawContents&	ioDrawContents)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mAttribute.GetObject());

	switch (inCell.col) {

		case 1: {
		
			// Display attribute name.
		
			ioDrawContents.outShowSelection = true;
			ioDrawContents.outTextBaseline++;
			LStr255 tempStr;
			mAttribute->GetTitle(tempStr);
			
			// If it's part of a dynamic list, add sequence number.
			
			DMListAttribute* super = (dynamic_cast<DMListAttribute*>(mAttribute->GetSuperElement()));
			if (super != nil) {
			
				ValidateObject_(super);

				SInt32 index = super->GetSubElements().FetchIndexOf(&mAttribute);
				if (super->GetZeroBasedIndex())
					index--;
				if (index >= 0) {
					tempStr += (unsigned char*)"\001 ";				//! TEMPORARY: get from STR#
					tempStr += LStr255((SInt32) index);
				}
			}
			
			// If attribute is disabled, dim title.
			// But... if object is enabled, don't dim title.
			
			if (!mAttribute->IsEnabled()) {

				DMObject* object = mAttribute->GetAffectedObject();
				ValidateObject_(object);
				
				if (object->IsEnabled())
					ioDrawContents.outTextTraits.mode = grayishTextOr;

			}
			
			// Copy name back to param block.
			
			LString::CopyPStr(tempStr, ioDrawContents.outTextString);
			break;
		}
	}
}


// ---------------------------------------------------------------------------
//		* DrawRowAdornments										[protected]
// ---------------------------------------------------------------------------
//	Draw the notebook line below the attribute item. Draw the
//	separator between value and title. Draw the modified flag.

void
PIAttributeItem::DrawRowAdornments(
	const Rect&		inLocalRowRect)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mOutlineTable);

	// Focus the table.

	mOutlineTable->FocusDraw();

	// Draw the notebook line under the item.
	
	StColorPenState::Normalize();
#if Constructor_ForPilot
	RGBColor theColor = UGAColorRamp::GetColor(3); //we have to draw on a grey screen
#else
	RGBColor theColor = UGAColorRamp::GetColor(2);
#endif
	::RGBForeColor(&theColor);
	::MoveTo(inLocalRowRect.left + 1, inLocalRowRect.top);
	::LineTo(inLocalRowRect.right - 2, inLocalRowRect.top);

	// Draw the value/title separator.

	SInt16 titleWidth = mOutlineTable->GetColWidth(1);
	::MoveTo(titleWidth, inLocalRowRect.top);
	::LineTo(titleWidth, inLocalRowRect.bottom - 1);

	// Draw modified flag.
	
	OVItem::DrawRowAdornments(inLocalRowRect);
	
}


// ---------------------------------------------------------------------------
//		* CreatePaneForCell										[protected]
// ---------------------------------------------------------------------------
//	Helper function that creates a pane for a cell, given a cell ID
//	and PPob resource ID.

LPane*
PIAttributeItem::CreatePaneForCell(
	const STableCell&	inCell,
	ResIDT				inPPobID)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mAttribute.GetObject());
	ValidateObject_(mOVTable);

	// Create the pane.

	LPane::SetDefaultView(mOVTable);
	LCommander::SetDefaultCommander(mOVTable);
	LAttachable::SetDefaultAttachable(nil);
	mOVTable->FocusDraw();

	StApplicationContext appContext;	
	LPane* thePane = (LPane*) UReanimator::ReadObjects('PPob', inPPobID);
	ValidateObject_(thePane);
	
	// If the attribute is disabled, disable the pane as well.
	
	if (!mAttribute->IsEnabled())
		thePane->Disable();

	// Find the cell location and place the pane so it's in the top
	// left corner of the cell.
	
	SInt32 left, top, right, bottom;
	mOVTable->GetImageCellBounds(inCell, left, top, right, bottom);
	thePane->PutInside(mOVTable);
	thePane->PlaceInSuperImageAt(left + 2, top + 1, false);
	thePane->FinishCreate();

	// Resize the pane to match the cell's location. Using the
	// pane's AdaptToSuperFrameSize method is a cute hack... :-)
	
	SDimension16 size;
	thePane->GetFrameSize(size);
	thePane->AdaptToSuperFrameSize((right - left) - size.width - 4,
									(bottom - top) - size.height - 1,
									false);
	
	// Install this pane in the table geometry.
	
	mOVTable->SetCellPane(inCell, thePane);

	// Return pane to caller.
	
	return thePane;
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** mouse-down behaviors

// ---------------------------------------------------------------------------
//		* DoubleClick											[protected]
// ---------------------------------------------------------------------------
//	Does nothing. Overriden to disable OVItem's double-click to open
//	property inspector.

void
PIAttributeItem::DoubleClick(
	const STableCell&			/* inCell */,
	const SMouseDownEvent&		/* inMouseDown */,
	const SOutlineDrawContents&	/* inDrawContents */,
	Boolean						/* inHitText */)
{
	// Overriden to disable double-click to open inspector.
}


// ===========================================================================

#pragma mark -
#pragma mark ** display synchronization

// ---------------------------------------------------------------------------
//		* SetValueFromAttribute									[protected]
// ---------------------------------------------------------------------------
//	The attribute's value has changed; make sure the display is updated
//	to match it.

void
PIAttributeItem::SetValueFromAttribute()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mAttribute.GetObject());

	// Don't listen to value change messages from the control.
	
	StValueChanger<Boolean> dontListen(mIsListening, false);

	// Set the display from the attribute.
	// Ignore exceptions that are thrown by the display.

	try {
		SetValueFromAttributeSelf();
	}
	catch(...) {
		// Not much we can do.
	}
}


// ---------------------------------------------------------------------------
//		* SetValueFromAttributeSelf								[protected]
// ---------------------------------------------------------------------------
//	Override hook. Ensure that the value display is updated.
//	Does not need to be overriden if in-place editing is used.

void
PIAttributeItem::SetValueFromAttributeSelf()
{

	// Default is just to refresh the affected cell.

	RefreshCell(2);

}


// ---------------------------------------------------------------------------
//		* SetValueFromDisplay									[protected]
// ---------------------------------------------------------------------------
//	The display value has changed. Ensure that the attribute's value
//	gets changed to match it. This method is not typically overriden.
//	It provides the default behavior of setting up a transaction so
//	that the value change can be undone.

void
PIAttributeItem::SetValueFromDisplay()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mAttribute.GetObject());

	// Wrap the actual set code in a try/catch block. This way we can
	// catch validation failures and share the code to report & fix it.
	
	try {
		SetValueFromDisplaySelf();
	}
	catch(const DMValidationException& error) {
	
		// Value was unacceptable. Show dialog explaining why.
	
		error.ShowValidationFailedAlert();

	}
	catch(...) {
	
		SignalCStr_("Unexpected exception type.");

	}
}


// ---------------------------------------------------------------------------
//		* SetValueFromDisplaySelf								[protected]
// ---------------------------------------------------------------------------
//	Override hook. Subclass to transfer the display's value to the
//	attribute object.

void
PIAttributeItem::SetValueFromDisplaySelf()
{
	// override hook
}


// ---------------------------------------------------------------------------
//		* DisableSelf											[protected]
// ---------------------------------------------------------------------------
//	Called when the attribute goes from enabled to disabled.
//	Override to dim any controls used to display the attribute's value.

void
PIAttributeItem::DisableSelf()
{
	// override hook
}


// ---------------------------------------------------------------------------
//		* EnableSelf
// ---------------------------------------------------------------------------
//	Called when the attribute goes from disabled to enabled.
//	Override to undim any controls used to display the attribute's value.

void
PIAttributeItem::EnableSelf()
{
	// override hook
}


// ===========================================================================

#pragma mark -
#pragma mark ** configuration

// ---------------------------------------------------------------------------
//		* SetElementSelf										[protected]
// ---------------------------------------------------------------------------
//	Set the element (probably an attribute) that we'll be displaying.

void
PIAttributeItem::SetElementSelf(
	DMElement*	inElement)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inElement);

	// Sanity check: Make sure we have an attribute.

	mAttribute = (dynamic_cast<DMAttribute*>(inElement));
	ValidateObject_(mAttribute.GetObject());

}


// ---------------------------------------------------------------------------
//		* CollectChanges										[protected]
// ---------------------------------------------------------------------------
//	When changes are collected, we transfer the value from display to
//	attribute.

void
PIAttributeItem::CollectChanges(
	DMElement*	inElement)
{

	// Validate pointers.

	ValidateThis_();

	// Copy any possible change from display object to data model.

	if (inElement == (DMAttribute*) mAttribute)
		SetValueFromDisplay();

}


// ---------------------------------------------------------------------------
//		* ValueChanged											[protected]
// ---------------------------------------------------------------------------
//	Make sure the display accurately reflects the attribute's new value.

void
PIAttributeItem::ValueChanged(
	DM_ValueChange*		inMessage)
{

	// Validate pointers.

	ValidateThis_();

	// If the changed item is our attribute, redisplay it.
	
	if (inMessage->mAttribute == mAttribute)
		SetValueFromAttribute();

	// See if we're displaying this attribute.
	// If so, redraw the cell.

	TableIndexT col = GetCellForAttribute(inMessage->mAttribute);
	if (col != 0)
		RefreshCell(col);

}
