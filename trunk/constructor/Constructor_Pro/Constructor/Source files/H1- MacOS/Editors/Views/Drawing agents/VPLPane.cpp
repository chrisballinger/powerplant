// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLPane.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/13/96
//     $Date: 2006/01/18 01:34:01 $
//  $History: VPLPane.cpp $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/18/97   Time: 16:53
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Fixed drawing agent so it forces a redraw-all before resizing the
//	object. (Bug fix #1279.)
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:48
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Fixed CR/LF problem
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/21/97   Time: 14:27
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Added code to ValueChangedSelf to prevent redraws when the object is
//	invisible.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/24/96   Time: 21:59
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Fixed a bug in ValueChangedSelf. It now responds to changes in the
//	visible attribute.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 14:04
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added drawing agent.
//	
// ===========================================================================

#include "VPLPane.h"

	// Core : Data model : Attributes
#include "DMIntegerAttribute.h"

	// MacOS : Editors : Views : Data model
#include "VPDataModel.h"


// ===========================================================================

#pragma mark *** VPLPane ***

// ---------------------------------------------------------------------------
//		* ValueChangedSelf										[protected]
// ---------------------------------------------------------------------------
//	An attribute of this model object has changed. Update the display
//	accordingly.

void
VPLPane::ValueChangedSelf(
	FourCharCode	inAttributeKey,
	DMAttribute*	inAttribute)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mDisplayPane);
	ValidateObject_(mModelObject.GetObject());
	ValidateObject_(inAttribute);
	
	// Dispatch attribute key.

	switch (inAttributeKey) {
	
		// Object class: Need to rebuild.

//		case pObjectClass:
//			RebuildMe();
//			break;
		
		// Descriptor: Change title and redraw.
		
		case pPaneDescriptor: {
			LStr255 title;
			inAttribute->GetTextValue(title);
			mDisplayPane->SetDescriptor(title);
			if (mDisplayPane->IsVisible())
				mDisplayPane->Refresh();
			break;
		}
		
		// Pane ID: Set pane ID (not sure this is necessary).

		case pPaneID: {
			DMIntegerAttribute* idAttr = (dynamic_cast<DMIntegerAttribute*>(inAttribute));
			ValidateObject_(idAttr);
			mDisplayPane->SetPaneID(idAttr->GetIntValue());
			if (mDisplayPane->IsVisible())
				mDisplayPane->Refresh();
			break;
		}
		
		// Width or height: Resize pane frame.

		case pPaneWidth:
		case pPaneHeight: {
			DMIntegerAttribute* widthAttr = mModelObject->FindIntegerAttribute(pPaneWidth);
			DMIntegerAttribute* heightAttr = mModelObject->FindIntegerAttribute(pPaneHeight);
			
			SInt32 width = widthAttr->GetIntValue();
			SInt32 height = heightAttr->GetIntValue();
			
			if ((height < 0) || (width < 0))
				RebuildMe();
			else {
				mDisplayPane->SetRefreshAllWhenResized(true);
				mDisplayPane->ResizeFrameTo(width, height, true);
			}
			
			break;
		}
		
		// Enabled flag: Enable or disable pane as appropriate.
		
		case pPaneEnabled: {
			DMIntegerAttribute* enabledAttr = (dynamic_cast<DMIntegerAttribute*>(inAttribute));
			ValidateObject_(enabledAttr);
			
			if (enabledAttr->GetIntValue() != 0)
				mDisplayPane->Enable();
			else
				mDisplayPane->Disable();

			if (mDisplayPane->IsVisible())
				mDisplayPane->Refresh();
			break;
		}
		
		// Visible flag: Redraw pane. (Layout view will take care of hiding it.)
		
		case pPaneVisible:
			mDisplayPane->Refresh();
			break;
		
		// Pane origin: Relocate frame (unless this is the root pane).
		
		case pPaneLeftOrigin:
		case pPaneTopOrigin:
			if (!mCantMove) {
				DMIntegerAttribute* leftAttr = mModelObject->FindIntegerAttribute(pPaneLeftOrigin);
				DMIntegerAttribute* topAttr = mModelObject->FindIntegerAttribute(pPaneTopOrigin);
				
				SInt32 left = leftAttr->GetIntValue();
				SInt32 top = topAttr->GetIntValue();

				mDisplayPane->PlaceInSuperImageAt(left, top, true);
			}
			break;
		
	}
}
