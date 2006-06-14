// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
#pragma once

class SUOffscreen;
class RFMap;

class PTGraphicConversions
{
	public:
	
	static PicHandle 			OffscreenToPicture( SUOffscreen *inBuffer, RgnHandle = nil );
	static void					OffscreenToClipboard( SUOffscreen *, RgnHandle = nil );

	static PicHandle 			GetPictFromClipboard();
	static RgnHandle 			GetRegionFromClipboard();

	static PicHandle			GetPictFromFile( const FSSpec & );
	
	static void					SaveOffscreenAsResource( 
										RFMap *, ResType inResType, ResIDT inResID,
										SUOffscreen *inBuffer, SInt32 rowBytes = -1,
										SUOffscreen *maskBuffer = nil, SInt32 maskOffset = 0,
										SInt32 maskRowBytes = -1 );

	static SUOffscreen *		PictureToOffscreen( PicHandle inPict );
};

