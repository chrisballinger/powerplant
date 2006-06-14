// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLGAEditField.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/15/96
//     $Date: 2006/01/18 01:33:59 $
//  $History: VPLGAEditField.cpp $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 03/20/97   Time: 11:00
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Fixed cosmetic problems with redrawing LGAEditField when it resizes.
//	(Bug fix #1186.)
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:56
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:46
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 13:49
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added drawing agent.
//	
// ===========================================================================

#include "VPLGAEditField.h"

	// Core : Data model : Attributes
#include "DMIntegerAttribute.h"

	// MacOS : Editors : Views : Data model
#include "VPDataModel.h"


// ===========================================================================

#pragma mark *** VPLEditField ***

// ---------------------------------------------------------------------------
//		* ValueChangedSelf										[protected]
// ---------------------------------------------------------------------------
//	An attribute of this model object has changed. Update the display
//	accordingly.

void
VPLGAEditField::ValueChangedSelf(
	FourCharCode	inAttributeKey,
	DMAttribute*	inAttribute)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mDisplayPane);
	
	// Dispatch attribute key.

	switch (inAttributeKey) {

		// Width or height: Resize pane frame, but use pane's Refresh() method
		// since LGAEditField draws outside its bounds. :-(

		case pPaneWidth:
		case pPaneHeight:
			DMIntegerAttribute* widthAttr = mModelObject->FindIntegerAttribute(pPaneWidth);
			DMIntegerAttribute* heightAttr = mModelObject->FindIntegerAttribute(pPaneHeight);
			
			SInt32 width = widthAttr->GetIntValue();
			SInt32 height = heightAttr->GetIntValue();
			
			mDisplayPane->Refresh();

			if ((height < 0) || (width < 0))
				RebuildMe();
			else {
				mDisplayPane->SetRefreshAllWhenResized(true);
				mDisplayPane->ResizeFrameTo(width, height, true);
				mDisplayPane->Refresh();
			}
			break;
		
		// Otherwise, see if LEditField wants it.
		
		default:
			VPLEditField::ValueChangedSelf(inAttributeKey, inAttribute);

	}
}
