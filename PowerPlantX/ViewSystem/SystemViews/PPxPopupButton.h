// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxPopupButton.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:43 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxPopupButton.h
	@brief		A system popup button control
*/

#ifndef H_PPxPopupButton
#define H_PPxPopupButton
#pragma once

#include <PPxView.h>

namespace PPx {

// ===========================================================================
//	PopupButton
/**
	A system popup button control */

class	PopupButton : public View {
public:
						PopupButton();
						
	virtual				~PopupButton();
	
	void				Initialize(
								View*			inSuperView,
								const HIRect&	inFrame,
								bool			inVisible,
								bool			inEnabled,
								CFStringRef		inTitle,
								SInt32			inMenuID,
								bool			inHasVariableWidth,
								SInt16			inTitleWidth,
								SInt16			inTitleJustification,
								Style			inTitleStyle);
	
	void				SetMenuRef( MenuRef inMenu );
	
	MenuRef				GetMenuRef() const;
	
	void				SetOwnedMenuRef( MenuRef inMenu );
	
	MenuRef				GetOwnedMenuRef() const;
	
	void				SetMenuID( SInt16 inMenuID );
	
	SInt16				GetMenuID() const;
	
	void				SetExtraHeight( SInt16 inExtraHeight );
	
	SInt16				GetExtraHeight() const;
	
	void				SetCheckCurrentItemFlag( bool inCheckIt );
	
	bool				GetCheckCurrentItemFlag() const;
	
protected:	
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;
								
private:
	virtual CFStringRef	ClassName() const;
	
private:
	SInt16		mMenuID;				/**< MENU resource ID */
	SInt16		mTitleWidth;			/**< Width of title area in pixels */
	SInt16		mTitleJustification;	/**< Justification of title */
	bool		mHasVariableWidth;		/**< Adjust title area to width of text? */
	Style		mTitleStyle;			/**< Text style of title */
};


} // namespace PPx

#endif	// H_PPxPopupButton
