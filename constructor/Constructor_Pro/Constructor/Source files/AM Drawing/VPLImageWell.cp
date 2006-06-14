// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLImageWell.cp					©1997 Metrowerks Inc. All rights reserved.
// ===========================================================================

#include "VPLImageWell.h"

#include "VELayoutView.h"
#include "RFMap.h"
#include "RFResource.h"

#include "UIconUtilities.h"

#include <UResourceMgr.h>

VPLImageWell::VPLImageWell()
{
	mContentInfo.contentType = kControlContentTextOnly;
	mContentInfo.u.iconSuite = nil;
	
	mOriginalContentInfo.contentType = kControlContentTextOnly;
	mOriginalContentInfo.u.iconSuite = nil;
}


VPLImageWell::~VPLImageWell()
{
	if (mContentInfo.u.iconSuite != nil) {
	
		if (mContentInfo.contentType == kControlContentIconSuiteHandle) {
			::DisposeIconSuite(mContentInfo.u.iconSuite, true);
			
		} else if (mContentInfo.contentType == kControlContentCIconHandle) {
			::DisposeCIcon(mContentInfo.u.cIconHandle);
			
		} else if (mContentInfo.contentType == kControlContentPictHandle) {
			::KillPicture(mContentInfo.u.picture);
		}
	}
}

LPane*
VPLImageWell::CreateFromStream(
	LStream			*inStream)
{
	LImageWell *view = new LImageWell(inStream);
	
	if (view != nil)
	{
		view->GetDataTag(kControlImageWellPart, kControlImageWellContentTag,
					sizeof(mOriginalContentInfo), &mOriginalContentInfo);
		
		if ((mOriginalContentInfo.contentType == kControlContentIconSuiteRes) ||
			(mOriginalContentInfo.contentType == kControlContentCIconRes) ||
			(mOriginalContentInfo.contentType == kControlContentPictRes))
		{
			RFMap* map = VELayoutView::GetCurrentMap();
			if (map != nil) {
				if (mOriginalContentInfo.contentType == kControlContentIconSuiteRes) {
					Handle suiteRsrc = UIconUtilities::GetIconSuiteFromMap(map, mOriginalContentInfo.u.resID, kSelectorAllAvailableData);
					if (suiteRsrc != nil) {
						mContentInfo.contentType = kControlContentIconSuiteHandle;
						mContentInfo.u.iconSuite = suiteRsrc;
					}
			
				} else if (mOriginalContentInfo.contentType == kControlContentCIconRes) {
					RFResource* iconRsrc = map->FindResource('cicn', mOriginalContentInfo.u.resID, false);
					if (iconRsrc != nil) {
						mContentInfo.contentType = kControlContentCIconHandle;
						mContentInfo.u.cIconHandle = UIconUtilities::GetCIconFromMap(map, mOriginalContentInfo.u.resID);
					}
					else if (mOriginalContentInfo.u.resID >= 0 && mOriginalContentInfo.u.resID <= 2)
					{
						StResourceContext sysContext((SInt16) 0);
						mContentInfo.contentType = kControlContentCIconHandle;
						mContentInfo.u.cIconHandle = ::GetCIcon(mOriginalContentInfo.u.resID);
					}
				
				} else if (mOriginalContentInfo.contentType == kControlContentPictRes) {
					RFResource* pictRsrc = map->FindResource('PICT', mOriginalContentInfo.u.resID, false);
					if (pictRsrc != nil) {
						mContentInfo.contentType = kControlContentPictHandle;
						mContentInfo.u.picture = (PicHandle) pictRsrc->GetResData();
					}
				}
			}
			
			view->SetDataTag(kControlImageWellPart, kControlImageWellContentTag,
						sizeof(mContentInfo), &mContentInfo);
		}
	}
	
	return view;
}


// ---------------------------------------------------------------------------
//		* ListenToMap											[protected]
// ---------------------------------------------------------------------------
//	We want to listen for change notices from graphics resources.

Boolean
VPLImageWell::ListenToMap()
{
	return true;
}


// ---------------------------------------------------------------------------
//		* ResourceChanged										[protected]
// ---------------------------------------------------------------------------
//	See if the graphics resource we're drawing has changed. If so,
//	rebuild the button.

