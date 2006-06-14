// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysAEHandler.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:33 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		SysAEHandler.h
	@brief		Utility classes for managing Apple Event Handlers
*/

#ifndef H_SysAEHandler
#define H_SysAEHandler
#pragma once

#include <PPxPrefix.h>
#include <ApplicationServices/ApplicationServices.h>

namespace PPx {

// ===========================================================================
//	SysAEHandler
/**
	Wrapper class for an Apple Event Handler */

class	SysAEHandler {
public:
						SysAEHandler();
						
						~SysAEHandler();
						
	void				Install(
								AEEventClass		inClassID,
								AEEventID			inEventID,
								AEEventHandlerUPP	inCallback,
								void*				inRefCon,
								bool				inIsSystemHandler = false);
								
	void				Remove();
	
	void				GetInfo(
								AEEventClass&		outClassID,
								AEEventID&			outEventID,
								bool&				outIsSystemHandler) const;

private:									// Forbid copy and assignment
						SysAEHandler( const SysAEHandler& );
	SysAEHandler&		operator = ( const SysAEHandler& );
						
private:
	AEEventClass		mAEClassID;			/**< Apple Event class ID */
	AEEventID			mAEEventID;			/**< Apple Event event ID */
	AEEventHandlerUPP	mAECallback;		/**< Callback function */
	bool				mIsSystemHandler;	/**< System or Application
													handler? All running
													programs can use
													system handlers. */
	bool				mIsInstalled;		/**< Is handler installed? */
};


// ===========================================================================
//	SysAEHandlerUPP
/**
	Wrapper class for an Apple Event callback function UPP */

class	SysAEHandlerUPP {
public:
	explicit			SysAEHandlerUPP( AEEventHandlerProcPtr inCallbackFunc );
						
						~SysAEHandlerUPP();
						
	AEEventHandlerUPP	Get() const;
	
private:									// Forbid copy and assignment
						SysAEHandlerUPP( const SysAEHandlerUPP& );
	SysAEHandlerUPP&	operator = ( const SysAEHandlerUPP& );
	
private:
	AEEventHandlerUPP	mAEHandlerUPP;		/**< Callback function UPP */
};


// ---------------------------------------------------------------------------
inline
AEEventHandlerUPP
SysAEHandlerUPP::Get() const
{
	return mAEHandlerUPP;
}


} // namespace PPx

#endif // H_SysAEHandler
