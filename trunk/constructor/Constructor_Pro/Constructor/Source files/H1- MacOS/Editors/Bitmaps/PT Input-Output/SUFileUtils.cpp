// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
  #include "strings.h"
#endif

#include "SUFileUtils.h"
#include "SUMiscUtils.h"
#include "SUSaveResFile.h"
#include "UResourceMgr.h"

/*=================================
	CheckConditionsForResourceSave

	Description:	
	Throws an error if there's not enough disk space or RAM for an operation.
==================================*/
void SUFileUtils::CheckConditionsForResourceSave( ResFileIDT inResFileID, UInt32 inDiskSpace, 
													UInt32 inTotalRAM, UInt32 inContigRAM )
{
		// check the disk space
	SInt32 theVol = GetResourceFileVolume( inResFileID );
	if ( !VolumeHasSpace( theVol, inDiskSpace ) )
		LException::Throw( dskFulErr );
		
		// check RAM
	UInt32	total, contig;
	::PurgeSpace( (SInt32*) &total, (SInt32*) &contig );
	if ( (contig < inContigRAM) || (total < inTotalRAM) )
		LException::Throw( memFullErr );
}

/*=================================
	VolumeHasSpace
	
	Description:
	Returns true if the volume has the required amount of free space.
	Throws an error if the id is invalid.
==================================*/
Boolean SUFileUtils::VolumeHasSpace( VolIDT inVol, UInt32 inSpace )
{
	OSErr				err = noErr;
	HParamBlockRec		pb;
	
	SUMiscUtils::ClearMemory( &pb, sizeof(pb) );		// note: ioVolIndex = 0 -> use vRefNum
	pb.volumeParam.ioVRefNum = inVol;
	
	err = PBHGetVInfoSync( &pb );
	ThrowIfOSErr_( err );
	
	if ( (UInt32) pb.volumeParam.ioVFrBlk * (UInt32) pb.volumeParam.ioVAlBlkSiz >= inSpace )
		return( true );
	else
		return( false );
}

/*=================================
	GetResourceFileVolume

	Description:	
	Returns the volume id for a given resource file.
	Throws an error if the fileID is invalid.
==================================*/
VolIDT SUFileUtils::GetResourceFileVolume( ResFileIDT inFileID )
{
	FCBPBRec		pb;
	OSErr			err = noErr;
	
	SUMiscUtils::ClearMemory( &pb, sizeof(pb) );		// note: ioFCBIndx = 0 -> use ioRefNum
	pb.ioRefNum = inFileID;
	
	/*
		note that PBGetFCBInfo works on both resource files and file mgr files.
		this will still be true under Copland.
	*/
	err = PBGetFCBInfoSync( &pb );
	ThrowIfOSErr_( err );
	
	return( pb.ioFCBVRefNum );		// return "hard" volumeID
}

