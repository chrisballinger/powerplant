// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMListAttribute.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 07/16/96
//	   $Date: 2006/04/12 08:47:44 $
//	$History: DMListAttribute.cpp $
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 03/11/97   Time: 10:56
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Removed CW Win32 template bug workaround. Got an update from Bob K that
//	fixes the problem.
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 03/05/97   Time: 09:35
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Added a workaround for CW Win32 compiler.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:14
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Improved commenting.
//	
//	*****************  Version 5  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:00p
//	Updated in $/ConstructorWin/Sources
//	Initial merge with MSVC build
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:19
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Fixed CR/LF problem
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/07/97   Time: 17:46
//	Updated in $/Constructor/Source files/Data model/Attributes
//	Rolled in an update from Clint 01/05/97.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 12/12/96   Time: 18:57
//	Updated in $/Constructor/Source files/Data model/Attributes
//	Rolled in Clint's changes for zero-based indexing in property
//	inspector. Improved commenting.
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

#include "DMListAttribute.h"

	// Core : Data model : Utility classes
#include "DMIterator.h"

	// Core : Utilities
#include "GSharableString.h"


// ===========================================================================

#pragma mark *** DMListAttribute ***

// ---------------------------------------------------------------------------
//		* DMListAttribute()
// ---------------------------------------------------------------------------
//	Default constructor

DMListAttribute::DMListAttribute()
{

	// Reset flags.

	mCountSize = 2;
	mZeroBasedCount = false;
	mZeroBasedIndex = false;

}


// ---------------------------------------------------------------------------
//		* DMListAttribute(LStream*)
// ---------------------------------------------------------------------------

DMListAttribute::DMListAttribute(
	LStream*	inStream)

: DMContainerAttribute(inStream)

{

	// Read stream data.

	Str255 subItemTitle;
	SInt8 flags;
	
	(*inStream)	>> subItemTitle
				>> mCountSize
				>> flags;

	// Set title and flags.

	SetItemTitle(subItemTitle);

	mZeroBasedCount = (flags & 0x01) != 0;
	mZeroBasedIndex = (flags & 0x02) != 0;

}


// ---------------------------------------------------------------------------
//		* DMListAttribute(DMListAttribute&)
// ---------------------------------------------------------------------------
//	Copy constructor

DMListAttribute::DMListAttribute(
	const DMListAttribute& inOriginal)

: DMContainerAttribute(inOriginal)

{

	// Copy values from original.

	mItemTitle = inOriginal.mItemTitle;
	mPrototype = inOriginal.mPrototype;		// share prototypes
	mCountSize = inOriginal.mCountSize;
	mZeroBasedCount = inOriginal.mZeroBasedCount;
	mZeroBasedIndex = inOriginal.mZeroBasedIndex;

}


// ---------------------------------------------------------------------------
//		* ~DMListAttribute
// ---------------------------------------------------------------------------
//	Destructor

DMListAttribute::~DMListAttribute()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** subitem information

// ---------------------------------------------------------------------------
//		* GetItemTitle
// ---------------------------------------------------------------------------
//	Return the title that's associated with each subitem.

void
DMListAttribute::GetItemTitle(
	LStr255&	outTitle) const
{

	// Validate pointers.

	ValidateThis_();
	
	// Return the item title.

	if (mItemTitle != nil)
		outTitle = *((LString*) mItemTitle);		// cast is necessary, otherwise it takes it as a void*
	else
		outTitle = "\001";

}


// ---------------------------------------------------------------------------
//		* SetItemTitle
// ---------------------------------------------------------------------------
//	Set the title that's associated with each subitem.

void
DMListAttribute::SetItemTitle(
	ConstStringPtr	inTitle)
{

	// Validate pointers.

	ValidateThis_();
	
	// Create a sharable string object that's optimized for
	// the size of this string.

	SInt16 length = inTitle[0];
	if (length > 63)
		mItemTitle = new TSharableString<Str255>(inTitle);
	else if (length > 31)
		mItemTitle = new TSharableString<Str63>(inTitle);
	else if (length > 15)
		mItemTitle = new TSharableString<Str31>(inTitle);
	else if (length > 0)
		mItemTitle = new TSharableString<Str15>(inTitle);
	else
		mItemTitle = nil;
	
	if (length > 0)
		ValidateObject_(mItemTitle.GetObject());

}


