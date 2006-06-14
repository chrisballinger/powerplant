// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RSDocument.h				© 1995-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 02/01/96
//	   $Date: 2006/01/18 01:32:59 $
//	$History: RSDocument.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:44
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Refactored RS* classes into PD* (generic) and RS* (MacOS resource
//	manager) classes.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:27
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/05/96   Time: 14:52
//	Updated in $/Constructor/Source files/Documents/+ Generic resource file
//	Resource editor shell overhaul.
//
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:55
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Core objects
#include "DMPointers.h"

	// Core : Documents : Generic resource document
#include "RSContainer.h"

	// Core : Documents : Project document
#include "PDDocument.h"

	// Core : Resource manager
#include "RFMap.h"

	// PowerPlant : Commander classes
#include <LSingleDoc.h>


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class REEditor;
class RESession;


// ===========================================================================
//		* RSDocument											[abstract]
// ===========================================================================
//
//	RSDocument is an abstract base class for all MacOS resource-file based
//	documents in Constructor.
//
// ===========================================================================

class RSDocument : public PDDocument {

public:
							RSDocument(
									LCommander*			inSuper);
	virtual					~RSDocument();

	// initialization

	virtual void			FinishCreate(
									const FSSpec*		inFileSpec);

	// file opening

protected:
	virtual void			SetupMapAndContainer(
									RMMap*&				outRMMap,
									ResIDT&				outContainerDSPC);

	virtual void			OpenResourceFile(
									LFile*				inFile);
	virtual void			OpenFolderForType(
									ResType				inResType);

	virtual void			BuildEditWindow();
	
	// file saving

	virtual void			DoSave();
	virtual void			DoAESave(
									FSSpec&				inFileSpec,
									OSType				inFileType);
	
	virtual void			SaveCDEFs();
	
	// resource editors
	
	virtual REEditor*		CreateEditorForResource(
									const RMIdentifier&	inResType,
									const RMIdentifier&	inResID);
	virtual REEditor*		CreateEditorForResource(
									ResType				inResType,
									ResIDT				inResID) = 0;

	virtual void			RecordCollapsedTypes();

	// configuration

	static void				RegisterRSClasses();



	// data members

protected:
	RFMap*					mRFMap;					// raw resource map
//	RSContainerPtr			mRsrcContainer;	//???	// objects created from resource map

	friend class RESession;

};