/*=================================
	SaveDataAsResource
	
	Notes:
	Throws an error if things go sour.
==================================*/
void SUFileUtils::SaveDataAsResource( ResFileIDT inFileID, ResType inResType, ResIDT inResID, 
										 void *inData, SInt32 inBytes, Boolean flushToo )
{
	StSaveResFile		save1;

	ThrowIf_( inFileID == -1 );
	
	/*********************************************
		Since the Resource Mgr sometimes trashes files if it runs out
		of disk space and we don't trust it in general, make sure we
		have enough disk space and RAM to continue before we start
		modifying files.
	*********************************************/
	const UInt32	kDiskSpace		=	7500;			// these are arbitrary -- just guesses
	const UInt32	kTotalRAM		=	10000;
	const UInt32	kContigRAM		=	200;
	
	SUFileUtils::CheckConditionsForResourceSave( inFileID, kDiskSpace + inBytes, 
												kTotalRAM + inBytes, kContigRAM + inBytes );

	/*******************************************
		Get the current resource (if it already exists)
	*******************************************/
	::UseResFile( inFileID );
	Handle h = ::Get1Resource( inResType, inResID );
	
	/*******************************************
		Modify the current resource or create a new one.
		
		According to Jim Luthor at Apple, we shouldn't remove a resource
		and then add a new one because of a bug in the Resource Mgr that
		can truncate the file if things go sour. (5/96)
	*******************************************/
	try
	{
		if ( h )			// resource exists
		{
			/*******************************************
				resize it and copy the data to it
			*******************************************/
			::SetHandleSize( h, inBytes );
			ThrowIfMemError_();
			::BlockMoveData( inData, *h, inBytes );
			
			/*******************************************
				Write the changed resource
			*******************************************/
			::ChangedResource( h );
			ThrowIfResError_();
			
			::WriteResource( h );
			ThrowIfResError_();
		}
		else			// resource doesn't exist
		{
			/*******************************************
				Allocate a block for the resource data and copy the data to it
			*******************************************/
			h = ::NewHandle( inBytes );
			ThrowIfMemFail_( h );
			::BlockMoveData( inData, *h, inBytes );

			/*******************************************
				Add it to the file and write it out
			*******************************************/
			::AddResource( h, inResType, inResID, (StringPtr)"" );
			ThrowIfResError_();
			
			::WriteResource( h);
			ThrowIfResError_();
		}	
	}
	catch( ... )
	{
		SUMiscUtils::DisposeHandle( h );
		throw;
	}
	
	/*****************************************
		Dispose of the resource, but only if we have flushed the file.
		Otherwise, the resource data might never get written.
	*****************************************/
	if ( flushToo )
	{
		FlushResourceFile( inFileID );
		SUMiscUtils::DisposeHandle( h );
	}
}

/*=================================
	FlushResourceFile
==================================*/
void SUFileUtils::FlushResourceFile( ResFileIDT inFileID )
{
	if ( inFileID == -1 ) return;

	::UpdateResFile( inFileID );
	ThrowIfResError_();
	
	OSErr err = ::FlushVol( nil, GetResourceFileVolume( inFileID ) );
	ThrowIfOSErr_( err );
}

/*============================================
	GetMainResourceFile
==============================================*/
ResFileIDT SUFileUtils::GetMainResourceFile()
{
	return( ::LMGetCurApRefNum() );			// change this for plug-ins ???
}

/*============================================
	SUFileUtils::GetResource
	
	Notes:
	Throws an exception if the resource can't be found.
==============================================*/
Handle SUFileUtils::GetResource( ResFileIDT rezFileID, ResType rezType, ResIDT rezID, Boolean *outLoadedFlag )
{
	Handle		h = nil;
	
	/*************************************************
		if caller is interested, see if already in RAM
	**************************************************/
	if ( outLoadedFlag )
	{
		*outLoadedFlag = false;
	
		h = SUFileUtils::GetLoadedResource( rezFileID, rezType, rezID );
		if ( h && *h )
		{
			HNoPurge( (Handle) h );					// since caller probably doesn't want it going away
			*outLoadedFlag = true;
			return( h );
		}
	}
	
	/*************************************************
		else just set the current resource file and read the resource in
	**************************************************/
	StSaveResFile	aSetAndSaver( rezFileID );		// save resFileID and set a new one
	
	h = Get1Resource( rezType, rezID );
	ThrowIfResFail_( h );
	return( h );
}


/*============================================
	SUFileUtils::GetAppResource
	
	Notes:
	Throws an exception if the resource can't be found.
==============================================*/
Handle SUFileUtils::GetAppResource( ResType rezType, ResIDT rezID, Boolean *outLoadedFlag )
{
	return( SUFileUtils::GetResource( LMGetCurApRefNum(), rezType, rezID, outLoadedFlag ) );
}

