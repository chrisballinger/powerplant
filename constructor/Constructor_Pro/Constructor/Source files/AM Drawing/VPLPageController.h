// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLPageController.h				©1997 Metrowerks Inc. All rights reserved.
// ===========================================================================

#pragma once

#include "VPLView.h"
#include "LPageController.h"


class VPLPageController : public VPLView {
public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLPageController; }

							VPLPageController() {}
	virtual					~VPLPageController() {}

	virtual LPane*			CreateFromStream(LStream* inStream);
									
	virtual void			DrawDisplayObject();

protected:
	virtual void			ValueChangedSelf(
									FourCharCode		inAttributeKey,
									DMAttribute*		inAttribute);
};

// ===========================================================================

class VPFPageController : public LPageController {
public:
						VPFPageController(	
								LStream		*inStream);
								
	virtual				~VPFPageController();
	
protected:
	virtual void		LoadPageTitles(
								ResIDT		inStringListID,
								SInt16		inSelectionIndex);
};
