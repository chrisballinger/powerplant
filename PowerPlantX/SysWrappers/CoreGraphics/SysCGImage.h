// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysCGImage.h
//
//	Copyright © 2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:32 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		SysCGImage.h
	@brief		Wrapper class for Core Graphics Image
*/

#ifndef H_SysCGImage
#define H_SysCGImage
#pragma once

#include <SysCFObject.h>
#include <SysCGColorSpace.h>
#include <SysCGDataProvider.h>

#include <ApplicationServices/ApplicationServices.h>

namespace PPx {

// ===========================================================================
//	CGImage
/**
	Wrapper class for Core Graphics Image */

class	CGImage : public CFObject<CGImageRef> {
public:			
									// Constructors
						CGImage();
						
						CGImage(
								CGImageRef	inImageRef,
								bool		inRetain = true);
						
						CGImage(
								CFIndex					inWidth,
								CFIndex					inHeight,
								CFIndex					inBitsPerComponent,
								CFIndex					inBitsPerPixel,
								CFIndex					inBytesPerRow,
								CGColorSpaceRef			inColorSpace,
								CGImageAlphaInfo		inAlphaInfo,
								CGDataProviderRef		inProvider,
								const float				inDecode[] = nil,
								bool					inShouldInterpolate = false,
								CGColorRenderingIntent	inIntent = kCGRenderingIntentDefault);
						
	void				CreateMask(
								CFIndex					inWidth,
								CFIndex					inHeight,
								CFIndex					inBitsPerComponent,
								CFIndex					inBitsPerPixel,
								CFIndex					inBytesPerRow,
								CGDataProviderRef		inProvider,
								const float				inDecode[] = nil,
								bool					inShouldInterpolate = false);
						
	void				CreateWithJPEGDataProvider(
								CGDataProviderRef		inProvider,
								const float				inDecode[] = nil,
								bool					inShouldInterpolate = false,
								CGColorRenderingIntent	inIntent = kCGRenderingIntentDefault);

	void				CreateWithPNGDataProvider(
								CGDataProviderRef		inProvider,
								const float				inDecode[] = nil,
								bool					inShouldInterpolate = false,
								CGColorRenderingIntent	inIntent = kCGRenderingIntentDefault);

	CGImage				CopyWithColorSpace( CGColorSpaceRef inColorSpaceRef ) const;

						CGImage( const CGImage& inOriginal );
						
	CGImage&			operator = ( const CGImage& inSource );
	
									// Accessors
	bool				IsMask() const;
	
	CFIndex				GetWidth() const;
	
	CFIndex				GetHeight() const;
	
	CFIndex				GetBitsPerComponent() const;
								
	CFIndex				GetBitsPerPixel() const;
								
	CFIndex				GetBytesPerRow() const;
								
	CGColorSpace		GetColorSpace() const;
								
	CGImageAlphaInfo	GetAlphaInfo() const;
								
	CGDataProvider		GetDataProvider() const;
								
private:	
	typedef CFObject<CGImageRef>	BaseT;
};


} // namespace PPx

#endif // H_SysCGImage
