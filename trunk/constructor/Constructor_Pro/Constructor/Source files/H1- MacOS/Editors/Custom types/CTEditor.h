// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	CTEditor.h					© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 12/09/96
//     $Date: 2006/01/18 01:33:50 $
//  $History: CTEditor.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/04/97   Time: 19:03
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Custom types
//	Finished adaptation to refactored RM/RF classes.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:42
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Custom types
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 12/10/96   Time: 14:22
//	Created in $/Constructor/Source files/Editors/Custom types
//	Baseline CTYP editor.
//	
// ===========================================================================

#pragma once

	// Core : Editors : Generic editor
#include "REEditor.h"

	// MacOS : Editors : Custom types
#include "CTModelObject.h"
#include "CTTypeList.h"


// ===========================================================================
//		* CTEditor
// ===========================================================================
//
//	CTEditor is the resource editor for CTYP (PowerPlant custom display
//	class description) resources. It encapsulates the knowledge of the
//	resource format, and the display and data model classes used to
//	implement the editor.
//
// ===========================================================================

class CTEditor : public REEditor {

public:
							CTEditor(
									LCommander*				inSuper,
									RESession&				inSession);
	virtual					~CTEditor();

	// editor override hooks

protected:
	virtual void			ReadResourceDataSelf();
	virtual void			WriteResourceDataSelf();
	virtual LWindow*		OpenEditorWindowSelf();

	// CTYP parser

	void					ReadObjectFromCTYPTags(
									CTModelObjectPtr&		inObjectPtr,
									CTModelObject*			inParentObject,
									LStream&				inCTYPStream);
	void					ReadSubObjectsFromCTYPTags(
									CTModelObject*			inParentObject,
									LStream&				inCTYPStream);

	// CTYP generator
	
	void					WriteCTYPTags(
									CTModelObject*			inObject,
									LHandleStream&			inCTYPStream);
	void					WriteCTYPTagsForSubs(
									DMContainerAttribute*	inAttributes,
									LHandleStream&			inCTYPStream);

	// dynamic menus
	
	virtual void			PutOnDuty( LCommander *inNewTarget);
	virtual void			TakeOffDuty();

	// class registration
	
	static void				RegisterCTClasses();


	// data members

protected:
	CTTypeListPtr			mTypeList;


	// class variables
	
protected:
	static LMenu*			sCtypMenu;					// Custom Type menu
	
};
