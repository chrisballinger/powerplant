// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPModelAttachment.cpp		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/14/96
//     $Date: 2006/01/18 01:33:54 $
//  $History: VPModelAttachment.cpp $
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

#include "VPModelAttachment.h"


// ===========================================================================

#pragma mark *** VPModelAttachment ***

// ---------------------------------------------------------------------------
//		* VPModelAttachment()
// ---------------------------------------------------------------------------
//	Default constructor

VPModelAttachment::VPModelAttachment()
{
}


// ---------------------------------------------------------------------------
//		* VPModelAttachment(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. No additional stream data is read

VPModelAttachment::VPModelAttachment(
	LStream*	inStream)

: VPModelObject(inStream)

{
}


// ---------------------------------------------------------------------------
//		* VPModelAttachment(VPModelAttachment&)
// ---------------------------------------------------------------------------
//	Copy constructor

VPModelAttachment::VPModelAttachment(
	const VPModelAttachment&	inOriginal)

: VPModelObject(inOriginal)

{
}


// ---------------------------------------------------------------------------
//		* ~VPModelAttachment
// ---------------------------------------------------------------------------
//	Destructor

VPModelAttachment::~VPModelAttachment()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** accessors

// ---------------------------------------------------------------------------
//		* IsVisualObject
// ---------------------------------------------------------------------------
//	Returns false since attachments are not visual objects.

Boolean
VPModelAttachment::IsVisualObject() const
{
	return false;
}
