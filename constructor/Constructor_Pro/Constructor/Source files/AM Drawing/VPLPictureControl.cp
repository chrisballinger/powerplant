// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLPictureControl.cp			©1997 Metrowerks Inc. All rights reserved.
// ===========================================================================

#include "VPLPictureControl.h"

#include "VELayoutView.h"
#include "RFMap.h"
#include "RFResource.h"

VPLPictureControl::VPLPictureControl()
{
	mPictHandle = nil;
}


VPLPictureControl::~VPLPictureControl()
{
	if (mPictHandle != nil) {
		::KillPicture(mPictHandle);
	}
}

LPane*
VPLPictureControl::CreateFromStream(
	LStream			*inStream)
{
	LPictureControl *view = new LPictureControl(inStream);
	
	if (view != nil)
	{
		mOriginalResID = view->GetValue();

		RFMap* map = VELayoutView::GetCurrentMap();
		if (map != nil) {
			RFResource* pictRsrc = map->FindResource('PICT', mOriginalResID, false);
			if (pictRsrc != nil) {
				mPictHandle = (PicHandle) pictRsrc->GetResData();
			}
			
			view->SetDataTag(kControlPicturePart, kControlPictureHandleTag,
						sizeof(mPictHandle), &mPictHandle);
		}
	}
	
	return view;
}


// ---------------------------------------------------------------------------
//		* ListenToMap											[protected]
// ---------------------------------------------------------------------------
//	We want to listen for change notices from graphics resources.

Boolean
VPLPictureControl::ListenToMap()
{
	return true;
}


// ---------------------------------------------------------------------------
//		* ResourceChanged										[protected]
// ---------------------------------------------------------------------------
//	See if the graphics resource we're drawing has changed. If so,
//	rebuild the button.

void
VPLPictureControl::ResourceChanged(
	RMResource*		inResource)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateSimpleObject_(inResource);
	
	// Cast the resource to a Mac resource.
	
	RFResource* changedRF = (dynamic_cast<RFResource*>(inResource));
	ValidateObject_(changedRF);
	
	// Check the resource type and ID against the image well's graphic type and ID.

	Boolean rebuild = false;
	if (changedRF->GetResID() == mOriginalResID) {
	
		// ID matches... see if type matches.
		
		if (changedRF->GetResType() == 'PICT') {
			rebuild = true;
		}
	}
	
	// If rebuild was called for do it now.
	
	if (rebuild) {
		RebuildMe();
		return;
	}

	// Let inherited grab this notification.

	VPLControlView::ResourceChanged(inResource);

}


// ===========================================================================
#pragma mark -

VPLGAPictureControlImp::VPLGAPictureControlImp(
	LControlPane	*inControlPane,
	ResIDT			inPICTid)
		: LGAPictureControlImp(inControlPane, inPICTid)
{
}


VPLGAPictureControlImp::VPLGAPictureControlImp(
	LStream		*inStream)
		: LGAPictureControlImp(inStream)
{
}


VPLGAPictureControlImp::~VPLGAPictureControlImp()
{
}


PicHandle
VPLGAPictureControlImp::LoadPictResource()
{
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