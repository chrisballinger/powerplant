// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxRetained.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:11 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxRetained.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	Retained
/**
	Default constructor */

Retained::Retained()
{
	mRetainCount = 0;
}


// ---------------------------------------------------------------------------
//	Retained
/**
	Copy constructor */

Retained::Retained(
	const Retained&	/* inOriginal */)
{
	mRetainCount = 0;
}


// ---------------------------------------------------------------------------
//	~Retained
/**
	Destructor */

Retained::~Retained()
{
}


// ---------------------------------------------------------------------------
//	operator =
/**
	Assignment operator */

Retained&
Retained::operator = (
	const Retained&	/* inSource */)
{
	return *this;						// Retain count does not change
}


// ---------------------------------------------------------------------------
//	Retain
/**
	Increment object's retain count */

void
Retained::Retain() const
{
	++mRetainCount;
}


// ---------------------------------------------------------------------------
//	Release
/**
	Decrements object's retain count */

void
Retained::Release()
{
	if (--mRetainCount == 0) {
		delete this;
	}
}


// ---------------------------------------------------------------------------
//	GetRetainCount
/**
	Returns object's retain count
	
	@return	Retain count of object											*/

UInt32
Retained::GetRetainCount() const
{
	return mRetainCount;
}


} // namespace PPx
