// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLPageController.cp			©1997 Metrowerks Inc. All rights reserved.
// ===========================================================================

#include "VPLPageController.h"
#include "VPDataModel.h"
#include "DMIntegerAttribute.h"
#include "VELayoutView.h"
#include "RFMap.h"
#include "RFResource.h"


LPane*
VPLPageController::CreateFromStream(
	LStream		*inStream)
{
	return new VPFPageController(inStream);
}


// ---------------------------------------------------------------------------
//		* ValueChangedSelf										[protected]
// ---------------------------------------------------------------------------
//	An attribute of this model object has changed. Update the display
//	accordingly.

void
VPLPageController::ValueChangedSelf(
	FourCharCode	inAttributeKey,
	DMAttribute*	inAttribute)
{
	switch (inAttributeKey) {
	
		case pPaneHeight:
		case pRebuildMe:
			RebuildMe();
			break;
			
		case pPaneValue: {
			LPageController* pager = (dynamic_cast<LPageController*>(mDisplayPane));
			if (pager != nil) {
				DMIntegerAttribute* attr = (dynamic_cast<DMIntegerAttribute*>(inAttribute));
				pager->SetSelectedPageButtonByIndex(attr->GetIntValue(), false);
			}
			break;
		}
	
		default:
			VPLView::ValueChangedSelf(inAttributeKey, inAttribute);
	}
}


// ---------------------------------------------------------------------------
//		* DrawDisplayObject
// ---------------------------------------------------------------------------
//	Draw the visual representation of this object. Do NOT draw any of
//	its subpanes.

void
VPLPageController::DrawDisplayObject()
{
	ValidateThis_();

	// See if we've created a display pane for this object.
	// If so, we can provide default behavior for drawing.
	
	LPageController* pager = dynamic_cast<LPageController*>(GetDisplayPane());
	
	if (pager != nil) {
		ValidateObject_(pager);
		pager->Draw(nil);
	}
}

// ===========================================================================
#pragma mark -

VPFPageController::VPFPageController(
	LStream		*inStream)
		: LPageController(inStream)
{
}


VPFPageController::~VPFPageController()
{
}


void
VPFPageController::LoadPageTitles(
	ResIDT		inStringListID,
	SInt16		inSelectionIndex)
{
	RFMap* map = VELayoutView::GetCurrentMap();
	if (map != nil) {
		ValidateObject_(map);
		
		RFResource* strRsrc = map->FindResource('STR#', inStringListID, false);
		if (strRsrc != nil) {

			SInt16	titleCount = 0;
			Handle	stringListH = strRsrc->GetResData();
			::HLock(stringListH);
			if (::GetHandleSize(stringListH) > 0) {
				titleCount = * (SInt16*) (*stringListH);
			}
			
			if (titleCount > 0) {
				StringPtr	strP = (StringPtr)(*stringListH) + 2;	// Skip past count
				for (SInt16 index = 1; index <= titleCount; index++) {
					AddPageButton(strP, false);
					strP += *strP + 1;
				}

				if (inSelectionIndex > 0) {
					SetSelectedPageButtonByIndex(inSelectionIndex);
				}
			}
			
			::DisposeHandle(stringListH);
		}
	}
}