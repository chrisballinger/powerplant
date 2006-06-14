// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxCorrespondent.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:13 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxCorrespondent.h
	@brief		A generic Carbon Event target
*/

#ifndef H_PPxCorrespondent
#define H_PPxCorrespondent
#pragma once

#include <PPxEventTarget.h>
#include <PPxAttachable.h>
#include <SysHIObject.h>


namespace PPx {

// ===========================================================================
/**	A generic Event Target. Install handlers for Carbon Events by adding
	Attachments or by creating subclasses that multiply inherit from
	Correspondent and EventDoer subclasses.									*/

class	Correspondent : public EventTarget,
						public Attachable {
public:
						Correspondent();
						
	virtual				~Correspondent();

protected:	
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;
								
private:
	virtual CFStringRef	ClassName() const;

	virtual EventTargetRef
						SysEventTarget() const;
						
										// Forbid copy and assignment
						Correspondent( const Correspondent& );
	Correspondent&		operator = ( const Correspondent& );
	
private:
	SysHIObject			mSysHIObject;
};


} // namespace PPx

#endif	// H_PPxCorrespondent
