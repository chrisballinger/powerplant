// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPModelScroller.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/22/96
//     $Date: 2006/01/18 01:33:55 $
//  $History: VPModelScroller.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:39
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:44
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/22/96   Time: 16:00
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added class.
//	
// ===========================================================================

#pragma once

	// MacOS : Editors : Views : Data model
#include "VPModelView.h"


// ===========================================================================
//		* VPModelScroller
// ===========================================================================
//
//	VPModelScroller is the data model object for the PowerPlant class
//	LScroller. It enforces a sanity check on the "scrolled view ID" field
//	which must in fact point to a view.
//
// ===========================================================================

class VPModelScroller : public VPModelView {

	// data model
	
	DMClassID_				('vpms', VPModelScroller, VPModelView);

public:
							VPModelScroller();
							VPModelScroller(LStream* inStream);
							VPModelScroller(const VPModelScroller& inOriginal);
	virtual					~VPModelScroller();

protected:
	virtual void			DoSanityCheckSelf();

	virtual void			SendUpdateMessage(
									MessageT		inMessage,
									void*			ioParam,
									DMElement*		inSubElement = nil);

};
