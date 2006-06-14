// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxListBox.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:42 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
//
//	A list box control does not work properly in a composited window.
//	This is a system limitation as noted in Apple's "Introducing HIView"
//	documentation.

#include <PPxListBox.h>
#include <PPxSerializer.h>
#include <PPxViewUtils.h>
#include <SysCreateView.h>

namespace PPx {

namespace {

	// -----------------------------------------------------------------------
	//	Key names for persistent data

	const CFStringRef	key_RowCount			= CFSTR("row count");
	const CFStringRef	key_ColCount			= CFSTR("column count");
	const CFStringRef	key_CellHeight			= CFSTR("cell heigth");
	const CFStringRef	key_CellWidth			= CFSTR("cell width");
	const CFStringRef	key_HasGrowSpace		= CFSTR("has grow space");
	const CFStringRef	key_HasHorizScroll		= CFSTR("has horizontal scrollbar");
	const CFStringRef	key_HasVertScroll		= CFSTR("has vertical scrollbar");
	const CFStringRef	key_ListDefID			= CFSTR("list definition id");
	const CFStringRef	key_StringList			= CFSTR("string list");
}


// ---------------------------------------------------------------------------
//	ListBox														  [public]
/**
	Default constructor */

ListBox::ListBox()
{
	mListHandle		= nil;
	mListDefID		= 0;
	mHasGrowSpace	= false;
}


// ---------------------------------------------------------------------------
//	~ListBox														  [public]
/**
	Destructor */

ListBox::~ListBox()
{
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initialize from list box creation parameters
	
	@param	inSuperView		Parent view
	@param	inFrame			Bounds for view, in local coordinates of parent
	@param	inVisible		Whether the view is visible
	@param	inEnabled		Whether the view is enabled
	@param	inRowCount		Number of rows
	@param	inColCount		Number of columns
	@param	inHorizScroll	Whether list box has a horizontal scroll bar
	@param	inVertScroll	Whether list box has a vertical scroll bar
	@param	inCellHeight	Pixel height of cells
	@param	inCellWidth		Pixel widht of cells
	@param	inHasGrowSpace	Whether to leave space for a grow box
	@param	inListDefID		List defintion ID. Pass 0 for a standard text list.
	
	A list box must be created within a window with the compositing attribute
	off. This is a limitation of the system list box control.
	
	@note If you specify a list defintion ID other than 0 (a standard text
	list), you must first call the Toolbox function RegisterListDefiniton to
	associate a list definition function with the ID.						*/

void
ListBox::Initialize(
	View*				inSuperView,
	const HIRect&		inFrame,
	bool				inVisible,
	bool				inEnabled,
	SInt16				inRowCount,
	SInt16				inColCount,
	SInt16				inCellHeight,
	SInt16				inCellWidth,
	bool				inHasGrowSpace,
	bool				inHorizScroll,
	bool				inVertScroll,
	SInt16				inListDefID)
{
							// Require that the list box be within a window
	if ((inSuperView == nil) or (inSuperView->GetSysWindow() == nil)) {
		PPx_Throw_(RuntimeError, err_Runtime, "List box must be created within a window");
	}
	
	Rect	qdFrame;
	ViewUtils::HIToQDRect(inFrame, qdFrame);

	HIViewRef	viewRef = SysCreateView::ListBox(
								inSuperView->GetSysWindow(), qdFrame,
								inRowCount, inColCount,
								inCellHeight, inCellWidth,
								inHasGrowSpace, inHorizScroll, inVertScroll);

	View::Initialize(viewRef, inSuperView, inFrame, inVisible, inEnabled);

	mListHandle		= GetListHandle();
	mListDefID		= inListDefID;
	mHasGrowSpace	= inHasGrowSpace;

	if (inListDefID != 0) {
		SetDataTag(kControlEntireControl, kControlListBoxLDEFTag,
					sizeof(SInt16), &inListDefID);
	}
}

#pragma mark -

// ---------------------------------------------------------------------------
//	GetListHandle													  [public]
/**
	Returns ListHandle for the list box control
	
	@return ListHandle for the list box control								*/

ListHandle
ListBox::GetListHandle() const
{
	ListHandle	listH;
	
	GetDataTag(kControlListBoxPart, kControlListBoxListHandleTag,
				sizeof(ListHandle), &listH);
				
	return listH;
}


// ---------------------------------------------------------------------------
//	GetRowCount														  [public]
/**
	Returns the number of rows in the list box
	
	@return Number of rows in the list box									*/

SInt16
ListBox::GetRowCount() const
{
	ListBounds	dataBounds;
	::GetListDataBounds(mListHandle, &dataBounds);
	return dataBounds.bottom;
}


// ---------------------------------------------------------------------------
//	GetColumnCount													  [public]
/**
	Returns the number of columns in the list box
	
	@return Number of columns in the list box								*/

SInt16
ListBox::GetColumnCount() const
{
	ListBounds	dataBounds;
	::GetListDataBounds(mListHandle, &dataBounds);
	return dataBounds.right;
}


// ---------------------------------------------------------------------------
//	GetCellSize														  [public]
/**
	Returns the width and height of a cell in the list box
	
	@return Width and height of a cell in the list box
	
	All cells in a list box are the same size. The .h field of the Point
	is the width; the .v is the height.										*/

Point
ListBox::GetCellSize() const
{
	Point	cellSize;
	::GetListCellSize(mListHandle, &cellSize);
	return cellSize;
}

#pragma mark -

// ---------------------------------------------------------------------------
//	AddColumns														  [public]
/**
	Adds columns to the list box
	
	@param	inColCount		Number of columns to add
	@param	inInsertAt		Zero-based index at which to insert columns		*/

void
ListBox::AddColumns(
	SInt16	inColCount,
	SInt16	inInsertAt)
{
	::LAddColumn(inColCount, inInsertAt, mListHandle);
}


// ---------------------------------------------------------------------------
//	RemoveColumns													  [public]
/**
	Removes columns from the list box
	
	@param	inColCount		Number of columns to remove
	@param	inDeleteAt		Zero-based index of first column to delete		*/

void
ListBox::RemoveColumns(
	SInt16	inColCount,
	SInt16	inDeleteAt)
{
	::LDelColumn(inColCount, inDeleteAt, mListHandle);
}


// ---------------------------------------------------------------------------
//	AddRows															  [public]
/**
	Adds rows to the list box
	
	@param	inRowCount		Number of row to add
	@param	inInsertAt		Zero-based index at which to insert rows		*/

void
ListBox::AddRows(
	SInt16	inRowCount,
	SInt16	inInsertAt)
{
	::LAddRow(inRowCount, inInsertAt, mListHandle);
}


// ---------------------------------------------------------------------------
//	RemoveRows														  [public]
/**
	Removes rows from the list box
	
	@param	inRowCount		Number of row to remove
	@param	inDeleteAt		Zero-based index of first row to delete			*/

void
ListBox::RemoveRows(
	SInt16	inRowCount,
	SInt16	inDeleteAt)
{
	::LDelRow(inRowCount, inDeleteAt, mListHandle);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SetCellData														  [public]
/**
	Sets the data for a cell in the list box
	
	@param	inCell			Cell whose data to set. Indexes are zero-based.
	@param	inDataPtr		Buffer containing cell data
	@param	inDataLength	Length of cell data								*/

void
ListBox::SetCellData(
	Cell			inCell,
	const void*		inDataPtr,
	SInt16			inDataLength)
{
	::LSetCell((Ptr) inDataPtr, inDataLength, inCell, mListHandle);
}


// ---------------------------------------------------------------------------
//	SetCellData														  [public]
/**
	Sets the cell data to a string
	
	@param	inCell		Cell whose data to set. Indexes are zero-based.
	@param	inString	Cell data string
	
	The typical list box is a text table. This is a conveniece function for
	setting the text from a CFString. However, the CFString must be
	convertable to a Pascal string, as that is what a list box supports.	*/

void
ListBox::SetCellData(
	Cell		inCell,
	CFStringRef	inString)
{
	CFString	str(inString);
	Str255		pString;
	
	if (str.GetPascalString(pString, sizeof(pString))) {
		SetCellData(inCell, &pString[1], pString[0]);
	}
}


// ---------------------------------------------------------------------------
//	GetCellData														  [public]
/**
	Passes back the data for a cell
	
	@param	inCell			Cell whose data to retrieve
	@param	outDataPtr		Buffer for storing cell data
	@param	ioDataLength	On entry, the maximum size of the data buffer.
							On exit, the actual bytes of cell data copied	*/

void
ListBox::GetCellData(
	Cell	inCell,
	void*	outDataPtr,
	SInt16&	ioDataLength) const
{
	::LGetCell(outDataPtr, &ioDataLength, inCell, mListHandle);
}


// ---------------------------------------------------------------------------
//	GetCellData														  [public]
/**
	Returns the data for a cell as a string
	
	@param	inCell		Cell whose data to retrieve
	
	The typical list box is a text table. This is a convenience function for
	getting the text as a CFString.											*/

CFString
ListBox::GetCellData(
	Cell	inCell) const
{
	Str255	pString;
	SInt16	strLength = sizeof(pString);
	GetCellData(inCell, &pString[1], strLength);
	pString[0] = strLength;
	return CFString(pString);
}


// ---------------------------------------------------------------------------
//	CellIsSelected													  [public]
/**
	Returns whether the cell is selected
	
	@param	inCell		Cell to check
	
	@return Whether the cell is selected									*/

bool
ListBox::CellIsSelected(
	Cell	inCell) const
{
	return ::LGetSelect(false, &inCell, mListHandle);
}


// ---------------------------------------------------------------------------
//	GetFirstSelectedCell											  [public]
/**
	Finds the first selected cell in the list box
	
	@param	outCell		First selected cell in the list box
	
	@return Whether there is a selected cell in the list box
	
	Cells are ordered by column, then by row. That is, the next cell is the
	one in the same row, next column; or the next row, first column if the
	cell is in the last column.
	
	If there is no selected cell, outCell is set to (0, 0)					*/

bool
ListBox::GetFirstSelectedCell(
	Cell&	outCell) const
{
	outCell.v = 0;
	outCell.h = 0;
	
	return ::LGetSelect(true, &outCell, mListHandle);
}


// ---------------------------------------------------------------------------
//	GetNextSelectedCell												  [public]
/**
	Finds the next selected cell in the list box
	
	@param	ioCell		On entry, the cell at which to start searching
						On exit, the next selected cell
	
	@return Whether there is a next selected cell in the list box
	
	Cells are ordered by column, then by row. That is, the next cell is the
	one in the same row, next column; or the next row, first column if the
	cell is in the last column.
	
	If there is no next selected cell, outCell is unchanged.				*/

bool
ListBox::GetNextSelectedCell(
	Cell&	ioCell) const
{
	return ::LGetSelect(true, &ioCell, mListHandle);
}


// ---------------------------------------------------------------------------
//	SelectCell														  [public]
/**
	Sets whether a cell is selected
	
	@param	inCell		The cell to select or deselect
	@param	inSelectIt	Whether to select or deselect the cell				*/

void
ListBox::SelectCell(
	Cell	inCell,
	bool	inSelectIt)
{
	::LSetSelect(inSelectIt, inCell, mListHandle);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
ListBox::ClassName() const
{
	return CFSTR("PPx::ListBox");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
ListBox::InitState(
	const DataReader&	inReader)
{
											// Data for list box
	SInt16		rowCount		= 0;
	SInt16		colCount		= 0;
	SInt16		cellHeight		= 0;
	SInt16		cellWidth		= 0;
	bool		horizScroll		= false;
	bool		vertScroll		= true;
	
	inReader.ReadOptional(key_RowCount,			rowCount);
	inReader.ReadOptional(key_ColCount,			colCount);
	inReader.ReadOptional(key_CellHeight,		cellHeight);
	inReader.ReadOptional(key_CellWidth,		cellWidth);
	inReader.ReadOptional(key_HasGrowSpace,		mHasGrowSpace);
	inReader.ReadOptional(key_HasHorizScroll,	horizScroll);
	inReader.ReadOptional(key_HasVertScroll,	vertScroll);
	inReader.ReadOptional(key_ListDefID,		mListDefID);
	
	View*	superView;
	HIRect	frame = { 0, 0, 0, 0 };
	bool	visible;
	bool	enabled;
	
	ReadViewState(inReader, superView, frame, visible, enabled);
	
							// Require that the list box be within a window
	if ((superView == nil) or (superView->GetSysWindow() == nil)) {
		PPx_Throw_(RuntimeError, err_Runtime, "List box must be created within a window");
	}
	
	Rect	qdFrame;
	ViewUtils::HIToQDRect(frame, qdFrame);

	HIViewRef	viewRef = SysCreateView::ListBox(
								superView->GetSysWindow(), qdFrame,
								rowCount, colCount,
								cellHeight, cellWidth,
								mHasGrowSpace, horizScroll, vertScroll);
	
	View::InitViewState(viewRef, inReader);	// Data for all Views

	mListHandle = GetListHandle();

	if (mListDefID != 0) {
		SetDataTag(kControlEntireControl, kControlListBoxLDEFTag,
					sizeof(SInt16), &mListDefID);
	}
	
		// For text tables, read the string for each cell
	
	if ( (rowCount > 0) and (colCount > 0) and (mListDefID == 0) and
		 inReader.ContainsKey(key_StringList) ) {
	
		std::vector<CFString>	stringList;
		stringList.reserve(rowCount * colCount);
		inReader.ReadContainer<CFString>(key_StringList, std::back_inserter(stringList));
		
		Cell	theCell;
		std::vector<CFString>::const_iterator	iter = stringList.begin();
		
		for (theCell.v = 0; theCell.v < rowCount; ++theCell.v) {
			for (theCell.h = 0; theCell.h < colCount; ++theCell.h) {
			
				SetCellData(theCell, *iter);
				++iter;
			}
		}
	}
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
ListBox::WriteState(
	DataWriter&	ioWriter) const
{
											// Data for list box
	Point	cellSize	= GetCellSize();
	bool	vertScroll  = ::GetListVerticalScrollBar(mListHandle) != nil;
	bool	horizScroll = ::GetListHorizontalScrollBar(mListHandle) != nil;
	
	ioWriter.WriteValue(key_RowCount,		GetRowCount());
	ioWriter.WriteValue(key_ColCount,		GetColumnCount());
	ioWriter.WriteValue(key_CellHeight,		cellSize.v);
	ioWriter.WriteValue(key_CellWidth,		cellSize.h);
	ioWriter.WriteValue(key_HasGrowSpace,	mHasGrowSpace);
	ioWriter.WriteValue(key_HasHorizScroll,	horizScroll);
	ioWriter.WriteValue(key_HasVertScroll,	vertScroll);
	ioWriter.WriteValue(key_ListDefID,		mListDefID);
	
	SInt16	rowCount = GetRowCount();
	SInt16	colCount = GetColumnCount();
	
		// For text tables, save the contents of the cells as strings
	
	if ( (rowCount > 0) and (colCount > 0) and (mListDefID == 0) ) {
	
		std::vector<CFString>	stringList;
		stringList.reserve(rowCount * colCount);
	
		Cell	theCell;
		for (theCell.v = 0; theCell.v < rowCount; ++theCell.v) {
			for (theCell.h = 0; theCell.h < colCount; ++theCell.h) {
			
				stringList.push_back( GetCellData(theCell) );
			}
		}
		
		ioWriter.WriteContainer(key_StringList, stringList.begin(),
									stringList.end());
	}
		
	View::WriteState(ioWriter);				// Data for all Views
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SetThemeFontID													  [public]
/**
	Sets the theme font ID for the clock
	
	@param	inFont		Theme font ID to use for text						*/

void
ListBox::SetThemeFontID(
	ThemeFontID		inFont)
{
	ViewUtils::SetControlThemeFontID(this, inFont);
}


} // namespace PPx
