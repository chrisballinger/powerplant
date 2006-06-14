// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxBevelButton.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:39 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxBevelButton.h
	@brief		A system bevel button control
*/

#ifndef H_PPxBevelButton
#define H_PPxBevelButton
#pragma once

#include <PPxView.h>

namespace PPx {

// ===========================================================================
//	BevelButton
/**
	A system bevel button control */

class	BevelButton : public View {
public:
						BevelButton();
						
	virtual				~BevelButton();
	
	void				Initialize(
								View*							inSuperView,
								const HIRect&					inFrame,
								bool							inVisible,
								bool							inEnabled,
								CFStringRef						inTitle,
								ControlBevelThickness			inThickness,
								ControlBevelButtonBehavior		inButtonBehavior,
								const ControlButtonContentInfo&	inContent,
								SInt16							inMenuID,
								ControlBevelButtonMenuBehavior	inMenuBehavior,
								ControlBevelButtonMenuPlacement	inMenuPlacement);
	
	void				SetTextAlignment( ControlButtonTextAlignment inAlignment );
	
	ControlButtonTextAlignment
						GetTextAlignment() const;
						
	void				SetTextOffset( SInt16 inOffset );
	
	SInt16				GetTextOffset() const;
	
	void				SetTextPlacement( ControlButtonTextPlacement inPlacement );
	
	ControlButtonTextPlacement
						GetTextPlacement() const;
								
	void				SetIconTransform( IconTransformType inTransform );
	
	IconTransformType	GetIconTransform() const;
	
	void				SetGraphicAlignment( ControlButtonGraphicAlignment inAlignment );
	
	ControlButtonGraphicAlignment
						GetGraphicAlignment() const;
						
	void				SetGraphicOffset( const Point& inOffset );
	
	Point				GetGraphicOffset() const;
	
	void				SetMenuValue( SInt16 inValue );
	
	SInt16				GetMenuValue() const;
	
	void				SetMenuRef( MenuRef inMenu );
	
	MenuRef				GetMenuRef() const;
	
	void				SetCenterPopupGlyph( bool inCenter );
	
	bool				GetCenterPopupGlyph() const;
	
	void				SetContentInfo( const ControlButtonContentInfo& inContent );
	
	void				GetContentInfo( ControlButtonContentInfo& outContent ) const;

protected:	
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;
								
private:
	virtual CFStringRef	ClassName() const;
	
private:
	ControlBevelThickness			mThickness;
	ControlBevelButtonBehavior		mButtonBehavior;
	SInt16							mMenuID;
	ControlBevelButtonMenuBehavior	mMenuBehavior;
	ControlBevelButtonMenuPlacement	mMenuPlacement;
};


} // namespace PPx

#endif	// H_PPxBevelButton
