// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
#pragma once

#include "SU_Types.h"

class SUFileUtils
{
	public:
	
		/***************************************
			Resource
		****************************************/
		static ResFileIDT	GetMainResourceFile();

		static Handle		GetResource( ResFileIDT rezFileID, ResType rezType, ResIDT rezID, Boolean *outLoadedFlag = nil );
		static Handle		GetAppResource( ResType rezType, ResIDT rezID, Boolean *outLoadedFlag = nil );
		static Handle		GetLoadedResource( ResFileIDT rezFileID, ResType rezType, ResIDT rezID );

		static void			SaveDataAsResource( ResFileIDT inFileID, ResType, ResIDT, 
												void *inData, SInt32 inBytes,
												Boolean flushToo );
		static void			FlushResourceFile( ResFileIDT inFileID );
		
		static void 		CheckConditionsForResourceSave( 
										ResFileIDT inResFileID, DiskSizeT inDiskSpace, 
										MemSizeT inTotalRAM, MemSizeT inContigRAM );

		static VolIDT		GetResourceFileVolume( ResFileIDT inFileID );
		static Boolean		FileHasResource( ResFileIDT rezFileID, ResType rezType, ResIDT rezID );
		
		/***************************************
			Misc
		****************************************/
		static void			CreateTempFile( FSSpec *outSpec );
		static Boolean		VolumeHasSpace( VolIDT inVol, DiskSizeT inSpace );
		static void			GetIndString( ResFileIDT, ResIDT, SInt32 inIndex, char *outString );
		static void			GetIndPString( ResFileIDT, ResIDT, SInt32 inIndex, unsigned char *outString );
};

