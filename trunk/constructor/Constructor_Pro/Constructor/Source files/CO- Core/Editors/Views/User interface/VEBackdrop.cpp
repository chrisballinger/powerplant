// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VEBackdrop.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/15/96
//     $Date: 2006/01/18 01:33:15 $
//	$History: VEBackdrop.cpp $
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 02/20/97   Time: 18:27
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Improved commenting.
//	
//	*****************  Version 5  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:07p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:30
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Fixed CR/LF problem
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/20/96   Time: 14:28
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Added CalcLocalBackdropFrame method.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/17/96   Time: 21:44
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Merged with VEBasicBackdrop class.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/16/96   Time: 23:11
//	Created in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Converted VEBackdrop from a pure abstract class to an LPane.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "VEBackdrop.h"

	// Core : Editors : Views : User interface
#include "VEDrawingAgent.h"

	// PowerPlant : GA : Grayscale utilities
#include <UGAColorRamp.h>


// ===========================================================================

#pragma mark *** VEBackdrop ***

// ---------------------------------------------------------------------------
//		* VEBackdrop()
// ---------------------------------------------------------------------------
//	Constructor

VEBackdrop::VEBackdrop()
{
	SetRefreshAllWhenResized(false);
}


// ---------------------------------------------------------------------------
//		* ~VEBackdrop
// ---------------------------------------------------------------------------
//	Destructor

VEBackdrop::~VEBackdrop()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** backdrop drawing

// ---------------------------------------------------------------------------
//		* DrawSelf
// ---------------------------------------------------------------------------
//	Draw a simple gray border around the root object.

void
VEBackdrop::DrawSelf()
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Frame the root pane.
	
	Rect rootFrame = { 0, 0, 0, 0 };
	if (CalcLocalFrameForRootObject(rootFrame)) {
		::InsetRect(&rootFrame, -1, -1);
		RGBColor theColor = UGAColorRamp::GetColor(9);
		::RGBForeColor(&theColor);
		::FrameRect(&rootFrame);
		::InsetRect(&rootFrame, 1, 1);
	}

	// Shade the remaining area.

	ShadeDesktopArea(rootFrame);

}


// ===========================================================================

#pragma mark -
#pragma mark ** backdrop location

// ---------------------------------------------------------------------------
//		* CalcLocalFrameForRootObject
// ---------------------------------------------------------------------------
//	Find the frame of the root object, in local coordinates of the
//	backdrop pane.

Boolean
VEBackdrop::CalcLocalFrameForRootObject(
	Rect&	outFrameRect)
{
	
	// Validate pointers.
	
	ValidateThis_();
	
	// First make sure there is a root object.
	
	if (mRootObject != nil) {
	
		// There is, make sure it has a drawing agent.
		
		ValidateObject_(mRootObject.GetObject());
		VEDrawingAgent* rootAgent = mRootObject->FindDrawingAgent();
		if (rootAgent != nil) {
		
			// There is an agent, see if it can give us a frame.
			
			ValidateObject_(rootAgent);
			if (rootAgent->CalcPortFrameRect(outFrameRect)) {
				
				PortToLocalPoint(topLeft(outFrameRect));
				PortToLocalPoint(botRight(outFrameRect));
				return true;
			
			}
		}
	}

	// Nope. Root object isn't visible.
	
	return false;
	
}


// ---------------------------------------------------------------------------
//		* CalcLocalBackdropFrame
// ---------------------------------------------------------------------------
//	Find the frame of the window simulation, in local coordinates of the
//	backdrop pane.

Boolean
VEBackdrop::CalcLocalBackdropFrame(
	Rect&	outFrameRect)
{

	// Validate pointers.
	
	ValidateThis_();

	// For simple backdrop, the backdrop frame
	// includes only the root object plus the one-pixel
	// border beyond it.

	if (!CalcLocalFrameForRootObject(outFrameRect))
		return false;
	
	::InsetRect(&outFrameRect, -1, -1);
	return true;
	
}


// ---------------------------------------------------------------------------
//		* GetBackdropOverscan
// ---------------------------------------------------------------------------
//	Return the preferred size of the gap between the root object and the
//	edge of the layout view.

