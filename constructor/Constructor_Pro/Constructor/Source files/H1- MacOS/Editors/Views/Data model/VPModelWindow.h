// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPModelWindow.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/14/96
//     $Date: 2006/01/18 01:33:56 $
//  $History: VPModelWindow.h $
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:39
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Improved commenting.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:45
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Fixed CR/LF problem
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 12/08/96   Time: 15:51
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Fixed some bugs with the close box flag.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/22/96   Time: 16:02
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Fixed a couple of commenting glitches.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/20/96   Time: 20:25
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Now inherits from VPModelView instead of VPModelPane.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 11:06
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added class.
//	
// ===========================================================================

#pragma once

	// MacOS : Editors : Views : Data model
#include "VPModelView.h"


// ===========================================================================
//		* VPModelWindow
// ===========================================================================
//
//	VPModelWindow is the data model representation of a PowerPlant window.
//
// ===========================================================================

class VPModelWindow : public VPModelView {

	// data model
	
	DMClassID_				('vpmw', VPModelWindow, VPModelView);

public:
							VPModelWindow();
							VPModelWindow(LStream* inStream);
							VPModelWindow(const VPModelWindow& inOriginal);
	virtual					~VPModelWindow();

	// raw data streaming

	virtual void			ReadStreamData(
									LStream*		inStream,
									FourCharCode	inStreamSelector = 0);

	// update notifications

protected:
	virtual void			SendUpdateMessage(
									MessageT		inMessage,
									void*			ioParam,
									DMElement*		inSubElement = nil);

	// internal data model consistency

	void					UpdateSizeFromBounds();
	void					UpdateBoundsFromSize();
	
	void					UpdateWindowFlagsFromKind(
									Boolean			inUpdateCloseBox);
	void					UpdateWindowKindFromFlags();


	// data members

protected:
	Boolean					mSuppressSizeUpdates;			// true while reading stream data
	Boolean					mSuppressFlagUpdates;			// true while updating window flags
	Boolean					mSuppressKindUpdates;			// true while UpdateWindowKindFromFlags is running

};
