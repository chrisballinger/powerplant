// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxImageWell.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:42 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxImageWell.h
	@brief		A system image well view
*/

#ifndef H_PPxImageWell
#define H_PPxImageWell
#pragma once

#include <PPxView.h>

namespace PPx {

// ===========================================================================
//	ImageWell
/**
	A system image well view */

class	ImageWell : public View {
public:
						ImageWell();
						
	virtual				~ImageWell();
	
	void				Initialize(
								View*			inSuperView,
								const HIRect&	inFrame,
								bool			inVisible,
								bool			inEnabled,
								const ControlButtonContentInfo&	inContent);
	
	void				SetContentInfo( const ControlButtonContentInfo& inContent );
	
	void				GetContentInfo( ControlButtonContentInfo& outContent ) const;
	
	void				SetImageTransform( IconTransformType inTransform );
	
	IconTransformType	GetImageTransform() const;
	
	void				SetDragDestinationFlag( bool inIsDragDestination );
	
	bool				GetDragDestinationFlag() const;
	
protected:	
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;
								
private:
	virtual CFStringRef	ClassName() const;
};


} // namespace PPx

#endif	// H_PPxImageWell
