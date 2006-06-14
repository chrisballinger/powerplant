// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxImageView.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:41 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxImageView.h
	@brief		A system view which displays a core graphics image
*/

#ifndef H_PPxImageView
#define H_PPxImageView
#pragma once

#include <PPxView.h>

namespace PPx {

// ===========================================================================
//	ImageView
/**
	A system view which displays a core graphics image */

class	ImageView : public View {
public:
						ImageView();
						
	virtual				~ImageView();
	
	void				Initialize(
								View*				inSuperView,
								const HIRect&		inFrame,
								bool				inVisible,
								bool				inEnabled,
								CGImageRef			inImage);
								
	void				SetOpaque( bool inOpaque );
	
	bool				IsOpaque() const;
	
	void				SetAlpha( Float32 inAlpha );
	
	Float32				GetAlpha() const;
	
	void				SetScaleToFit( bool inScaleToFit );
	
	bool				GetScaleToFit() const;
	
	void				SetImage( CGImageRef inImage );
	
	CGImageRef			CopyImage() const;
	
protected:	
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;
								
private:
	virtual CFStringRef	ClassName() const;
};


} // namespace PPx

#endif	// H_PPxImageView
