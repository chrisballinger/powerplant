// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PRDocument.h				© 1995-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 10/09/95
//	   $Date: 2006/01/18 01:34:06 $
//	$History: PRDocument.h $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/19/97   Time: 20:04
//	Updated in $/Constructor/Source files/H1- MacOS/MacOS project
//	Improved commenting.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:53
//	Updated in $/Constructor/Source files/H1- MacOS/MacOS project
//	Finished adaptation to refactored RM/RF classes.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:50
//	Updated in $/Constructor/Source files/H1- MacOS/MacOS project
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/05/96   Time: 15:10
//	Updated in $/Constructor/Source files/Documents/MacOS project
//	Resource editor shell overhaul.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:57
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// Core : Documents : Generic resource file
#include "RSDocument.h"


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class RESession;


// ===========================================================================
//		* PRDocument
// ===========================================================================
//
//	PRDocument is the document for user project files (resource files).
//
// ===========================================================================

class PRDocument : public RSDocument {

public:
							PRDocument(
									LCommander*			inSuper);
	virtual					~PRDocument();

	// initialization

	virtual void			FinishCreate(
									const FSSpec*		inFileSpec);

	// file opening

protected:
	LFile*			mUnflattenedFile;
	Boolean			mIsFlattened;

	virtual void			SetupMapAndContainer(
									RMMap*&				outRMMap,
									ResIDT&				outContainerDSPC);

	virtual LFile*			CreateNewUnflattenedFile(
									ConstFSSpecPtr		inFileSpec);

	virtual void			CommonOpenFile(
									LFile*				inFile,
									PDContainer*		inRsrcContainer);

	// file saving

	virtual void			DoAESave(
									FSSpec&				inFileSpec,
									OSType				inFileType);

	virtual void			DoSave();

	virtual Boolean			AskSaveAs(
									FSSpec&				outFSSpec,
									Boolean				inRecordIt);

	virtual void			RawSave(
									const FSSpec&		inTempFileSpec,
									const FSSpec&		inCurrentFileSpec);

	virtual OSType			GetCreatorCode();
	virtual OSType			GetFileTypeCode();

	// resource editors

	virtual REEditor*		CreateEditorForResource(
									ResType				inResType,
									ResIDT				inResID);

};


// ---------------------------------------------------------------------------

OSErr	IsFlattenedResourceFile(
			ConstFSSpecPtr	inFile,
			Boolean*		outIsFlat);
