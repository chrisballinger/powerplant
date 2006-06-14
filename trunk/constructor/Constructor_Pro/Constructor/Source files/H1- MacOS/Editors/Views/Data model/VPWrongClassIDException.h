// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPWrongClassIDException.h   © 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 12/06/96
//     $Date: 2006/01/18 01:33:57 $
//  $History: VPWrongClassIDException.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:40
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:45
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 12/06/96   Time: 15:11
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added  class.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Attributes
#include "DMAttribute.h"

	// Core : Data model : Exceptions
#include "DMValidationException.h"


// ===========================================================================
//		* VPWrongClassIDException
// ===========================================================================
//
//	This exception is thrown by VPModelObject::BashPaneID when it detects
//	that a pane ID field points to an object that isn't of the correct
//	class.
//
// ===========================================================================

class VPWrongClassIDException : public DMValidationException {

public:
							VPWrongClassIDException(
									DMAttribute*		inAttribute,
									PaneIDT				inAttemptedPaneID,
									ClassIDT			inClassID);
	virtual					~VPWrongClassIDException();

	virtual void			GetErrorString(
									LStr255&			outDescriptor) const;


	// data members

protected:
	DMAttributePtr			mAttribute;					// the attribute containing the pane ID
	PaneIDT					mAttemptedPaneID;			// the pane ID that was attempted
	ClassIDT				mClassID;					// ID of the required class

};
