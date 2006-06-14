// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPEditor.h					© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 06/12/96
//	   $Date: 2006/01/18 01:34:05 $
//	$History: VPEditor.h $
//	
//	*****************  Version 13  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:55
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/User interface
//	Finished adaptation to refactored RM/RF classes.
//	
//	*****************  Version 12  *****************
//	User: scouten      QDate: 01/29/97   Time: 18:01
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/User interface
//	Fixed ReadObjectFromPPobTags so that it safely recovers from undershoot
//	when reading stream data. (Bug fix #1233.)
//	
//	*****************  Version 11  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:50
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/User interface
//	Fixed CR/LF problem
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 11/25/96   Time: 21:36
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/+ User interface
//	Updated to use the new mSuppressErrors flag in REEditor.
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 11/25/96   Time: 20:35
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/+ User interface
//	Improved handling of missing custom types.
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 11/22/96   Time: 09:21
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/+ User interface
//	Fixed a silly syntax error.
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 11/22/96   Time: 09:20
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/+ User interface
//	Finished code in UpdateResIDSelf.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 11/21/96   Time: 11:31
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/+ User interface
//	Added Make Tab Group command and hooks for implementing Make Radio
//	Group.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 11/20/96   Time: 13:51
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/+ User interface
//	Added code to generate RidL and wctb resources.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/19/96   Time: 17:31
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/+ User interface
//	Added support for dynamic columns in hierarchy window.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/19/96   Time: 12:18
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/+ User interface
//	Added special case for tab groups.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/17/96   Time: 10:48
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/+ User interface
//	Added PPob generator code.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/31/96   Time: 13:59
//	Created
//	Comment: 
//	
// ===========================================================================

#pragma once

#include "VEEditor.h"
#include "VPModelObject.h"
#include "VPTypeList.h"


// ===========================================================================
//		* VPEditor
// ===========================================================================
//
//	VPEditor is the class that implements the PowerPlant view editor.
//	It encapsulates the knowledge of the PPob resource format and how
//	to create appropriate editor windows for layout and object hierarchy.
//
// ===========================================================================

class VPEditor : public VEEditor {

public:
							VPEditor(
									LCommander*				inSuper,
									RESession&				inSession);
	virtual					~VPEditor();

	// commander behaviors

	virtual void			FindCommandStatus(
									CommandT				inCommand,
									Boolean&				outEnabled,
									Boolean&				outUsesMark,
									UInt16&					outMark,
									Str255					outName);
	virtual Boolean			ObeyCommand(
									CommandT				inCommand,
									void*					ioParam = nil);

	// initialization

	static void				RegisterVPClasses();

	// editor override hooks

protected:
	virtual void			ReadResourceDataSelf();
	virtual void			WriteResourceDataSelf();
	virtual void			UpdateResIDSelf(
									RMResource*				inResource,
									ResIDT					inOldResID,
									ResIDT					inNewResID);

	// editor windows

	virtual void			AddDefaultHierColumns(
									VEHierTable*			inHierTable);

	// PPob parser

	void					ReadObjectFromPPobTags(
									VPModelObjectPtr&		inObjectPtr,
									VPModelObject*			inParentObject,
									LStream&				inPPobStream,
									LStream&				inWindStream);
	void					ReadSubObjectsFromPPobTags(
									VPModelObject*			inParentObject,
									LStream&				inPPobStream,
									LStream&				inWindStream);
	void					ReadWctbResource(
									RMResource*				inWctbResource,
									VPModelObject*			inRootObject);

	void					ComplainAboutUnknownType(
									ClassIDT				inUnknownType);
	
	// PPob generator
	
	void					WritePPobTags(
									VPModelObject*			inObject,
									LHandleStream&			inPPobStream,
									LHandleStream&			inWindStream);
	void					WritePPobTagsForSubs(
									DMContainerAttribute*	inAttachments,
									DMContainerAttribute*	inSubPanes,
									LHandleStream&			inPPobStream,
									LHandleStream&			inWindStream);

	void					WriteRidLData(
									VPModelObject*			inObject,
									LHandleStream&			inRidLStream);
	void					WriteWctbData(
									VPModelObject*			inRootObject,
									LHandleStream&			inWctbStream);

	// radio and tab group commands

	virtual void			PutOnDuty( LCommander *inNewTarget);

	virtual void			MakeRadioGroup();
	virtual void			MakeTabGroup();


	// data members

protected:
	VPTypeListPtr			mTypeList;						// type list object
	VPModelObjectPtr		mRootObject;					// root object in data model

	Boolean					mDeleteUnknownPanes;			// true to delete pane types that aren't reconized

};
