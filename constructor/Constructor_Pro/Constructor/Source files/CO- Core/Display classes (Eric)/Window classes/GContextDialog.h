// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	GContextDialog.h			© 1995-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/15/95
//     $Date: 2006/01/18 01:32:46 $
//	$History: GContextDialog.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:57
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Window classes
//	Improved commenting.
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 01/24/97   Time: 17:25
//	Checked in '$/Constructor/Source files/CO- Core/Display classes (Eric)/Window classes'
//	Comment: Fixed CR/LF problem
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:45
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// PowerPlant : Pane classes
#include <LDialogBox.h>


// ===========================================================================
//		* GContextDialog
// ===========================================================================
//
//	Variant on LDialogBox that ensures that the application's resource
//	fork is topmost before attempting any drawing behavior.
//
//	In Constructor, this class is registered in place of LDialogBox.
//	It takes the same stream data as LDialogBox.
//
// ===========================================================================

class GContextDialog : public LDialogBox {

public:
	static LDialogBox*		CreateFromStream(LStream* inStream)
									{ return new GContextDialog(inStream); }

							GContextDialog(LStream*inStream);
	virtual					~GContextDialog();

	// overrides to enforce resource context
	
	virtual void			Activate();
	virtual void			Deactivate();
	virtual void			UpdatePort();

	virtual void			HandleClick(
									const EventRecord&	inMacEvent,
									SInt16				inPart);
	
};
