// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPLWindow.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/14/96
//     $Date: 2006/04/12 08:48:30 $
//  $History: VPLWindow.cpp $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/19/97   Time: 19:54
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Improved commenting.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:49
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Fixed CR/LF problem
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/19/96   Time: 17:44
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Improved handling of window title changes.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/18/96   Time: 20:32
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added support for layout backdrops.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 14:22
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Added drawing agent.
//	
// ===========================================================================

#include "VPLWindow.h"

	// Core : Data model : Attributes
#include "DMIntegerAttribute.h"

	// Core : Editors : Views : User interface
#include "VELayoutView.h"

	// MacOS : Editors : Views : Data model
#include "VPDataModel.h"

	// MacOS : Editors : Views : Layout backdrops
#include "VPDocumentWindowBackdrop.h"
#include "VPFloatSideBackdrop.h"
#include "VPFloatTopBackdrop.h"
#include "VPMovableModalBackdrop.h"
#include "VPNormalDialogBackdrop.h"
#include "VPWindowBackdrop.h"


// ===========================================================================

#pragma mark *** VPLWindow ***

// ---------------------------------------------------------------------------
//		* CreateFromStream
// ---------------------------------------------------------------------------
//	Create a view that will mimic the window's content area size.

LPane*
VPLWindow::CreateFromStream(
	LStream*	/* inStream */)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mModelObject.GetObject());
	
	// Update layout kind.

	UpdateLayoutKind();

	// Set up pane and window parameter blocks.
	
	SPaneInfo paneInfo = { 0, 0, 0, true, true, { false, false, false, false }, 0, 0, 0, (LView*) -1 };
	SViewInfo viewInfo = { {0, 0}, {0, 0}, {0, 0}, false };

	// Read window's height and width.

	DMIntegerAttribute* widthAttr = mModelObject->FindIntegerAttribute(pPaneWidth);
	DMIntegerAttribute* heightAttr = mModelObject->FindIntegerAttribute(pPaneHeight);
	
	SInt32 width = widthAttr->GetIntValue();
	SInt32 height = heightAttr->GetIntValue();

	viewInfo.imageSize.width = paneInfo.width = width;
	viewInfo.imageSize.height = paneInfo.height = height;

	// Create a view to stand in for the window.

	return new VPFWindow(paneInfo, viewInfo);

}


// ---------------------------------------------------------------------------
//		* ValueChangedSelf										[protected]
// ---------------------------------------------------------------------------
//	An attribute of this model object has changed. Update the display
//	accordingly.

void
VPLWindow::ValueChangedSelf(
	FourCharCode	inAttributeKey,
	DMAttribute*	inAttribute)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mDisplayPane);
	ValidateObject_(mModelObject.GetObject());
	ValidateObject_(inAttribute);
	
	// Dispatch attribute key.

	switch (inAttributeKey) {

		// Window kind: Replace the layout backdrop object.

#if PP_Target_Carbon
		case pPaneWindowCloseBox:
#endif
		case pPaneWindowProc:
			UpdateLayoutKind();
			break;
		
		// Window title: Ignore it. The layout backdrop will catch this change.
		//	(We override here to silence the LPane::SetDescriptor signal.)
		
		case pPaneWindowTitle:
			break;

		// Otherwise, see if LView wants it.

		default:
			VPLView::ValueChangedSelf(inAttributeKey, inAttribute);

	}
}


// ---------------------------------------------------------------------------
//		* UpdateLayoutKind										[protected]
// ---------------------------------------------------------------------------
//	Make sure the window display matches the window kind we currently have.

void
VPLWindow::UpdateLayoutKind()
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mLayoutView);

	// Read window's kind attribute.

	DMIntegerAttribute* wdefAttr = mModelObject->FindIntegerAttribute(pPaneWindowProc);
	ValidateObject_(wdefAttr);
	
	SInt32 wdef = wdefAttr->GetIntValue();
	
	// Choose a backdrop based on window kind.
	
	switch (wdef) {

		// document windows

		case zoomNoGrow:
		case noGrowDocProc:
		case documentProc:
		case zoomDocProc:
			mLayoutView->SetLayoutBackdrop(new VPDocumentWindowBackdrop(wdef));
			break;

		// dialog windows

		case movableDBoxProc:
			mLayoutView->SetLayoutBackdrop(new VPMovableModalBackdrop);
			break;
		case dBoxProc:
			mLayoutView->SetLayoutBackdrop(new VPNormalDialogBackdrop);
			break;

		// floating windows (title bar at top)

		case floatZoomProc:
		case floatProc:
		case floatGrowProc:
		case floatZoomGrowProc:
			mLayoutView->SetLayoutBackdrop(new VPFloatTopBackdrop);
			break;

		// floating windows (title bar at side)

		case floatSideProc:
		case floatSideGrowProc:
		case floatSideZoomProc:
		case floatSideZoomGrowProc:
			mLayoutView->SetLayoutBackdrop(new VPFloatSideBackdrop);
			break;

		// other: just use regular backdrop, but paint content area as per 'wctb'

//		case plainDBox:				//! TEMPORARY: not implemented yet...
//		case altDBoxProc:
//		case rDocProc:

		default:
			mLayoutView->SetLayoutBackdrop(new VPWindowBackdrop);

	}
}



// ===========================================================================

#pragma mark -
#pragma mark *** VPFWindow ***

// ---------------------------------------------------------------------------
//		* VPFWindow(...)
// ---------------------------------------------------------------------------
//	Constructor

VPFWindow::VPFWindow(
	const SPaneInfo&	inPaneInfo,
	const SViewInfo&	inViewInfo)

: LView(inPaneInfo, inViewInfo)

{
}



// ---------------------------------------------------------------------------
//		* ~VPFWindow
// ---------------------------------------------------------------------------
//	Destructor

VPFWindow::~VPFWindow()
{
}


// ---------------------------------------------------------------------------
//		* ExpandSubPane
// ---------------------------------------------------------------------------
//	Overriden to ensure that the "fake window" view has the same expand
//	sub pane behavior as the real LWindow.

void
VPFWindow::ExpandSubPane(
	LPane*		inSub,
	Boolean		inExpandHoriz,
	Boolean		inExpandVert)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inSub);

	//* 2.2a1: BUG FIX #625: stole code from LWindow

	SDimension16 subSize;
	inSub->GetFrameSize(subSize);

	SPoint32 subLocation;
	inSub->GetFrameLocation(subLocation);
	
	// Subpane overlaps 1-pixel window border.

	if (inExpandHoriz) {
		subSize.width = mFrameSize.width + 2;
		subLocation.h = -1;
	}
	else
		subLocation.h -= mFrameLocation.h;
	
	if (inExpandVert) {
		subSize.height = mFrameSize.height + 2;
		subLocation.v = -1;
	}
	else
		subLocation.v -= mFrameLocation.v;
	
	inSub->PlaceInSuperFrameAt(subLocation.h, subLocation.v, false);
	inSub->ResizeFrameTo(subSize.width, subSize.height, false);

}