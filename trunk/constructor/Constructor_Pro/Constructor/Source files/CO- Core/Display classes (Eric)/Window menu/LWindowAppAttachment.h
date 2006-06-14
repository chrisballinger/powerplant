// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	LWindowAppAttachment.h		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================

#pragma once

#include <LAttachment.h>

struct SWindowMenuInfo;

const CommandT	cmd_FirstWindow		= 0x1F000000;
const CommandT	cmd_LastWindow		= 0x1FFFFFFF;

class LWindowMenuAttachment;


// ===========================================================================
//		* LWindowAppAttachment
// ===========================================================================
//
//		This class is designed as an attachment to the application object.
//		It intercepts menu commands and command-status queries aimed for
//

class LWindowAppAttachment : public LAttachment {

public:
							LWindowAppAttachment();
	virtual					~LWindowAppAttachment();

	// menu adders/removers
	
	static void				AddWindow(
									LWindow*				inWindow,
									LWindowMenuAttachment*	inAttachment,
									LWindow*				inAfterWindow,
									Boolean					inAddDividerLine);
	static void				RemoveWindow(
									LWindow*				inWindow);

	static void				SetWindowMenu(
									SInt16					inMenuID);

	// menu updating

protected:
	virtual void			ExecuteSelf(
									MessageT				inMessage,
									void*					ioParam);
	virtual void			FindCommandStatus(
									SCommandStatus*			inCommandStatus);

	// menu command helpers

	LWindow*				CommandToWindow(
									CommandT				inCommand);
	LWindowMenuAttachment*	CommandToAttachment(
									CommandT				inCommand);
	CommandT				WindowToCommand(
									const LWindow*			inWindow);

	// internal menu adders/removers
	
	virtual void			InternalAddWindow(
									LWindow*				inWindow,
									LWindowMenuAttachment*	inAttachment,
									LWindow*				inAfterWindow,
									Boolean					inAddDividerLine);
	virtual void			InternalRemoveWindow(
									LWindow*				inWindow);


	// data members

	LArray					mWindowMenuInfoList;
	SInt16					mWindowMenuID;
	SInt16					mWindowMenuStart;
	CommandT				mNextCommandNumber;
	
	static LWindowAppAttachment* sWindowAppAttachment;


	friend class LWindowMenuAttachment;

};
