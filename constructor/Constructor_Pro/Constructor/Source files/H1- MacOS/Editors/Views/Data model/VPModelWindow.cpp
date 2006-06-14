// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPModelWindow.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/14/96
//     $Date: 2006/01/18 01:33:56 $
//  $History: VPModelWindow.cpp $
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:39
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Improved commenting.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:45
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Fixed CR/LF problem
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 12/08/96   Time: 15:51
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Fixed some bugs with the close box flag.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/22/96   Time: 16:03
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Finished implementation of window kind/flags relationship
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/20/96   Time: 20:42
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Now inherits from VPModelView instead of VPModelPane.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/17/96   Time: 11:06
//	Created in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added class.
//	
// ===========================================================================

#include "VPModelWindow.h"

	// MacOS : Editors : Views : Data model
#include "VPDataModel.h"


// ===========================================================================

#pragma mark *** VPModelWindow ***

// ---------------------------------------------------------------------------
//		* VPModelWindow()
// ---------------------------------------------------------------------------
//	Default constructor

VPModelWindow::VPModelWindow()
{
	mSuppressSizeUpdates = false;
	mSuppressFlagUpdates = false;
	mSuppressKindUpdates = false;
}


// ---------------------------------------------------------------------------
//		* VPModelWindow(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. No additional stream data is read.

VPModelWindow::VPModelWindow(
	LStream*	inStream)

: VPModelView(inStream)

{
	mSuppressSizeUpdates = false;
	mSuppressFlagUpdates = false;
	mSuppressKindUpdates = false;
}


// ---------------------------------------------------------------------------
//		* VPModelWindow(VPModelWindow&)
// ---------------------------------------------------------------------------
//	Copy constructor

VPModelWindow::VPModelWindow(
	const VPModelWindow&	inOriginal)

: VPModelView(inOriginal)

{
	mSuppressSizeUpdates = false;
	mSuppressFlagUpdates = false;
	mSuppressKindUpdates = false;
}


// ---------------------------------------------------------------------------
//		* ~VPModelWindow
// ---------------------------------------------------------------------------
//	Destructor

VPModelWindow::~VPModelWindow()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** raw data streaming

// ---------------------------------------------------------------------------
//		* ReadStreamData
// ---------------------------------------------------------------------------
//	Overriden to disable internal data consistency checks while streaming in.
//	The consistency check is done at the end instead.

