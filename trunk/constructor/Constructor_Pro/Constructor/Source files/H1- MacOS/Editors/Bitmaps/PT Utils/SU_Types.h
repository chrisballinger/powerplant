// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
#pragma once

typedef SInt16		VolIDT;
typedef SInt32		DirIDT;
typedef SInt16		FileIDT;
typedef SInt16		ResFileIDT;

/*
	HFS volumes may break the 32-bit limit, so we might as well
	make it possible to find code dependent on the file size.
*/
typedef UInt32		DiskSizeT;
typedef UInt32		FileSizeT;		
typedef UInt32		MemSizeT;

/*
	a few other things shared by several pieces of code
*/
typedef UInt32		PixelValue;		// raw values within a bitmap/pixmap
typedef UInt32		Color32;		// RGB color in a long (0:R:G:B)

typedef UInt8 *		RawPtr;
typedef UInt8		RawByte;

/*
	Three ways to redraw
*/
enum ERedrawOptions
{
	redraw_Now,
	redraw_Later,
	redraw_Dont
};

