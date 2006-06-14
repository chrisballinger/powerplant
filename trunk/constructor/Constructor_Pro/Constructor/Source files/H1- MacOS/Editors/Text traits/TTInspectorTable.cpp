// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	TTInspectorTable.cpp		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 10/29/96
//	   $Date: 2006/01/18 01:33:53 $
//	$History: TTInspectorTable.cpp $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:18
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Text traits
//	Improved commenting.
//
//	*****************  Version 2   *****************
//	User: scouten      Date: 01/24/97   Time: 17:43
//	Checked in '$/Constructor/Source files/H1- MacOS/Editors/Text traits'
//	Comment: Fixed CR/LF problem
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/30/96   Time: 18:33
//	Created
//	Comment: Baseline Txtr editor.
//	
// ===========================================================================

#include "TTInspectorTable.h"

	// Core : Data model : Attributes
#include "DMContainerAttribute.h"

	// MacOS : Editors : Text traits
#include "TTContainerItem.h"

	// PowerPlant : GA : Grayscale utilities
#include <UGAColorRamp.h>


// ===========================================================================

#pragma mark *** TTInspectorTable ***

// ---------------------------------------------------------------------------
//		* TTInspectorTable()
// ---------------------------------------------------------------------------
//	Constructor

TTInspectorTable::TTInspectorTable(
	LStream*	inStream)

: PIInspectorTable(inStream)

{
}


// ---------------------------------------------------------------------------
//		* ~TTInspectorTable
// ---------------------------------------------------------------------------
//	Destructor

TTInspectorTable::~TTInspectorTable()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** element accessors

// ---------------------------------------------------------------------------
//		* SetElement
// ---------------------------------------------------------------------------
//	Overriden to disallow changing elements on the fly. (This is the normal
//	behavior for OVTable; changing elements is added in PIInspectorTable.)

void
TTInspectorTable::SetElement(
	DMElement*	inElement)
{
	OVTable::SetElement(inElement);
}


// ===========================================================================

#pragma mark -
#pragma mark ** drawing behaviors

// ---------------------------------------------------------------------------
//		* DrawSelf												[protected]
// ---------------------------------------------------------------------------
//	Overriden to erase the table's background to white. The Txtr window
//	has a gray background, except for the inspector area.

void
TTInspectorTable::DrawSelf()
{

	// Validate input parameters.

	ValidateThis_();
	
	// Erase background to white.
	
	Rect frame;
	CalcLocalFrameRect(frame);
	RGBColor theColor = UGAColorRamp::GetWhiteColor();
	::RGBBackColor(&theColor);
	::EraseRect(&frame);

	// Draw table contents.

	PIInspectorTable::DrawSelf();

}


// ---------------------------------------------------------------------------
//		* CreateItemForElementSelf								[protected]
// ---------------------------------------------------------------------------
//	If creating a container item, use the Txtr variant which has
//	subtler drawing.

OVItem*
TTInspectorTable::CreateItemForElementSelf(
	DMElement*	inElement)
{

	// Validate input parameters.

	ValidateObject_(inElement);

	// Overriden to catch container attributes.

	if ((dynamic_cast<DMContainerAttribute*>(inElement)))
		return new TTContainerItem;

	// Not a container, just use standard item factory.
	
	return OVListener::CreateItemForElementSelf(inElement);
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** configuration

// ---------------------------------------------------------------------------
//		* FinishCreateSelf										[protected]
// ---------------------------------------------------------------------------
//	Overriden to set preferred column sizes.

void
TTInspectorTable::FinishCreateSelf()
{
	ValidateThis_();

	PIInspectorTable::FinishCreateSelf();
	SetColWidth(114, 1, 1);			// override column width
}
