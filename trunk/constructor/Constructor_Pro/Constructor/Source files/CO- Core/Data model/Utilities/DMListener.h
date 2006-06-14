// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMListener.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 05/10/96
//	   $Date: 2006/01/18 01:32:41 $
//	$History: DMListener.h $
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 02/28/97   Time: 11:18
//	Updated in $/Constructor/Source files/CO- Core/Data model/Utilities
//	Added support for PreValueChange.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:14
//	Updated in $/Constructor/Source files/CO- Core/Data model/Utilities
//	Improved commenting.
//	
//	*****************  Version 4   *****************
//	User: scouten      Date: 01/24/97   Time: 17:23
//	Checked in '$/Constructor/Source files/CO- Core/Data model/Utilities'
//	Comment: Fixed CR/LF problem
//	
//	*****************  Version 3   *****************
//	User: scouten      Date: 10/17/96   Time: 17:50
//	Checked in '$/Constructor/Source files/Data model/Utilities'
//	Comment: Replaced DMElement::CanMakeChanges with IsEnabled.
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/17/96   Time: 14:40
//	Checked in '$/Constructor/Source files/Data model/Utilities'
//	Comment: Added enabled flag to attributes.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:43
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// Data model : Attributes
#include "DMAttribute.h"

	// Data model : Core objects
#include "DMElement.h"
#include "DMContainerMixin.h"
#include "DMSelection.h"

	// PowerPlant : Features
#include <LListener.h>


// ===========================================================================
//		* DMListener											[abstract]
// ===========================================================================
//
//	DMListener is a helper class that listens to the messages
//	from the core data model classes (DMElement, DMAttribute,
//	DMContainerMixin, DMSelection) and parses them into method calls
//	that can be overriden. It is intended to be used as a mix-in
//	class with pane classes that display the data model.
//
// ===========================================================================

class DMListener : public LListener {

public:
							DMListener();
	virtual					~DMListener();

	// update notice parsing

protected:
	virtual void			ListenToMessage(
									MessageT		inMessage,
									void*			ioParam);

	// override hooks for update notices

	virtual void			GoingAway(DMElement* inElement);
	virtual void			CollectChanges(DMElement* inElement);
	virtual void			CollectTentativeChanges(DM_CollectTentativeChanges* inMessage);
	virtual void			FindUIObject(DM_FindUIObject* inFindUIMsg);
	virtual void			ModifiedFlagChanged(DMElement* inElement);
	virtual void			EnabledChanged(DMElement* inElement);

	virtual void			ContainerListChanged(DM_ListChange* inMessage);
	virtual void			SelectionChanged(DM_SelectionChanged* inMessage);	
	virtual void			PreValueChanged(DM_PreValueChange* inMessage);
	virtual void			ValueChanged(DM_ValueChange* inMessage);

};
