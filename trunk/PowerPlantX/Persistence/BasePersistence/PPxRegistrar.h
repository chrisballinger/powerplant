// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxRegistrar.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:20 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxRegistrar.h
	@brief		Functions for managing a table of class names and creator
				functions used for implementig new-by-name for Persistent
				objects.
*/

#ifndef H_PPxRegistrar
#define H_PPxRegistrar
#pragma once

#include <SysCFString.h>
#include <map>

namespace PPx {

class	Persistent;

// ===========================================================================
//	Registrar
/**
	Implements new-by-name creation of Persistent objects */

namespace	Registrar {

	typedef Persistent*	(*CreatorFunction)();

	void				RegisterClass(
								const CFString&	inClassName,
								CreatorFunction	inCreatorFunc);
								
	template <class TSubClass>
	void				RegisterClass( const CFString& inClassName );
	
	void				UnregisterClass( const CFString& inClassName );
	
	bool				IsRegistered( const CFString& inClassName );
	
	Persistent*			CreateObject( const CFString& inClassName );
	
	template <class TSubClass>
	Persistent*			CreateNewObject();
}


// ---------------------------------------------------------------------------
//	RegisterClass
/**
	Template function to register a class
	
	The template parameter is the class, which must be a subclass of
	Persistent
	
	@param	inClassName		Class name										*/

template <class TSubClass>
void
Registrar::RegisterClass(
	const CFString&	inClassName)
{
	RegisterClass(inClassName, &CreateNewObject<TSubClass>);
}


// ---------------------------------------------------------------------------
//	CreateNewObject
/**
	Creates a new object of the class specified by the template parameter
	
	@return	Pointer to newly created object									*/

template <class TSubClass>
Persistent*
Registrar::CreateNewObject()
{
	return (new TSubClass);
}


// ---------------------------------------------------------------------------
//	PPx_RegisterPersistent_( Class )
/**	
	Registers a subclass of PPx::Persistent so that objects can be created
	via new-by-name using the Registrar
	
	@param	Class		Class name											*/			

#define PPx_RegisterPersistent_(Class)										\
	PPx::Registrar::RegisterClass< Class >(CFSTR(#Class))
								

} // namepsace PPx

#endif	// H_PPxRegistrar
