// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	Constructor.menu.h			© 1995-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//	Description of Constructor menus & related resources
//
//   Created: 03/30/95
//     $Date: 2006/01/18 01:32:26 $
//  $History: Constructor.menu.h $
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 02/18/97   Time: 18:18
//	Updated in $/Constructor/Source files/CO- Core/00- Headers
//	Improved commenting.
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:18
//	Updated in $/Constructor/Source files/CO- Core/00- Headers
//	Fixed CR/LF problem
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 12/05/96   Time: 19:14
//	Updated in $/Constructor/Source files/+ Headers
//	Removed cmd_ShowTypes.
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 11/26/96   Time: 16:52
//	Updated in $/Constructor/Source files/+ Headers
//	Added cmd_Catalog command.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 11/14/96   Time: 14:53
//	Updated in $/Constructor/Source files/+ Headers
//	Removed cmd_NewCatalog. Fixed menu_Window constant (was pointing to
//	menu 133, now points to 131).
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 11/14/96   Time: 13:46
//	Updated in $/Constructor/Source files/+ Headers
//	Removed outdated menu commands. Moved some editor-specific commands to
//	headers in the editor folders.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/05/96   Time: 14:37
//	Updated in $/Constructor/Source files/+ Headers
//	Added Save All and Close All commands.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/02/96   Time: 15:00
//	Updated in $/Constructor/Source files/+ Headers
//	Added cmd_NewItemDialog menu command.
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/21/96   Time: 23:06
//	Checked in '$/Constructor/Source files/+ Headers'
//	Comment: Added comment about str_GetInfo
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:28
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once


// ===========================================================================
//		* Application-wide menu command numbers
// ===========================================================================

enum {

	// * Apple menu
	
	menu_Apple							= 128,

	// * File menu
	
	menu_File							= 129,
		cmd_CloseAll					= 'xall',
		cmd_SaveAll						= 'sall',

	// * Edit menu
	
	menu_Edit							= 130,
		cmd_NewItem						= 'newi',
		cmd_NewItemDialog				= 'newd',
		cmd_EditItem					= 'edit',
		cmd_Duplicate					= 'dupl',
	
		cmd_NewMenu						= 'nmnu',			// added when menu bar editor is active
		cmd_NewSeparatorItem			= 'nsep',			// added when menu editor is active
		cmd_NewSubMenuItem				= 'nsub',			// added when menu editor is active

	// * Windows menu
	
	menu_Windows						= 131,
		cmd_PropertyInspector			= 'insp',
		cmd_Catalog						= 'catl',
		cmd_AlignPalette				= 'apal',
		cmd_ZoomWindow					= 'zoom',

	// * Not in any menus
	cmd_AddFileToRecentItems			= 'AtRi'			// Sent by PRDocument::DoAESave when a new file is saved.
};


// ===========================================================================
//		* Menu strings resources for edit menu
// ===========================================================================
//	Resource 1420 is the default string set. Other resources will
//	follow the same sequence.

const ResIDT		STR_DefaultEditStrings	= 1420;
//const SInt16		str_CutDefault			= 1;
//const SInt16		str_CopyDefault			= 2;
//const SInt16		str_PasteDefault		= 3;
//const SInt16		str_ClearDefault		= 4;
const SInt16		str_NewItem				= 5;
const SInt16		str_EditItem			= 6;
//					str_GetInfo				= 7;			// now obsolete
const SInt16		str_Duplicate			= 8;
const SInt16		str_Drag				= 9;
