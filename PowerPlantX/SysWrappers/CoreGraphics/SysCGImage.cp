// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysCGImage.cp
//
//	Copyright © 2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:32 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <SysCGImage.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	CGImage															  [public]
/**
	Default constructor	*/

CGImage::CGImage()
{
}


// ---------------------------------------------------------------------------
//	CGImage															  [public]
/**
	Construct from a CGImageRef

	@param	inImageRef		CG image reference
	@param	inRetain		Whether to retain the CG image
	
	@note Toolbox functions which return a CGImageRef and have the word
	"copy" or "create" in their name return a image with a retain count
	of one. Pass false for inRetain when constructing from such a image.
	The CGImage won't retain the image, but will release the image in
	its destructor.
	
	Toolbox functions which return a CGImageRef and have the word "get"
	in their name return a image without incrementing its retain count.
	Pass true for inRetain when constructing from such a image.
	The CGImage will retain the image, and later release it.				*/

CGImage::CGImage(
	CGImageRef	inImageRef,
	bool		inRetain)
	
	: BaseT(inImageRef, inRetain)
{
}


// ---------------------------------------------------------------------------
//	CGImage															  [public]
/**
	Constructs a Quartz bitmap image
	
	@param	inWidth				The width, in pixels, of the required image
	@param	inHeight			The height, in pixels, of the required image
	@param	inBitsPerComponent	The number of bits for each component in a
								source pixel. For example, if the source image
								uses the RGBA-32 format, you would specify 8
								bits per component
	@param	inBitsPerPixel		The total number of bits in a source pixel.
								This value must be at least bitsPerComponent
								times the number of components per pixel.
	@param	inBytesPerRow		The number of bytes of memory for each
								horizontal row of the bitmap.
	@param	inColorSpace		The color space for the image
	@param	inAlphaInfo			A CGImageAlphaInfo constant that specifies (1)
								whether the bitmap contains an alpha channel,
								(2) where the alpha bits are located in the
								image data, and (3) and whether the alpha
								value is premultiplied.
	@param	inProvider			The source of data for the bitmap
	@param	inDecode			The decode array for the image. If you do not
								want to allow remapping of the image’s color
								values, pass nil for the decode array
	@param	inShouldInterpolate	Specifies whether interpolation should occur.
								The interpolation setting specifies whether
								Quartz should apply a pixel-smoothing
								algorithm to the image.
	@param	inIntent			A CGColorRenderingIntent constant that
								specifies how Quartz should handle colors that
								are not located within the gamut of the
								destination color space of a graphics context */

