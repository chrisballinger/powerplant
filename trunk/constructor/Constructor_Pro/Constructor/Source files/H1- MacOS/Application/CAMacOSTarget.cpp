// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	CAMacOSTarget.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 09/26/96
//     $Date: 2006/04/12 08:48:30 $
//	$History: CAMacOSTarget.cpp $
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 03/10/97   Time: 16:19
//	Updated in $/Constructor/Source files/H1- MacOS/Application
//	Updated to use PDDocument instead of RSDocument.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 02/04/97   Time: 19:05
//	Updated in $/Constructor/Source files/CO- Core/Application
//	Finished adaptation to refactored RM/RF classes.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:18
//	Updated in $/Constructor/Source files/CO- Core/Application
//	Fixed CR/LF problem
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 12/05/96   Time: 19:13
//	Updated in $/Constructor/Source files/Application
//	Deleted RememberOpenWindowsSelf, which wasn't doing anything anyway.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/24/96   Time: 23:05
//	Updated in $/Constructor/Source files/Application
//	Removed alignment palette code from RememberOpenWindowsSelf.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/13/96   Time: 10:37
//	Updated in $/Constructor/Source files/Application
//	Moved OpenDisplayClassWindow from target-specific application objects
//	to CAGenericTarget.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:28
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#include "CAMacOSTarget.h"

#include "Constructor.menu.h"

	// MacOS : MacOS project
#include "PRDocument.h"

	// MacOS : Editors : Views : Type list
#include "VPTypeList.h"

#include "CAPreferencesFile.h"
#include "FullPath.h"

// ===========================================================================
//		* Constants
// ===========================================================================

const CommandT	cmd_RecentItems				= 'RItm';
const ResIDT	kRecentItemsSubMenu			= 200;
const ResType	kRecentItemsPrefsType		= 'RItm';
const ResIDT	kRecentItemsPrefsID			= 1000;
const SInt16	kMaxNumberOfRecentItems		= 10;


TArray<SRecentItem>	CAMacOSTarget::sRecentItems(sizeof(SRecentItem), NULL, true);


// ===========================================================================

#pragma mark *** CAMacOSTarget ***

// ---------------------------------------------------------------------------
//		* CAMacOSTarget()
// ---------------------------------------------------------------------------
//	Application constructor for MacOS target

CAMacOSTarget::CAMacOSTarget()
{
}


// ---------------------------------------------------------------------------
//		* ~CAMacOSTarget
// ---------------------------------------------------------------------------
//	Destructor

CAMacOSTarget::~CAMacOSTarget()
{
	SaveRecentItems();

#if __PowerPlant__ < 0x02114003
	// icw -- PP 2.1.1a3 no longer requires LDropArea::RemoveHandlers() for cleanup
	
	// icw -- Remove drag handlers so we don't leak the memory
	if (LDropArea::DragAndDropIsPresent())
	{
		LDropArea::RemoveHandlers();
	}
#endif

	SRecentItemComparator* comparator = SRecentItemComparator::GetComparator();
	delete comparator;
}


// ---------------------------------------------------------------------------
//		* Initialize
// ---------------------------------------------------------------------------

void
CAMacOSTarget::Initialize()
{
	CAGenericTarget::Initialize();
	
	LoadRecentItems();
	
	UpdateRecentItemsMenu();
}

// ===========================================================================

// ---------------------------------------------------------------------------
//		* FindCommandStatus
// ---------------------------------------------------------------------------
//	Set default menu titles for menu items that can change dynamically.

void
CAMacOSTarget::FindCommandStatus(
	CommandT	inCommand,
	Boolean&	outEnabled,
	Boolean&	outUsesMark,
	UInt16&		outMark,
	Str255		outName)
{
	ResIDT	menuID;
	SInt16	menuItem;

	if (LCommander::IsSyntheticCommand(inCommand, menuID, menuItem))
	{
		if (menuID == kRecentItemsSubMenu)
		{
			outEnabled = true;
			return;
		}
	}

	// Dispatch command.

	switch (inCommand)
	{
		case cmd_RecentItems:
		{
			outEnabled = (sRecentItems.GetCount() > 0);
			break;
		}

		default:
			CAGenericTarget::FindCommandStatus(inCommand, outEnabled, outUsesMark, outMark, outName);
			break;
	}
}


// ---------------------------------------------------------------------------
//		* ObeyCommand
// ---------------------------------------------------------------------------
//	Respond to requests for global palettes only.

