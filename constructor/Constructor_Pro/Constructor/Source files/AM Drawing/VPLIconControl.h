// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLIconControl.h				©1997 Metrowerks Inc. All rights reserved.
// ===========================================================================

#pragma once

#include "VPLControlPane.h"
#include <LIconControl.h>
#include <LGAIconControlImp.h>

class VPLIconControl : public VPLControlPane {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLIconControl; }

							VPLIconControl() { }
	virtual					~VPLIconControl() { }

	virtual LPane*			CreateFromStream(
									LStream			*inStream)
								{
									return new LIconControl(inStream);
								}
								
protected:
	virtual void			ValueChangedSelf(
									FourCharCode	inAttributeKey,
									DMAttribute*	inAttribute);
};

// ===========================================================================

class VPLGAIconControlImp : public LGAIconControlImp{
public:
						VPLGAIconControlImp(
								LControlPane	*inControlPane);
								
						VPLGAIconControlImp(
								LStream			*inStream);
	
	virtual				~VPLGAIconControlImp();
	
	virtual void		LoadIconSuiteHandle();
	virtual	void		LoadCIconHandle();
};
