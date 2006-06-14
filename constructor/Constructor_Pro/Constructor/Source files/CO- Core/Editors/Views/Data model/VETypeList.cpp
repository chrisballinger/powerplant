// Copyright й2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VETypeList.cpp				й 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 06/12/96
//	   $Date: 2006/01/18 01:33:13 $
//	$History: VETypeList.cpp $
//
//	// еее GHD/JCD	Date: 04/23/97
//		Search "Constructor Plugins" folder for custom CTYPs
//	
//	*****************  Version 14  *****************
//	User: scouten      QDate: 03/19/97   Time: 14:53
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/Data model
//	Eliminated dangling pointers in VETypeEntry. (Bug fix #1310.)
//	
//	*****************  Version 13  *****************
//	User: scouten      QDate: 03/18/97   Time: 15:30
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/Data model
//	Improved handling of CTYP changes.
//	
//	*****************  Version 12  *****************
//	User: scouten      QDate: 03/17/97   Time: 14:05
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/Data model
//	Added VEResIDComparator class.
//	
//	*****************  Version 11  *****************
//	User: scouten      QDate: 03/05/97   Time: 10:47
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/Data model
//	Shielded access to RF* classes behind Constructor_UseRF switch.
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 02/20/97   Time: 18:26
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/Data model
//	Improved commenting.
//	
//	*****************  Version 9  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:09p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 01/24/97   Time: 15:06
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/Data model
//	Update for the refactored RFMap classes.
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 12/08/96   Time: 11:32
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/Data model
//	Improved memory efficiency in PrepareTypeEntry by sharing prototype's
//	attributes.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 12/01/96   Time: 02:08
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/Data model
//	Fixed a bug in InstallTypeEntry. It no longer gets confused if
//	FindCatalogPage returns nil.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 11/30/96   Time: 18:32
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/Data model
//	Added support for catalog. Refactored parts of the MakeSpecFrom<xxxx>
//	routines.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/17/96   Time: 10:36
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/Data model
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/13/96   Time: 13:37
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor
//	Rearranged view editor classes.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/05/96   Time: 21:23
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor
//	Made CanDeleteThis const to avoid hiding DMElement's method.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 02:00
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "VETypeList.h"
#include <LStaticText.h>

	// Core : Data model : Attributes
#include "DMContainerAttribute.h"

	// Core : Data model : Core objects
#include "DMReanimator.h"

	// Core : Data model : Utilities
#include "DMIterator.h"

	// Core : Editors : Views : Data model
#include "VECatalogPage.h"
#include "VETypeEntry.h"

	// Core : Resource manager
//#include "StResourceContext.h"

	// Core : Utilities
#include "UApplicationFile.h"

	// MacOS : Resource manager
#if Constructor_UseRF
	#include "RFMap.h"
	#include "RFResource.h"
	#include "RFType.h"
#endif

	// PowerPlant : Utility classes
#include <UMemoryMgr.h>
#include <UResourceMgr.h>
#include <UModalDialogs.h>

	// Universal headers
#include <Finder.h>


// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	DSPC_CatalogPage		= 2050;

const ResIDT	STR_TargetInfo			= 1099;
const SInt16	str_PluginsFolderName	= 3;

const ResIDT	PPob_NoCPPbSupport		=	23901;	// in Data Model.PPob
const PaneIDT	NoCPPb_Caption			=	'CAPT';
const ResIDT	STRx_NoCPPbSupport		=	23901;
const SInt16	NoCPPb_StringFirst		=	1;
const SInt16	NoCPPb_StringSecond		=	2;


// ===========================================================================
//		* Class variables
// ===========================================================================

VETypeListPtr VETypeList::sTypeList;


// ===========================================================================

#pragma mark *** VETypeList ***

// ---------------------------------------------------------------------------
//		* VETypeList(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor

VETypeList::VETypeList(
	LStream*	inStream)

: DMObject(inStream)

{

	// Reset variables.
	
	mTypesAreBuiltIn = false;
	
	// Read stream data.
	
	(*inStream)	>> mCatalogPageSpecID
				>> mClassIDsSignificant;
	
}


// ---------------------------------------------------------------------------
//		* VETypeList(VETypeList&)
// ---------------------------------------------------------------------------
//	Copy constructor

VETypeList::VETypeList(
	const VETypeList&	inOriginal)

: DMObject(inOriginal)

{
	mTypesAreBuiltIn = false;
	mClassIDsSignificant = inOriginal.mClassIDsSignificant;
	mCatalogPageSpecID = inOriginal.mCatalogPageSpecID;
}


// ---------------------------------------------------------------------------
//		* ~VETypeList
// ---------------------------------------------------------------------------
//	Destructor

VETypeList::~VETypeList()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** access control

// ---------------------------------------------------------------------------
//		* CanDeleteThis
// ---------------------------------------------------------------------------
//	Overriden to always disallow deletion of the type list.

