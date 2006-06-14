// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxPersistent.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:20 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxPersistent.h
	@brief		Abstract base class for persistent objects
*/

#ifndef H_PPxPersistent
#define H_PPxPersistent
#pragma once

#include <PPxPrefix.h>

namespace PPx {

class DataWriter;
class DataReader;


// ===========================================================================
//	Persistent
/**
	Abstract base class for persistent objects. A persistent object can
	write its state to external data, and then later on be recreated
	from that data.															*/

class	Persistent {
public:
	virtual				~Persistent();
	
	CFStringRef			GetClassName() const;
	
	void				InitPersistent( const DataReader& inReader );
	
	void				FinishInitPersistent();
	
	void				WritePersistent( DataWriter& ioWriter ) const;
	
protected:
	virtual void		InitState( const DataReader& inReader);

	virtual void		WriteState( DataWriter& ioWriter ) const;

private:
	virtual void		FinishInit();
	
							/** Pure virtual function that concrete subclasses
								must override to return the class name */
	virtual CFStringRef	ClassName() const = 0;
};


// ---------------------------------------------------------------------------
//	GetClassName													  [public]
/**
	Returns name of class as a string */

inline CFStringRef
Persistent::GetClassName() const
{
	return ClassName();
}


// ---------------------------------------------------------------------------
//	InitPersistent													  [public]
/**
	Initializes persistent object from a DataReader. This is a non-virtual
	wrapper function that calls the virtual InitState() function.			*/

inline void
Persistent::InitPersistent(
	const DataReader&	inReader)
{
	InitState(inReader);
}


// ---------------------------------------------------------------------------
//	FinishInitPersistent											  [public]
/**
	Completes the initialization of a Peristent object. This is a
	non-virtual wrapper function that calls the virtual FinishInit()
	function.																*/

inline void
Persistent::FinishInitPersistent()
{
	FinishInit();
}


// ---------------------------------------------------------------------------
//	WritePersistent													  [public]
/**
	Writes data of a persistent object to a DataWriter. This is a
	non-virtual wrapper function that calls the virtual WriteState()
	function.																*/

inline void
Persistent::WritePersistent(
	DataWriter&	ioWriter) const
{
	WriteState(ioWriter);
}


} // namepsace PPx

#endif	// H_PPxPersistent
