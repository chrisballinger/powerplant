// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VEPrefsResources.h			© 1995-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 07/22/95
//	   $Date: 2006/01/18 01:33:19 $
//	$History: VEPrefsResources.h $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/20/97   Time: 18:27
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Improved commenting.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:31
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Fixed CR/LF problem
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/16/96   Time: 23:39
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Fixed a commenting glitch.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/13/96   Time: 13:37
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


const ResType pref_ResourceType = 'Vprf';			// ResType for view editor prefs


// ===========================================================================
//		* Show/hide preferences information
// ===========================================================================

const ResIDT pref_ShowHide = 129;

struct SShowHideInfo {
	Boolean			mShowGrid;
	Boolean			mShowPaneAdornments;
	Boolean			mShowPaneEdges;
	Boolean			mShowInvisiblePanes;
	Boolean			x1; // mShowViewPlaceholders;	// obsolete... left for compatibility
	Boolean			x2; // mUseTextPaneIDs;			// obsolete
	Boolean			x3; // mUseTextMsgIDs;			// obsolete
	Boolean			mShowActualWindow;				// new to CW7
};


// ===========================================================================
//		* Grid preferences information
// ===========================================================================

const ResIDT pref_Grid = 130;

struct SGridInfo {
	Boolean			mSnapToGrid;
	SDimension16	mGridSize;
};