Boolean
VETypeList::CanDeleteThis() const
{
	return false;
}


// ===========================================================================

#pragma mark -
#pragma mark ** types list lookup

// ---------------------------------------------------------------------------
//		* FindTypeByID
// ---------------------------------------------------------------------------
//	Given a class ID (which may be assigned by either the framework or
//	by Constructor, if none exists), find the type description.

VETypeEntry*
VETypeList::FindTypeByID(
	ClassIDT	inClassID) const
{

	// Validate pointers.
	
	ValidateThis_();

	// Find resource type list attribute.

	DMContainerAttribute* attr = GetClassList();
	ValidateObject_(attr);
	
	// Search for the named type.
	
	DMFastIterator iter(attr->GetSubElements());
	while (iter.NextElement()) {
		VETypeEntry* theType = (dynamic_cast<VETypeEntry*>(iter.CurrentElement()));
		ValidateObject_(theType);
		if (theType->GetClassID() == inClassID)
			return theType;
	}
	
	// Couldn't find it.
	
	return nil;

}


// ---------------------------------------------------------------------------
//		* FindTypeByName
// ---------------------------------------------------------------------------
//	Given a class name, find the type description. Slower than FindTypeByID.

VETypeEntry*
VETypeList::FindTypeByName(
	ConstStringPtr	inClassName) const
{

	// Validate pointers.
	
	ValidateThis_();

	// Find resource type list attribute.

	DMContainerAttribute* attr = GetClassList();
	ValidateObject_(attr);
	
	// Search for the named type.
	
	DMFastIterator iter(attr->GetSubElements());
	while (iter.NextElement()) {
		VETypeEntry* theType = (dynamic_cast<VETypeEntry*>(iter.CurrentElement()));
		ValidateObject_(theType);
		LStr255 name;
		theType->GetClassName(name);
		if (name.CompareTo(inClassName) == 0)
			return theType;
	}

	// Didn't find it.
	
	return nil;

}


// ===========================================================================

#pragma mark -
#pragma mark ** resource file scanning

// ---------------------------------------------------------------------------
//		* ScanAtLaunch
// ---------------------------------------------------------------------------
//	Called when the types list is first being built. Scans Constructor's
//	resource fork for descriptions of built-in display classes.

void
VETypeList::ScanAtLaunch()
{

	// Validate pointers.
	
	ValidateThis_();

	// Find application file.
	
	FSSpec appFile;
	UApplicationFile::GetApplicationFile(appFile);

	// Read from application file.
	
	{
		StApplicationContext appContext;
		StValueChanger<Boolean> builtIn(mTypesAreBuiltIn, true);
		ScanCurrentResourceFile(appFile);
	}

	// Read application folder.
	
	try {
		ScanFolderForPlugIns(appFile.vRefNum, appFile.parID);
	}
	catch(...) {
		// ignore errors
	}

	// Read "Constructor Plugins" folder in same folder as application
	
	// find the folder
	
	CInfoPBRec folderInfo;
	LStr255 folderName(STR_TargetInfo, str_PluginsFolderName);
	
	folderInfo.hFileInfo.ioCompletion = nil;
	folderInfo.hFileInfo.ioNamePtr = (StringPtr) folderName;
	folderInfo.hFileInfo.ioVRefNum = appFile.vRefNum;
	folderInfo.hFileInfo.ioFDirIndex = 0;
	folderInfo.hFileInfo.ioDirID = appFile.parID;
	
	OSErr err = ::PBGetCatInfoSync( &folderInfo );
	
	if (err == noErr) {
	
		// read it
	
		try {
			ScanFolderForPlugIns(folderInfo.hFileInfo.ioVRefNum,
									folderInfo.hFileInfo.ioDirID);
		}
		catch(...) {
			// ignore errors
		}
	}
}


// ---------------------------------------------------------------------------
//		* ScanProjectFile
// ---------------------------------------------------------------------------
//	Called when a user file is opened. Scan the file for descriptions of
//	user-defined display classes.

