// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxNibConstants.h
//
//	Copyright © 2004-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:21 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxNibConstants.h
	@brief		Constants for Nib identifiers
*/

#ifndef H_PPxNibConstants
#define H_PPxNibConstants
#pragma once

#include <CoreFoundation/CFString.h>

namespace PPx {

// ===========================================================================
//	NibConstants
/**
	Constants for Nib identifiers */

namespace NibConstants {

	extern const CFStringRef	elem_array;
	extern const CFStringRef	elem_boolean;
	extern const CFStringRef	elem_dictionary;
	extern const CFStringRef	elem_float;
	extern const CFStringRef	elem_int;
	extern const CFStringRef	elem_NSIBObjectData;
	extern const CFStringRef	elem_object;
	extern const CFStringRef	elem_ostype;
	extern const CFStringRef	elem_reference;
	extern const CFStringRef	elem_string;
	extern const CFStringRef	elem_unsigned_int;
	
	extern const CFStringRef	attr_class;
	extern const CFStringRef	attr_count;
	extern const CFStringRef	attr_id;
	extern const CFStringRef	attr_idRef;
	extern const CFStringRef	attr_name;

	extern const CFStringRef	value_1;
	extern const CFStringRef	value_allObjects;
	extern const CFStringRef	value_FALSE;
	extern const CFStringRef	value_nameTable;
	extern const CFStringRef	value_rootControl;
	extern const CFStringRef	value_subviews;
	extern const CFStringRef	value_TRUE;

