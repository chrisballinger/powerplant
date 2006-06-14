// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysEventTypes.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:34 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		SysEventTypes.h
	@brief		Wrapper classes for types used as Carbon Event parameters
*/

#ifndef H_SysEventTypes
#define H_SysEventTypes
#pragma once

#include <PPxPrefix.h>

#include <Carbon/Carbon.h>

namespace PPx {


// ===========================================================================
/**	Template wrapper class for HIObjectRef types */

//	??? Using HIObjectRef as the TValueType in the IntegerType template
//		above produces a compiler error about an illegal non-type parameter.
//		Not sure why, but it seems related to it being a pointer type.

template <class TType>
class	HIObjectRefType {
public:
	HIObjectRefType()						: mObjectRef(nil) { }
	
	HIObjectRefType( HIObjectRef inRef )	: mObjectRef(inRef) { }
	
	operator HIObjectRef() const			{ return mObjectRef; }
	
	operator HIObjectRef&()					{ return mObjectRef; }
	
	HIObjectRef	Get() const					{ return mObjectRef; }
	
private:
	HIObjectRef	mObjectRef;
};


// ===========================================================================
/**	Wrapper for HIToolbarRef*/

struct HIToolbarRefStruct { };
typedef HIObjectRefType<HIToolbarRefStruct>		HIToolbarRefType;


// ===========================================================================
/**	Wrapper for HIToolbarItemRef */

struct HIToolbarItemRefStruct { };
typedef HIObjectRefType<HIToolbarItemRefStruct>	HIToolbarItemRefType;


} // namespace PPx

#endif	// H_SysEventTypes
