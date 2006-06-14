// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	LIconTextTable.h				PowerPlant 2.1		©2000 Metrowerks Inc.
// ===========================================================================

#ifndef _H_LIconTextTable
#define _H_LIconTextTable
#pragma once

#include <LBroadcaster.h>
#include <LScrollerView.h>
#include <LTableView.h>

#if PP_Uses_Pragma_Import
	#pragma import on
#endif

PP_Begin_Namespace_PowerPlant

typedef struct
{
	SInt16 theIconID;
	Str255 theIconLabel;
	ResIDT theTextTraitsID;
} LIconTextItem, *LIconTextItemPtr;

class LIconTextTable : public LTableView, public LBroadcaster {
public:
	enum { class_ID = FOUR_CHAR_CODE('litt') };

						LIconTextTable(
								LStream				*inStream);
	virtual				~LIconTextTable();
	

	MessageT			GetControllerMessage() const	{ return mControllerMessage; }
	
	virtual void		SetControllerMessage(MessageT inControllerMessage);
	
	virtual void		SetScroller(LScrollerView *inScroller);
	
	virtual void		SelectionChanged();
	
	virtual void		Click(	SMouseDownEvent&	inMouseDown);
	
	virtual void		HiliteCellInactively(
								const STableCell		&inCell,
								Boolean					inHilite);
	
							// Column operations to disallow
						
	virtual void		InsertRows(
								UInt32				inHowMany,
								TableIndexT			inAfterCol,
								const void*			inDataPtr,
								UInt32				inDataSize,
								Boolean				inRefresh);
								
	virtual void		RemoveRows(
								UInt32				inHowMany,
								TableIndexT			inFromCol,
								Boolean				inRefresh);
	
	virtual SInt32		GetValue() const;
	virtual void		SetValue(
								SInt32				inValue);
	
	virtual StringPtr	GetDescriptor(
								Str255				outDescriptor) const;
	virtual void		SetDescriptor(
								ConstStringPtr		inDescriptor);
	
	virtual void		RevealSelectedIcon();
	virtual SInt16		GetIconCount() const;
	virtual void		RemoveIconAt(SInt16 inIndex);
	virtual void		AddIcon(LIconTextItem	*inButtonItem);
	virtual void		GetIconAt(SInt16 inIndex, LIconTextItem *outButtonItem);
	virtual void		SetSelectedIconByIndex(SInt16	inIndex);
	
protected:
	virtual void		ClickSelf(
								const SMouseDownEvent	&inMouseDown);
	
	virtual void		DrawCell(
								const STableCell		&inCell,
								const Rect				&inLocalRect);
	
private:
	MessageT			mControllerMessage;
	LScrollerView		*mScroller;
};

PP_End_Namespace_PowerPlant

#if PP_Uses_Pragma_Import
	#pragma import reset
#endif

#endif