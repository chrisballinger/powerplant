// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxImageView.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:41 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxImageView.h>
#include <PPxSerializer.h>
#include <SysCreateView.h>

namespace PPx {

namespace {		// unnamed namespace 

	// -----------------------------------------------------------------------
	//	Key names for persistent data

	const CFStringRef	key_IsOpaque	= CFSTR("is opaque");
	const CFStringRef	key_Alpha		= CFSTR("alpha");
	const CFStringRef	key_ScaleToFit	= CFSTR("scale to fit");
}


// ---------------------------------------------------------------------------
//	ImageView														  [public]
/**
	Default constructor */

ImageView::ImageView()
{
}


// ---------------------------------------------------------------------------
//	~ImageView													  [public]
/**
	Destructor */

ImageView::~ImageView()
{
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initialize from image view creation parameters
	
	@param	inSuperView		Parent view
	@param	inFrame			Bounds for view, in local coordinates of parent
	@param	inVisible		Whether the view is visible
	@param	inEnabled		Whether the view is enabled	
	@param	inImage			CGImage to display								*/

void
ImageView::Initialize(
	View*			inSuperView,
	const HIRect&	inFrame,
	bool			inVisible,
	bool			inEnabled,
	CGImageRef		inImage)
{
	HIViewRef	viewRef = SysCreateView::ImageView(inImage);
	
	View::Initialize(viewRef, inSuperView, inFrame, inVisible, inEnabled);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
ImageView::ClassName() const
{
	return CFSTR("PPx::ImageView");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
ImageView::InitState(
	const DataReader&	inReader)
{
	bool		opaque		= true;			// Data for Image View
	float		alpha		= 1.0;
	bool		scaleToFit	= false;
	
	inReader.ReadOptional(key_IsOpaque, opaque);
	inReader.ReadOptional(key_Alpha, alpha);
	inReader.ReadOptional(key_ScaleToFit, scaleToFit);
	
		// ### Need a way to read a CGImageRef
	
	HIViewRef	viewRef = SysCreateView::ImageView(nil);
	
	View::InitViewState(viewRef, inReader);	// Data for all Views
	
	SetOpaque(opaque);
	SetAlpha(alpha);
	SetScaleToFit(scaleToFit);
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
ImageView::WriteState(
	DataWriter&	ioWriter) const
{
											// Data for Image View
	ioWriter.WriteValue(key_IsOpaque, IsOpaque());
	ioWriter.WriteValue(key_Alpha, GetAlpha());
	ioWriter.WriteValue(key_ScaleToFit, GetScaleToFit());
	
		// ??? Need a way to write CGImageRef ???
	
	View::WriteState(ioWriter);				// Data for all Views
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SetOpaque														  [public]
/**
	Sets whether the image is opaque
	
	@param	inOpaque	Whether the image is opaque							*/

void
ImageView::SetOpaque(
	bool	inOpaque)
{
	OSStatus	status = ::HIImageViewSetOpaque(GetSysView(), inOpaque);
									
	PPx_ThrowIfOSError_(status, "HIImageViewSetOpaque failed");
}


// ---------------------------------------------------------------------------
//	IsOpaque														  [public]
/**
	Returns whether the image is opaque
	
	@return Whether the image is opaque										*/

bool
ImageView::IsOpaque() const
{
	return ::HIImageViewIsOpaque(GetSysView());
}


// ---------------------------------------------------------------------------
//	SetAlpha														  [public]
/**
	Sets the alpha value for the image view
	
	@param	inAlpha		Alpha value for the iimage view						*/

void
ImageView::SetAlpha(
	Float32	inAlpha)
{
	OSStatus	status = ::HIImageViewSetAlpha(GetSysView(), inAlpha);
									
	PPx_ThrowIfOSError_(status, "HIImageViewSetAlpha failed");
}


// ---------------------------------------------------------------------------
//	GetAlpha														  [public]
/**
	Returns the alpha value for the image view
	
	@return Alpha value for the iimage view									*/

Float32
ImageView::GetAlpha() const
{
	return ::HIImageViewGetAlpha(GetSysView());
}


// ---------------------------------------------------------------------------
//	SetScaleToFit													  [public]
/**
	Sets whether to scale the image to fit in the frame
	
	@param	inScaleToFit	Whether to scale the image to fit				*/

void
ImageView::SetScaleToFit(
	bool	inScaleToFit)
{
	OSStatus	status = ::HIImageViewSetScaleToFit(
									GetSysView(), inScaleToFit);
									
	PPx_ThrowIfOSError_(status, "HIImageViewSetScaleToFit failed");
}


// ---------------------------------------------------------------------------
//	GetScaleToFit													  [public]
/**
	Returns whether the image scales to fit
	
	@return Whether the image scales to fit									*/

bool
ImageView::GetScaleToFit() const
{
	return ::HIImageViewGetScaleToFit(GetSysView());
}


// ---------------------------------------------------------------------------
//	SetImage														  [public]
/**
	Sets the image for the image view
	
	@param	inImage		CG image for the view								*/

void
ImageView::SetImage(
	CGImageRef	inImage)
{
	OSStatus	status = ::HIImageViewSetImage(GetSysView(), inImage);
									
	PPx_ThrowIfOSError_(status, "HIImageViewSetImage failed");
}


// ---------------------------------------------------------------------------
//	CopyImage														  [public]
/**
	Returns a copy of the image that is in the
	
	@return Copy of the image in the view									*/

CGImageRef
ImageView::CopyImage() const
{
	return ::HIImageViewCopyImage(GetSysView());
}


} // namespace PPx
