// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	TTEditor.h					© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 10/29/96
//	   $Date: 2006/01/18 01:33:53 $
//	$History: TTEditor.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:18
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Text traits
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:43
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Text traits
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/05/96   Time: 15:23
//	Updated in $/Constructor/Source files/Editors/Text traits
//	Resource editor shell overhaul.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/30/96   Time: 18:28
//	Created
//	Comment: Baseline Txtr editor.
//	
// ===========================================================================

#pragma once

	// Core : Editors : Generic editor
#include "REEditor.h"


// ===========================================================================
//		* TTEditor
// ===========================================================================
//
//	MEEditor is the resource editor for Txtr (PowerPlant text traits)
//	resources. It encapsulates the knowledge of the resource format, and the
//	display and data model classes used to implement the editor.
//
// ===========================================================================

class TTEditor : public REEditor {

public:
							TTEditor(
									LCommander*			inSuper,
									RESession&			inSession);
	virtual					~TTEditor();

	// session management implementation overrides

protected:
	virtual void			ReadResourceDataSelf();
	virtual void			WriteResourceDataSelf();
	virtual LWindow*		OpenEditorWindowSelf();

	// dynamic updating

	virtual void			ValueChanged(DM_ValueChange* inMessage);

	// class registration
	
	static void				RegisterTTClasses();

};
