// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLCicnButton.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/15/96
//     $Date: 2006/04/12 08:48:30 $
//  $History: VPLCicnButton.cpp $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 03/19/97   Time: 15:14
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Fixed drawing agent for LCicnButton so that it now loads cicn resources
//	0, 1, and 2 from system file.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 15:00
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Updated for refactored RFMap classes.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 12/13/96   Time: 15:51
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Updated to Clint's drop 12/13/96.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 11:26
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added drawing agent.
//	
// ===========================================================================

#include "VPLCicnButton.h"
	
	// Core : Data model : Attributes
#include "DMIntegerAttribute.h"

	// Core : Editors : Views : User interface
#include "VELayoutView.h"

	// Core : Resource manager
//#include "StResourceContext.h"
#include <UResourceMgr.h>

	// MacOS : Editors : Views : Data model
#include "VPDataModel.h"

	// MacOS : Resource manager
#include "RFMap.h"
#include "RFResource.h"


// ===========================================================================

#pragma mark *** VPLCicnButton ***

// ---------------------------------------------------------------------------
//		* CreateFromStream
// ---------------------------------------------------------------------------
//	Create the PowerPlant display class for LCicnButton.

LPane*
VPLCicnButton::CreateFromStream(
	LStream*	inStream)
{
	return new VPFCicnButton(inStream);
}


// ---------------------------------------------------------------------------
//		* ValueChangedSelf										[protected]
// ---------------------------------------------------------------------------
//	An attribute of this model object has changed. Update the display
//	accordingly.

void
VPLCicnButton::ValueChangedSelf(
	FourCharCode	inAttributeKey,
	DMAttribute*	inAttribute)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mDisplayPane);
	ValidateObject_(mModelObject.GetObject());
	ValidateObject_(inAttribute);
	
	// Cast display pane to LCicnButton.
	
	LCicnButton* scroller = (dynamic_cast<LCicnButton*>(mDisplayPane));
	ValidateObject_(scroller);
	
	// Dispatch attribute key.

	switch (inAttributeKey) {
	
		// Graphics ID: Rebuild control

		case pPaneButtonNormalGraphicID:
		case pPaneButtonPushedGraphicID:
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
VPLCicnButton::ListenToMap()
{
	return true;
}


// ---------------------------------------------------------------------------
//		* ResourceChanged										[protected]
// ---------------------------------------------------------------------------
//	See if the graphics resource we're drawing has changed. If so,
//	rebuild the button.

void
VPLCicnButton::ResourceChanged(
	RMResource*		inResource)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateSimpleObject_(inResource);
	
	// Cast our display pane to button.
	
	VPFCicnButton* button = (dynamic_cast<VPFCicnButton*>(mDisplayPane));
	ValidateObject_(button);
	
	// Cast the resource to a Mac resource.
	
	RFResource* changedRF = (dynamic_cast<RFResource*>(inResource));
	ValidateObject_(changedRF);
	
	// Check the resource type and ID against the button's graphic type and ID.

	if ((changedRF->GetResID() == button->mNormalID)
	 && (changedRF->GetResType() == 'cicn')) {

		RebuildMe();
		return;
	}
	
	// Let inherited grab this notification.

	VPLControl::ResourceChanged(inResource);

}


// ===========================================================================

#pragma mark -
#pragma mark *** VPFCicnButton ***

// ---------------------------------------------------------------------------
//		* VPFCicnButton()
// ---------------------------------------------------------------------------
//	Constructor

VPFCicnButton::VPFCicnButton(
	LStream*	inStream)

: LCicnButton(inStream)

{

	// Reset graphics handles
	
	mIcon = nil;

	// Try to preload the graphic.
	
	LoadGraphic();

}


// ---------------------------------------------------------------------------
//		* ~VPFCicnButton
// ---------------------------------------------------------------------------
//	Destructor

VPFCicnButton::~VPFCicnButton()
{

	// Dispose of graphics resources we may have cached.

	if (mIcon != nil) {
		::DisposeCIcon(mIcon);
	}
}

// ---------------------------------------------------------------------------
//		* LoadGraphic											[protected]
// ---------------------------------------------------------------------------
//	Load the graphic resource to be drawn for this button.
//	Unlike LButton, we preload the resource and keep it for the lifetime
//	of the object. (The drawing agent for LButton will destroy this pane
//	if the graphic changes.)

void
VPFCicnButton::LoadGraphic()
{

	// Validate pointers.
	
	ValidateThis_();

	// If icon ID is 0, 1, or 2, load it from system resource file.
	
	if (mNormalID >= 0 && mNormalID <= 2) {
		StResourceContext sysContext((SInt16) 0);
		mIcon = ::GetCIcon(mNormalID);
	}
	else {

		// Load the appropriate graphic if possible.
		
		RFMap* map = VELayoutView::GetCurrentMap();
		if (map != nil) {
			ValidateObject_(map);
			RFResource* iconRsrc = map->FindResource('cicn', mNormalID, false);
			if (iconRsrc != nil) {
				//we don't actually use the RFResource; we just use it to make sure a cicn
				//exists in the project's map, so that GetCIcon doesn't grab one from some
				//other map
				mIcon = ::GetCIcon(mNormalID);
			}
		}
	}
}

// ---------------------------------------------------------------------------
//		* DrawSelf												[protected]
// ---------------------------------------------------------------------------
//	Overridden to ensure that 'cicn' resource is only fetched from user's
//	document.

void
VPFCicnButton::DrawSelf()
{
	Rect frame;
	CalcLocalFrameRect(frame);
	StColorPenState::Normalize();
	
	if (mIcon != nil) {
		ValidateHandle_((Handle)mIcon);
		::PlotCIconHandle(&frame, atNone, atNone, mIcon);
	}
	else {
		Pattern thePattern;
		UQDGlobals::GetLightGrayPat(&thePattern);
		::FillRect(&frame, &thePattern);
		::FrameRect(&frame);
	}
}
