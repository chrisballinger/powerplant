// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxIconControl.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:41 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxIconControl.h
	@brief		A system icon control
*/

#ifndef H_PPxIconControl
#define H_PPxIconControl
#pragma once

#include <PPxView.h>

namespace PPx {

// ===========================================================================
//	IconControl
/**
	A system icon control */

class	IconControl : public View {
public:
						IconControl();
						
	virtual				~IconControl();
	
	void				Initialize(
								View*			inSuperView,
								const HIRect&	inFrame,
								bool			inVisible,
								bool			inEnabled,
								const ControlButtonContentInfo&	inContent,
								bool			inDontTrack);
	
	void				SetIconTransform( IconTransformType inTransform );
	
	IconTransformType	GetIconTransform() const;
	
	void				SetIconAlignment( IconAlignmentType inAlignment );
	
	IconAlignmentType	GetIconAlignment() const;
	
	void				SetIconResourceID( SInt16 inResID );
	
	SInt16				GetIconResourceID() const;
	
	void				SetContentInfo( const ControlButtonContentInfo& inContent );
	
	void				GetContentInfo( ControlButtonContentInfo& outContent ) const;
	
protected:	
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;
								
private:
	virtual CFStringRef	ClassName() const;
	
private:
	bool		mDontTrack;		/**< Whether to hilite and track mouse when clicked */
};


} // namespace PPx

#endif	// H_PPxIconControl
