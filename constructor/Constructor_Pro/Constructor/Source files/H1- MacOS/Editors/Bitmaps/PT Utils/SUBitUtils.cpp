// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:					SUBitUtils.cp

CLASSES:				SUBitUtils

DESCRIPTION:			Implementation file for Object

AUTHOR:					Eric Shapiro

CREATION DATE :			96.4.18

CHANGE HISTORY :

		96.6.17		ebs		Added SetTwoBits & GetTwoBits
		96.5.9		ebs		Added SetNibble & GetNibble
		96.4.18		ebs		Integrated into Robin's sources

---------------------------------------<< ¥ >>-----------------------------------------

	Notes:
	Bits are numbered from 0..n.
	Bits are set from left to right, just like pixels in a bitmap.
	
	These routines may only work up to 32K bits. (???) 
	  -- they haven't been tested any higher 
	
---------------------------------------<< ¥ >>----------------------------------------
*/

#include "SUBitUtils.h"
 
 
// note: this stuff might not work beyond 32K bits -- test this ???

/*=================
	GetBit
	
	Returns 0..1
==================*/
UInt8 SUBitUtils::GetBit( void *firstByte, SInt32 flagNo )
{
	SInt32 	byteIndex = (flagNo >> 3);
	SInt32	bitNo = flagNo & 0x07;
	UInt8	bitMask = 0x0080 >> bitNo;

	if ( *(byteIndex + (UInt8*)firstByte) & bitMask )
		return( 1 );
	else
		return( 0 );
}

/*=================
	SetBit
==================*/
void SUBitUtils::SetBit( void *firstByte, SInt32 flagNo, UInt8 inValue )
{
	ThrowIf_( inValue >= 2 );		// debug only ???

	SInt32 	byteIndex = (flagNo >> 3);
	SInt32	bitNo = flagNo & 0x07;
	UInt8	bitMask = 0x0080 >> bitNo;

	if ( inValue )
		*( (UInt8*) firstByte + byteIndex ) |= bitMask;		// set bit
	else
		*( (UInt8*) firstByte + byteIndex ) &= ~bitMask;	// clear bit
}


/*=================
	GetNibble
	
	Returns 0..15
==================*/
UInt8 SUBitUtils::GetNibble( void *firstByte, SInt32 nibbleNo )
{
	UInt8	theValue;
	
	theValue = *( (UInt8*) firstByte + (nibbleNo>>1) );
	if ( nibbleNo & 1 )
		return( theValue & 0x0F );
	else
		return( (theValue >> 4) & 0x0F );
}

/*=================
	SetNibble
	
	nibbleValue is 0..15
==================*/
void SUBitUtils::SetNibble( void *firstByte, SInt32 nibbleNo, UInt8 nibbleValue )
{
	ThrowIf_( nibbleValue >= 16 );		// debug only ???

	UInt8	*bytePtr = (UInt8*) firstByte + (nibbleNo>>1);
		
	if ( nibbleNo & 1 )
		*bytePtr = (*bytePtr & 0xF0) | nibbleValue;
	else
		*bytePtr = (*bytePtr & 0x0F) | (nibbleValue << 4);
}

/*=================
	GetTwoBits
	
	Returns 0..3
==================*/
UInt8 SUBitUtils::GetTwoBits( void *firstByte, SInt32 index )
{
	UInt8			theValue;
	static UInt8	shifts[] = { 6, 4, 2, 0 };
	
	theValue = *( (UInt8*) firstByte + (index>>2) );
	return( (theValue >> shifts[ index & 0x03 ]) & 0x03 );
}

/*=======================
	SetTwoBits
	
	nibbleValue is 0..3
=======================*/
void SUBitUtils::SetTwoBits( void *firstByte, SInt32 index, UInt8 inValue )
{
	static UInt8	shifts[] = { 6, 4, 2, 0 };

	ThrowIf_( inValue >= 4 );		// debug only ???

	UInt8	*bytePtr = (UInt8*) firstByte + (index>>2);
	UInt8	shiftBy = shifts[ index & 0x03 ];
	UInt8	clearMask = ~(0x3 << shiftBy);
	*bytePtr = (*bytePtr & clearMask) | (inValue << shiftBy) ;
}