/*============================================
	SUFileUtils::FileHasResource
==============================================*/
Boolean SUFileUtils::FileHasResource( ResFileIDT resFileID, ResType resType, ResIDT resID )
{
	StSaveResFile	saveFile;				// save/restore resource file
	StResLoad		dontLoad( false );		// prevent loading the resource
	
	::UseResFile( resFileID );
	Handle h = ::Get1Resource( resType, resID );
	return( h != nil );
}

/*============================================
	SUFileUtils::GetLoadedResource
	
	Note:
	Never throws exceptions.
	May return purged handles (be careful).
==============================================*/
Handle SUFileUtils::GetLoadedResource( ResFileIDT rezFileID, ResType rezType, ResIDT rezID )
{
	StSaveResFile	aSetAndSaver( rezFileID );		// save resFileID and set a new one
	StResLoad		aDontLoad( false );				// don't load resources here
	
	return( ::Get1Resource( rezType, rezID ) );
}

/*============================================
	SUFileUtils::GetIndPString
	
	Throws an error if index is out-of-range or resource can't be found.
==============================================*/
void SUFileUtils::GetIndPString( ResFileIDT inResFile, ResIDT inResID, SInt32 inIndex, unsigned char *outString )
{
	Boolean				wasLoaded;
	Byte				oldFlags;
	Handle				h = SUFileUtils::GetResource( inResFile, 'STR#', inResID, &wasLoaded );
	
	if ( wasLoaded )
		oldFlags = ::HGetState( h );
	
	unsigned char		*p;
	short				numStrings;
	
	::HLock( h );
	p = (unsigned char*)(*h);

	ThrowIf_( ::GetHandleSize(h) < static_cast<SInt32>(sizeof(short)) );
	numStrings = *(short*)p;

	ThrowIf_( (inIndex > numStrings) || (inIndex <= 0) );	
	p = (unsigned char*)(*h + sizeof(short));					// point to first string
	
		// step through preceeding strings
	for ( short count=1; count < inIndex; count++ )
		p += *p + 1;

		// copy the string we've found
	::BlockMoveData( p, outString, 1 + *p );
	
	if ( wasLoaded )
		::HSetState( h, oldFlags );
	else
		::ReleaseResource( h );
}

/*============================================
	SUFileUtils::GetIndString
	
	Throws an error if index is out-of-range or resource can't be found.
==============================================*/
void SUFileUtils::GetIndString( ResFileIDT inResFile, ResIDT inResID, SInt32 inIndex, char *outString )
{
	SUFileUtils::GetIndPString( inResFile, inResID, inIndex, (unsigned char*) outString );
	LString::PToCStr( (unsigned char*) outString );
}

#ifdef NOT_USED_NEW_ONE_ABOVE
/*============================================
	SUFileUtils::GetIndString
	
	Throws an error if index is out-of-range or resource can't be found.
==============================================*/
void SUFileUtils::GetIndString( ResFileIDT inResFile, ResIDT inResID, Int32 inIndex, char *outString )
{
	Boolean				wasLoaded;
	Byte				oldFlags;
	Handle				h = SUFileUtils::GetResource( inResFile, 'STR#', inResID, &wasLoaded );
	
	if ( wasLoaded )
		oldFlags = ::HGetState( h );
	
	unsigned char		*p;
	short				numStrings;
	
	::HLock( h );
	p = (unsigned char*)(*h);

	ThrowIf_( ::GetHandleSize(h) < sizeof(short) );
	numStrings = *(short*)p;

	ThrowIf_( (inIndex > numStrings) || (inIndex <= 0) );	
	p = (unsigned char*)(*h + sizeof(short));					// point to first string
	
		// step through preceeding strings
	for ( short count=1; count < inIndex; count++ )
		p += *p + 1;

		// copy the string we've found (convert from P to C)
	::BlockMoveData( p, outString, 1 + *p );
	PtoCstr( (unsigned char*) outString );
	
	DONE:
	if ( wasLoaded )
		::HSetState( h, oldFlags );
	else
		::ReleaseResource( h );
}
#endif

