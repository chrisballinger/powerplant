// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysCFTree.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:31 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		SysCFTree.h
	@brief		Wrapper class for Core Foundation Tree
*/

#ifndef H_SysCFTree
#define H_SysCFTree
#pragma once

#include <SysCFObject.h>
#include <CoreFoundation/CoreFoundation.h>

namespace PPx {

// ===========================================================================
//	CFTree
/**
	Wrapper class for Core Foundation Tree */

class	CFTree : public CFObject<CFTreeRef> {
public:
									// Constructors
						CFTree();
						
						CFTree(	CFTreeRef	inTreeRef,
								bool		inRetain);
						
	explicit			CFTree(	const CFTreeContext&	inContext,
								CFAllocatorRef			inAllocator = nil);
								
						CFTree( const CFTree& inOriginal );

	CFTree&				operator = ( const CFTree& inSource );

									// Accessors
	void				GetContext( CFTreeContext& ioContext ) const;

	CFTree				FindRoot() const;

	CFTree				GetParent() const;

	CFTree				GetNextSibling() const;

	CFIndex				GetChildCount() const;

	CFTree				GetFirstChild() const;

	CFTree				GetChildAtIndex( CFIndex inIndex ) const;

	void				GetChildren( CFTreeRef* outChildren ) const;

									// Mutators
	void				SetContext( const CFTreeContext& inContext );
	
	void				RemoveFromParent();

	void				InsertSibling( CFTreeRef inNewSibling );

	void				PrependChild( CFTreeRef inNewChild );

	void				AppendChild( CFTreeRef inNewChild );

	void				RemoveAllChildren();

	void				ApplyFunctionToChildren(
								CFTreeApplierFunction	inFunction,
								void*					inParam);

	void				SortChildren(
								CFComparatorFunction	inComparator,
								void*					inParam);

private:
	typedef CFObject<CFTreeRef>	BaseT;
};

} // namespace PPx

#endif // H_SysCFTree
