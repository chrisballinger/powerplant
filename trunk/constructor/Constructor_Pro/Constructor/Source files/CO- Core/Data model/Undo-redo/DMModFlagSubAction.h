// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMModFlagSubAction.h		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 05/02/96
//	   $Date: 2006/01/18 01:32:38 $
//	$History: DMModFlagSubAction.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:50
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

	// Core : Data model : Core objects
#include "DMElement.h"
#include "DMPointers.h"

	// Core : Data model : Undo-rdo
#include "DMSubAction.h"


// ===========================================================================
//		* DMModFlagSubAction
// ===========================================================================
//
//	A DMModFlagSubAction is a subaction to set or clear the mModified
//	and mSubModified flags of a DMElement. It is automatically created
//	by DMElement::MakeModified or DMElement::SubModified if a transaction
//	builder is active.
//
// ===========================================================================

class DMModFlagSubAction : public DMSubAction {

public:
							DMModFlagSubAction(
									DMElement*	inAttribute,
									Boolean		inSetModified,
									Boolean		inSetSubModified);
	virtual					~DMModFlagSubAction();
	
	virtual void			RedoSelf();
	virtual void			UndoSelf();

	
protected:
	DMElementPtr			mElement;				// element affected by this transaction
	Boolean					mSetModified;			// true if "mElement->mModified" was set to true
	Boolean					mSetSubModified;		// true if "mElement->mSubModified" was set to true

};
