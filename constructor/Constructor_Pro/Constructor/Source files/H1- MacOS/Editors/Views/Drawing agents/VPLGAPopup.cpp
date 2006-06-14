// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLGAPopup.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/15/96
//     $Date: 2006/04/12 08:48:30 $
//  $History: VPLGAPopup.cpp $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/19/97   Time: 19:52
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:47
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 13:59
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added drawing agent.
//	
// ===========================================================================

#include "VPLGAPopup.h"

	// Core : Data model : Attributes
#include "DMIntegerAttribute.h"

	// Core : Resource manager
//#include "StResourceContext.h"
#include <UResourceMgr.h>

	// MacOS : Editors : Views : Data model
#include "VPDataModel.h"


// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	MENU_GAPopupMenu		= 3509;


// ===========================================================================

#pragma mark *** VPLGAPopup ***

// ---------------------------------------------------------------------------
//		* CreateFromStream
// ---------------------------------------------------------------------------
//	Create the PowerPlant pane class for LGAPopup. Substitutes the
//	helper class VPFGAPopup.

LPane*
VPLGAPopup::CreateFromStream(
	LStream*	inStream)
{
	return new VPFGAPopup(inStream);
}


// ---------------------------------------------------------------------------
//		* ValueChangedSelf										[protected]
// ---------------------------------------------------------------------------
//	An attribute of this model object has changed. Update the display
//	accordingly.

void
VPLGAPopup::ValueChangedSelf(
	FourCharCode	inAttributeKey,
	DMAttribute*	inAttribute)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mDisplayPane);
	ValidateObject_(mModelObject.GetObject());
	ValidateObject_(inAttribute);
	
	// Cast display pane to LGAPopup.
	
	LGAPopup* scroller = (dynamic_cast<LGAPopup*>(mDisplayPane));
	ValidateObject_(scroller);
	
	// Dispatch attribute key.

	switch (inAttributeKey) {

		// Basically all parameters for LGAPopup result
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
	
		// Otherwise, see if LControl wants it.

		default:
			VPLControl::ValueChangedSelf(inAttributeKey, inAttribute);

	}
}


// ===========================================================================

#pragma mark -
#pragma mark *** VPFGAPopup ***

// ---------------------------------------------------------------------------
//		* VPFGAPopup(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor

VPFGAPopup::VPFGAPopup(
	LStream*	inStream)

: LGAPopup(inStream)

{
}


// ---------------------------------------------------------------------------
//		* ~VPFGAPopup
// ---------------------------------------------------------------------------
//	Destructor

VPFGAPopup::~VPFGAPopup()
{
}


// ---------------------------------------------------------------------------
//		* LoadPopupMenuH
// ---------------------------------------------------------------------------
//	Overriden to prevent the menu from loading MENU resources from the
//	application and then disposing them.

MenuHandle
VPFGAPopup::LoadPopupMenuH() const
{
	StApplicationContext appContext;
	MenuHandle menuH = ::GetMenu(MENU_GAPopupMenu);
	return menuH;
}