void
VETypeList::ScanProjectFile(
	const FSSpec&	inSourceFile,
	RFMap&			inRFMap)
{

#if Constructor_UseRF

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(&inRFMap);

	// Scan project file.
	
	ScanResourceMap(inSourceFile, inRFMap);
	inRFMap.AddListener(this);
	
	// Scan project file's subfolders.
	
//	ScanFolderForPlugIns(inSourceFile);			// then again, maybe not...

#else

	// This call is only valid for MacOS resource file-based targets.
	
	#pragma unused (inSourceFile)
	#pragma unused (inRFMap)

#endif
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** singleton accessor

// ---------------------------------------------------------------------------
//		* GetTypeList											[static]
// ---------------------------------------------------------------------------
//	Returns the singleton type list pointer. Overriden versions should
//	create

VETypeList*
VETypeList::GetTypeList()
{
	return sTypeList;
}


// ---------------------------------------------------------------------------
//		* ReleaseTypeList										[static]
// ---------------------------------------------------------------------------
//	Call when the application terminates to release the type list object.

void
VETypeList::ReleaseTypeList()
{
	sTypeList = nil;
}


// ===========================================================================

#pragma mark -
#pragma mark ** resource scanning internals

// ---------------------------------------------------------------------------
//		* ScanFolderForPlugIns									[protected]
// ---------------------------------------------------------------------------
//	Override hook. Scan a folder for plug-in files which may contain
//	class descriptions.

void
VETypeList::ScanFolderForPlugIns(
	SInt16	/* inVRefNum */,
	SInt32	/* inDirID */)
{
	// default does nothing
}


// ---------------------------------------------------------------------------
//		* MakeSpecsFromFolder									[protected]
// ---------------------------------------------------------------------------
//	Scan a folder (non-recursively) for files which may contain custom
//	types descriptions.

void
VETypeList::MakeSpecsFromFolder(
	SInt16		inVRefNum,
	SInt32		inDirID,
	OSType		inCreatorCode,
	OSType		inFileType)
{

	// Validate pointers.
	
	ValidateThis_();

	// Get a copy of application's file spec. We ignore the
	// application if it shows up in this list.
	
	FSSpec appSpec;
	UApplicationFile::GetApplicationFile(appSpec);

	// Scan folder non-recursively.
	// Based on code from IM-Files, p. 2-43.
	
	SInt16 index = 1;
	while (1) {
	
		// Set up search parameters.
	
		CInfoPBRec fileInfo;
		Str255 fileName;
		fileInfo.hFileInfo.ioCompletion = nil;
		fileInfo.hFileInfo.ioNamePtr = fileName;
		fileInfo.hFileInfo.ioVRefNum = inVRefNum;
		fileInfo.hFileInfo.ioFDirIndex = index++;
		fileInfo.hFileInfo.ioDirID = inDirID;
		
		OSErr result = ::PBGetCatInfoSync(&fileInfo);
		if (result != noErr)
			return;
		
		// Check finder information.
		
		if (fileInfo.hFileInfo.ioFlAttrib & ioDirMask)
			continue;					// it's a directory: ignore it
		if (inFileType && (fileInfo.hFileInfo.ioFlFndrInfo.fdType != inFileType))
			continue;					// file type mismatch: ignore it
		if (inCreatorCode && (fileInfo.hFileInfo.ioFlFndrInfo.fdCreator != inCreatorCode))
			continue;					// creator code mismatch: ignore it
		
		// Got a file, build an FSSpec for it.
		
		FSSpec theFile;
		theFile.vRefNum = inVRefNum;
		theFile.parID = inDirID;
		LString::CopyPStr(fileName, theFile.name, 63);
		
		// Make sure this isn't the application.

		if (::EqualString(appSpec.name, theFile.name, false, true)
		   && (appSpec.vRefNum == theFile.vRefNum)
		   && (appSpec.parID == theFile.parID))
		   	continue;						// it is the application, ignore it

		// Resolve alias if applicable.
		//* 2.2a2: BUG FIX #605: added section
		
		if (fileInfo.hFileInfo.ioFlFndrInfo.fdFlags & kIsAlias) {
			StResourceContext saveContext;
			saveContext.Save();
			{
				StResLoad load(false);			// we don't want preloaded resources
				SInt16 aliasRefNum = ::FSpOpenResFile(&theFile, fsRdPerm);
				if (aliasRefNum == -1)
					continue;					// open failed: ignore file
				
				try {
					StResourceContext aliasContext(aliasRefNum);
					StResLoad nowLoad(true);
					StResource aliasR('alis', (ResIDT) 0, true, true);
					Boolean changed = false;
					ThrowIfOSErr_(::ResolveAlias(nil, (AliasHandle) ((Handle) aliasR), &theFile, &changed));
				}
				catch(...) {
					::CloseResFile(aliasRefNum);
					continue;
				}
				::CloseResFile(aliasRefNum);
			}
		}

		// Okay, we have an acceptable file. Open it and scan it.
		
		StResourceContext saveContext;
		saveContext.Save();
		{
			StResLoad load(false);			// we don't want preloaded resources
			SInt16 fileRefNum = ::FSpOpenResFile(&theFile, fsRdPerm);
			if (fileRefNum == -1)
				continue;					// open failed: ignore file

			try {
				StResourceContext fileContext(fileRefNum);
				StResLoad load(true);
				ScanCurrentResourceFile(theFile);
			}
			catch(...) {
				// ignore errors & continue
			}
			::CloseResFile(fileRefNum);
		}
	}

}


// ---------------------------------------------------------------------------
//		* MakeSpecsFromFile										[protected]
// ---------------------------------------------------------------------------
//	Scan a resource file for resources of a given type. Make type
//	specifications for the classes described by those resources.
//	Allows caller to specify that all resources outside of a given
//	ID range should be ignored. (This is typically used when scanning
//	Constructor's own resource fork.)
//	
//	Typically called by the subclass' ScanCurrentResourceFile method.
//	Calls the subclass' MakeTypeSpec method.

void
VETypeList::MakeSpecsFromFile(
	ResType			inResType,
	const FSSpec&	inSourceFile,
	ResIDT			inMinimumID,
	ResIDT			inMaximumID)
{

	// Validate pointers.
	
	ValidateThis_();

	// Get all the resources of this type.
	
	LArray resList(sizeof (Handle));
	SInt32 numResources = ::Count1Resources(inResType);
	
	// JCD 980612
	// The old custom type resource ('CPPb') is still supported
	// today, but it's been long obsolete. Support was kept in to
	// allow for a transition period to the new 'CTYP' format. But
	// it's now been long enough, everyone should have converted
	// by now.
	//
	// Furthermore, the existance of both a CPPb and CTYP for the
	// same object confuses Constructor. Removing 'CPPb' support
	// cleans this up. This fixes bug MW02546.
	//
	// We perform the check here (instead of in VPTypeList's
	// ScanCurrentResourceFile or ScanResourceMap, which call this)
	// because we still wish to initiate a scan for 'CPPb's...we will
	// present a dialog, if any are found, saying 'CPPb's are no
	// longer supported, then return.
	
	if ( (inResType == 'CPPb') && (numResources > 0) ) {
	
		DoNoCPPbSupportDialog(inSourceFile.name);
		
		return;
	}
	
	
	
	Handle theResource;
	
	for (SInt32 index = 1; index <= numResources; index++) {

		// Get the next indexed resource.

		theResource = ::Get1IndResource(inResType, index);
		ValidateHandle_(theResource);

		// Check its ID against the min/max bounds.

		Str255 name;
		ResIDT theID;
		ResType theType;
		::GetResInfo(theResource, &theID, &theType, name);
		if ((theID < inMinimumID) || (theID > inMaximumID))
			continue;
	
		// Bounds check is OK. Add it to the list
		// and process it later.
	
		resList.InsertItemsAt(1, 0, &theResource);
		::LoadResource(theResource);
		::HNoPurge(theResource);

	}
	
	// Sort the resources by resource ID.

	resList.SetComparator(new VEResIDComparator);
	resList.Sort();
	resList.SetComparator(new LLongComparator);
	
	// Load and detach resources.

	LArrayIterator iter(resList);
	while (iter.Next(&theResource)) {
		::DetachResource(theResource);
	}

	// Now that we have all the resource, create specifications for them.
	// Keep iterating over the list until we've either handled all
	// resources, or we can't make any further progress.

	Boolean progress;
	do {
		progress = false;
		
		iter.ResetTo(LArrayIterator::from_Start);
		while (iter.Next(&theResource)) {
			ValidateHandle_(theResource);
			LHandleStream resDataStream(theResource);
			try {
				if (MakeTypeSpec(resDataStream, inResType, inSourceFile)) {
					resList.Remove(&theResource);
					progress = true;
					continue;			// skip the detach handle
				}
			}
			catch(...) {
				// ignore errors
			}
			resDataStream.DetachDataHandle();
		}
	} while (progress && (resList.GetCount() > 0));

	// Release forgotten resources.
	
	iter.ResetTo(LArrayIterator::from_Start);
	while (iter.Next(&theResource)) {
		::DisposeHandle(theResource);
	}

}


// ---------------------------------------------------------------------------
//		* MakeSpecsFromMap										[protected]
// ---------------------------------------------------------------------------
//	Scan a resource map for resources of a given type. Make type
//	specifications for the classes described by those resources.
//	Typically called by the subclass' ScanResourceMap method.
//	Calls the subclass' MakeTypeSpec method.

void
VETypeList::MakeSpecsFromMap(
	ResType			inResType,
	const FSSpec&	inSourceFile,
	RFMap&			inRFMap)
{

#if Constructor_UseRF

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(&inRFMap);

	// Get all the resources.
	
	LArray resList(sizeof (void*));
	RFType* theRFType = inRFMap.FindResType(inResType, false);
	if (!theRFType)
		return;
	ValidateSimpleObject_(theRFType);

	LArrayIterator resListIter(theRFType->GetResources(), LArrayIterator::from_Start);
	RFResource* theResource;
	while (resListIter.Next(&theResource)) {
		ValidateSimpleObject_(theResource);
		resList.InsertItemsAt(1, 0, &theResource);
	}

	// Now that they're in order, create specifications for them.

	Boolean progress;
	do {
		progress = false;						//* 2.2d9: BUG FIX #550: ensure loop terminates
	
		LArrayIterator iter(resList, LArrayIterator::from_Start);
		while (iter.Next(&theResource)) {
			ValidateSimpleObject_(theResource);
			LHandleStream resDataStream(theResource->GetResData());
			try {
				if (MakeTypeSpec(resDataStream, inResType, inSourceFile)) {
					resList.Remove(&theResource);
					progress = true;
				}
			}
			catch(...) {
				// ignore errors
			}
		}
	} while (progress && (resList.GetCount() > 0));

#else

	// This call is only valid for MacOS resource file-based targets.

	#pragma unused (inResType)
	#pragma unused (inSourceFile)
	#pragma unused (inRFMap)

#endif

}


// ---------------------------------------------------------------------------
//		* DoNoCPPbSupportDialog				[static, public]
// ---------------------------------------------------------------------------
//	Displays a dialog saying that we no longer support the 'CPPb' resource
//	type for custom type specifications.

void
VETypeList::DoNoCPPbSupportDialog(
	ConstStringPtr	inFileName )
{
	StApplicationContext	appContext;
	StDialogHandler			dialog(PPob_NoCPPbSupport, LCommander::GetTopCommander());
	
	// make the error string
	
	LStr255 errorStr(STRx_NoCPPbSupport, NoCPPb_StringFirst);
	if ( inFileName[0] > 0 ) {
		errorStr += inFileName;
	}
	errorStr += LStr255(STRx_NoCPPbSupport, NoCPPb_StringSecond);
	
	LStaticText *theCaption = dynamic_cast<LStaticText *> (dialog.GetDialog()->FindPaneByID(NoCPPb_Caption));
	theCaption->SetDescriptor(errorStr);
			
	dialog.GetDialog()->Show();
	
	MessageT theMessage;
	do {
		theMessage = dialog.DoDialog();
	} while ( (theMessage != msg_OK) && (theMessage != msg_Cancel) );

	LCommander::SetUpdateCommandStatus(true);
}


// ===========================================================================

#pragma mark -
#pragma mark ** type specification builders

// ---------------------------------------------------------------------------
//		* MakeSpecFromCTYP										[protected]
// ---------------------------------------------------------------------------
//	Make a type description from a CTYP resource.

Boolean
VETypeList::MakeSpecFromCTYP(
	LStream&		inStream,
	const FSSpec&	/* inSourceFile */)
{

	// Validate pointers.
	
	ValidateThis_();

	// Use the data model reanimator to create the type spec.

	DMReanimator* reanimator = DMReanimator::GetReanimator();
	ValidateObject_(reanimator);

	DMElementPtr tempContainer;
	reanimator->ElementsFromStream(&inStream, tempContainer);
	ValidateObject_(tempContainer.GetObject());

	VETypeEntryPtr thisEntry;
	thisEntry = (dynamic_cast<VETypeEntry*>(tempContainer.GetObject()));
	ValidateObject_(thisEntry.GetObject());

	// Place this class in the type list hierarchy.
	
	VETypeEntry* previousEntry = nil;
	VETypeEntry* dataModelParent = nil;
	VETypeEntry* runtimeParent = nil;

	PrepareTypeEntry(thisEntry, dataModelParent, runtimeParent, previousEntry);

	// Move attributes from subitem list into prototype.
	// Make a container attribute to reflect this class name.
	
	DMContainerAttributePtr container;
	container = MakeContainerAttribute(thisEntry);
	ValidateObject_(container.GetObject());
	
	// Move each subelement from type entry object to
	// the new container we just created.
	
	DMIterator iter(const_cast<LSharableArray&>(thisEntry->GetSubElements()));
	while (iter.NextElement()) {
	
		// Use the DMAttributePtr to ensure that attribute
		// stays around while we're moving it.
	
		DMAttributePtr attr = (dynamic_cast<DMAttribute*>(iter.CurrentElement()));
		ValidateObject_(attr.GetObject());
		
		// If this is the catalog subclasses entry, leave it.
		
		if (attr->GetKey() == VETypeEntry::attrKey_SubClasses)
			continue;
		
		// Remove it from the type entry's attribute list.
		
		thisEntry->RemoveElement(attr);
		
		// Add it to the prototype's attribute list.
		
		container->InsertElementAt(LArray::index_Last, attr, true);
		
	}
	
	// Put this into the type list.

	InstallTypeEntry(thisEntry, runtimeParent, previousEntry);	

	// Signal a successful build.

	return true;

}


// ---------------------------------------------------------------------------
//		* MakeContainerAttribute								[protected]
// ---------------------------------------------------------------------------
//	Make a container attribute which will hold the attributes specific
//	to this class.

DMContainerAttribute*
VETypeList::MakeContainerAttribute(
	VETypeEntry*	inEntry)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inEntry);

	// Find the prototype object. If no prototype, something's hosed.
	
	DMObject* prototype = inEntry->mPrototype;
	ValidateObject_(prototype);

	// Make class name for attribute label.
	
	LStr255 attrTitle;
	inEntry->GetShortClassName(attrTitle);				//* J-1.0a3: BUG FIX #918: changed to ShortClassName
	if (attrTitle.Length() == 0)
		attrTitle = (FourCharCode) inEntry->GetClassID();

	// Make an attribute object.

	DMContainerAttribute* attr = new DMContainerAttribute;
	ValidateObject_(attr);
	attr->SetTitle(attrTitle);
	attr->SetKey(inEntry->GetClassID());

	// Put it in the prototype.

	prototype->InsertElementAt(LArray::index_Last, attr, true);
	return attr;

}


