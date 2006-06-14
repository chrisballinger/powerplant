// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLPicture.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/14/96
//     $Date: 2006/04/12 08:48:30 $
//  $History: VPLPicture.cpp $
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 15:02
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Updated for refactored RFMap classes.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 14:08
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added drawing agent.
//	
// ===========================================================================

#include "VPLPicture.h"

	// Core : Data model : Attributes
#include "DMIntegerAttribute.h"

	// Core : Editors : Views : User interface
#include "VELayoutView.h"

	// MacOS : Editors : Views : Data model
#include "VPDataModel.h"

	// MacOS : Resource manager
#include "RFMap.h"
#include "RFResource.h"


// ===========================================================================

#pragma mark *** VPLPicture ***

// ---------------------------------------------------------------------------
//		* CreateFromStream
// ---------------------------------------------------------------------------
//	Create the PowerPlant display class for LPicture. Substitute our
//	VPFPicture which loads resources from user's document.

LPane*
VPLPicture::CreateFromStream(
	LStream*	inStream)
{
	return new VPFPicture(inStream);
}


// ---------------------------------------------------------------------------
//		* ValueChangedSelf										[protected]
// ---------------------------------------------------------------------------
//	An attribute of this model object has changed. Update the display
//	accordingly.

void
VPLPicture::ValueChangedSelf(
	FourCharCode	inAttributeKey,
	DMAttribute*	inAttribute)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mDisplayPane);
	ValidateObject_(mModelObject.GetObject());
	ValidateObject_(inAttribute);
	
	// Cast display pane to LPicture.
	
	LPicture* scroller = (dynamic_cast<LPicture*>(mDisplayPane));
	ValidateObject_(scroller);
	
	// Dispatch attribute key.

	switch (inAttributeKey) {

		// Basically all parameters for LPicture result
		// in a rebuild of the drawing agent.
		
		case pPaneImageWidth:
		case pPaneImageHeight:
		case pPanePictureID:
			RebuildMe();
			break;
		
		// Otherwise, see if LView wants it.

		default:
			VPLView::ValueChangedSelf(inAttributeKey, inAttribute);

	}
}


// ---------------------------------------------------------------------------
//		* ResourceChanged										[protected]
// ---------------------------------------------------------------------------
//	A resource has changed. If it's the PICT resource we're drawing,
//	trigger a rebuild of the drawing agent.

void
VPLPicture::ResourceChanged(
	RMResource*		inResource)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateSimpleObject_(inResource);

	// Cast the changed resource to a MacOS resource.
	
	RFResource* changedRF = (dynamic_cast<RFResource*>(inResource));
	ValidateObject_(changedRF);

	// See if a picture resource was affected.
	
	if (changedRF->GetResType() == 'PICT') {
	
		// Yes, it was. Look for the model object's pict ID attribute.
	
		ValidateObject_(mModelObject.GetObject());
		DMAttribute* pictAttr = mModelObject->FindAttributeByKey(pPanePictureID);
		if (pictAttr != nil) {
		
			// Got it. See if the ID matches.
			
			DMIntegerAttribute* pictIntAttr = (dynamic_cast<DMIntegerAttribute*>(pictAttr));
			ValidateObject_(pictIntAttr);
			
			if (pictIntAttr->GetIntValue() == changedRF->GetResID()) {
				RebuildMe();
				return;
			}
		}
	}

	// Otherwise see if generic drawing agent wants it.

	VPDrawingAgent::ResourceChanged(inResource);

}


// ===========================================================================

#pragma mark -
#pragma mark *** VPFPicture ***

// ---------------------------------------------------------------------------
//		* VPFPicture(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor

VPFPicture::VPFPicture(
	LStream*	inStream)

: LPicture(inStream)

{

	// Stomp out what was done in InitPicture(). Grrr...

	mRefreshAllWhenResized = false;

	// If the PICT Resource exists (can be loaded), set the image
	// size to the size of the picture.
		
	PicHandle macPictureH = LoadPicture();
	if (macPictureH != nil) {
	
#if !PP_Target_Carbon
		ValidateHandle_((Handle) macPictureH);
#endif
	
		Rect picFrame = (**macPictureH).picFrame;
		ResizeImageTo(picFrame.right - picFrame.left,
					  picFrame.bottom - picFrame.top, false);
		
		::KillPicture(macPictureH);
	}
}


// ---------------------------------------------------------------------------
//		* ~VPFPicture
// ---------------------------------------------------------------------------
//	Destructor

VPFPicture::~VPFPicture()
{
}


// ---------------------------------------------------------------------------
//		* DrawSelf												[protected]
// ---------------------------------------------------------------------------
//	Overriden to grab picture from resource map and then dispose of it.

void
VPFPicture::DrawSelf()
{

	// If picture resource exists, draw it. Otherwise, fill the
	// frame with a light gray pattern and a one-pixel border.
		
	PicHandle macPictureH = LoadPicture();
	if (macPictureH != nil) {
	
#if !PP_Target_Carbon
		ValidateHandle_((Handle) macPictureH);
#endif

		SDimension32 imageSize;
		GetImageSize(imageSize);
		
		Rect pictureBounds;
		pictureBounds.left   = 0;
		pictureBounds.top    = 0;
		pictureBounds.right  = imageSize.width;
		pictureBounds.bottom = imageSize.height;
		
		::DrawPicture(macPictureH, &pictureBounds);
		::KillPicture(macPictureH);
		
	}
	else {
		Rect frame;
		CalcLocalFrameRect(frame);
		::PenNormal();
		Pattern thePattern;
		UQDGlobals::GetLightGrayPat(&thePattern);
		::FillRect(&frame, &thePattern);
		::FrameRect(&frame);
	}
}


// ---------------------------------------------------------------------------
//		* LoadPicture											[protected]
// ---------------------------------------------------------------------------
//	Overriden to grab picture from resource map.

PicHandle
VPFPicture::LoadPicture()
{
	
	// Use the current resoure map (if possible).
	
	RFMap* map = VELayoutView::GetCurrentMap();
	if (map != nil) {
		
		// Ask it for the given picture resource.
		
		ValidateObject_(map);
		
		RFResource* pictRsrc = map->FindResource('PICT', mPICTid, false);
		if (pictRsrc != nil) {
			ValidateSimpleObject_(pictRsrc);
			return (PicHandle) pictRsrc->GetResData();
		}
	}
	
	// No such resource.
	
	return nil;
	
}
