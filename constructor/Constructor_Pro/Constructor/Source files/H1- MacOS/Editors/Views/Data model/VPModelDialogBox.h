// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPModelDialogBox.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/22/96
//     $Date: 2006/01/18 01:33:54 $
//  $History: VPModelDialogBox.h $
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
//	User: scouten      QDate: 11/22/96   Time: 15:55
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added class.
//	
// ===========================================================================

#pragma once

	// MacOS : Editors : Views : Data model
#include "VPModelWindow.h"


// ===========================================================================
//		* VPModelDialogBox
// ===========================================================================
//
//	VPModelDialogBox is the data model representation of a PowerPlant
//	dialog box. It enforces the rule that the default and cancel button IDs
//	must point to LControl objects.
//
// ===========================================================================

class VPModelDialogBox : public VPModelWindow {

	// data model
	
	DMClassID_				('vpmx', VPModelDialogBox, VPModelWindow);

public:
							VPModelDialogBox();
							VPModelDialogBox(LStream* inStream);
							VPModelDialogBox(const VPModelDialogBox& inOriginal);
	virtual					~VPModelDialogBox();

protected:
	virtual void			DoSanityCheckSelf();

	virtual void			SendUpdateMessage(
									MessageT		inMessage,
									void*			ioParam,
									DMElement*		inSubElement = nil);

};
