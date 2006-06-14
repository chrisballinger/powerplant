// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLGAIconSuite.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/15/96
//     $Date: 2006/04/12 08:48:30 $
//  $History: VPLGAIconSuite.cpp $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 15:01
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Updated for refactored RFMap classes.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 12/13/96   Time: 15:52
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Updated to Clint's drop 12/13/96.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/17/96   Time: 14:45
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Fixed a missing include.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 13:56
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added drawing agent.
//	
// ===========================================================================

#include "VPLGAIconSuite.h"

	// Core : Data model : Attributes
#include "DMIntegerAttribute.h"

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

#pragma mark *** VPLGAIconSuite ***

// ---------------------------------------------------------------------------
//		* CreateFromStream
// ---------------------------------------------------------------------------
//	Create the PowerPlant pane for LGAIconSuiteControl.

LPane*
VPLGAIconSuite::CreateFromStream(
	LStream*	inStream)
{
	return new VPFGAIconSuite(inStream);
}

// ---------------------------------------------------------------------------
//		* ValueChangedSelf										[protected]
// ---------------------------------------------------------------------------
//	An attribute of this model object has changed. Update the display
//	accordingly.

void
VPLGAIconSuite::ValueChangedSelf(
	FourCharCode	inAttributeKey,
	DMAttribute*	inAttribute)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mDisplayPane);
	ValidateObject_(mModelObject.GetObject());
	ValidateObject_(inAttribute);
	
	// Cast display pane to LGAIconSuite.
	
	LGAIconSuite* caption = (dynamic_cast<LGAIconSuite*>(mDisplayPane));
	ValidateObject_(caption);
	
	// Dispatch attribute key.

	switch (inAttributeKey) {

		// Most of the parameters to LGAIconSuite result in a rebuild.

		case pPaneIconResource:
		case pPaneIconSizeSelector:
		case pPaneIconPositionSelector:
		case pPaneTextTraits:
			RebuildMe();
			break;

		// Otherwise, see if LPane wants it.

		default:
			VPLPane::ValueChangedSelf(inAttributeKey, inAttribute);

	}
}


// ---------------------------------------------------------------------------
//		* ListenToMap											[protected]
// ---------------------------------------------------------------------------
//	We want to listen for change notices from graphics resources.

Boolean
VPLGAIconSuite::ListenToMap()
{
	return true;
}


// ---------------------------------------------------------------------------
//		* ResourceChanged										[protected]
// ---------------------------------------------------------------------------
//	See if the graphics resource we're drawing has changed. If so,
//	rebuild the button.

void
VPLGAIconSuite::ResourceChanged(
	RMResource*		inResource)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateSimpleObject_(inResource);
	
	// Cast our display pane to button.
	
	LGAIconMixin* button = (dynamic_cast<LGAIconMixin*>(mDisplayPane));
	ValidateObject_(button);
	
	// Cast the changed resource to a MacOS resource.
	
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
				return;  //since RebuildMe will delete us, have to return
		}

	}
	
 	// Let inherited grab this notification.

	VPLPane::ResourceChanged(inResource);

}


// ===========================================================================

#pragma mark -
#pragma mark *** VPFGAIconSuite ***

// ---------------------------------------------------------------------------
//		* VPFGAIconSuite(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor

VPFGAIconSuite::VPFGAIconSuite(
	LStream*	inStream)

: LGAIconSuite(inStream)

{
}


// ---------------------------------------------------------------------------
//		* LoadIconSuiteHandle
// ---------------------------------------------------------------------------
//	Override to restrict the icons to the proper resource file

void
VPFGAIconSuite::LoadIconSuiteHandle()
{
	RFMap* map = VELayoutView::GetCurrentMap();
	if (map != nil) {
		ValidateObject_(map);
		mIconSuite = UIconUtilities::GetIconSuiteFromMap(map, mIconSuiteID, svAllAvailableData);
	}
}
