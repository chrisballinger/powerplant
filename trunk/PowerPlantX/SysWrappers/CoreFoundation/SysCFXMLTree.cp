// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysCFXMLTree.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:32 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <SysCFXMLTree.h>
#include <CoreFoundation/CoreFoundation.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	CFXMLTree														  [public]
/**
	Default constructor

	Default construction does not create an underlying tree data
	structure. You must call AttachRef() to associate this object with a
	valid CFXMLTreeRef before you can use it.								*/

CFXMLTree::CFXMLTree()
{
}


// ---------------------------------------------------------------------------
//	CFXMLTree														  [public]
/**
	Constructs from a CFXMLTreeRef
	
	@param	inTreeRef	CFXMLTreeRef to use for this Tree
	@param	inRetain	Whether to retain the input CFXMLTreeRef

	@note Although CFXMLTreeRefs are always mutable, the Toolbox does not
	have a function to copy a CFXMLTreeRef. So both the caller and this
	object share the CFXMLTreeRef. Any change made to the tree will be
	reflected in all trees that share the same CFXMLTreeRef.				*/

CFXMLTree::CFXMLTree(
	CFXMLTreeRef	inTreeRef,
	bool			inRetain)
	
	: CFTree(inTreeRef, inRetain)
{
}


// ---------------------------------------------------------------------------
//	CFXMLTree														  [public]
/**
	Constructs from a CFDataRef containing XML text
	
	@param	inXMLData		XML Data
	@param	inSourceURL		Source URL for the XML data, may be nil
	@param	inOptions		XML parsing options
	@param	inNodeVersion	Node Version
	@param	inAllocator		CF Allocator
	
	Parses the xML data and builds a hierarchy of XML Trees/Nodes
	that corresponds the the structure of the XML data						*/

CFXMLTree::CFXMLTree(
	CFDataRef		inXMLData,
	CFURLRef		inSourceURL,
	CFOptionFlags	inOptions,
	CFIndex			inNodeVersion,
	CFAllocatorRef	inAllocator)
{
	CFXMLTreeRef	ref = ::CFXMLTreeCreateFromData(inAllocator, inXMLData,
									inSourceURL, inOptions, inNodeVersion);
	PPx_ThrowIfCFCreateFailed_(ref, "CFXMLTreeCreateFromData");

	AttachRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	CFXMLTree														  [public]
/**
	Constructs from a CFURLRef containing XML text
	
	@param	inSourceURL		URL for XML document
	@param	inOptions		XML parsing options
	@param	inNodeVersion	Node Version
	@param	inAllocator		CF Allocator
	
	Parses the xML data and builds a hierarchy of XML Trees/Nodes
	that corresponds the the structure of the XML data						*/

CFXMLTree::CFXMLTree(
	CFURLRef		inSourceURL,
	CFOptionFlags	inOptions,
	CFIndex			inNodeVersion,
	CFAllocatorRef	inAllocator)
{
	CFXMLTreeRef	ref = ::CFXMLTreeCreateWithDataFromURL(inAllocator,
									inSourceURL, inOptions, inNodeVersion);
	PPx_ThrowIfCFCreateFailed_(ref, "CFXMLTreeCreateWithDataFromURL");

	AttachRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	CFXMLTree														  [public]
/**
	Constructs from an XML Node
	
	@param	inNodeRef		XML Node for the Tree
	@param	inAllocator		CF Allocator
	
	Every XML Tree has an associated XML Node. The Tree stores
	information about the tree structure relationships (parent, sibling,
	and child trees) and the Node stores the XML entity information.		*/
	
CFXMLTree::CFXMLTree(
	CFXMLNodeRef	inNodeRef,
	CFAllocatorRef	inAllocator)
{
	CFXMLTreeRef	ref = ::CFXMLTreeCreateWithNode(inAllocator, inNodeRef);
	PPx_ThrowIfCFCreateFailed_(ref, "CFXMLTreeCreateWithNode");

	AttachRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	CFXMLTree														  [public]
/**
	Constructs from a CF Tree
	
	@param	inTree		CFTree object
	
	@note Although CFXMLTree is a subclass of CFTree, the underlying
	Core Foundation type is the same, as CFXMLTreeRef is typedef'd to
	CFTreeRef. So we allow construction of a CFXMLTree from a CFTree,
	but the caller is responsible for ensuring this is appropriate.			*/

CFXMLTree::CFXMLTree(
	const CFTree&	inTree)
	
	: CFTree(inTree)
{
}


// ---------------------------------------------------------------------------
//	CFXMLTree														  [public]
/**
	Copy constructor
	
	@note See comments for CFXMLTree(CFXMLTreeRef) about the CFXMLTreeRef
	being shared rather than copied											*/

CFXMLTree::CFXMLTree(
	const CFXMLTree&	inOriginal)
	
	: CFTree(inOriginal)
{
}


// ---------------------------------------------------------------------------
//	operator =														  [public]
/**
	Assignment operator
	
	@note See comments for CFXMLTree(CFXMLTreeRef) about the CFXMLTreeRef
	being shared rather than copied											*/

CFXMLTree&
CFXMLTree::operator = (
	const CFXMLTree&	inSource)
{
	AssignObject(inSource);
	return *this;
}


// ---------------------------------------------------------------------------
//	GetXMLData														  [public]
/**
	Generates XML text suitable for external output from the XML tree
	
	@param	inAllocator		CF Allocator									*/

CFData
CFXMLTree::GetXMLData(
	CFAllocatorRef	inAllocator) const
{
	return CFData( ::CFXMLTreeCreateXMLData(inAllocator, UseRef()),
				   retain_No );
}


// ---------------------------------------------------------------------------
//	GetNode															  [public]
/**
	Returns the XML node associated with the XML tree
	
	@return XML Node associated with the XML Tree							*/

CFXMLNode
CFXMLTree::GetNode() const
{
	return CFXMLNode( ::CFXMLTreeGetNode(UseRef()), retain_Yes );
}


} // namespace PPx