// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*
	96.12.09	ebs		Added DefaultToFullColorTable() for Robin
	96.08.08	ebs		Added support for PTPF prefs resource (reading only - no saving) 
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "PTPrefs.h"
#include "SUFileUtils.h"
#include "SUMiscUtils.h"

/*=================================
	PTPreferenceRec
	
	this structure should stay the same between 68K & PowerPC
	we can either use #pragma's or keep everything as long integers
==================================*/
const ResIDT	pref_ResID				= 9000;
const ResType	pref_ResType			= 'PTPF';

const UInt32	pref_Version = 0;

	// flags
const UInt32	pref_Use8ForPicts 		= 0x01;		// 8-bit instead of 32?
const UInt32	pref_Use8ForOthers		= 0x02;		// 8-bit instead of 32?
const UInt32	pref_UsePalettes		= 0x04;
const UInt32	pref_DefaultToFullTable	= 0x08;		// default to 256 colors (rather than 36)
const UInt32	pref_DefaultFlags		= 0;


typedef struct PTPreferenceRec
{
	UInt32			version;				// hiword/loword format
	UInt32			flags;					// see above
	SInt32			reserved[ 18 ];
} PTPreferenceRec, **PTPreferenceH;

static PTPreferenceRec		gPrefs;

/*=================================
	Initialize
==================================*/
void PTPrefs::Initialize()
{
	PTPreferenceH	h = nil;
	static Boolean 	sCalledOnce = false;
	
	/***************************************
		we only want a single call
	**************************************/
	if ( sCalledOnce ) return;
	sCalledOnce = true;
	
	/***************************************
		in case of error or if prefs don't exist, default everything first
	**************************************/
	GetDefaultPrefs( &gPrefs );
	
	/***************************************
		get the resource out of the application
	**************************************/
	try
	{
		/*
			look in the app resource file for a preference resource.
			we'll use FileHasResource() before GetResource() to keep from
			  breaking into the debugger if the resource doesn't exist
			  since it doesn't currently exist.
		*/
		ResFileIDT	appFileID = SUFileUtils::GetMainResourceFile();
		
		if ( !SUFileUtils::FileHasResource( appFileID, pref_ResType, pref_ResID ) )
			return;

		h = (PTPreferenceH) SUFileUtils::GetResource( appFileID, pref_ResType, pref_ResID );

		/***************************************
			if the version & size of the resource are ok, store the data away
		**************************************/
		if (	h &&
				( HiWord((**h).version) == HiWord(pref_Version) ) &&
				( ::GetHandleSize( (Handle) h ) == sizeof( PTPreferenceRec ) ) )
			gPrefs = **h;
	}
	catch( ... )
	{
		return;
	}

	/***************************************
		cleanup
	**************************************/
	SUMiscUtils::DisposeHandle( (Handle) h );
}

/*=================================
	GetDefaultPrefs
==================================*/
void PTPrefs::GetDefaultPrefs( struct PTPreferenceRec *outRec )
{
		// start everything off cleared
	SUMiscUtils::ClearMemory( outRec, sizeof( PTPreferenceRec ) );
	 
	outRec->version = pref_Version;
	outRec->flags = pref_DefaultFlags;
}

/*=================================
	Save
==================================*/
void PTPrefs::Save()
{
}

/*=================================
	Use32BitBuffersForPicts
==================================*/
Boolean PTPrefs::Use32BitBuffersForPicts()
{
	return( (gPrefs.flags & pref_Use8ForPicts) == 0 );
}

/*=================================
	UsePalettesForWindows
==================================*/
Boolean PTPrefs::UsePalettesForWindows()
{
	return( (gPrefs.flags & pref_UsePalettes) != 0 );
}

/*=================================
	Use32BitBuffersForOthers
==================================*/
Boolean PTPrefs::Use32BitBuffersForOthers()
{
	return( (gPrefs.flags & pref_Use8ForOthers) == 0 );
}

/*=================================
	DefaultToFullColorTable
==================================*/
Boolean PTPrefs::DefaultToFullColorTable()
{
	return( (gPrefs.flags & pref_DefaultToFullTable) != 0 );
}
