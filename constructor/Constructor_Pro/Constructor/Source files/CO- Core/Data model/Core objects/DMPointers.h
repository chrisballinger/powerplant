// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMPointers.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 04/26/96
//	   $Date: 2006/01/18 01:32:36 $
//	$History: DMPointers.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:24
//	Updated in $/Constructor/Source files/CO- Core/Data model/Core objects
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:20
//	Updated in $/Constructor/Source files/CO- Core/Data model/Core objects
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 12/01/96   Time: 00:08
//	Updated in $/Constructor/Source files/Data model/Core objects
//	Improved commenting.
//
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:30
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// PowerPlant : CA : Feature classes
#include <LSharable.h>


// ===========================================================================
//
//	All classes in the data modelling framework are derived from
//	LSharable. To facilitate clean memory management, we use smart
//	pointers to refer to these objects throughout the data modelling
//	framework. This file defines pointer classes for the common
//	classes in the data model.
//
// ===========================================================================


class DMElement;
typedef TSharablePtr<DMElement> DMElementPtr;

class DMAttribute;
typedef TSharablePtr<DMAttribute> DMAttributePtr;

class DMContainerAttribute;
typedef TSharablePtr<DMContainerAttribute> DMContainerAttributePtr;

class DMObject;
typedef TSharablePtr<DMObject> DMObjectPtr;

class DMSelection;
typedef TSharablePtr<DMSelection> DMSelectionPtr;

class GSharableString;			// not really part of the data model, but useful anyway
typedef TSharablePtr<GSharableString> GSharableStringPtr;