void
VEBackdrop::GetBackdropOverscan(
	SDimension32&	outTopLeft,
	SDimension32&	outBotRight)
{
	outTopLeft.width = 16;
	outTopLeft.height = 16;
	outBotRight.width = 16;
	outBotRight.height = 16;
}


// ===========================================================================

#pragma mark -
#pragma mark ** configuration

// ---------------------------------------------------------------------------
//		* SetRootObject											[protected]
// ---------------------------------------------------------------------------
//	Generally called by VELayoutView::SetLayoutBackdrop. Informs the backdrop
//	what object is the root object in the display.

void
VEBackdrop::SetRootObject(
	VEModelObject*	inRootObject)
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inRootObject);
	
	// Remember pointer to listener.
	
	mRootObject = inRootObject;
	mRootObject->AddListener(this);
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** update notifications

// ---------------------------------------------------------------------------
//		* ValueChanged											[protected]
// ---------------------------------------------------------------------------
//	An attribute's value has changed. Determine if this attribute is
//	related to the root object in the display. If so, call
//	RootAttributeChanged to see if this attribute should trigger a redraw.

void
VEBackdrop::ValueChanged(
	DM_ValueChange*		inMessage)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mRootObject.GetObject());
	ThrowIfNil_(inMessage);
	ValidateObject_(inMessage->mAttribute);

	// See if this message relates to the root object in the display.
	
	if (inMessage->mObject == mRootObject.GetObject())
		RootAttributeChanged(inMessage->mAttribute);

}


// ---------------------------------------------------------------------------
//		* RootAttributeChanged									[protected]
// ---------------------------------------------------------------------------
//	An attribute of the root object has changed. See if this attribute
//	is a size attribute (i.e. width or height). If so, redraw the display.

void
VEBackdrop::RootAttributeChanged(
	DMAttribute*	inAttribute)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mRootObject.GetObject());
	ValidateObject_(inAttribute);

	// If the root object has resized, refresh the full display.
	
	if (mRootObject->IsSizeAttribute(inAttribute->GetKey()))
		Refresh();
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** drawing helpers

// ---------------------------------------------------------------------------
//		* ShadeDesktopArea										[protected]
// ---------------------------------------------------------------------------
//	Fill in the area outside of the root frame (i.e. inRootFrameRect)
//	with a light gray color.

void
VEBackdrop::ShadeDesktopArea(
	const Rect&		inRootFrameRect)
{

	// Validate pointers.
	
	ValidateThis_();

	// Find the backdrop region.
	
	RgnHandle deskRgn = MakeBackdropRegion(inRootFrameRect);

	// Fill the background (desktop) region.
	
	RGBColor theColor = UGAColorRamp::GetColor(3);
	::RGBBackColor(&theColor);
	::EraseRgn(deskRgn);
	
	// Release memory.
	
	::DisposeRgn(deskRgn);

}


// ---------------------------------------------------------------------------
//		* MakeBackdropRegion									[protected]
// ---------------------------------------------------------------------------
//	Create a region that represents the area between the edge of the 
//	root object and the edge of the layout view.

RgnHandle
VEBackdrop::MakeBackdropRegion(
	const Rect&		inWindowFrameArea)
{

	// Validate pointers.
	
	ValidateThis_();

	// Make sure we can get the necessary regions.
	
	RgnHandle deskRgn = ::NewRgn();
#if PP_Target_Carbon
	ThrowIfNil_(deskRgn);
#else
	ValidateHandle_((Handle) deskRgn);
#endif
	
	RgnHandle rootRgn = ::NewRgn();
#if PP_Target_Carbon
	ThrowIfNil_(rootRgn);
#else
	ValidateHandle_((Handle) rootRgn);
#endif

	// Build a region representing the entire backdrop area.
	
	Rect localFrame;
	CalcLocalFrameRect(localFrame);
	::RectRgn(deskRgn, &localFrame);

	// Build a region for the cutout (root pane)
	// and cut it out of the backdrop area.

	Rect baseFrame = inWindowFrameArea;
	if (baseFrame.right > baseFrame.left) {
		::InsetRect(&baseFrame, -1, -1);
		::RectRgn(rootRgn, &baseFrame);
		::DiffRgn(deskRgn, rootRgn, deskRgn);
	}
	
	// Release temporary region.
	
	::DisposeRgn(rootRgn);

	// Return the desktop region.
	
	return deskRgn;

}
