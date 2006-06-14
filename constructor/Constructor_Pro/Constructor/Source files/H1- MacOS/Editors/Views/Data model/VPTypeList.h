// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPTypeList.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 06/12/96
//	   $Date: 2006/01/18 01:33:57 $
//	$History: VPTypeList.h $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/24/97   Time: 15:00
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Updated for refactored RFMap classes.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 12/16/96   Time: 13:59
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added HandleResourceDataChanged override to allow dynamic updating of
//	type list as CTYPs are edited. (Bug fix #1098.)
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/30/96   Time: 18:52
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Improved commenting (again).
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/17/96   Time: 11:12
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Improved commenting.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/31/96   Time: 13:59
//	Created
//	Comment: 
//	
// ===========================================================================

#pragma once

	// Core : Editors : Views : Data model
#include "VETypeList.h"


// ===========================================================================
//		* Helper type definitions
// ===========================================================================

class VEModelObject;
class VPTypeEntry;

class VPTypeList;
typedef TSharablePtr<VPTypeList> VPTypeListPtr;


// ===========================================================================
//		* VPTypeList
// ===========================================================================
//
//	VPTypeList contains the list of all known PowerPlant display classes.
//	It encapsulates the knowledge of the various resource formats for
//	describing custom display classes.
//
// ===========================================================================

class VPTypeList : public VETypeList {

	// data model description
	
	DMClassID_				('vptl', VPTypeList, VETypeList);

	// constructor/destructor

public:
							VPTypeList(LStream* inStream);
							VPTypeList(const VPTypeList& inOriginal);
	virtual					~VPTypeList();

	// type list accessor

	static void				CreateGlobalTypeList();

	// resource file scanning

protected:
	virtual void			ScanCurrentResourceFile(
									const FSSpec&		inSourceFile);
	virtual void			ScanResourceMap(
									const FSSpec&		inSourceFile,
									RFMap&				inRFMap);
	virtual void			ScanFolderForPlugIns(
									SInt16				inVRefNum,
									SInt32				inDirID);

	// resource -> data model conversion

	virtual Boolean			MakeTypeSpec(
									LStream&			inStream,
									ResType				inResType,
									const FSSpec&		inSourceFile);
	Boolean					MakeSpecFromCPPb(
									LStream&			inStream,
									const FSSpec&		inSourceFile);
	Boolean					MakeSpecFromCLSS(
									LStream&			inStream,
									const FSSpec&		inSourceFile);

	// item list readers

	void					ReadItemListFromCPPb(
									VPTypeEntry*		inEntry,
									LStream&			inStream);
	void					ReadItemListFromCLSS(
									VPTypeEntry*		inEntry,
									LStream&			inStream);

	// data model connection
	
	VEModelObject*			MakeModelObjectForClass(
									ClassIDT			inClassID);
	void					FixRuntimeParent(
									ClassIDT			inClassID,
									ClassIDT&			ioRuntimeParent);
	virtual VE_DrawAgentCreatorFunc
							ChooseDrawingAgent(
									ClassIDT			inClassID);

	// resource change messages

	virtual void			HandleResourceDataChanged(
									const RMMap_ChangeMessage* inMessage);

};