Boolean
CAMacOSTarget::ObeyCommand(
	CommandT	inCommand,
	void*		ioParam)
{
	ResIDT	menuID;
	SInt16	menuItem;

	if (LCommander::IsSyntheticCommand(inCommand, menuID, menuItem))
	{
		if (menuID == kRecentItemsSubMenu)
		{
			SRecentItem	recentItem;
			
			if (sRecentItems.FetchItemAt(menuItem, recentItem))
			{
				FInfo	info;
				if (::FSpGetFInfo(&recentItem.file, &info) == noErr)
					SendAEOpenDoc(recentItem.file);
				else
					::SysBeep(1);
			}
			return true;
		}
	}

	switch (inCommand)
	{
		case cmd_AddFileToRecentItems:
		{
			const FSSpec* file = static_cast<const FSSpec*>(ioParam);
			if (file != NULL)
				AddFileToRecentItems(file);
			return true;
			break;
		}
		
		default:
			return CAGenericTarget::ObeyCommand(inCommand, ioParam);
	}
}

#pragma mark -
#pragma mark ** project file behaviors

// ---------------------------------------------------------------------------
//		* MakeEmptyProject										[protected]
// ---------------------------------------------------------------------------
//	Create a new untitled MacOS project document.

PDDocument*
CAMacOSTarget::MakeEmptyProject()
{
	PRDocument* doc = new PRDocument(this);
	ValidateObject_(doc);
	
	doc->FinishCreate(nil);
	
	return doc;
}


// ---------------------------------------------------------------------------
//		* MakeProjectFromFile									[protected]
// ---------------------------------------------------------------------------
//	Create a MacOS project document from an existing resource file.
	
PDDocument*
CAMacOSTarget::MakeProjectFromFile(
	const FSSpec&	inFileSpec)
{
	PRDocument* doc = new PRDocument(this);
	ValidateObject_(doc);
	
	doc->FinishCreate(&inFileSpec);

	AddFileToRecentItems(&inFileSpec);

	return doc;
}


// ---------------------------------------------------------------------------
//		* ChooseDocument										[protected]
// ---------------------------------------------------------------------------
//	Choose a file to edit.

void
CAMacOSTarget::ChooseDocument()
{
	DoStdFileOpenDialog();
}

#pragma mark -
#pragma mark ** recent menu items


// ---------------------------------------------------------------------------
//		* LoadRecentItems									[protected]
// ---------------------------------------------------------------------------

void
CAMacOSTarget::LoadRecentItems()
{
	OSStatus	status;
	Boolean		wasChanged;
	
	sRecentItems.SetComparator(SRecentItemComparator::GetComparator(), false);
	sRecentItems.RemoveItemsAt(sRecentItems.GetCount(), LArray::index_First);

	StPreferencesContext prefsContext;
	if (prefsContext.IsValid())
	{
		StResource	recentItemsR(kRecentItemsPrefsType, kRecentItemsPrefsID, false, true);
		if (recentItemsR.IsValid())
		{
			try
			{
				LHandleStream	recentItemsStream((Handle) recentItemsR);
				while (recentItemsStream.GetMarker() < recentItemsStream.GetLength())
				{
					SRecentItem	recentItem;
					SInt32		bytesToRead;
					
					recentItemsStream >> recentItem.lastOpened;
					recentItemsStream >> bytesToRead;
					StHandleBlock	aliasH(bytesToRead, false, true);
					recentItemsStream.ReadBlock(*aliasH, bytesToRead);
					status = ::ResolveAliasWithMountFlags(NULL, (AliasHandle) aliasH.Get(),
						&recentItem.file, &wasChanged, kResolveAliasFileNoUI);
					if (status == noErr)
					{
						sRecentItems.AddItem(recentItem);
						Assert_(sRecentItems.GetCount() <= kMaxNumberOfRecentItems);
					}
				}
				recentItemsStream.DetachDataHandle();
			}
			catch (...)
			{
				sRecentItems.RemoveItemsAt(sRecentItems.GetCount(), LArray::index_First);
			}
		}
	}
}

// ---------------------------------------------------------------------------
//		* SaveRecentItems									[protected]
// ---------------------------------------------------------------------------

void
CAMacOSTarget::SaveRecentItems()
{
	OSStatus	status;
	AliasHandle	alias;

	StPreferencesContext prefsContext;
	if (prefsContext.IsValid())
	{
		StNewResource	recentItemsR(kRecentItemsPrefsType, kRecentItemsPrefsID, 0, true);
		if (recentItemsR.IsValid())
		{
			try
			{
				LHandleStream	recentItemsStream((Handle) recentItemsR);
				SRecentItem		recentItem;
				SInt32			bytesToWrite;
				
				Assert_(sRecentItems.GetCount() <= kMaxNumberOfRecentItems);

				TArrayIterator<SRecentItem>	iter(sRecentItems);
				while (iter.Next(recentItem))
				{
					status = ::NewAlias(NULL, &recentItem.file, &alias);
					if (status == noErr and alias != NULL)
					{
						StHandleLocker	lock((Handle) alias);
						recentItemsStream << recentItem.lastOpened;
						bytesToWrite = ::GetHandleSize((Handle) alias);
						recentItemsStream << bytesToWrite;
						recentItemsStream.WriteBlock(*alias, bytesToWrite);
						lock.Release();
						::DisposeHandle((Handle) alias);
					}
				}
				recentItemsStream.SetLength(recentItemsStream.GetMarker());
				recentItemsStream.DetachDataHandle();
				recentItemsR.Write();
			}
			catch (...)
			{
			}
		}
	}
}


