// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	CTModelObject.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 12/09/96
//     $Date: 2006/01/18 01:33:50 $
//  $History: CTModelObject.cpp $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:02
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Custom types
//	Improved commenting.
//	
//	*****************  Version 4  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:00p
//	Updated in $/ConstructorWin/Sources
//	Initial merge with MSVC build
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:42
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Custom types
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 12/17/96   Time: 11:14
//	Updated in $/Constructor/Source files/Editors/Custom types
//	Updated GetDisplayableObjectName to wrap attribute title in quotes.
//	(Bug fix #1106.)
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

#include "CTModelObject.h"


// ===========================================================================

#pragma mark *** CTModelObject ***

// ---------------------------------------------------------------------------
//		* CTModelObject()
// ---------------------------------------------------------------------------
//	Default constructor

CTModelObject::CTModelObject()
{
}


// ---------------------------------------------------------------------------
//		* CTModelObject(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. Stream data is:
//
//		[DMObject data]
//		Str255:			Attribute type title
//		Uint32:			Data model type ID for this attribute type
//		Uint32:			Data model type ID for parent object's type

CTModelObject::CTModelObject(
	LStream*	inStream)

: DMObject(inStream)

{

	// Read stream data.
	
	Str255 title;
	
	(*inStream)	>> title
				>> mTypeID
				>> mParentTypeID;
	
	// Set title.
	
	mTitle = new TSharableString<Str31>(title);
	ValidateObject_(mTitle.GetObject());

}

// ---------------------------------------------------------------------------
//		* CTModelObject(CTModelObject&)
// ---------------------------------------------------------------------------
//	Copy constructor

CTModelObject::CTModelObject(
	const CTModelObject&	inOriginal)

: DMObject(inOriginal)

{
	mTitle = inOriginal.mTitle;
	mTypeID = inOriginal.mTypeID;
	mParentTypeID = inOriginal.mParentTypeID;
}


// ---------------------------------------------------------------------------
//		* ~CTModelObject
// ---------------------------------------------------------------------------
//	Destructor

CTModelObject::~CTModelObject()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** accessors

// ---------------------------------------------------------------------------
//		* GetDisplayableObjectType
// ---------------------------------------------------------------------------
//	Return a string that identifies this object's type.

void
CTModelObject::GetDisplayableObjectType(
	LStr255&	outObjectType) const
{
	outObjectType = *mTitle;
}

// ---------------------------------------------------------------------------
//		* GetDisplayableName
// ---------------------------------------------------------------------------
//	Return a string that can be used as a title for this object
//	(typically used in property inspectors).

void
CTModelObject::GetDisplayableName(
	LStr255&	outTitle) const
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Build displayable name from class name and object ID.
	
	GetDisplayableObjectType(outTitle);
	
	// If the object has a title or caption, add that in, too.
	
	LStr255 title;
	GetTitle(title);
	
	outTitle += (StringPtr)"\002 \"";			//! TEMPORARY
	outTitle += title;
	outTitle += (StringPtr)"\001\"";
	
}


// ---------------------------------------------------------------------------
//	* CanDeleteThis
// ---------------------------------------------------------------------------
//	We override here to prevent root-level objects from being deleted.
//	Of course, anything that inherits from this, if it can be deleted, will
//	need to override this (see CTModelAttribute and CTModelValueLabel)
//
//	This should only affect the top-level object in the CTYP editor (the
//	actual class properties (name, icon, is abstract, class ID, parent
//	class ID, default height/width, is must be root).
//
//	This fixes MW07458. - JCD 980619

Boolean
CTModelObject::CanDeleteThis() const
{
	ValidateThis_();
	
	return false;
}