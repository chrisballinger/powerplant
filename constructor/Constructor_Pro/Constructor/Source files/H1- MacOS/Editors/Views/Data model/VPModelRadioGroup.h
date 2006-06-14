// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPModelRadioGroup.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 12/08/96
//     $Date: 2006/01/18 01:33:55 $
//  $History: VPModelRadioGroup.h $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 03/19/97   Time: 14:55
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Removed DMListener inheritance. (VEModelObject now inherits from
//	DMListener.)
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:39
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:44
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 12/08/96   Time: 19:41
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Finished baseline radio group support.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 12/08/96   Time: 18:01
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added class.
//	
// ===========================================================================

#pragma once

	// MacOS : Editors : Views : Data model
#include "VPModelObject.h"


// ===========================================================================
//		* VPModelRadioGroup
// ===========================================================================
//
//	VPModelRadioGroup is the data model object for the PowerPlant class
//	LRadioGroup. It ensures that the objects that are added to the radio
//	group are at least controls.
//
// ===========================================================================

class VPModelRadioGroup :	public VPModelObject {

	// data model
	
	DMClassID_				('vpmr', VPModelRadioGroup, VPModelObject);

	// constructor/destructor

public:
							VPModelRadioGroup();
							VPModelRadioGroup(LStream* inStream);
							VPModelRadioGroup(const VPModelRadioGroup& inOriginal);
	virtual					~VPModelRadioGroup();

	// accessors

	virtual Boolean			IsVisualObject() const;

	// radio group management

	virtual void			AddMember(
									VPModelObject*		inObject);

	// initialization

protected:
	virtual void			FinishCreateSelf();

	// radio button management
	
	virtual VPModelObject*	FindRadioButton(
									PaneIDT				inButtonID);
	virtual Boolean			RadioButtonAdded(
									PaneIDT				inButtonID);
	virtual Boolean			RadioButtonRemoved(
									PaneIDT				inButtonID);

	// update notifications
	
	virtual void			SendUpdateMessage(
									MessageT			inMessage,
									void*				ioParam,
									DMElement*			inSubElement = nil);

	virtual void			DoSanityCheckSelf();

	virtual void			GoingAway();
	virtual void			GoingAway(
									DMElement*			inElement);
	

	// data members

protected:
	DMContainerAttributePtr	mButtonList;				// pointer to radio button list attribute

};
