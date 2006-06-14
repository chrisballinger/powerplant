// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLGATabPanel.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 12/17/96
//     $Date: 2006/01/18 01:34:01 $
//  $History: VPLGATabPanel.cpp $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/19/97   Time: 19:53
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:47
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 12/21/96   Time: 18:57
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Fixed drawing problems with LGATabPanel. (Bug fix #1152.)
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 12/17/96   Time: 14:22
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added class.
//	
// ===========================================================================

#include "VPLGATabPanel.h"

	// MacOS : Editors : Views : Data model
#include "VPDataModel.h"


// ===========================================================================
//		* VPFGATabPanel
// ===========================================================================
//	Skanky hack #1152.

class VPFGATabPanel : public LGATabPanel {
	friend class VPLGATabPanel;
};


// ===========================================================================

#pragma mark *** VPLGATabPanel ***

// ---------------------------------------------------------------------------
//		* ValueChangedSelf										[protected]
// ---------------------------------------------------------------------------
//	An attribute of this model object has changed. Update the display
//	accordingly.

void
VPLGATabPanel::ValueChangedSelf(
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

		// Most LGATabPanel parameters trigger a rebuild.

		case pPaneTextTraits:
		case 'pSTR':				//! TEMPORARY: magic numbers
		case 'pISI':
		case 'pDES':
			RebuildMe();
			break;
			
		// Otherwise, see if LView wants it.

		default:
			VPLView::ValueChangedSelf(inAttributeKey, inAttribute);

	}
}

// ---------------------------------------------------------------------------
//		* 
// ---------------------------------------------------------------------------

void
VPLGATabPanel::ContainerListChanged(
	DM_ListChange*	inMessage)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inMessage->mContainer);
	
	// If a title is being added or removed, rebuild the tab panel.
	
	DMAttribute* attr = (dynamic_cast<DMAttribute*>(inMessage->mContainer));
	ValidateObject_(attr);
	
	if (attr->GetKey() == 'pTPT')
		RebuildMe();
	
}

// ---------------------------------------------------------------------------
//		* 
// ---------------------------------------------------------------------------

void
VPLGATabPanel::DrawDisplayObject()
{

	// Validate pointers.
	
	ValidateThis_();

	// See if we've created a display pane for this object.
	// If so, we can provide default behavior for drawing.
	
	LPane* pane = GetDisplayPane();
	if (pane == nil)
		return;
	ValidateObject_(pane);

	// There is a pane, ask it to draw itself, but not subpanes.
	
	if (pane->FocusDraw()) {
		pane->DrawSelf();
		
		LView* panels = ((VPFGATabPanel*) pane)->GetTabButtonView();
		ValidateObject_(panels);

		panels->Draw(nil);
		
	}
}


