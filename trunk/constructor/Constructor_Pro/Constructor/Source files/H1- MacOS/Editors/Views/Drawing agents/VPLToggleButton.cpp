// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLToggleButton.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/15/96
//     $Date: 2006/04/12 08:48:30 $
//  $History: VPLToggleButton.cpp $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 15:02
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Updated for refactored RFMap classes.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 12/13/96   Time: 15:52
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Updated to Clint's drop 12/13/96.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 14:18
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added drawing agent.
//	
// ===========================================================================

#include "VPLToggleButton.h"

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

#pragma mark *** VPLToggleButton ***

// ---------------------------------------------------------------------------
//		* CreateFromStream
// ---------------------------------------------------------------------------
//	Create the PowerPlant pane for LToggleButton.

LPane*
VPLToggleButton::CreateFromStream(
	LStream*	inStream)
{
	return new VPFToggleButton(inStream);
}


// ---------------------------------------------------------------------------
//		* ValueChangedSelf										[protected]
// ---------------------------------------------------------------------------
//	An attribute of this model object has changed. Update the display
//	accordingly.

void
VPLToggleButton::ValueChangedSelf(
	FourCharCode	inAttributeKey,
	DMAttribute*	inAttribute)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mDisplayPane);
	ValidateObject_(mModelObject.GetObject());
	ValidateObject_(inAttribute);
	
	// Cast display pane to LToggleButton.
	
	LToggleButton* scroller = (dynamic_cast<LToggleButton*>(mDisplayPane));
	ValidateObject_(scroller);
	
	// Dispatch attribute key.

	switch (inAttributeKey) {
	
		// All parameters to LToggleButton result in a rebuild.

		case pPaneButtonGraphicsType:
		case pPaneButtonOnGraphicID:
		case pPaneButtonOnClickedGraphicID:
		case pPaneButtonOffGraphicID:
		case pPaneButtonOffClickedGraphicID:
		case pPaneButtonTransitionGraphicID:
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
VPLToggleButton::ListenToMap()
{
	return true;
}


// ---------------------------------------------------------------------------
//		* ResourceChanged										[protected]
// ---------------------------------------------------------------------------
//	See if the graphics resource we're drawing has changed. If so,
//	rebuild the button.

void
VPLToggleButton::ResourceChanged(
	RMResource*		inResource)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateSimpleObject_(inResource);
	
	// Cast our display pane to button.
	
	VPFToggleButton* button = (dynamic_cast<VPFToggleButton*>(mDisplayPane));
	ValidateObject_(button);

	// Cast the changed resource to a MacOS resource.
	
	RFResource* changedRF = (dynamic_cast<RFResource*>(inResource));
	ValidateObject_(changedRF);
		
	// Check the resource type and ID against the button's graphic type and ID.

	Boolean rebuild = false;
	if (changedRF->GetResID() == button->mOnID) {
	
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
				if (button->mGraphicsType == 'ICN#')
					rebuild = true;
				break;
			default:
				if (button->mGraphicsType == changedRF->GetResType())
					rebuild = true;
				break;
		}

	}
	
	// If rebuild was called for do it now.
	
	if (rebuild) {
		RebuildMe();
		return;
	}

	// Let inherited grab this notification.

	VPLControl::ResourceChanged(inResource);

}


// ===========================================================================

#pragma mark -
#pragma mark *** VPFToggleButton ***

// ---------------------------------------------------------------------------
//		* VPFToggleButton(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor

VPFToggleButton::VPFToggleButton(
	LStream*	inStream)

: LToggleButton(inStream)

{

	// Reset graphics handles.
	
	mIcon = nil;
	mIconSuite = nil;
	mPicture = nil;

	// Try to preload the graphic.
	
	LoadGraphic();
}


// ---------------------------------------------------------------------------
//		* ~VPFToggleButton
// ---------------------------------------------------------------------------
//	Destructor

