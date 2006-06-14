// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxApplication.cp
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:26 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxApplication.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	Application														  [public]
/**
	Default constructor */

Application::Application()
{
}


// ---------------------------------------------------------------------------
//	~Application													  [public]
/**
	Destructor */

Application::~Application()
{
}


// ---------------------------------------------------------------------------
//	Run																  [public]
/**
	Run the main applicaton event loop */

void
Application::Run()
{
	::RunApplicationEventLoop();
}


// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
Application::ClassName() const
{
	return CFSTR("PPx::Application");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
Application::InitState(
	const DataReader&	inReader)
{
	ReadAttachments(inReader);
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
Application::WriteState(
	DataWriter&	ioWriter) const
{
	WriteAttachments(ioWriter);
}


} // namespace PPx