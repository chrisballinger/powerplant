// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VEEditor.h					© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 06/12/96
//     $Date: 2006/01/18 01:33:16 $
//	$History: VEEditor.h $
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 02/07/97   Time: 13:59
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Rolled in Clint's fixes 02/06/97.
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:47
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Finished adaptation to refactored RM/RF classes.
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:30
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Fixed CR/LF problem
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 11/24/96   Time: 23:25
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Added CreateAlignmentPalette static function.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 11/24/96   Time: 22:10
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Fixed a bug that allowed multiple hierarchy windows to be opened.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 11/20/96   Time: 14:54
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Added UpdateEditorWindowTitleSelf method.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/19/96   Time: 17:22
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Added support for dynamic columns in hierarchy window. Added
//	AddDefaultHierColumns method.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/16/96   Time: 23:28
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/13/96   Time: 13:36
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor
//	Rearranged view editor classes.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 02:00
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// Core : Editors : Generic editor
#include "REEditor.h"


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class VEHierTable;


// ===========================================================================
//		* Resource IDs
// ===========================================================================

const ResIDT	STR_VEEditStrings		= 2000;


// ===========================================================================
//		* VEEditor												[abstract]
// ===========================================================================
//	VEEditor is the base class for layout editors for various targets.

class VEEditor : public REEditor {

public:
							VEEditor(
									LCommander*		inSuper,
									RESession&		inSession);
	virtual					~VEEditor();
	
	// menu commands
	
	virtual void			FindCommandStatus(
									CommandT		inCommand,
									Boolean&		outEnabled,
									Boolean&		outUsesMark,
									UInt16&			outMark,
									Str255			outName);
	virtual Boolean			ObeyCommand(
									CommandT		inCommand,
									void*			ioParam = nil);
	virtual Boolean			HandleKeyPress(
									const EventRecord& inKeyEvent);

	virtual void			DuplicateSelection();

	// alignment palette
	
	static LWindow*			CreateAlignmentPalette();

	// initialization

	static void				RegisterVEClasses();

	// editor windows

protected:
	virtual LWindow*		OpenEditorWindowSelf();
	virtual LWindow*		OpenHierWindowSelf();
	virtual ResIDT			GetLayoutViewPPob();
	
	virtual void			UpdateEditorWindowTitleSelf();

	virtual void			AddDefaultHierColumns(
									VEHierTable*	inHierTable);

	virtual void			RemoveSubCommander(
									LCommander*		inSub);

	// alignment implementation

	virtual void			AlignPanes(
									CommandT		inWhichAlign,
									void*			ioParam);
	virtual void			ShowArrangeDialog();

	// dynamic menus
	
	virtual void			PutOnDuty(LCommander *inNewTarget);
	virtual void			TakeOffDuty();



	// data member

protected:
	LWindow*				mHierWindow;			// object hierarchy window


	// class variables
	
protected:
	static LMenu*			sArrangeMenu;			// Arrange menu
	static LMenu*			sLayoutMenu;			// Layout menu

};
