// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysCGColorSpace.cp
//
//	Copyright © 2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:32 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

// ### Should we add routines to create color spaces from the ICC color
// profiles like shown here?  http://developer.apple.com/qa/qa2004/qa1396.html

#include <SysCGColorSpace.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	CFString														  [public]
/**
	Default constructor	*/

CGColorSpace::CGColorSpace()
{
}


// ---------------------------------------------------------------------------
//	CFString														  [public]
/**
	Construct from a CGColorSpaceRef

	@param	inColorSpaceRef	CG color space reference
	@param	inRetain		Whether to retain the CG color space
	
	@note Toolbox functions which return a CGColorSpaceRef and have the word
	"copy" or "create" in their name return a color space with a retain count
	of one. Pass false for inRetain when constructing from such a color space.
	The CGColorSpace won't retain the color space, but will release the color
	space in its destructor.
	
	Toolbox functions which return a CGColorSpaceRef and have the word "get"
	in their name return a color space without incrementing its retain count.
	Pass true for inRetain when constructing from such a color space.
	The CGColorSpace will retain the color space, and later release it.		*/

CGColorSpace::CGColorSpace(
	CGColorSpaceRef	inColorSpaceRef,
	bool			inRetain)
	
	: BaseT(inColorSpaceRef, inRetain)
{
}


// ---------------------------------------------------------------------------
//	GetSystemColorSpace												  [public]
/**
	Returns a color space that represents the main display.
	
	@return Color space that represents the main display					*/

CGColorSpace
CGColorSpace::GetSystemColorSpace()
{
	CMProfileRef	systemProfile; // ### Do we need a CMProfileRef wrapper?
	CGColorSpace	displayColorSpace;
	
	// Get the Systems Profile for the main display
	if (::CMGetSystemProfile(&systemProfile) == noErr)
	{
		// Create a colorspace with the systems profile
		displayColorSpace.AttachRef(
			::CGColorSpaceCreateWithPlatformColorSpace(systemProfile), retain_No);

		// Close the profile
		::CMCloseProfile(systemProfile);
	}
	
	return displayColorSpace;
}


// ---------------------------------------------------------------------------
//	GetDeviceGray													  [public]
/**
	Returns a DeviceGray device-dependent color space.
	
	@return DeviceGray color space											*/

CGColorSpace
CGColorSpace::GetDeviceGray()
{
	return CGColorSpace(::CGColorSpaceCreateDeviceGray(), retain_No);
}


// ---------------------------------------------------------------------------
//	GetDeviceRGB													  [public]
/**
	Returns a DeviceRGB device-dependent color space.
	
	@return DeviceRGB color space											*/

CGColorSpace
CGColorSpace::GetDeviceRGB()
{
	return CGColorSpace(::CGColorSpaceCreateDeviceRGB(), retain_No);
}


// ---------------------------------------------------------------------------
//	GetDeviceCMYK													  [public]
/**
	Returns a DeviceCMYK device-dependent color space.
	
	@return DeviceCMYK color space											*/

CGColorSpace
CGColorSpace::GetDeviceCMYK()
{
	return CGColorSpace(::CGColorSpaceCreateDeviceCMYK(), retain_No);
}


// ---------------------------------------------------------------------------
//	CFString														  [public]
/**
	Copy constructor */

CGColorSpace::CGColorSpace(
	const CGColorSpace&	inOriginal)
	
	: BaseT(inOriginal)
{
}


// ---------------------------------------------------------------------------
//	operator =														  [public]
/**
	Assignment Operator */

CGColorSpace&
CGColorSpace::operator = (
	const CGColorSpace&	inSource)
{
	AssignObject(inSource);
	return *this;
}

#pragma mark -

// ---------------------------------------------------------------------------
//	GetNumberOfComponents											  [public]
/**
	Returns the number of color components in the color space
	
	@return Number of color components in the color space					*/

CFIndex
CGColorSpace::GetNumberOfComponents() const
{
	return ::CGColorSpaceGetNumberOfComponents( UseRef() );
}


} // namespace PPx