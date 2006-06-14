// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPModelPane.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/14/96
//     $Date: 2006/01/18 01:33:55 $
//  $History: VPModelPane.h $
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 01/29/97   Time: 18:54
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Added DoSanityCheckSelf method to ensure that superview field has -1 or
//	0 as indicated by the class ID. (Bug fix #1253.)
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:44
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Fixed CR/LF problem
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/24/96   Time: 21:58
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added IsVisible method (overridden from VEModelObject).
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/24/96   Time: 19:40
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added AdaptToSuperFrameSize method (overridden from VPModelObject).
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/20/96   Time: 20:16
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added GetSubObjectList method.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 10:59
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added class.
//	
// ===========================================================================

#pragma once

	// MacOS : Editors : Views : Data model
#include "VPModelObject.h"


// ===========================================================================
//		* VPModelPane
// ===========================================================================
//
//	VPModelObject is the data model representation of a PowerPlant pane.
//	It doesn't really do anything different than VPModelObject. It just has
//	a different class ID, which is useful for drag-receive handling.
//
// ===========================================================================

class VPModelPane : public VPModelObject {

	// data model
	
	DMClassID_				('vpmp', VPModelPane, VPModelObject);

public:
							VPModelPane();
							VPModelPane(LStream* inStream);
							VPModelPane(const VPModelPane& inOriginal);
	virtual					~VPModelPane();

	// accessors

	virtual Boolean			IsVisible() const;

	virtual DMContainerAttribute*
							GetSubObjectList() const;

	// pane resizing helper

protected:
	virtual void			AdaptToSuperFrameSize(
									SInt32			inSurrWidthDelta,
									SInt32			inSurrHeightDelta);

	virtual void			DoSanityCheckSelf();

};