// ---------------------------------------------------------------------------
//		* FixRuntimeParent										[protected]
// ---------------------------------------------------------------------------
//	Override hook. If there are cases where the runtime superclass may
//	be different than the data model superclass, this method can be used
//	to make that switch.

void
VETypeList::FixRuntimeParent(
	ClassIDT		/* inClassID */,
	ClassIDT&		/* ioRuntimeParent */)
{
	// override hook
}


// ---------------------------------------------------------------------------
//		* ChooseDrawingAgent									[protected]
// ---------------------------------------------------------------------------
//	Should be overriden. Return a pointer to the function which will create
//	a drawing agent for objects of a given class.

VE_DrawAgentCreatorFunc
VETypeList::ChooseDrawingAgent(
	ClassIDT	/* inClassID */)
{
	return nil;
}


// ---------------------------------------------------------------------------
//		* PrepareTypeEntry										[protected]
// ---------------------------------------------------------------------------
//	Common code for use in MakeSpecFrom<xxxx>. Finds data model and runtime
//	parent type entries for the new type entry. Performs sanity checks to
//	make sure this entry doesn't replace a built-in class.

void
VETypeList::PrepareTypeEntry(
	VETypeEntry*	inNewTypeEntry,
	VETypeEntry*&	inDataModelParent,
	VETypeEntry*&	inRuntimeParent,
	VETypeEntry*&	inPreviousEntry)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inNewTypeEntry);

	// Make sure the class ID is valid.

	if (inNewTypeEntry->GetClassID() == 0)
		Throw_(err_CantMakeTypeSpec);
	
	// See if this class already exists. Can't replace built-in or locked types.
	
	inPreviousEntry = FindTypeByID(inNewTypeEntry->GetClassID());
	if (inPreviousEntry != nil) {
		ValidateObject_(inPreviousEntry);
		if (inPreviousEntry->IsBuiltIn())
			Throw_(err_CantMakeTypeSpec);
	}
	
	// See if we can find parent class. There are two parent classes for each
	// class, a runtime parent (i.e. the class hierarchy that exists at runtime)
	// and a data model parent. The data model parent is usually the same as
	// the runtime parent, but may be different if there is a need for a
	// different property inspector window.

	ClassIDT dataModelParentID = inNewTypeEntry->GetParentClassID();
	ClassIDT runtimeParentID = dataModelParentID;

	FixRuntimeParent(inNewTypeEntry->GetClassID(), runtimeParentID);

	inDataModelParent = nil;
	inRuntimeParent = nil;

	if ((dataModelParentID != 0) && (dataModelParentID != 0x3f3f3f3f)) {
		inDataModelParent = FindTypeByID(dataModelParentID);
		if (inDataModelParent == nil)
			Throw_(err_CantMakeTypeSpec);
		inDataModelParent->AddListener(inNewTypeEntry);
	}

	if ((runtimeParentID != 0) && (runtimeParentID != 0x3f3f3f3f)) {
		inRuntimeParent = FindTypeByID(runtimeParentID);
		if (inRuntimeParent == nil)
			Throw_(err_CantMakeTypeSpec);
		inRuntimeParent->AddListener(inNewTypeEntry);
	}

	inNewTypeEntry->mDataSuperClass = inDataModelParent;
	inNewTypeEntry->mRuntimeSuperClass = inRuntimeParent;
	
	// Create a prototype for this object.
	
	VEModelObjectPtr prototype;
	prototype = MakeModelObjectForClass(inNewTypeEntry->GetClassID());
	
	// Copy attributes from parent class.
	
	if (inDataModelParent != nil) {
	
		// If there is no class-specific prototype for this class,
		// we can borrow the parent class' prototype.
	
		if (prototype == nil) 
			prototype = (dynamic_cast<VEModelObject*>(inDataModelParent->mPrototype->ShallowClone()));

		// Copy attributes over. Since the attribute values don't change from parent
		// class to subclass, we can share the attribute objects between them.
		
		VEModelObject* parentProto = inDataModelParent->mPrototype;
		ValidateObject_(parentProto);
		
		DMFastIterator parentIter(parentProto->GetAttributes());
		while (parentIter.NextElement()) {
			
			DMElement* parentAttr = parentIter.CurrentElement();
			ValidateObject_(parentAttr);
			
//			DMElementPtr cloneAttr = parentAttr->Clone();
//			ValidateObject_(cloneAttr.GetObject());
			
			prototype->InsertElementAt(LArray::index_Last, parentAttr /* cloneAttr */, true);
		}
	}
	
	// If there's still no prototype, this type entry will be bogus.

	if (prototype == nil)
		Throw_(err_CantMakeTypeSpec);
	ValidateObject_(prototype.GetObject());

	// Set basic information for class.

	inNewTypeEntry->mPrototype = prototype;
	inNewTypeEntry->mIsBuiltIn = mTypesAreBuiltIn;

	if (inRuntimeParent != nil) {
		ValidateObject_(inRuntimeParent);
		if (inRuntimeParent->mMustBeRoot)
			inNewTypeEntry->mMustBeRoot = true;
	}
}


