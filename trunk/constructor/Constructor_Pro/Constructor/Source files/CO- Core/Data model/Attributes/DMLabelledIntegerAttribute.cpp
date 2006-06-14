// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMLabelledIntegerAttribute.cpp
//								© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 07/17/96
//	   $Date: 2006/01/18 01:32:31 $
//	$History: DMLabelledIntegerAttribute.cpp $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:13
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Improved commenting.
//	
//	*****************  Version 4  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:00p
//	Updated in $/ConstructorWin/Sources
//	Initial merge with MSVC build
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:19
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 12/08/96   Time: 14:22
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

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "DMLabelledIntegerAttribute.h"

	// Core : Data model : Attributes
#include "DMContainerAttribute.h"
#include "DMValueLabel.h"

	// Core : Data model : Utility classes
#include "DMIterator.h"


// ===========================================================================

#pragma mark *** DMLabelledIntegerAttribute ***

// ---------------------------------------------------------------------------
//		* DMLabelledIntegerAttribute()
// ---------------------------------------------------------------------------
//	Default constructor

DMLabelledIntegerAttribute::DMLabelledIntegerAttribute()

: DMContainerMixin((DMElement&) *this)

{
	mLabelsOnly = false;
}


// ---------------------------------------------------------------------------
//		* DMLabelledIntegerAttribute(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. Stream data is:
//
//		[DMIntegerAttribute data]
//		Boolean:		true if values must be among the labelled value set

DMLabelledIntegerAttribute::DMLabelledIntegerAttribute(
	LStream*	inStream)

: DMIntegerAttribute(inStream),
  DMContainerMixin((DMElement&) *this)

{
	(*inStream)	>> mLabelsOnly;
}


// ---------------------------------------------------------------------------
//		* DMLabelledIntegerAttribute(DMLabelledIntegerAttribute&)
// ---------------------------------------------------------------------------
//	Copy constructor

DMLabelledIntegerAttribute::DMLabelledIntegerAttribute(
	const DMLabelledIntegerAttribute&	inOriginal)

: DMIntegerAttribute(inOriginal),
  DMContainerMixin((DMElement&) *this, inOriginal)

{

	// Validate pointers.

	ValidateObject_(&inOriginal);
	ValidateObject_(inOriginal.mValueLabels.GetObject());
	
	// Copy reference to original's value labels. For memory
	// efficiency, we don't clone the labels themselves.
	
	mValueLabels = inOriginal.mValueLabels;

}


// ---------------------------------------------------------------------------
//		* ~DMLabelledIntegerAttribute
// ---------------------------------------------------------------------------
//	Destructor

DMLabelledIntegerAttribute::~DMLabelledIntegerAttribute()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** value accessors

// ---------------------------------------------------------------------------
//		* GetTextValue
// ---------------------------------------------------------------------------
//	Overriden to return the value label instead of the number (if a
//	value label exists.)

void
DMLabelledIntegerAttribute::GetTextValue(
	LStr255&	outValue) const
{

	// Validate pointers.

	ValidateThis_();

	// Get the integer value.
	
	SInt32 value = GetIntValue();
	
	// See if there's a value label that matches it.
	
	Boolean foundLabel = false;
	if (mValueLabels != nil) {
	
		DMFastIterator iter(mValueLabels->GetSubElements());
		while (iter.NextElement()) {

			DMElement* objDM = iter.CurrentElement();
			ValidateObject_(objDM);
			
			DMValueLabel* label = (dynamic_cast<DMValueLabel*>(objDM));
			ValidateObject_(label);

			if (label->GetValue() == value) {
				label->GetLabel(outValue);
				foundLabel = true;
				break;
			}
		}
	}
	
	// If we didn't find a label, just return the numeric value.
	
	if (!foundLabel)
		DMIntegerAttribute::GetTextValue(outValue);

}


// ---------------------------------------------------------------------------
//		* SetTextValue
// ---------------------------------------------------------------------------
//	Overriden to see if the text string matches one of the value labels.

void
DMLabelledIntegerAttribute::SetTextValue(
	ConstStringPtr	inValue)
{

	// Validate pointers.

	ValidateThis_();

	// See if there's a value label that matches it.
	
	if (mValueLabels != nil) {
	
		DMFastIterator iter(mValueLabels->GetSubElements());
		while (iter.NextElement()) {

			DMElement* objDM = iter.CurrentElement();
			ValidateObject_(objDM);
			
			DMValueLabel* label = (dynamic_cast<DMValueLabel*>(objDM));
			ValidateObject_(label);
			
			if (label->MatchLabel(inValue)) {
				SetIntValue(label->GetValue());
				return;
			}
		}
	}
	
	// If we didn't find a label, just read the value as a numeric string.
	
	DMIntegerAttribute::SetTextValue(inValue);

}


// ===========================================================================

#pragma mark -
#pragma mark ** initialization

// ---------------------------------------------------------------------------
//		* FinishCreateSelf										[protected]
// ---------------------------------------------------------------------------
//	Move the value labels to a container which can be shared among
//	multiple instances of this attribute.

void
DMLabelledIntegerAttribute::FinishCreateSelf()
{

	// Validate pointers.

	ValidateThis_();

	// Create a container for value labels.

	if (mValueLabels == nil)
		mValueLabels = new DMContainerAttribute;
	ValidateObject_(mValueLabels.GetObject());
	
	// Move all value labels to this container.
	
	DMIterator iter(mSubElements);
	while (iter.NextElement()) {
	
		DMElement* objDM = iter.CurrentElement();
		ValidateObject_(objDM);
		
		TSharablePtr<DMValueLabel> label;
		label = (dynamic_cast<DMValueLabel*>(objDM));
		
		ValidateObject_(label.GetObject());
		RemoveElement(label);
		mValueLabels->InsertElementAt(LArray::index_Last, label, true);

	}
}
