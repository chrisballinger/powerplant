// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*
	note: most of the code for these routines are inlined in the header file
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "SUPixelHelper.h"


/*=========================================
	GetPixelFunctions
==========================================*/
void SUPixelHelper::GetPixelFunctions( SInt32 inDepth, SUPixelGetter *outGetter, SUPixelSetter *outSetter )
{
	switch( inDepth )
	{
		case 32:
			*outGetter = GetColor32;
			*outSetter = SetColor32;
			break;
		case 16:
			*outGetter = GetColor16;
			*outSetter = SetColor16;
			break;
		case 8:
			*outGetter = GetColor8;
			*outSetter = SetColor8;
			break;
		case 4:
			*outGetter = GetColor4;
			*outSetter = SetColor4;
			break;
		case 2:
			*outGetter = GetColor2;
			*outSetter = SetColor2;
			break;
		case 1:
			*outGetter = GetColor1;
			*outSetter = SetColor1;
			break;
		default:
			LException::Throw( paramErr );
	}
}