// ---------------------------------------------------------------------------
//		* InstallTypeEntry										[protected]
// ---------------------------------------------------------------------------
//	Common code for use in MakeSpecFrom<xxxx>. Should be called once the
//	new type entry is complete. Adds the type entry into the type list and
//	catalog in the appropriate locations.

void
VETypeList::InstallTypeEntry(
	VETypeEntry*	inNewTypeEntry,
	VETypeEntry*	inRuntimeParent,
	VETypeEntry*	inPreviousEntry)
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inNewTypeEntry);

	// Make sure this class has a reasonable default size.

	VETypeEntry* dataModelParent = inNewTypeEntry->mDataSuperClass;
	
	if (dataModelParent != nil) {
		ValidateObject_(dataModelParent);
		if (inNewTypeEntry->mDragWidth == 0)
			inNewTypeEntry->mDragWidth = dataModelParent->mDragWidth;
		if (inNewTypeEntry->mDragHeight == 0)
			inNewTypeEntry->mDragHeight = dataModelParent->mDragHeight;
	}
	else {
		if (inNewTypeEntry->mDragWidth == 0)
			inNewTypeEntry->mDragWidth = 75;
		if (inNewTypeEntry->mDragHeight == 0)
			inNewTypeEntry->mDragHeight = 75;
	}

	// Choose drawing agent for class.
	
	inNewTypeEntry->mDrawingAgentCreatorFunc = ChooseDrawingAgent(inNewTypeEntry->GetClassID());

	// Enter it into the class list.

	DMContainerAttribute* classList = GetClassList();
	ValidateObject_(classList);
	classList->InsertElementAt(LArray::index_Last, inNewTypeEntry, true);

	// Find the catalog page where this element belongs.

	DMContainerAttribute* container = FindCatalogPage(inNewTypeEntry, inRuntimeParent);
	if (container != nil) {
		ValidateObject_(container);
		container->InsertElementAt(LArray::index_Last, inNewTypeEntry, false);
	}

	// Remove previous entry, if any.
	
	if (inPreviousEntry != nil) {
	
		// Create a forwarding link.
		
		ValidateObject_(inPreviousEntry);
		inPreviousEntry->SetReplacementEntry(inNewTypeEntry);
		
		// Sanity check: Make sure previous type wasn't built in.
		
		if (inPreviousEntry->IsBuiltIn())
			SignalCStr_("Replacing built-in class");

		// Remove previous entry from the type list and catalog page.
		// For now we assume that type entries won't migrate from one page
		// to another. This is a risky assumption.
				
		if (container != nil)
			container->RemoveElement(inPreviousEntry);
		classList->RemoveElement(inPreviousEntry);
		
	}
}