// ===========================================================================

#pragma mark -
#pragma mark ** object streaming

// ---------------------------------------------------------------------------
//		* ReadStreamData
// ---------------------------------------------------------------------------
//	Read the item count and create that number of objects.
//	The subobjects will be streamed in by DMContainerAttribute's
//	ReadStreamData.

void
DMListAttribute::ReadStreamData(
	LStream*		inStream,
	FourCharCode	inStreamSelector)
{

	// Validate pointers.

	ValidateThis_();

	// Check stream selector and include in stream flag.
	
	if ((inStreamSelector == mStreamSelector) && mInStream) {

		// Read item count.
		
		SInt32 count = 0;
		switch (mCountSize) {
			case 1:
				UInt8 count8;
				count8 = 0;
				*inStream >> count8;
				if (mZeroBasedCount && count8 == 255)
					count = -1;
				else
					count = count8;
				break;
			
			case 2:
				UInt16 count16;
				count16 = 0;
				*inStream >> count16;
				if (mZeroBasedCount && count16 == 65535)
					count = -1;
				else
					count = count16;
				break;
				
			case 4:
				*inStream >> count;
				break;
			
			default:
				SignalCStr_("mCountSize is invalid");

		}
		
		// Adjust count for zero-based count.
		
		if (mZeroBasedCount)
			count++;
		
		// Create this number of items.
		
		ValidateObject_(mPrototype.GetObject());

		while (count--) {
			InsertElementAt(LArray::index_Last, mPrototype->Clone(), true);
		}

		// Read their data.
		
		DMContainerAttribute::ReadStreamData(inStream, inStreamSelector);
		
	}
}


// ---------------------------------------------------------------------------
//		* WriteStreamData
// ---------------------------------------------------------------------------
//	Write the item count in the appropriate format, then call
//	DMContainerAttribute::WriteStreamData to write the object
//	data for each subitem.

void
DMListAttribute::WriteStreamData(
	LStream*		inStream,
	FourCharCode	inStreamSelector) const
{

	// Validate pointers.

	ValidateThis_();

	// Check stream selector and include in stream flag.
	
	if ((inStreamSelector == mStreamSelector) && mInStream) {

		// Write item count.
		
		SInt32 count = mSubElements.GetCount();
		if (mZeroBasedCount)
			count--;
		
		switch (mCountSize) {
			case 1:
				UInt8 count8;
				count8 = count;
				*inStream << count8;
				break;
			
			case 2:
				UInt16 count16;
				count16 = count;
				*inStream << count16;
				break;
				
			case 4:
				*inStream << count;
				break;

			default:
				SignalCStr_("mCountSize is invalid");

		}
		
		// Write each object's data.
		
		DMContainerAttribute::WriteStreamData(inStream, inStreamSelector);
		
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** post-initialization

// ---------------------------------------------------------------------------
//		* FinishCreateSelf										[protected]
// ---------------------------------------------------------------------------
//	If we don't have a prototype, we need to get one.
//	We assume that the first element created is an attribute which represents
//	the subitem to be repeated. Move it to the special "mPrototype" slot
//	and leave an empty subitems list. Subitems will be created later
//	when ReadStreamData is called.

void
DMListAttribute::FinishCreateSelf()
{

	// Validate pointers.

	ValidateThis_();

	// Prevent duplication of prototype.

	if (mPrototype != nil)
		return;  //we already have a prototype (we are a clone) [CCP: 1/5/97]
	
	// Move the first element, which we hope is an attribute,
	// to the prototype slot.

	DMIterator iter(mSubElements);
	while (iter.NextElement()) {

		DMElement* element = iter.CurrentElement();
		ValidateObject_(element);
		
		DMAttributePtr attr;
		attr = (dynamic_cast<DMAttribute*>(element));
		ValidateObject_(attr.GetObject());

		RemoveElement(element);
		mPrototype = attr;
		break;

	}
	
		//! TEMPORARY: what if there are multiple attributes?
		//	Should we build a container attribute and put our subs in it?
	
}
