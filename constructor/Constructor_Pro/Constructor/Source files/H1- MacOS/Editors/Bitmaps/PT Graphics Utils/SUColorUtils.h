// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
#pragma once

#include "SU_Types.h"

class SUColorUtils
{
	public:
		static SInt32 		DepthToNumColors( SInt32 inDepth );
		static SInt32 		NumColorsToDepth( SInt32 inColors );

		static Boolean		IsColorInTable( CTabHandle inTable, const RGBColor &inColor, UInt8 *outIndex = nil );
		static double		CalcRGBDistance( const RGBColor &, const RGBColor & );

		static CTabHandle	NewColorTableByDepth( SInt32 depth );
		static CTabHandle	NewColorTableByEntries( SInt32 depth );
		static CTabHandle	NewColorTableFromPtr( SInt32 depth, UInt8 *sourceData );
		static CTabHandle	GetColorTable( SInt32 inResourceID );
		static void			FixColorTableBW( CTabHandle inTable, Boolean inForceEm );
		
		static UInt16		CalcRowBytes( SInt32 width, SInt32 height );
		static UInt16		CalcPixmapRowBytes( SInt32 width, SInt32 height );
		
		static RGBColor		Color32ToRGB( Color32 inColor );
		static RGBColor		Color16ToRGB( Color32 inColor );
		static Color32		RGBToColor32( const RGBColor & );
		static Color32		RGBToColor16( const RGBColor & );
		
		static Boolean		EqualRGB( const RGBColor &, const RGBColor & );
		static Boolean		EqualColor32( Color32, Color32 );
		static Boolean		EqualColor16( Color32, Color32 );
		
		static void			SetForeColor32( Color32 );
		static void			SetBackColor32( Color32 );
};


