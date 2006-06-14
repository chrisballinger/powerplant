// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysCFData.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:29 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		SysCFData.h
	@brief		Wrapper class for a Core Foundataion Data object
*/

#ifndef H_SysCFData
#define H_SysCFData
#pragma once

#include <SysCFMutableObject.h>
#include <CoreFoundation/CoreFoundation.h>

namespace PPx {

// ===========================================================================
//	CFData
/**
	Wrapper class for a Core Foundataion Data object */

class	CFData : public CFMutableObject<CFDataRef, CFMutableDataRef> {
public:
									// Constructors
						CFData();
						
						CFData(	CFDataRef	inDataRef,
								bool		inRetain);
						
						CFData( CFMutableDataRef	inDataRef,
								bool				inRetain);
						
	explicit			CFData(	CFIndex			inCapacity,
								CFAllocatorRef	inAllocator = nil);
								
						CFData(	const UInt8*	inBytes,
								CFIndex			inByteCount,
								CFAllocatorRef	inAllocator = nil);
						
						CFData( const CFData& inOriginal );
								
	CFData&				operator = ( const CFData& inSource );
								
									// Accessors
	CFIndex				GetLength() const;
	
	const UInt8*		GetBytePtr() const;
	
	void				GetDataBytes(
								CFRange		inRange,
								UInt8*		outBuffer) const;

									// Mutators
	UInt8*				GetMutableBytePtr();

	void				SetLength( CFIndex inNewLength );

	void				IncreaseLength( CFIndex inExtraLength );

	void				AppendBytes(
								const UInt8*	inBytes,
								CFIndex			inByteCount);

	void				ReplaceBytes(
								CFRange			inRange,
								const UInt8*	inNewBytes,
								CFIndex			inByteCount);

	void				DeleteBytes( CFRange inRange );

private:
	virtual CFMutableDataRef
						MutableCopy( CFDataRef inRefToCopy ) const;

private:	
	typedef CFMutableObject<CFDataRef, CFMutableDataRef>	BaseT;
};

} // namespace PPx

#endif // H_SysCFData
