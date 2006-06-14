// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLImageWell.h					©1997 Metrowerks Inc. All rights reserved.
// ===========================================================================

#pragma once

#include "VPLControlView.h"

#include <LImageWell.h>
#include <LGAImageWellImp.h>

class VPLImageWell : public VPLControlView {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLImageWell; }

							VPLImageWell();
	virtual					~VPLImageWell();
	
	virtual LPane*			CreateFromStream(
									LStream			*inStream);
	
protected:
	virtual Boolean			ListenToMap();
	virtual void			ResourceChanged(
									RMResource*			inResource);
	
	ControlButtonContentInfo	mContentInfo;
	ControlButtonContentInfo	mOriginalContentInfo;
};

// ===========================================================================

class VPLGAImageWellImp : public LGAImageWellImp {
public:
							VPLGAImageWellImp(	
									LControlPane	*inControlPane);
														
							VPLGAImageWellImp( 	
									LStream			*inStream );	

	virtual					~VPLGAImageWellImp();
	
	virtual void			Init(	LControlPane	*inControlPane,
									LStream			*inStream );

	virtual	void			Init(	LControlPane*	inControlPane,
									SInt16			inControlKind,
									ConstStringPtr	inTitle = Str_Empty,
									ResIDT			inTextTraitsID = 0,
									SInt32			inRefCon = 0);
									
protected:
	ControlButtonContentInfo	mContentInfo;
};