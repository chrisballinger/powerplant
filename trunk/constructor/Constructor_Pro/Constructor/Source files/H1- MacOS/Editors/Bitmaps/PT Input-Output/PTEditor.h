// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PTEditor.h								©1995-96 Metrowerks Inc. All rights reserved.
// ===========================================================================

#pragma once

#include "REEditor.h"


// ===========================================================================
//		¥ PTEditor
// ===========================================================================
//	The editor for PowerPlant bitmap resources.

class PTPaintView;

class PTEditor :	public REEditor 
{

	public:
								PTEditor(
										LCommander*			inSuper,
										RESession&			inSession);
		virtual					~PTEditor();
		
		virtual Boolean			ObeyCommand( CommandT inCommand, void *ioParam );

	protected:
		PTPaintView				*mPaintView;
		Boolean					mFileIsLocked;
		
		virtual void			ReadResourceDataSelf();
		virtual void			WriteResourceDataSelf();
		virtual LWindow*		OpenEditorWindowSelf();
		virtual void			CloseEditorWindowSelf();
		
		virtual void			EnableSelf();
		virtual void			DisableSelf();
		
		virtual Boolean			IsModifiedSelf();
		virtual void			ResetModifiedSelf();
		
		static void				RegisterPTClasses();
};