void
VPModelWindow::ReadStreamData(
	LStream*		inStream,
	FourCharCode	inStreamSelector)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Disallow size and window flag updates while reading data.

	StValueChanger<Boolean> noSizeUpdates(mSuppressSizeUpdates, true);
	StValueChanger<Boolean> noKindUpdates(mSuppressKindUpdates, true);
	StValueChanger<Boolean> noFlagUpdates(mSuppressFlagUpdates, true);

	// Read stream data normally.

	VPModelView::ReadStreamData(inStream, inStreamSelector);
	
	// Now make sure width/height fields are consistent.
	
	if (inStreamSelector == 'WIND') {
		UpdateSizeFromBounds();
		UpdateWindowFlagsFromKind(false);
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** update notifications

// ---------------------------------------------------------------------------
//		* SendUpdateMessage										[protected]
// ---------------------------------------------------------------------------
//	Overriden to ensure that size attributes and window kind attributes
//	are internally consistent.

void
VPModelWindow::SendUpdateMessage(
	MessageT		inMessage,
	void*			ioParam,
	DMElement*		inSubElement)
{

	// Validate pointers.

	ValidateThis_();

	// Intercept change messages from resource ID, attribute, and name.

	if (inMessage == Attribute_ValueChange) {
		
		// Make sure it's this object that's affected.
		
		DM_ValueChange* valueMsg = (DM_ValueChange*) ioParam;
		if (valueMsg->mObject == this) {
		
			ValidateObject_(valueMsg->mAttribute);
		
			// It is. Check attribute key.
			
			switch (valueMsg->mAttribute->GetKey()) {

				case pPaneWindowProc:
					if (!mSuppressKindUpdates)
						UpdateWindowFlagsFromKind(true);
					break;
				
				case pPaneWindowSizeBox:
				case pPaneWindowZoomable:
					if (!mSuppressFlagUpdates)
						UpdateWindowKindFromFlags();
					break;
		
				case pPaneWindowTop:
				case pPaneWindowLeft:
				case pPaneHeight:
				case pPaneWidth:
					if (!mSuppressSizeUpdates)
						UpdateBoundsFromSize();
					break;
				
				case pPaneWindowCloseBox:
					if (!mSuppressFlagUpdates) {
						DMIntegerAttribute* flagsCloseBox = FindIntegerAttribute(pPaneWindowCloseBox);
						DMIntegerAttribute* wmgrCloseBox = FindIntegerAttribute(pPaneWindowMgrCloseBox);
						wmgrCloseBox->SetIntValue(flagsCloseBox->GetIntValue());
					}
					break;
				
				case pPaneWindowContentColor:
					if (!mSuppressFlagUpdates) {
						DMIntegerAttribute* hasWCTB = FindIntegerAttribute(pPaneWindowHasWCTB);
						hasWCTB->SetIntValue(1);
					}
					break;

			}
		}	
	}

	// Relay message as usual.

	VPModelView::SendUpdateMessage(inMessage, ioParam, inSubElement);

}


// ===========================================================================

#pragma mark -
#pragma mark ** internal data model consistency

// ---------------------------------------------------------------------------
//		* UpdateSizeFromBounds									[protected]
// ---------------------------------------------------------------------------
//	One of the bounds attributes (left, top, right, bottom) has changed.
//	Update the width and height attributes to match.

void
VPModelWindow::UpdateSizeFromBounds()
{
	
	// Validate pointers.
	
	ValidateThis_();

	// Look for bounds and size attributes. FindIntegerAttribute validates
	// the pointers before returning them, so there's no need
	// to revalidate.

	DMIntegerAttribute* left = FindIntegerAttribute(pPaneWindowLeft);
	DMIntegerAttribute* right = FindIntegerAttribute(pPaneWindowRight);
	DMIntegerAttribute* top = FindIntegerAttribute(pPaneWindowTop);
	DMIntegerAttribute* bottom = FindIntegerAttribute(pPaneWindowBottom);

	DMIntegerAttribute* width = FindIntegerAttribute(pPaneWidth);
	DMIntegerAttribute* height = FindIntegerAttribute(pPaneHeight);
	
	// Update the width & height attributes to match bounds attributes.
	
	width->SetIntValue(right->GetIntValue() - left->GetIntValue());
	height->SetIntValue(bottom->GetIntValue() - top->GetIntValue());
	
}


// ---------------------------------------------------------------------------
//		* UpdateBoundsFromSize									[protected]
// ---------------------------------------------------------------------------
//	One of the size attributes (width, height) has changed. Update the
//	bounds attributes (right, bottom) to match.

void
VPModelWindow::UpdateBoundsFromSize()
{

	// Look for bounds and size attributes. FindIntegerAttribute validates
	// the pointers before returning them, so there's no need
	// to revalidate.

	DMIntegerAttribute* left = FindIntegerAttribute(pPaneWindowLeft);
	DMIntegerAttribute* right = FindIntegerAttribute(pPaneWindowRight);
	DMIntegerAttribute* top = FindIntegerAttribute(pPaneWindowTop);
	DMIntegerAttribute* bottom = FindIntegerAttribute(pPaneWindowBottom);

	DMIntegerAttribute* width = FindIntegerAttribute(pPaneWidth);
	DMIntegerAttribute* height = FindIntegerAttribute(pPaneHeight);
	
	// Update the right & bottom attributes to match size attributes.
	
	right->SetIntValue(left->GetIntValue() + width->GetIntValue());
	bottom->SetIntValue(top->GetIntValue() + height->GetIntValue());
	
}


// ---------------------------------------------------------------------------
//		* UpdateWindowFlagsFromKind								[protected]
// ---------------------------------------------------------------------------
//	One of the window flags (close box, size box, etc.) has changed. Update 
//	the window kind to match it.

void
VPModelWindow::UpdateWindowFlagsFromKind(
	Boolean		inUpdateCloseBox)
{

	// Validate pointers.
	
	ValidateThis_();

	// Look for window kind and flag attributes. FindIntegerAttribute
	// validates the pointers before returning them, so there's no need
	// to revalidate.

	DMIntegerAttribute* wdefAttr = FindIntegerAttribute(pPaneWindowProc);
	SInt32 wdef = wdefAttr->GetIntValue();
	
	DMIntegerAttribute* close = FindIntegerAttribute(pPaneWindowCloseBox);
	DMIntegerAttribute* title = FindIntegerAttribute(pPaneWindowTitleBar);
	DMIntegerAttribute* resize = FindIntegerAttribute(pPaneWindowResizable);
	DMIntegerAttribute* size = FindIntegerAttribute(pPaneWindowSizeBox);
	DMIntegerAttribute* zoom = FindIntegerAttribute(pPaneWindowZoomable);

	// Prevent recursion by disabling UpdateWindowKindFromFlags.

	StValueChanger<Boolean> noUpdates(mSuppressSizeUpdates, true);

	// Dispatch by window kind.
	
	switch (wdef) {

		// Regular document windows (and all others):

		case noGrowDocProc:			// z g
		case documentProc:			// z G
		case zoomNoGrow:			// Z g
		case zoomDocProc:			// Z G
			close->Enable();
			title->Disable();
			resize->Enable();
			size->Enable();
			zoom->Enable();
			
			if (inUpdateCloseBox)
				close->SetIntValue(1);
			title->SetIntValue(1);
			resize->SetIntValue(wdef == documentProc || wdef == zoomDocProc);
			size->SetIntValue(wdef == documentProc || wdef == zoomDocProc);
			zoom->SetIntValue(wdef == zoomNoGrow || wdef == zoomDocProc);
			break;

		// Movable modal dialog:

		case movableDBoxProc:
			close->Disable();
			title->Disable();
			resize->Disable();
			size->Disable();
			zoom->Disable();
			
			close->SetIntValue(0);
			title->SetIntValue(1);
			resize->SetIntValue(0);
			size->SetIntValue(0);
			zoom->SetIntValue(0);
			break;
		
		// Regular modal dialog:
		
		case dBoxProc:
		case plainDBox:
		case altDBoxProc:
			close->Disable();
			title->Disable();
			resize->Disable();
			size->Disable();
			zoom->Disable();
			
			close->SetIntValue(0);
			title->SetIntValue(0);
			resize->SetIntValue(0);
			size->SetIntValue(0);
			zoom->SetIntValue(0);
			break;

		// Round document window:

#if CALL_NOT_IN_CARBON || UNIVERSAL_INTERFACES_VERSION < 0x0337
		case rDocProc:
#else
		case 16:
			// icw -- rDocProc isn't available in Carbon
#endif
			close->Enable();
			title->Disable();
			resize->Disable();
			size->Disable();
			zoom->Enable();
			
			if (inUpdateCloseBox)
				close->SetIntValue(1);
			title->SetIntValue(1);
			resize->SetIntValue(0);
			size->SetIntValue(0);
			zoom->SetIntValue(0);
			break;
		
		// Floating window (top title bar):

		case floatProc:					// z g
		case floatGrowProc:				// z G
		case floatZoomProc:				// Z g
		case floatZoomGrowProc:			// Z G
			close->Enable();
			title->Enable();
			resize->Enable();
			size->Enable();
			zoom->Enable();
			
			if (inUpdateCloseBox)
				close->SetIntValue(1);
			title->SetIntValue(1);
			resize->SetIntValue(wdef == floatGrowProc || wdef == floatZoomGrowProc);
			size->SetIntValue(wdef == floatGrowProc || wdef == floatZoomGrowProc);
			zoom->SetIntValue(wdef == floatZoomGrowProc || wdef == floatZoomProc);
			break;

		// Floating window (side title bar):

		case floatSideProc:				// z g
		case floatSideGrowProc:			// z G
		case floatSideZoomProc:			// Z g
		case floatSideZoomGrowProc:		// Z G
			close->Enable();
			title->Enable();
			resize->Enable();
			size->Enable();
			zoom->Enable();
			
			if (inUpdateCloseBox)
				close->SetIntValue(1);
			title->SetIntValue(1);
			resize->SetIntValue(wdef == floatSideGrowProc || wdef == floatSideZoomGrowProc);
			size->SetIntValue(wdef == floatSideGrowProc || wdef == floatSideZoomGrowProc);
			zoom->SetIntValue(wdef == floatSideZoomGrowProc || wdef == floatSideZoomProc);
			break;

		// Unknown:
		
		default:
			close->Enable();
			title->Enable();
			resize->Enable();
			size->Enable();
			zoom->Enable();

	}
}


// ---------------------------------------------------------------------------
//		* UpdateWindowKindFromFlags								[protected]
// ---------------------------------------------------------------------------
//	The window kind attribute has changed. Update the window flags (close box,
//	size box, etc.) to match.

void
VPModelWindow::UpdateWindowKindFromFlags()
{

	// Validate pointers.
	
	ValidateThis_();

	// Look for window kind and flag attributes. FindIntegerAttribute
	// validates the pointers before returning them, so there's no need
	// to revalidate.

	DMIntegerAttribute* wdefAttr = FindIntegerAttribute(pPaneWindowProc);
	SInt32 wdef = wdefAttr->GetIntValue();
	
	DMIntegerAttribute* sizeAttr = FindIntegerAttribute(pPaneWindowSizeBox);
	DMIntegerAttribute* zoomAttr = FindIntegerAttribute(pPaneWindowZoomable);
	
	SInt32 size = sizeAttr->GetIntValue();
	SInt32 zoom = zoomAttr->GetIntValue();

	// Prevent recursion by disabling UpdateWindowFlagsFromKind.

	StValueChanger<Boolean> noUpdates(mSuppressKindUpdates, true);

	// Dispatch by window kind.
	
	switch (wdef) {

		// Regular document window:

		case noGrowDocProc:			// z g
		case documentProc:			// z G
		case zoomNoGrow:			// Z g
		case zoomDocProc:			// Z G
			if (zoom)
				wdefAttr->SetIntValue(size ? zoomDocProc : zoomNoGrow);
			else
				wdefAttr->SetIntValue(size ? documentProc : noGrowDocProc);
			break;

		// Floating windows (side title):

		case floatSideProc:			// z g
		case floatSideGrowProc:		// z G
		case floatSideZoomProc:		// Z g
		case floatSideZoomGrowProc:	// Z G
			if (zoom)
				wdefAttr->SetIntValue(size ? floatSideZoomGrowProc : floatSideZoomProc);
			else
				wdefAttr->SetIntValue(size ? floatSideGrowProc : floatSideProc);
			break;

		// Floating windows (top title):

		case floatProc:				// z g
		case floatGrowProc:			// z G
		case floatZoomProc:			// Z g
		case floatZoomGrowProc:		// Z G
			if (zoom)
				wdefAttr->SetIntValue(size ? floatZoomGrowProc : floatZoomProc);
			else
				wdefAttr->SetIntValue(size ? floatGrowProc : floatProc);
			break;

	}
}