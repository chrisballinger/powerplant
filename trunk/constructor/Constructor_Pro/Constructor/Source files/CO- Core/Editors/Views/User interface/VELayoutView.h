// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VELayoutView.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 06/17/96
//     $Date: 2006/01/18 01:33:19 $
//	$History: VELayoutView.h $
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 02/28/97   Time: 10:23
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Added SuperActivate and SuperDeactivate overrides to ensure that
//	classes that redraw themselves on (de)activate events draw properly.
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 02/07/97   Time: 14:08
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Rolled in Clint's fixes 02/07/97.
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 02/07/97   Time: 13:59
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Rolled in Clint's fixes 02/06/97.
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 01/31/97   Time: 10:12
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Enabled printing of views. (Bug fix #1204.)
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 01/21/97   Time: 17:10
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 12/06/96   Time: 18:32
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Added friend class declaration for VPDrawingAgent. (Ugh!)
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/24/96   Time: 22:27
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Fixed a bug in ClickSelf. Command-click now properly selects the object
//	behind the current selection.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/16/96   Time: 23:38
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Tweaked refactoring of layout view.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/13/96   Time: 13:37
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
#include "DMDragSelector.h"
#include "DMDropArea.h"
#include "DMListener.h"

	// Core : Display classes (Eric) : Windows
#include "GReferencedWindow.h"

	// Core : Editors : Views : Data model
#include "VEModelObject.h"

	// Core : Editors : Views : User interface
#include "VEPrefsResources.h"


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class RFMap;
class VEDrawingAgent;
class VEBackdrop;


// ===========================================================================
//		* VELayoutView
// ===========================================================================
//
//	VELayoutView is the layout editor display for all targets. It provides
//	the behaviors for setting up and tearing down display objects, and
//	handling mouse and menu commands. In general, drawing agents are used
//	to handle the target-specific behaviors. Subclassing VELayoutView should
//	not be necessary.
//
// ===========================================================================

class VELayoutView :	public LView,
						public LCommander,
						public DMDropArea,
						public DMListener,
						public DMDragSelector,
						public GZoomer {

public:
	enum { class_ID = 'VELV' };
	
	static LPane*			CreateFromStream(LStream* inStream)
									{ return new VELayoutView(inStream); }

							VELayoutView(LStream* inStream);
	virtual					~VELayoutView();

	// menu and key commands

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

	// clipboard behaviors
	
	virtual Boolean			CanPaste() const;
	virtual void			DoPaste();
	
	// accessors
	
	inline VEModelObject*	GetRootObject() const
									{ return mRootObject; }
	virtual void			SetRootObject(VEModelObject* inPane);
	
	inline Boolean			GetRootObjectMoves()
									{ return mRootObjectMoves; }
	inline void				SetRootObjectMoves(Boolean inValue)
									{ mRootObjectMoves = inValue; }
									
	inline DMSelection*		GetSelection() const
									{ return mSelection; }
	virtual void			SetSelection(DMSelection*	inSelection);

	virtual void			SetLayoutBackdrop(
									VEBackdrop*			inBackdrop);

	// resource map accessors

	inline static RFMap*	GetCurrentMap()
									{ return sCurrentMap; }
	inline RFMap*			GetRFMap()
									{ return mRFMap; }
	inline void				SetRFMap(RFMap* inMap)
									{ mRFMap = inMap; }

	// window sizing

	virtual void			AdjustImageSize();
	virtual void			CalcStandardSize(
									GReferencedWindow*	inWindow);

	// public configuration
	
	virtual void			EditGridDialog();

	// activate/deactivate events

	virtual void			SuperActivate();
	virtual void			SuperDeactivate();

	// drawing behaviors

protected:
	virtual void			Draw(
									RgnHandle			inSuperDrawRgnH);

	virtual void			DrawSelf();

	virtual void			DrawModelObject(
									VEModelObject*		inObject,
									Boolean				inPrinting);
	virtual void			DrawPaneAdornments(
									VEModelObject*		inObject,
									const Rect&			inLocalFrame,
									Boolean				inPrinting);
	virtual void			DrawSelection();

	
	virtual void			DrawGrid(
									const Rect&			inBaseFrame);

	virtual void			GetGridColor(
									RGBColor&			outColor);
								
	virtual void			ActivateSelf();
	virtual void			DeactivateSelf();
	virtual void			RefreshSelection();
	
	virtual void			RefreshRootObject();

	virtual Boolean			ObjectIsVisible(
									VEModelObject*		inObject) const;
	

	// printing behaviors

	virtual void			SuperPrintPanel(
									const PanelSpec&	inSuperPanel,
									RgnHandle			inSuperPrintRgnH);
	virtual void			PrintPanel(
									const PanelSpec&	inPanel,
									RgnHandle			inSuperPrintRgnH);
//	virtual void			PrintPanelSelf(
//									const PanelSpec&	inPanel);
//	virtual void			PrintAdornments(
//									const Rect&			inBaseFrame);
	
	// mouse-down behaviors
	
	virtual void			Click(
									SMouseDownEvent&	inMouseDown);
	virtual void			ClickSelf(
									const SMouseDownEvent& inMouseDown);

	virtual VEModelObject*	HitTestObject(
									VEModelObject*		inParentObject,
									VEModelObject*&		inBehindObject,
									SInt32				inHorizPort,
									SInt32				inVertPort);
	virtual VEModelObject*	HitTestSelectedObjects(
									SInt32				inHorizPort,
									SInt32				inVertPort);
	virtual VEModelObject*	HitTestSelectionHandles(
									SInt32				inHorizPort,
									SInt32				inVertPort,
									Boolean&			outBottomCorner,
									Boolean&			outRightCorner);

	virtual void			NearestSnapPoint(
									Point&				ioClickPoint);

	virtual void			DragSelectionHandle(
									VEModelObject*			inHitObject,
									Boolean					inHitBottomCorner,
									Boolean					inHitRightCorner,
									const SMouseDownEvent&	inMouseDown);
	virtual void			DragSelectedObjects(
									const SMouseDownEvent&	inMouseDown);
	virtual void			DoMarqueeSelection(
									const SMouseDownEvent&	inMouseDown);

	virtual void			ClickOnDisplayObject(
									VEModelObject*			inObject,
									const SMouseDownEvent&	inMouseDown,
									Boolean					inDeselectObject);

	// cursor adjusting

#if __PowerPlant__ < 0x02114006

	virtual void			AdjustCursor(
									Point					inPortPt,
									const EventRecord&		inMacEvent);

#else

	virtual void			AdjustMouse(
									Point					inPortPt,
									const EventRecord&		inMacEvent,
									RgnHandle				outMouseRgn);

#endif // __PowerPlant__ < 0x02114006

	// drawing agent interactions

	virtual void			BuildObjectDisplay(
									VEModelObject*		inObject,
									Boolean				inCallFinishCreate = true);
	virtual void			DeleteObjectDisplay(
									VEModelObject*		inObject,
									Boolean				inRefresh);

	VEDrawingAgent*			FindDrawingAgentForObject(
									VEModelObject*		inObject);

	virtual void			CheckRootObjectPosition();

	// drag-tracking helpers
	
	virtual Boolean			MakeDragRegion(
									VEModelObject*		inElement,
									RgnHandle			outGlobalDragRgn,
									Rect&				outGlobalItemBounds);
	virtual void			MakeDragImage(
									DragReference		inDragRef,
									VEModelObject*		inObject,
									RgnHandle&			ioImageRgn,
									GWorldPtr&			ioImageGWorld);

	// drag-selection behaviors
	
	virtual void			CheckElements(
									const Rect&			inOldMarqueeRect,
									const Rect&			inNewMarqueeRect);
	virtual void			CheckMarqueeSelection(
									VEModelObject*		inParentObject,
									const Rect&			inNewMarqueeRect,
									const Rect&			inTestAreaRect);

	// drag-receive behaviors
									
	virtual Boolean			ChoosePlacementForElement(
									DragReference		inDragRef,
									ItemReference		inItemRef,
									const DMElement*	inElement,
									DM_DragPlacement&	ioCurrentPlacement);
	virtual void			InsideDropArea(
									DragReference		inDragRef);
	virtual void			HiliteDropArea(
									DragReference		inDragRef);
	virtual void			DoDragConstraints(
									DragReference		inDragRef);

	virtual DMElement*		HostDragElement(
									DMElement*			inElement,
									Rect&				inItemLocalBounds,
									DM_DragPlacement&	ioCurrentPlacement);
	virtual void			AdjustScreenPosition(
									VEModelObject*		inObject,
									Rect&				inItemLocalBounds);

	// selection behaviors
	
	virtual void			SelectAll(
									VEModelObject*		inParentObject);

	// update notifications

	virtual void			ContainerListChanged(
									DM_ListChange*		inMessage);

	// pane resizing behaviors
	
	virtual void			MoveSelectionBy(
									SInt32				inHorizDelta,
									SInt32				inVertDelta);
	virtual void			ResizeSelectionBy(
									SInt32				inHorizDelta,
									SInt32				inVertDelta);

	// internal layout configuration
	
	
	virtual LPane*			GetBasePaneDisplay();

	virtual void			GetDesktopOverscan(
									SDimension32&		outTopLeft,
									SDimension32&		outBotRight);

	// initialization

	virtual void			FinishCreateSelf();


	// data members

protected:
	VEModelObjectPtr		mRootObject;				// root pane in this display
	DMSelectionPtr			mSelection;					// selection for panes in the display
	SShowHideInfo			mShowHideInfo;				// local copy of show/hide prefs
	SGridInfo				mGridInfo;					// local copy of grid prefs

	VEBackdrop*				mLayoutBackdrop;			// layout backdrop object
	RFMap*					mRFMap;						// resource ap associated with this layout
	
	Boolean					mRootObjectMoves;		// whether to allow the root object to move

	// class variables

protected:
	static SShowHideInfo	sShowHideInfo;				// global copy of show/hide prefs
	static SGridInfo		sGridInfo;					// global copy of grid prefs
	static Boolean			sPrefsLoaded;				// true if prefs have been loaded
	static Boolean			sPrefsChanged;				// true if prefs have been changed since last save
	static RFMap*			sCurrentMap;				// current resource map


	friend class VEDrawingAgent;
	friend class VEResizeReceiver;
	friend class VPDrawingAgent;

};