// ---------------------------------------------------------------------------
//		* AddFileToRecentItems										[protected]
// ---------------------------------------------------------------------------

void
CAMacOSTarget::AddFileToRecentItems(
	const FSSpec*	inFile)
{
	SRecentItem	recentItem;
	bool		fileInList = false;
	
	// Check if file is in the recent items list
	TArrayIterator<SRecentItem>	iter(sRecentItems);
	while (iter.Next(recentItem))
	{
		if (LFile::EqualFileSpec(*inFile, recentItem.file))
		{
			SRecentItem*	itemPtr;

			// Update the date and time in the list
			sRecentItems.Lock();
			itemPtr = static_cast<SRecentItem*>(sRecentItems.GetItemPtr(iter.GetCurrentIndex()));
			Assert_(itemPtr);
			::GetDateTime(&itemPtr->lastOpened);
			sRecentItems.Unlock();
			fileInList = true;
			break;
		}
	}

	// Add the file to the list
	if (not fileInList)
	{
		recentItem.file = *inFile;
		::GetDateTime(&recentItem.lastOpened);

		sRecentItems.AddItem(recentItem);
		RemoveOldestRecentItem();
		UpdateRecentItemsMenu();
	}
}


// ---------------------------------------------------------------------------
//		* RemoveOldestRecentItem									[protected]
// ---------------------------------------------------------------------------

void
CAMacOSTarget::RemoveOldestRecentItem()
{
	while (static_cast<SInt32>(sRecentItems.GetCount()) > kMaxNumberOfRecentItems)
	{
		ArrayIndexT	oldestItem = LArray::index_Bad;
		UInt32		oldestTime = 0xFFFFFFFF;
		SRecentItem	recentItem;

		TArrayIterator<SRecentItem>	iter(sRecentItems);
		while (iter.Next(recentItem))
		{
			if (recentItem.lastOpened < oldestTime)
			{
				oldestItem = iter.GetCurrentIndex();
				oldestTime = recentItem.lastOpened;
			}
		}

		if (oldestItem != LArray::index_Bad)
			sRecentItems.RemoveItemsAt(1, oldestItem);
	}
}


// ---------------------------------------------------------------------------
//		* UpdateRecentItemsMenu										[protected]
// ---------------------------------------------------------------------------

void
CAMacOSTarget::UpdateRecentItemsMenu()
{
	LMenuBar*	menuBar;
	LMenu*		recentItemsMenu;

	menuBar = LMenuBar::GetCurrentMenuBar();
	recentItemsMenu = menuBar->FetchMenu(kRecentItemsSubMenu);
	if (recentItemsMenu)
	{
		SRecentItem	recentItem;
		
		// Clear out the previous menu items
		while (::CountMenuItems(recentItemsMenu->GetMacMenuH()) > 0)
			recentItemsMenu->RemoveItem(1);

		FindDuplicateFileNames();

		// Add the menu items
		SInt16	menuItem = 1;
		TArrayIterator<SRecentItem>	iter(sRecentItems);
		while (iter.Next(recentItem))
		{
			recentItemsMenu->InsertCommand("\px", cmd_UseMenuItem, menuItem);
			if (not recentItem.nameIsDuplicate)
			{
				::SetMenuItemText(recentItemsMenu->GetMacMenuH(), menuItem, recentItem.file.name);
			}
			else
			{
				Str255	fullPath;

				GetFullPathForFile(&recentItem.file, fullPath);
				::SetMenuItemText(recentItemsMenu->GetMacMenuH(), menuItem, fullPath);
			}
			menuItem++;
		}
	}
}


// ---------------------------------------------------------------------------
//		* FindDuplicateFileNames										[protected]
// ---------------------------------------------------------------------------

