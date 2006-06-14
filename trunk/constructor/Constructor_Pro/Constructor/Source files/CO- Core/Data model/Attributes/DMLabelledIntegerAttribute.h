// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMLabelledIntegerAttribute.h
//								© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 07/17/96
//	   $Date: 2006/01/18 01:32:31 $
//	$History: DMLabelledIntegerAttribute.h $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:14
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Improved commenting.
//	
//	*****************  Version 4  *****************
//	User: Andrew       Date: 2/03/97    Time: 5:03p
//	Updated in $/ConstructorWin/Includes
//	Added #include for template generation under MSVC++
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:19
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 12/08/96   Time: 14:23
//	Updated in $/Constructor/Source files/Data model/Attributes
//	Fixed a bug in FinishCreateSelf that caused it to lose value labels
//	when the attribute was cloned from another attribute.
//
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:29
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Attributes
#include "DMContainerAttribute.h"
#include "DMIntegerAttribute.h"

	// Core : Data model : Core objects
#include "DMContainerMixin.h"


// ===========================================================================
//		* DMLabelledIntegerAttribute
// ===========================================================================
//
//	DMLabelledIntegerAttribute implements an integer attribute with optional
//	value labels.
//
// ===========================================================================

class DMLabelledIntegerAttribute :
							public DMIntegerAttribute,
							public DMContainerMixin {

	// data model description

	DMClassID_				('labi', DMLabelledIntegerAttribute, DMIntegerAttribute);

	// constructor/destructor

public:
							DMLabelledIntegerAttribute();
							DMLabelledIntegerAttribute(LStream* inStream);
							DMLabelledIntegerAttribute(const DMLabelledIntegerAttribute& inOriginal);
	virtual					~DMLabelledIntegerAttribute();

	// value accessors

	virtual void			GetTextValue(
									LStr255&		outValue) const;
	virtual void			SetTextValue(
									ConstStringPtr	inValue);

	DMContainerAttribute*	GetValueLabels() const
									{ return mValueLabels; }

	// initialization
	
protected:
	virtual void			FinishCreateSelf();


	// data members

protected:
	DMContainerAttributePtr	mValueLabels;				// container for value labels (if any)
	Boolean					mLabelsOnly;				// false if other values are allowed

};
