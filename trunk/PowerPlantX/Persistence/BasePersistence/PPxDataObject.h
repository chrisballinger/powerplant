// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxDataObject.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:20 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxDataObject.h
	@brief		Classes for storing data values of a particular type
*/

#ifndef H_PPxDataObject
#define H_PPxDataObject
#pragma once

#include <PPxRetained.h>
#include <SysCFString.h>
#include <vector>

namespace PPx {

// ===========================================================================
//	DataObject
/**
	Base class for objects that store a data value. The template classes
	TDataObject and TDataVector inherit from this class, so all their
	template instantiations have a common base class. This lets us store
	pointers to DataObjects in containers.									*/

class	DataObject : public Retained {
public:
						DataObject() { }

	virtual				~DataObject() { }
};


// ===========================================================================
//	TDataObject< TData >
/**
	Template class for objects that store a single data value of type TData	*/

template <typename TData>
struct	TDataObject : public DataObject {
public:
						TDataObject()
							: mValue() { }
						
						TDataObject( const TData& inValue )
							: mValue(inValue) { }

	TData		mValue;
};


// ===========================================================================
//	TDataVector< TData >
/**
	Template class for objects that store a vector of data values of
	type TData																*/

template <typename TData>
struct	TDataVector : public DataObject {

						TDataVector() { }
						
	template <typename TInputIterator>
						TDataVector(
								TInputIterator	inFirst,
								TInputIterator	inLast)
							: mDataValues(inFirst, inLast) { }

	std::vector<TData>	mDataValues;
};


} // namespace PPx

#endif // H_PPxDataObject
