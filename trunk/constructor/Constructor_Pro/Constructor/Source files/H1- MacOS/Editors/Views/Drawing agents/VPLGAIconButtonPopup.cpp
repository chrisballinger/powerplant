// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLGAIconButtonPopup.cpp	© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/15/96
//     $Date: 2006/01/18 01:33:59 $
//  $History: VPLGAIconButtonPopup.cpp $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:56
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:46
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 12/13/96   Time: 15:52
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Updated to Clint's drop 12/13/96.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 13:55
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added drawing agent.
//	
// ===========================================================================

#include "VPLGAIconButtonPopup.h"

	// Core : Resource manager
//#include "StResourceContext.h"
#include <UResourceMgr.h>

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
//		* Resource IDs
// ===========================================================================

const ResIDT	MENU_GAPopupMenu		= 3509;


// ===========================================================================

#pragma mark *** VPLGAIconButtonPopup ***

// ---------------------------------------------------------------------------
//		* CreateFromStream
// ---------------------------------------------------------------------------
//	Create the PowerPlant display class for LGAIconButtonPopup.
//	Substitute our VPFGAIconButtonPopup.

LPane*
VPLGAIconButtonPopup::CreateFromStream(
	LStream*	inStream)
{
	return new VPFGAIconButtonPopup(inStream);
}


// ---------------------------------------------------------------------------
//		* ValueChangedSelf										[protected]
// ---------------------------------------------------------------------------
//	An attribute of this model object has changed. Update the display
//	accordingly.

void
VPLGAIconButtonPopup::ValueChangedSelf(
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

		// Basically all parameters for LGAIconButtonPopup result
		// in a rebuild of the drawing agent.
		
		case pPaneMenuTitleBold:
		case pPaneMenuTitleUnderline:
		case pPaneMenuTitleItalic:
		case pPaneMenuTitleOutline:
		case pPaneMenuTitleShadow:
		case pPaneMenuTitleCondensed:
		case pPaneMenuTitleExtended:
		case pPaneMenuTitleJustification:
		case pPaneMenuTitleWidth:
		case pPaneMenuResourceID:
		case pPaneMenuAddResMenu:
		case pPaneMenuFixedWidth:
		case pPaneMenuInitialMenuItem:
		case pPaneTextTraits:
			RebuildMe();
			break;
	
		// Otherwise, see if LGAIconButton wants it.

		default:
			VPLGAIconButton::ValueChangedSelf(inAttributeKey, inAttribute);

	}
}


// ===========================================================================

#pragma mark -
#pragma mark *** VPFGAIconButtonPopup ***

// ---------------------------------------------------------------------------
//		* VPFGAIconButtonPopup()
// ---------------------------------------------------------------------------
//	Constructor

VPFGAIconButtonPopup::VPFGAIconButtonPopup(
	LStream*	inStream)

: LGAIconButtonPopup(inStream)

{
}


// ---------------------------------------------------------------------------
//		* ~VPFGAIconButtonPopup
// ---------------------------------------------------------------------------
//	Destructor

VPFGAIconButtonPopup::~VPFGAIconButtonPopup()
{
}


// ---------------------------------------------------------------------------
//		* LoadPopupMenuH
// ---------------------------------------------------------------------------
//	Overriden to prevent the menu from loading MENU resources from the
//	application and then disposing them.

MenuHandle
VPFGAIconButtonPopup::LoadPopupMenuH() const
{
	StApplicationContext appContext;
	MenuHandle menuH = ::GetMenu(MENU_GAPopupMenu);
	return menuH;
}


// ---------------------------------------------------------------------------
//		* LoadIconSuiteHandle
// ---------------------------------------------------------------------------
//	Override to restrict the icons to the proper resource file

void
VPFGAIconButtonPopup::LoadIconSuiteHandle()
{
	RFMap* map = VELayoutView::GetCurrentMap();
	if (map != nil) 
	{
		ValidateObject_(map);
		mIconSuite = UIconUtilities::GetIconSuiteFromMap(map, mIconSuiteID, svAllAvailableData);
	}
}
