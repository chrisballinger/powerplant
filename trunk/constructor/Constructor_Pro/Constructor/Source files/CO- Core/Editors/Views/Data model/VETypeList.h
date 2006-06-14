// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VETypeList.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 06/12/96
//	   $Date: 2006/01/18 01:33:13 $
//	$History: VETypeList.h $
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 03/17/97   Time: 14:05
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/Data model
//	Added VEResIDComparator class.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/24/97   Time: 15:06
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/Data model
//	Update for the refactored RFMap classes.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/30/96   Time: 18:32
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/Data model
//	Added support for catalog. Refactored parts of the MakeSpecFrom<xxxx>
//	routines.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/13/96   Time: 13:37
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor
//	Rearranged view editor classes.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/05/96   Time: 21:23
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor
//	Made CanDeleteThis const to avoid hiding DMElement's method.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 02:00
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Attributes
#include "DMContainerAttribute.h"

	// Core : Data model : Core classes
#include "DMObject.h"

	// Core : Editors : Views : Data model
#include "VETypeEntry.h"


// ===========================================================================
//		* Public exception codes
// ===========================================================================

const ExceptionCode	err_CantMakeTypeSpec	= 'xtyp';


// ===========================================================================
//		* Helper type definitions
// ===========================================================================

struct RMMap_ChangeMessage;

class RFMap;
class VEModelObject;

class VETypeList;
typedef TSharablePtr<VETypeList> VETypeListPtr;


// ===========================================================================
//		* VETypeList											[abstract]
// ===========================================================================
//
//	VETypeList contains the list of all known classes in a given
//	framework. It is an abstract base class, since some knowledge of
//	the class hierarchy is necessarily framework-specific.
//
// ===========================================================================

class VETypeList :	public DMObject,
					public LListener {

	// data model description
	
	DMAbstractClassID_		('vetl', VETypeList, DMObject);
	DMContainerAttribute_	('CLST', ClassList);
	DMContainerAttribute_	('CATP', CatalogPages);

	// constructor/destructor
	
public:
							VETypeList(LStream* inStream);
							VETypeList(const VETypeList& inOriginal);
	virtual					~VETypeList();

	// access control

	virtual Boolean			CanDeleteThis() const;

	// types list lookup
	
	virtual VETypeEntry*	FindTypeByID(
									ClassIDT		inClassID) const;
	virtual VETypeEntry*	FindTypeByName(
									ConstStringPtr	inClassName) const;

	// resource file scanning
	
	virtual void			ScanAtLaunch();
	virtual void			ScanProjectFile(
									const FSSpec&	inSourceFile,
									RFMap&			inRFMap);

	// accessors
	
	inline Boolean			GetClassIDsSignificant() const
									{ return mClassIDsSignificant; }

	// singleton accessor
	
	static VETypeList*		GetTypeList();
	static void				ReleaseTypeList();

	static	void			DoNoCPPbSupportDialog( ConstStringPtr inFileName = nil );

	// resource scanning internals

protected:
	virtual void			ScanFolderForPlugIns(
									SInt16			inVRefNum,
									SInt32			inDirID);
	virtual void			ScanCurrentResourceFile(
									const FSSpec&	inSourceFile) = 0;
	virtual void			ScanResourceMap(
									const FSSpec&	inSourceFile,
									RFMap&			inRFMap) = 0;

	virtual void			MakeSpecsFromFolder(
									SInt16			inVRefNum,
									SInt32			inDirID,
									OSType			inCreatorCode,
									OSType			inFileType);
	virtual void			MakeSpecsFromFile(
									ResType			inResType,
									const FSSpec&	inSourceFile,
									ResIDT			inMinimumID = -32768,
									ResIDT			inMaximumID = 32767);
	virtual void			MakeSpecsFromMap(
									ResType			inResType,
									const FSSpec&	inSourceFile,
									RFMap&			inRFMap);

	// type specification builders
	
	virtual Boolean			MakeTypeSpec(
									LStream&		inStream,
									ResType			inResType,
									const FSSpec&	inSourceFile) = 0;
	virtual Boolean			MakeSpecFromCTYP(
									LStream&		inStream,
									const FSSpec&	inSourceFile);

	virtual VEModelObject*	MakeModelObjectForClass(
									ClassIDT		inClassID) = 0;
	virtual DMContainerAttribute*
							MakeContainerAttribute(
									VETypeEntry*	inEntry);

	virtual void			FixRuntimeParent(
									ClassIDT		inClassID,
									ClassIDT&		ioRuntimeParent);

	virtual VE_DrawAgentCreatorFunc
							ChooseDrawingAgent(
									ClassIDT		inClassID);

	virtual void			PrepareTypeEntry(
									VETypeEntry*	inNewTypeEntry,
									VETypeEntry*&	inDataModelParent,
									VETypeEntry*&	inRuntimeParent,
									VETypeEntry*&	inPreviousEntry);
	virtual void			InstallTypeEntry(
									VETypeEntry*	inNewTypeEntry,
									VETypeEntry*	inRuntimeParent,
									VETypeEntry*	inPreviousEntry);

	virtual DMContainerAttribute*
							FindCatalogPage(
									VETypeEntry*	inNewTypeEntry,
									VETypeEntry*	inRuntimeParent);

	// resource change messages

	virtual void			ListenToMessage(
									MessageT		inMessage,
									void*			ioParam);
	virtual void			HandleResourceAdded(
									const RMMap_ChangeMessage* inMessage);
	virtual void			HandleResourceDataChanged(
									const RMMap_ChangeMessage* inMessage);

	// initialization

	virtual void			FinishCreateSelf();


	// data members

protected:
	Boolean					mTypesAreBuiltIn;		// true while building class list if
													//	classes are built-in
	Boolean					mClassIDsSignificant;	// true if four-character class IDs
													//	should be made visible in the catalog
	ResIDT					mCatalogPageSpecID;		// resource ID of catalog page
													//	specification ('VECP' resource)

	// class variables

protected:
	static VETypeListPtr	sTypeList;				// pointer to singleton class list

};


// ===========================================================================
//		* VEResIDComparator
// ===========================================================================
//	Used by VETypeList::MakeSpecsFromFile to sort resource handles by ID.

class VEResIDComparator : public LComparator {

public:
							VEResIDComparator() {}
	virtual					~VEResIDComparator() {}

	virtual SInt32			Compare(
									const void*		inItemOne,
									const void* 	inItemTwo,
									UInt32			inSizeOne,
									UInt32			inSizeTwo) const;

};
