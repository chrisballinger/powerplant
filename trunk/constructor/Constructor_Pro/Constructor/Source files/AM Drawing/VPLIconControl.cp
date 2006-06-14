// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLIconControl.cp				©1997 Metrowerks Inc. All rights reserved.
// ===========================================================================

#include "VPLIconControl.h"
#include "VPDataModel.h"

#include "VELayoutView.h"
#include "RFMap.h"
#include "RFResource.h"
//#include "StResourceContext.h"
#include <UResourceMgr.h>



// ---------------------------------------------------------------------------
//		* ValueChangedSelf										[protected]
// ---------------------------------------------------------------------------
//	An attribute of this model object has changed. Update the display
//	accordingly.

void
VPLIconControl::ValueChangedSelf(
	FourCharCode	inAttributeKey,
	DMAttribute*	inAttribute)
{
	switch (inAttributeKey) {
	
		case pPaneWidth:		// Changing size could change what icon we display
		case pPaneHeight:
			RebuildMe();
			break;
	
		default:
			VPLControlPane::ValueChangedSelf(inAttributeKey, inAttribute);
	}
}

// ===========================================================================
#pragma mark -

VPLGAIconControlImp::VPLGAIconControlImp(
	LControlPane	*inControlPane)
		: LGAIconControlImp(inControlPane)
{
}


VPLGAIconControlImp::VPLGAIconControlImp(
	LStream		*inStream)
		: LGAIconControlImp(inStream)
{
}


VPLGAIconControlImp::~VPLGAIconControlImp()
{
}


void
VPLGAIconControlImp::LoadIconSuiteHandle()
{
	RFMap* map = VELayoutView::GetCurrentMap();
	if (map != nil) {
		CalcIconSuiteSize();
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

		RFResource* suiteRsrc = map->FindResource(iconType, mIconSuiteID, false);
		if (suiteRsrc != nil) {
			Handle	theIconSuite;
			::GetIconSuite(&theIconSuite, mIconSuiteID, iconSelector);
			if (theIconSuite != nil) {
				SetIconSuiteH(theIconSuite, true);
			}
		}
	}
}


void
VPLGAIconControlImp::LoadCIconHandle()
{
	CIconHandle	theIconH = nil;
	
	if ( (mCIconResID >= 0) && (mCIconResID <= 2) ) {
		StResourceContext sysContext((SInt16) 0);
		theIconH = ::GetCIcon(mCIconResID);
		
	} else {

		RFMap* map = VELayoutView::GetCurrentMap();
		if (map != nil) {
			ValidateObject_(map);
			RFResource* iconRsrc = map->FindResource('cicn', mCIconResID, false);
			if (iconRsrc != nil) {
				//we don't actually use the RFResource; we just use it to make sure a cicn
				//exists in the project's map, so that GetCIcon doesn't grab one from some
				//other map
				theIconH = ::GetCIcon ( mCIconResID );
			}
		}
	}
	
	if (theIconH != nil) {
		SetCIconH(theIconH, true);
	}
}
