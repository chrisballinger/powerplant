// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLPictureControl.h				©1997 Metrowerks Inc. All rights reserved.
// ===========================================================================

#pragma once

#include "VPLControlView.h"
#include <LPictureControl.h>
#include <LGAPictureControlImp.h>

class VPLPictureControl : public VPLControlView {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLPictureControl; }

							VPLPictureControl();
	virtual					~VPLPictureControl();

	virtual LPane*			CreateFromStream(
									LStream			*inStream);
protected:
	virtual Boolean			ListenToMap();
	virtual void			ResourceChanged(
									RMResource*			inResource);
	
	ResIDT						mOriginalResID;
	PicHandle					mPictHandle;
};

// ===========================================================================

class VPLGAPictureControlImp : public LGAPictureControlImp{
public:
						VPLGAPictureControlImp(
								LControlPane	*inControlPane,
								ResIDT			inPICTid);
								
						VPLGAPictureControlImp(
								LStream			*inStream);
	
	virtual				~VPLGAPictureControlImp();
	
	virtual PicHandle	LoadPictResource();
};