// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysCFTree.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:31 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <SysCFTree.h>
#include <SysCFUtils.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	CFTree															  [public]

/**	Default constructor

	@note Default construction does not create an underlying tree data
	structure. You must call AttachRef() to associate this object with a
	valid CFTreeRef before you can use it.									*/

CFTree::CFTree()
{
}


// ---------------------------------------------------------------------------
//	CFTree															  [public]

/** Constructs from a CFTreeRef.
	
	@note Although CFTrees are always mutable, the Toolbox does not have a
	function to copy a CFTree. So both the caller and this object share
	the CFTreeRef. Any change made to the tree will be reflected in all
	trees that share the same CFTreeRef.									*/

CFTree::CFTree(
	CFTreeRef	inTreeRef,
	bool		inRetain)
	
	: BaseT(inTreeRef, inRetain)
{
}


// ---------------------------------------------------------------------------
//	CFTree															  [public]
/**
	Constructs an empty tree from a context
	
	@param	inContext		CFTreeContext from which to make tree
	@param	inAllocator		CF Allocator									*/

CFTree::CFTree(
	const CFTreeContext&	inContext,
	CFAllocatorRef			inAllocator)
{
	CFTreeRef	ref = ::CFTreeCreate(inAllocator, &inContext);
	PPx_ThrowIfCFCreateFailed_(ref, "CFTreeCreate");

	AttachRef(ref, retain_No);
}


// ---------------------------------------------------------------------------
//	CFTree															  [public]

/** Copy constructor
	
	@note See comments for CFTree(CFTreeRef) about the CFTreeRef being
	shared rather than copied												*/

CFTree::CFTree(
	const CFTree&	inOriginal)
	
	: BaseT(inOriginal)
{
}


// ---------------------------------------------------------------------------
//	operator =														  [public]

/** Assignment operator
	
	@note See comments for CFTree(CFTreeRef) about the CFTreeRef being
	shared rather than copied												*/

CFTree&
CFTree::operator = (
	const CFTree&	inSource)
{
	AssignObject(inSource);
	return *this;
}

#pragma mark -

// ---------------------------------------------------------------------------
//	GetContext														  [public]
/**
	Passes back the context for the Tree

	@param	ioContext	CFTreeContext for the Tree
	
	@note Caller must fill in the version field of ioContext with a valid
	version number. See <CoreFouncation/CFTree.h> for CFTreeContext version
	numbers.																*/

void
CFTree::GetContext(
	CFTreeContext&	ioContext) const
{
	::CFTreeGetContext(UseRef(), &ioContext);
}


// ---------------------------------------------------------------------------
//	FindRoot														  [public]
/**
	Returns the root tree containing this Tree
	
	@return Root tree containing this Tree									*/

CFTree
CFTree::FindRoot() const
{
	return CFTree( ::CFTreeFindRoot(UseRef()), retain_Yes );
}


// ---------------------------------------------------------------------------
//	GetParent														  [public]
/**
	Returns the parent tree of this Tree
	
	@return Parent tree of this Tree										*/

CFTree
CFTree::GetParent() const
{
	return CFTree( ::CFTreeGetParent(UseRef()), retain_Yes );
}


// ---------------------------------------------------------------------------
//	GetNextSibling													  [public]
/**
	Returns the next sibling tree of this Tree
	
	@return Next sibling tree of this Tree									*/

CFTree
CFTree::GetNextSibling() const
{
	return CFTree( ::CFTreeGetNextSibling(UseRef()), retain_Yes );
}


// ---------------------------------------------------------------------------
//	GetChildCount													  [public]
/**
	Returns the number of children of this Tree
	
	@return Nnumber of children of this Tree								*/

CFIndex
CFTree::GetChildCount() const
{
	return ::CFTreeGetChildCount( UseRef() );
}


// ---------------------------------------------------------------------------
//	GetFirstChild													  [public]
/**
	Returns the first child tree of this Tree
	
	@return First child tree of this Tree									*/

CFTree
CFTree::GetFirstChild() const
{
	return CFTree( ::CFTreeGetFirstChild(UseRef()), retain_Yes );
}


// ---------------------------------------------------------------------------
//	GetChildAtIndex													  [public]
/**
	Returns the child tree at the given index
	
	@param	inIndex		Index of child tree
	
	@return Child tree at the given index									*/

CFTree
CFTree::GetChildAtIndex(
	CFIndex		inIndex) const
{
	CFUtils::VerifyIndex(GetChildCount(), inIndex);
	return CFTree( ::CFTreeGetChildAtIndex(UseRef(), inIndex), retain_Yes );
}


// ---------------------------------------------------------------------------
//	GetChildren														  [public]
/**
	Passes back an array of CFTreeRefs for all the children of this Tree
	
	@param	outChildren		Pointer to array of CFTreeRef					*/

void
CFTree::GetChildren(
	CFTreeRef*	outChildren) const
{
	::CFTreeGetChildren( UseRef(), outChildren );
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SetContext														  [public]
/**
	Sets the context for the Tree
	
	@param	inContext	CFTreeContext to use for this tree					*/

void
CFTree::SetContext(
	const CFTreeContext&	inContext)
{
	::CFTreeSetContext( UseRef(), &inContext );
}


// ---------------------------------------------------------------------------
//	RemoveFromParent												  [public]
/**
	Removes Tree from its parent tree										*/

void
CFTree::RemoveFromParent()
{
	::CFTreeRemove( UseRef() );
}


// ---------------------------------------------------------------------------
//	InsertSibling													  [public]
/**
	Inserts a sibling tree after this tree
	
	@param	inNewSibling	Tree to insert as a sibling after this tree		*/

void
CFTree::InsertSibling(
	CFTreeRef	inNewSibling)
{
	::CFTreeInsertSibling( UseRef(), inNewSibling );
}


// ---------------------------------------------------------------------------
//	PrependChild													  [public]
/**
	Adds a child tree as the first child of this Tree
	
	@param	inNewChild		Child tree to add								*/

void
CFTree::PrependChild(
	CFTreeRef	inNewChild)
{
	::CFTreePrependChild( UseRef(), inNewChild );
}


// ---------------------------------------------------------------------------
//	AppendChild														  [public]
/**
	Adds a child tree as the last child of this Tree
	
	@param	inNewChild		Child tree to add								*/

void
CFTree::AppendChild(
	CFTreeRef	inNewChild)
{
	::CFTreeAppendChild( UseRef(), inNewChild );
}


// ---------------------------------------------------------------------------
//	RemoveAllChildren												  [public]
/**
	Removes all child trees from this Tree									*/

void
CFTree::RemoveAllChildren()
{
	::CFTreeRemoveAllChildren( UseRef() );
}


// ---------------------------------------------------------------------------
//	ApplyFunctionToChildren											  [public]
/**
	Calls function for each child of the Tree
	
	@param	inFunction		Function to call
	@param	inParam			User-defined parameter for the function			*/

void
CFTree::ApplyFunctionToChildren(
	CFTreeApplierFunction	inFunction,
	void*					inParam)
{
	::CFTreeApplyFunctionToChildren( UseRef(), inFunction, inParam );
}


// ---------------------------------------------------------------------------
//	SortChildren													  [public]
/**
	Sorts child trees
	
	@param	inComparator	Comparison function for trees
	@param	inParam			User-defined parameter for comparison function	*/

void
CFTree::SortChildren(
	CFComparatorFunction	inComparator,
	void*					inParam)
{
	::CFTreeSortChildren( UseRef(), inComparator, inParam );
}


} // namespace PPx