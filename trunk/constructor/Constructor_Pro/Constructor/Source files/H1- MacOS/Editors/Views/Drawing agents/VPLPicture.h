// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLPicture.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/14/96
//     $Date: 2006/01/18 01:34:01 $
//  $History: VPLPicture.h $
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 15:02
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Updated for refactored RFMap classes.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 14:07
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added drawing agent.
//	
// ===========================================================================

#pragma once

	// MacOS : Editors : Views : Drawing agents
#include "VPLView.h"


// ===========================================================================
//		* VPLPicture
// ===========================================================================
//	Drawing agent for the PowerPlant class LPicture.

class VPLPicture : public VPLView {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLPicture; }

							VPLPicture() {}
	virtual					~VPLPicture() {}

	virtual LPane*			CreateFromStream(LStream* inStream);

protected:
	virtual void			ValueChangedSelf(
									FourCharCode		inAttributeKey,
									DMAttribute*		inAttribute);
	virtual void			ResourceChanged(
									RMResource*			inResource);

};


// ===========================================================================
//		* VPFPicture
// ===========================================================================
//	Helper class for LPicture drawing agent. Restricts search for
//	picture resources to the top resource file.

class VPFPicture : public LPicture {

public:
							VPFPicture(LStream* inStream);
	virtual					~VPFPicture();

protected:
	virtual void			DrawSelf();
	virtual PicHandle		LoadPicture();


	friend class VPLPicture;

};

