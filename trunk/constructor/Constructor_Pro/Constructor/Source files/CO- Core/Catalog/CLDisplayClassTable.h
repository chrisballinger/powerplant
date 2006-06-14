// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	CLDisplayClassTable.h		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/26/96
//     $Date: 2006/01/18 01:32:29 $
//	$History: CLDisplayClassTable.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 03/18/97   Time: 15:46
//	Updated in $/Constructor/Source files/CO- Core/Catalog
//	Diabled double-click for property inspector in catalog window. (Bug fix
//	#1308.)
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/20/97   Time: 13:23
//	Updated in $/Constructor/Source files/CO- Core/Catalog
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:18
//	Updated in $/Constructor/Source files/CO- Core/Catalog
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/30/96   Time: 17:46
//	Created in $/Constructor/Source files/Catalog
//	Added class (adapted from old VEDisplayClassTable).
//	
// ===========================================================================

#pragma once

	// Core : Data model : Utilities
#include "DMComparator.h"

	// Core : Display classes (Eric) : Outline table
#include "OVTable.h"
#include "OVItem.h"
#include "OVItemFactory.h"


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class VECatalogPage;
class VETypeEntry;


// ===========================================================================
//		* CLDisplayClassTable
// ===========================================================================
//
//	CLDisplayClassTable presents the list of display classes known
//	to this view editor. It relies on information provided by a
//	VETypeList.
//
// ===========================================================================

class CLDisplayClassTable : public OVTable {

public:
	enum { class_ID = 'CLDC' };
	
	static LPane*			CreateFromStream(LStream* inStream)
									{ return new CLDisplayClassTable(inStream); }

							CLDisplayClassTable(LStream* inStream);
	virtual					~CLDisplayClassTable();

	// menu command status
	
	virtual void			FindCommandStatus(
									CommandT			inCommand,
									Boolean&			outEnabled,
									Boolean&			outUsesMark,
									UInt16&				outMark,
									Str255				outName);

	// configuration
	
	inline Boolean			GetAlphabeticDisplay() const
									{ return mAlphabeticSort; }
	virtual void			SetAlphabeticDisplay(
									Boolean				inAlphaSort);

	// internal configuration

protected:
	virtual void			FinishCreateSelf();

	virtual void			SetElement(
									DMElement*			inElement);
	virtual void			SetElementSelf(
									DMElement*			inElement);
	
	virtual void			AddSubClasses(
									DMElement*			inContainerClass);
	
	// custom resizing behaviors

	virtual void			CalcStandardSize(
									GReferencedWindow*	inWindow);

	virtual void			ResizeFrameBy(
									SInt16				inWidthDelta,
									SInt16				inHeightDelta,
									Boolean				inRefresh);


	// data members

protected:
	VECatalogPage*			mCatalogPage;				// catalog page we're displaying
	Boolean					mAlphabeticSort;			// true to sort classes by name


	friend class CLWindow;

};


// ===========================================================================
//		* CLClassItemFactory
// ===========================================================================
//	CLClassItemFactory creates CLTypeEntryItems for each display class.

class CLClassItemFactory : public OVItemFactory {

public:
							CLClassItemFactory() {}
	virtual					~CLClassItemFactory() {}

	virtual OVItem*			CreateItemForElement(DMElement* inElement);

};


// ===========================================================================
//		* CLTypeEntryItem
// ===========================================================================
//	CLTypeEntryItem is the outline item for a single display class.

class CLTypeEntryItem : public OVItem {

public:
								CLTypeEntryItem();
	virtual						~CLTypeEntryItem();

	// configuration

protected:
	virtual void				SetElementSelf(
										DMElement*				inElement);
	// drawing
	
	virtual void				GetDrawContentsSelf(
										const STableCell&		inCell,
										SOutlineDrawContents&	ioDrawContents);

	// drag-and-drop behaviors

	virtual void				TrackDrag(
										const STableCell&		inCell,
										const SMouseDownEvent&	inMouseDown,
										const SOutlineDrawContents&	inDrawContents);
	virtual void				MakeDragRegion(
										const STableCell&		inCell,
										RgnHandle				outGlobalDragRgn,
										Rect&					outGlobalItemBounds);
	virtual void			DoubleClick(
									const STableCell&		inCell,
									const SMouseDownEvent&	inMouseDown,
									const SOutlineDrawContents&	inDrawContents,
									Boolean					inHitText);



	// data members

protected:
	VETypeEntry*				mTypeEntry;

};


// ===========================================================================
//		* CLClassNameComparator
// ===========================================================================
//	CLClassNameComparator compares display class entries by their
//	class name.

class CLClassNameComparator : public DMComparator {

public:
							CLClassNameComparator() {}
	virtual					~CLClassNameComparator() {}

	virtual SInt32			CompareElements(
									const DMElement*	inElement1,
									const DMElement*	inElement2) const;

};
