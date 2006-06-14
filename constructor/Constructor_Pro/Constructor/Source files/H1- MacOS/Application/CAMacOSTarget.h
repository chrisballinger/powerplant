// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	CAMacOSTarget.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 09/26/96
//     $Date: 2006/01/18 01:33:33 $
//	$History: CAMacOSTarget.h $
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 03/10/97   Time: 16:20
//	Updated in $/Constructor/Source files/H1- MacOS/Application
//	Updated to use PDDocument instead of RSDocument.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/04/97   Time: 19:05
//	Updated in $/Constructor/Source files/CO- Core/Application
//	Finished adaptation to refactored RM/RF classes.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:18
//	Updated in $/Constructor/Source files/CO- Core/Application
//	Fixed CR/LF problem
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 12/05/96   Time: 19:13
//	Updated in $/Constructor/Source files/Application
//	Deleted RememberOpenWindowsSelf, which wasn't doing anything anyway.
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

#pragma once

	// Core : Application
#include "CAGenericTarget.h"
#include <LComparator.h>


// ===========================================================================
//		* CAMacOSTarget
// ===========================================================================
//
//	CAMacOSTarget is the Constructor application class for the MacOS
//	target. It overrides the specific behaviors of CAGenericTarget
//	to provide knowledge of the specific document types it can open.
//
// ===========================================================================
struct SRecentItem
{
	FSSpec	file;
	UInt32	lastOpened;
	bool	nameIsDuplicate;
};
typedef SRecentItem	SRecentItem;

class CAMacOSTarget : public CAGenericTarget {

public:
							CAMacOSTarget();
	virtual					~CAMacOSTarget();

	// project file behaviors

	virtual void			FindCommandStatus(
									CommandT			inCommand,
									Boolean&			outEnabled,
									Boolean&			outUsesMark,
									UInt16&				outMark,
									Str255				outName);
	virtual Boolean			ObeyCommand(
									CommandT			inCommand,
									void*				ioParam);

protected:
	virtual PDDocument*
MakeEmptyProject();
	virtual PDDocument*
MakeProjectFromFile(
	const FSSpec&	inFileSpec);
	virtual void			ChooseDocument();
	
	// display class window behaviors

	virtual void			MakeTypeList();

	virtual	void			Initialize();

	static TArray<SRecentItem>	sRecentItems;

	virtual void			LoadRecentItems();
	virtual void			SaveRecentItems();
	virtual void			AddFileToRecentItems(const FSSpec* inFile);
	virtual	void			RemoveOldestRecentItem();
	virtual	void			UpdateRecentItemsMenu();
	virtual	void			FindDuplicateFileNames();
	virtual	void			GetFullPathForFile(const FSSpec* inFile, StringPtr ioFullPath);
};

//===========================================================
class	SRecentItemComparator : public LComparator
{
public:
							SRecentItemComparator();
	virtual					~SRecentItemComparator();

	virtual SInt32			Compare(
								const void*			inItemOne,
								const void* 		inItemTwo,
								UInt32				inSizeOne,
								UInt32				inSizeTwo) const;

	virtual LComparator*	Clone();

	static SRecentItemComparator*	GetComparator();

protected:
	static SRecentItemComparator*	sRecentItemComparator;
};
