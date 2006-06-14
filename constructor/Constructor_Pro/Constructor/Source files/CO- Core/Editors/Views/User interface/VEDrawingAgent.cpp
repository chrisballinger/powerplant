// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VEDrawingAgent.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 06/17/96
//     $Date: 2006/04/12 08:48:13 $
//	$History: VEDrawingAgent.cpp $
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 02/20/97   Time: 18:27
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Improved commenting.
//	
//	*****************  Version 6  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:08p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/24/97   Time: 15:06
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Update for the refactored RFMap classes.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/20/96   Time: 10:12
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Fixed a bug in HitTestDisplayObject that allowed some classes to be
//	unclickable (especially LButton).
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

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "VEDrawingAgent.h"

	// Core : Editors : Views : Data model
#include "VEModelObject.h"

	// Core : Editors : Views : User interface
#include "VELayoutView.h"

	// MacOS : Resource manager
#include "RFMap.h"
#include "RFResource.h"


// ===========================================================================

#pragma mark *** VEDrawingAgent ***

// ---------------------------------------------------------------------------
//		* VEDrawingAgent()
// ---------------------------------------------------------------------------
//	Constructor

VEDrawingAgent::VEDrawingAgent()
{
	mLayoutView = nil;
	mDisplayPane = nil;
	mCantMove = false;
}


// ---------------------------------------------------------------------------
//		* ~VEDrawingAgent
// ---------------------------------------------------------------------------
//	Destructor

VEDrawingAgent::~VEDrawingAgent()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** initial configuration

// ---------------------------------------------------------------------------
//		* SetModelObject
// ---------------------------------------------------------------------------
//	Set the object that we'll be drawing on screen.

void
VEDrawingAgent::SetModelObject(
	VEModelObject*	inModelObject)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inModelObject);
	
	// Remember this object and listen to its changes.

	mModelObject = inModelObject;
	mModelObject->AddListener(this); 

}


// ---------------------------------------------------------------------------
//		* SetSelection
// ---------------------------------------------------------------------------
//	Set the selection for this view editor. The drawing agent is responsible
//	for drawing and removing selection indications.

void
VEDrawingAgent::SetSelection(
	DMSelection*	inSelection)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inSelection);
	
	// Remember selection pointer and listen to its change message.

	mSelection = inSelection;
	mSelection->AddListener(this);
	
}


// ---------------------------------------------------------------------------
//		* SetLayoutView
// ---------------------------------------------------------------------------
//	Set the layout view that we belong to. The drawing agent occasionally
//	uses this information to trigger redraws, etc.

void
VEDrawingAgent::SetLayoutView(
	VELayoutView*	inLayoutView)
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inLayoutView);
	
	// Remember pointer to layout view.
	
	mLayoutView = inLayoutView;
	
	// See if we need to listen to resource map
	
	if (ListenToMap()) {
		
		RFMap* map = inLayoutView->GetRFMap();
		ValidateObject_(map);
		
		map->AddListener(this);
	
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** drawing behaviors

// ---------------------------------------------------------------------------
//		* RemoveDisplayObject
// ---------------------------------------------------------------------------
//	Tear down the display for this object.

void
VEDrawingAgent::RemoveDisplayObject()
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Delete the display pane.

	if (mDisplayPane != nil) {
		ValidateObject_(mDisplayPane);
		SubPaneDeleted(mDisplayPane);
		delete mDisplayPane;
		mDisplayPane = nil;
	}
}


// ---------------------------------------------------------------------------
//		* RefreshDisplayObject
// ---------------------------------------------------------------------------
//	Cause the display object to redraw itself.

void
VEDrawingAgent::RefreshDisplayObject()
{

	// Validate pointers.
	
	ValidateThis_();

	// See if we've created a display pane for this object.
	// If so, we can provide default behavior for drawing.
	
	LPane* pane = GetDisplayPane();
	if (pane == nil)
		return;
	ValidateObject_(pane);

	// There is a pane, ask it to update itself.
	
	pane->Refresh();

}

