// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxListBox.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:42 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxListBox.h
	@brief		A system list box control
	
	@note A list box control does not work properly in a composited window.
	This is a system limitation as noted in Apple's "Introducing HIView"
	documentation.
*/

#ifndef H_PPxListBox
#define H_PPxListBox
#pragma once

#include <PPxView.h>

namespace PPx {

// ===========================================================================
//	ListBox
/**
	A system list box control */

class	ListBox : public View {
public:
						ListBox();
						
	virtual				~ListBox();
	
	void				Initialize(
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
								SInt16				inListDefID = 0);
								
	ListHandle			GetListHandle() const;
	
	SInt16				GetRowCount() const;
	
	SInt16				GetColumnCount() const;
	
	Point				GetCellSize() const;
	
	void				AddColumns(
								SInt16		inColCount,
								SInt16		inInsertAt);
								
	void				RemoveColumns(
								SInt16		inColCount,
								SInt16		inDeleteAt);
								
	void				AddRows(
								SInt16		inRowCount,
								SInt16		inInsertAt);
								
	void				RemoveRows(
								SInt16		inRowCount,
								SInt16		inDeleteAt);
								
	void				SetCellData(
								Cell		inCell,
								const void*	inDataPtr,
								SInt16		inDataLength);
								
	void				SetCellData(
								Cell		inCell,
								CFStringRef	inString);
								
	void				GetCellData(
								Cell		inCell,
								void*		outDataPtr,
								SInt16&		ioDataLength) const;
								
	CFString			GetCellData(
								Cell		inCell) const;
								
	bool				CellIsSelected( Cell inCell ) const;
	
	bool				GetFirstSelectedCell( Cell& outCell ) const;
	
	bool				GetNextSelectedCell( Cell& ioCell ) const;
	
	void				SelectCell(
								Cell		inCell,
								bool		inSelectIt);				
	
	void				SetThemeFontID( ThemeFontID inFontID );
	
protected:	
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;
								
private:
	virtual CFStringRef	ClassName() const;
	
private:
	ListHandle		mListHandle;
	SInt16			mListDefID;
	bool			mHasGrowSpace;
};


} // namespace PPx

#endif	// H_PPxListBox