CGImage::CGImage(
	CFIndex					inWidth,
	CFIndex					inHeight,
	CFIndex					inBitsPerComponent,
	CFIndex					inBitsPerPixel,
	CFIndex					inBytesPerRow,
	CGColorSpaceRef			inColorSpace,
	CGImageAlphaInfo		inAlphaInfo,
	CGDataProviderRef		inProvider,
	const float				inDecode[],
	bool					inShouldInterpolate,
	CGColorRenderingIntent	inIntent)
{
	CGImageRef	ref = ::CGImageCreate(inWidth, inHeight, inBitsPerComponent,
							inBitsPerPixel, inBytesPerRow, inColorSpace,
							inAlphaInfo, inProvider, inDecode,
							inShouldInterpolate, inIntent);
	PPx_ThrowIfCFCreateFailed_(ref, "CGImageCreate");
	
	AttachRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	CreateMask														  [public]
/**
	Constructs a Quartz bitmap image mask
	
	@param	inWidth				The width, in pixels, of the required image mask
	@param	inHeight			The height, in pixels, of the required image mask
	@param	inBitsPerComponent	The number of significant masking bits in a
								source pixel. For example, if the source image
								is an 8-bit mask, you would specify 8 bits per
								component.
	@param	inBitsPerPixel		The total number of bits in a source pixel.
	@param	inBytesPerRow		The number of bytes of memory for each
								horizontal row of the image mask.
	@param	inProvider			The data source for the image mask
	@param	inDecode			The decode array for the image mask.
	@param	inShouldInterpolate	Specifies whether interpolation should occur.
								The interpolation setting specifies whether
								Quartz should apply an edge-smoothing
								algorithm to the image mask.				*/

void
CGImage::CreateMask(
	CFIndex				inWidth,
	CFIndex				inHeight,
	CFIndex				inBitsPerComponent,
	CFIndex				inBitsPerPixel,
	CFIndex				inBytesPerRow,
	CGDataProviderRef	inProvider,
	const float			inDecode[],
	bool				inShouldInterpolate)
{
	CGImageRef	ref = ::CGImageMaskCreate(inWidth, inHeight, inBitsPerComponent,
							inBitsPerPixel, inBytesPerRow, inProvider, inDecode,
							inShouldInterpolate);
	PPx_ThrowIfCFCreateFailed_(ref, "CGImageMaskCreate");
	
	AttachRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	CreateWithJPEGDataProvider										  [public]
/**
	Constructs a Quartz bitmap image using JPEG-encoded data
	
	@param	inProvider			A data provider supplying JPEG-encoded data
	@param	inDecode			The decode array for the image
	@param	inShouldInterpolate	Specifies whether interpolation should occur.
								The interpolation setting specifies whether
								Quartz should apply a pixel-smoothing
								algorithm to the image.
	@param	inIntent			A CGColorRenderingIntent constant that
								specifies how Quartz should handle colors that
								are not located within the gamut of the
								destination color space of a graphics context */

void
CGImage::CreateWithJPEGDataProvider(
	CGDataProviderRef		inProvider,
	const float				inDecode[],
	bool					inShouldInterpolate,
	CGColorRenderingIntent	inIntent)
{
	CGImageRef	ref = ::CGImageCreateWithJPEGDataProvider(inProvider,
		inDecode, inShouldInterpolate, inIntent);
	PPx_ThrowIfCFCreateFailed_(ref, "CGImageCreateWithJPEGDataProvider");
	
	AttachRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	CreateWithPNGDataProvider										  [public]
/**
	Constructs a Quartz bitmap image using PNG-encoded data
	
	@param	inProvider			A data provider supplying PNG-encoded data
	@param	inDecode			The decode array for the image
	@param	inShouldInterpolate	Specifies whether interpolation should occur.
								The interpolation setting specifies whether
								Quartz should apply a pixel-smoothing
								algorithm to the image.
	@param	inIntent			A CGColorRenderingIntent constant that
								specifies how Quartz should handle colors that
								are not located within the gamut of the
								destination color space of a graphics context */

void
CGImage::CreateWithPNGDataProvider(
	CGDataProviderRef		inProvider,
	const float				inDecode[],
	bool					inShouldInterpolate,
	CGColorRenderingIntent	inIntent)
{
	CGImageRef	ref = ::CGImageCreateWithPNGDataProvider(inProvider,
		inDecode, inShouldInterpolate, inIntent);
	PPx_ThrowIfCFCreateFailed_(ref, "CGImageCreateWithPNGDataProvider");
	
	AttachRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	CopyWithColorSpace												  [public]
/**
	Constructs a copy of a Quartz bitmap image, replacing its colorspace
	
	@param	inColorSpaceRef		The destination color space. The number of
								components in this color space must be the
								same as the number in the specified image

	@return A new Quartz image that is a copy of the image passed as the
			image parameter but with its color space replaced by that
			specified by the colorspace parameter							*/

CGImage
CGImage::CopyWithColorSpace(
	CGColorSpaceRef	inColorSpaceRef) const
{
	CGImage	imageCopy(::CGImageCreateCopyWithColorSpace(UseRef(),
		inColorSpaceRef), retain_No);
	PPx_ThrowIfCFCreateFailed_(imageCopy.GetRefValue(), "CGImageCreateCopyWithColorSpace");
	
	return imageCopy;
}


// ---------------------------------------------------------------------------
//	CGImage															  [public]
/**
	Copy constructor */

CGImage::CGImage(
	const CGImage&	inOriginal)
	
	: BaseT(inOriginal)
{
}


// ---------------------------------------------------------------------------
//	operator =														  [public]
/**
	Assignment Operator */

CGImage&
CGImage::operator = (
	const CGImage&	inSource)
{
	AssignObject(inSource);
	return *this;
}

#pragma mark -

// ---------------------------------------------------------------------------
//	IsMask															  [public]
/**
	Returns if the image is an image mask
	
	@return True if the image is an image mask								*/

bool
CGImage::IsMask() const
{
	return ::CGImageIsMask( UseRef() );
}


// ---------------------------------------------------------------------------
//	GetWidth														  [public]
/**
	Returns the width of the image
	
	@return Width of the image												*/

CFIndex
CGImage::GetWidth() const
{
	return ::CGImageGetWidth( UseRef() );
}


// ---------------------------------------------------------------------------
//	GetHeight														  [public]
/**
	Returns the height of the image
	
	@return Height of the image												*/

CFIndex
CGImage::GetHeight() const
{
	return ::CGImageGetHeight( UseRef() );
}


// ---------------------------------------------------------------------------
//	GetBitsPerComponent												  [public]
/**
	Returns the number of bits allocated for a single color component
	
	@return Number of bits allocated for a single color component			*/

CFIndex
CGImage::GetBitsPerComponent() const
{
	return ::CGImageGetBitsPerComponent( UseRef() );
}


// ---------------------------------------------------------------------------
//	GetBitsPerPixel													  [public]
/**
	Returns the number of bits allocated for a single pixel
	
	@return Number of bits allocated for a single pixel						*/

CFIndex
CGImage::GetBitsPerPixel() const
{
	return ::CGImageGetBitsPerPixel( UseRef() );
}


// ---------------------------------------------------------------------------
//	GetBytesPerRow													  [public]
/**
	Returns the number of bytes allocated for a single row
	
	@return Number of bytes allocated for a single row						*/

CFIndex
CGImage::GetBytesPerRow() const
{
	return ::CGImageGetBytesPerRow( UseRef() );
}


// ---------------------------------------------------------------------------
//	GetColorSpace													  [public]
/**
	Returns the color space of the image
	
	@return CGColorSpace of the image (may be if image is a mask)			*/

CGColorSpace
CGImage::GetColorSpace() const
{
	return CGColorSpace(::CGImageGetColorSpace(UseRef()), retain_Yes);
}


// ---------------------------------------------------------------------------
//	GetAlphaInfo													  [public]
/**
	Returns the alpha channel information of the image
	
	@return Alpha channel information of the image							*/

CGImageAlphaInfo
CGImage::GetAlphaInfo() const
{
	return ::CGImageGetAlphaInfo( UseRef() );
}


// ---------------------------------------------------------------------------
//	GetDataProvider													  [public]
/**
	Returns the data provider of the image
	
	@return CGDataProvider of the image										*/

CGDataProvider
CGImage::GetDataProvider() const
{
	return CGDataProvider(::CGImageGetDataProvider(UseRef()), retain_Yes);
}




} // namespace PPx