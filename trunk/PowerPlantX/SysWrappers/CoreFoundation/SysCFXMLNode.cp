// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysCFXMLNode.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:31 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <SysCFXMLNode.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	CFXMLNode														  [public]
/**
	Default constructor	*/

CFXMLNode::CFXMLNode()
{
}


// ---------------------------------------------------------------------------
//	CFXMLNode														  [public]
/**
	Constructs from a CFXMLNodeRef
	
	@param	inNodeRef	Node ref to use for this XMLNode
	@param	inRetain	Whether to retain the input node ref				*/

CFXMLNode::CFXMLNode(
	CFXMLNodeRef	inNodeRef,
	bool			inRetain)
	
	: BaseT(inNodeRef, inRetain)
{
}


// ---------------------------------------------------------------------------
//	CFXMLNode														  [public]
/**
	Constructs from a specified node type code and associated data
	
	@param	inType			Type of node
	@param	inDataString	String information for node
	@param	inInfoPtr		Pointer to extra data for the ndoe
	@param	inAllocator		CF Allocator,
	@param	inVersion		Node version
	
	The data pointed to by inInfoPtr depends on the type of the node.
	This is the generic constructor. Other constructors create specific
	node types and pass a reference to the struct containing the
	data for that type of node.												*/

CFXMLNode::CFXMLNode(
	CFXMLNodeTypeCode	inType,
	CFStringRef			inDataString,
	const void*			inInfoPtr,
	CFAllocatorRef		inAllocator,
	CFIndex				inVersion)
{
	CreateNode(inType, inDataString, inInfoPtr, inAllocator, inVersion);
}


// ---------------------------------------------------------------------------
//	CFXMLNode														  [public]
/**
	Constructs an XML Element node
	
	@param	inElementInfo	XML Element information
	@param	inTagName		Name for XML element tag
	@param	inAllocator		CF Allocator
	@param	inVersion		Node version									*/

CFXMLNode::CFXMLNode(
	const CFXMLElementInfo&	inElementInfo,
	CFStringRef				inTagName,
	CFAllocatorRef			inAllocator,
	CFIndex					inVersion)
{
	CreateNode(kCFXMLNodeTypeElement, inTagName, &inElementInfo,
				inAllocator, inVersion);
}


// ---------------------------------------------------------------------------
//	CFXMLNode														  [public]
/**
	Constructs an XML Document node
	
	@param	inDocInfo		Document information
	@param	inAllocator		CF Allocator
	@param	inVersion		Node version									*/

CFXMLNode::CFXMLNode(
	const CFXMLDocumentInfo&	inDocInfo,
	CFAllocatorRef				inAllocator,
	CFIndex						inVersion)
{
	CreateNode(kCFXMLNodeTypeDocument, nil, &inDocInfo,
				inAllocator, inVersion);
}


// ---------------------------------------------------------------------------
//	CFXMLNode														  [public]
/**
	Constructs an XML processing instruction node
	
	@param	inInstructionInfo	Processing instruction information
	@param	inTarget			XML Target
	@param	inAllocator			CF Allocator
	@param	inVersion			Node version								*/

CFXMLNode::CFXMLNode(
	const CFXMLProcessingInstructionInfo&	inInstructionInfo,
	CFStringRef								inTarget,
	CFAllocatorRef							inAllocator,
	CFIndex									inVersion)
{
	CreateNode(kCFXMLNodeTypeProcessingInstruction, inTarget,
					&inInstructionInfo, inAllocator, inVersion);
}


// ---------------------------------------------------------------------------
//	CFXMLNode														  [public]
/**
	Constructs a node requiring a single text string as its information
	
	@param	inText			Text string
	@param	inType			Type of node
	@param	inAllocator		CF Allocator
	@param	inVersion		Node version									*/

CFXMLNode::CFXMLNode(
	CFStringRef			inText,
	CFXMLNodeTypeCode	inType,
	CFAllocatorRef		inAllocator,
	CFIndex				inVersion)
{
	CreateNode(inType, inText, nil, inAllocator, inVersion);
}


// ---------------------------------------------------------------------------
//	CFXMLNode														  [public]
/**
	Copy constructor */

CFXMLNode::CFXMLNode(
	const CFXMLNode&	inOriginal)
	
	: BaseT(inOriginal)
{
}


