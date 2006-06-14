// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysEventHandler.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:34 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		SysEventHandler.h
	@brief		Utility classes for managing Carbon Event handlers
*/

#ifndef H_SysEventHandler
#define H_SysEventHandler
#pragma once

#include <PPxPrefix.h>
#include <Carbon/Carbon.h>

namespace PPx {

// ===========================================================================
//	SysEventHandler
/**
	Wrapper class for a Carbon Event Handler */

class	SysEventHandler {
public:
						SysEventHandler();
						
						~SysEventHandler();
						
	void				Install(
								EventTargetRef			inTarget,
								EventHandlerUPP			inCallback,
								void*					inUserData,
								UInt32					inNumTypes,
								const EventTypeSpec*	inTypeList);
							
	void				Install(
								EventTargetRef			inTarget,
								EventHandlerUPP			inCallback,
								void*					inUserData,
								UInt32					inEventClass,
								UInt32					inEventKind);
							
	void				Remove();
	
	bool				IsInstalled() const;
	
	void				Adopt( EventHandlerRef inHandlerRef );
	
	EventHandlerRef		Detach();
	
private:									// Forbid copy and assignment
						SysEventHandler( const SysEventHandler& );
	SysEventHandler&	operator = ( const SysEventHandler& );
	
private:
	EventHandlerRef		mHandlerRef;	/**< System reference to the
												installed event handler */
};


// ===========================================================================
//	SysEventHandlerUPP
/**
	Wrapper class for a Carbon Event callback function UPP */

class	SysEventHandlerUPP {
public:
	explicit			SysEventHandlerUPP( EventHandlerProcPtr inCallbackFunc );
					
						~SysEventHandlerUPP();
					
	EventHandlerUPP		Get() const;
	
private:									// Forbid copy and assignment
						SysEventHandlerUPP( const SysEventHandlerUPP& );
	SysEventHandlerUPP&	operator = ( const SysEventHandlerUPP& );
	
private:
	EventHandlerUPP		mEventHandlerUPP;	/**< Callback function UPP */
};


// ---------------------------------------------------------------------------

inline
EventHandlerUPP
SysEventHandlerUPP::Get() const
{
	return mEventHandlerUPP;
}


} // namespace PPx

#endif // H_SysEventHandler
