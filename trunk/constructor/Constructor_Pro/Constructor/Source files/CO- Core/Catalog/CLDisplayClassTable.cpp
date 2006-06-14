// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	CLDisplayClassTable.cpp		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/26/96
//     $Date: 2006/01/18 01:32:28 $
//	$History: CLDisplayClassTable.cpp $
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 03/18/97   Time: 15:46
//	Updated in $/Constructor/Source files/CO- Core/Catalog
//	Diabled double-click for property inspector in catalog window. (Bug fix
//	#1308.)
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 02/28/97   Time: 11:04
//	Updated in $/Constructor/Source files/CO- Core/Catalog
//	Added an #ifdef to keep Pilot from appearing in table.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 02/20/97   Time: 13:23
//	Updated in $/Constructor/Source files/CO- Core/Catalog
//	Improved commenting.
//	
//	*****************  Version 5  *****************
//	User: Andrew       Date: 2/06/97    Time: 11:50a
//	Updated in $/ConstructorWin/Sources
//	Converted dynamic const casts to MSVC-compatible format
//	
//	*****************  Version 4  *****************
//	User: Andrew       Date: 2/04/97    Time: 2:48p
//	Updated in $/ConstructorWin/Sources
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:18
//	Updated in $/Constructor/Source files/CO- Core/Catalog
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 12/06/96   Time: 18:51
//	Updated in $/Constructor/Source files/Catalog
//	Fixed a couple of bugs relating to dragging abstract classes.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/30/96   Time: 17:46
//	Created in $/Constructor/Source files/Catalog
//	Added class (adapted from old VEDisplayClassTable).
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "CLDisplayClassTable.h"

	// Core : Headers
#include "Constructor.menu.h"

	// Core : Data model : Attributes
#include "DMContainerAttribute.h"

	// Core : Data model : Utilities
#include "DMDragTask.h"
#include "DMIterator.h"

	// Core : Editors : Views : Data model
#include "VECatalogPage.h"
#include "VETypeEntry.h"
#include "VETypeList.h"


// ===========================================================================

#pragma mark *** CLDisplayClassTable ***

// ---------------------------------------------------------------------------
//		* CLDisplayClassTable(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor

CLDisplayClassTable::CLDisplayClassTable(
	LStream*	inStream)

: OVTable(inStream, 17, true)

{
	SetItemFactory(new CLClassItemFactory);
	SetSubItemComparator(new CLClassNameComparator);
	mCatalogPage = nil;
	mAlphabeticSort = false;
}


// ---------------------------------------------------------------------------
//		* ~CLDisplayClassTable
// ---------------------------------------------------------------------------
//	Destructor

CLDisplayClassTable::~CLDisplayClassTable()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** menu command status

// ---------------------------------------------------------------------------
//		* FindCommandStatus
// ---------------------------------------------------------------------------
//	Overriden to check the Show Display Classes menu item.

