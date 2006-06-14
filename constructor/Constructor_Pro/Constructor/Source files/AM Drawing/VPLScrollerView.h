// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLScrollerView.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================

#pragma once

#include "VPLView.h"

#include <LScrollerView.h>


class VPLScrollerView : public VPLView {
public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLScrollerView; }

							VPLScrollerView() {}
	virtual					~VPLScrollerView() {}

	virtual LPane*			CreateFromStream(LStream* inStream);
	virtual void			DrawDisplayObject();


protected:
	virtual void			ValueChangedSelf(
									FourCharCode		inAttributeKey,
									DMAttribute*		inAttribute);
	virtual void			SubPaneDeletedSelf(
									LPane*				inSubPane);

};

// ===========================================================================

class VPFScrollerView : public LScrollerView {

public:
							VPFScrollerView(LStream* inStream);
	virtual					~VPFScrollerView();

protected:
	virtual void			FinishCreateSelf();


	friend class VPLScrollerView;

};
