// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxRegistrar.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:20 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxRegistrar.h>

namespace PPx {

namespace {	// unnamed

	typedef std::map<CFString, Registrar::CreatorFunction>	RegistryType;
	
	RegistryType&
	GetRegistry()
	{
		static RegistryType		sRegistry;
		return sRegistry;
	}
	
} // unnamed namespace


// ---------------------------------------------------------------------------
//	RegisterClass													  [public]
/**
	Registers a class name and creator function pair
	
	@param	inClassName		Name of class to register
	@param	inCreatorFunc	Function which creates an object of the class	*/

void
Registrar::RegisterClass(
	const CFString&		inClassName,
	CreatorFunction		inCreatorFunc)
{
	GetRegistry().insert(std::make_pair(inClassName, inCreatorFunc));
}


// ---------------------------------------------------------------------------
//	UnregisterClass													  [public]
/**
	Removes registration for a class
	
	@param	inClassName		Name of class to unregister						*/

void
Registrar::UnregisterClass(
	const CFString&		inClassName)
{
	GetRegistry().erase(inClassName);
}


// ---------------------------------------------------------------------------
//	IsRegistered													  [public]
/**
	Tests if a class is already registered
	
	@param	inClassName		Class name
	
	@return	Whether or not the class is registered							*/

bool
Registrar::IsRegistered(
	const CFString&		inClassName)
{
	return (GetRegistry().find(inClassName) != GetRegistry().end());
}



// ---------------------------------------------------------------------------
//	CreateObject													  [public]
/**
	Create an object of the class with the specified name
	
	@param	inClassName		Class name
	
	@return	Pointer to newly created object									*/

Persistent*
Registrar::CreateObject(
	const CFString&		inClassName)
{
	RegistryType::const_iterator	classPos = GetRegistry().find(inClassName);
	
	if (classPos == GetRegistry().end()) {
		PPx_Throw_(RuntimeError, err_Runtime, "Unregistered class name");
	}
	
	return (classPos->second)();
}

} // namespace PPx