// ---------------------------------------------------------------------------
//		* FindCatalogPage										[protected]
// ---------------------------------------------------------------------------
//	Called by InstallTypeEntry. Choose the catalog page which will host
//	the new type entry.

DMContainerAttribute*
VETypeList::FindCatalogPage(
	VETypeEntry*	inNewTypeEntry,
	VETypeEntry*	inRuntimeParent)
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inNewTypeEntry);

	// See if this class anchors a catalog page.
	
	DMFastIterator iter(GetCatalogPages()->GetSubElements());
	while (iter.NextElement()) {
	
		VECatalogPage* page = (dynamic_cast<VECatalogPage*>(iter.CurrentElement()));
		ValidateObject_(page);
		
		if (page->GetPageClassID() == inNewTypeEntry->GetClassID())
			return page->GetSubClasses();
	
	}
	
	// It's not an anchor class, place it under its superclass.
	
	if (inRuntimeParent != nil) {
		ValidateObject_(inRuntimeParent);
		return inRuntimeParent->GetSubClasses();
	}
	
	// No runtime parent class, let's hope there's a "miscellaneous" page.
	
	iter.ResetTo(LArrayIterator::from_Start);
	while (iter.NextElement()) {
		
		VECatalogPage* page = (dynamic_cast<VECatalogPage*>(iter.CurrentElement()));
		ValidateObject_(page);
		
		switch (page->GetPageClassID()) {
		
			case '****':
			case '\?\?\?\?':
			case 0:
				return page->GetSubClasses();
			
		}
	}
	
	// No catalog page. This is a Bad Thing.
	
	SignalCStr_("Couldn't find catalog page.");
	return nil;
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** resource change messages

