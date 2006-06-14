// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	TCLModelTabControl.cpp		© 2001-2002 Metrowerks Inc. All rights reserved.
// ===========================================================================


// ===========================================================================

#include "TCLModelTabControl.h"


// ===========================================================================

#pragma mark *** TCLModelTabControl ***

// ---------------------------------------------------------------------------
//		* TCLModelTabControl(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. No extra data is read for MEModelMenu.

TCLModelTabControl::TCLModelTabControl(
	LStream* inStream)

: DMObject(inStream)

{
}


// ---------------------------------------------------------------------------
//		* TCLModelTabControl(TCLModelTabControl&)
// ---------------------------------------------------------------------------
//	Copy constructor

TCLModelTabControl::TCLModelTabControl(
	const TCLModelTabControl& inOriginal)

: DMObject(inOriginal)

{
	ValidateObject_(&inOriginal);
}


// ---------------------------------------------------------------------------
//		* ~TCLModelTabControl
// ---------------------------------------------------------------------------
//	Destructor

TCLModelTabControl::~TCLModelTabControl()
{
}


Boolean
TCLModelTabControl::IsDerivedFrom(FourCharCode inClassID) const
{
	return (inClassID == class_ID) || (DMObject::IsDerivedFrom(inClassID));
}
	
DMElement*
TCLModelTabControl::CreateFromStream(LStream* inStream)
{
	return new TCLModelTabControl(inStream);
}

DMElement*
TCLModelTabControl::Clone() const
{
	DMElement* clone = new TCLModelTabControl(*this);
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
TCLModelTabControl::ShallowClone() const
{
	DMElement* clone = new TCLModelTabControl(*this);
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
