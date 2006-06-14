// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxNibDecoderFuncs.cp
//
//	Copyright © 2004-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:23 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxNibDecoderFuncs.h>
#include <PPxNibReader.h>
#include <PPxNibDecoderUtils.h>
#include <PPxNibConstants.h>
#include <PPxNibRegistrar.h>

#include <PPxBevelButton.h>
#include <PPxChasingArrows.h>
#include <PPxCheckBox.h>
#include <PPxCheckBoxGroupBox.h>
#include <PPxClockControl.h>
#include <PPxComboBox.h>
#include <PPxDataBrowserView.h>
#include <PPxDisclosureTriangle.h>
#include <PPxEditTextControl.h>
#include <PPxEditUnicodeText.h>
#include <PPxIconControl.h>
#include <PPxIconPushButton.h>
#include <PPxImageView.h>
#include <PPxImageWell.h>
#include <PPxLittleArrows.h>
#include <PPxPictureControl.h>
#include <PPxPopupArrow.h>
#include <PPxPopupButton.h>
#include <PPxPopupGroupBox.h>
#include <PPxProgressBar.h>
#include <PPxPushButton.h>
#include <PPxRadioButton.h>
#include <PPxRadioGroup.h>
#include <PPxRelevanceBar.h>
#include <PPxRoundButton.h>
#include <PPxScrollBar.h>
#include <PPxScrollView.h>
#include <PPxSearchField.h>
#include <PPxSegmentedView.h>
#include <PPxSeparatorLine.h>
#include <PPxSlider.h>
#include <PPxStaticText.h>
#include <PPxTabView.h>
#include <PPxTextGroupBox.h>
#include <PPxTextView.h>
#include <PPxUserPane.h>
#include <PPxXMLDecoder.h>

#include <SysCFArray.h>
#include <SysCFXMLNode.h>
#include <SysIcon.h>


