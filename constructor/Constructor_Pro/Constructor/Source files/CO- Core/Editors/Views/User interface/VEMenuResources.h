// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VEMenuResources.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 11/14/96
//	   $Date: 2006/01/18 01:33:19 $
//	$History: VEMenuResources.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 18:27
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:31
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/21/96   Time: 10:17
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Removed the (already commented out) definitions of cmd_MakeRadioGroup
//	and cmd_MakeTabGroup.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/16/96   Time: 23:38
//	Created in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Moved menu command IDs from Constructor.menu.h to VE folder.
//	
// ===========================================================================

#pragma once


// ===========================================================================
//		* View editor menu command numbers
// ===========================================================================

enum {

	// * Arrange menu
	
	menu_Arrange						= 2000,
		cmd_AlignDialog					= 'algn',
		
		cmd_AlignLeft					= 'alft',
		cmd_AlignHorizCenter			= 'ahrz',
		cmd_AlignRight					= 'argt',
		cmd_SpreadHoriz					= 'shrz',
		cmd_SpreadHorizContainer		= 'shct',
		
		cmd_AlignTop					= 'atop',
		cmd_AlignVertCenter				= 'avrt',
		cmd_AlignBottom					= 'abtm',
		cmd_SpreadVert					= 'svrt',
		cmd_SpreadVertContainer			= 'svct',
		
	// * Layout menu

	menu_Layout							= 2001,
		cmd_ShowGrid					= 'sgrd',
		cmd_SnapToGrid					= 'snap',
		cmd_EditGrid					= 'egrd',

		cmd_ShowHierarchy				= 'hier',
		cmd_ShowPaneIDs					= 'pid ',
		cmd_ShowPaneEdges				= 'pedg',
		cmd_ShowInvisiblePanes			= 'sinv',
	
	//* Internal commands
	
		// alignment dialog

		cmd_SpreadHorizUserAmt			= 'husd',
		cmd_SpreadVertUserAmt			= 'vusd',
		cmd_SpreadHorizQuery			= 'shr?',
		cmd_SpreadVertQuery				= 'svr?',
		cmd_SpreadHorizContQuery		= 'shc?',
		cmd_SpreadVertContQuery			= 'svc?'
		
};