// ---------------------------------------------------------------------------
//		* ListenToMessage										[protected]
// ---------------------------------------------------------------------------
//	Listen for changes to a resource map. Update the type list as
//	appropriate.

void
VETypeList::ListenToMessage(
	MessageT	inMessage,
	void*		ioParam)
{

#if Constructor_UseRF

	// Validate pointers.
	
	ValidateThis_();
	
	// Listen for resource map change messages.

	switch (inMessage) {
		case Resource_Added:
			HandleResourceAdded((RMMap_ChangeMessage*) ioParam);
			break;
		case Resource_DataChanged:
			HandleResourceDataChanged((RMMap_ChangeMessage*) ioParam);
			break;
	}

#else
	
	// We don't care about resource changed messages from Win32.
	
	#pragma unused (inMessage)
	#pragma unused (ioParam)
	
#endif

}


// ---------------------------------------------------------------------------
//		* HandleResourceAdded									[protected]
// ---------------------------------------------------------------------------
//	Override hook. A resource was added to the resource map. If it contains
//	a custom type description, add it to the type list.

void
VETypeList::HandleResourceAdded(
	const RMMap_ChangeMessage*	/* inMessage */)
{
	// override hook
}


// ---------------------------------------------------------------------------
//		* HandleResourceDataChanged								[protected]
// ---------------------------------------------------------------------------
//	Override hook. An existing resource's data was changed. If it contains
//	a custom type description, add it to the type list.

