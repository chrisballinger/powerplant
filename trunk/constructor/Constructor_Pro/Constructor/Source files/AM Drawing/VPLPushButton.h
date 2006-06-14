// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLPushButton.h					©1997 Metrowerks Inc. All rights reserved.
// ===========================================================================

#pragma once

#include "VPLControlPane.h"

#include <LPushButton.h>
#include <LGAPushButtonImp.h>

class VPLPushButton : public VPLControlPane {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLPushButton; }

							VPLPushButton() { }
	virtual					~VPLPushButton() { }

	virtual LPane*			CreateFromStream(
									LStream			*inStream);
protected:
	virtual void			ValueChangedSelf(
									FourCharCode	inAttributeKey,
									DMAttribute*	inAttribute);
};

// ===========================================================================

class	VPLGAPushButtonImp : public LGAPushButtonImp {
public:
						VPLGAPushButtonImp(
								LControlPane	*inControlPane);
						
						VPLGAPushButtonImp(
								LStream			*inStream);
								
	virtual				~VPLGAPushButtonImp();
	
	virtual void		LoadCIconHandle();

};
