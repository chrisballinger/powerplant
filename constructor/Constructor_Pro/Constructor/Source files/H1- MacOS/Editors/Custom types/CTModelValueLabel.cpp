// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	CTModelValueLabel.cpp	    © 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: Tue, 10 Dec 1996
//     $Date: 2006/01/18 01:33:50 $
//  $History: CTModelValueLabel.cpp $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:02
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Custom types
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:00p
//	Updated in $/ConstructorWin/Sources
//	Initial merge with MSVC build
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:42
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Custom types
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 12/10/96   Time: 14:22
//	Created in $/Constructor/Source files/Editors/Custom types
//	Baseline CTYP editor.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "CTModelValueLabel.h"


// ===========================================================================

#pragma mark *** CTModelValueLabel ***

// ---------------------------------------------------------------------------
//		* CTModelValueLabel()
// ---------------------------------------------------------------------------
//	Default constructor

CTModelValueLabel::CTModelValueLabel()
{
}


// ---------------------------------------------------------------------------
//		* CTModelValueLabel(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. No additional data is read.

CTModelValueLabel::CTModelValueLabel(
	LStream*	inStream)

: CTModelObject(inStream)

{
}


// ---------------------------------------------------------------------------
//		* CTModelValueLabel(CTModelValueLabel&)
// ---------------------------------------------------------------------------
//	Copy constructor

CTModelValueLabel::CTModelValueLabel(
	const CTModelValueLabel&	inOriginal)

: CTModelObject(inOriginal)

{
}


// ---------------------------------------------------------------------------
//		* ~CTModelValueLabel
// ---------------------------------------------------------------------------
//	Destructor

CTModelValueLabel::~CTModelValueLabel()
{
}


// ---------------------------------------------------------------------------
//	* CanDeleteThis
// ---------------------------------------------------------------------------
//	We override here because CTYP labels (e.g. RGB color, labeled int) can
//	be deleted. We skip over the direct-inheritance of CTModelObject because
//	that explicitly denies deletion.
//
//	This fixes MW07458 - JCD 980619

Boolean
CTModelValueLabel::CanDeleteThis() const
{
	// skip up one level of inheritance
	
	return DMObject::CanDeleteThis();
}