void
CLDisplayClassTable::FindCommandStatus(
	CommandT	inCommand,
	Boolean&	outEnabled,
	Boolean&	outUsesMark,
	UInt16&		outMark,
	Str255		outName)
{
	switch (inCommand) {

		case cmd_Catalog:
			outEnabled = true;
			outUsesMark = true;
			outMark = checkMark;
			break;
		
		case cmd_Duplicate:
			outEnabled = false;
			break;
	
		default:
			OVTable::FindCommandStatus(inCommand, outEnabled, outUsesMark, outMark, outName);
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** configuration

// ---------------------------------------------------------------------------
//		* SetAlphabeticDisplay
// ---------------------------------------------------------------------------
//	Triggered by the scroll-bar button for alpha/hier sort. Turn on or off
//	alphabetic sorting of the classes.

void
CLDisplayClassTable::SetAlphabeticDisplay(
	Boolean		inAlphaSort)
{
	
	// Validate pointers.
	
	ValidateThis_();
	
	// Make sure we aren't changing the display.
	
	if (inAlphaSort == mAlphabeticSort)
		return;
	
	// Okay. Tear down the current display.
	
	DMElement* element = mElement;
	SetElement(nil);
	
	mAlphabeticSort = inAlphaSort;
	
	if (inAlphaSort)
		mFirstLevelIndent = 12;
	else
		mFirstLevelIndent = 24;
	
	SetElement(element);
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** internal configuration

// ---------------------------------------------------------------------------
//		* FinishCreateSelf										[protected]
// ---------------------------------------------------------------------------
//	Configure the table display.

void
CLDisplayClassTable::FinishCreateSelf()
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Do inherited configuration.

	OVTable::FinishCreateSelf();

	// Set up default columns. Number of columns depends on whether
	// class IDs are significant.
	
	VETypeList* typeList = VETypeList::GetTypeList();
	ValidateObject_(typeList);
	
	if (typeList->GetClassIDsSignificant()) {
		InsertCols(2, 0, nil, 0, false);
		SetColWidth(mFrameSize.width - 40, 1, 1);
		SetColWidth(40, 2, 2);
	}
	else {
		InsertCols(1, 0, nil, 0, false);
		SetColWidth(mFrameSize.width, 1, 1);
	}
}


// ---------------------------------------------------------------------------
//		* SetElement											[protected]
// ---------------------------------------------------------------------------
//	Overridden to allow the element to be changed.

void
CLDisplayClassTable::SetElement(
	DMElement*	inElement)
{

	// Validate pointers.

	ValidateThis_();
	if (inElement != nil)
		ValidateObject_(inElement);

	// See if the element has changed. If not, ignore request.
	
	if (inElement == mElement)
		return;

	// Make sure screen doesn't flash while changing elements.
	
#if PP_Target_Carbon
	StClipRgnState noFlash(nil);
#else
	StVisRgn noFlash(GetMacPort());
#endif
	ValidateObject_(mSuperView);
	mSuperView->Refresh();

	// Override to allow changing elements. If we are assigned
	// to an element already, empty the table.

	if (mElement != nil) {
	
		ValidateObject_(mElement.GetObject());

		while (mRows > 0) {
			LOutlineItem* item = FindItemForRow(1);
			ValidateObject_(item);
			RemoveItem(item, false, false);
		}
		
		AdjustImageSize(false);
		mSuperView->Refresh();		// GHD: Refresh SuperView so that scroll bars get redrawn
		
		mElement->RemoveListener(this);
		mElement = nil;
		mContainer = nil;
		
	}

	// Now call the inherited method for set element only
	// if the new element is non-nil.

	if (inElement != nil)
		OVTable::SetElement(inElement);
}


// ---------------------------------------------------------------------------
//		* SetElementSelf										[protected]
// ---------------------------------------------------------------------------
//	Create display items for each named type.

void
CLDisplayClassTable::SetElementSelf(
	DMElement*	inElement)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inElement);

	// Sanity check: Make sure we have a type list.

	mCatalogPage = (dynamic_cast<VECatalogPage*>(inElement));
	ValidateObject_(mCatalogPage);
	
	// Okay, it's a catalog page. Create type views for all of its classes.

	if (mAlphabeticSort)
		AddSubClasses(mCatalogPage);
	else
		SetContainer(mCatalogPage->GetSubClasses());

}


// ---------------------------------------------------------------------------
//		* AddSubClasses											[protected]
// ---------------------------------------------------------------------------
//	Called only when alphabetic sorting is enabled. Add all of the subclasses
//	of this element to the table.

void
CLDisplayClassTable::AddSubClasses(
	DMElement*	inContainerClass)
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inContainerClass);
	
	DMObject* object = (dynamic_cast<DMObject*>(inContainerClass));
	
	// Get subclass list.
	
	DMContainerAttribute* container = object->FindContainerAttribute('SCLS');
	ValidateObject_(container);
	
	DMFastIterator iter(container->GetSubElements());
	while (iter.NextElement()) {
		DMElement* objDM = iter.CurrentElement();
		ValidateObject_(objDM);
		CreateItemForElement(objDM, false, false);
		AddSubClasses(objDM);
	}

	// Redraw the table.

	AdjustImageSize(false);
	Refresh();

}


// ===========================================================================

#pragma mark -
#pragma mark ** custom resizing behaviors

// ---------------------------------------------------------------------------
//		* CalcStandardSize										[protected]
// ---------------------------------------------------------------------------
//	Make sure the window zooms to our preferred size.

void
CLDisplayClassTable::CalcStandardSize(
	GReferencedWindow*	inWindow)
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inWindow);

	// Find our image size.

	SDimension32 hierImageSize;
	GetImageSize(hierImageSize);
	hierImageSize.width = 130;

	// Calculate overscan (i.e. how far are we inside the window).
	
	SDimension16 overscanSize;
	SDimension16 windowFrameSize;
	inWindow->GetFrameSize(windowFrameSize);
	overscanSize.width = windowFrameSize.width - mFrameSize.width;
	overscanSize.height = windowFrameSize.height - mFrameSize.height;

	// Set window's size.

	SDimension16 windStandardSize;
	windStandardSize.width = ((hierImageSize.width) > 16000
								? 16000
								: hierImageSize.width) + overscanSize.width;
	windStandardSize.height = ((hierImageSize.height) > 16000
								? 16000
								: hierImageSize.height) + overscanSize.height;

	inWindow->SetStandardSize(windStandardSize);
	
}


// ---------------------------------------------------------------------------
//		* ResizeFrameBy											[protected]
// ---------------------------------------------------------------------------
//	Make sure the table column follows the window size.

