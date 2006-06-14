// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxApplication.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:26 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxApplication.h
	@brief		Class for an executable program
*/

#ifndef H_PPxApplication
#define H_PPxApplication
#pragma once

#include <PPxEventTarget.h>
#include <PPxAttachable.h>

namespace PPx {

// ===========================================================================
//	Application
/**
	An executable program */

class Application : public ApplicationEventTarget,
					public Attachable {
public:
						Application();

	virtual 			~Application();
	
	void				Run();

protected:	
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;
								
private:
	virtual CFStringRef	ClassName() const;

private:									// Forbid copy and assignment
						Application( const Application& );
	Application&		operator = ( const Application& );
};

} // namespace PPx

#endif	// H_PPxApplication
