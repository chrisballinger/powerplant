// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	REEditor.h					© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 04/27/96
//	   $Date: 2006/01/18 01:33:06 $
//	$History: REEditor.h $
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 02/28/97   Time: 11:34
//	Updated in $/Constructor/Source files/CO- Core/Editors/00- Generic editor
//	Added GetPDContainer method.
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:33
//	Updated in $/Constructor/Source files/CO- Core/Editors/00- Generic editor
//	Refactored RS* classes into PD* (generic) and RS* (MacOS resource
//	manager) classes.
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:28
//	Updated in $/Constructor/Source files/CO- Core/Editors/00- Generic editor
//	Fixed CR/LF problem
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 01/07/97   Time: 17:43
//	Updated in $/Constructor/Source files/Editors/+ Generic editor
//	Rolled in an update from Clint 01/01/97.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 11/30/96   Time: 20:14
//	Updated in $/Constructor/Source files/Editors/+ Generic editor
//	Added SetModified, EnableSelf, and DisableSelf methods.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/25/96   Time: 21:01
//	Updated in $/Constructor/Source files/Editors/+ Generic editor
//	Added mSuppressErrors flag.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/11/96   Time: 10:40
//	Updated in $/Constructor/Source files/Editors/+ Generic editor
//	Added FinishCreateSelf method to REEditor.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/05/96   Time: 15:15
//	Updated in $/Constructor/Source files/Editors/+ Generic editor
//	Resource editor shell overhaul.
//
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:58
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Utility classes
#include "DMListener.h"


// ===========================================================================
//		* Forward class definitions
// ===========================================================================

class RESession;
class RMResource;
class PDContainer;


// ===========================================================================
//		* Exception codes
// ===========================================================================
//	These exceptions codes may be used by REEditor during the creation
//	process to trigger or suppress the "can't open resource" dialog.

const ExceptionCode	err_CantOpen			= -1;
const ExceptionCode	err_CantOpenSilent		= -2;



// ===========================================================================
//		* REEditor
// ===========================================================================
//
//	REEditor is the base class for all resource editors in Constructor.
//	It defines the basic API that all editors must respond to. It will
//	form the basis for the external editor API.
//
// ===========================================================================

class REEditor :	public LCommander,
					public DMListener,
					public virtual LSharable {

public:
	enum { objID_Editor = 'edit' };

							REEditor(
									LCommander*			inSuper,
									RESession&			inSession);
	virtual					~REEditor();

	// editor management functions
	
	inline RMResource*		GetPrimaryResource() const
									{ return mPrimaryResource; }
	void					SetPrimaryResource(RMResource* inResource);
	void					ReadResourceData();
	void					WriteResourceData();
	void					ResourceDeleted(RMResource* inResource);

	virtual DMElement*		GetRootElement() 
									{ return mDataModelRoot; }	

	virtual PDContainer*	GetPDContainer() const;

	LWindow*				OpenEditorWindow();
	void					CloseEditorWindow();
	void					UpdateEditorWindowTitle();

	Boolean					IsModified();
	void					SetModified();
	void					ResetModified();

	void					UpdateResID(
									RMResource*			inResource,
									ResIDT				inOldResID,
									ResIDT				inNewResID);

	// editor override hooks

protected:
	virtual void			FinishCreateSelf();

	virtual void			SetPrimaryResourceSelf(RMResource* inResource);
	virtual void			ReadResourceDataSelf();
	virtual void			WriteResourceDataSelf();
	virtual void			ResourceDeletedSelf(RMResource* inResource);

	virtual LWindow*		OpenEditorWindowSelf();
	virtual void			CloseEditorWindowSelf();
	virtual void			UpdateEditorWindowTitleSelf();

	virtual Boolean			IsModifiedSelf();
	virtual void			ResetModifiedSelf();
	
	virtual void			EnableSelf();
	virtual void			DisableSelf();

	virtual void			UpdateResIDSelf(
									RMResource*			inResource,
									ResIDT				inOldResID,
									ResIDT				inNewResID);

	// editor management

	virtual void			NoMoreUsers();
	virtual void			RemoveSubCommander(LCommander* inSub);
	virtual Boolean			AllowSubRemoval(LCommander* inSub);	// GHD

	// update notifications

	virtual void			FindUIObject(DM_FindUIObject* inFindUIMsg);
	virtual void			GoingAway(DMElement* inElement);

	virtual void			CollectChanges(DMElement* inElement);
	virtual void			CollectTentativeChanges(DM_CollectTentativeChanges* inMessage);
	virtual void			ValueChanged(DM_ValueChange* inMessage);
	virtual void			EnabledChanged(DMElement* inElement);


	// data members
	
protected:
	RESession&				mSession;					// editor session
	RMResource*				mPrimaryResource;			// primary resource
	Boolean					mResourceDataRead;			// true if ReadResourceData has been called
	Boolean					mSuppressErrors;			// true if error dialogs should be suppressed

	LWindow*				mEditorWindow;				// primary editor window
	DMElementPtr			mDataModelRoot;				// root of data model (optional)
	DMSelectionPtr			mSelection;					// selection for data model (optional)
	
private:
	LSharableArray			mResources;					// hidden because this will not be available
														//	to editor plug-ins

	friend class RESession;

};


// ===========================================================================
//		* Update message
// ===========================================================================
//	The following message is sent by RESession to indicate that a resource
//	has gained or lost an editor. The message is sent through the PDResource's
//	BroadcastMessage method.

const MessageT	msg_HasEditor		= 'edit';
	// ioParam = PDResource*
