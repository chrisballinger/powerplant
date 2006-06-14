// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RESession.h					© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 06/10/96
//	   $Date: 2006/01/18 01:33:06 $
//	$History: RESession.h $
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 02/20/97   Time: 17:06
//	Updated in $/Constructor/Source files/CO- Core/Editors/00- Generic editor
//	Improved commenting.
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:33
//	Updated in $/Constructor/Source files/CO- Core/Editors/00- Generic editor
//	Refactored RS* classes into PD* (generic) and RS* (MacOS resource
//	manager) classes.
//	
//	*****************  Version 6  *****************
//	User: Andrew       Date: 2/03/97    Time: 5:17p
//	Updated in $/ConstructorWin/Includes
//	Exported FlushDeletedEditors() for use by other classes
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:28
//	Updated in $/Constructor/Source files/CO- Core/Editors/00- Generic editor
//	Fixed CR/LF problem
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/07/97   Time: 17:52
//	Updated in $/Constructor/Source files/Editors/+ Generic editor
//	Rolled in an update from Clint 01/05/97.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/25/96   Time: 21:34
//	Updated in $/Constructor/Source files/Editors/+ Generic editor
//	Improved error handling in the create editor process.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/05/96   Time: 15:17
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

	// Core : Data model : Core objects
#include "DMPointers.h"


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class PDContainer;
class PDDocument;
class REEditor;
class RMMap;
class RMResource;

class LStr255;


// ===========================================================================
//		* RESession
// ===========================================================================
//
//	RESession is the primary point of connection between resource editors
//	and the rest of Constructor. A session is a generic host for resources
//	which may or may not correspond to a user file.
//
// ===========================================================================

class RESession : public LPeriodical {

public:
							RESession(
									PDDocument*			inDocument,
									PDContainer*		inContainer,
									RMMap*				inMap);
	virtual 				~RESession();

	// editor/resource management

	virtual REEditor*		GetEditorForResource(
									ResType				inResType,
									ResIDT				inResID,
									Boolean				inCreateIfNeeded,
									Boolean				inSuppressErrors = false);
	virtual void			TearDownEditor(
									REEditor*			inEditor);	

	virtual RMResource*		GetResource(
									ResType				inResType,
									ResIDT				inResID,
									Boolean				inCreateIfNeeded);

	virtual RMResource*		AcquireResourceForEditing(
									REEditor*			inEditor,
									ResType				inResType,
									ResIDT				inResID,
									Boolean				inCreateIfNeeded);
	virtual void			ReleaseResourceFromEditing(
									REEditor*			inEditor,
									RMResource*			inResource);

	virtual void			GetWindowTitleForEditor(
									REEditor*			inEditor,
									LStr255&			outTitle);
	virtual void			GetSelectionForEditor(
									REEditor*			inEditor,
									DMSelectionPtr&		outSelection);
	virtual void			FlushDeletedEditors();

	// resource configuration

protected:
	virtual void			ConfigureDataModel(
									REEditor*			inEditor);

	// resource tear-down

	virtual void			SpendTime(
									const EventRecord&	inMacEvent);

	// error dialogs
	
protected:
	virtual void			ShowCantOpenDialog();


	// data members

protected:
	PDDocument*				mDocument;
	PDContainer*			mContainer;
	RMMap*					mMap;
	LArray					mDeletedEditors;			// (of REEditor*)

	friend class REEditor;

};
