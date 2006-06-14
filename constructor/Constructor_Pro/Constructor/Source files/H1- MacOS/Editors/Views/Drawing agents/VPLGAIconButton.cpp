// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLGAIconButton.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/15/96
//     $Date: 2006/01/18 01:33:59 $
//  $History: VPLGAIconButton.cpp $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 15:01
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Updated for refactored RFMap classes.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 12/13/96   Time: 15:52
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Updated to Clint's drop 12/13/96.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 13:53
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added drawing agent.
//	
// ===========================================================================

#include "VPLGAIconButton.h"

	// Core : Editors : Views : User interface
#include "VELayoutView.h"

	// Core : Utilities
#include "UIconUtilities.h"

	// MacOS : Editors : Views : Data model
#include "VPDataModel.h"

	// MacOS : Resource manager
#include "RFMap.h"
#include "RFResource.h"


// ===========================================================================

#pragma mark *** VPLGAIconButton ***

// ---------------------------------------------------------------------------
//		* CreateFromStream
// ---------------------------------------------------------------------------
//	Create the PowerPlant pane for LGAIconTextButton

LPane*
VPLGAIconButton::CreateFromStream(
	LStream*	inStream)
{
	return new VPFGAIconButton(inStream);
}


// ---------------------------------------------------------------------------
//		* ValueChangedSelf										[protected]
// ---------------------------------------------------------------------------
//	An attribute of this model object has changed. Update the display
//	accordingly.

void
VPLGAIconButton::ValueChangedSelf(
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

		// Most parameters to LGAIconButton result in a rebuild.

		case pPaneIconResource:
		case pPaneIconSizeSelector:
		case pPaneIconPositionSelector:
		case pPaneIconHiliteIcon:
		case pPaneIconOffsetOnHilite:
		case pPaneTextTraits:
			RebuildMe();
			break;

		// Otherwise, see if LControl wants it.

		default:
			VPLControl::ValueChangedSelf(inAttributeKey, inAttribute);

	}
}


// ---------------------------------------------------------------------------
//		* ListenToMap											[protected]
// ---------------------------------------------------------------------------
//	We want to listen for change notices from graphics resources.

Boolean
VPLGAIconButton::ListenToMap()
{
	return true;
}


// ---------------------------------------------------------------------------
//		* ResourceChanged										[protected]
// ---------------------------------------------------------------------------
//	See if the graphics resource we're drawing has changed. If so,
//	rebuild the button.

void
VPLGAIconButton::ResourceChanged(
	RMResource*		inResource)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateSimpleObject_(inResource);
	
	// Cast our display pane to button.
	
	LGAIconMixin* button = (dynamic_cast<LGAIconMixin*>(mDisplayPane));
	ValidateObject_(button);
	
	// Cast the changed resource to a Mac resource.
	
	RFResource* changedRF = (dynamic_cast<RFResource*>(inResource));
	ValidateObject_(changedRF);
	
	// Check the resource type and ID against the button's graphic type and ID.

	if (changedRF->GetResID() == button->GetIconResourceID()) {
	
		// ID matches... see if type matches.
		
		switch (changedRF->GetResType()) {
		
			case 'ICN#':
			case 'icl8':
			case 'icl4':
			case 'ics#':
			case 'ics8':
			case 'ics4':
			case 'icm#':
			case 'icm8':
			case 'icm4':
				RebuildMe();
				return;  // since RebuildMe will delete us, have to return
		}

	}
	
 	// Let inherited grab this notification.

	VPLControl::ResourceChanged(inResource);

}

// ===========================================================================

#pragma mark -
#pragma mark *** VPFGAIconButton ***

// ---------------------------------------------------------------------------
//		* VPFGAIconButton(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor

VPFGAIconButton::VPFGAIconButton(
	LStream*	inStream)

: LGAIconButton(inStream)

{
}


// ---------------------------------------------------------------------------
//		* LoadIconSuiteHandle
// ---------------------------------------------------------------------------
//	Override to restrict the icons to the proper resource file

void
VPFGAIconButton::LoadIconSuiteHandle()
{
	RFMap* map = VELayoutView::GetCurrentMap();
	if (map != nil)  {
		ValidateObject_(map);
		mIconSuite = UIconUtilities::GetIconSuiteFromMap(map, mIconSuiteID, svAllAvailableData);
	}
}
