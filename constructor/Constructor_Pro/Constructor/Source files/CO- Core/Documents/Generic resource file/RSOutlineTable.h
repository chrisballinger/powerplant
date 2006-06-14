// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RSOutlineTable.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/01/96
//     $Date: 2006/01/18 01:33:00 $
//  $History: RSOutlineTable.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:44
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Refactored RS* classes into PD* (generic) and RS* (MacOS resource
//	manager) classes.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:28
//	Updated in $/Constructor/Source files/CO- Core/Documents/Generic resource file
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/05/96   Time: 14:54
//	Created in $/Constructor/Source files/Documents/+ Generic resource file
//	Resource editor shell overhaul.
//	
// ===========================================================================

#pragma once

	// Core : Display classes (Eric) : Outline table
#include "OVItemFactory.h"
#include "OVTable.h"

	// Core : Documents : Generic resource file
#include "RSContainer.h"


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class LPopupButton;
class RESession;
class RETypeList;


// ===========================================================================
//		* RSOutlineTable
// ===========================================================================
//
//	RSOutlineTable is the class for the project windows of most
//	Constructor documents. It is organized as a hierarchical list, with
//	resource types as the first-level entries and individual resources
//	as the second-level entries. This class provides the basic keyboard,
//	menu, and drag-and-drop behaviors for the project window.
//
// ===========================================================================

class RSOutlineTable : public OVTable {

public:
	enum { class_ID = 'RSot' };

	static LPane*			CreateFromStream(LStream* inStream)
									{ return new RSOutlineTable(inStream); }

							RSOutlineTable(LStream* inStream);
	virtual					~RSOutlineTable();

	// menu command behaviors
	
	virtual void			FindCommandStatus(
									CommandT			inCommand,
									Boolean&			outEnabled,
									Boolean&			outUsesMark,
									UInt16&				outMark,
									Str255				outName);
	virtual Boolean			ObeyCommand(
									CommandT			inCommand,
									void*				ioParam = nil);
	virtual Boolean			HandleKeyPress(
									const EventRecord&	inKeyEvent);

	virtual void			EditSelection();

	// selection accessors

	virtual void			SetSelection(
									DMSelection*		inSelection);
	inline void				SetSession(RESession*		inSession)
									{ mSession = inSession; }

	// container accessors
	
	inline RSContainer*		GetRSContainer() const
									{ return mContainer; }
	inline Boolean			GetResIDsSignificant() const
									{ return mContainer->GetResIDsSignificant(); }

	// custom resizing behaviors

	virtual void			AdjustImageSize(
									Boolean				inRefresh);
	virtual void			ResizeFrameBy(
									SInt16				inWidthDelta,
									SInt16				inHeightDelta,
									Boolean				inRefresh);
	virtual void			CalcStandardSize(
									GReferencedWindow*	inWindow);

	// clipboard behaviors

	virtual void			DoPaste();
	virtual void			DoDuplicate();

	// drag-and-drop behaviors

protected:
	virtual void			DoDragReceive(
									DragReference		inDragRef);

	// new resource dialog
	
	virtual void			DoNewResource(
									Boolean				inMustShowDialog);

	virtual Boolean			ShowNewResourceDialog(
									ResType&			outSelectedType,
									SInt16&				outSelectedMasterID,
									ResIDT&				outNewResID,
									LStr255&			outNewResName);
	virtual void			SetupResTypePopup(
									LPopupButton*			inPopup,
									RETypeList*			inTypeList,
									LArray&				ioPopupTypes,
									ResType&			ioSelectedType);
	virtual void			SetupResMasterPopup(
									LPopupButton*			inPopup,
									ResType				inSelectedType);
	
	virtual ResIDT			ChooseNewResID(
									RETypeEntry*		inTypeEntry);
	
	virtual ResType			GetSelectedResType() const;

	// configuration

	virtual void			SetElementSelf(DMElement*	inElement);
	virtual void			FinishCreateSelf();


	// data members

protected:
	RSContainer*			mContainer;					// data model container for resource list
	RESession*				mSession;					// editor "session"

	friend class RSTypeItem;							// *sigh* to get access to "sClickCount"

};


// ===========================================================================
//		* RSItemFactory
// ===========================================================================
//
//	RSItemFactory creates outline items for the project window.
//	It knows about RSResource and RSType data model objects.
//
// ===========================================================================

class RSItemFactory : public OVItemFactory {

public:
							RSItemFactory() {}
	virtual					~RSItemFactory() {}

	virtual OVItem*			CreateItemForElement(DMElement* inElement);

};