namespace PPx {

// ---------------------------------------------------------------------------
//	CreateBevelButton
/**
	+++
	
	@param	inSuperView		+++
	@param	inTree			+++
	
	@return	Pointer to newly created object									*/

Persistent*
NibDecoderFuncs::CreateBevelButton(
	View*				inSuperView,
	const CFXMLTree&	inTree)
{
	NibReader reader(inTree);
	
	HIRect frame;
	bool isVisible;
	bool isEnabled;
	reader.ReadCommonValues(inSuperView, frame, isVisible, isEnabled);

	CFString title("");
	reader.ReadOptionalString(NibConstants::tag_title, title);

	ControlBevelThickness thickness = kControlBevelButtonSmallBevel;
	reader.ReadOptionalInt(NibConstants::tag_bevelThickness, thickness);
	
	ControlBevelButtonBehavior behavior = kControlBehaviorPushbutton;
	reader.ReadOptionalInt(NibConstants::tag_behavior, behavior);
	
	ControlButtonContentInfo contentInfo;
	contentInfo.contentType = kControlContentCIconRes;
	reader.ReadOptionalInt(NibConstants::tag_contentType, contentInfo.contentType);
	contentInfo.u.resID = 1;
	reader.ReadOptionalInt(NibConstants::tag_contentResID, contentInfo.u.resID);
	
	SInt16 menuID = -12345;
	
	ControlBevelButtonMenuBehavior menuBehavior = kControlBehaviorSingleValueMenu;
	reader.ReadOptionalInt(NibConstants::tag_menuBehavior, menuBehavior);
	
	ControlBevelButtonMenuPlacement menuPlacement = kControlBevelButtonMenuOnBottom;
	reader.ReadOptionalInt(NibConstants::tag_menuPlacement, menuPlacement);
	
	std::auto_ptr<BevelButton> view(new BevelButton);

	view->Initialize(inSuperView, frame, isVisible, isEnabled, title, thickness,
		behavior, contentInfo, menuID, menuBehavior, menuPlacement);

	bool isRounded = false;
	reader.ReadOptionalBool(NibConstants::tag_rounded, isRounded);
	if (isRounded)
	{
		ThemeButtonKind buttonKind = kThemeRoundedBevelButton;
		view->SetDataTag(kControlEntireControl, kControlBevelButtonKindTag,
			sizeof(buttonKind), &buttonKind);
	}
	
	ControlButtonTextPlacement textPlacement = kControlBevelButtonPlaceNormally;
	reader.ReadOptionalInt(NibConstants::tag_textPlacement, textPlacement);
	view->SetTextPlacement(textPlacement);
	
	ControlButtonGraphicAlignment graphicAlignment = kControlBevelButtonAlignCenter;
	reader.ReadOptionalInt(NibConstants::tag_graphicAlignment, graphicAlignment);
	view->SetGraphicAlignment(graphicAlignment);
	
	Point graphicOffset = { 0, 0 };
	reader.ReadOptionalInt(NibConstants::tag_graphicXOffset, graphicOffset.h);
	reader.ReadOptionalInt(NibConstants::tag_graphicYOffset, graphicOffset.v);
	view->SetGraphicOffset(graphicOffset);

	SInt16 textOffset = 0;
	reader.ReadOptionalInt(NibConstants::tag_textOffset, textOffset);
	view->SetTextOffset(textOffset);

	ControlButtonTextAlignment textAlignment = kControlBevelButtonAlignCenter;
	if (reader.ReadOptionalInt(NibConstants::tag_textAlignment, textAlignment))
		view->SetTextAlignment(textAlignment);

	SInt32 selectedItem = 1;
	reader.ReadOptionalInt(NibConstants::tag_selectedItem, selectedItem);

	MenuRef menu = NULL;
	if (reader.ReadOptionalMenu(NibConstants::tag_menu, menu) and ::IsValidMenu(menu))
	{
		view->SetMenuRef(menu);
		if (selectedItem != 1)
			view->SetMenuValue(selectedItem);
	}

	reader.ReadOptionalControlInfo(view.get());
	reader.ReadOptionalLayoutInfo(view.get());
	reader.ReadOptionalHelpInfo(view.get());
	reader.ReadOptionalSubViews(view.get());
	
	view->FinishInitPersistent();
	
	return view.release();
}


// ---------------------------------------------------------------------------
//	CreateButton
/**
	+++
	
	@param	inSuperView		+++
	@param	inTree			+++
	
	@return	Pointer to newly created object									*/

Persistent*
NibDecoderFuncs::CreateButton(
	View*				inSuperView,
	const CFXMLTree&	inTree)
{
	NibReader reader(inTree);

	HIRect frame;
	bool isVisible;
	bool isEnabled;
	reader.ReadCommonValues(inSuperView, frame, isVisible, isEnabled);

	CFString title("");
	reader.ReadOptionalString(NibConstants::tag_title, title);
	
	SInt32 buttonType = 0;
	reader.ReadOptionalInt(NibConstants::tag_buttonType, buttonType);

	ControlPushButtonIconAlignment alignment = kControlPushButtonIconOnLeft;
	reader.ReadOptionalInt(NibConstants::tag_iconPosition, alignment);

	ControlButtonContentInfo contentInfo;
	contentInfo.contentType = kControlContentTextOnly;
	reader.ReadOptionalInt(NibConstants::tag_contentType, contentInfo.contentType);
	if ((contentInfo.contentType == kControlContentIconSuiteRes) or
		(contentInfo.contentType == kControlContentCIconRes) or
		(contentInfo.contentType == kControlContentPictRes) or
		(contentInfo.contentType == kControlContentICONRes))
	{
		contentInfo.u.resID = 0;
		reader.ReadOptionalInt(NibConstants::tag_contentResID, contentInfo.u.resID);
	}
	else if (contentInfo.contentType != kControlContentTextOnly)
		PPx_SignalString_("iconpushbutton unknown content type");

	std::auto_ptr<View> view;

	if (contentInfo.contentType == kControlContentTextOnly)
	{
		std::auto_ptr<PushButton> pushButton(new PushButton);

		pushButton->Initialize(inSuperView, frame, isVisible, isEnabled, title);

		if (buttonType == 1)
		{
			pushButton->SetDefaultFlag(true);
			::SetWindowDefaultButton(::GetControlOwner(pushButton->GetSysView()),
				pushButton->GetSysView());
		}
		if (buttonType == 2)
		{
			pushButton->SetCancelFlag(true);
			::SetWindowCancelButton(::GetControlOwner(pushButton->GetSysView()),
				pushButton->GetSysView());
		}
		
		view.reset(pushButton.release());
	}
	else
	{
		std::auto_ptr<IconPushButton> iconPushButton(new IconPushButton);

		iconPushButton->Initialize(inSuperView, frame, isVisible, isEnabled,
			title, contentInfo, alignment);

		if (buttonType == 1)
		{
			iconPushButton->SetDefaultFlag(true);
			::SetWindowDefaultButton(::GetControlOwner(iconPushButton->GetSysView()),
				iconPushButton->GetSysView());
		}
		if (buttonType == 2)
		{
			iconPushButton->SetCancelFlag(true);
			::SetWindowCancelButton(::GetControlOwner(iconPushButton->GetSysView()),
				iconPushButton->GetSysView());
		}

		view.reset(iconPushButton.release());
	}

	ControlSize controlSize = kControlSizeNormal;
	if (reader.ReadOptionalInt(NibConstants::tag_controlSize, controlSize))
		NibDecoderUtils::SetViewSizeAndFont(view.get(), controlSize);

	reader.ReadOptionalControlInfo(view.get());
	reader.ReadOptionalLayoutInfo(view.get());
	reader.ReadOptionalHelpInfo(view.get());
	reader.ReadOptionalSubViews(view.get());
	
	view->FinishInitPersistent();
	
	return view.release();
}

// ---------------------------------------------------------------------------
//	CreateChasingArrows
/**
	+++
	
	@param	inSuperView		+++
	@param	inTree			+++
	
	@return	Pointer to newly created object									*/

Persistent*
NibDecoderFuncs::CreateChasingArrows(
	View*				inSuperView,
	const CFXMLTree&	inTree)
{
	NibReader reader(inTree);

	HIRect frame;
	bool isVisible;
	bool isEnabled;
	reader.ReadCommonValues(inSuperView, frame, isVisible, isEnabled);

	std::auto_ptr<ChasingArrows> view(new ChasingArrows);

	view->Initialize(inSuperView, frame, isVisible, isEnabled);

	reader.ReadOptionalControlInfo(view.get());
	reader.ReadOptionalLayoutInfo(view.get());
	reader.ReadOptionalHelpInfo(view.get());
	reader.ReadOptionalSubViews(view.get());
	
	view->FinishInitPersistent();
	
	return view.release();
}


// ---------------------------------------------------------------------------
//	CreateCheckBox
/**
	+++
	
	@param	inSuperView		+++
	@param	inTree			+++
	
	@return	Pointer to newly created object									*/

Persistent*
NibDecoderFuncs::CreateCheckBox(
	View*				inSuperView,
	const CFXMLTree&	inTree)
{
	NibReader reader(inTree);

	HIRect frame;
	bool isVisible;
	bool isEnabled;
	reader.ReadCommonValues(inSuperView, frame, isVisible, isEnabled);

	CFString title("");
	reader.ReadOptionalString(NibConstants::tag_title, title);

	SInt32 initialValue = 0;
	reader.ReadOptionalInt(NibConstants::tag_initialState, initialValue);
	
	bool autoToggle = true;
	reader.ReadOptionalBool(NibConstants::tag_autoToggle, autoToggle);

	std::auto_ptr<CheckBox> view(new CheckBox);

	view->Initialize(inSuperView, frame, isVisible, isEnabled, title,
		initialValue, autoToggle);

	ControlSize controlSize = kControlSizeNormal;
	if (reader.ReadOptionalInt(NibConstants::tag_controlSize, controlSize))
		NibDecoderUtils::SetViewSizeAndFont(view.get(), controlSize);

	reader.ReadOptionalControlInfo(view.get());
	reader.ReadOptionalLayoutInfo(view.get());
	reader.ReadOptionalHelpInfo(view.get());
	reader.ReadOptionalSubViews(view.get());
	
	view->FinishInitPersistent();
	
	return view.release();
}


// ---------------------------------------------------------------------------
//	CreateClockDate
/**
	+++
	
	@param	inSuperView		+++
	@param	inTree			+++
	
	@return	Pointer to newly created object									*/

Persistent*
NibDecoderFuncs::CreateClockDate(
	View*				inSuperView,
	const CFXMLTree&	inTree)
{
	NibReader reader(inTree);

	HIRect frame;
	bool isVisible;
	bool isEnabled;
	reader.ReadCommonValues(inSuperView, frame, isVisible, isEnabled);

	ControlClockType clockType = kControlClockTypeHourMinute;
	reader.ReadOptionalInt(NibConstants::tag_clockType, clockType);

	ControlClockFlags clockFlags = kControlClockNoFlags;
	bool isEditable = true;
	reader.ReadOptionalBool(NibConstants::tag_isEditable, isEditable);
	if (not isEditable)
		clockFlags |= kControlClockIsDisplayOnly;
	bool isLive = false;
	reader.ReadOptionalBool(NibConstants::tag_isLive, isLive);
	if (isLive)
		clockFlags |= kControlClockIsLive;
	
	std::auto_ptr<ClockControl> view(new ClockControl);

	view->Initialize(inSuperView, frame, isVisible, isEnabled,
		clockType, clockFlags);

	reader.ReadOptionalControlInfo(view.get());
	reader.ReadOptionalLayoutInfo(view.get());
	reader.ReadOptionalHelpInfo(view.get());
	reader.ReadOptionalSubViews(view.get());
	
	view->FinishInitPersistent();
	
	return view.release();
}


// ---------------------------------------------------------------------------
//	CreateComboBox
/**
	+++
	
	@param	inSuperView		+++
	@param	inTree			+++
	
	@return	Pointer to newly created object									*/

Persistent*
NibDecoderFuncs::CreateComboBox(
	View*				inSuperView,
	const CFXMLTree&	inTree)
{
	NibReader reader(inTree);

	HIRect frame;
	bool isVisible;
	bool isEnabled;
	reader.ReadCommonValues(inSuperView, frame, isVisible, isEnabled);

	CFString title("");
	reader.ReadOptionalString(NibConstants::tag_title, title);

	ControlFontStyleRec fontStyle;
	fontStyle.flags = 0;

	OptionBits attributes = kHIComboBoxNoAttributes;
	
	bool hasAutoCompletion = true;
	reader.ReadOptionalBool(NibConstants::tag_autoCompletion, hasAutoCompletion);
	if (hasAutoCompletion)
		attributes |= kHIComboBoxAutoCompletionAttribute;

	bool hasAutoDisclosure = true;
	reader.ReadOptionalBool(NibConstants::tag_autoDisclosure, hasAutoDisclosure);
	if (hasAutoDisclosure)
		attributes |= kHIComboBoxAutoDisclosureAttribute;

	bool hasAutoSort = false;
	reader.ReadOptionalBool(NibConstants::tag_autoSort, hasAutoSort);
	if (hasAutoSort)
		attributes |= kHIComboBoxAutoSortAttribute;

	bool hasAutoSizeList = true;
	reader.ReadOptionalBool(NibConstants::tag_autoSizeList, hasAutoSizeList);
	if (hasAutoSizeList)
		attributes |= kHIComboBoxAutoSizeListAttribute;
	
	CFArray<CFStringRef> valueList(cfSize_Unlimited);
	CFXMLTree listItemsTree;
	if (reader.ReadOptionalElement(NibConstants::elem_array, NibConstants::tag_list, listItemsTree))
	{
		CFIndex itemCount = listItemsTree.GetChildCount();
		for (CFIndex index = 0; index < itemCount; index++)
		{
			CFXMLTree childTree(listItemsTree.GetChildAtIndex(index));
			CFXMLNode childNode(childTree.GetNode());
			if ((childNode.GetTypeCode() == kCFXMLNodeTypeElement) and
				(childNode.GetString() == NibConstants::elem_string))
			{
				valueList.AppendValue(XMLTreeBrowser::GetValue<CFString>(childTree));
			}
		}
	}

	std::auto_ptr<ComboBox> view(new ComboBox);

	view->Initialize(inSuperView, frame, isVisible, isEnabled, title, &fontStyle,
		valueList, attributes);

	ControlSize controlSize = kControlSizeNormal;
	if (reader.ReadOptionalInt(NibConstants::tag_controlSize, controlSize))
		NibDecoderUtils::SetViewSizeAndFont(view.get(), controlSize);

	reader.ReadOptionalControlInfo(view.get());
	reader.ReadOptionalLayoutInfo(view.get());
	reader.ReadOptionalHelpInfo(view.get());
	reader.ReadOptionalSubViews(view.get());
	
	view->FinishInitPersistent();
	
	return view.release();
}


// ---------------------------------------------------------------------------
//	CreateDBColumnView
/**
	+++
	
	@param	inSuperView		+++
	@param	inTree			+++
	
	@return	Pointer to newly created object									*/

Persistent*
NibDecoderFuncs::CreateDBColumnView(
	View*				inSuperView,
	const CFXMLTree&	inTree)
{
	OSStatus status;

	NibReader reader(inTree);

	HIRect frame;
	bool isVisible;
	bool isEnabled;
	reader.ReadCommonValues(inSuperView, frame, isVisible, isEnabled);

	DataBrowser::Content* content = NULL;
	
	std::auto_ptr<DataBrowserView> view(new DataBrowserView);

	view->Initialize(inSuperView, frame, isVisible, isEnabled,
		kDataBrowserColumnView, content);

	// SetHiliteStyle() is only supported with list view even though the
	// IB data contains tag_hiliteStyle.

	UInt16 rowHeight = 22;
	reader.ReadOptionalInt(NibConstants::tag_rowHeight, rowHeight);
	view->SetDefaultRowHeight(rowHeight);

	UInt16 columnWidth = 0;
	reader.ReadOptionalInt(NibConstants::tag_columnWidth, columnWidth);
	view->SetDefaultColumnWidth(columnWidth);
	
	bool variableWidthColumns = false;
	bool variableHeightRows = false;
	reader.ReadOptionalBool(NibConstants::tag_variableWidthColumns, variableWidthColumns);
	reader.ReadOptionalBool(NibConstants::tag_variableHeightRows, variableHeightRows);
	status = ::SetDataBrowserTableViewGeometry(view->GetSysView(), variableWidthColumns, variableHeightRows);
	PPx_ThrowIfOSError_(status, "SetDataBrowserTableViewGeometry failed");

	Rect insetRect = { 0, 0, 0, 15 };
	CFString scrollBarInsetString("");
	if (reader.ReadOptionalString(NibConstants::tag_scrollBarInset, scrollBarInsetString))
		NibDecoderUtils::SetQDRectFromString(scrollBarInsetString, insetRect);
	status = ::SetDataBrowserScrollBarInset(view->GetSysView(), &insetRect);
	PPx_ThrowIfOSError_(status, "SetDataBrowserScrollBarInset failed");
	
	// SetDataBrowserHasScrollBars() is only supported with list view even
	// though the IB data contains tag_horzScrollBar and tag_vertScrollBar.

	DataBrowserSelectionFlags selectionFlags = 0;
	reader.ReadOptionalInt(NibConstants::tag_selectionFlags, selectionFlags);
	status = ::SetDataBrowserSelectionFlags(view->GetSysView(), selectionFlags);
	PPx_ThrowIfOSError_(status, "SetDataBrowserSelectionFlags failed");

	Boolean showFocusRing = true;
	reader.ReadOptionalBoolean(NibConstants::tag_showFocusRing, showFocusRing);
	view->SetDataTag(kControlDataBrowserPart, kControlDataBrowserIncludesFrameAndFocusTag,
		sizeof(showFocusRing), &showFocusRing);

	reader.ReadOptionalControlInfo(view.get());
	reader.ReadOptionalLayoutInfo(view.get());
	reader.ReadOptionalHelpInfo(view.get());
	reader.ReadOptionalSubViews(view.get());
	
	view->FinishInitPersistent();
	
	return view.release();
}


// ---------------------------------------------------------------------------
//	CreateDBListView
/**
	+++
	
	@param	inSuperView		+++
	@param	inTree			+++
	
	@return	Pointer to newly created object									*/

Persistent*
NibDecoderFuncs::CreateDBListView(
	View*				inSuperView,
	const CFXMLTree&	inTree)
{
	NibReader reader(inTree);

	HIRect frame;
	bool isVisible;
	bool isEnabled;
	reader.ReadCommonValues(inSuperView, frame, isVisible, isEnabled);

	DataBrowser::Content* content = NULL;
	
	std::auto_ptr<DataBrowserView> view(new DataBrowserView);

	view->Initialize(inSuperView, frame, isVisible, isEnabled,
		kDataBrowserListView, content);

	// ### finish this up
/*
            <int name="fontStyle">-3</int>
            <boolean name="small">TRUE</boolean>
            <int name="controlSize">1</int>
          <int name="selectionFlags">11</int>
              <int name="textAlignment">-2</int>
*/

	DataBrowserTableViewHiliteStyle hiliteStyle = kDataBrowserTableViewMinimalHilite;
	reader.ReadOptionalInt(NibConstants::tag_hiliteStyle, hiliteStyle);
	view->SetHiliteStyle(hiliteStyle);

	bool horzScrollBar = true;
	reader.ReadOptionalBool(NibConstants::tag_horzScrollBar, horzScrollBar);
	bool vertScrollBar = true;
	reader.ReadOptionalBool(NibConstants::tag_vertScrollBar, vertScrollBar);
	::SetDataBrowserHasScrollBars(view->GetSysView(), horzScrollBar, vertScrollBar);

	DataBrowserSelectionFlags selectionFlags = kDataBrowserDragSelect | kDataBrowserCmdTogglesSelection;
	reader.ReadOptionalInt(NibConstants::tag_selectionFlags, selectionFlags);
	::SetDataBrowserSelectionFlags(view->GetSysView(), selectionFlags);

	UInt16 columnWidth = 80;
	reader.ReadOptionalInt(NibConstants::tag_columnWidth, columnWidth);
	view->SetDefaultColumnWidth(columnWidth);

	UInt16 rowHeight = 24;
	reader.ReadOptionalInt(NibConstants::tag_rowHeight, rowHeight);
	view->SetDefaultRowHeight(rowHeight);
	
	UInt16 listViewHeaderBtnHeight = 0;
	if (reader.ReadOptionalInt(NibConstants::tag_listViewHeaderBtnHeight, listViewHeaderBtnHeight))
		::SetDataBrowserListViewHeaderBtnHeight(view->GetSysView(), listViewHeaderBtnHeight);
	
	Boolean showFocusRing = true;
	reader.ReadOptionalBoolean(NibConstants::tag_showFocusRing, showFocusRing);
	view->SetDataTag(kControlDataBrowserPart, kControlDataBrowserIncludesFrameAndFocusTag,
		sizeof(showFocusRing), &showFocusRing);

	CFXMLTree columnsTree;
	if (reader.ReadOptionalElement(NibConstants::elem_array, NibConstants::tag_listViewColumns, columnsTree))
	{
		for (CFIndex columnIndex = 0; columnIndex < columnsTree.GetChildCount(); columnIndex++)
		{
			CFXMLTree columnInfoTree(columnsTree.GetChildAtIndex(columnIndex));
			NibReader columnInfoReader(columnInfoTree);

			DataBrowserListViewColumnDesc columnDesc;

			columnDesc.propertyDesc.propertyID = 0;
			columnInfoReader.ReadOptionalOSType(NibConstants::tag_propertyID, columnDesc.propertyDesc.propertyID);

			columnDesc.propertyDesc.propertyType = kDataBrowserTextType;
			columnInfoReader.ReadOptionalOSType(NibConstants::tag_propertyType, columnDesc.propertyDesc.propertyType);
			
			columnDesc.propertyDesc.propertyFlags = kDataBrowserListViewDefaultColumnFlags;
			columnInfoReader.ReadOptionalInt(NibConstants::tag_propertyFlags, columnDesc.propertyDesc.propertyFlags);
			
			columnDesc.headerBtnDesc.version = kDataBrowserListViewLatestHeaderDesc;
			
			columnDesc.headerBtnDesc.minimumWidth = 20;
			columnInfoReader.ReadOptionalInt(NibConstants::tag_minWidth, columnDesc.headerBtnDesc.minimumWidth);
			
			columnDesc.headerBtnDesc.maximumWidth = 100;
			columnInfoReader.ReadOptionalInt(NibConstants::tag_maxWidth, columnDesc.headerBtnDesc.maximumWidth);
			
			columnDesc.headerBtnDesc.titleOffset = 0;
			columnInfoReader.ReadOptionalInt(NibConstants::tag_titleOffset, columnDesc.headerBtnDesc.titleOffset);

			CFString columnTitle("");
			columnInfoReader.ReadOptionalString(NibConstants::tag_titleString, columnTitle);
			columnDesc.headerBtnDesc.titleString = columnTitle.UseRef();

			columnDesc.headerBtnDesc.initialOrder = kDataBrowserOrderIncreasing;
			columnInfoReader.ReadOptionalInt(NibConstants::tag_sortOrder, columnDesc.headerBtnDesc.initialOrder);

	// ### finish this up
#if 0
			ControlFontStyleRec style = { 0 };
//			columnDesc.headerBtnDesc.btnFontStyle.flags = 0;
//			columnDesc.headerBtnDesc.btnContentInfo.contentType = kControlNoContent;
			
			ControlSize controlSize = kControlSizeNormal;
			if (reader.ReadOptionalInt(NibConstants::tag_controlSize, controlSize))
			{
				if (controlSize == kControlSizeSmall)
				{
					style.flags |= kControlUseFontMask;
					style.font = kControlFontSmallSystemFont;
				}
			}

			SInt32 fontStyle = 0;
			if (reader.ReadOptionalInt(NibConstants::tag_fontStyle, fontStyle))
			{
				if ((fontStyle <= kControlFontBigSystemFont) and
					(fontStyle >= kControlFontMiniSystemFont))
				{
					// Meta-font numbering
					style.flags |= kControlUseFontMask;
					style.font = fontStyle;
				}
				else
				{
					// ThemeFontID
					style.flags |= kControlUseFontMask | kControlUseThemeFontIDMask;
					style.font = fontStyle;
				}
			}
#else
			columnDesc.headerBtnDesc.btnFontStyle.flags = kControlUseJustMask; //kControlUseThemeFontIDMask | kControlUseJustMask;

			columnDesc.headerBtnDesc.btnFontStyle.just = teCenter;
			columnInfoReader.ReadOptionalInt(NibConstants::tag_textAlignment, columnDesc.headerBtnDesc.btnFontStyle.just);
			
//			if (just)
//				columnDesc.headerBtnDesc.btnFontStyle.flags |= kControlUseJustMask;

//			columnDesc.headerBtnDesc.btnFontStyle.font = kControlFontViewSystemFont;
//			columnDesc.headerBtnDesc.btnFontStyle.just = teCenter;
			columnDesc.headerBtnDesc.btnContentInfo.contentType = kControlNoContent;
#endif

			view->AddColumn(columnDesc, columnIndex);

		}
	}

	reader.ReadOptionalControlInfo(view.get());
	reader.ReadOptionalLayoutInfo(view.get());
	reader.ReadOptionalHelpInfo(view.get());
	reader.ReadOptionalSubViews(view.get());
	
	view->FinishInitPersistent();
	
	return view.release();
}


// ---------------------------------------------------------------------------
//	CreateEditText
/**
	+++
	
	@param	inSuperView		+++
	@param	inTree			+++
	
	@return	Pointer to newly created object									*/

Persistent*
NibDecoderFuncs::CreateEditText(
	View*				inSuperView,
	const CFXMLTree&	inTree)
{
	NibReader reader(inTree);

	HIRect frame;
	bool isVisible;
	bool isEnabled;
	reader.ReadCommonValues(inSuperView, frame, isVisible, isEnabled);

	CFString title("");
	reader.ReadOptionalString(NibConstants::tag_title, title);

	bool isPassword = false;
	reader.ReadOptionalBool(NibConstants::tag_isPassword, isPassword);

	bool useInlineInput = false;
	reader.ReadOptionalBool(NibConstants::tag_useInlineInput, useInlineInput);

	ControlFontStyleRec style = { 0 };

	ControlSize controlSize = kControlSizeNormal;
	if (reader.ReadOptionalInt(NibConstants::tag_controlSize, controlSize))
	{
		if (controlSize == kControlSizeSmall)
		{
			style.flags |= kControlUseFontMask;
			style.font = kControlFontSmallSystemFont;
		}

#if (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_3)

		else if (controlSize == kControlSizeMini)
		{
			style.flags |= kControlUseFontMask;
			style.font = kControlFontMiniSystemFont;
		}

#endif // 10.3 or later

	}

	SInt32 fontStyle = 0;
	if (reader.ReadOptionalInt(NibConstants::tag_fontStyle, fontStyle))
	{
		if (fontStyle < 0)
		{
			// Meta-font numbering
			style.flags |= kControlUseFontMask;
			style.font = fontStyle;
		}
		else
		{
			// ThemeFontID
			style.flags |= kControlUseThemeFontIDMask;
			style.font = fontStyle;
		}
	}
	bool isUnicode = false;
	reader.ReadOptionalBool(NibConstants::tag_isUnicode, isUnicode);
	
	std::auto_ptr<View> view;

	if (isUnicode)
	{
		std::auto_ptr<EditUnicodeText> editUnicodeText(new EditUnicodeText);

		editUnicodeText->Initialize(inSuperView, frame, isVisible, isEnabled,
			title, isPassword, &style);

		Boolean isSingleLine = false;
		reader.ReadOptionalBoolean(NibConstants::tag_isSingleLine, isSingleLine);
		editUnicodeText->SetDataTag(kControlEntireControl, kControlEditTextSingleLineTag,
			sizeof(isSingleLine), &isSingleLine);
		
		view.reset(editUnicodeText.release());
	}
	else
	{
		std::auto_ptr<EditTextControl> editTextControl(new EditTextControl);

		editTextControl->Initialize(inSuperView, frame, isVisible, isEnabled,
			title, isPassword, useInlineInput, &style);

		view.reset(editTextControl.release());
	}

	reader.ReadOptionalControlInfo(view.get());
	reader.ReadOptionalLayoutInfo(view.get());
	reader.ReadOptionalHelpInfo(view.get());
	reader.ReadOptionalSubViews(view.get());
	
	view->FinishInitPersistent();
	
	return view.release();
}


// ---------------------------------------------------------------------------
//	CreateGroupBox
/**
	+++
	
	@param	inSuperView		+++
	@param	inTree			+++
	
	@return	Pointer to newly created object									*/

Persistent*
NibDecoderFuncs::CreateGroupBox(
	View*				inSuperView,
	const CFXMLTree&	inTree)
{
	NibReader reader(inTree);

	HIRect frame;
	bool isVisible;
	bool isEnabled;
	reader.ReadCommonValues(inSuperView, frame, isVisible, isEnabled);

	CFString title("");
	reader.ReadOptionalString(NibConstants::tag_title, title);

	SInt32 initialValue = 0;
	reader.ReadOptionalInt(NibConstants::tag_initialValue, initialValue);

	bool isPrimary = true;
	reader.ReadOptionalBool(NibConstants::tag_primary, isPrimary);

	bool autoToggle = true;
	reader.ReadOptionalBool(NibConstants::tag_autoToggle, autoToggle);

	SInt32 titleType = 0;
	reader.ReadOptionalInt(NibConstants::tag_titleType, titleType);

	std::auto_ptr<View> view;

	if (titleType == 1)
	{
		std::auto_ptr<CheckBoxGroupBox> checkBoxGroupBox(new CheckBoxGroupBox);

		checkBoxGroupBox->Initialize(inSuperView, frame, isVisible, isEnabled,
			title, initialValue, isPrimary, autoToggle);

		ControlSize controlSize = kControlSizeNormal;
		if (reader.ReadOptionalInt(NibConstants::tag_controlSize, controlSize))
			NibDecoderUtils::SetViewSizeAndFont(checkBoxGroupBox.get(), controlSize);

		view.reset(checkBoxGroupBox.release());
	}
	else if (titleType == 2)
	{
		bool hasVariableWidth = false;
		reader.ReadOptionalBool(NibConstants::tag_variableWidth, hasVariableWidth);

		SInt32 selectedItem = 1;
		reader.ReadOptionalInt(NibConstants::tag_selectedItem, selectedItem);

		SInt32 menuID = -12345;
		SInt16 titleWidth = 0;
		SInt16 titleJust = teFlushDefault;
		Style titleStyle = normal;

		std::auto_ptr<PopupGroupBox> popupGroupBox(new PopupGroupBox);

		popupGroupBox->Initialize(inSuperView, frame, isVisible, isEnabled, title,
			isPrimary, menuID, hasVariableWidth, titleWidth, titleJust, titleStyle);

		MenuRef menu = NULL;
		if (reader.ReadOptionalMenu(NibConstants::tag_menu, menu) and ::IsValidMenu(menu))
		{
				popupGroupBox->SetMenuRef(menu);
				popupGroupBox->SetMaxValue(::CountMenuItems(menu));
				if (selectedItem != 1)
					popupGroupBox->SetValue(selectedItem);
		}

		ControlSize controlSize = kControlSizeNormal;
		if (reader.ReadOptionalInt(NibConstants::tag_controlSize, controlSize))
			NibDecoderUtils::SetViewSizeAndFont(popupGroupBox.get(), controlSize);

		view.reset(popupGroupBox.release());
	}
	else
	{
		std::auto_ptr<TextGroupBox> textGroupBox(new TextGroupBox);

		textGroupBox->Initialize(inSuperView, frame, isVisible, isEnabled,
			title, isPrimary);

		ControlSize controlSize = kControlSizeNormal;
		if (reader.ReadOptionalInt(NibConstants::tag_controlSize, controlSize))
			NibDecoderUtils::SetViewFont(textGroupBox.get(), controlSize);

		view.reset(textGroupBox.release());
	}

	reader.ReadOptionalControlInfo(view.get());
	reader.ReadOptionalLayoutInfo(view.get());
	reader.ReadOptionalHelpInfo(view.get());
	reader.ReadOptionalSubViews(view.get());
	
	view->FinishInitPersistent();
	
	return view.release();
}


// ---------------------------------------------------------------------------
//	CreateHIView
/**
	+++
	
	@param	inSuperView		+++
	@param	inTree			+++
	
	@return	Pointer to newly created object									*/

Persistent*
NibDecoderFuncs::CreateHIView(
	View*				inSuperView,
	const CFXMLTree&	inTree)
{
	NibReader reader(inTree);

	CFString classID("");
	reader.ReadRequiredString(NibConstants::tag_classID, classID);

	Persistent* persistent = NibRegistrar::CreateObject(classID, inSuperView, inTree);

	return persistent;
}


// ---------------------------------------------------------------------------
//	CreateIcon
/**
	+++
	
	@param	inSuperView		+++
	@param	inTree			+++
	
	@return	Pointer to newly created object									*/

Persistent*
NibDecoderFuncs::CreateIcon(
	View*				inSuperView,
	const CFXMLTree&	inTree)
{
	NibReader reader(inTree);

	HIRect frame;
	bool isVisible;
	bool isEnabled;
	reader.ReadCommonValues(inSuperView, frame, isVisible, isEnabled);

	bool dontTrack = true;
	reader.ReadOptionalBool(NibConstants::tag_dontTrack, dontTrack);

	AutoIcon icon;
	ControlButtonContentInfo contentInfo;
	contentInfo.contentType = kControlContentCIconRes;
	reader.ReadOptionalInt(NibConstants::tag_contentType, contentInfo.contentType);
	if ((contentInfo.contentType == kControlContentIconSuiteRes) or
		(contentInfo.contentType == kControlContentCIconRes) or
		(contentInfo.contentType == kControlContentICONRes))
	{
		contentInfo.u.resID = 1;
		reader.ReadOptionalInt(NibConstants::tag_contentResID, contentInfo.u.resID);
	}
	else if (contentInfo.contentType == kControlContentIconRef)
	{
		OSType creator = 0;
		reader.ReadOptionalInt(NibConstants::tag_creator, creator);

		OSType iconType = 0;
		reader.ReadOptionalInt(NibConstants::tag_contentResID, iconType);

		OSStatus status;
		status = ::GetIconRef(kOnSystemDisk, creator, iconType, &contentInfo.u.iconRef);
		if (status == noErr)
			icon.Reset(contentInfo.u.iconRef);
	}
	else
		PPx_SignalString_("iconControl unknown content type");

	std::auto_ptr<IconControl> view(new IconControl);

	view->Initialize(inSuperView, frame, isVisible, isEnabled, contentInfo, dontTrack);

	IconTransformType transformType = kTransformNone;
	reader.ReadOptionalInt(NibConstants::tag_transform, transformType);
	view->SetIconTransform(transformType);

	reader.ReadOptionalControlInfo(view.get());
	reader.ReadOptionalLayoutInfo(view.get());
	reader.ReadOptionalHelpInfo(view.get());
	reader.ReadOptionalSubViews(view.get());
	
	view->FinishInitPersistent();
	
	return view.release();
}


// ---------------------------------------------------------------------------
//	CreateImageView
/**
	+++
	
	@param	inSuperView		+++
	@param	inTree			+++
	
	@return	Pointer to newly created object									*/

Persistent*
NibDecoderFuncs::CreateImageView(
	View*				inSuperView,
	const CFXMLTree&	inTree)
{
	NibReader reader(inTree);

	HIRect frame;
	bool isVisible;
	bool isEnabled;
	reader.ReadCommonValues(inSuperView, frame, isVisible, isEnabled);

	CGImageRef image = NULL;

	std::auto_ptr<ImageView> view(new ImageView);

	view->Initialize(inSuperView, frame, isVisible, isEnabled, image);

	float alpha = 1.0;
	reader.ReadOptionalFloat(NibConstants::tag_alpha, alpha);
	view->SetAlpha(alpha);
	
	bool isOpaque = true;
	reader.ReadOptionalBool(NibConstants::tag_opaque, isOpaque);
	view->SetOpaque(isOpaque);

	bool scaleToFit = false;
	reader.ReadOptionalBool(NibConstants::tag_scaleToFit, scaleToFit);
	view->SetScaleToFit(scaleToFit);

	reader.ReadOptionalControlInfo(view.get());
	reader.ReadOptionalLayoutInfo(view.get());
	reader.ReadOptionalHelpInfo(view.get());
	reader.ReadOptionalSubViews(view.get());
	
	view->FinishInitPersistent();
	
	return view.release();
}


// ---------------------------------------------------------------------------
//	CreateImageWell
/**
	+++
	
	@param	inSuperView		+++
	@param	inTree			+++
	
	@return	Pointer to newly created object									*/

Persistent*
NibDecoderFuncs::CreateImageWell(
	View*				inSuperView,
	const CFXMLTree&	inTree)
{
	NibReader reader(inTree);

	HIRect frame;
	bool isVisible;
	bool isEnabled;
	reader.ReadCommonValues(inSuperView, frame, isVisible, isEnabled);

	ControlButtonContentInfo contentInfo;
	contentInfo.contentType = kControlContentCIconRes;
	reader.ReadOptionalInt(NibConstants::tag_contentType, contentInfo.contentType);
	if ((contentInfo.contentType == kControlNoContent) or
		(contentInfo.contentType == kControlContentIconSuiteRes) or
		(contentInfo.contentType == kControlContentCIconRes) or
		(contentInfo.contentType == kControlContentPictRes))
	{
		contentInfo.u.resID = 1;
		reader.ReadOptionalInt(NibConstants::tag_contentResID, contentInfo.u.resID);
	}
	else
		PPx_SignalString_("imageWell unknown content type");

	std::auto_ptr<ImageWell> view(new ImageWell);

	view->Initialize(inSuperView, frame, isVisible, isEnabled, contentInfo);

	bool isDragDestination = false;
	reader.ReadOptionalBool(NibConstants::tag_dropTarget, isDragDestination);
	view->SetDragDestinationFlag(isDragDestination);
	
	IconTransformType transformType = kTransformNone;
	reader.ReadOptionalInt(NibConstants::tag_contentTransform, transformType);
	view->SetImageTransform(transformType);

	reader.ReadOptionalControlInfo(view.get());
	reader.ReadOptionalLayoutInfo(view.get());
	reader.ReadOptionalHelpInfo(view.get());
	reader.ReadOptionalSubViews(view.get());
	
	view->FinishInitPersistent();
	
	return view.release();
}


// ---------------------------------------------------------------------------
//	CreateLittleArrows
/**
	+++
	
	@param	inSuperView		+++
	@param	inTree			+++
	
	@return	Pointer to newly created object									*/

Persistent*
NibDecoderFuncs::CreateLittleArrows(
	View*				inSuperView,
	const CFXMLTree&	inTree)
{
	NibReader reader(inTree);

	HIRect frame;
	bool isVisible;
	bool isEnabled;
	reader.ReadCommonValues(inSuperView, frame, isVisible, isEnabled);

	SInt32 initialValue = 0;
	reader.ReadOptionalInt(NibConstants::tag_initialValue, initialValue);

	SInt32 minValue = 0;
	reader.ReadOptionalInt(NibConstants::tag_minimumValue, minValue);

	SInt32 maxValue = 100;
	reader.ReadOptionalInt(NibConstants::tag_maximumValue, maxValue);

	SInt32 increment = 1;
	reader.ReadOptionalInt(NibConstants::tag_increment, increment);

	std::auto_ptr<LittleArrows> view(new LittleArrows);

	view->Initialize(inSuperView, frame, isVisible, isEnabled, initialValue,
		minValue, maxValue, increment);

	ControlSize controlSize = kControlSizeNormal;
	if (reader.ReadOptionalInt(NibConstants::tag_controlSize, controlSize))
		view->SetDataTag(kControlEntireControl, kControlSizeTag,
			sizeof(controlSize), &controlSize);

	reader.ReadOptionalControlInfo(view.get());
	reader.ReadOptionalLayoutInfo(view.get());
	reader.ReadOptionalHelpInfo(view.get());
	reader.ReadOptionalSubViews(view.get());
	
	view->FinishInitPersistent();
	
	return view.release();
}


// ---------------------------------------------------------------------------
//	CreatePicture
/**
	+++
	
	@param	inSuperView		+++
	@param	inTree			+++
	
	@return	Pointer to newly created object									*/

Persistent*
NibDecoderFuncs::CreatePicture(
	View*				inSuperView,
	const CFXMLTree&	inTree)
{
	NibReader reader(inTree);

	HIRect frame;
	bool isVisible;
	bool isEnabled;
	reader.ReadCommonValues(inSuperView, frame, isVisible, isEnabled);

	SInt16 pictResID = -1395;
	reader.ReadOptionalInt(NibConstants::tag_contentResID, pictResID);

	PicHandle pictureHandle = NULL;
	
	bool dontTrack = true;
	reader.ReadOptionalBool(NibConstants::tag_dontTrack, dontTrack);
	
	std::auto_ptr<PictureControl> view(new PictureControl);

	view->Initialize(inSuperView, frame, isVisible, isEnabled, pictResID,
		pictureHandle, dontTrack);

	reader.ReadOptionalControlInfo(view.get());
	reader.ReadOptionalLayoutInfo(view.get());
	reader.ReadOptionalHelpInfo(view.get());
	reader.ReadOptionalSubViews(view.get());
	
	view->FinishInitPersistent();
	
	return view.release();
}


// ---------------------------------------------------------------------------
//	CreatePopupArrow
/**
	+++
	
	@param	inSuperView		+++
	@param	inTree			+++
	
	@return	Pointer to newly created object									*/

Persistent*
NibDecoderFuncs::CreatePopupArrow(
	View*				inSuperView,
	const CFXMLTree&	inTree)
{
	NibReader reader(inTree);

	HIRect frame;
	bool isVisible;
	bool isEnabled;
	reader.ReadCommonValues(inSuperView, frame, isVisible, isEnabled);

	ControlPopupArrowOrientation orientation = kControlPopupArrowOrientationEast;
	reader.ReadOptionalInt(NibConstants::tag_orientation, orientation);
	
	ControlPopupArrowSize size = kControlPopupArrowSizeNormal;
	reader.ReadOptionalInt(NibConstants::tag_size, size);
	
	std::auto_ptr<PopupArrow> view(new PopupArrow);

	view->Initialize(inSuperView, frame, isVisible, isEnabled, orientation, size);

	reader.ReadOptionalControlInfo(view.get());
	reader.ReadOptionalLayoutInfo(view.get());
	reader.ReadOptionalHelpInfo(view.get());
	reader.ReadOptionalSubViews(view.get());
	
	view->FinishInitPersistent();
	
	return view.release();
}


// ---------------------------------------------------------------------------
//	CreatePopupButton
/**
	+++
	
	@param	inSuperView		+++
	@param	inTree			+++
	
	@return	Pointer to newly created object									*/

Persistent*
NibDecoderFuncs::CreatePopupButton(
	View*				inSuperView,
	const CFXMLTree&	inTree)
{
	NibReader reader(inTree);

	HIRect frame;
	bool isVisible;
	bool isEnabled;
	reader.ReadCommonValues(inSuperView, frame, isVisible, isEnabled);

	CFString title("");
	reader.ReadOptionalString(NibConstants::tag_title, title);

	// Interface Builder has a misspelling of "variableWith" within its XML.
	// We will also check for the correct spelling just in case Apple changes
	// this in the future.
	bool hasVariableWidth = false;
	if (not reader.ReadOptionalBool(NibConstants::tag_variableWith, hasVariableWidth))
		reader.ReadOptionalBool(NibConstants::tag_variableWidth, hasVariableWidth);

	SInt32 selectedItem = 1;
	reader.ReadOptionalInt(NibConstants::tag_selectedItem, selectedItem);
	
	SInt32 menuID = -12345;
	SInt16 titleWidth = -1;
	Style titleStyle = normal;

	SInt16 titleJust = teFlushDefault;
	reader.ReadOptionalInt(NibConstants::tag_titleJustification, titleJust);
	
	std::auto_ptr<PopupButton> view(new PopupButton);

	view->Initialize(inSuperView, frame, isVisible, isEnabled, title,
		menuID, hasVariableWidth, titleWidth, titleJust, titleStyle);

	ControlSize controlSize = kControlSizeNormal;
	if (reader.ReadOptionalInt(NibConstants::tag_controlSize, controlSize))
		NibDecoderUtils::SetViewSizeAndFont(view.get(), controlSize);

	MenuRef menu = NULL;
	if (reader.ReadOptionalMenu(NibConstants::tag_menu, menu) and ::IsValidMenu(menu))
	{
		view->SetMenuRef(menu);
		view->SetMinValue((::CountMenuItems(menu)) ? 1 : 0);
		view->SetMaxValue(::CountMenuItems(menu));
		if (selectedItem != 1)
			view->SetValue(selectedItem);
	}

	reader.ReadOptionalControlInfo(view.get());
	reader.ReadOptionalLayoutInfo(view.get());
	reader.ReadOptionalHelpInfo(view.get());
	reader.ReadOptionalSubViews(view.get());
	
	view->FinishInitPersistent();
	
	return view.release();
}


// ---------------------------------------------------------------------------
//	CreateProgressBar
/**
	+++
	
	@param	inSuperView		+++
	@param	inTree			+++
	
	@return	Pointer to newly created object									*/

Persistent*
NibDecoderFuncs::CreateProgressBar(
	View*				inSuperView,
	const CFXMLTree&	inTree)
{
	NibReader reader(inTree);

	HIRect frame;
	bool isVisible;
	bool isEnabled;
	reader.ReadCommonValues(inSuperView, frame, isVisible, isEnabled);

	SInt32 initialValue = 0;
	reader.ReadOptionalInt(NibConstants::tag_initialValue, initialValue);
	
	SInt32 minValue = 0;
	reader.ReadOptionalInt(NibConstants::tag_minimumValue, minValue);
	
	SInt32 maxValue = 100;
	reader.ReadOptionalInt(NibConstants::tag_maximumValue, maxValue);
	
	bool isIndeterminate = true;
	reader.ReadOptionalBool(NibConstants::tag_indeterminate, isIndeterminate);
	
	std::auto_ptr<ProgressBar> view(new ProgressBar);

	view->Initialize(inSuperView, frame, isVisible, isEnabled,
		initialValue, minValue, maxValue, isIndeterminate);

	bool small = false;
	ControlSize controlSize = kControlSizeLarge;
	if (reader.ReadOptionalBool(NibConstants::tag_small, small) and small)
		controlSize = kControlSizeNormal;
	view->SetDataTag(kControlEntireControl, kControlSizeTag, sizeof(controlSize), &controlSize);
	
	reader.ReadOptionalControlInfo(view.get());
	reader.ReadOptionalLayoutInfo(view.get());
	reader.ReadOptionalHelpInfo(view.get());
	reader.ReadOptionalSubViews(view.get());
	
	view->FinishInitPersistent();
	
	return view.release();
}


// ---------------------------------------------------------------------------
//	CreateRadioButton
/**
	+++
	
	@param	inSuperView		+++
	@param	inTree			+++
	
	@return	Pointer to newly created object									*/

Persistent*
NibDecoderFuncs::CreateRadioButton(
	View*				inSuperView,
	const CFXMLTree&	inTree)
{
	NibReader reader(inTree);

	HIRect frame;
	bool isVisible;
	bool isEnabled;
	reader.ReadCommonValues(inSuperView, frame, isVisible, isEnabled);

	CFString title("");
	reader.ReadOptionalString(NibConstants::tag_title, title);

	bool autoToggle = true;
	reader.ReadOptionalBool(NibConstants::tag_autoToggle, autoToggle);

	SInt32 initialValue = 0;
	reader.ReadOptionalInt(NibConstants::tag_initialState, initialValue);
	
	std::auto_ptr<RadioButton> view(new RadioButton);

	view->Initialize(inSuperView, frame, isVisible, isEnabled, title,
		initialValue, autoToggle);

	ControlSize controlSize = kControlSizeNormal;
	if (reader.ReadOptionalInt(NibConstants::tag_controlSize, controlSize))
		NibDecoderUtils::SetViewSizeAndFont(view.get(), controlSize);

	reader.ReadOptionalControlInfo(view.get());
	reader.ReadOptionalLayoutInfo(view.get());
	reader.ReadOptionalHelpInfo(view.get());
	reader.ReadOptionalSubViews(view.get());
	
	view->FinishInitPersistent();
	
	return view.release();
}


// ---------------------------------------------------------------------------
//	CreateRadioGroup
/**
	+++
	
	@param	inSuperView		+++
	@param	inTree			+++
	
	@return	Pointer to newly created object									*/

Persistent*
NibDecoderFuncs::CreateRadioGroup(
	View*				inSuperView,
	const CFXMLTree&	inTree)
{
	NibReader reader(inTree);

	HIRect frame;
	bool isVisible;
	bool isEnabled;
	reader.ReadCommonValues(inSuperView, frame, isVisible, isEnabled);

	std::auto_ptr<RadioGroup> view(new RadioGroup);

	view->Initialize(inSuperView, frame, isVisible, isEnabled);

	SInt32 numRows = 2;
	reader.ReadOptionalInt(NibConstants::tag_numRows, numRows);
	
	SInt32 numCols = 1;
	reader.ReadOptionalInt(NibConstants::tag_numCols, numCols);

	SInt32 selectedRow = 1;
	reader.ReadOptionalInt(NibConstants::tag_selectedRow, selectedRow);

	SInt32 selectedCol = 1;
	reader.ReadOptionalInt(NibConstants::tag_selectedCol, selectedCol);

	SInt32 intercellHeight = 2;
	reader.ReadOptionalInt(NibConstants::tag_intercellHeight, intercellHeight);
	
	ControlSize controlSize = kControlSizeNormal;
	reader.ReadOptionalInt(NibConstants::tag_controlSize, controlSize);

	CFXMLTree radioTitles;
	if (reader.ReadOptionalElement(NibConstants::elem_array, NibConstants::tag_radioTitles, radioTitles))
	{
		PPx_SignalIf_(radioTitles.GetChildCount() != numRows);

		frame.origin.x = 0;
		frame.origin.y = 0;
		frame.size.width = frame.size.width / numCols;
		frame.size.height = 18;
		
		OSStatus status;
		SInt32 buttonHeight;
		if (controlSize == kControlSizeNormal)
		{
			status = ::GetThemeMetric(kThemeMetricRadioButtonHeight, &buttonHeight);
			PPx_ThrowIfOSError_(status, "GetThemeMetric failed");
			frame.size.height = buttonHeight;
		}
		else if (controlSize == kControlSizeSmall)
		{
			status = ::GetThemeMetric(kThemeMetricSmallRadioButtonHeight, &buttonHeight);
			PPx_ThrowIfOSError_(status, "GetThemeMetric failed");
			frame.size.height = buttonHeight;
		}

#if (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_3)

		else if (controlSize == kControlSizeMini)
		{
			status = ::GetThemeMetric(kThemeMetricMiniRadioButtonHeight, &buttonHeight);
			PPx_ThrowIfOSError_(status, "GetThemeMetric failed");
			frame.size.height = buttonHeight;
		}

#endif // 10.3 or later

		for (CFIndex colIndex = 0; colIndex < numCols; colIndex++)
		{
			frame.origin.x = colIndex * frame.size.width;
			frame.origin.y = 0;
			
			for (CFIndex rowIndex = 0; rowIndex < numRows; rowIndex++)
			{
				CFXMLTree rowTree(radioTitles.GetChildAtIndex(rowIndex));
				CFXMLNode rowNode(rowTree.GetNode());

				CFXMLTree titleTree(rowTree.GetChildAtIndex(colIndex));
				CFXMLNode titleNode(titleTree.GetNode());
				
				CFString title(XMLTreeBrowser::GetValue<CFString>(titleTree));

				std::auto_ptr<RadioButton> radioButton(new RadioButton);

				radioButton->Initialize(view.get(), frame, visible_Yes,
					enabled_Yes, title, 0, true);

				if (controlSize != kControlSizeNormal)
					NibDecoderUtils::SetViewSizeAndFont(radioButton.get(), controlSize);

				radioButton.release();
				
				frame.origin.y += frame.size.height + intercellHeight;
			}
		}
	}

	view->SetValue(selectedRow + numRows * (selectedCol - 1));
	
	reader.ReadOptionalControlInfo(view.get());
	reader.ReadOptionalLayoutInfo(view.get());
	reader.ReadOptionalHelpInfo(view.get());
	reader.ReadOptionalSubViews(view.get());
	
	view->FinishInitPersistent();
	
	return view.release();
}


// ---------------------------------------------------------------------------
//	CreateRelevanceBar
/**
	+++
	
	@param	inSuperView		+++
	@param	inTree			+++
	
	@return	Pointer to newly created object									*/

Persistent*
NibDecoderFuncs::CreateRelevanceBar(
	View*				inSuperView,
	const CFXMLTree&	inTree)
{
	NibReader reader(inTree);

	HIRect frame;
	bool isVisible;
	bool isEnabled;
	reader.ReadCommonValues(inSuperView, frame, isVisible, isEnabled);

	SInt32 initialValue = 50;
	reader.ReadOptionalInt(NibConstants::tag_initialValue, initialValue);
	
	SInt32 minValue = 0;
	reader.ReadOptionalInt(NibConstants::tag_minimumValue, minValue);
	
	SInt32 maxValue = 100;
	reader.ReadOptionalInt(NibConstants::tag_maximumValue, maxValue);
	
	std::auto_ptr<RelevanceBar> view(new RelevanceBar);

	view->Initialize(inSuperView, frame, isVisible, isEnabled, initialValue,
		minValue, maxValue);

	reader.ReadOptionalControlInfo(view.get());
	reader.ReadOptionalLayoutInfo(view.get());
	reader.ReadOptionalHelpInfo(view.get());
	reader.ReadOptionalSubViews(view.get());
	
	view->FinishInitPersistent();
	
	return view.release();
}


// ---------------------------------------------------------------------------
//	CreateRoundButton
/**
	+++
	
	@param	inSuperView		+++
	@param	inTree			+++
	
	@return	Pointer to newly created object									*/

Persistent*
NibDecoderFuncs::CreateRoundButton(
	View*				inSuperView,
	const CFXMLTree&	inTree)
{
	NibReader reader(inTree);

	HIRect frame;
	bool isVisible;
	bool isEnabled;
	reader.ReadCommonValues(inSuperView, frame, isVisible, isEnabled);

	CFString title;
	reader.ReadOptionalString(NibConstants::tag_title, title);
	
	ControlRoundButtonSize buttonSize = kControlRoundButtonNormalSize;
	reader.ReadOptionalInt(NibConstants::tag_size, buttonSize);

	AutoIcon icon;
	ControlButtonContentInfo contentInfo;
	contentInfo.contentType = kControlNoContent;
	reader.ReadOptionalInt(NibConstants::tag_contentType, contentInfo.contentType);
	if (contentInfo.contentType == kControlContentIconRef)
	{
		OSType creator = 0;
		reader.ReadOptionalInt(NibConstants::tag_iconCreator, creator);

		OSType iconType = 0;
		reader.ReadOptionalInt(NibConstants::tag_iconType, iconType);

		OSStatus status;
		status = ::GetIconRef(kOnSystemDisk, creator, iconType, &contentInfo.u.iconRef);
		if (status == noErr)
			icon.Reset(contentInfo.u.iconRef);
	}
	else if (contentInfo.contentType == kControlNoContent)
		; // do nothing
	else
		PPx_SignalString_("roundButton unknown content type");

	std::auto_ptr<RoundButton> view(new RoundButton);

	view->Initialize(inSuperView, frame, isVisible, isEnabled, buttonSize,
		contentInfo);

	reader.ReadOptionalControlInfo(view.get());
	reader.ReadOptionalLayoutInfo(view.get());
	reader.ReadOptionalHelpInfo(view.get());
	reader.ReadOptionalSubViews(view.get());
	
	view->FinishInitPersistent();
	
	return view.release();
}


// ---------------------------------------------------------------------------
//	CreateScrollBar
/**
	+++
	
	@param	inSuperView		+++
	@param	inTree			+++
	
	@return	Pointer to newly created object									*/

Persistent*
NibDecoderFuncs::CreateScrollBar(
	View*				inSuperView,
	const CFXMLTree&	inTree)
{
	NibReader reader(inTree);

	HIRect frame;
	bool isVisible;
	bool isEnabled;
	reader.ReadCommonValues(inSuperView, frame, isVisible, isEnabled);

	SInt32 initialValue = 0;
	reader.ReadOptionalInt(NibConstants::tag_initialValue, initialValue);
	
	SInt32 minValue = initialValue;
	reader.ReadOptionalInt(NibConstants::tag_minimumValue, minValue);
	
	SInt32 maxValue = 100;
	reader.ReadOptionalInt(NibConstants::tag_maximumValue, maxValue);
	
	SInt32 viewSize = 0;
	
	bool hasLiveTracking = true;
	reader.ReadOptionalBool(NibConstants::tag_isLive, hasLiveTracking);
	
	std::auto_ptr<ScrollBar> view(new ScrollBar);

	view->Initialize(inSuperView, frame, isVisible, isEnabled, initialValue,
		minValue, maxValue, viewSize, hasLiveTracking);

	reader.ReadOptionalControlInfo(view.get());
	reader.ReadOptionalLayoutInfo(view.get());
	reader.ReadOptionalHelpInfo(view.get());
	reader.ReadOptionalSubViews(view.get());
	
	view->FinishInitPersistent();
	
	return view.release();
}


// ---------------------------------------------------------------------------
//	CreateScrollView
/**
	+++
	
	@param	inSuperView		+++
	@param	inTree			+++
	
	@return	Pointer to newly created object									*/

Persistent*
NibDecoderFuncs::CreateScrollView(
	View*				inSuperView,
	const CFXMLTree&	inTree)
{
	NibReader reader(inTree);

	HIRect frame;
	bool isVisible;
	bool isEnabled;
	reader.ReadCommonValues(inSuperView, frame, isVisible, isEnabled);

	OptionBits options = 0;

	bool showVertical = true;
	reader.ReadOptionalBool(NibConstants::tag_scrollVertically, showVertical);
	if (showVertical)
		options |= kHIScrollViewOptionsVertScroll;
	
	bool showHorizontal = true;
	reader.ReadOptionalBool(NibConstants::tag_scrollHorizontally, showHorizontal);
	if (showHorizontal)
		options |= kHIScrollViewOptionsHorizScroll;

	bool allowGrowBox = false;
	reader.ReadOptionalBool(NibConstants::tag_allowGrowBox, allowGrowBox);
	if (allowGrowBox)
		options |= kHIScrollViewOptionsAllowGrow;
	
	std::auto_ptr<ScrollView> view(new ScrollView);

	view->Initialize(inSuperView, frame, isVisible, isEnabled, options);

	bool autoHideScrollBars = false;
	reader.ReadOptionalBool(NibConstants::tag_autoHideScrollBars, autoHideScrollBars);
	view->SetAutoHideScrollBars(autoHideScrollBars);
	
	reader.ReadOptionalControlInfo(view.get());
	reader.ReadOptionalLayoutInfo(view.get());
	reader.ReadOptionalHelpInfo(view.get());
	reader.ReadOptionalSubViews(view.get());
	
	view->FinishInitPersistent();
	
	return view.release();
}


// ---------------------------------------------------------------------------
//	CreateSearchField
/**
	+++
	
	@param	inSuperView		+++
	@param	inTree			+++
	
	@return	Pointer to newly created object									*/

Persistent*
NibDecoderFuncs::CreateSearchField(
	View*				inSuperView,
	const CFXMLTree&	inTree)
{
#if (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_3)

	NibReader reader(inTree);

	HIRect frame;
	bool isVisible;
	bool isEnabled;
	reader.ReadCommonValues(inSuperView, frame, isVisible, isEnabled);

	CFString descriptiveText("");
	reader.ReadOptionalString(NibConstants::tag_descriptiveText, descriptiveText);

	OptionBits attributes = kHISearchFieldNoAttributes;
	
	bool hasCancelIcon = false;
	reader.ReadOptionalBool(NibConstants::tag_hasCancelIcon, hasCancelIcon);
	if (hasCancelIcon)
		attributes |= kHISearchFieldAttributesCancel;

	MenuRef menu = NULL;
	reader.ReadOptionalMenu(NibConstants::tag_menu, menu);
	
	std::auto_ptr<SearchField> view(new SearchField);

	view->Initialize(inSuperView, frame, isVisible, isEnabled, attributes,
		menu, descriptiveText);

	ControlSize controlSize = kControlSizeNormal;
	if (reader.ReadOptionalInt(NibConstants::tag_controlSize, controlSize))
		NibDecoderUtils::SetViewSizeAndFont(view.get(), controlSize);

	reader.ReadOptionalControlInfo(view.get());
	reader.ReadOptionalLayoutInfo(view.get());
	reader.ReadOptionalHelpInfo(view.get());
	reader.ReadOptionalSubViews(view.get());
	
	view->FinishInitPersistent();
	
	return view.release();

#else

	#pragma unused (inSuperView, inTree)
	PPx_SignalString_("SearchField requires 10.3");
	return NULL;

#endif // 10.3 or later
}


// ---------------------------------------------------------------------------
//	CreateSegmentedView
/**
	+++
	
	@param	inSuperView		+++
	@param	inTree			+++
	
	@return	Pointer to newly created object									*/

Persistent*
NibDecoderFuncs::CreateSegmentedView(
	View*				inSuperView,
	const CFXMLTree&	inTree)
{
#if (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_3)

	NibReader reader(inTree);

	HIRect frame;
	bool isVisible;
	bool isEnabled;
	reader.ReadCommonValues(inSuperView, frame, isVisible, isEnabled);

	std::auto_ptr<SegmentedView> view(new SegmentedView);

	view->Initialize(inSuperView, frame, isVisible, isEnabled);

	CFXMLTree segments;
	reader.ReadRequiredElement(NibConstants::elem_array, NibConstants::tag_segments, segments);
	CFIndex numSegments = segments.GetChildCount();

	view->SetSegmentCount(numSegments);
	
	for (CFIndex index = 1; index <= numSegments; index++)
	{
		CFXMLTree segmentTree(segments.GetChildAtIndex(index - 1));
		CFXMLNode segmentNode(segmentTree.GetNode());

		PPx_SignalIfNot_((segmentNode.GetTypeCode() == kCFXMLNodeTypeElement) and
			(segmentNode.GetString() == NibConstants::elem_object));

		NibReader segmentReader(segmentTree);
		
		OptionBits attributes = kHISegmentNoAttributes;
		segmentReader.ReadOptionalInt(NibConstants::tag_Attributes, attributes);
		view->ChangeSegmentAttributes(index, attributes, 0);
		
		SInt32 value = 0;
		segmentReader.ReadOptionalInt(NibConstants::tag_value, value);
		view->SetSegmentValue(index, value);
		
		HISegmentBehavior behavior = kHISegmentBehaviorMomentary;
		segmentReader.ReadOptionalInt(NibConstants::tag_behavior, behavior);
		view->SetSegmentBehavior(index, behavior);
		
		bool autoWidth = true;
		segmentReader.ReadOptionalBool(NibConstants::tag_autoWidth, autoWidth);
		float width = 0.0;
		segmentReader.ReadOptionalFloat(NibConstants::tag_width, width);
		view->SetSegmentContentWidth(index, autoWidth, width);
		
		bool segmentEnabled = true;
		segmentReader.ReadOptionalBool(NibConstants::tag_enabled, segmentEnabled);
		view->SetSegmentEnabled(index, segmentEnabled);

		CFString label("");
		segmentReader.ReadOptionalString(NibConstants::tag_label, label);
		view->SetSegmentLabel(index, label);
		
		OSType cmd = 0;
		if (segmentReader.ReadOptionalOSType(NibConstants::tag_command, cmd))
			view->SetSegmentCommand(index, cmd);

// ### Does IB ever set anything up for the image?
//		HIViewImageContentInfo imageInfo;
//		view->SetSegmentImage(index, imageInfo);
	}

	reader.ReadOptionalControlInfo(view.get());
	reader.ReadOptionalLayoutInfo(view.get());
	reader.ReadOptionalHelpInfo(view.get());
	reader.ReadOptionalSubViews(view.get());
	
	view->FinishInitPersistent();
	
	return view.release();

#else

	#pragma unused (inSuperView, inTree)
	PPx_SignalString_("SegmentedView requires 10.3");
	return NULL;

#endif // 10.3 or later
}


// ---------------------------------------------------------------------------
//	CreateSeparator
/**
	+++
	
	@param	inSuperView		+++
	@param	inTree			+++
	
	@return	Pointer to newly created object									*/

Persistent*
NibDecoderFuncs::CreateSeparator(
	View*				inSuperView,
	const CFXMLTree&	inTree)
{
	NibReader reader(inTree);

	HIRect frame;
	bool isVisible;
	bool isEnabled;
	reader.ReadCommonValues(inSuperView, frame, isVisible, isEnabled);

	std::auto_ptr<SeparatorLine> view(new SeparatorLine);

	view->Initialize(inSuperView, frame, isVisible, isEnabled);

	reader.ReadOptionalControlInfo(view.get());
	reader.ReadOptionalLayoutInfo(view.get());
	reader.ReadOptionalHelpInfo(view.get());
	reader.ReadOptionalSubViews(view.get());
	
	view->FinishInitPersistent();
	
	return view.release();
}


// ---------------------------------------------------------------------------
//	CreateSlider
/**
	+++
	
	@param	inSuperView		+++
	@param	inTree			+++
	
	@return	Pointer to newly created object									*/

Persistent*
NibDecoderFuncs::CreateSlider(
	View*				inSuperView,
	const CFXMLTree&	inTree)
{
	NibReader reader(inTree);

	HIRect frame;
	bool isVisible;
	bool isEnabled;
	reader.ReadCommonValues(inSuperView, frame, isVisible, isEnabled);

	SInt32 initialValue = 0;
	reader.ReadOptionalInt(NibConstants::tag_initialValue, initialValue);
	
	SInt32 minValue = 0;
	reader.ReadOptionalInt(NibConstants::tag_minimumValue, minValue);
	
	SInt32 maxValue = 100;
	reader.ReadOptionalInt(NibConstants::tag_maximumValue, maxValue);
	
	ControlSliderOrientation orientation = kControlSliderPointsDownOrRight;
	reader.ReadOptionalInt(NibConstants::tag_orientation, orientation);

	UInt16 numTickMarks = 0;
	reader.ReadOptionalInt(NibConstants::tag_numTickMarks, numTickMarks);

	bool hasLiveTracking = true;
	reader.ReadOptionalBool(NibConstants::tag_isLive, hasLiveTracking);
	
	std::auto_ptr<Slider> view(new Slider);

	view->Initialize(inSuperView, frame, isVisible, isEnabled, initialValue,
		minValue, maxValue, orientation, numTickMarks, hasLiveTracking);

	ControlSize controlSize = kControlSizeNormal;
	if (reader.ReadOptionalInt(NibConstants::tag_controlSize, controlSize))
		view->SetDataTag(kControlEntireControl, kControlSizeTag,
			sizeof(controlSize), &controlSize);

	reader.ReadOptionalControlInfo(view.get());
	reader.ReadOptionalLayoutInfo(view.get());
	reader.ReadOptionalHelpInfo(view.get());
	reader.ReadOptionalSubViews(view.get());
	
	view->FinishInitPersistent();
	
	return view.release();
}


// ---------------------------------------------------------------------------
//	CreateStaticText
/**
	+++
	
	@param	inSuperView		+++
	@param	inTree			+++
	
	@return	Pointer to newly created object									*/

Persistent*
NibDecoderFuncs::CreateStaticText(
	View*				inSuperView,
	const CFXMLTree&	inTree)
{
	NibReader reader(inTree);

	HIRect frame;
	bool isVisible;
	bool isEnabled;
	reader.ReadCommonValues(inSuperView, frame, isVisible, isEnabled);

	CFString title("");
	reader.ReadOptionalString(NibConstants::tag_title, title);
	
	ControlFontStyleRec style = { 0 };
	
	if (reader.ReadOptionalInt(NibConstants::tag_justification, style.just))
		style.flags |= kControlUseJustMask;

	ControlSize controlSize = kControlSizeNormal;
	if (reader.ReadOptionalInt(NibConstants::tag_controlSize, controlSize))
	{
		if (controlSize == kControlSizeSmall)
		{
			style.flags |= kControlUseFontMask;
			style.font = kControlFontSmallSystemFont;
		}

#if (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_3)

		else if (controlSize == kControlSizeMini)
		{
			style.flags |= kControlUseFontMask;
			style.font = kControlFontMiniSystemFont;
		}

#endif // 10.3 or later

	}

	SInt32 fontStyle = 0;
	if (reader.ReadOptionalInt(NibConstants::tag_fontStyle, fontStyle))
	{
		if (fontStyle < 0)
		{
			// Meta-font numbering
			style.flags |= kControlUseFontMask;
			style.font = fontStyle;
		}
		else
		{
			// ThemeFontID
			style.flags |= kControlUseThemeFontIDMask;
			style.font = fontStyle;
		}
	}

	std::auto_ptr<StaticText> view(new StaticText);

	view->Initialize(inSuperView, frame, isVisible, isEnabled, title, &style);

	reader.ReadOptionalControlInfo(view.get());
	reader.ReadOptionalLayoutInfo(view.get());
	reader.ReadOptionalHelpInfo(view.get());
	reader.ReadOptionalSubViews(view.get());
	
	view->FinishInitPersistent();
	
	return view.release();
}


// ---------------------------------------------------------------------------
//	CreateTab
/**
	+++
	
	@param	inSuperView		+++
	@param	inTree			+++
	
	@return	Pointer to newly created object									*/

Persistent*
NibDecoderFuncs::CreateTab(
	View*				inSuperView,
	const CFXMLTree&	inTree)
{
	NibReader reader(inTree);

	HIRect frame;
	bool isVisible;
	bool isEnabled;
	reader.ReadCommonValues(inSuperView, frame, isVisible, isEnabled);

	ControlTabSize tabSize = kControlTabSizeLarge;
	reader.ReadOptionalInt(NibConstants::tag_controlSize, tabSize);

	ControlTabDirection tabDirection = kControlTabDirectionNorth;
	reader.ReadOptionalInt(NibConstants::tag_direction, tabDirection);
	
	CFXMLTree tabItems;
	reader.ReadRequiredElement(NibConstants::elem_array, NibConstants::tag_tabItems, tabItems);
	SInt32 numTabItems = tabItems.GetChildCount();
	
	std::vector<ControlTabEntry> tabEntries(numTabItems);
	std::vector<ControlButtonContentInfo> iconEntries(numTabItems);
	std::vector<CFString> userPanes(numTabItems);
	std::vector<ControlID> userPaneIDs(numTabItems);
	
	for (UInt16 index = 0; index < numTabItems; index++)
	{
		tabEntries[index].icon = NULL;
		tabEntries[index].name = NULL;
		tabEntries[index].enabled = true;

		// Each array item maps to one dictionary per tab
		CFXMLTree tabItemTree(tabItems.GetChildAtIndex(index));
		CFIndex tabItemCount = tabItemTree.GetChildCount();
		for (CFIndex tabItemIndex = 0; tabItemIndex < tabItemCount; tabItemIndex++)
		{
			CFXMLTree itemTree(tabItemTree.GetChildAtIndex(tabItemIndex));
			CFXMLNode itemNode(itemTree.GetNode());
			CFString itemNodeString = itemNode.GetString();
			CFXMLNodeTypeCode typeCode = itemNode.GetTypeCode();
			if ((typeCode == kCFXMLNodeTypeElement) and (itemNodeString == NibConstants::elem_string))
			{
				CFXMLTree stringTree(itemTree.GetChildAtIndex(0));
				CFXMLNode stringNode(stringTree.GetNode());
				if (stringNode.GetTypeCode() == kCFXMLNodeTypeText)
				{
					CFString string = stringNode.GetString();
					if (tabItemIndex == 1)
					{
						SInt16 iconSuiteID = 0;
						iconSuiteID = string.GetNumericValue<SInt16>();
						if (iconSuiteID != 0)
						{
							iconEntries[index].contentType = kControlContentIconSuiteRes;
							iconEntries[index].u.resID = iconSuiteID;
							
							tabEntries[index].icon = &iconEntries[index];
						}
					}
					else if (tabItemIndex == 3)
					{
						// value of tabEnabled
						tabEntries[index].enabled = (string == NibConstants::value_1);
					}
					else if (tabItemIndex == 5)
					{
						// value of tabName
						tabEntries[index].name = string.DetachRef();
					}
				}
			}
			else if ((typeCode == kCFXMLNodeTypeElement) and (itemNodeString == NibConstants::elem_reference))
			{
				if (tabItemIndex == 7)
				{
					// id of the user pane
					CFXMLElement elementInfo(itemNode);
					CFString idRefValue(elementInfo.GetAttributeValue(NibConstants::attr_idRef), retain_Yes);
					if (idRefValue.IsValid())
					{
						userPanes[index] = idRefValue;
					}
				}
			}
		}
	}

	CFXMLTree subViewsTree;
	if (reader.ReadOptionalElement(NibConstants::elem_array, NibConstants::tag_subviews, subViewsTree))
	{
		SInt32 userPanesCount = userPanes.size();
		for (SInt32 index = 0; index < userPanesCount; index++)
		{
			CFString idRefString = userPanes[index];

			CFIndex childCount = subViewsTree.GetChildCount();
			for (CFIndex childIndex = 0; childIndex < childCount; childIndex++)
			{
				CFXMLTree childTree(subViewsTree.GetChildAtIndex(childIndex));
				CFXMLNode childNode(childTree.GetNode());
				if ((childNode.GetTypeCode() != kCFXMLNodeTypeElement) or
					(childNode.GetString() != NibConstants::elem_object))
					continue;

				CFXMLElement childElem(childNode);
				CFString idStr(childElem.GetAttributeValue(NibConstants::attr_id));
				if (idStr.IsValid() and (idStr == idRefString))
				{
					NibReader userPaneReader(childTree);
					
					ControlID controlID = { 0, 0 };
					userPaneReader.ReadOptionalOSType(NibConstants::tag_controlSignature, controlID.signature);
					userPaneReader.ReadOptionalInt(NibConstants::tag_controlID, controlID.id);
					
					userPaneIDs[index] = controlID;
				}
			}
		}
	}

	// Just use a regular PPx::TabView for now
	// ### We need to decide on how to use the userPaneIDs so that when a user
	// switches between tabs the appropriate pane is shown.  The biggest problem
	// I see is having the system continue to work when tabs are add/removed at
	// runtime.
	std::auto_ptr<TabView> view(new TabView);

	view->Initialize(inSuperView, frame, isVisible, isEnabled, tabSize,
		tabDirection, tabEntries.size(), &tabEntries[0]);

	SInt32 tabSelection = 1;
	reader.ReadOptionalInt(NibConstants::tag_tabSelection, tabSelection);
	view->SetValue(tabSelection);

	reader.ReadOptionalControlInfo(view.get());
	reader.ReadOptionalLayoutInfo(view.get());
	reader.ReadOptionalHelpInfo(view.get());
	reader.ReadOptionalSubViews(view.get());
	
	view->FinishInitPersistent();
	
	return view.release();
}


// ---------------------------------------------------------------------------
//	CreateTextView
/**
	+++
	
	@param	inSuperView		+++
	@param	inTree			+++
	
	@return	Pointer to newly created object									*/

Persistent*
NibDecoderFuncs::CreateTextView(
	View*				inSuperView,
	const CFXMLTree&	inTree)
{
#if (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_3)

	NibReader reader(inTree);

	HIRect frame;
	bool isVisible;
	bool isEnabled;
	reader.ReadCommonValues(inSuperView, frame, isVisible, isEnabled);

	TXNFrameOptions frameOptions = 0;
	
	bool readOnly = false;
	reader.ReadOptionalBool(NibConstants::tag_readOnly, readOnly);
	if (readOnly)
		frameOptions |= kTXNReadOnlyMask;

	bool noSelection = false;
	reader.ReadOptionalBool(NibConstants::tag_noSelection, noSelection);
	if (noSelection)
		frameOptions |= kTXNNoSelectionMask;

	bool outputInUnicode = false;
	reader.ReadOptionalBool(NibConstants::tag_outputInUnicode, outputInUnicode);
	if (outputInUnicode)
		frameOptions |= kOutputTextInUnicodeEncodingMask;

	bool noDragProcs = false;
	reader.ReadOptionalBool(NibConstants::tag_noDragProcs, noDragProcs);
	if (noDragProcs)
		frameOptions |= kTXNDoNotInstallDragProcsMask;

	bool wrap = false;
	reader.ReadOptionalBool(NibConstants::tag_wrap, wrap);
	if (wrap)
		frameOptions |= kTXNAlwaysWrapAtViewEdgeMask;

	bool noCaretWhenInactive = false;
	reader.ReadOptionalBool(NibConstants::tag_noCaretWhenInactive, noCaretWhenInactive);
	if (noCaretWhenInactive)
		frameOptions |= kTXNDontDrawCaretWhenInactiveMask;

	bool noSelectionWhenInactive = false;
	reader.ReadOptionalBool(NibConstants::tag_noSelectionWhenInactive, noSelectionWhenInactive);
	if (noSelectionWhenInactive)
		frameOptions |= kTXNDontDrawSelectionWhenInactiveMask;

	bool singleLine = false;
	reader.ReadOptionalBool(NibConstants::tag_singleLine, singleLine);
	if (singleLine)
		frameOptions |= kTXNSingleLineOnlyMask;

	bool disableDragAndDrop = false;
	reader.ReadOptionalBool(NibConstants::tag_disableDragAndDrop, disableDragAndDrop);
	if (disableDragAndDrop)
		frameOptions |= kTXNDisableDragAndDropMask;

	bool monoStyle = false;
	reader.ReadOptionalBool(NibConstants::tag_monoStyle, monoStyle);
	if (monoStyle)
		frameOptions |= kTXNMonostyledTextMask;

	bool fontSubstitution = false;
	reader.ReadOptionalBool(NibConstants::tag_fontSubstitution, fontSubstitution);
	if (fontSubstitution)
		frameOptions |= kTXNDoFontSubstitutionMask;

	std::auto_ptr<TextView> view(new TextView);

	view->Initialize(inSuperView, frame, isVisible, isEnabled, 0, frameOptions);

	reader.ReadOptionalControlInfo(view.get());
	reader.ReadOptionalLayoutInfo(view.get());
	reader.ReadOptionalHelpInfo(view.get());
	reader.ReadOptionalSubViews(view.get());
	
	view->FinishInitPersistent();
	
	return view.release();

#else

	#pragma unused (inSuperView, inTree)
	PPx_SignalString_("TextView requires 10.3");
	return NULL;

#endif // 10.3 or later
}


// ---------------------------------------------------------------------------
//	CreateTriangle
/**
	+++
	
	@param	inSuperView		+++
	@param	inTree			+++
	
	@return	Pointer to newly created object									*/

Persistent*
NibDecoderFuncs::CreateTriangle(
	View*				inSuperView,
	const CFXMLTree&	inTree)
{
	NibReader reader(inTree);

	HIRect frame;
	bool isVisible;
	bool isEnabled;
	reader.ReadCommonValues(inSuperView, frame, isVisible, isEnabled);

	CFString title("");
	reader.ReadOptionalString(NibConstants::tag_title, title);
	
	SInt32 initialValue = 0;
	bool isCollapsed = true;
	reader.ReadOptionalBool(NibConstants::tag_collapsed, isCollapsed);
	if (not isCollapsed)
		initialValue = 1;

	bool drawTitle = false;
	reader.ReadOptionalBool(NibConstants::tag_drawTitle, drawTitle);

	bool autoToggle = true;
	reader.ReadOptionalBool(NibConstants::tag_autoToggle, autoToggle);
	
	ControlDisclosureTriangleOrientation orientation = kControlDisclosureTrianglePointDefault;
	reader.ReadOptionalInt(NibConstants::tag_orientation, orientation);

	std::auto_ptr<DisclosureTriangle> view(new DisclosureTriangle);

	view->Initialize(inSuperView, frame, isVisible, isEnabled, orientation, title,
		initialValue, drawTitle, autoToggle);

	reader.ReadOptionalControlInfo(view.get());
	reader.ReadOptionalLayoutInfo(view.get());
	reader.ReadOptionalHelpInfo(view.get());
	reader.ReadOptionalSubViews(view.get());
	
	view->FinishInitPersistent();
	
	return view.release();
}


// ---------------------------------------------------------------------------
//	CreateUserPane
/**
	+++
	
	@param	inSuperView		+++
	@param	inTree			+++
	
	@return	Pointer to newly created object									*/

Persistent*
NibDecoderFuncs::CreateUserPane(
	View*				inSuperView,
	const CFXMLTree&	inTree)
{
	NibReader reader(inTree);

	HIRect frame;
	bool isVisible;
	bool isEnabled;
	reader.ReadCommonValues(inSuperView, frame, isVisible, isEnabled);

	OptionBits features = 0;
	if (not reader.ReadOptionalInt(NibConstants::tag_featureBits, features))
		reader.ReadOptionalInt(NibConstants::tag_featureBits, features);
	
	std::auto_ptr<UserPane> view(new UserPane);

	view->Initialize(inSuperView, frame, isVisible, isEnabled, features);

	reader.ReadOptionalControlInfo(view.get());
	reader.ReadOptionalLayoutInfo(view.get());
	reader.ReadOptionalHelpInfo(view.get());
	reader.ReadOptionalSubViews(view.get());
	
	view->FinishInitPersistent();
	
	return view.release();
}


} // namespace PPx
