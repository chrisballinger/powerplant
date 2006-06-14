// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxAttachment.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:20 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxAttachment.h
	@brief		Abstract class for identifiable persistent objects
*/

#ifndef H_PPxAttachment
#define H_PPxAttachment
#pragma once

#include <PPxPersistent.h>
#include <PPxIdentifiable.h>

namespace PPx {

// ===========================================================================
//	Attachment
/**
	Abstract class for identifiable persistent objects
	
	Other persistent objects may store pointers to Attachments. The Object ID
	allows an Attachment created from persistent data to be found at runtime.
*/

class	Attachment : public Persistent,
					 public Identifiable {
protected:
	virtual void		InitState( const DataReader& inReader );

	virtual void		WriteState( DataWriter& ioWriter ) const;
};


} // namespace PPx

#endif	// H_PPxAttachment
