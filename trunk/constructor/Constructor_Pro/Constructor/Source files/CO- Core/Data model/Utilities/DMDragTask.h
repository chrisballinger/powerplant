// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMDragTask.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 06/07/96
//	   $Date: 2006/01/18 01:32:39 $
//	$History: DMDragTask.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:14
//	Updated in $/Constructor/Source files/CO- Core/Data model/Utilities
//	Improved commenting.
//
//	*****************  Version 3   *****************
//	User: scouten      Date: 01/24/97   Time: 17:22
//	Checked in '$/Constructor/Source files/CO- Core/Data model/Utilities'
//	Comment: Fixed CR/LF problem
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/23/96   Time: 23:26
//	Checked in '$/Constructor/Source files/Data model/Utilities'
//	Comment: Added WasDraggedToTrash method.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:43
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// Core : Headers
#include "Constructor.file.h"

	// PowerPlant : Features
#include <LDragTask.h>


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

const FlavorType DM_DragFlavor = Type_CreatorCode;
class DMElement;


// ===========================================================================
//		* DMDragTask
// ===========================================================================
//
//	DMDragTask is a helper class for originating drags with data
//	model objects. It uses a single private drag flavor, which is
//	implemented by casting from a DMElement* to ItemReference. No data
//	is generated for dragging.
//
// ===========================================================================

class DMDragTask : public LDragTask {

public:
							DMDragTask(
									const EventRecord&	inEventRecord);
	virtual					~DMDragTask();

	// drag setup

	virtual ItemReference	AddDragElement(
									DMElement*			inElement,
									RgnHandle			inGlobalDragRgn,
									const Rect*			inGlobalItemBounds);

	// drag information accessors
	
	Boolean					WasDraggedToTrash();

};