// ---------------------------------------------------------------------------
//		* RefreshSelection
// ---------------------------------------------------------------------------
//	Cause the selection handles on this display object to be redrawn.

void
VEDrawingAgent::RefreshSelection()
{

	// Validate pointers.
	
	ValidateThis_();

	// See if we've created a display pane for this object.
	// If so, we can provide default behavior for selection
	// handles.
	
	LPane* pane = GetDisplayPane();
	if (pane == nil)
		return;
	ValidateObject_(pane);

	// If this pane is not exposed, don't bother drawing.
	
	if (pane->FocusExposed() == false)
		return;

	// Okay, the lights are all green. Set up to draw.
	
	Rect frame;
	pane->FocusDraw();
	pane->CalcLocalFrameRect(frame);

	// Update handles in all four corners.
	
 	for (Boolean bottom = false; bottom <= true; bottom++) {
		for (Boolean right = false; right <= true; right++) {
			Rect handleFrame;
			CalcSelectionHandleRect(frame, handleFrame, bottom, right);
			pane->CalcPortFrameRect(handleFrame);
			pane->InvalPortRect(&handleFrame);			
		}
	}
}


// ---------------------------------------------------------------------------
//		* DrawDisplayObject
// ---------------------------------------------------------------------------
//	Draw the visual representation of this object. Do NOT draw any of
//	its subpanes.

void
VEDrawingAgent::DrawDisplayObject()
{

	// Validate pointers.
	
	ValidateThis_();

	// See if we've created a display pane for this object.
	// If so, we can provide default behavior for drawing.
	
	LPane* pane = GetDisplayPane();
	if (pane == nil)
		return;
	ValidateObject_(pane);

	// There is a pane, ask it to draw itself, but not subpanes.
	
	if (pane->FocusDraw())
		pane->DrawSelf();

}


// ---------------------------------------------------------------------------
//		* DrawSelection
// ---------------------------------------------------------------------------
//	Draw selection handles on this object (if it is selected).

void
VEDrawingAgent::DrawSelection()
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mSelection.GetObject());

	// See if we've created a display pane for this object.
	// If so, we can provide default behavior for selection
	// handles.
	
	LPane* pane = GetDisplayPane();
	if (pane == nil)
		return;
	ValidateObject_(pane);

	// If this pane is not selected, there's nothing to draw.
	
	if (mSelection->IsSelected(mModelObject) == false)
		return;

	// If this pane is not exposed, don't bother drawing.
	
	if (pane->FocusExposed() == false)
		return;

	// Okay, the lights are all green. Set up to draw.
	
	Rect frame;
	pane->FocusDraw();
	pane->CalcLocalFrameRect(frame);

	StClipRgnState clipToPane;							//* 2.2d9: BUG FIX #388: don't draw selection beyond pane
	clipToPane.ClipToIntersection(frame);				//* 2.2a1: BUG FIX #623: but not beyond edges of superview

	RGBColor hiliteColor;
#ifdef WINVER
	hiliteColor = LMGetHiliteRGB();						// Altura bug
#else
	LMGetHiliteRGB(&hiliteColor);
#endif
	::RGBForeColor(&hiliteColor);
	
	RGBColor white = { 65535, 65535, 65535 };
	::RGBBackColor(&white);
	
	Boolean active = pane->IsActive();
	
	// Draw handles in all four corners.
	
 	for (Boolean bottom = false; bottom <= true; bottom++) {
		for (Boolean right = false; right <= true; right++) {
			Rect handleFrame;
			CalcSelectionHandleRect(frame, handleFrame, bottom, right);
			
			if (active)
			{
				Pattern thePattern;
				UQDGlobals::GetBlackPat(&thePattern);
				::FillRect(&handleFrame, &thePattern);
			}
			else {
				::EraseRect(&handleFrame);
				::FrameRect(&handleFrame);
			}
		}
	}
}


// ---------------------------------------------------------------------------
//		* HitTestDisplayObject
// ---------------------------------------------------------------------------
//	Return true if this object is hit by the specified point.

