// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMReanimator.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 04/27/96
//	   $Date: 2006/01/18 01:32:36 $
//	$History: DMReanimator.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:24
//	Updated in $/Constructor/Source files/CO- Core/Data model/Core objects
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:21
//	Updated in $/Constructor/Source files/CO- Core/Data model/Core objects
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/03/96   Time: 11:38
//	Updated in $/Constructor/Source files/Data model/Core objects
//	Changed internal CTYP resources to DSPC.
//
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:30
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Core objects
#include "DMPointers.h"

// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class DMContainerMixin;


// ===========================================================================
//		* DM_ClassTable
// ===========================================================================
//
//	This table is used to hold the mapping of class IDs to
//	CreateFromStream functions.
//
// ===========================================================================

typedef DMElement* (*DM_ElementCreatorFunc)(LStream*);

typedef struct DM_ClassTable {
	ClassIDT				classID;
	DM_ElementCreatorFunc	creatorFunc;
} DMClassTable, *DM_ClassTableP, **DM_ClassTableH;


// ===========================================================================
//		* DMReanimator
// ===========================================================================
//
//	DMReanimator is a helper class for creating object prototypes from
//	type description ('DSPC') resources. Generally, 'DSPC' resources
//	are used to describe data model objects used by Constructor. There
//	are some variants of this format, such as the 'CTYP' resource used
//	by the PowerPlant view editor to describe custom display classes.
//
//	DMReanimator is based heavily on PowerPlant's UReanimator and URegistrar.
//
// ===========================================================================

class DMReanimator : public LSharable {

public:
							DMReanimator();
	virtual					~DMReanimator();

	// singleton accessor

	static DMReanimator*	GetReanimator()
									{ return sReanimator; }

	// prototype builder
	
	void					ElementsFromResource(
									ResType				inResType,
									ResIDT				inResID,
									DMElementPtr&		inElementPtr);
	void					ElementsFromHandle(
									Handle				inHandle,
									DMElementPtr&		inElementPtr);
	void					ElementsFromStream(
									LStream*			inStream,
									DMElementPtr&		inElementPtr);

	// element class registration
	
	void					RegisterClass(
									ClassIDT			inClassID,
									DM_ElementCreatorFunc inCreatorFunc);
	void					CreateObject(
									ClassIDT			inClassID,
									LStream*			inStream,
									DMElementPtr&		inElementPtr);

	// implementation details
	
protected:
	void					ReadElementsFromTags(
									LStream*			inStream,
									DMContainerMixin*	inHostElement,
									DMElementPtr&		inElementPtr);

	SInt16					FetchClassIndex(ClassIDT	inClassID);
	
	
	// data members

protected:
	SInt16					mClassCount;				// number of classes known to reanimator
	DM_ClassTableH			mTableH;					// class ID -> CreateFromStream function mapping

	static DMReanimator*	sReanimator;				// singleton pointer

};
