// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysCreateView.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:35 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
//
//	There are Toolbox calls for creating each standard system view. The
//	creator functions have parameters for the most common options and
//	return a HIViewRef.
//
//	The creator functions for the classic Appearance controls have not
//	been updated to be HIView-savvy. They require a window parameter and
//	a bounds in QuickDraw coordinates. For these functions, we pass nil
//	for the window and a 1x1 pixel bounds. The PPx view classes put the
//	view within the proper superview and location afterwards.

#include <SysCreateView.h>
#include <SysWindow.h>
#include <Carbon/Carbon.h>

namespace PPx {

namespace {

	// -----------------------------------------------------------------------
	//	Classic Appearance controls take a QuickDraw rectangle for the
	//	bounds. However, HIViews use a HIRect bounds. We use a 1x1 pixel
	//	bounds. Client code should set the bounds appropriately afterwards.

	Rect		rect_DefaultBounds = { 0, 0, 1, 1 };
	HIRect		hirect_DefaultBounds = { 0, 0, 1, 1 };
}


// ---------------------------------------------------------------------------
//
// 	$$ OS Bug Workaround $$ Needed for OS 10.2.x
//
//	Passing nil for the window parameter does not work for certain controls.
//	This is an OS bug documented in Technical Q&A QA1211 on Apple's developer
//	web site. The affected controls are: StaticText, RoundButton, Placard,
//	IconControl, PictureControl, PopupArrow, and PopupButton.
//
//	As a workaround, we use a scratch window (invisible window managed by
//	the SysWindow class). The bug is fixed in OS 10.3.x.

namespace Workarounds {

// ---------------------------------------------------------------------------
//	Function Prototypes

bool		CreateViewInNilWindowFails();

WindowRef	GetWindowForCreateView();


// ---------------------------------------------------------------------------
//	CreateViewInNilWindowFails
//
//	Returns whether creating certain controls in a nil window fails.

bool
CreateViewInNilWindowFails()
{
	static bool	callChecked = false;		// Use static flags since we only
	static bool	callFails;					//   need to check the call once
	
	if (not callChecked) {
	
			// The OS bug exists if trying to create a static text
			// control in a nil window results in a paramErr. Testing
			// shows that if this fails, then all the other affected
			// controls mentioned above will also fail. There's no need
			// to test them all.

		HIViewRef	viewRef = nil;
		OSStatus	status  = ::CreateStaticTextControl(
									nil, &rect_DefaultBounds,
									nil, nil, &viewRef);
									
		if (viewRef != nil) {
			::CFRelease(viewRef);
		}
									
		callFails   = (status == paramErr);
		callChecked = true;
	}
								
	return callFails;
}


// ---------------------------------------------------------------------------
//	GetWindowForCreateView
//
//	Returns a WindowRef to use when creating a view. If bug exists, use
//	a scratch window. Otherwise, use a nil window.

WindowRef
GetWindowForCreateView()
{
	if (CreateViewInNilWindowFails()) {
		return SysWindow::GetScratchWindow();
	}
	
	return nil;
}

} // namespace Workarounds

#pragma mark -

// ---------------------------------------------------------------------------
//	BevelButton
/**
	Creates a system bevel button
	
	@param	inTitle				Text title for button
	@param	inThickness			Thickness of the beveled edges
	@param	inButtonBehavior	How button behaves when clicked
	@param	inButtonContent		Kind of button content
	@param	inMenuID			Menu ID for popup menu
	@param	inMenuBehavior		How menu behaves when item is selected
	@param	inMenuPlacement		Placement of menu glyph within button
	
	@return	HIViewRef for bevel button										*/

HIViewRef
SysCreateView::BevelButton(
	CFStringRef						inTitle,
	ControlBevelThickness			inThickness,
	ControlBevelButtonBehavior		inButtonBehavior,
	const ControlButtonContentInfo&	inButtonContent,
	SInt16							inMenuID,
	ControlBevelButtonMenuBehavior	inMenuBehavior,
	ControlBevelButtonMenuPlacement	inMenuPlacement)
{
	HIViewRef	viewRef;
	OSStatus	status = ::CreateBevelButtonControl(
								nil, &rect_DefaultBounds, inTitle,
								inThickness, inButtonBehavior,
								const_cast<ControlButtonContentInfo*>
									(&inButtonContent),
								inMenuID, inMenuBehavior, inMenuPlacement,
								&viewRef);
								
	PPx_ThrowIfOSError_(status, "CreateBevelButtonControl failed");
	
	return viewRef;
}


// ---------------------------------------------------------------------------
//	ChasingArrows
/**
	Creates a system chasing arrows indicator
	
	@return	HIViewRef for chasing arrows									*/

HIViewRef
SysCreateView::ChasingArrows()
{
	HIViewRef	viewRef;
	OSStatus	status = ::CreateChasingArrowsControl(
								nil, &rect_DefaultBounds,  &viewRef);
							
	PPx_ThrowIfOSError_(status, "CreateChasingArrowsControl failed");
	
	return viewRef;
}


// ---------------------------------------------------------------------------
//	CheckBox
/**
	Creates a system check box control
	
	@param	inTitle			Title of check box
	@param	inInitialValue	State of check box (0 = unchecked, 1 = checked,
								2 = mixed)
	@param	inAutoToggle	Whether box is checked/unchecked automatically
								when clicked
	
	@return	HIViewRef for check box											*/

HIViewRef
SysCreateView::CheckBox(
	CFStringRef	inTitle,
	SInt32		inInitialValue,
	bool		inAutoToggle)
{
	HIViewRef	viewRef;
	OSStatus	status = ::CreateCheckBoxControl(
								nil, &rect_DefaultBounds, inTitle,
								inInitialValue, inAutoToggle, &viewRef);
							
	PPx_ThrowIfOSError_(status, "CreateCheckBoxControl failed");
	
	return viewRef;
}


// ---------------------------------------------------------------------------
//	CheckBoxGroupBox
/**
	Creates a system check box group box
	
	@param	inTitle			Title of check box
	@param	inInitialValue	State of check box (0 = unchecked, 1 = checked,
								2 = mixed)
	@param	inIsPrimary		Group box kind (true = primary, false = secondary)
	@param	inAutoToggle	Whether box is checked/unchecked automatically
								when clicked								*/

HIViewRef
SysCreateView::CheckBoxGroupBox(
	CFStringRef	inTitle,
	SInt32		inInitialValue,
	bool		inIsPrimary,
	bool		inAutoToggle)
{
	HIViewRef	viewRef;
	OSStatus	status = ::CreateCheckGroupBoxControl(
								nil, &rect_DefaultBounds, inTitle,
								inInitialValue, inIsPrimary, inAutoToggle,
								&viewRef);
							
	PPx_ThrowIfOSError_(status, "CreateCheckGroupBoxControl failed");
	
	return viewRef;
}


// ---------------------------------------------------------------------------
//	ClockControl
/**
	Creates a system clock control
	
	@param	inClockType		Kind of clock (time or date)
	@param	inClockFlags	Clock options
	
	@return	HIView for clock control										*/

HIViewRef
SysCreateView::ClockControl(
	ControlClockType	inClockType,
	ControlClockFlags	inClockFlags)
{
	HIViewRef	viewRef;
	OSStatus	status = ::CreateClockControl(
								nil, &rect_DefaultBounds, inClockType,
								inClockFlags, &viewRef);
							
	PPx_ThrowIfOSError_(status, "CreateClockControl failed");
	
	return viewRef;
}


// ---------------------------------------------------------------------------
//	ComboBox
/**
	Creates a system combo box, which combines an edit field and list of values
	
	@param	inBounds		Bounding box
	@param	inDefaultText	Initial text in the edit field
	@param	inStyle			Text style
	@param	inValueList		List of value to diplay as choices
	@param	inAttributes	Option flags
	
	@return HIViewRef for combo box											*/

HIViewRef
SysCreateView::ComboBox(
	const HIRect&				inBounds,
	CFStringRef					inDefaultText,
	const ControlFontStyleRec*	inStyle,
	CFArrayRef					inValueList,
	OptionBits					inAttributes)
{
	HIViewRef	viewRef;
	OSStatus	status = ::HIComboBoxCreate(
								&inBounds, inDefaultText, inStyle,
								inValueList, inAttributes, &viewRef);
							
	PPx_ThrowIfOSError_(status, "HIComboBoxCreate failed");
	
	return viewRef;
}


// ---------------------------------------------------------------------------
//	DataBrowser
/**
	Creates a system data browswer
	
	@param	inStyle			kDataBrowserListView or kDataBrowserColumnView
	
	@return HIViewRef for data browser										*/

HIViewRef
SysCreateView::DataBrowser(
	DataBrowserViewStyle	inStyle)
{
	HIViewRef	viewRef;
	OSStatus	status = ::CreateDataBrowserControl(
								nil, &rect_DefaultBounds, inStyle, &viewRef);
							
	PPx_ThrowIfOSError_(status, "CreateDataBrowserControl failed");
	
	return viewRef;
}


// ---------------------------------------------------------------------------
//	DisclosureButton
/**
	Creates a system disclosure button
	
	@param	inInitialValue		kControlDisclosureButtonClosed or
									kControlDisclosureButtonDisclosed
	@param	inAutoToggle		Whether button automatically toggles
									state when clicked
									
	@return HIViewRef for disclosure button									*/

HIViewRef
SysCreateView::DisclosureButton(
	SInt32		inInitialValue,
	bool		inAutoToggle)
{
	HIViewRef	viewRef;
	OSStatus	status = ::CreateDisclosureButtonControl(
								nil, &rect_DefaultBounds, inInitialValue,
								inAutoToggle, &viewRef);
							
	PPx_ThrowIfOSError_(status, "CreateDisclosureButtonControl failed");
	
	return viewRef;
}


// ---------------------------------------------------------------------------
//	DisclosureTriangle
/**
	Creates a system disclosur triangle
	
	@param	inOrientation	Direction triangle points when closed
	@param	inTitle			Title for disclosure triangle
	@param	inInitialValue	0 = closed, 1 = open
	@param	inDrawTitle		Whether to draw the title
	@param	inAutoToggle	Whether the triangle automaticallys toggles
								beteeen open/closed when clicked
								
	@return HIViewRef for disclosure triangle								*/

HIViewRef
SysCreateView::DisclosureTriangle(
	ControlDisclosureTriangleOrientation inOrientation,
	CFStringRef		inTitle,
	SInt32			inInitialValue,
	bool			inDrawTitle,
	bool			inAutoToggle)
{
	HIViewRef	viewRef;
	OSStatus	status = ::CreateDisclosureTriangleControl(
								nil, &rect_DefaultBounds, inOrientation,
								inTitle, inInitialValue, inDrawTitle,
								inAutoToggle, &viewRef);
							
	PPx_ThrowIfOSError_(status, "CreateDisclosureTriangleControl failed");
	
	return viewRef;
}


// ---------------------------------------------------------------------------
//	EditTextControl
/**
	Creates a system edit text control
	
	@param	inText				Initial text in edit field
	@param	inIsPassword		Whether the field is a for password
	@param	inUseInlineInput	Whether to use inline input
	@param	inStyle				Text style
	
	@return HIViewRef for edit text control									*/

HIViewRef
SysCreateView::EditTextControl(
	CFStringRef		inText,
	bool			inIsPassword,
	bool			inUseInlineInput,
	const ControlFontStyleRec*	inStyle)
{
	HIViewRef	viewRef;
	OSStatus	status = ::CreateEditTextControl(
								nil, &rect_DefaultBounds, inText,
								inIsPassword, inUseInlineInput, inStyle,
								&viewRef);
							
	PPx_ThrowIfOSError_(status, "CreateEditTextControl failed");
	
	return viewRef;
}


// ---------------------------------------------------------------------------
//	EditUnicodeText
/**
	Creates a system edit unicode text control
	
	@param	inText			Initial text in edit field
	@param	inIsPassword	Whether the field is for a password
	@param	inStyle			Font style for the text
	
	@return HIViewRef for edit unicode text control							*/

HIViewRef
SysCreateView::EditUnicodeText(
	CFStringRef		inText,
	bool			inIsPassword,
	const ControlFontStyleRec*	inStyle)
{
	HIViewRef	viewRef;
	OSStatus	status = ::CreateEditUnicodeTextControl(
							nil, &rect_DefaultBounds, inText, inIsPassword,
							inStyle, &viewRef);
							
	PPx_ThrowIfOSError_(status, "CreateEditUnicodeTextControl failed");
	
	return viewRef;
}


// ---------------------------------------------------------------------------
//	IconControl
/**
	Creates a system icon control
	
	@param	inContent		Content of icon control
	@param	inDontTrack		Whether to not track mouse downs in the control
	
	@return HIViewRef for icon control										*/

HIViewRef
SysCreateView::IconControl(
	const ControlButtonContentInfo&	inContent,
	bool							inDontTrack)
{
	HIViewRef	viewRef;
	OSStatus	status = ::CreateIconControl(
								Workarounds::GetWindowForCreateView(),
								&rect_DefaultBounds, &inContent,
								inDontTrack, &viewRef);
							
	PPx_ThrowIfOSError_(status, "CreateIconControl failed");
	
	return viewRef;
}


// ---------------------------------------------------------------------------
//	IconPushButton
/**
	Creates a system icon push button control
	
	@param	inTitle			Title for button
	@param	inContent		Content of icon push button
	@param	inAlignment		Alignement of icon in the button
	
	@return	HIViewRef for icon push button									*/

HIViewRef
SysCreateView::IconPushButton(
	CFStringRef						inTitle,
	const ControlButtonContentInfo&	inContent,
	ControlPushButtonIconAlignment	inAlignment)
{
		// $$ OS Bug Workaround $$ Current OS (10.3.3)
		//
		// The ControlButtonContentInfo* parameter should be const, as
		// the struct is not changed by creating the icon control.
		// The PPx API uses a const reference, but we cast away the
		// const when calling the Toolbox function.

	HIViewRef	viewRef;
	OSStatus	status = ::CreatePushButtonWithIconControl(
								nil, &rect_DefaultBounds, inTitle,
								const_cast<ControlButtonContentInfo*>
									(&inContent),
								inAlignment, &viewRef);
							
	PPx_ThrowIfOSError_(status, "CreatePushButtonWithIconControl failed");
	
	return viewRef;
}


// ---------------------------------------------------------------------------
//	ImageView
/**
	Creates a system image view
	
	@param	inImage		CGImage to display
	
	@return HIViewRef for image view										*/

HIViewRef
SysCreateView::ImageView(
	CGImageRef	inImage)
{
	HIViewRef	viewRef;
	OSStatus	status = ::HIImageViewCreate(inImage, &viewRef);
							
	PPx_ThrowIfOSError_(status, "HIImageViewCreate failed");
	
	return viewRef;
}


// ---------------------------------------------------------------------------
//	ImageWell
/**
	Creates a system image well
	
	@param	inContent	Content of image well
	
	@return	HIViewRef for image well										*/

HIViewRef
SysCreateView::ImageWell(
	const ControlButtonContentInfo&	inContent)
{
	HIViewRef	viewRef;
	OSStatus	status = ::CreateImageWellControl(
								nil, &rect_DefaultBounds, &inContent,
								&viewRef);
							
	PPx_ThrowIfOSError_(status, "CreateImageWellControl failed");
	
	return viewRef;
}


// ---------------------------------------------------------------------------
//	ListBox
/**
	Creates a system list box
	
	@param	inWindow		Window in which to place the list box
	@param	inFrame			Size and location of the list box
	@param	inRowCount		Number of rows
	@param	inColCount		Number of columns
	@param	inCellHeight	Pixel height of cells. Pass 0 for default height.
	@param	inCellWidth		Pixel widht of cells. Pass 0 for default width.
	@param	inHasGrowSpace	Whether to leave space for a grow box
	@param	inHorizScroll	Whether list box has a horizontal scroll bar
	@param	inVertScroll	Whether list box has a vertical scroll bar
	
	@return HIViewRef for list box
	
	@note A list box must be installed in a valid window without the compositing
	attribute. This is a limitation of the system list box control.				*/

HIViewRef
SysCreateView::ListBox(
	WindowRef			inWindow,
	const Rect&			inFrame,
	SInt16				inRowCount,
	SInt16				inColCount,
	SInt16				inCellHeight,
	SInt16				inCellWidth,
	bool				inHasGrowSpace,
	bool				inHorizScroll,
	bool				inVertScroll)
{
	SysWindow	window(inWindow);
	if (window.GetWindowAttributes() & kWindowCompositingAttribute) {
		PPx_SignalString_("List Box does not work properly in a composited window");
	}
	
		// Always make a standard text list. You can change the LDEF afterwards
		// by using the kControlListBoxLDEFTag.
	
	ListDefSpec	listDef;
	listDef.defType = kListDefStandardTextType;
	
		// Always pass false for the "auto size" parameter. We want to
		// specify the frame size.
		
	HIViewRef	viewRef;
	OSStatus	status = ::CreateListBoxControl(
								inWindow, &inFrame, false,
								inRowCount, inColCount, inHorizScroll,
								inVertScroll, inCellHeight, inCellWidth,
								inHasGrowSpace, &listDef, &viewRef);

	PPx_ThrowIfOSError_(status, "CreateListBoxControl failed");
	
	return viewRef;
}


// ---------------------------------------------------------------------------
//	LittleArrows
/**
	Creates a system little arrows control
	
	@param	inInitialValue	Initial value of control
	@param	inMinValue		Minimum value of control
	@param	inMaxValue		Maximum value fo control
	@param	inIncrement		Amout to increment/decrement value when clicked

	@return HIViewRef for little arrows control								*/

HIViewRef
SysCreateView::LittleArrows(
	SInt32			inInitialValue,
	SInt32			inMinValue,
	SInt32			inMaxValue,
	SInt32			inIncrement)
{
	HIViewRef	viewRef;
	OSStatus	status = ::CreateLittleArrowsControl(
								nil, &rect_DefaultBounds, inInitialValue,
								inMinValue, inMaxValue, inIncrement,
								&viewRef);
							
	PPx_ThrowIfOSError_(status, "CreateLittleArrowsControl failed");
	
	return viewRef;
}


// ---------------------------------------------------------------------------
//	PictureControl
/**
	Creates a system picture control
	
	@param	inPictResID			PICT resource ID of picture to display
	@param	inPictureHandle		Handle to picture to dispaly
	@param	inDontTrack			Whether not to track mouse downs
	
	@return HIViewRef for picture control							*/

HIViewRef
SysCreateView::PictureControl(
	SInt16			inPictResID,
	PicHandle		inPictureHandle,
	bool			inDontTrack)
{
	ControlButtonContentInfo	content;
	
	if (inPictResID != 0) {
		content.contentType = kControlContentPictRes;
		content.u.resID = inPictResID;
		
	} else {
		content.contentType = kControlContentPictHandle;
		content.u.picture = inPictureHandle;
	}

	HIViewRef	viewRef;
	OSStatus	status = ::CreatePictureControl(
								Workarounds::GetWindowForCreateView(),
								&rect_DefaultBounds, &content,
								inDontTrack, &viewRef);
							
	PPx_ThrowIfOSError_(status, "CreatePictureControl failed");
	
	return viewRef;
}


// ---------------------------------------------------------------------------
//	Placard
/**
	Creates a system placard
	
	@return HIViewRef for placard											*/

HIViewRef
SysCreateView::Placard()
{
	HIViewRef	viewRef;
	OSStatus	status = ::CreatePlacardControl(
								Workarounds::GetWindowForCreateView(),
								&rect_DefaultBounds, &viewRef);
							
	PPx_ThrowIfOSError_(status, "CreatePlacardControl failed");
	
	return viewRef;
}


// ---------------------------------------------------------------------------
//	PopupArrow
/**
	Creates a system popup arrow control
	
	@param	inOrientation	Direction arrow points (north, east, south, west)
	@param	inArrowSize		Size of arrow (normal or small)
	
	@return HIViewRef for popup arrow										*/

HIViewRef
SysCreateView::PopupArrow(
	ControlPopupArrowOrientation	inOrientation,
	ControlPopupArrowSize			inArrowSize)
{
	HIViewRef	viewRef;
	OSStatus	status = ::CreatePopupArrowControl(
								Workarounds::GetWindowForCreateView(),
								&rect_DefaultBounds, inOrientation,
								inArrowSize, &viewRef);
							
	PPx_ThrowIfOSError_(status, "CreatePopupArrowControl failed");
	
	return viewRef;
}


// ---------------------------------------------------------------------------
//	PopupButton
/**
	Creates a system popup button
	
	@param	inTitle				Title for popup
	@param	inMenuID			Menu ID for popup
	@param	inHasVariableWidth	Whether the menu has variable width
	@param	inTitleWidth		Width of title (use -1 for variable width)
	@param	inTitleJust			Justification of title text
	@param	inTitleStyle		Font style for title	
	
	@return HIViewRef for popup button										*/

HIViewRef
SysCreateView::PopupButton(
	CFStringRef		inTitle,
	SInt32			inMenuID,
	bool			inHasVariableWidth,
	SInt16			inTitleWidth,
	SInt16			inTitleJust,
	Style			inTitleStyle)
{
	HIViewRef	viewRef;
	OSStatus	status = ::CreatePopupButtonControl(
								Workarounds::GetWindowForCreateView(),
								&rect_DefaultBounds, inTitle, inMenuID,
								inHasVariableWidth, inTitleWidth,
								inTitleJust, inTitleStyle,
								&viewRef);
							
	PPx_ThrowIfOSError_(status, "CreatePopupButtonControl failed");
	
	return viewRef;
}


// ---------------------------------------------------------------------------
//	PopupGroupBox
/**
	Creates a system popup group box
	
	@param	inTitle				Title for popup
	@param	inIsPrimary			Group box kind (true = primary, false = secondary)
	@param	inMenuID			Menu ID for popup
	@param	inHasVariableWidth	Whether the menu has variable width
	@param	inTitleWidth		Width of title (use -1 for variable width)
	@param	inTitleJust			Justification of title text
	@param	inTitleStyle		Font style for title	
	
	@return HIViewRef for popup group box									*/

HIViewRef
SysCreateView::PopupGroupBox(
	CFStringRef		inTitle,
	bool			inIsPrimary,
	SInt32			inMenuID,
	bool			inHasVariableWidth,
	SInt16			inTitleWidth,
	SInt16			inTitleJust,
	Style			inTitleStyle)
{
	HIViewRef	viewRef;
	OSStatus	status = ::CreatePopupGroupBoxControl(
								nil, &rect_DefaultBounds, inTitle,
								inIsPrimary, inMenuID, inHasVariableWidth,
								inTitleWidth, inTitleJust,
								inTitleStyle, &viewRef);
							
	PPx_ThrowIfOSError_(status, "CreatePopupGroupBoxControl failed");
	
	return viewRef;
}


// ---------------------------------------------------------------------------
//	ProgressBar
/**
	Creates a system jprogress bar
	
	@param	inInitialValue		Initial value of control
	@param	inMinValue			Minimum value of control
	@param	inMaxValue			Maximum value fo control
	@param	inIsIndeterminate	Whether progress bar is indeterminate
	
	@return HIViewRef for progress bar
	
	An indeterminate progress bar just animates and has no value			*/

HIViewRef
SysCreateView::ProgressBar(
	SInt32			inInitialValue,
	SInt32			inMinValue,
	SInt32			inMaxValue,
	bool			inIsIndeterminate)
{
	HIViewRef	viewRef;

	OSStatus	status = ::CreateProgressBarControl(
								nil, &rect_DefaultBounds, inInitialValue,
								inMinValue, inMaxValue, inIsIndeterminate,
								&viewRef);
							
	PPx_ThrowIfOSError_(status, "CreateProgressBarControl failed");
	
	return viewRef;
}


// ---------------------------------------------------------------------------
//	PushButton
/**
	Creates a system push button
	
	@param	inTitle		Title of push button
	
	@return HIViewRef for push button										*/

HIViewRef
SysCreateView::PushButton(
	CFStringRef		inTitle)
{
	HIViewRef	viewRef;
	OSStatus	status = ::CreatePushButtonControl(
								nil, &rect_DefaultBounds, inTitle, &viewRef);
							
	PPx_ThrowIfOSError_(status, "CreatePushButtonControl failed");
	
	return viewRef;
}


// ---------------------------------------------------------------------------
//	RadioButton
/**
	Creates a system radio button
	
	@param	inTitle			Title of radio button
	@param	inInitialValue	0 = off, 1 = on
	@param	inAutoToggle	Whether button automatically toggles when clicked
	
	@return HIViewRef for radio button										*/

HIViewRef
SysCreateView::RadioButton(
	CFStringRef		inTitle,
	SInt32			inInitialValue,
	bool			inAutoToggle)
{
	HIViewRef	viewRef;
	OSStatus	status = ::CreateRadioButtonControl(
								nil, &rect_DefaultBounds, inTitle,
								inInitialValue, inAutoToggle, &viewRef);
							
	PPx_ThrowIfOSError_(status, "CreateRadioButtonControl failed");
	
	return viewRef;
}


// ---------------------------------------------------------------------------
//	RadioGroup
/**
	Creates a system radio group
	
	@return HIViewRef for radio group
	
	All views inside a radio group must be radio buttons					*/

HIViewRef
SysCreateView::RadioGroup()
{
	HIViewRef	viewRef;
	OSStatus	status = ::CreateRadioGroupControl(
								nil, &rect_DefaultBounds, &viewRef);
							
	PPx_ThrowIfOSError_(status, "CreateRadioGroupControl failed");
	
	return viewRef;
}


// ---------------------------------------------------------------------------
//	RelevanceBar
/**
	Creates a system relevance bar
	
	@param	inInitialValue		Initial value of control
	@param	inMinValue			Minimum value of control
	@param	inMaxValue			Maximum value fo control
	
	@return HIViewRef for relevance bar										*/

HIViewRef
SysCreateView::RelevanceBar(
	SInt32			inInitialValue,
	SInt32			inMinValue,
	SInt32			inMaxValue)
{
	HIViewRef	viewRef;
	OSStatus	status = ::CreateRelevanceBarControl(
								nil, &rect_DefaultBounds, inInitialValue,
								inMinValue, inMaxValue,
								&viewRef);
							
	PPx_ThrowIfOSError_(status, "CreateRelevanceBarControl failed");
	
	return viewRef;
}


// ---------------------------------------------------------------------------
//	RoundButton
/**
	Creates a system round button
	
	@param	inButtonSize		Size of round button (large or small)
	@param	inContent			Content of round button
	
	@return HIViewRef for round button										*/

HIViewRef
SysCreateView::RoundButton(
	ControlRoundButtonSize			inButtonSize,
	const ControlButtonContentInfo&	inContent)
{
		// $OS Bug Workaround$ Current OS (10.2.6)
		//
		// The ControlButtonContentInfo* parameter should be const, as
		// the struct is not changed by creating the icon control.
		// The PPx API uses a const reference, but we cast away the
		// const when calling the Toolbox function.

	HIViewRef	viewRef;
	OSStatus	status = ::CreateRoundButtonControl(
								Workarounds::GetWindowForCreateView(),
								&rect_DefaultBounds, inButtonSize,
								const_cast<ControlButtonContentInfo*>
									(&inContent),
								&viewRef);
							
	PPx_ThrowIfOSError_(status, "CreateRoundButtonControl failed");
	
	return viewRef;
}


// ---------------------------------------------------------------------------
//	ScrollBar
/**
	Creates a system scroll bar
	
	@param	inInitialValue		Initial value of control
	@param	inMinValue			Minimum value of control
	@param	inMaxValue			Maximum value fo control
	@param	inViewSize			Size of view being scrolled
	@param	inHasLiveTracking	Whether content scrolls as thumb is dragged
	@param	inLiveTrackingUPP	Callback function for handling live tracking
	
	@return HIViewRef for scroll bar										*/

HIViewRef
SysCreateView::ScrollBar(
	SInt32				inInitialValue,
	SInt32				inMinValue,
	SInt32				inMaxValue,
	SInt32				inViewSize,
	bool				inHasLiveTracking,
	ControlActionUPP	inLiveTrackingUPP)
{
	HIViewRef	viewRef;
	OSStatus	status = ::CreateScrollBarControl(
								nil, &rect_DefaultBounds, inInitialValue,
								inMinValue, inMaxValue, inViewSize,
								inHasLiveTracking, inLiveTrackingUPP,
								&viewRef);
							
	PPx_ThrowIfOSError_(status, "CreateScrollBarControl failed");
	
	return viewRef;
}


// ---------------------------------------------------------------------------
//	ScrollView
/**
	Creates a system scroll view
	
	@param	inOptions		Options for scroll view
	
	@return HIViewRef for scroll view										*/

HIViewRef
SysCreateView::ScrollView(
	OptionBits	inOptions)
{
	HIViewRef	viewRef;
	OSStatus	status = ::HIScrollViewCreate(inOptions, &viewRef);
							
	PPx_ThrowIfOSError_(status, "HIScrollViewCreate failed");
	
	return viewRef;
}


#if (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_3)

// ---------------------------------------------------------------------------
//	SearchField
/**
	Creates a system search field
	
	@param	inAttributes		Initial attributes of the search field
	@param	inSearchMenu		Menu to be associated with this search field
	@param	inDescriptiveText	Text to be displayed in the text field when
								the field does not have focus and contains
								no user entered text
	
	@return HIViewRef for search field										*/

HIViewRef
SysCreateView::SearchField(
	OptionBits	inAttributes,
	MenuRef		inSearchMenu,
	CFStringRef	inDescriptiveText)
{
	HIViewRef	viewRef;
	OSStatus	status = ::HISearchFieldCreate(
								&hirect_DefaultBounds, inAttributes,
								inSearchMenu, inDescriptiveText, &viewRef);
								
	PPx_ThrowIfOSError_(status, "HISearchFieldCreate failed");
	
	return viewRef;
}


// ---------------------------------------------------------------------------
//	SegmentedView
/**
	Creates a system segmented view
	
	@return HIViewRef for segmented view									*/

HIViewRef
SysCreateView::SegmentedView()
{
	HIViewRef	viewRef;
	OSStatus	status = ::HISegmentedViewCreate(
								&hirect_DefaultBounds, &viewRef);
							
	PPx_ThrowIfOSError_(status, "HISegmentedViewCreate failed");
	
	return viewRef;
}

#endif // 10.3 or later


// ---------------------------------------------------------------------------
//	SeparatorLine
/**
	Creates a system separator line
	
	@return HIViewRef for separator line									*/

HIViewRef
SysCreateView::SeparatorLine()
{
	HIViewRef	viewRef;
	OSStatus	status = ::CreateSeparatorControl(
								nil, &rect_DefaultBounds, &viewRef);
							
	PPx_ThrowIfOSError_(status, "CreateSeparatorControl failed");
	
	return viewRef;
}


// ---------------------------------------------------------------------------
//	Slider
/**
	Creates a system slider control
	
	@param	inInitialValue		Initial value of control
	@param	inMinValue			Minimum value of control
	@param	inMaxValue			Maximum value fo control
	@param	inOrientation		Orientation of thumb
	@param	inTickMarksCount	Number of tick marks to draw
	@param	inHasLiveTracking	Whether slider live tracks the thumb
	@param	inLiveTrackingUPP	Function for handling live tracking
	
	@return HIViewRef for slider											*/

HIViewRef
SysCreateView::Slider(
	SInt32						inInitialValue,
	SInt32						inMinValue,
	SInt32						inMaxValue,
	ControlSliderOrientation	inOrientation,
	UInt16						inTickMarksCount,
	bool						inHasLiveTracking,
	ControlActionUPP			inLiveTrackingUPP)
{
	HIViewRef	viewRef;
	OSStatus	status = ::CreateSliderControl(
								nil, &rect_DefaultBounds,
								inInitialValue, inMinValue, inMaxValue,
								inOrientation, inTickMarksCount,
								inHasLiveTracking, inLiveTrackingUPP, &viewRef );
								
	PPx_ThrowIfOSError_(status, "CreateSliderControl failed");
	
	return viewRef;
}


// ---------------------------------------------------------------------------
//	StaticText
/**
	Creates a system static text control
	
	@param	inText		Text to display
	@param	inStyle		Text style
	
	@return HIViewRef for static text										*/

HIViewRef
SysCreateView::StaticText(
	CFStringRef					inText,
	const ControlFontStyleRec*	inStyle)
{
	HIViewRef	viewRef;
	OSStatus	status = ::CreateStaticTextControl(
								Workarounds::GetWindowForCreateView(),
								&rect_DefaultBounds, inText, inStyle,
								&viewRef);
							
	PPx_ThrowIfOSError_(status, "CreateStaticTextControl failed");
	
	return viewRef;
}


// ---------------------------------------------------------------------------
//	TabView
/**
	Creates a system tab view
	
	@param	inTabSize			Size of tabs (normal or small)
	@param	inTabDirection		Dirction of tabs (north, south, east, west)
	@param	inTabCount			Number of tabs
	@param	inTabEntries		Data for each tab
	
	@return HIViewRef for tab view
*/

HIViewRef
SysCreateView::TabView(
	ControlTabSize			inTabSize,
	ControlTabDirection		inTabDirection,
	UInt16					inTabCount,
	const ControlTabEntry*	inTabEntries)
{
	HIViewRef	viewRef;
	OSStatus	status = ::CreateTabsControl(
								nil, &rect_DefaultBounds, inTabSize,
								inTabDirection, inTabCount, inTabEntries,
								&viewRef);
							
	PPx_ThrowIfOSError_(status, "CreateTabViewControl failed");
	
	return viewRef;
}


// ---------------------------------------------------------------------------
//	TextGroupBox
/**
	Creates a system text group box
	
	@param	inTitle			Title for text group box
	@param	inIsPrimary		Group box kind (true = primary, false = secondary)
	
	@return HIViewRef for text group box
*/

HIViewRef
SysCreateView::TextGroupBox(
	CFStringRef		inTitle,
	bool			inIsPrimary)
{
	HIViewRef	viewRef;
	OSStatus	status = ::CreateGroupBoxControl(
								nil, &rect_DefaultBounds, inTitle,
								inIsPrimary, &viewRef);
							
	PPx_ThrowIfOSError_(status, "CreateGroupBoxControl failed");
	
	return viewRef;
}


#if (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_3)

// ---------------------------------------------------------------------------
//	TextView
/**
	Creates a system text view
	
	@param	inOptions		Any options desired for the view
	@param	inFrameOptions	Any frame options desired for the TXN object creation
	
	@return HIViewRef for text view
*/

HIViewRef
SysCreateView::TextView(
	OptionBits		inOptions,
	TXNFrameOptions	inFrameOptions)
{
	HIViewRef	viewRef;
	OSStatus	status = ::HITextViewCreate(
								&hirect_DefaultBounds, inOptions,
								inFrameOptions, &viewRef);
							
	PPx_ThrowIfOSError_(status, "HITextViewCreate failed");
	
	return viewRef;
}

#endif // 10.3 or later


// ---------------------------------------------------------------------------
//	UserPane
/**
	Creates a system user pane
	
	@param	inFeatures		Control feature bits
	
	@return HIViewRef for user pane
*/

HIViewRef
SysCreateView::UserPane( UInt32 inFeatures )
{
	HIViewRef	viewRef;
	OSStatus	status = ::CreateUserPaneControl(
								nil, &rect_DefaultBounds, inFeatures,
								&viewRef);
							
	PPx_ThrowIfOSError_(status, "CreateUserPaneControl failed");
	
	return viewRef;
}


// ---------------------------------------------------------------------------
//	WindowHeader
/**
	Creates a system window header
	
	@param	inIsListHeader	Whether it's a list header, which as not bottom line
	
	@return HIViewRef for window header
*/

HIViewRef
SysCreateView::WindowHeader(
	bool		inIsListHeader)
{
	HIViewRef	viewRef;
	OSStatus	status = ::CreateWindowHeaderControl(
								nil, &rect_DefaultBounds, inIsListHeader,
								&viewRef );
							
	PPx_ThrowIfOSError_(status, "CreateWindowHeaderControl failed");
	
	return viewRef;
}


} // namespace PPx