Boolean
VEDrawingAgent::HitTestDisplayObject(
	SInt32	inHorizPort,
	SInt32	inVertPort)
{

	// Validate pointers.
	
	ValidateThis_();

	// See if we've created a display pane for this object.
	// If so, we can provide default behavior for hit testing.
	
	LPane* pane = GetDisplayPane();
	if (pane == nil)
		return false;
	ValidateObject_(pane);

	// Get the revealed area of the pane or its superview.
	
	Rect revealed =  { 0, 0, 0, 0 };
	LView* view = (dynamic_cast<LView*>(pane));
	if (view == nil)
		view = pane->GetSuperView();
	if (view != nil) {
		ValidateObject_(view);
		view->GetRevealedRect(revealed);
	}
	else
		return false;

	Point hitPt = { inVertPort, inHorizPort };
	if (!::PtInRect(hitPt, &revealed))
		return false;

	// See if the mouse-down is within the pane's frame.
	// Do not allow subclasses of LPane to specify hit-testing.
	// Classes such as LButton will do things like testing against
	// the icon mask that make hit testing fail in Constructor.

	return pane->LPane::PointIsInFrame(inHorizPort, inVertPort);

}


// ---------------------------------------------------------------------------
//		* HitTestSelection
// ---------------------------------------------------------------------------
//	Return true if one of the selection handles was hit by the
//	specified mouse location.

Boolean
VEDrawingAgent::HitTestSelection(
	SInt32		inHorizPort,
	SInt32		inVertPort,
	Boolean&	outBottomCorner,
	Boolean&	outRightCorner)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mSelection.GetObject());

	// See if we've created a display pane for this object.
	// If so, we can provide default behavior for hit testing.
	
	LPane* pane = GetDisplayPane();
	if (pane == nil)
		return false;
	ValidateObject_(pane);

	// See if the mouse-down is within the pane's frame.
	// If not, don't bother testing the handles.

	if (!HitTestDisplayObject(inHorizPort, inVertPort))
		return false;

	// If this pane is not selected, no handles are drawn.
	
	if (mSelection->IsSelected(mModelObject) == false)
		return false;

	// If this pane is not exposed, don't bother hit testing.
	
	if (pane->FocusExposed() == false)
		return false;

	// Okay, the lights are all green. Set up to do the hit testing.
	
	Rect frame;
	pane->CalcPortFrameRect(frame);

	// Test handles in all four corners.
	
	Point testPt = { inVertPort, inHorizPort };
	
 	for (Boolean bottom = false; bottom <= true; bottom++) {
		for (Boolean right = false; right <= true; right++) {
			Rect handleFrame;
			CalcSelectionHandleRect(frame, handleFrame, bottom, right);
			
			if (::PtInRect(testPt, &handleFrame)) {
				outBottomCorner = bottom;
				outRightCorner = right;
				return true;
			}
		}
	}
	
	// Didn't hit any of the handles.
	
	return false;
	
}


// ---------------------------------------------------------------------------
//		* CalcPortFrameRect
// ---------------------------------------------------------------------------
//	Return the location of the display objects in port coordinates of
//	the layout window. Note that we do not provide access to the pane
//	object itself.

Boolean
VEDrawingAgent::CalcPortFrameRect(
	Rect&	outFrame)
{

	// Validate pointers.
	
	ValidateThis_();

	// Set return frame to empty rect if we can't compute
	// the real frame.
	
	::SetRect(&outFrame, 0, 0, 0, 0);

	// See if we've created a display pane for this object.
	// If so, we can provide a bounding box for the pane.
	
	LPane* pane = GetDisplayPane();
	if (pane == nil)
		return false;
	ValidateObject_(pane);

	// We've got a pane, let it do the math.
	
	return pane->CalcPortFrameRect(outFrame);

}


// ---------------------------------------------------------------------------
//		* CalcPortFrameRectForSuperview
// ---------------------------------------------------------------------------
//	Find the frame of the enclosing view of pane we're drawing.