void
VPLImageWell::ResourceChanged(
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
	if (changedRF->GetResID() == mOriginalContentInfo.u.resID) {
	
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
				if (mOriginalContentInfo.contentType == kControlContentIconSuiteRes)
					rebuild = true;
				break;
			
			case 'cicn':
				if (mOriginalContentInfo.contentType == kControlContentCIconRes)
					rebuild = true;
				break;
			
			case 'PICT':
				if (mOriginalContentInfo.contentType == kControlContentPictRes)
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

	VPLControlView::ResourceChanged(inResource);

}


// ===========================================================================
#pragma mark -


VPLGAImageWellImp::VPLGAImageWellImp(
	LControlPane	*inControlPane)
		: LGAImageWellImp(inControlPane)
{
	mContentInfo.contentType = kControlContentTextOnly;
	mContentInfo.u.iconSuite = nil;
}


VPLGAImageWellImp::VPLGAImageWellImp(
	LStream		*inStream)
		: LGAImageWellImp(inStream)
{
	mContentInfo.contentType = kControlContentTextOnly;
	mContentInfo.u.iconSuite = nil;
}


VPLGAImageWellImp::~VPLGAImageWellImp()
{
	if (mContentInfo.u.iconSuite != nil) {
	
		if (mContentInfo.contentType == kControlContentIconSuiteRes) {
			::DisposeIconSuite(mContentInfo.u.iconSuite, true);
			
		} else if (mContentInfo.contentType == kControlContentCIconRes) {
			::DisposeCIcon(mContentInfo.u.cIconHandle);
			
		} else if (mContentInfo.contentType == kControlContentPictRes) {
			::KillPicture(mContentInfo.u.picture);
		}
	}
}


void
VPLGAImageWellImp::Init(	
	LControlPane	*inControlPane,
	LStream			*inStream)
{
	LGAImageWellImp::Init(inControlPane, inStream);
}


void
VPLGAImageWellImp::Init (	
	LControlPane*	inControlPane,
	SInt16			inControlKind,
	ConstStringPtr	inTitle,
	ResIDT			inTextTraitsID,
	SInt32			inRefCon)
{
	SInt32	saveMin		= inControlPane->GetMinValue();
	ResIDT	resID		= inControlPane->GetValue();
	
		// To avoid Resource File context problems, we map
		// from Resource-based content to Handle-based content

	UInt8	origType = saveMin;
	UInt8	mappedType = origType;
	if (origType == kControlContentIconSuiteRes) {
		mappedType = kControlContentIconSuiteHandle;
		
	} else if (origType == kControlContentCIconRes) {
		mappedType = kControlContentCIconHandle;
		
	} else if (origType == kControlContentPictRes) {
		mappedType = kControlContentPictHandle;
	}
			
	inControlPane->PokeValue(saveMin - origType + mappedType);
	inControlPane->SetMinValue(saveMin - origType + mappedType);
	inControlPane->PokeValue(resID);
	
		// Initialize superclass
	
	LGAImageWellImp::Init(inControlPane, inControlKind, inTitle, inTextTraitsID, inRefCon);
	
		// If original content type was resource-based, get handle for the
		// content from the current resource map and pass it to the ImageWell
		
	if (origType != mappedType) {
		RFMap* map = VELayoutView::GetCurrentMap();
		if (map != nil) {
			mContentInfo.contentType = mappedType;
			
			if (origType == kControlContentIconSuiteRes) {
				RFResource* suiteRsrc = map->FindResource('ICN#', resID, false);
				if (suiteRsrc != nil) {
					GetIconSuite(&(mContentInfo.u.iconSuite), resID, kSelectorAllAvailableData);
				}
		
			} else if (origType == kControlContentCIconRes) {
				RFResource* iconRsrc = map->FindResource('cicn', resID, false);
				if (iconRsrc != nil) {
					mContentInfo.u.cIconHandle = ::GetCIcon(resID);
				}
			
			} else if (origType == kControlContentPictRes) {
				RFResource* pictRsrc = map->FindResource('PICT', resID, false);
				if (pictRsrc != nil) {
					mContentInfo.u.picture = (PicHandle) pictRsrc->GetResData();
				}
			}
			
			SetDataTag(kControlButtonPart, kControlImageWellContentTag,
						sizeof(ControlButtonContentInfo), &mContentInfo);
		}
	}
}