void
VETypeList::HandleResourceDataChanged(
	const RMMap_ChangeMessage*	/* inMessage */)
{
	// override hook
}


// ===========================================================================

#pragma mark -
#pragma mark ** initialization

// ---------------------------------------------------------------------------
//		* FinishCreateSelf										[protected]
// ---------------------------------------------------------------------------
//	Build list of catalog pages.

void
VETypeList::FinishCreateSelf()
{

	// Validate pointers.

	ValidateThis_();

	// Read the prototype catalog page.
	
	DMReanimator* reanimator = DMReanimator::GetReanimator();
	ValidateObject_(reanimator);

	DMElementPtr catProto;
	reanimator->ElementsFromResource('DSPC', DSPC_CatalogPage, catProto);
	ValidateObject_(catProto.GetObject());
	
	// Read catalog page spec resource.
	
	StApplicationContext appContext;
	StResLoad loadEmUp(true);
	
	Handle vecpRes = ::Get1Resource('VECP', mCatalogPageSpecID);
	ValidateHandle_(vecpRes);
	::HNoPurge(vecpRes);
	::DetachResource(vecpRes);

	LHandleStream vecpStream(vecpRes);

	SInt16 numPages;
	vecpStream >> numPages;
	
	// Read page specifications.
	
	while (numPages--) {

		// Read the parent class ID and page name.
		
		ClassIDT type;
		Str255 pageName;
		SInt16 pageIconID;
		vecpStream >> type >> pageIconID >> pageName;

		// Build a data model for this catalog page.

		DMElementPtr newCatPage;
		newCatPage = catProto->Clone();
		ValidateObject_(newCatPage.GetObject());
		
		VECatalogPage* newCatVE = (dynamic_cast<VECatalogPage*>(newCatPage.GetObject()));
		ValidateObject_(newCatVE);
		
		newCatVE->SetPageClassID(type);
		newCatVE->SetPageTitle(pageName);
		newCatVE->SetPageIconID(pageIconID);
		
		// Add this catalog page to our list of pages.
		
		GetCatalogPages()->InsertElementAt(LArray::index_Last, newCatVE, true);
	
	}
}


// ===========================================================================

#pragma mark -
#pragma mark *** VEResIDComparator ***

// ---------------------------------------------------------------------------
//		* Compare
// ---------------------------------------------------------------------------
//	Compare two resource handles by resource ID.

SInt32
VEResIDComparator::Compare(
	const void*		inItemOne,
	const void* 	inItemTwo,
	UInt32			/* inSizeOne */,
	UInt32			/* inSizeTwo */) const
{

	// Cast the items to resource handles.
	
	Handle h1 = *(Handle*) inItemOne;
	Handle h2 = *(Handle*) inItemTwo;

	if ((h1 == nil) || (h2 == nil))
		return 0;

	// Get res info on each handle.

	Str255 name;
	ResIDT id1, id2;
	ResType theType;

	::GetResInfo(h1, &id1, &theType, name);
	::GetResInfo(h2, &id2, &theType, name);

	// Return the difference between handles.
	
	return id1 - id2;

}