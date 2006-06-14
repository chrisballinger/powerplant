// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxPopupGroupBox.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:43 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxPopupGroupBox.h
	@brief		A system group box with a popup menu title
*/

#ifndef H_PPxPopupGroupBox
#define H_PPxPopupGroupBox
#pragma once

#include <PPxView.h>

namespace PPx {

// ===========================================================================
//	PopupGroupBox
/**
	A system group box with a popup menu title */

class	PopupGroupBox : public View {
public:
						PopupGroupBox();
						
	virtual				~PopupGroupBox();
	
	void				Initialize(
								View*			inSuperView,
								const HIRect&	inFrame,
								bool			inVisible,
								bool			inEnabled,
								CFStringRef		inTitle,
								bool			inIsPrimary,
								SInt32			inMenuID,
								bool			inHasVariableWidth,
								SInt16			inTitleWidth,
								SInt16			inTitleJust,
								Style			inTitleStyle);
	
	void				SetMenuRef( MenuRef inMenu );
	
	MenuRef				GetMenuRef() const;

	void				GetTitleRect( Rect& outTitleRect ) const;
	
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
	bool		mIsPrimaryGroup;		/**< Primary (true) or Secondary (false) group */
	Style		mTitleStyle;			/**< Text style of title */
};


} // namespace PPx

#endif	// H_PPxPopupGroupBox
