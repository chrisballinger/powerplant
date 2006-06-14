// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	LWindowMenuAttachment.h		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================

#pragma once

#include <LAttachment.h>


// ===========================================================================
//		* LWindowMenuAttachment
// ===========================================================================
//
//	When this attachment is attached to an LWindow, it causes the
//	window to be added to the Windows menu. Requires that an
//	LWindowAppAttachment be attached to the application object.
//
//	By default, each new window is simply added to the bottom of the
//	Windows menu. If you require other behavior, you may create a
//	subclass of LWindowMenuAttachment which overrides InstallMenu.
//
// ===========================================================================

class LWindowMenuAttachment : public LAttachment {

public:
	enum { class_ID = 'wmnu' };

	static void*			CreateFromStream(LStream*	inStream)
									{ return new LWindowMenuAttachment(inStream); }

							LWindowMenuAttachment();
							LWindowMenuAttachment(
									LStream*			inStream);

	virtual					~LWindowMenuAttachment();

	// menu installation

protected:
	virtual void			ExecuteSelf(
									MessageT			inMessage,
									void*				ioParam);

	virtual void			InstallMenu();
	void					RemoveMenu();

	// menu configuration

	virtual void			FindCommandStatus(
									SCommandStatus*		inCommandStatus);


	// data members
	
	LWindow*				mWindow;	
	Boolean					mAddedMenu;


	// common initialization

private:
	void					InitWindowMenuAttachment();

	friend class LWindowAppAttachment;

};
