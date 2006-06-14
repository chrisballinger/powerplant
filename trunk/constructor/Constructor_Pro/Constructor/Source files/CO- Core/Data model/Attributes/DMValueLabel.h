// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMValueLabel.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 06/13/96
//	   $Date: 2006/01/18 01:32:33 $
//	$History: DMValueLabel.h $
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

	// Core : Data model : Core objects
#include "DMElement.h"

	// PowerPlant : Support classes
#include <LString.h>


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class LStream;


// ===========================================================================
//		* DMValueLabel
// ===========================================================================
//
//	DMValueLabel contains a single value/label pair. It is intended for use
//	as a subitem of DMLabelledIntegerAttribute.
//
// ===========================================================================

class DMValueLabel : public DMElement {

	DMClassID_				('labl', DMValueLabel, DMElement);

public:
							DMValueLabel();
							DMValueLabel(LStream* inStream);
							DMValueLabel(const DMValueLabel& inOriginal);
	virtual					~DMValueLabel();

	// accessors
	
	inline void				GetLabel(LStr255& outLabel) const
									{ outLabel = mLabel; }
	inline void				SetLabel(const LStr255& inLabel)
									{ mLabel = inLabel; }
	Boolean					MatchLabel(ConstStringPtr inLabel) const;

	inline SInt32			GetValue() const
									{ return mValue; }
	inline void				SetValue(SInt32 inValue)
									{ mValue = inValue; }


	// data members

protected:
	SInt32					mValue;					// the value
	LStr255					mLabel;					// label for value

};
