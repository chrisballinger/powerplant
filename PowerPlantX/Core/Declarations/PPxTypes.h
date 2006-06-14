// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxTypes.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:10 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxTypes.h
	@brief		Common type definitions
*/

#ifndef H_PPxTypes
#define H_PPxTypes
#pragma once

#include <CoreServices/CoreServices.h>

namespace PPx {

// ===========================================================================
/**	Template which defines a class based on a built-in integer type */

//	A typedef definies a synonym for an existing type, but does not create
//	a unique new type suitable for overloading functions. For example,
//
//		typedef	unsigned long	FourCharCode;
//		typedef unsigned long	UInt32;
//
//	FourCharCode, UInt32, and unsigned long are really all the same type,
//	so the following is illegal code:
//
//		void foo( UInt32 x ) { }
//		void foo( FourCharCode y ) { }			// same function redefined
//
//	The IntegerType template declares a class that holds the value as
//	as member, and provides coercion operators to access the value.
//
//		struct FourCharCodeStruct { };
//		typedef IntegerType<FourCharCodeStruct, FourCharCode> FourCharCodeType;
//
//		void foo( UInt32 x ) { }
//		void foo( FourCharCodeType y ) { }	// OK, different types

template <class TType, typename TValueType, TValueType defaultValue = 0>
struct	IntegerType {

	IntegerType()						: mValue(defaultValue) { }
						
	IntegerType( TValueType inValue )	: mValue(inValue) { }
						
	operator TValueType() const			{ return mValue; }
	
	operator TValueType&()				{ return mValue; }
	
	TValueType	Get() const				{ return mValue; }

	TValueType		mValue;
};


// ---------------------------------------------------------------------------
//	Identifier Types

typedef		SInt16			ResIDT;
typedef		UInt32			CommandIDT;

typedef		FourCharCode	EventClassT;
typedef		UInt32			EventKindT;

struct ObjectIDStruct { };
typedef IntegerType<ObjectIDStruct, UInt32>			ObjectIDT;


} // namespace PPx

#endif	// H_PPxTypes
