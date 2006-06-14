// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	TCLModelTabInfo.cpp			© 2001-2002 Metrowerks Inc. All rights reserved.
// ===========================================================================


// ===========================================================================

#include "TCLModelTabInfo.h"


// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	STR_TCLTitleStrings		= 7081;
const ResIDT	str_TabInfo				= 1;
const ResIDT	str_SpaceOpenQuote		= 2;
const ResIDT	str_CloseQuote			= 3;


// ===========================================================================

#pragma mark *** TCLModelTabInfo ***

// ---------------------------------------------------------------------------
//		* TCLModelTabInfo(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. No extra data is read for MEModelMenu.

TCLModelTabInfo::TCLModelTabInfo(
	LStream* inStream)

	: DMObject(inStream), mCopyValues(false)
{
}


// ---------------------------------------------------------------------------
//		* TCLModelTabInfo(TCLModelTabInfo&)
// ---------------------------------------------------------------------------
//	Copy constructor

TCLModelTabInfo::TCLModelTabInfo(
	const TCLModelTabInfo& inOriginal)

	: DMObject(inOriginal)
{
	ValidateObject_(&inOriginal);

	mCopyValues = true;
	mIconSuiteID = inOriginal.GetIconSuiteID();
	inOriginal.GetTabName(mTabName);
}


// ---------------------------------------------------------------------------
//		* TCLModelTabInfo()
// ---------------------------------------------------------------------------
//	Plain constructor

TCLModelTabInfo::TCLModelTabInfo()

	: DMObject(), mCopyValues(false)
{
	SetIconSuiteID(0);
	LStr255 text = Str_Empty;
	SetTabName(text);
}


// ---------------------------------------------------------------------------
//		* ~TCLModelTabInfo
// ---------------------------------------------------------------------------
//	Destructor

TCLModelTabInfo::~TCLModelTabInfo()
{
}


// ---------------------------------------------------------------------------
//		* FinishCreateSelf
// ---------------------------------------------------------------------------

void
TCLModelTabInfo::FinishCreateSelf()
{
	if (mCopyValues)
	{
		SetIconSuiteID(mIconSuiteID);
		SetTabName(mTabName);
	}
}

Boolean
TCLModelTabInfo::IsDerivedFrom(FourCharCode inClassID) const
{
	return (inClassID == class_ID) || (DMObject::IsDerivedFrom(inClassID));
}
	
DMElement*
TCLModelTabInfo::CreateFromStream(LStream* inStream)
{
	return new TCLModelTabInfo(inStream);
}

DMElement*
TCLModelTabInfo::Clone() const
{
	DMElement* clone = new TCLModelTabInfo(*this);
	try
	{
		clone->FinishCreate();
	}
	catch(...)
	{
		delete clone;
		throw;
	}

	return clone;
}
	
DMElement*
TCLModelTabInfo::ShallowClone() const
{
	DMElement* clone = new TCLModelTabInfo(*this);
	clone->SetShallowClone();
	try
	{
		clone->FinishCreate();
	}
	catch(...)
	{
		delete clone;
		throw;
	}

	return clone;
}


// ===========================================================================

#pragma mark -
#pragma mark ** accessors

// ---------------------------------------------------------------------------
//		* GetDisplayableName
// ---------------------------------------------------------------------------
//	Make a displayable name containing the word Menu Item and the text
//	of the menu item.

void
TCLModelTabInfo::GetDisplayableName(
	LStr255&	outTitle) const
{
	// Validate pointers.
	ValidateThis_();
	
	// Start with object type.
	outTitle = LStr255(STR_TCLTitleStrings, str_TabInfo);

	// Add tab info name.
	
	LStr255 name;
	GetTabName(name);
	if (name.Length() > 0)
	{
		outTitle += LStr255(STR_TCLTitleStrings, str_SpaceOpenQuote);
		outTitle += name;
		outTitle += LStr255(STR_TCLTitleStrings, str_CloseQuote);
	}
}
