// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	CTOutlineItem.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 12/10/96
//     $Date: 2006/01/18 01:33:50 $
//  $History: CTOutlineItem.cpp $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:02
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Custom types
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:00p
//	Updated in $/ConstructorWin/Sources
//	Initial merge with MSVC build
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:42
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Custom types
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 12/10/96   Time: 14:23
//	Created in $/Constructor/Source files/Editors/Custom types
//	Baseline CTYP editor.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "CTOutlineItem.h"

	// Core : Display classes (Eric) : Outline table
#include "OVTable.h"

	// MacOS : Editors : Custom types
#include "CTModelObject.h"


// ===========================================================================

#pragma mark *** CTOutlineItem ***

// ---------------------------------------------------------------------------
//		* CTOutlineItem()
// ---------------------------------------------------------------------------
//	Constructor

CTOutlineItem::CTOutlineItem()
{
	mModelObject = nil;
	mIndentSize = 12;
}


// ---------------------------------------------------------------------------
//		* ~CTOutlineItem
// ---------------------------------------------------------------------------
//	Destructor

CTOutlineItem::~CTOutlineItem()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** attribute accessors

// ---------------------------------------------------------------------------
//		* GetAttributeKeyForCell
// ---------------------------------------------------------------------------
//	Tell me what attribute is in a given cell.

DM_AttributeKey
CTOutlineItem::GetAttributeKeyForCell(
	TableIndexT		inColumn) const
{
	switch (inColumn) {
		case 2:
			return 'cSIZ';
		case 3:
			return 'cNAM';
		case 4:
			return 'cDEF';
		default:
			return 0;
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** drawing setup

// ---------------------------------------------------------------------------
//		* GetDrawContentsSelf									[protected]
// ---------------------------------------------------------------------------
//	Return the contents of each drawing cell.

void
CTOutlineItem::GetDrawContentsSelf(
	const STableCell&		inCell,
	SOutlineDrawContents&	ioDrawContents)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mOVTable);
	ValidateObject_(mModelObject);
	
	// Create a temporary string.

	LStr255 tempStr;

	// Dispatch column number.

	switch (inCell.col) {
	
		// Column 1: Data type.
	
		case 1: {
			mModelObject->GetDisplayableObjectType(tempStr);
			LString::CopyPStr(tempStr, ioDrawContents.outTextString);
			ioDrawContents.outTextTraits.style |= bold;
			ioDrawContents.outShowSelection = true;
			break;
		}
		
		// Column 2: Data type size.
		
		case 2: {
			DMAttribute* sizeAttr = mModelObject->FindAttributeByKey('cSIZ');
			if (sizeAttr != nil) {
				sizeAttr->GetTextValue(tempStr);
				LString::CopyPStr(tempStr, ioDrawContents.outTextString);
				ioDrawContents.outCanDoInPlaceEdit = true;
			}
			break;
		}
		
		// Column 3: Attribute title.
		
		case 3: {
			DMAttribute* titleAttr = mModelObject->FindAttributeByKey('cNAM');
			if (titleAttr != nil) {
				titleAttr->GetTextValue(tempStr);
				LString::CopyPStr(tempStr, ioDrawContents.outTextString);
				ioDrawContents.outCanDoInPlaceEdit = true;
			}
			break;
		}

		// Column 3: Default value.
		
		case 4: {
			DMAttribute* dfltAttr = mModelObject->FindAttributeByKey('cDEF');
			if (dfltAttr != nil) {
				dfltAttr->GetTextValue(tempStr);
				LString::CopyPStr(tempStr, ioDrawContents.outTextString);
				ioDrawContents.outCanDoInPlaceEdit = true;
			}
			break;
		}
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** configuration

// ---------------------------------------------------------------------------
//		* SetElementSelf										[protected]
// ---------------------------------------------------------------------------
//	Make sure that the element is really a CTModelObject.

void
CTOutlineItem::SetElementSelf(
	DMElement*	inElement)
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inElement);
	
	// Make sure we really have a CTModelObject.

	mModelObject = (dynamic_cast<CTModelObject*>(inElement));
	ValidateObject_(mModelObject);

	// Specify container for submodels.
	
	DMElement* subPanes = mModelObject->FindAttributeByKey(CTModelObject::attrKey_AttributeList);
	if (subPanes != nil) {
		ValidateObject_(subPanes);
		mExpanded = true;
		SetContainer(subPanes);
	}
}
