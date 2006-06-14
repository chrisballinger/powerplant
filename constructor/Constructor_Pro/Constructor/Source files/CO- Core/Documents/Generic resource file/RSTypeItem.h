// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RSTypeItem.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/01/96
//     $Date: 2006/01/18 01:33:03 $
//  $History: RSTypeItem.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 16:39
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:28
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 12/21/96   Time: 15:19
//	Updated in $/Constructor/Source files/Documents/+ Generic resource file
//	Added an override to AddSubItem which fixes the "changing a resource ID
//	doesn't cause it to be resorted" problem. (Bug fix #1078.)
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/05/96   Time: 15:03
//	Created in $/Constructor/Source files/Documents/+ Generic resource file
//	Resource editor shell overhaul.
//	
// ===========================================================================

#pragma once

	// Core : Documents : Generic resource file
#include "RSType.h"

	// Core : Display classes (Eric) : Outline table
#include "OVItem.h"

	// Editors : Generic resource editor
#include "RETypeEntry.h"


// ===========================================================================
//		* RSTypeItem
// ===========================================================================
//
//	PRTypeItem is the outline item for resource type entries.
//
// ===========================================================================

class RSTypeItem : public OVItem {

public:
							RSTypeItem();
	virtual					~RSTypeItem();

	// disclosure triangle

	virtual Boolean			CanExpand() const;

	// drawing setup

protected:
	virtual void			GetDrawContentsSelf(
									const STableCell&		inCell,
									SOutlineDrawContents&	ioDrawContents);
	virtual void			DrawRowAdornments(
									const Rect&				inLocalRowRect);

	// update notifications

	virtual void			ValueChanged(
									DM_ValueChange*			inMessage);
	virtual void			ContainerListChanged(
									DM_ListChange*			inMessage);

	virtual void			AddSubItem(
									LOutlineItem*			inSubItem,
									LOutlineItem*			inAfterItem);

	// resource list

	virtual void			ExpandSelf();
	virtual void			CollapseSelf();
	
	virtual void			DoubleClick(
									const STableCell&		inCell,
									const SMouseDownEvent&	inMouseDown,
									const SOutlineDrawContents&	inDrawContents,
									Boolean					inHitText);
	// configuration

	virtual void			SetElementSelf(
									DMElement*				inElement);


	// data members

protected:
	RSType*					mRSType;						// the data model resource type we're displaying
	RETypeEntry*			mTypeEntry;						// information about this type

};
