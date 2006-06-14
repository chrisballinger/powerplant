// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SLEditor.h					© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 12/02/96
//     $Date: 2006/01/18 01:33:51 $
//  $History: SLEditor.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/28/97   Time: 20:03
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/String list
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:43
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/String list
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 12/04/96   Time: 16:37
//	Created in $/Constructor/Source files/Editors/String list
//	Added class.
//	
// ===========================================================================

#pragma once

	// Core : Editors : Generic editor
#include "REEditor.h"


// ===========================================================================
//		* SLEditor
// ===========================================================================
//	Editor for MacOS string list (STR#) resources.

class SLEditor : public REEditor {

public:
							SLEditor(
									LCommander*		inSuper,
									RESession&		inSession);
	virtual					~SLEditor();

	// editor override hooks

protected:
	virtual void			ReadResourceDataSelf();
	virtual void			WriteResourceDataSelf();

	virtual LWindow*		OpenEditorWindowSelf();
	
	// initialization
	
	static void				RegisterSLClasses();
	
};
