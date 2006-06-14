// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysCFXMLNode.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:32 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		SysCFXMLNode.h
	@brief		Wrapper class for Core Foundation XML Node
*/

#ifndef H_SysCFXMLNode
#define H_SysCFXMLNode
#pragma once

#include <SysCFArray.h>
#include <SysCFDictionary.h>
#include <SysCFObject.h>
#include <SysCFString.h>
#include <CoreFoundation/CoreFoundation.h>

namespace PPx {

// ===========================================================================
//	CFXMLNode
/**
	Wrapper class for Core Foundation XML Node */

class	CFXMLNode : public CFObject<CFXMLNodeRef> {
public:
									// Constructors
						CFXMLNode();
						
						CFXMLNode(
								CFXMLNodeRef	inNodeRef,
								bool			inRetain);
						
						CFXMLNode(
								CFXMLNodeTypeCode	inType,
								CFStringRef			inDataString,
								const void*			inInfoPtr,
								CFAllocatorRef		inAllocator = nil,
								CFIndex				inVersion = kCFXMLNodeCurrentVersion);
						
						CFXMLNode(
								const CFXMLElementInfo&	inElementInfo,
								CFStringRef			inTagName,
								CFAllocatorRef		inAllocator = nil,
								CFIndex				inVersion = kCFXMLNodeCurrentVersion);
						
	explicit			CFXMLNode(
								const CFXMLDocumentInfo& inDocInfo,
								CFAllocatorRef		inAllocator = nil,
								CFIndex				inVersion = kCFXMLNodeCurrentVersion);
								
						CFXMLNode(
								const CFXMLProcessingInstructionInfo& inInstructionInfo,
								CFStringRef			inTarget,
								CFAllocatorRef		inAllocator = nil,
								CFIndex				inVersion = kCFXMLNodeCurrentVersion);
								
	explicit			CFXMLNode(
								CFStringRef			inText,
								CFXMLNodeTypeCode	inType = kCFXMLNodeTypeText,
								CFAllocatorRef		inAllocator = nil,
								CFIndex				inVersion = kCFXMLNodeCurrentVersion);
								
						CFXMLNode( const CFXMLNode& inOriginal );
						
	CFXMLNode&			operator = ( const CFXMLNode& inSource );
	
									// Accessors
	CFIndex				GetVersion() const;
	
	CFXMLNodeTypeCode	GetTypeCode() const;
	
	CFString			GetString() const;
	
	const void*			GetInfoPtr() const;
	
private:
	void				CreateNode(
								CFXMLNodeTypeCode	inType,
								CFStringRef			inDataString,
								const void*			inInfoPtr,
								CFAllocatorRef		inAllocator,
								CFIndex				inVersion);
	
	typedef CFObject<CFXMLNodeRef>		BaseT;
};


// ===========================================================================
//	CFXMLElement
/**
	Helper class for accessing the attributes of an XML Node for an element	*/

class	CFXMLElement {
public:
						CFXMLElement( const CFXMLNode& inNode );
						
	CFIndex				GetAttributeCount() const;
	
	CFStringRef			GetAttributeValue( CFIndex inAttrIndex ) const;
	
	CFStringRef			GetAttributeValue( CFStringRef inAttrName ) const;
						
private:
	CFArray<CFStringRef>					mAttrNames;
	CFDictionary<CFStringRef, CFStringRef>	mAttrValues;
};


} // namespace PPx

#endif // H_SysCFXMLNode
