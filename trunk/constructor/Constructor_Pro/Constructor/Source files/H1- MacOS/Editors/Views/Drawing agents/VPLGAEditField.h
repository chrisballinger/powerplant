// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLGAEditField.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/15/96
//     $Date: 2006/01/18 01:33:59 $
//  $History: VPLGAEditField.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 03/20/97   Time: 11:00
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Fixed cosmetic problems with redrawing LGAEditField when it resizes.
//	(Bug fix #1186.)
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:56
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:46
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 13:49
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added drawing agent.
//	
// ===========================================================================

#pragma once

	// MacOS : Editors : Views : Drawing agents
#include "VPLEditField.h"

	// PowerPlant : GA : Grayscale panes
#include <LGAEditField.h>


// ===========================================================================
//		* VPLGAEditField
// ===========================================================================
//	Drawing agent for the PowerPlant class LGAEditField.

class VPLGAEditField : public VPLEditField {

public:
	static VEDrawingAgent*	CreateAgent()
									{ return new VPLGAEditField; }

							VPLGAEditField() {}
	virtual					~VPLGAEditField() {}

	virtual LPane*			CreateFromStream(LStream* inStream)
									{ return new LGAEditField(inStream); }

protected:
	virtual void			ValueChangedSelf(
									FourCharCode		inAttributeKey,
									DMAttribute*		inAttribute);

};
