// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	UCustomTextTraits.cpp		© 2001 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	This file is designed to allow for CodeWarrior preference panel developers
//	to build their UI using specific text traits without actually storing the
//	text trait resources in their resource file.  The idea behind this is that
//	the panels will use the IDE's text traits.
//
//	To use this feature, create a folder named "Text Traits" and place it next
//	to the Constructor application.  Any items inside this folder will be
//	scanned for 'Txtr' resources which Constructor will use for displaying in
//	the views.


// ===========================================================================

#include "UCustomTextTraits.h"

#include <UResourceMgr.h>

#include "UApplicationFile.h"
#include "MoreFilesExtras.h"



// You can turn on/off this file's features with this macro
#ifndef UseTxtrPrefPanelBuildingFeature
#define UseTxtrPrefPanelBuildingFeature		1
#endif




// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	DSPC_CatalogPage		= 2050;

const ResIDT	STR_TargetInfo			= 1099;
const SInt16	str_PluginsFolderName	= 3;
const SInt16	str_TxtrFolderName		= 5;


#if PRAGMA_STRUCT_ALIGN
#pragma options align=mac68k
#endif
struct FindTxtrByIDData
{
	TextTraitsH	traitsH;	// TextTraitsH
	ResIDT		resID;		// Resource ID of the 'Txtr' to load
};
#if PRAGMA_STRUCT_ALIGN
#pragma options align=reset
#endif

typedef struct FindTxtrByIDData	FindTxtrByIDData;
typedef FindTxtrByIDData*		FindTxtrByIDDataPtr;



// ---------------------------------------------------------------------------
//		* FindTextTraitsFolder									[static]
// ---------------------------------------------------------------------------
//	Get the directory information of the Text Traits folder

OSErr
UCustomTextTraits::FindTextTraitsFolder(
	SInt16*	outFoundVRefNum,
	SInt32*	outFoundDirID)
{
	OSErr	err;
	FSSpec	appFile;
	FSSpec	txtrFolder;
	SInt32	txtrFolderDirID;
	Boolean	isDir;
	LStr255 folderName(STR_TargetInfo, str_TxtrFolderName);

	UApplicationFile::GetApplicationFile(appFile);
	err = ::FSMakeFSSpec(appFile.vRefNum, appFile.parID, folderName,
		&txtrFolder);
	if (err == noErr)
	{
		err = ::FSpGetDirectoryID(&txtrFolder, &txtrFolderDirID, &isDir);
		if (err == noErr && isDir)
		{
			*outFoundVRefNum = txtrFolder.vRefNum;
			*outFoundDirID = txtrFolderDirID;
		}
	}
	
	return err;
}


// ---------------------------------------------------------------------------
//		* ScanFilesInFolder
// ---------------------------------------------------------------------------
//	Scan a folder (non-recursively) for resource files which may contain 'Txtr'
//	resources and call the passed in filter function to perform any actions
//	on the files.  Based on VETypeList::MakeSpecsFromFolder().

void
UCustomTextTraits::ScanFilesInFolder(
	SInt16				inVRefNum,
	SInt32				inDirID,
	TxtrFilterProcPtr	inTxtrFilter,
	void*				ioUserData)
{
	SInt16	index = 1;
	Boolean	quitScan = false;

	if (inTxtrFilter == nil)
	{
		return;
	}
	
	while (true)
	{
		// Set up search parameters.
	
		CInfoPBRec	fileInfo;
		Str255		fileName;
		OSErr		result;
		FSSpec		theFile;

		fileInfo.hFileInfo.ioCompletion = nil;
		fileInfo.hFileInfo.ioNamePtr = fileName;
		fileInfo.hFileInfo.ioVRefNum = inVRefNum;
		fileInfo.hFileInfo.ioFDirIndex = index++;
		fileInfo.hFileInfo.ioDirID = inDirID;

		result = ::PBGetCatInfoSync(&fileInfo);
		if (result != noErr)
			return;
		
		// Check finder information.
		if (fileInfo.hFileInfo.ioFlAttrib & ioDirMask)
			continue;					// it's a directory: ignore it
		if ((fileInfo.hFileInfo.ioFlFndrInfo.fdType != 'rsrc') &&
			(fileInfo.hFileInfo.ioFlFndrInfo.fdType != 'RSRC') &&
			(fileInfo.hFileInfo.ioFlFndrInfo.fdType != 'Rsrc'))
			continue;					// file type mismatch: ignore it
		
		// Got a file, build an FSSpec for it.
		theFile.vRefNum = inVRefNum;
		theFile.parID = inDirID;
		LString::CopyPStr(fileName, theFile.name, 63);

#if 1
		// Resolve alias if applicable.
		//* 2.2a2: BUG FIX #605: added section
		if (fileInfo.hFileInfo.ioFlFndrInfo.fdFlags & kIsAlias)
		{
			StResourceContext saveContext;
			saveContext.Save();
			{
				StResLoad load(false);			// we don't want preloaded resources
				SInt16 aliasRefNum = ::FSpOpenResFile(&theFile, fsRdPerm);
				if (aliasRefNum == -1)
					continue;					// open failed: ignore file
				
				try
				{
					StResourceContext aliasContext(aliasRefNum);
					StResLoad nowLoad(true);
					StResource aliasR('alis', (ResIDT) 0, true, true);
					Boolean changed = false;
					ThrowIfOSErr_(::ResolveAlias(nil, (AliasHandle) ((Handle) aliasR), &theFile, &changed));
				}
				catch(...)
				{
					::CloseResFile(aliasRefNum);
					continue;
				}
				::CloseResFile(aliasRefNum);
			}
		}
#endif

		// Okay, we have an acceptable file. Open it and scan it.
		StResourceContext saveContext;
		saveContext.Save();
		{
			SInt16 fileRefNum;
			
			StResLoad load(false);			// we don't want preloaded resources
			fileRefNum = ::FSpOpenResFile(&theFile, fsRdPerm);
			if (fileRefNum == -1)
				continue;					// open failed: ignore file

			try
			{
				StResourceContext fileContext(fileRefNum);
				StResLoad load(true);
				// Call our specific file filter
				CallTxtrFilterProc(inTxtrFilter, fileRefNum, &quitScan, ioUserData);
			}
			catch(...)
			{
				// ignore errors & continue
			}
			::CloseResFile(fileRefNum);
		}
		if (quitScan == true)
			return;
	}
}


