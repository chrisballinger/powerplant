// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPDuplicatePaneIDException.h
//								© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 12/06/96
//     $Date: 2006/01/18 01:33:54 $
//  $History: VPDuplicatePaneIDException.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:39
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:44
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 12/06/96   Time: 15:08
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added  class.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Exceptions
#include "DMValidationException.h"


// ===========================================================================
//		* VPDuplicatePaneIDException
// ===========================================================================
//
//	This exception is thrown by VPModelObject::BashPaneID when it detects
//	that a pane ID field points to an object whose ID is duplicate.
//
// ===========================================================================

class VPDuplicatePaneIDException : public DMValidationException {

public:
							VPDuplicatePaneIDException(
									PaneIDT			inAttemptedPaneID);
	virtual					~VPDuplicatePaneIDException();

	virtual void			GetErrorString(
									LStr255&		outDescriptor) const;


	// data members

protected:
	PaneIDT					mAttemptedPaneID;		// the pane ID entered by the user

};