Boolean
VEDrawingAgent::CalcPortFrameRectForSuperview(
	Rect&	outFrame)
{

	// Validate pointers.
	
	ValidateThis_();

	// Set return frame to empty rect if we can't compute
	// the real frame.
	
	::SetRect(&outFrame, 0, 0, 0, 0);

	// See if we've created a display pane for this object.
	// If so, we can provide a bounding box for the pane.
	
	LPane* pane = GetDisplayPane();
	if (pane == nil)
		return false;
	ValidateObject_(pane);
	
	LView* superView = pane->GetSuperView();
	if (superView == nil)
		return false;
	ValidateObject_(superView);

	// We've got a pane, let it do the math.
	
	return superView->CalcPortFrameRect(outFrame);

}


// ===========================================================================

#pragma mark -
#pragma mark ** update notifications

// ---------------------------------------------------------------------------
//		* FindUIObject											[protected]
// ---------------------------------------------------------------------------
//	Return a pointer to the drawing agent if so requested.

void
VEDrawingAgent::FindUIObject(
	DM_FindUIObject*	inFindUIMsg)
{

	// Validate pointers.
	
	ValidateThis_();
	ThrowIfNil_(inFindUIMsg);
	
	// If caller is looking for a drawing agent, we're probably it.

	if ((inFindUIMsg->mElement == (VEModelObject*) mModelObject)
	  && (inFindUIMsg->mObjectIdentifier == object_ID))
		inFindUIMsg->mObjectPointer = this;

}


// ---------------------------------------------------------------------------
//		* SelectionChanged										[protected]
// ---------------------------------------------------------------------------
//	Redraw the selection handle are if this object becomes selected
//	or unselected.

void
VEDrawingAgent::SelectionChanged(
	DM_SelectionChanged*	inMessage)
{

	// Validate pointers.
	
	ValidateThis_();
	ThrowIfNil_(inMessage);
	
	// If our element was (un)selected, redraw selection handles.

	if (inMessage->mElement == mModelObject.GetObject())
		RefreshSelection();

}


// ---------------------------------------------------------------------------
//		* ValueChanged											[protected]
// ---------------------------------------------------------------------------
//	An attribute's value has changed. If the attribute belongs to the
//	object we're displaying, call ValueChangedSelf.

void
VEDrawingAgent::ValueChanged(
	DM_ValueChange*		inMessage)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mModelObject.GetObject());
	ThrowIfNil_(inMessage);
	ValidateObject_(inMessage->mAttribute);

	// See if this message pertains to the object we're displaying.
	
	if (inMessage->mObject == mModelObject.GetObject()) {
	
		// It does. Call the override hook.

		ValueChangedSelf(inMessage->mAttribute->GetKey(), inMessage->mAttribute);
	
	}
}


// ---------------------------------------------------------------------------
//		* ValueChangedSelf										[protected]
// ---------------------------------------------------------------------------
//	An attribute of the object we're displaying has changed. Redraw or rebuild
//	the display pane as appropriate.

void
VEDrawingAgent::ValueChangedSelf(
	FourCharCode	/* inAttributeKey */,
	DMAttribute*	/* inAttribute */)
{
	// override hook
}


// ---------------------------------------------------------------------------
//		* SubPaneDeleted										[protected]
// ---------------------------------------------------------------------------
//	Called by RemoveDisplayObject to provide a notification to other
//	drawing agents (for superviews only) that this pane is being deleted.

void
VEDrawingAgent::SubPaneDeleted(
	LPane*	inSubPane)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mModelObject.GetObject());

	// First, see if the sub's deletion affects us.

	SubPaneDeletedSelf(inSubPane);
	
	// Okay, now pass notification along to drawing agent for
	// superelement (if there is one).

	VEModelObject* super = mModelObject->GetSuperModelObject();
	if (super == nil)
		return;
	ValidateObject_(super);

	VEDrawingAgent* superDA = super->FindDrawingAgent();
	if (superDA != nil) {
		ValidateObject_(superDA);
		superDA->SubPaneDeleted(inSubPane);
	}
}


