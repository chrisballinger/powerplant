// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysCFXMLTree.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:32 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		SysCFXMLTree.h
	@brief		Wrapper class for Core Foundation XML Tree
*/

#ifndef H_SysCFXMLTree
#define H_SysCFXMLTree
#pragma once

#include <SysCFTree.h>
#include <SysCFData.h>
#include <SysCFXMLNode.h>

#include <CoreFoundation/CoreFoundation.h>

namespace PPx {

// ===========================================================================
//	CFXMLTree
/**
	Wrapper class for Core Foundation XML Tree */

class	CFXMLTree : public CFTree {
public:
									// Constructors
						CFXMLTree();
						
						CFXMLTree(
								CFXMLTreeRef	inTreeRef,
								bool			inRetain);
						
	explicit			CFXMLTree(
								CFDataRef		inXMLData,
								CFURLRef		inSourceURL = nil,
								CFOptionFlags	inOptions = kCFXMLParserSkipWhitespace,
								CFIndex			inNodeVersion = kCFXMLNodeCurrentVersion,
								CFAllocatorRef	inAllocator = nil);
						
	explicit			CFXMLTree(
								CFURLRef		inSourceURL,
								CFOptionFlags	inOptions = kCFXMLParserSkipWhitespace,
								CFIndex			inNodeVersion = kCFXMLNodeCurrentVersion,
								CFAllocatorRef	inAllocator = nil);
						
	explicit			CFXMLTree(
								CFXMLNodeRef	inNodeRef,
								CFAllocatorRef	inAllocator = nil);
								
	explicit			CFXMLTree( const CFTree& inTree );
								
						CFXMLTree( const CFXMLTree& inOriginal );
						
	CFXMLTree&			operator = ( const CFXMLTree& inSource );
	
									// Accessors
	CFData				GetXMLData( CFAllocatorRef inAllocator = nil ) const;
	
	CFXMLNode			GetNode() const;
};

} // namespace PPx

#endif // H_SysCFXMLTree
