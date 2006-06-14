// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxNibRegistrar.cp
//
//	Copyright © 2004-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:24 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxNibRegistrar.h>
#include <map>
#include <PPxPersistent.h>
#include <PPxView.h>

namespace PPx {

namespace {	// unnamed

	typedef std::map<CFString, NibRegistrar::CreatorFunction>	RegistryType;
	typedef std::map<CFString, NibRegistrar::CreatorFunction>	CustomRegistryType;
	
	RegistryType&
	GetRegistry()
	{
		static RegistryType			sRegistry;
		return sRegistry;
	}
	
	CustomRegistryType&
	GetCustomRegistry()
	{
		static CustomRegistryType	sCustomRegistry;
		return sCustomRegistry;
	}
	
} // unnamed namespace


// ---------------------------------------------------------------------------
//	RegisterClass													  [public]
/**
	Registers a class name and creator function pair
	
	@param	inClassName		Name of class to register
	@param	inCreatorFunc	Function which creates an object of the class	*/

void
NibRegistrar::RegisterClass(
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
NibRegistrar::UnregisterClass(
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
NibRegistrar::IsRegistered(
	const CFString&		inClassName)
{
	return (GetRegistry().find(inClassName) != GetRegistry().end());
}


// ---------------------------------------------------------------------------
//	RegisterCustomClass												  [public]
/**
	Registers a class name and creator function pair
	
	@param	inClassName		Name of class to register
	@param	inCreatorFunc	Function which creates an object of the class	*/

void
NibRegistrar::RegisterCustomClass(
	const CFString&		inClassName,
	CreatorFunction		inCreatorFunc)
{
	GetCustomRegistry().insert(std::make_pair(inClassName, inCreatorFunc));
}


// ---------------------------------------------------------------------------
//	UnregisterCustomClass											  [public]
/**
	Removes registration for a class
	
	@param	inClassName		Name of class to unregister						*/

void
NibRegistrar::UnregisterCustomClass(
	const CFString&		inClassName)
{
	GetCustomRegistry().erase(inClassName);
}


// ---------------------------------------------------------------------------
//	IsCustomRegistered												  [public]
/**
	Tests if a class is already registered
	
	@param	inClassName		Class name
	
	@return	Whether or not the class is registered							*/

bool
NibRegistrar::IsCustomRegistered(
	const CFString&		inClassName)
{
//	return (GetCustomRegistry().find(inClassName) != GetCustomRegistry().end());
	return (GetCustomRegistry().count(inClassName) != 0);
}


// ---------------------------------------------------------------------------
//	CreateObject													  [public]
/**
	Create an object of the class with the specified name
	
	@param	inClassName		Class name
	@param	inSuperView		+++
	@param	inTree			+++
	
	@return	Pointer to newly created object									*/

Persistent*
NibRegistrar::CreateObject(
	const CFString&		inClassName,
	View*				inSuperView,
	const CFXMLTree&	inTree)
{
	Persistent*	object = nil;

	RegistryType::const_iterator	classPos = GetRegistry().find(inClassName);
	
	if (classPos != GetRegistry().end()) {

		object = (classPos->second)(inSuperView, inTree);

	} else {

		CustomRegistryType::const_iterator	customClassPos = GetCustomRegistry().find(inClassName);

		if (customClassPos != GetCustomRegistry().end()) {
		
			object = (customClassPos->second)(inSuperView, inTree);

		} else {
			std::string tempClassName;
			inClassName.GetString(tempClassName);
			tempClassName.insert(0, "Unregistered class name:  ");
			PPx_Throw_(RuntimeError, err_Runtime, tempClassName.c_str());
			//PPx_SignalString_(tempClassName.c_str());
		}
	}
	
	return object;
}


} // namespace PPx