// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLPushButton.cp				©1997 Metrowerks Inc. All rights reserved.
// ===========================================================================

#include "VPLPushButton.h"
#include "VPDataModel.h"
#include "DMIntegerAttribute.h"
#include "VELayoutView.h"

#include "RFMap.h"
#include "RFResource.h"
//#include "StResourceContext.h"
#include <UResourceMgr.h>

LPane*
VPLPushButton::CreateFromStream(
	LStream		*inStream)
{
	return new LPushButton(inStream);
}


// ---------------------------------------------------------------------------
//		* ValueChangedSelf										[protected]
// ---------------------------------------------------------------------------
//	An attribute of this model object has changed. Update the display
//	accordingly.

void
VPLPushButton::ValueChangedSelf(
	FourCharCode	inAttributeKey,
	DMAttribute*	inAttribute)
{
	switch (inAttributeKey) {
	
		case pDefaultButton: {
			LPushButton	*button = dynamic_cast<LPushButton*>(mDisplayPane);
			if (button != nil) {
				DMIntegerAttribute* attr = (dynamic_cast<DMIntegerAttribute*>(inAttribute));
				if (attr != nil) {
				
					// JCD 980630 - trying to fix MW07655.
					// If the default status changes, we will need to force a
					// rebuild.
					
					bool useDefault = (attr->GetIntValue() != 0);
					button->SetDefaultButton(useDefault);

					RebuildMe();
				}
			}
			break;
		}

		// JCD 980701 - fix for MW07655. These items affect the object's
		// frame, so we'll need to force a rebuild of the drawing agent
		
		case pPaneWidth:
		case pPaneHeight:
		case pPaneLeftOrigin:
		case pPaneTopOrigin:
			RebuildMe();
			break;
				
		default:
			VPLControlPane::ValueChangedSelf(inAttributeKey, inAttribute);
	}
}

// ===========================================================================
#pragma mark -

VPLGAPushButtonImp::VPLGAPushButtonImp(
	LControlPane	*inControlPane)
		: LGAPushButtonImp(inControlPane)
{
}


VPLGAPushButtonImp::VPLGAPushButtonImp(
	LStream		*inStream)
		: LGAPushButtonImp(inStream)
{
}


VPLGAPushButtonImp::~VPLGAPushButtonImp()
{
}


void
VPLGAPushButtonImp::LoadCIconHandle ()
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
