// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMAttributeSubAction.h		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 04/27/96
//	   $Date: 2006/01/18 01:32:37 $
//	$History: DMAttributeSubAction.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:49
//	Updated in $/Constructor/Source files/CO- Core/Data model/Undo-redo
//	Improved commenting.
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 01/24/97   Time: 17:21
//	Checked in '$/Constructor/Source files/CO- Core/Data model/Undo-redo'
//	Comment: Fixed CR/LF problem
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:32
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

#include "DMSubAction.h"

	// Core : Data model : Attributes
#include "DMAttribute.h"

	// Core : Data model : Core objects
#include "DMObject.h"
#include "DMPointers.h"


// ===========================================================================
//		* DMAttributeSubAction
// ===========================================================================
//
//	A DMListSubAction is a subaction to record a change to an
//	attribute's value. It is automatically created by the Set___Value
//	method of the DMAttribute subclass if a transaction builder is active.
//
// ===========================================================================

class DMAttributeSubAction : public DMSubAction {

public:
							DMAttributeSubAction(DMAttribute* inAttribute);
	virtual					~DMAttributeSubAction();

	virtual void			Commit();
	virtual void			RedoSelf();
	virtual void			UndoSelf();
	virtual DMObject*		GetAffectedObject() const;


	// data members

protected:
	DMAttributePtr			mAttribute;
	DMAttributePtr			mBeforeValue;
	DMAttributePtr			mAfterValue;
	DMObjectPtr				mAffectedObject;

};
