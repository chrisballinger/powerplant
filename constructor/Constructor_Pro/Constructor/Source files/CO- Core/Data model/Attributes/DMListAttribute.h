// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMListAttribute.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 07/16/96
//	   $Date: 2006/01/18 01:32:32 $
//	$History: DMListAttribute.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:15
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:19
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 12/12/96   Time: 18:58
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

#pragma once

	// Core : Data model : Attributes
#include "DMContainerAttribute.h"


// ===========================================================================
//		* DMListAttribute
// ===========================================================================
//
//	A list attribute is a container that contains a repeating list of
//	a particular kind of attribute.
//
// ===========================================================================

class DMListAttribute : public DMContainerAttribute {

	DMClassID_				('list', DMListAttribute, DMContainerAttribute);

public:
							DMListAttribute();
							DMListAttribute(LStream* inStream);
							DMListAttribute(const DMListAttribute& inOriginal);
	virtual					~DMListAttribute();

	// prototype accessor
	
	inline DMAttribute*		GetPrototype() const
									{ return mPrototype; }

	// subitem information
	
	void					GetItemTitle(
									LStr255&			outTitle) const;
	void					SetItemTitle(
									ConstStringPtr		inTitle);

	inline Boolean			GetZeroBasedIndex() const
									{ return mZeroBasedIndex; }

	// object streaming

	virtual void			ReadStreamData(
									LStream*			inStream,
									FourCharCode		inStreamSelector = 0);
	virtual void			WriteStreamData(
									LStream*			inStream,
									FourCharCode		inStreamSelector = 0) const;

	// post-initialization

protected:
	virtual void			FinishCreateSelf();


	// data members

protected:
	GSharableStringPtr		mItemTitle;					// item title
	DMAttributePtr			mPrototype;					// prototype of objects used in list
	SInt32					mCountSize;					// number of bytes to use for count
	Boolean					mZeroBasedCount;			// true to write list count starting
														//	from zero, not one (not recommended,
														//	used for ResEdit compatibility)
	Boolean					mZeroBasedIndex;			// true to count members from zero
														//	in property inspector

};
