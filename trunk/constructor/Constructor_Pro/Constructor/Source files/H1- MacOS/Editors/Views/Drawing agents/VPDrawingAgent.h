// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPDrawingAgent.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 06/17/96
//     $Date: 2006/01/18 01:33:57 $
//	$History: VPDrawingAgent.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 15:00
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Updated for refactored RFMap classes.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/17/96   Time: 11:19
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Layout view
//	Added resource map listening and Txtr update behaviors.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/31/96   Time: 13:59
//	Created
//	Comment: 
//	
// ===========================================================================

#pragma once

	// Core : Editors : Views : User interface
#include "VEDrawingAgent.h"


// ===========================================================================
//		* VPDrawingAgent										[abstract]
// ===========================================================================
//
//	Abstract base class for all PowerPlant drawing agents. Contains the
//	knowledge of how to create a PPob stream for a class.
//
// ===========================================================================

class VPDrawingAgent : public VEDrawingAgent {

public:
							VPDrawingAgent();
	virtual					~VPDrawingAgent();

	// drawing behaviors

	virtual void			CreateDisplayObject(LView* inSuperView);
	virtual LPane*			CreateFromStream(LStream* inStream) = 0;

	// resource map update notifications

protected:
	virtual Boolean			ListenToMap();

	virtual void			ResourceChanged(RMResource* inResource);
	virtual void			TxtrChanged();

};