void
CAMacOSTarget::FindDuplicateFileNames()
{
	SRecentItem		itemOne;
	SRecentItem		itemTwo;
	SRecentItem*	itemOnePtr;
	SRecentItem*	itemTwoPtr;

	sRecentItems.Lock();

	// Reset the duplicate name flags
	TArrayIterator<SRecentItem>	iterOne(sRecentItems);
	while (iterOne.Next(itemOne))
	{
		itemOnePtr = static_cast<SRecentItem*>(sRecentItems.GetItemPtr(iterOne.GetCurrentIndex()));
		Assert_(itemOnePtr);
		itemOnePtr->nameIsDuplicate = false;
	}
	
	// Find the duplicate names
	iterOne.ResetTo(LArrayIterator::from_Start);
	while (iterOne.Next(itemOne) and (iterOne.GetCurrentIndex() < static_cast<SInt32>(sRecentItems.GetCount())))
	{
		TArrayIterator<SRecentItem>	iterTwo(sRecentItems, iterOne.GetCurrentIndex());
		while (iterTwo.Next(itemTwo))
		{
			if (::RelString(itemOne.file.name, itemTwo.file.name, false, true) == 0)
			{
				itemOnePtr = static_cast<SRecentItem*>(sRecentItems.GetItemPtr(iterOne.GetCurrentIndex()));
				Assert_(itemOnePtr);
				itemTwoPtr = static_cast<SRecentItem*>(sRecentItems.GetItemPtr(iterTwo.GetCurrentIndex()));
				Assert_(itemTwoPtr);

				itemOnePtr->nameIsDuplicate = true;
				itemTwoPtr->nameIsDuplicate = true;
			}
		}
	}

	sRecentItems.Unlock();
}


// ---------------------------------------------------------------------------
//		* GetFullPathForFile										[protected]
// ---------------------------------------------------------------------------

void
CAMacOSTarget::GetFullPathForFile(
	const FSSpec*	inFile,
	StringPtr		ioFullPath)
{
	Assert_(inFile);
	Assert_(ioFullPath);
	
	OSStatus	status;
	Handle		fullPathH;
	SInt16		fullPathLength;
	
	ioFullPath[0] = 0;
	status = ::FSpGetFullPath(inFile, &fullPathLength, &fullPathH);
	if (status == noErr and fullPathH != NULL)
	{
		StHandleBlock	pathH(fullPathH);
		StHandleLocker	lock(pathH);
		if (fullPathLength > 255)
		{
			// Truncate the beginning of the string
			ioFullPath[0] = 255;
			::BlockMoveData(*pathH + fullPathLength - 255, ioFullPath + 1, 255);
			ioFullPath[1] = 'É';
		}
		else
		{
			ioFullPath[0] = fullPathLength;
			::BlockMoveData(*pathH, ioFullPath + 1, fullPathLength);
		}
	}

}


// ===========================================================================

#pragma mark -
#pragma mark ** display class window behaviors
	
// ---------------------------------------------------------------------------
//		* MakeTypeList											[protected]
// ---------------------------------------------------------------------------
//	Create the global class list.

void
CAMacOSTarget::MakeTypeList()
{
	VPTypeList::CreateGlobalTypeList();
}



#pragma mark -
#pragma mark *** SRecentItemComparator ***

// ===========================================================================
//	SRecentItemComparator
// ===========================================================================
//	Compares items as SRecentItem


// ---------------------------------------------------------------------------
//	Class Variables

SRecentItemComparator*	SRecentItemComparator::sRecentItemComparator = NULL;



// ---------------------------------------------------------------------------
//	¥ SRecentItemComparator									[public, virtual]
// ---------------------------------------------------------------------------

SRecentItemComparator::SRecentItemComparator()
{
}


// ---------------------------------------------------------------------------
//	¥ ~SRecentItemComparator									[public, virtual]
// ---------------------------------------------------------------------------

SRecentItemComparator::~SRecentItemComparator()
{
}


// ---------------------------------------------------------------------------
//	¥ GetComparator									[public, static]
// ---------------------------------------------------------------------------

SRecentItemComparator*
SRecentItemComparator::GetComparator()
{
	if (sRecentItemComparator == NULL)
	{
		sRecentItemComparator = new SRecentItemComparator;
	}

	return sRecentItemComparator;
}


// ---------------------------------------------------------------------------
//	¥ Clone									[public, virtual]
// ---------------------------------------------------------------------------

LComparator*
SRecentItemComparator::Clone()
{
	return new SRecentItemComparator;
}


// ---------------------------------------------------------------------------
//	¥ Compare									[public, virtual]
// ---------------------------------------------------------------------------

SInt32
SRecentItemComparator::Compare(
	const void*		inItemOne,
	const void*		inItemTwo,
	UInt32			inSizeOne,
	UInt32			inSizeTwo) const
{
// This pragma unused prevents the release builds from spitting out a warning.
#pragma unused (inSizeOne, inSizeTwo)
	Assert_(inSizeOne == sizeof(SRecentItem));
	Assert_(inSizeTwo == sizeof(SRecentItem));

	SInt32	result;
	const SRecentItem*	itemOne = static_cast<const SRecentItem*>(inItemOne);
	Assert_(itemOne);
	const SRecentItem*	itemTwo = static_cast<const SRecentItem*>(inItemTwo);
	Assert_(itemTwo);

	result = ::RelString(itemOne->file.name, itemTwo->file.name, false, true);
	
	return result;
}
