// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMSubAction.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 04/27/96
//	   $Date: 2006/01/18 01:32:38 $
//	$History: DMSubAction.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:50
//	Updated in $/Constructor/Source files/CO- Core/Data model/Undo-redo
//	Improved commenting.
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 01/24/97   Time: 17:22
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

	// PowerPlant : Action classes
#include <LAction.h>


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class DMObject;


// ===========================================================================
//		* DMSubAction											[abstract]
// ===========================================================================
//
//	A DMSubAction is an LAction (PowerPlant) that represents one part
//	of a more complex user action. DMTransaction maintains a list of
//	subactions to be redone or undone as a group.
//
// ===========================================================================

class DMSubAction : public LAction {

public:
							DMSubAction();
	virtual					~DMSubAction();

	virtual void			Commit();
	virtual DMObject*		GetAffectedObject() const;

};
