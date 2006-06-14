// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	MEEditor.h					© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 10/10/96
//	   $Date: 2006/01/18 01:33:08 $
//	$History: MEEditor.h $
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 02/20/97   Time: 17:06
//	Updated in $/Constructor/Source files/CO- Core/Editors/Menus
//	Improved commenting.
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:49
//	Updated in $/Constructor/Source files/CO- Core/Editors/Menus
//	Finished adaptation to refactored RM/RF classes.
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 12/06/96   Time: 23:55
//	Updated in $/Constructor/Source files/Editors/Menus/+ Generic menu editor
//	Updated to Clint's drop 12/06/96.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 11/05/96   Time: 15:22
//	Updated in $/Constructor/Source files/Editors/Menus
//	Resource editor shell overhaul.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 11/04/96   Time: 12:01
//	Updated in $/Constructor/Source files/Editors/Menus
//	Added ContainerListChanged method.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/03/96   Time: 11:31
//	Updated in $/Constructor/Source files/Editors/Menus
//	Removed resIDT_FakeMenu constant.
//
//	*****************  Version 3   *****************
//	User: scouten      Date: 10/24/96   Time: 01:17
//	Checked in '$/Constructor/Source files/Editors/Menus'
//	Comment: Finished menu editor.
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/23/96   Time: 20:56
//	Checked in '$/Constructor/Source files/Editors/Menus'
//	Comment: Baseline working version of menu editor.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:59
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

class MEModelMenuBar;
class MEModelMenu;
class MEModelMenuItem;
class DMReanimator;
class RFResource;
class RSResource;


// ===========================================================================
//		* MEEditor
// ===========================================================================
//
//	MEEditor is the resource editor for MBAR and MENU resources.
//	It encapsulates the knowledge of these resource formats, and the
//	display and data model classes used to implement the editor.
//
// ===========================================================================

class MEEditor : public REEditor {

public:
							MEEditor(
									LCommander*			inSuper,
									RESession&			inSession);
	virtual					~MEEditor();
	
	// resource addition and deletion
	
	virtual void			AddMenu(
									ResIDT				inMenuID,
									DMElementPtr&		inObjectPtr);
	virtual void			ReleaseMenu(
									MEModelMenu*		inMenu);

	virtual ResType			GetMenuResourceType() const;
								
	virtual void			GetDefaultTitleForPosition(
									SInt32				inPosition,
									LStr255&			outString);
	
	// prototype menu item accessor
	
	void					GetPrototypeMenu(DMElementPtr& outMenu);
	inline void				GetPrototypeMenuItem(DMElementPtr& outMenuItem)
									{ outMenuItem = mPrototypeMenuItem->Clone(); }

	// session management implementation overrides

protected:
	virtual void			ReadResourceDataSelf();
	virtual void			WriteResourceDataSelf();
	virtual LWindow*		OpenEditorWindowSelf();

	virtual void			ResourceDeletedSelf(
									RMResource*			inResource);

	virtual void			UpdateResIDSelf(
									RMResource*			inResource,
									ResIDT				inOldResID,
									ResIDT				inNewResID);

	// menu streaming helpers
	
	virtual void			ReadMbarResource(
									RFResource&			inResource,
									DMElementPtr&		inObjectPtr);
	virtual void			ReadMenuResource(
									RFResource&			inResource,
									DMElementPtr&		inObjectPtr);
	virtual void			ReadMenuItem(
									LStream&			inMenuStream,
									LStream&			inXmnuStream,
									Boolean				inEnabled,
									SInt32				inMenuCommand,
									DMElementPtr&		inObjectPtr);

	virtual void			WriteMbarResource(
									RFResource&			inResource,
									MEModelMenuBar*		inMenuBar);
	virtual void			WriteMenuResource(
									RFResource&			inResource,
									MEModelMenu*		inMenu);
	virtual void			WriteMenuItem(
									LStream&			inMenuStream,
									LStream&			inXmnuStream,
									MEModelMenuItem*	inMenuItem);

	// initialization

	virtual void			FinishCreateSelf();

	// update notifications

	virtual void			ContainerListChanged(
									DM_ListChange*		inMessage);

	// menu helper
	
	MEModelMenu*			FindMenu(
									ResIDT				inMenuID);

	// class registration
	
	virtual void			RegisterMEClasses(
									DMReanimator*		inReanimator);


	// data members

protected:
	DMElementPtr			mPrototypeMenuItem;			// prototype menu item (used for efficiency)

	
	// class variables

protected:
	static Boolean			sRegisteredClasses;

};
