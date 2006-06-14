// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLPrintout.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/15/96
//     $Date: 2006/04/12 08:48:30 $
//  $History: VPLPrintout.cpp $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/19/97   Time: 19:53
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:48
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 14:08
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added drawing agent.
//	
// ===========================================================================

#include "VPLPrintout.h"

	// Core : Data model : Attributes
#include "DMIntegerAttribute.h"

	// MacOS : Editors : Views : Data model
#include "VPDataModel.h"


// ===========================================================================

#pragma mark *** VPLPrintout ***

// ---------------------------------------------------------------------------
//		* CreateFromStream
// ---------------------------------------------------------------------------
//	Create a view that simulates the printout.

LPane*
VPLPrintout::CreateFromStream(
	LStream*	inStream)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mModelObject.GetObject());

	// Set up pane and window parameter blocks.
	
	SPaneInfo paneInfo = { 0, 0, 0, true, true, { false, false, false, false }, 0, 0, 0, (LView*) -1 };
	SViewInfo viewInfo = { {0, 0}, {0, 0}, {0, 0}, false };

	// Read LPrintout stream information.
	
	SPrintoutInfo printoutInfo;
	*inStream >> printoutInfo.width;
	*inStream >> printoutInfo.height;
	*inStream >> printoutInfo.active;
	*inStream >> printoutInfo.enabled;
	*inStream >> printoutInfo.userCon;
	*inStream >> printoutInfo.attributes;

	paneInfo.width = printoutInfo.width;
	paneInfo.height = printoutInfo.height;
	paneInfo.enabled = printoutInfo.enabled;

	// Create a view to stand in for the printout.

	return new LView(paneInfo, viewInfo);

}