void
CLDisplayClassTable::ResizeFrameBy(
	SInt16		inWidthDelta,
	SInt16		inHeightDelta,
	Boolean		inRefresh)
{

	// Validate pointers.
	
	ValidateThis_();

	// Resize the view.

	LView::ResizeFrameBy(inWidthDelta, inHeightDelta, inRefresh);
	
	// Stretch the class name column to fill space.
	
	if (inWidthDelta != 0)
		SetColWidth(GetColWidth(1) + inWidthDelta, 1, 1);

}


// ===========================================================================

#pragma mark -
#pragma mark *** CLClassItemFactory ***

// ---------------------------------------------------------------------------
//		* CreateItemForElement
// ---------------------------------------------------------------------------

OVItem*
CLClassItemFactory::CreateItemForElement(
	DMElement*	inElement) 
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inElement);
	
	// See what type of object this is.

	VETypeEntry* type = (dynamic_cast<VETypeEntry*>(inElement));
	if (type == nil)
		return nil;
	else {
		ValidateObject_(type);
#if Constructor_ForPilot
		if (type->MustBeRoot())
			return nil;
#endif
		return new CLTypeEntryItem;
	}
}

// ===========================================================================

#pragma mark -
#pragma mark *** CLTypeEntryItem ***

// ---------------------------------------------------------------------------
//		* CLTypeEntryItem()
// ---------------------------------------------------------------------------
//	Constructor

CLTypeEntryItem::CLTypeEntryItem()
{
	mTypeEntry = nil;
}


// ---------------------------------------------------------------------------
//		* ~CLTypeEntryItem
// ---------------------------------------------------------------------------
//	Destructor

CLTypeEntryItem::~CLTypeEntryItem()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** configuration

// ---------------------------------------------------------------------------
//		* SetElementSelf										[protected]
// ---------------------------------------------------------------------------
//	Make sure this element is a VETypeEntry.

void
CLTypeEntryItem::SetElementSelf(
	DMElement*	inElement)
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inElement);
	
	// Make sure we really have a VETypeEntry.

	mTypeEntry = (dynamic_cast<VETypeEntry*>(inElement));
	ValidateObject_(mTypeEntry);

	// See how we're sorting the classes. If it's alphabetical,
	// we don't need to specify the container for subclasses.
	
	CLDisplayClassTable* table = (dynamic_cast<CLDisplayClassTable*>(mOVTable));
	ValidateObject_(table);
	
	if (!table->GetAlphabeticDisplay()) {
	
		// Hierarchical sort: Specify container for submodels.
	
		DMElement* subComponents = mTypeEntry->GetSubClasses();
		if (subComponents != nil) {
			mExpanded = true;
			SetSubItemComparator(new CLClassNameComparator);
			SetContainer(subComponents);
		}
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** drawing

// ---------------------------------------------------------------------------
//		* GetDrawContentsSelf									[protected]
// ---------------------------------------------------------------------------
//	We draw only one column: the class name.

void
CLTypeEntryItem::GetDrawContentsSelf(
	const STableCell&		inCell,
	SOutlineDrawContents&	ioDrawContents)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mTypeEntry);
	
	// Return draw contents.

	LStr255 tempStr;

	switch (inCell.col) {
	
		case 1:
			mTypeEntry->GetShortClassName(tempStr);
			LString::CopyPStr(tempStr, ioDrawContents.outTextString);
			if (mTypeEntry->IsBuiltIn())
				ioDrawContents.outTextTraits.style |= bold;
			ioDrawContents.outIconSuite = mTypeEntry->GetClassIcon();
			ioDrawContents.outHasIcon = true;
			ioDrawContents.outShowSelection = true;
			if (mTypeEntry->IsAbstractClass()) {
				ioDrawContents.outTextTraits.color.red = 65535;		// abstract classes drawn in red
				ioDrawContents.outTextTraits.color.blue = 0;
				ioDrawContents.outTextTraits.color.green = 0;
			}
			break;
		
		case 2:
			tempStr = (FourCharCode) mTypeEntry->GetClassID();
			LString::CopyPStr(tempStr, ioDrawContents.outTextString);
			break;

	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** drag-and-drop behaviors

// ---------------------------------------------------------------------------
//		* TrackDrag												[protected]
// ---------------------------------------------------------------------------
//	Drag an instance of this type to view editor window.

void
CLTypeEntryItem::TrackDrag(
	const STableCell&			inCell,
	const SMouseDownEvent&		inMouseDown,
	const SOutlineDrawContents&	/* inDrawContents */)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mTypeEntry);

	// Don't allow drag of abstract class.

	if (mTypeEntry->IsAbstractClass())
		return;
	
	// Create an instance of this type that we can drag.
	
	VEModelObjectPtr instance;
	mTypeEntry->CreateInstance(instance);
	if (instance == nil)
		return;

	// Start a drag task.
	
	DMDragTask drag(inMouseDown.macEvent);

	// Add the instance of this object to the drag.
	
	RgnHandle tempRgn = ::NewRgn();
