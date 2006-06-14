// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxAttachable.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:20 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxAttachable.h
	@brief		Class for objects which have an associated list of attachments
*/

#ifndef H_PPxAttachable
#define H_PPxAttachable
#pragma once

#include <PPxPrefix.h>
#include <vector>

namespace PPx {

class Attachment;
class DataReader;
class DataWriter;

// ===========================================================================
//	Attachable
/**
	Class for objects which have an associated list of Attachments. Attachable
	is designed to be a mix-in base class or data member of a Persistent
	subclass that wants to support Attachments.								*/

class	Attachable {
public:
						Attachable();
						
						Attachable( const Attachable& inOriginal );
						
	virtual				~Attachable();
	
	Attachable&			operator = ( const Attachable& inSource );

	void				AddAttachment( Attachment* inAttachment );
	
	void				RemoveAttachment( Attachment* inAttachment );
	
	void				RemoveAllAttachments();
	
	Attachment*			FindAttachmentByID( ObjectIDT inID ) const;

protected:
	void				ReadAttachments( const DataReader& inReader );

	void				WriteAttachments( DataWriter& ioWriter ) const;
	
private:

	std::vector<Attachment*>*	mAttachments;	/**< List of Attachments */
};


} // namespace PPx

#endif	// H_PPxAttachable
