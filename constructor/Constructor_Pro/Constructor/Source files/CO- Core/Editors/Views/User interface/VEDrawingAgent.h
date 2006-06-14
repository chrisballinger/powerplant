// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VEDrawingAgent.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 06/17/96
//     $Date: 2006/01/18 01:33:15 $
//	$History: VEDrawingAgent.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 15:06
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Update for the refactored RFMap classes.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/16/96   Time: 23:24
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Added code to handle resource map change messages.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/13/96   Time: 13:36
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor
//	Rearranged view editor classes.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 02:00
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Utilities
#include "DMListener.h"

	// Core : Editors : Views : Data model
#include "VEModelObject.h"


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class RMResource;
class VELayoutView;


// ===========================================================================
//		* VEDrawingAgent										[abstract]
// ===========================================================================
//
//	VEDrawingAgent is a base class for the object that helps the
//	layout view draw an object of a given class. It encapsulates all
//	of the knowledge about the actual display class within
//	Constructor's runtime environment. One drawing agent object is
//	created for every data model object (VEModelObject) which is
//	visible in the layout.
//
// ===========================================================================

class VEDrawingAgent : public DMListener {

public:
	enum { object_ID = 'VEda' };

							VEDrawingAgent();
	virtual					~VEDrawingAgent();

	// initial configuration
	
	virtual void			SetModelObject(
									VEModelObject*		inModelObject);
	virtual void			SetSelection(
									DMSelection*		inSelection);
	virtual void			SetLayoutView(
									VELayoutView*		inLayoutView);

	inline void				SetCantMove(Boolean			inCantMove)
									{ mCantMove = inCantMove; }

	// drawing behaviors
	
	virtual void			CreateDisplayObject(
									LView*				inSuperView) = 0;
	virtual void			RemoveDisplayObject();

	virtual void			RefreshDisplayObject();
	virtual void			RefreshSelection();

	virtual void			DrawDisplayObject();
	virtual void			DrawSelection();
	
	virtual Boolean			HitTestDisplayObject(
									SInt32				inHorizPort,
									SInt32				inVertPort);
	virtual Boolean			HitTestSelection(
									SInt32				inHorizPort,
									SInt32				inVertPort,
									Boolean&			outBottomCorner,
									Boolean&			outRightCorner);
	
	virtual Boolean			CalcPortFrameRect(
									Rect&				outFrame);
	virtual Boolean			CalcPortFrameRectForSuperview(
									Rect&				outFrame);
	
	// update notifications

protected:
	virtual void			FindUIObject(
									DM_FindUIObject*	inFindUIMsg);
	virtual void			SelectionChanged(
									DM_SelectionChanged* inMessage);
	
	virtual void			ValueChanged(
									DM_ValueChange*		inMessage);
	virtual void			ValueChangedSelf(
									FourCharCode		inAttributeKey,
									DMAttribute*		inAttribute);

	void					SubPaneDeleted(
									LPane*				inSubPane);
	virtual void			SubPaneDeletedSelf(
									LPane*				inSubPane);

	// resource map notifications
	
	virtual void			ListenToMessage(
									MessageT			inMessage,
									void*				ioParam);
	virtual void			ResourceChanged(
									RMResource*		inResource);
	virtual Boolean			ListenToMap();
	
	// internal accessors

	virtual LPane*			GetDisplayPane() const;

	// selection handle helper

	virtual void			CalcSelectionHandleRect(
									const Rect&			inDisplayFrame,
									Rect&				outHandleFrame,
									Boolean				inBottomCorner,
									Boolean				inRightCorner) const;

	// rebuild trigger
	
	virtual void			RebuildMe();


	// data members

protected:
	VEModelObjectPtr		mModelObject;				// the pane we're displaying
	VELayoutView*			mLayoutView;				// layout view we're displayed in
	DMSelectionPtr			mSelection;					// selection for the view editor
	LPane*					mDisplayPane;				// pane object we created
	Boolean					mCantMove;					// true if this is the root pane in the display
														//	(and thus it is not allowed to move)

	friend class VELayoutView;

};
