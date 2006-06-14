// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
#pragma once

class SUPopupArtist
{
	public:
			// these are the only calls you'll usually need to make
		static void			DrawPopup( const Rect &inBounds, Boolean inHilited, Boolean inEnabled );
		static void			CalculateSwatchArea( const Rect &inBounds, Rect *outSwatchR );
		
			// these are the specific drawing routines
		static void			DrawPopupNormalBW( const Rect &inBounds );
		static void			DrawPopupNormalColor( const Rect &inBounds );
	
		static void			DrawPopupHilitedBW( const Rect &inBounds );
		static void			DrawPopupHilitedColor( const Rect &inBounds );
		
		static void			DrawPopupDimmedBW( const Rect &inBounds );
		static void			DrawPopupDimmedColor( const Rect &inBounds );
		
		
	protected:
		static void			DrawPopupArrow( const Rect &inBounds, SInt16 inDepth, 
											Boolean inHilited, Boolean inEnabled );

		static void			GetArrowColor( SInt16 inDepth, 
											Boolean inHilited, Boolean inEnabled,
											RGBColor *outColor );

};

