// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	UPropertyInspector.h		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 05/14/96
//     $Date: 2006/01/18 01:33:24 $
//	$History: UPropertyInspector.h $
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 02/28/97   Time: 13:30
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Added support for property inspectors in the Window menu.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/18/97   Time: 16:03
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Added support for multiple property inspector windows.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:32
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Fixed CR/LF problem
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/26/96   Time: 18:05
//	Updated in $/Constructor/Source files/Property inspector
//	Changed resource ID allocation to avoid conflict with Display classes
//	(Eric) folder. Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 10/31/96   Time: 17:44
//	Updated in $/Constructor/Source files/Property inspector
//	Improved pointer validation. Registered PIFontPopup class.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 02:09
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class DMElement;
class DMObject;
class DMSelection;


// ===========================================================================
//		* UPropertyInspector
// ===========================================================================
//
//	UPropertyInspector is a utility class which allows other parts of
//	Constructor to configure the property inspector window.
//
// ===========================================================================

class UPropertyInspector {

public:
	static void				InspectSelection(
									DMSelection*	inSelection,
									LCommander*		inUndoHost,
									Boolean 		inUseHostAsCommander = false,
									Boolean			inPlaceInWindowMenu = false);
	static void				SetTopSelection(
									DMSelection*	inSelection,
									LCommander*		inUndoHost);

	static void				RegisterPIClasses();

protected:
	static LWindow*			CreateInspectorWindow(
									LCommander*		inCommander = nil,
									Boolean			inPlaceInWindowMenu = false);
	static void				ConfigureInspectorWindow(
									DMSelection*	inSelection,
									LCommander*		inUndoHost);
	static void				ClosingInspectorWindow(
									LWindow*		inWindow);
	static Boolean			SelectInspectorWindow(
									DMElement*		inElement);


	// class variables

protected:
	static LWindow*			sInspectorWindow;
	
	
	friend class PIInspectorTable;

};
