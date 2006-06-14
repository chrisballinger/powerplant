// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxComboBox.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:40 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxComboBox.h
	@brief		A system combo box control
*/

#ifndef H_PPxComboBox
#define H_PPxComboBox
#pragma once

#include <PPxView.h>
#include <SysCFArray.h>

namespace PPx {

// ===========================================================================
//	ComboBox
/**
	A system combo box control */

class	ComboBox : public View {
public:
						ComboBox();
						
	virtual				~ComboBox();
	
	void				Initialize(
								View*			inSuperView,
								const HIRect&	inFrame,
								bool			inVisible,
								bool			inEnabled,
								CFStringRef		inDefaultText,
								const ControlFontStyleRec*	inStyle,
								CFArrayRef		inValueList,
								OptionBits		inAttributes);
								
	void				SetText( CFStringRef inText );
	
	CFString			GetText() const;
	
	SInt32				GetListItemsCount() const;
	
	void				InsertListItemAt(
								CFIndex			inIndex,
								CFStringRef		inItemText);
								
	CFIndex				AppendListItem( CFStringRef inItemText );
	
	void				RemoveListItem( CFIndex inIndex );
								
	CFString			GetListItemText( CFIndex inIndex ) const;
	
	void				SetListItems( CFArrayRef inListItems );
	
	CFArray<CFStringRef>
						GetListItems() const;
	
	void				ChangeAttributes(
								OptionBits		inAttributesToSet,
								OptionBits		inAttributesToClear);
								
	OptionBits			GetAttributes() const;
	
protected:	
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;
								
private:
	virtual CFStringRef	ClassName() const;
};


} // namespace PPx

#endif	// H_PPxComboBox