// ---------------------------------------------------------------------------
//		* SubPaneDeletedSelf									[protected]
// ---------------------------------------------------------------------------
//	Override hook. Subclasses that maintain a pointer to contained panes
//	should override this to prevent dangling pointers.

void
VEDrawingAgent::SubPaneDeletedSelf(
	LPane*	/* inSubPane */)
{
	// override hook
}


// ===========================================================================

#pragma mark -
#pragma mark ** resource map notifications

// ---------------------------------------------------------------------------
//		* ListenToMessage										[protected]
// ---------------------------------------------------------------------------
//	Overriden to catch messages from the resource map. Calls ResourceChanged
//	if such a message is received.

void
VEDrawingAgent::ListenToMessage(
	MessageT		inMessage,
	void*			ioParam)
{

	// Validate pointers.
	
	ValidateThis_();

	// Dispatch message.

	switch (inMessage) {

		case Resource_Added:
		case Resource_Removed:
		case Resource_DataChanged: {
			RMMap_ChangeMessage* message = (RMMap_ChangeMessage*) ioParam;
			ThrowIfNil_(message);
			
			RMResource* resource = message->mResource;
			ResourceChanged(resource);
			break;
		}

		default:
			DMListener::ListenToMessage(inMessage, ioParam);

	}
}


// ---------------------------------------------------------------------------
//		* ResourceChanged										[protected]
// ---------------------------------------------------------------------------
//	A resource has been added, removed, or changed. See if it means anything
//	for our display pane.

void
VEDrawingAgent::ResourceChanged(
	RMResource*		/* inResource */)
{
	// override hook
}


// ---------------------------------------------------------------------------
//		* ListenToMap											[protected]
// ---------------------------------------------------------------------------
//	Override hook. Return true if drawing agent needs to listen to
//	resource map.

Boolean
VEDrawingAgent::ListenToMap()
{
	return false;
}


// ===========================================================================

#pragma mark -
#pragma mark ** internal accessors

// ---------------------------------------------------------------------------
//		* GetDisplayPane										[protected]
// ---------------------------------------------------------------------------
//	Internal accessor used by drawing agents and VELayoutView.
//	Get the PowerPlant pane that is used to display this object.

LPane*
VEDrawingAgent::GetDisplayPane() const
{

	// Validate pointers.
	
	ValidateThis_();

	// Return the pane.

	return mDisplayPane;
	
}


// ---------------------------------------------------------------------------
//		* CalcSelectionHandleRect								[protected]
// ---------------------------------------------------------------------------
//	Given the frame rectangle of the display object, calculate
//	the frame of one of the selection handles.

void
VEDrawingAgent::CalcSelectionHandleRect(
	const Rect&		inDisplayFrame,
	Rect&			outHandleFrame,
	Boolean			inBottomCorner,
	Boolean			inRightCorner) const
{

	const SInt16 handleSize = 5;

	// Validate pointers.
	
	ValidateThis_();

	// Figure handle frame for the specified corner.

	outHandleFrame = inDisplayFrame;
	
	if (inBottomCorner)
		outHandleFrame.top = outHandleFrame.bottom - handleSize;
	else
		outHandleFrame.bottom = outHandleFrame.top + handleSize;
	
	if (inRightCorner)
		outHandleFrame.left = outHandleFrame.right - handleSize;
	else
		outHandleFrame.right = outHandleFrame.left + handleSize;

}


// ===========================================================================

#pragma mark -
#pragma mark ** rebuild trigger

// ---------------------------------------------------------------------------
//		* RebuildMe												[protected]
// ---------------------------------------------------------------------------
//	Called by subclasses when a data model change means that the display
//	pane must be torn down and rebuilt.

void
VEDrawingAgent::RebuildMe()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mLayoutView);
	ValidateObject_(mModelObject.GetObject());

	// Ask the layout view to rebuild this display object.
	// This drawing agent will get deleted, so we have to cache
	// the local variables on the stack.
	
	VELayoutView* layoutView = mLayoutView;
	
	layoutView->BuildObjectDisplay(mModelObject);

}
