// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPModelView.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/20/96
//     $Date: 2006/01/18 01:33:56 $
//  $History: VPModelView.h $
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:39
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Improved commenting.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:44
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Fixed CR/LF problem
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 12/08/96   Time: 15:09
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added FinishCreate method. Fixed a bug in SyncSubObjectList that caused
//	it to forget subobjects in some cases.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/24/96   Time: 19:43
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added FrameSizeChanged method.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/22/96   Time: 16:02
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Fixed a couple of commenting glitches.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/20/96   Time: 20:23
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added class.
//	
// ===========================================================================

#pragma once

	// Core : Editors : Views : Data model
#include "VPModelPane.h"


// ===========================================================================
//		* VPModelView
// ===========================================================================
//
//	VPModelView is the data model object for the PowerPlant class LView.
//	It provides a subobject list which is the concatenation of the
//	attachments and subpanes.
//
// ===========================================================================

class VPModelView : public VPModelPane {

	// data model description
	
	DMClassID_				('vpmv', VPModelView, VPModelPane);

	// constructor/destructor

public:
							VPModelView();
							VPModelView(LStream* inStream);
							VPModelView(const VPModelView& inOriginal);
	virtual					~VPModelView();

	// accessors

	virtual DMContainerAttribute*
							GetSubObjectList() const;

	// update notifications

protected:
	virtual void			SendUpdateMessage(
									MessageT			inMessage,
									void*				ioParam,
									DMElement*			inSubElement = nil);

	virtual void			SyncSubObjectList(
									DM_ListChange&		inChangeMessage);

	virtual void			FrameSizeChanged(
									DMAttribute*		inAttribute);

	// subobject list helper

	virtual void			FinishCreate();

	// common initialization

private:
	void					InitModelView();
	

	// data members

protected:
	DMContainerAttributePtr	mSubObjects;				// subobjects list
	Boolean					mHandlingUpdateMessage;		// true if SendUpdateMessage is being called

	SDimension32			mFrameSize;					// cached copy of frame size

};
