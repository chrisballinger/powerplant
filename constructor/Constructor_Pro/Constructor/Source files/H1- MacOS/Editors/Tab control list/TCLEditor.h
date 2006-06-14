// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	TCLEditor.h					© 2001-2002 Metrowerks Inc. All rights reserved.
// ===========================================================================

#pragma once

#include "REEditor.h"


// ===========================================================================
//		* TCLEditor
// ===========================================================================
//	Editor for Mac OS tab control list (tab#) resources.

class TCLEditor : public REEditor {

public:
							TCLEditor(
									LCommander*		inSuper,
									RESession&		inSession);
	virtual					~TCLEditor();

	// editor override hooks

protected:
	virtual void			ReadResourceDataSelf();
	virtual void			WriteResourceDataSelf();

	virtual LWindow*		OpenEditorWindowSelf();
	
	// initialization
	
	static void				RegisterTCLClasses();
	
};
