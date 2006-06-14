// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxNibRegistrar.h
//
//	Copyright © 2004-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:24 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#ifndef H_PPxNibRegistrar
#define H_PPxNibRegistrar
#pragma once

#include <SysCFString.h>
#include <SysCFXMLTree.h>

namespace PPx {

class	Persistent;
class	View;

// ===========================================================================
//	NibRegistrar
/**
	Implements new-by-name creation of Persistent objects from nib files */

namespace	NibRegistrar {

						/** Nib decoder function signature */
	typedef Persistent*	(*CreatorFunction)(
								View*				inSuperView,
								const CFXMLTree&	inTree);

										// Class name registry for system views
	void				RegisterClass(
								const CFString&	inClassName,
								CreatorFunction	inCreatorFunc);
								
	void				UnregisterClass( const CFString& inClassName );
	
	bool				IsRegistered( const CFString& inClassName );
	
										// Class name registry for custom HIViews
	void				RegisterCustomClass(
								const CFString&	inClassName,
								CreatorFunction	inCreatorFunc);
								
	void				UnregisterCustomClass( const CFString& inClassName );
	
	bool				IsCustomRegistered( const CFString& inClassName );
	
										// Create an object from a class name
	Persistent*			CreateObject(
								const CFString&		inClassName,
								View*				inSuperView,
								const CFXMLTree&	inTree);
}


} // namepsace PPx

#endif	// H_PPxNibRegistrar
