// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
#pragma once

class SURegionUtils
{
	public:
		static RgnHandle	NewRegionFromRect( const Rect &r );
		static RgnHandle	NewRegionFromRect( SInt32 left, SInt32 top, SInt32 right, SInt32 bottom );
		static RgnHandle	NewRegionFromPolygon( PolyHandle inPoly );
		static RgnHandle	NewRegionFromRegion( RgnHandle );
		static RgnHandle	NewRegion();
		static void			DisposeRegion( RgnHandle & );
		
	protected:
		static Handle		GetHandleFromClipboard();
};

