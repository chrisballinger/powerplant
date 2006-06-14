// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMToolboxBooleanAttribute.h	© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 06/13/96
//	   $Date: 2006/01/18 01:32:33 $
//	$History: DMToolboxBooleanAttribute.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:15
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Improved commenting.
//
//	*****************  Version 2   *****************
//	User: scouten      Date: 01/24/97   Time: 17:20
//	Checked in '$/Constructor/Source files/CO- Core/Data model/Attributes'
//	Comment: Fixed CR/LF problem
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:29
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Attributes
#include "DMIntegerAttribute.h"


// ===========================================================================
//		* DMToolboxBooleanAttribute
// ===========================================================================
//
//	DMToolboxBooleanAttribute is a variant on DMInteger attribute that writes
//	one- or two-byte boolean values. These are required for certain Toolbox
//	managers. A value of "true" is recorded as 0xFF or 0x01FF. A value of
//	"false" is always recorded as zero.
//
//	Updated September 27, 1996 (eas).
//
// ===========================================================================

class DMToolboxBooleanAttribute : public DMIntegerAttribute {

	DMClassID_				('tbxb', DMToolboxBooleanAttribute, DMIntegerAttribute);

public:
							DMToolboxBooleanAttribute();
							DMToolboxBooleanAttribute(LStream* inStream);
							DMToolboxBooleanAttribute(const DMToolboxBooleanAttribute& inOriginal);
	virtual					~DMToolboxBooleanAttribute();

	// display information
	
	inline SInt16			GetValueBytes() const
									{ return mValueBytes; }
	inline void				SetValueBytes(SInt16 inValueBytes)
									{ mValueBytes = inValueBytes; }

	// raw data streaming

protected:
	virtual void			ReadStreamDataSelf(LStream* inStream);
	virtual void			WriteStreamDataSelf(LStream* inStream) const;


	// data members

protected:
	SInt16					mValueBytes;					// number of bytes occupied by this value (1 or 2)

};
