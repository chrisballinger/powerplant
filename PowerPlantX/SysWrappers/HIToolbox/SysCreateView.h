// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysCreateView.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:35 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		SysCreateView.h
	@brief		Wrapper functions for creating system view objects
*/

#ifndef H_SysCreateView
#define H_SysCreateView
#pragma once

#include <PPxPrefix.h>

namespace PPx {

// ===========================================================================
//	SusCreateView
/**
	Functions for creating system views */

namespace	SysCreateView {

	HIViewRef	BevelButton(
						CFStringRef						inTitle,
						ControlBevelThickness			inThickness,
						ControlBevelButtonBehavior		inButtonBehavior,
						const ControlButtonContentInfo&	inButtonContent,
						SInt16							inMenuID,
						ControlBevelButtonMenuBehavior	inMenuBehavior,
						ControlBevelButtonMenuPlacement	inMenuPlacement);

	HIViewRef	ChasingArrows();
	
	HIViewRef	CheckBox(
						CFStringRef		inTitle,
						SInt32			inInitialValue,
						bool			inAutoToggle);
						
	HIViewRef	CheckBoxGroupBox(
						CFStringRef		inTitle,
						SInt32			inInitialValue,
						bool			inIsPrimary,
						bool			inAutoToggle);
						
	HIViewRef	ClockControl(
						ControlClockType	inClockType,
						ControlClockFlags	inClockFlags);
	
	HIViewRef	ComboBox(
						const HIRect&				inBounds,
						CFStringRef					inDefaultText,
						const ControlFontStyleRec*	inStyle,
						CFArrayRef					inValueList,
						OptionBits					inAttributes);

	HIViewRef	DataBrowser( DataBrowserViewStyle inStyle );
						
	HIViewRef	DisclosureButton(
						SInt32			inInitialValue,
						bool			inAutoToggle);
						
	HIViewRef	DisclosureTriangle(
						ControlDisclosureTriangleOrientation inOrientation,
						CFStringRef		inTitle,
						SInt32			inInitialValue,
						bool			inDrawTitle,
						bool			inAutoToggle);
						
	HIViewRef	EditTextControl(
						CFStringRef		inText,
						bool			inIsPassword,
						bool			inUseInlineInput,
						const ControlFontStyleRec*	inStyle);
						
	HIViewRef	EditUnicodeText(
						CFStringRef					inText,
						bool						inIsPassword,
						const ControlFontStyleRec*	inStyle);
						
	HIViewRef	IconControl(
						const ControlButtonContentInfo&	inContent,
						bool							inDontTrack);
	
	HIViewRef	IconPushButton(
						CFStringRef						inTitle,
						const ControlButtonContentInfo&	inContent,
						ControlPushButtonIconAlignment	inAlignment);
	
	HIViewRef	ImageView( CGImageRef inImage );
						
	HIViewRef	ImageWell( const ControlButtonContentInfo& inContent );
						
	HIViewRef	ListBox(
						WindowRef			inWindow,
						const Rect&			inFrame,
						SInt16				inRowCount,
						SInt16				inColCount,
						SInt16				inCellHeight,
						SInt16				inCellWidth,
						bool				inHasGrowSpace,
						bool				inHorizScroll,
						bool				inVertScroll);
						
	HIViewRef	LittleArrows(
						SInt32			inInitialValue,
						SInt32			inMinValue,
						SInt32			inMaxValue,
						SInt32			inIncrement);
						
	HIViewRef	PictureControl(
						SInt16			inPictResID,
						PicHandle		inPictureHandle,
						bool			inDontTrack);
						
	HIViewRef	Placard();
	
	HIViewRef	PopupArrow(
						ControlPopupArrowOrientation	inOrientation,
						ControlPopupArrowSize			inArrowSize);
						
	HIViewRef	PopupButton(
						CFStringRef		inTitle,
						SInt32			inMenuID,
						bool			inHasVariableWidth,
						SInt16			inTitleWidth,
						SInt16			inTitleJust,
						Style			inTitleStyle);
						
	HIViewRef	PopupGroupBox(
						CFStringRef		inTitle,
						bool			inIsPrimary,
						SInt32			inMenuID,
						bool			inHasVariableWidth,
						SInt16			inTitleWidth,
						SInt16			inTitleJust,
						Style			inTitleStyle);
						
	HIViewRef	ProgressBar(
						SInt32			inInitialValue,
						SInt32			inMinValue,
						SInt32			inMaxValue,
						bool			inIsIndeterminate);
						
	HIViewRef	PushButton( CFStringRef inTitle );
						
	HIViewRef	RadioButton(
						CFStringRef		inTitle,
						SInt32			inInitialValue,
						bool			inAutoToggle);
						
	HIViewRef	RadioGroup();
						
	HIViewRef	RelevanceBar(
						SInt32			inInitialValue,
						SInt32			inMinValue,
						SInt32			inMaxValue);
						
	HIViewRef	RoundButton(
						ControlRoundButtonSize			inButtonSize,
						const ControlButtonContentInfo&	inContent);
						
	HIViewRef	ScrollBar(
						SInt32				inInitialValue,
						SInt32				inMinValue,
						SInt32				inMaxValue,
						SInt32				inViewSize,
						bool				inHasLiveTracking,
						ControlActionUPP	inLiveTrackingUPP);
						
	HIViewRef	ScrollView( OptionBits inOptions );
						
#if (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_3)

	HIViewRef	SearchField(
						OptionBits	inAttributes,
						MenuRef		inSearchMenu,
						CFStringRef	inDescriptiveText);

	HIViewRef	SegmentedView();
						
#endif // 10.3 or later

	HIViewRef	SeparatorLine();
						
	HIViewRef	Slider(
						SInt32						inInitialValue,
						SInt32						inMinValue,
						SInt32						inMaxValue,
						ControlSliderOrientation	inOrientation,
						UInt16						inTickMarksCount,
						bool						inHasLiveTracking,
						ControlActionUPP			inLiveTrackingUPP);
						
	HIViewRef	StaticText(
						CFStringRef					inText,
						const ControlFontStyleRec*	inStyle);
						
	HIViewRef	TabView(
						ControlTabSize			inTabSize,
						ControlTabDirection		inTabDirection,
						UInt16					inTabCount,
						const ControlTabEntry*	inTabEntries);
						
	HIViewRef	TextGroupBox(
						CFStringRef		inTitle,
						bool			inIsPrimary);
						
#if (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_3)

	HIViewRef	TextView(
						OptionBits		inOptions,
						TXNFrameOptions	inFrameOptions);
						
#endif // 10.3 or later

	HIViewRef	UserPane( UInt32 inFeatures );
						
	HIViewRef	WindowHeader( bool inIsListHeader );

} // namespace SysCreateView

} // namespace PPx

#endif	// H_SysCreateView

	
