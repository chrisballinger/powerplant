// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLBevelButton.h				©1997 Metrowerks Inc. All rights reserved.
// ===========================================================================

#pragma once

#include "VPLControlPane.h"

#include <LBevelButton.h>
#include <LGABevelButtonImp.h>

class VPLBevelButton : public VPLControlPane {
public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLBevelButton; }

							VPLBevelButton();
	virtual					~VPLBevelButton();

	virtual LPane*			CreateFromStream(
									LStream			*inStream);
									
protected:
	virtual void			ValueChangedSelf(
									FourCharCode	inAttributeKey,
									DMAttribute*	inAttribute);
	
	virtual Boolean			ListenToMap();
	virtual void			ResourceChanged(
									RMResource*			inResource);
	
	ControlButtonContentInfo	mContentInfo;
	ControlButtonContentInfo	mOriginalContentInfo;
};

// ===========================================================================

class VPLGABevelButtonImp : public LGABevelButtonImp {
public:
							VPLGABevelButtonImp(	
									LControlPane	*inControlPane);
														
							VPLGABevelButtonImp( 	
									LStream			*inStream );	

	virtual					~VPLGABevelButtonImp();
													
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