// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLBevelButton.cp				©1997 Metrowerks Inc. All rights reserved.
// ===========================================================================

#include "VPLBevelButton.h"
#include "VPDataModel.h"

#include "VELayoutView.h"
#include "RFMap.h"
#include "RFResource.h"

#include "UIconUtilities.h"

#include <UResourceMgr.h>


VPLBevelButton::VPLBevelButton()
{
	mContentInfo.contentType = kControlContentTextOnly;
	mContentInfo.u.iconSuite = nil;
	
	mOriginalContentInfo.contentType = kControlContentTextOnly;
	mOriginalContentInfo.u.iconSuite = nil;
}


VPLBevelButton::~VPLBevelButton()
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
VPLBevelButton::CreateFromStream(
	LStream			*inStream)
{
	LBevelButton *view = new LBevelButton(inStream);
	
	if (view != nil)
	{
		view->GetDataTag(kControlButtonPart, kControlBevelButtonContentTag,
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
			
			view->SetDataTag(kControlButtonPart, kControlBevelButtonContentTag,
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
VPLBevelButton::ListenToMap()
{
	return true;
}


// ---------------------------------------------------------------------------
//		* ResourceChanged										[protected]
// ---------------------------------------------------------------------------
//	See if the graphics resource we're drawing has changed. If so,
//	rebuild the button.

void
VPLBevelButton::ResourceChanged(
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

	VPLControlPane::ResourceChanged(inResource);

}


// ===========================================================================
#pragma mark -


// ---------------------------------------------------------------------------
//		* ValueChangedSelf										[protected]
// ---------------------------------------------------------------------------
//	An attribute of this model object has changed. Update the display
//	accordingly.

void
VPLBevelButton::ValueChangedSelf(
	FourCharCode	inAttributeKey,
	DMAttribute*	inAttribute)
{
	switch (inAttributeKey) {
	
		case pRebuildMe:
		case pPaneWidth:		// Change size could change which icon displays
		case pPaneHeight:
			RebuildMe();
			break;
	
		default:
			VPLControlPane::ValueChangedSelf(inAttributeKey, inAttribute);
	}
}

// ===========================================================================
#pragma mark -

VPLGABevelButtonImp::VPLGABevelButtonImp(
	LControlPane	*inControlPane)
		: LGABevelButtonImp(inControlPane)
{
	mContentInfo.contentType = kControlContentTextOnly;
	mContentInfo.u.iconSuite = nil;
}


VPLGABevelButtonImp::VPLGABevelButtonImp(
	LStream		*inStream)
		: LGABevelButtonImp(inStream)
{
	mContentInfo.contentType = kControlContentTextOnly;
	mContentInfo.u.iconSuite = nil;
}


VPLGABevelButtonImp::~VPLGABevelButtonImp()
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
VPLGABevelButtonImp::Init(	
	LControlPane	*inControlPane,
	LStream			*inStream)
{
	LGABevelButtonImp::Init(inControlPane, inStream);
}


void
VPLGABevelButtonImp::Init (	
	LControlPane*	inControlPane,
	SInt16			inControlKind,
	ConstStringPtr	inTitle,
	ResIDT			inTextTraitsID,
	SInt32			inRefCon)
{
	ResIDT	menuID		= inControlPane->PeekValue();
	SInt32	saveMin		= inControlPane->GetMinValue();
	ResIDT	resID		= inControlPane->GetMaxValue();
	
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
	
		// If there's a popup menu, we don't need the actual menu
		// since it's never visible in Constructor. We use a dummy
		// MENU resource to avoid problems.
	
	if (menuID != 0) {
		menuID = 3509;		// Empty MENU within Constructor's resource fork
	}
	
	inControlPane->PokeValue(saveMin - origType + mappedType);
	inControlPane->SetMinValue(saveMin - origType + mappedType);
	inControlPane->PokeValue(menuID);
	
		// Initialize superclass
	
	LGABevelButtonImp::Init(inControlPane, inControlKind, inTitle, inTextTraitsID, inRefCon);
	
		// If original content type was resource-based, get handle for the
		// content from the current resource map and pass it to the BevelButton
		
	if (origType != mappedType) {
		RFMap* map = VELayoutView::GetCurrentMap();
		if (map != nil) {
			mContentInfo.contentType = mappedType;
			
			if (origType == kControlContentIconSuiteRes) {
			
					// Determine icon resource type that we need
					// It's based on the size of the BevelButton
				ESizeSelector		iconSize = GetIconSize();
				ResType				iconType = 'ICN#';
				IconSelectorValue	iconSelector = kSelectorAllLargeData;		
				if (iconSize == sizeSelector_SmallIconSize) {
					iconType = 'ics#';
					iconSelector = kSelectorAllSmallData;
				} else if (iconSize == sizeSelector_MiniIconSize) {
					iconType = 'icm#';
					iconSelector = kSelectorAllMiniData;
				}
			
				RFResource* suiteRsrc = map->FindResource(iconType, resID, false);
				if (suiteRsrc != nil) {
					::GetIconSuite(&(mContentInfo.u.iconSuite), resID, iconSelector);
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
			
			SetDataTag(kControlButtonPart, kControlBevelButtonContentTag,
						sizeof(ControlButtonContentInfo), &mContentInfo);
		}
	}
}