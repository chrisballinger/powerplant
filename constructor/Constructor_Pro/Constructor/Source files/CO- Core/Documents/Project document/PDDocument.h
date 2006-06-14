// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PDDocument.h				   © 1997 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 02/02/97
//     $Date: 2006/01/18 01:33:04 $
//  $History: PDDocument.h $
//	
//	*****************  Version 2  *****************
//	User: Andrew       Date: 2/05/97    Time: 4:39p
//	Updated in $/ConstructorWin/Includes
//	Added include to let MSVC build the DMSelectionPtr template
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:31
//	Created in $/Constructor/Source files/CO- Core/Documents/Project document
//	Refactored RS* classes into PD* (generic) and RS* (MacOS resource
//	manager) classes.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Core objects
#include "DMPointers.h"
#include "DMSelection.h"

	// Core : Documents : Project document
#include "PDContainer.h"

	// PowerPlant : Commander classes
#include <LSingleDoc.h>


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class REEditor;
class RESession;
class RMMap;


// ===========================================================================
//		* PDDocument											[abstract]
// ===========================================================================
//
//	RSDocument is an abstract base class for all project file
//	documents in Constructor. It provides the helpers for building the
//	resource map object and improves on the default PowerPlant behavior
//	for the "Save Changes?" dialog.
//
// ===========================================================================

class PDDocument : public LSingleDoc {

public:
							PDDocument(
									LCommander*			inSuper);
	virtual					~PDDocument();

	// file initialization
	
	virtual void			FinishCreate(
									const FSSpec*		inFileSpec);

	// file closing confirmation

	virtual void			Close();
	virtual void			AttemptClose(
									Boolean				inRecordIt);
	virtual Boolean			AttemptQuitSelf(
									SInt32				inSaveOption);

	// duplicate-file testing
	
	virtual Boolean			OpenIfMatch(
									FSSpec*				inMacFSSpec);

	// accessors
	
	inline LWindow*			GetWindow()
									{ return mWindow; }
	inline DMSelection*		GetSelection() const
									{ return mSelection; }
	
	// file opening

protected:
	virtual void			SetupMapAndContainer(
									RMMap*&				outRMMap,
									ResIDT&				outContainerDSPC) = 0;
	virtual void			CommonOpenFile(
									LFile*				inFile,
									PDContainer*		inRsrcContainer);

	virtual void			NameNewDoc(
									ResIDT				inFileTitles = 0);
	virtual void			OpenResourceFile(
									LFile*				inFile) = 0;

	virtual void			OpenDefaultFolders();
	virtual void			OpenFolderForType(
									ResType				inResType) = 0;

	virtual void			BuildEditWindow() = 0;
	virtual void			CheckCollapsedTypes();
	virtual void			RecordCollapsedTypes();

	// file saving

	virtual Boolean			AskSaveAs(
									FSSpec&				outFSSpec,
									Boolean				inRecordIt);
	

	
	virtual void			RawSave(
									const FSSpec&		inTempFileSpec,
									const FSSpec&		inCurrentFileSpec);

	virtual Boolean			IsModified();

	virtual MessageT		DoSaveChangesDialog(
									ResIDT				inDialogPPob,
									SInt32				inStringIndex);

	// file closing

	virtual void			RemoveSubCommander(
									LCommander*			inSub);

	// resource editors
	
	virtual REEditor*		CreateEditorForResource(
									const RMIdentifier&	inResType,
									const RMIdentifier&	inResID) = 0;

	// subclass information

	virtual OSType			GetCreatorCode() = 0;
	virtual OSType			GetFileTypeCode() = 0;


	// data members

protected:
	RMMap*					mRMMap;					// raw resource map
	RESession*				mSession;				// editor "session" that controls interactions
													//	with resource editors

	PDContainerPtr			mRsrcContainer;			// objects created from resource map
	DMSelectionPtr			mSelection;				// selected objects in document


	friend class RESession;

};
