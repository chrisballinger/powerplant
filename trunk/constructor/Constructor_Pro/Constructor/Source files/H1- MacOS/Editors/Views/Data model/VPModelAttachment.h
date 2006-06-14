// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPModelAttachment.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/14/96
//     $Date: 2006/01/18 01:33:54 $
//  $History: VPModelAttachment.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:39
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:44
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/20/96   Time: 20:14
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added IsVisualObject.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 10:52
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added class.
//	
// ===========================================================================

#pragma once

	// MacOS : Editors : Views : Data model
#include "VPModelObject.h"


// ===========================================================================
//		* VPModelAttachment
// ===========================================================================
//
//	VPModelObject is the data model representation of a PowerPlant attachment.
//	It doesn't really do anything different than VPModelObject. It just has
//	a different class ID, which is useful for drag-receive handling.
//
// ===========================================================================

class VPModelAttachment : public VPModelObject {

	// data model
	
	DMClassID_				('vpma', VPModelAttachment, VPModelObject);

public:
							VPModelAttachment();
							VPModelAttachment(LStream* inStream);
							VPModelAttachment(const VPModelAttachment& inOriginal);
	virtual					~VPModelAttachment();

	// accessors

	virtual Boolean			IsVisualObject() const;

};
