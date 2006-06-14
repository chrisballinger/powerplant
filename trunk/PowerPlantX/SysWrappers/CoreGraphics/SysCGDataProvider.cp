// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysCGDataProvider.cp
//
//	Copyright © 2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:32 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <SysCGDataProvider.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	CGDataProvider													  [public]
/**
	Default constructor	*/

CGDataProvider::CGDataProvider()
{
}


// ---------------------------------------------------------------------------
//	CGDataProvider													  [public]
/**
	Construct from a CGDataProviderRef

	@param	inProviderRef	CG data provider reference
	@param	inRetain		Whether to retain the CG data provider
	
	@note Toolbox functions which return a CGDataProviderRef and have the
	word "copy" or "create" in their name return a data provider with a
	retain count of one. Pass false for inRetain when constructing from such
	a data provider. The CGDataProvider won't retain the data provider, but
	will release the data provider in its destructor.
	
	Toolbox functions which return a CGDataProviderRef and have the word
	"get" in their name return a data provider without incrementing its
	retain count. Pass true for inRetain when constructing from such a data
	provider. The CGDataProvider will retain the data provider, and later
	release it.																*/

CGDataProvider::CGDataProvider(
	CGDataProviderRef	inProviderRef,
	bool				inRetain)
	
	: BaseT(inProviderRef, inRetain)
{
}


// ---------------------------------------------------------------------------
//	CGDataProvider													  [public]
/**
	Constructs a Quartz sequential-access data provider which uses callbacks
	to sequentially access the data
	
	@param	inCallbacks		A structure that specifies the callback functions
							you implement to handle the data provider’s basic
							memory management
	@param	inUserData		User data passed to callback functions			*/

CGDataProvider::CGDataProvider(
	const CGDataProviderCallbacks&	inCallbacks,
	void*							inUserData)
{
	CGDataProviderRef	ref = ::CGDataProviderCreate(inUserData, &inCallbacks);
	PPx_ThrowIfCFCreateFailed_(ref, "CGDataProviderCreate");
	
	AttachRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	CGDataProvider													  [public]
/**
	Constructs a Quartz direct-access data provider that uses data you supply
	
	@param	inData				A pointer to the array of data that the
								provider contains
	@param	inSize				A value that specifies the number of bytes
								that the data provider contains
	@param	inReleaseDataFunc	A pointer to a release callback for the data
								provider
	@param	inUserData			User data passed to inReleaseDataFunc		*/

CGDataProvider::CGDataProvider(
	void*	inData,
	size_t	inSize,
	void	(*inReleaseDataFunc)(void* inUserData, const void* inData, size_t inSize),
	void*	inUserData)
{
	CGDataProviderRef	ref = ::CGDataProviderCreateWithData(inUserData, inData,
									inSize, inReleaseDataFunc);
	PPx_ThrowIfCFCreateFailed_(ref, "CGDataProviderCreateWithData");
	
	AttachRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	CGDataProvider													  [public]
/**
	Constructs a Quartz direct-access data provider that uses a URL to supply data
	
	@param	inURL		URL for Quartz to use as the data provider			*/

CGDataProvider::CGDataProvider(
	CFURLRef	inURL)
{
	CGDataProviderRef	ref = ::CGDataProviderCreateWithURL(inURL);
	PPx_ThrowIfCFCreateFailed_(ref, "CGDataProviderCreateWithURL");
	
	AttachRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	CGDataProvider													  [public]
/**
	Copy constructor */

CGDataProvider::CGDataProvider(
	const CGDataProvider&	inOriginal)
	
	: BaseT(inOriginal)
{
}


// ---------------------------------------------------------------------------
//	operator =														  [public]
/**
	Assignment Operator */

CGDataProvider&
CGDataProvider::operator = (
	const CGDataProvider&	inSource)
{
	AssignObject(inSource);
	return *this;
}

} // namespace PPx