#if PP_Target_Carbon
	ThrowIfNil_(tempRgn);
#else
	ValidateHandle_((Handle) tempRgn);
#endif

	Rect itemBounds;
	MakeDragRegion(inCell, tempRgn, itemBounds);
	drag.AddDragElement(instance, tempRgn, &itemBounds);
	::DisposeRgn(tempRgn);

	// Make the translucent drag image for the dragged item.

	RgnHandle imageRgn = nil;
	GWorldPtr imageGWorld = nil;
//	mOVTable->MakeDragImage(drag.GetDragReference(), inCell, inMouseDown, imageRgn, imageGWorld);
	
	// Track the drag.
	
	drag.DoDrag();

	// Dispose image.
	
	if (imageRgn != nil) {
#if !PP_Target_Carbon
		ValidateHandle_((Handle) imageRgn);
#endif
		::DisposeRgn(imageRgn);
	}
	if (imageGWorld != nil) {
		ValidatePtr_((Ptr) imageGWorld);
		::DisposeGWorld(imageGWorld);
	}
}


// ---------------------------------------------------------------------------
//		* MakeDragRegion										[protected]
// ---------------------------------------------------------------------------
//	Make the outline for Drag Manager tracking. Overriden to create a
//	rectangle the size of the placed object, rather than a shadow of the
//	table entry.

void
CLTypeEntryItem::MakeDragRegion(
	const STableCell&	inCell,
	RgnHandle			outGlobalDragRgn,
	Rect&				outGlobalItemBounds)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mOutlineTable);

	// Make sure this cell is visible.

	Rect ignore;
	if (!mOutlineTable->GetLocalCellRect(inCell, ignore))
		return;

	// Find out what we're supposed to draw.

	SOutlineDrawContents drawInfo;
	GetDrawContents(inCell, drawInfo);

	// Create a bounds rect that starts at the upper left corner
	// of the icon and has the width & height of the placed item.
	
	if (drawInfo.outHasIcon && (drawInfo.outIconSuite != nil) && !::EmptyRect(&drawInfo.prIconFrame))
		outGlobalItemBounds = drawInfo.prIconFrame;
	else
		outGlobalItemBounds = drawInfo.prTextFrame;

	SDimension32 dragSize;
	mTypeEntry->GetDragSize(dragSize);
	
	outGlobalItemBounds.bottom = outGlobalItemBounds.top + dragSize.height;
	outGlobalItemBounds.right = outGlobalItemBounds.left + dragSize.width;

	// Convert to global coordinates.

	mOutlineTable->LocalToPortPoint(topLeft(outGlobalItemBounds));
	mOutlineTable->LocalToPortPoint(botRight(outGlobalItemBounds));
	mOutlineTable->PortToGlobalPoint(topLeft(outGlobalItemBounds));
	mOutlineTable->PortToGlobalPoint(botRight(outGlobalItemBounds));

	// Item bounds have been created. Now create drag region that
	// is the same size.

	RgnHandle tempRgn = ::NewRgn();
#if PP_Target_Carbon
	ThrowIfNil_(tempRgn);
#else
	ValidateHandle_((Handle) tempRgn);
#endif
	::RectRgn(tempRgn, &outGlobalItemBounds);
	::UnionRgn(outGlobalDragRgn, tempRgn, outGlobalDragRgn);
	::DisposeRgn(tempRgn);
	
}

// ---------------------------------------------------------------------------
//		* 
// ---------------------------------------------------------------------------

void
CLTypeEntryItem::DoubleClick(
	const STableCell&			/* inCell */,
	const SMouseDownEvent&		/* inMouseDown */,
	const SOutlineDrawContents&	/* inDrawContents */,
	Boolean						/* inHitText */)
{
	// Do nothing. Disables property inspector behavior
	// in OVItem::DoubleClick.
}



// ===========================================================================

#pragma mark -
#pragma mark *** CLClassNameComparator ***

// ---------------------------------------------------------------------------
//		* CompareElements										[protected]
// ---------------------------------------------------------------------------
//	Compare two type entries by name.

SInt32
CLClassNameComparator::CompareElements(
	const DMElement*	inElement1,
	const DMElement*	inElement2) const 
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inElement1);
	ValidateObject_(inElement2);
	
	// Cast elements to VETypeEntry and compare by name.

	const VETypeEntry* type1 = dynamic_const_cast(inElement1,DMElement*,VETypeEntry*);
	const VETypeEntry* type2 = dynamic_const_cast(inElement2,DMElement*,VETypeEntry*);
	ValidateObject_(type1);
	ValidateObject_(type2);
	
	LStr255 name1;
	LStr255 name2;
	type1->GetClassName(name1);
	type2->GetClassName(name2);

	return name1.CompareTo(name2);
}