// ---------------------------------------------------------------------------
//	operator =														  [public]
/**
	Assignment operator */

CFXMLNode&
CFXMLNode::operator =(
	const CFXMLNode&	inSource)
{
	AssignObject(inSource);
	return *this;
}


// ---------------------------------------------------------------------------
//	GetVersion														  [public]
/**
	Returns the node version
	
	@return Node version													*/

CFIndex
CFXMLNode::GetVersion() const
{
	return ::CFXMLNodeGetVersion( UseRef() );
}


// ---------------------------------------------------------------------------
//	GetTypeCode														  [public]
/**
	Returns the node type
	
	@return	Node type														*/

CFXMLNodeTypeCode
CFXMLNode::GetTypeCode() const
{
	return ::CFXMLNodeGetTypeCode( UseRef() );
}


// ---------------------------------------------------------------------------
//	GetString														  [public]
/**
	Returns the data string for the nade
	
	@return Data string for the node										*/

CFString
CFXMLNode::GetString() const
{
	return CFString( ::CFXMLNodeGetString(UseRef()), retain_Yes );
}


// ---------------------------------------------------------------------------
//	GetInfoPtr														  [public]
/**
	Returns a information pointer for the node
	
	@return Information pointer for the node
	
	The struct pointed to by the information pointer depends on the
	type of the node														*/

const void*
CFXMLNode::GetInfoPtr() const
{
	return ::CFXMLNodeGetInfoPtr( UseRef() );
}


// ---------------------------------------------------------------------------
//	CreateNode														 [private]
//
//	Private function for creating the Toolbox CF Node

void
CFXMLNode::CreateNode(
	CFXMLNodeTypeCode	inType,
	CFStringRef			inDataString,
	const void*			inInfoPtr,
	CFAllocatorRef		inAllocator,
	CFIndex				inVersion)
{
	CFXMLNodeRef	ref = ::CFXMLNodeCreate(inAllocator, inType, inDataString,
											inInfoPtr, inVersion);
	PPx_ThrowIfCFCreateFailed_(ref, "CFXMLNodeCreate");
	
	AttachRef(ref, retain_No);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	CFXMLElement
/**
	Constructs from an XML element node
	
	@param	inNode	Access the attributes of this element node				*/

CFXMLElement::CFXMLElement(
	const CFXMLNode&	inNode)
{
	if ( inNode.GetTypeCode() != kCFXMLNodeTypeElement) {
		PPx_Throw_(DataError, err_XMLFormat, "XML Node is not an element");
	}
	
		// Information for an element node contains CFArrayRef of attribute
		// names and a CFDictionaryRef of (attr name, attr value) pairs.
		// We put this items into PPx objects so we can easily look
		// up attribute values by name or index
	
	CFXMLElementInfo*	elemInfo = (CFXMLElementInfo*) inNode.GetInfoPtr();
	
	mAttrNames.AttachRef(elemInfo->attributeOrder, retain_Yes);
	mAttrValues.AttachRef(elemInfo->attributes, retain_Yes);
}


// ---------------------------------------------------------------------------
//	GetAttributeCount
/**
	Returns the number of attributes in the element
	
	@return Number of attributes in the element								*/

CFIndex
CFXMLElement::GetAttributeCount() const
{
	return mAttrNames.IsValid() ? mAttrNames.GetCount() : 0;
}


// ---------------------------------------------------------------------------
//	GetAttributeValue
/**
	Returns the value for the attribute specified by index
	
	@param	inAttrIndex		Attribute index number
	
	@result	Value of the attribute
	
	Attributes are indexed in the order that they appear in the XML data	*/

CFStringRef
CFXMLElement::GetAttributeValue(
	CFIndex	inAttrIndex) const
{
	return mAttrValues.GetValue(mAttrNames[inAttrIndex]);
}


// ---------------------------------------------------------------------------
//	GetAttributeValue
/**
	Returns the value of the attribute specify by name
	
	@param	inAttrName	Name of the attribute
	
	@result Value of the attribute											*/

CFStringRef
CFXMLElement::GetAttributeValue(
	CFStringRef	inAttrName) const
{
	return mAttrValues.GetValue(inAttrName);
}


} // namespace PPx