// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:					SUBitUtils.h

CLASSES:				SUBitUtils

DESCRIPTION:		

AUTHOR:				Eric Shapiro

CREATION DATE :	96.4.18

CHANGE HISTORY :

		96.4.18		ebs	Integrated into Robin's sources
	
---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

	This class is just a wrapper for some bit set/get/clear utility
	routines. It is based on code from my old "E-Lib".
	
	Notes:
	Bits are numbered from 0..n.
	Bits are set from left to right, just like pixels in a bitmap.
	
	These routines may only work up to 32K bits. (???) 
	  -- they haven't been tested any higher 
	
---------------------------------------<< ¥ >>----------------------------------------
*/
	
class SUBitUtils
{
	public:
		static UInt8		GetBit( void *firstByte, SInt32 bitNo );
		static void			SetBit( void *firstByte, SInt32 bitNo, UInt8 inValue );

		static UInt8		GetNibble( void *firstByte, SInt32 nibbleNo );
		static void			SetNibble( void *firstByte, SInt32 nibbleNo, UInt8 inValue );
		
		static UInt8		GetTwoBits( void *firstByte, SInt32 index );
		static void			SetTwoBits( void *firstByte, SInt32 index, UInt8 inValue );
};