VPFToggleButton::~VPFToggleButton()
{
	// Dispose of graphics resources we may have cached.
	
	if (mIcon != nil) {
		ValidateHandle_(mIcon);
		::DisposeHandle(mIcon);
	}
	
	if (mIconSuite != nil) {
#if PP_Target_Carbon
		ThrowIfNil_(mIconSuite);
#else
		ValidateHandle_(mIconSuite);
#endif
		::DisposeIconSuite(mIconSuite, true);
	}
	
	if (mPicture != nil) {
#if PP_Target_Carbon
		ThrowIfNil_(mPicture);
#else
		ValidateHandle_((Handle) mPicture);
#endif
		::KillPicture(mPicture);
	}
}


// ---------------------------------------------------------------------------
//		* LoadGraphic											[protected]
// ---------------------------------------------------------------------------
//	Load the graphic resource to be drawn for this button.
//	Unlike LToggleButton, we preload the resource and keep it for the lifetime
//	of the object. (The drawing agent for LToggleButton will destroy this pane
//	if the graphic changes.)

void
VPFToggleButton::LoadGraphic()
{

	// Validate pointers.
	
	ValidateThis_();

	// Load the appropriate graphic if possible.
	
	RFMap* map = VELayoutView::GetCurrentMap();
	if (map != nil) {
	
		ValidateObject_(map);

		// What kind of resource are we drawing?
		
		switch (mGraphicsType) {
		
			// Icon suite:
			
			case 'ICN#': {
				mIconSuite = UIconUtilities::GetIconSuiteFromMap(map, mOnID, svAllAvailableData);
				break;
			}
			
			// Simple icon:
			
			case 'ICON': {
				RFResource* iconRsrc = map->FindResource('ICON', mOnID, false);
				if (iconRsrc != nil) {
					ValidateSimpleObject_(iconRsrc);
					mIcon = iconRsrc->GetResData();
				}
				break;
			}
			
			// Picture:
			
			case 'PICT': {
				RFResource* iconRsrc = map->FindResource('PICT', mOnID, false);
				if (iconRsrc != nil) {
					ValidateSimpleObject_(iconRsrc);
					mPicture = (PicHandle) iconRsrc->GetResData();
				}
				break;
			}
		}
	}
}


// ---------------------------------------------------------------------------
//		* DrawGraphic											[protected]
// ---------------------------------------------------------------------------
//	Overriden to grab graphics from user document.

void
VPFToggleButton::DrawGraphic(
	ResIDT	/* inGraphicID */)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Set up the drawing environment.

	Rect frame;
	CalcLocalFrameRect(frame);
	StColorPenState::Normalize();
	Boolean isUndefined = false;

	// See what kind of resource we're drawing.
	
	switch (mGraphicsType) {
	
		// Icon suite:
		
		case 'ICN#':
			if (mIconSuite != nil) {
#if !PP_Target_Carbon
				ValidateHandle_(mIconSuite);
#endif
				::PlotIconSuite(&frame, atNone, IsEnabled() ? ttNone : ttDisabled, mIconSuite);
			}
			else
				isUndefined = true;
			break;

		// Simple icon:
		
		case 'ICON':
			if (mIcon != nil) {
				ValidateHandle_(mIcon);
				::PlotIcon(&frame, mIcon);
			}
			else
				isUndefined = true;
			break;
		
		// Picture:
		
		case 'PICT':
			if (mPicture != nil) {
#if !PP_Target_Carbon
				ValidateHandle_((Handle) mPicture);
#endif
				::DrawPicture(mPicture, &frame);
			}
			else
				isUndefined = true;
			break;
	
		// Unknown graphics type.
	
		default:
			isUndefined = true;
	
	}
	
	// If no resource, just draw a gray box.

	if (isUndefined) {
		Pattern thePattern;
		UQDGlobals::GetLightGrayPat(&thePattern);
		::FillRect(&frame, &thePattern);
		::FrameRect(&frame);
	}
}
