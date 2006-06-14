// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLGAIconTextButton.cpp		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/15/96
//     $Date: 2006/01/18 01:34:00 $
//  $History: VPLGAIconTextButton.cpp $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:56
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:46
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 12/13/96   Time: 15:52
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Updated to Clint's drop 12/13/96.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 13:57
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added drawing agent.
//	
// ===========================================================================

#include "VPLGAIconTextButton.h"

	// Core : Editors : Views : User interface
#include "VELayoutView.h"

	// Core : Utilities
#include "UIconUtilities.h"

	// MacOS : Resource manager
#include "RFMap.h"
#include "RFResource.h"


// ===========================================================================

#pragma mark *** VPLGAIconTextButton ***

// ---------------------------------------------------------------------------
//		* CreateFromStream
// ---------------------------------------------------------------------------
//	Create the PowerPlant pane for LGAIconTextButton

LPane*
VPLGAIconTextButton::CreateFromStream(
	LStream*	inStream)
{
	return new VPFGAIconTextButton(inStream);
}


// ===========================================================================

#pragma mark -
#pragma mark *** VPFGAIconTextButton ***

// ---------------------------------------------------------------------------
//		* VPFGAIconTextButton(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor

VPFGAIconTextButton::VPFGAIconTextButton(
	LStream*	inStream)

: LGAIconTextButton(inStream)

{
}


// ---------------------------------------------------------------------------
//		* LoadIconSuiteHandle
// ---------------------------------------------------------------------------
//	Override to restrict the icons to the proper resource file

void
VPFGAIconTextButton::LoadIconSuiteHandle()
{
	RFMap* map = VELayoutView::GetCurrentMap();
	if (map != nil) 
	{
		ValidateObject_(map);
		mIconSuite = UIconUtilities::GetIconSuiteFromMap(map, mIconSuiteID, svAllAvailableData);
	}
}
