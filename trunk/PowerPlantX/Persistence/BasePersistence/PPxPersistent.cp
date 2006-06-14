// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxPersistent.cp
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:20 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxPersistent.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	~Persistent														  [public]
/**
	Destructor	*/

Persistent::~Persistent()
{
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state of persistent object from a data dictionary

	@param	inReader	Data dictionary from which to read persistent data

	@note Subclasses with state data should override this function to
	read their own data, and call the inherited function					*/

void
Persistent::InitState(
	const DataReader&	/* inReader */)
{
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state of persistent object to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data

	@note Subclasses with state data should override this function to
	write their own data, and call the inherited function					*/

void
Persistent::WriteState(
	DataWriter&	/* ioWriter */) const
{
}


// ---------------------------------------------------------------------------
//	FinishInit														 [private]
/**
	Completes the initialization process for a persistent object.
	Subclasses should override to perform intialization tasks.				*/
	
void
Persistent::FinishInit()
{
}


} // namespace PPx