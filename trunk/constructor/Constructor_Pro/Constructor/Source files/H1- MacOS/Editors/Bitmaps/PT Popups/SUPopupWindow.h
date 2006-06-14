// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				SUPopupWindow.h

PROJECT:			Constructor Icon Editor

CLASSES:			SUPopupWindow, SUColorPopup, SUPatternPopup

DESCRIPTION:		Main include file for phony popup menus (Color & Pattern)

AUTHOR:				Eric Shapiro

CREATION DATE :		96.5.22

CHANGE HISTORY :

		96.06.27	ebs		Fixed the look & feel (& changed class names)
		96.05.22	ebs		Integrated into project from previous code
	
---------------------------------------<< ¥ >>----------------------------------------
*/

#pragma once

	// note: items are numbered from 0..n-1, -1 indicates "nothing hit"
	
class SUPopupWindow
{
	public:
							SUPopupWindow();
		virtual				~SUPopupWindow();
	
	protected:
		Rect				mWindowRect;				// in global coords
		SInt32				mItems;
		SInt32				mRows, mCols;
		Rect				mFirstItemRect;
		SInt32				mDefaultItem;
		SInt32				mCellWidth, mCellHeight;
		
			// override these	
		virtual void		Prepare( Point inTopLeft ) = 0;	
		virtual void		DrawInitialState() = 0;
		virtual void		DrawOneCell( SInt32 inIndex, Boolean inHilited = false ) = 0;
	
			// probably don't need to override these
		virtual	SInt32		DoPopup( Point inTopLeft, SInt32 defaultItem );
		
		virtual SInt32		TrackMouse();
		virtual SInt32		LocalPointToIndex( Point pt );
		virtual Boolean		GetItemRect( SInt32 itemNo, Rect *r );
		virtual void		HiliteItem( SInt32 inItem, Boolean hiliteIt = true );
		virtual void		FlashItem( SInt32 inItem );
		
#if PP_Target_Carbon
		virtual WindowPtr	CreateMacWindow( void );
#else
		virtual WindowPtr	CreateMacWindow( WindowRecord *inStorage = nil );
#endif
};