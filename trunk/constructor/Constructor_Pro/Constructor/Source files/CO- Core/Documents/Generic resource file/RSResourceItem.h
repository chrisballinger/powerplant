// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RSResourceItem.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/01/96
//     $Date: 2006/01/18 01:33:01 $
//  $History: RSResourceItem.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/28/97   Time: 16:18
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Fixed in-place editing so it scrolls properly. (Bug fix #1211.)
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:28
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/05/96   Time: 14:56
//	Created in $/Constructor/Source files/Documents/+ Generic resource file
//	Resource editor shell overhaul.
//	
// ===========================================================================

#pragma once

	// Core : Display classes (Eric) : Outline table
#include "OVItem.h"


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class RETypeEntry;
class RSResource;


// ===========================================================================
//		* RSResourceItem
// ===========================================================================
//
//	RSResourceItem is the outline item for individual resource entries.
//
// ===========================================================================

class RSResourceItem : public OVItem {

public:
							RSResourceItem();
	virtual					~RSResourceItem();

	// attribute accessors

	virtual DM_AttributeKey	GetAttributeKeyForCell(
									TableIndexT				inColumn) const;

	// drawing setup

protected:
	virtual void			GetDrawContentsSelf(
									const STableCell&		inCell,
									SOutlineDrawContents&	ioDrawContents);
	virtual void			ConfigureInPlaceEdit(
									const STableCell&		inCell,
									ResIDT&					outTextTraitsID,
									SInt16&					outMaxChars,
									UInt8&					outAttributes,
									TEKeyFilterFunc&		outKeyFilter);

	// resource editing

	virtual void			DoubleClick(
									const STableCell&		inCell,
									const SMouseDownEvent&	inMouseDown,
									const SOutlineDrawContents& inDrawContents,
									Boolean					inHitText);

	// has editor flag
	
	virtual void			ListenToMessage(
									MessageT			inMessage,
									void*				ioParam);

	virtual void			CheckHasEditor();

	// configuration

	virtual void			SetElementSelf(DMElement*		inElement);

	
	// data members

protected:
	RSResource*				mRSResource;					// the resource we're representing
	RETypeEntry*			mTypeEntry;						// information about its resource type
	Boolean					mHasEditor;						// true if the resource is open for editing

};