	extern const CFStringRef	tag_alpha;
	extern const CFStringRef	tag_allowGrowBox;
	extern const CFStringRef	tag_Attributes;
	extern const CFStringRef	tag_autoCompletion;
	extern const CFStringRef	tag_autoDisable;
	extern const CFStringRef	tag_autoDisclosure;
	extern const CFStringRef	tag_autoHideScrollBars;
	extern const CFStringRef	tag_autoLayoutPanes;
	extern const CFStringRef	tag_autoRepeat;
	extern const CFStringRef	tag_autoSizeList;
	extern const CFStringRef	tag_autoSort;
	extern const CFStringRef	tag_autoToggle;
	extern const CFStringRef	tag_autoWidth;
	extern const CFStringRef	tag_auxiliaryDictionary;
	extern const CFStringRef	tag_behavior;
	extern const CFStringRef	tag_bevelThickness;
	extern const CFStringRef	tag_bindingBottomKind;
	extern const CFStringRef	tag_bindingLeftKind;
	extern const CFStringRef	tag_bindingRightKind;
	extern const CFStringRef	tag_bindingTopKind;
	extern const CFStringRef	tag_bounds;
	extern const CFStringRef	tag_buttonType;
	extern const CFStringRef	tag_checked;
	extern const CFStringRef	tag_classID;
	extern const CFStringRef	tag_clockType;
	extern const CFStringRef	tag_collapsed;
	extern const CFStringRef	tag_columnWidth;
	extern const CFStringRef	tag_command;
	extern const CFStringRef	tag_contentResID;
	extern const CFStringRef	tag_contentTransform;
	extern const CFStringRef	tag_contentType;
	extern const CFStringRef	tag_controlID;
	extern const CFStringRef	tag_controlSignature;
	extern const CFStringRef	tag_controlSize;
	extern const CFStringRef	tag_creator;
	extern const CFStringRef	tag_customDraw;
	extern const CFStringRef	tag_descriptiveText;
	extern const CFStringRef	tag_disabled;
	extern const CFStringRef	tag_direction;
	extern const CFStringRef	tag_disableDragAndDrop;
	extern const CFStringRef	tag_dontTrack;
	extern const CFStringRef	tag_drawTitle;
	extern const CFStringRef	tag_dropTarget;
	extern const CFStringRef	tag_dynamic;
	extern const CFStringRef	tag_enabled;
	extern const CFStringRef	tag_featureBits;
	extern const CFStringRef	tag_fontStyle;
	extern const CFStringRef	tag_fontSubstitution;
	extern const CFStringRef	tag_graphicAlignment;
	extern const CFStringRef	tag_graphicXOffset;
	extern const CFStringRef	tag_graphicYOffset;
	extern const CFStringRef	tag_hasCancelIcon;
	extern const CFStringRef	tag_helpTagDisplaySide;
	extern const CFStringRef	tag_helpTagExtendedText;
	extern const CFStringRef	tag_helpTagText;
	extern const CFStringRef	tag_hidden;
	extern const CFStringRef	tag_hiliteStyle;
	extern const CFStringRef	tag_horzScrollBar;
	extern const CFStringRef	tag_iconCreator;
	extern const CFStringRef	tag_iconPosition;
	extern const CFStringRef	tag_iconType;
	extern const CFStringRef	tag_ignoreMeta;
	extern const CFStringRef	tag_includeInCmdKeyMatching;
	extern const CFStringRef	tag_increment;
	extern const CFStringRef	tag_indeterminate;
	extern const CFStringRef	tag_initialState;
	extern const CFStringRef	tag_initialValue;
	extern const CFStringRef	tag_intercellHeight;
	extern const CFStringRef	tag_isEditable;
	extern const CFStringRef	tag_isLive;
	extern const CFStringRef	tag_isPassword;
	extern const CFStringRef	tag_isSingleLine;
	extern const CFStringRef	tag_isUnicode;
	extern const CFStringRef	tag_items;
	extern const CFStringRef	tag_justification;
	extern const CFStringRef	tag_keyEquivalent;
	extern const CFStringRef	tag_keyEquivalentModifier;
	extern const CFStringRef	tag_label;
	extern const CFStringRef	tag_layoutInfo;
	extern const CFStringRef	tag_layoutSuspended;
	extern const CFStringRef	tag_list;
	extern const CFStringRef	tag_listViewColumns;
	extern const CFStringRef	tag_listViewHeaderBtnHeight;
	extern const CFStringRef	tag_maximumValue;
	extern const CFStringRef	tag_maxWidth;
	extern const CFStringRef	tag_menu;
	extern const CFStringRef	tag_menuBehavior;
	extern const CFStringRef	tag_menuPlacement;
	extern const CFStringRef	tag_minimumValue;
	extern const CFStringRef	tag_minWidth;
	extern const CFStringRef	tag_monoStyle;
	extern const CFStringRef	tag_noCaretWhenInactive;
	extern const CFStringRef	tag_noDragProcs;
	extern const CFStringRef	tag_noSelection;
	extern const CFStringRef	tag_noSelectionWhenInactive;
	extern const CFStringRef	tag_notPreviousAlternate;
	extern const CFStringRef	tag_numCols;
	extern const CFStringRef	tag_numRows;
	extern const CFStringRef	tag_numTickMarks;
	extern const CFStringRef	tag_opaque;
	extern const CFStringRef	tag_orientation;
	extern const CFStringRef	tag_outputInUnicode;
	extern const CFStringRef	tag_parameters;
	extern const CFStringRef	tag_primary;
	extern const CFStringRef	tag_propertyFlags;
	extern const CFStringRef	tag_propertyID;
	extern const CFStringRef	tag_propertyType;
	extern const CFStringRef	tag_radioTitles;
	extern const CFStringRef	tag_readOnly;
	extern const CFStringRef	tag_rounded;
	extern const CFStringRef	tag_rowHeight;
	extern const CFStringRef	tag_scaleToFit;
	extern const CFStringRef	tag_scrollBarInset;
	extern const CFStringRef	tag_scrollHorizontally;
	extern const CFStringRef	tag_scrollVertically;
	extern const CFStringRef	tag_sectionHeader;
	extern const CFStringRef	tag_segments;
	extern const CFStringRef	tag_selectedCol;
	extern const CFStringRef	tag_selectedItem;
	extern const CFStringRef	tag_selectedRow;
	extern const CFStringRef	tag_selectionFlags;
	extern const CFStringRef	tag_separator;
	extern const CFStringRef	tag_showFocusRing;
	extern const CFStringRef	tag_singleLine;
	extern const CFStringRef	tag_size;
	extern const CFStringRef	tag_small;
	extern const CFStringRef	tag_sortOrder;
	extern const CFStringRef	tag_submenuParentChoosable;
	extern const CFStringRef	tag_subviews;
	extern const CFStringRef	tag_tabItems;
	extern const CFStringRef	tag_tabSelection;
	extern const CFStringRef	tag_textAlignment;
	extern const CFStringRef	tag_textOffset;
	extern const CFStringRef	tag_textPlacement;
	extern const CFStringRef	tag_title;
	extern const CFStringRef	tag_titleJustification;
	extern const CFStringRef	tag_titleOffset;
	extern const CFStringRef	tag_titleString;
	extern const CFStringRef	tag_titleType;
	extern const CFStringRef	tag_transform;
	extern const CFStringRef	tag_updateSingleItem;
	extern const CFStringRef	tag_useInlineInput;
	extern const CFStringRef	tag_value;
	extern const CFStringRef	tag_variableHeightRows;
	extern const CFStringRef	tag_variableWidth;
	extern const CFStringRef	tag_variableWidthColumns;
	extern const CFStringRef	tag_variableWith;
	extern const CFStringRef	tag_vertScrollBar;
	extern const CFStringRef	tag_viewFrame;
	extern const CFStringRef	tag_width;
	extern const CFStringRef	tag_wrap;

} // namespace NibConstants


} // namespace PPx

#endif // H_PPxNibConstants