// ---------------------------------------------------------------------------
//		* FindTextTraitsByID									[static]
// ---------------------------------------------------------------------------
//	Search the Text Traits folder for a specific 'Txtr' resource by ID.

TextTraitsH
UCustomTextTraits::FindTextTraitsByID(
	ResIDT		inTextTraitsID)
{
#if !UseTxtrPrefPanelBuildingFeature
	#pragma unused (inTextTraitsID)
	return nil;
#else
	SInt16		txtrFolderVRefNum;
	SInt32		txtrFolderDirID;
	FindTxtrByIDData	userData;
	
	userData.traitsH = nil;
	userData.resID = inTextTraitsID;

	if (UCustomTextTraits::FindTextTraitsFolder(&txtrFolderVRefNum, &txtrFolderDirID) == noErr)
	{
		ScanFilesInFolder(txtrFolderVRefNum, txtrFolderDirID, FindTextTraitsByIDFilter, &userData);
	}
	
	return userData.traitsH;
#endif
}


// ---------------------------------------------------------------------------
//		* FindCustomTextTraits									[static]
// ---------------------------------------------------------------------------
//	Search the Text Traits folder for aall 'Txtr' resources.  This infomation
//	will be used to fill in the PILabelPopup.

void
UCustomTextTraits::FindCustomTextTraits(
	LArray&	ioResInfo)
{
#if !UseTxtrPrefPanelBuildingFeature
	#pragma unused (ioResInfo)
	return;
#else
	SInt16		txtrFolderVRefNum;
	SInt32		txtrFolderDirID;
	
	if (UCustomTextTraits::FindTextTraitsFolder(&txtrFolderVRefNum, &txtrFolderDirID) == noErr)
	{
		ScanFilesInFolder(txtrFolderVRefNum, txtrFolderDirID, FindAllTextTraitsFilter, &ioResInfo);
	}
#endif
}


// ---------------------------------------------------------------------------
//		* FindTextTraitsByIDFilter
// ---------------------------------------------------------------------------
//	See if the current resource fork contains the 'Txtr' we're looking for.

pascal void
FindTextTraitsByIDFilter(
	const SInt16	/* inRefNum */,
	Boolean*		outQuitFlag,
	void*			ioUserData)
{
	SignalIf_(ioUserData == nil);
	
	FindTxtrByIDDataPtr	userData = (FindTxtrByIDDataPtr) ioUserData;

	userData->traitsH = (TextTraitsH) ::Get1Resource(ResType_TextTraits, userData->resID);
	if (userData->traitsH)
	{
		::DetachResource((Handle) userData->traitsH);
		*outQuitFlag = true;
	}
}


// ---------------------------------------------------------------------------
//		* FindAllTextTraitsFilter
// ---------------------------------------------------------------------------
//	Add each 'Txtr' of the current resource fork to our array.

pascal void
FindAllTextTraitsFilter(
	const SInt16	/* inRefNum */,
	Boolean*		/* outQuitFlag */,
	void*			ioUserData)
{
	SignalIf_(ioUserData == nil);

	SInt16		numTxtr;
	SInt16		index;
	Handle		txtrH;
	SInt16		txtrResID;
	Str255		txtrResName;
	ResType		txtrResType;
	LArray*		ioResInfo = (LArray*) ioUserData;
	SCustomTextTraitsResInfo	txtrResInfo;
	
	numTxtr = ::Count1Resources(ResType_TextTraits);
	for (index = 1; index <= numTxtr; index++)
	{
		txtrH = ::Get1IndResource(ResType_TextTraits, index);
		if (txtrH)
		{
			::GetResInfo(txtrH, &txtrResID, &txtrResType, txtrResName);
			::ReleaseResource(txtrH);

			txtrResInfo.mResID = txtrResID;
			LString::CopyPStr(txtrResName, txtrResInfo.mResName);
			
			ioResInfo->InsertItemsAt(1, LArray::index_Last, &txtrResInfo);
		}
	}
}
