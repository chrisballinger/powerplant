// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxNibReader.cp
//
//	Copyright © 2004-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 05:04:49 $
//	$Revision: 1.2 $
// ===========================================================================

#include <PPxNibDecoderUtils.h>
#include <PPxNibReader.h>
#include <PPxNibRegistrar.h>
#include <SysIcon.h>
#include <SysMenu.h>

namespace PPx {


// ---------------------------------------------------------------------------
//	NibReader														  [public]
/**
	+++
	
	@param	inTree	+++						*/

NibReader::NibReader( const CFXMLTree& inTree )
	
	: mXMLTree(inTree)
{
	PPx_SignalIf_(not mXMLTree.IsValid());
}


// ---------------------------------------------------------------------------
//	ReadOptionalElement
/**
	+++
	
	@param	inElemType		+++
	@param	inName			Key name of item
	@param	outValue		+++

	@return Whether an item with the key name exists						*/

bool
NibReader::ReadOptionalElement(
	CFStringRef	inElemType,
	CFStringRef	inName,
	CFXMLTree&	outValue) const
{
	CFIndex childCount = mXMLTree.GetChildCount();
	for (CFIndex index = 0; index < childCount; index++)
	{
		CFXMLTree childTree(mXMLTree.GetChildAtIndex(index));
		CFXMLNode childNode(childTree.GetNode());
		
		if ((childNode.GetTypeCode() == kCFXMLNodeTypeElement) and
			(childNode.GetString() == inElemType))
		{
			CFXMLElement childElem(childNode);
			CFString nameStr(childElem.GetAttributeValue(NibConstants::attr_name));
			if (nameStr.IsValid() and (nameStr == inName))
			{
				outValue = childTree;
				return true;
			}
		}
	}
	return false;
}


// ---------------------------------------------------------------------------
//	ReadRequiredElement
/**
	+++
	
	@param	inElemType		+++
	@param	inName			Key name of item
	@param	outValue		+++												*/

void
NibReader::ReadRequiredElement(
	CFStringRef	inElemType,
	CFStringRef	inName,
	CFXMLTree&	outValue) const
{
	if (not ReadOptionalElement(inElemType, inName, outValue))
		PPx_Throw_(DataError, err_MissingData, "Required value not found");
}


// ---------------------------------------------------------------------------
//	ReadOptionalString
/**
	+++
	
	@param	inName			Key name of item
	@param	outValue		+++

	@return Whether an item with the key name exists						*/

bool
NibReader::ReadOptionalString(
	CFStringRef	inName,
	CFString&	outValue) const
{
	CFXMLTree stringTree;
	if (ReadOptionalElement(NibConstants::elem_string, inName, stringTree))
	{
		CFString stringStr(XMLTreeBrowser::GetValue<CFString>(stringTree));
		if (stringStr.IsValid())
		{
			outValue = stringStr;
			return true;
		}
		PPx_SignalString_("A problem occurred getting the value");
		return false;
	}
	return false;
}

// ---------------------------------------------------------------------------
//	ReadRequiredString
/**
	+++
	
	@param	inName			Key name of item
	@param	outValue		+++												*/

void
NibReader::ReadRequiredString(
	CFStringRef	inName,
	CFString&	outValue) const
{
	if (not ReadOptionalString(inName, outValue))
		PPx_Throw_(DataError, err_MissingData, "Required value not found");
}


// ---------------------------------------------------------------------------
//	ReadOptionalBool
/**
	+++
	
	@param	inName			Key name of item
	@param	outValue		+++

	@return Whether an item with the key name exists						*/

bool
NibReader::ReadOptionalBool(
	CFStringRef	inName,
	bool&		outValue) const
{
	CFXMLTree boolTree;
	if (ReadOptionalElement(NibConstants::elem_boolean, inName, boolTree))
	{
		CFString boolStr(XMLTreeBrowser::GetValue<CFString>(boolTree));
		if (boolStr.IsValid())
		{
			if (boolStr == NibConstants::value_TRUE)
			{
				outValue = true;
				return true;
			}
			else if (boolStr == NibConstants::value_FALSE)
			{
				outValue = false;
				return true;
			}
			else
			{
				PPx_SignalString_("The value is neither 'TRUE' or 'FALSE' so we are assuming false");
				outValue = false;
				return true;
			}
		}
		PPx_SignalString_("A problem occurred getting the value");
		return false;
	}
	return false;
}


// ---------------------------------------------------------------------------
//	ReadRequiredBool
/**
	+++
	
	@param	inName			Key name of item
	@param	outValue		+++												*/

void
NibReader::ReadRequiredBool(
	CFStringRef	inName,
	bool&		outValue) const
{
	if (not ReadOptionalBool(inName, outValue))
		PPx_Throw_(DataError, err_MissingData, "Required value not found");
}


// ---------------------------------------------------------------------------
//	ReadOptionalBoolean
/**
	+++
	
	@param	inName			Key name of item
	@param	outValue		+++

	@return Whether an item with the key name exists						*/

bool
NibReader::ReadOptionalBoolean(
	CFStringRef	inName,
	Boolean&	outValue) const
{
	bool boolValue = false;
	if (ReadOptionalBool(inName, boolValue))
	{
		outValue = boolValue;
		return true;
	}
	return false;
}


// ---------------------------------------------------------------------------
//	ReadRequiredBoolean
/**
	+++
	
	@param	inName			Key name of item
	@param	outValue		+++												*/

void
NibReader::ReadRequiredBoolean(
	CFStringRef	inName,
	Boolean&	outValue) const
{
	if (not ReadOptionalBoolean(inName, outValue))
		PPx_Throw_(DataError, err_MissingData, "Required value not found");
}


// ---------------------------------------------------------------------------
//	ReadOptionalOSType
/**
	+++
	
	@param	inName			Key name of item
	@param	outValue		+++

	@return Whether an item with the key name exists						*/

bool
NibReader::ReadOptionalOSType(
	CFStringRef	inName,
	OSType&		outValue) const
{
	CFXMLTree ostypeTree;
	if (ReadOptionalElement(NibConstants::elem_ostype, inName, ostypeTree))
	{
		CFString ostypeStr(XMLTreeBrowser::GetValue<CFString>(ostypeTree));
		if (ostypeStr.IsValid())
		{
			outValue = ostypeStr.Get4CharCodeValue();
			return true;
		}
		PPx_SignalString_("A problem occurred getting the value");
		return false;
	}
	return false;
}


// ---------------------------------------------------------------------------
//	ReadRequiredOSType
/**
	+++
	
	@param	inName			Key name of item
	@param	outValue		+++												*/

void
NibReader::ReadRequiredOSType(
	CFStringRef	inName,
	OSType&		outValue) const
{
	if (not ReadOptionalOSType(inName, outValue))
		PPx_Throw_(DataError, err_MissingData, "Required value not found");
}


// ---------------------------------------------------------------------------
//	ReadOptionalFloat
/**
	+++
	
	@param	inName			Key name of item
	@param	outValue		+++

	@return Whether an item with the key name exists						*/

bool
NibReader::ReadOptionalFloat(
	CFStringRef	inName,
	float&		outValue) const
{
	CFXMLTree floatTree;
	if (ReadOptionalElement(NibConstants::elem_float, inName, floatTree))
	{
		CFString floatStr(XMLTreeBrowser::GetValue<CFString>(floatTree));
		if (floatStr.IsValid())
		{
			outValue = floatStr.GetNumericValue<float>();
			return true;
		}
		PPx_SignalString_("A problem occurred getting the value");
		return false;
	}
	return false;
}


// ---------------------------------------------------------------------------
//	ReadRequiredFloat
/**
	+++
	
	@param	inName			Key name of item
	@param	outValue		+++												*/

void
NibReader::ReadRequiredFloat(
	CFStringRef	inName,
	float&		outValue) const
{
	if (not ReadOptionalFloat(inName, outValue))
		PPx_Throw_(DataError, err_MissingData, "Required value not found");
}


// ---------------------------------------------------------------------------
//	ReadOptionalMenu
/**
	+++
	
	@param	inName			Key name of item
	@param	outValue		+++

	@return Whether an item with the key name exists						*/

bool
NibReader::ReadOptionalMenu(
	CFStringRef	inName,
	MenuRef&	outValue) const
{
	CFXMLTree menuTree;
	if (ReadOptionalElement(NibConstants::elem_object, inName, menuTree))
	{
		OSStatus status;
		NibReader menuReader(menuTree);
		
		MenuRef tempMenu = NULL;
		MenuID menuID = 0;
		MenuAttributes menuAttributes = 0;
		status = ::CreateNewMenu(menuID, menuAttributes, &tempMenu);
		PPx_ThrowIfOSError_(status, "CreateNewMenu failed");
		AutoMenu menu(tempMenu);

		CFString menuTitle("");
		menuReader.ReadOptionalString(NibConstants::tag_title, menuTitle);
		status = SetMenuTitleWithCFString(menu, menuTitle);
		PPx_ThrowIfOSError_(status, "SetMenuTitleWithCFString failed");
		
		CFXMLTree menuItems;
		menuReader.ReadOptionalElement(NibConstants::elem_array, NibConstants::tag_items, menuItems);
		if (menuItems.IsValid())
		{
			CFIndex itemCount = menuItems.GetChildCount();
			for (CFIndex index = 0; index < itemCount; index++)
			{
				CFXMLTree menuItemTree(menuItems.GetChildAtIndex(index));
				NibReader itemReader(menuItemTree);
				
				CFString itemTitle("");
				itemReader.ReadOptionalString(NibConstants::tag_title, itemTitle);

				MenuCommand cmdID = 0;
				itemReader.ReadOptionalOSType(NibConstants::tag_command, cmdID);

				MenuItemAttributes itemAttributes = 0;

				bool isChecked = false;
				itemReader.ReadOptionalBool(NibConstants::tag_checked, isChecked);
				// ### How should isChecked be used?

				bool submenuParentChoosable = false;
				itemReader.ReadOptionalBool(NibConstants::tag_submenuParentChoosable, submenuParentChoosable);
				if (submenuParentChoosable)
					itemAttributes |= kMenuItemAttrSubmenuParentChoosable;
				
				bool dynamic = false;
				itemReader.ReadOptionalBool(NibConstants::tag_dynamic, dynamic);
				if (dynamic)
					itemAttributes |= kMenuItemAttrDynamic;
				
				bool notPreviousAlternate = false;
				itemReader.ReadOptionalBool(NibConstants::tag_notPreviousAlternate, notPreviousAlternate);
				if (notPreviousAlternate)
					itemAttributes |= kMenuItemAttrNotPreviousAlternate;
				
				bool menuItemHidden = false;
				itemReader.ReadOptionalBool(NibConstants::tag_hidden, menuItemHidden);
				if (menuItemHidden)
					itemAttributes |= kMenuItemAttrHidden;
				
				bool ignoreMeta = false;
				itemReader.ReadOptionalBool(NibConstants::tag_ignoreMeta, ignoreMeta);
				if (ignoreMeta)
					itemAttributes |= kMenuItemAttrIgnoreMeta;
				
				bool sectionHeader = false;
				itemReader.ReadOptionalBool(NibConstants::tag_sectionHeader, sectionHeader);
				if (sectionHeader)
					itemAttributes |= kMenuItemAttrSectionHeader;
				
				bool customDraw = false;
				itemReader.ReadOptionalBool(NibConstants::tag_customDraw, customDraw);
				if (customDraw)
					itemAttributes |= kMenuItemAttrCustomDraw;
				
				bool autoRepeat = false;
				itemReader.ReadOptionalBool(NibConstants::tag_autoRepeat, autoRepeat);
				if (autoRepeat)
					itemAttributes |= kMenuItemAttrAutoRepeat;
				
				bool includeInCmdKeyMatching = false;
				itemReader.ReadOptionalBool(NibConstants::tag_includeInCmdKeyMatching, includeInCmdKeyMatching);
				if (includeInCmdKeyMatching)
					itemAttributes |= kMenuItemAttrIncludeInCmdKeyMatching;
				
				bool disabled = false;
				itemReader.ReadOptionalBool(NibConstants::tag_disabled, disabled);
				if (disabled)
					itemAttributes |= kMenuItemAttrDisabled;
				
				bool separator = false;
				itemReader.ReadOptionalBool(NibConstants::tag_separator, separator);
				if (separator)
					itemAttributes |= kMenuItemAttrSeparator;

				bool useVirtualKey = false;
				// ### How does IB use "useVirtualKey"?
				//itemReader.ReadOptionalBool(NibConstants::tag_useVirtualKey, useVirtualKey);
				//if (useVirtualKey)
				//	itemAttributes |= kMenuItemAttrUseVirtualKey;

				// ### Does IB do anything with kMenuItemAttrIconDisabled?

#if (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_3)

				bool updateSingleItem = false;
				itemReader.ReadOptionalBool(NibConstants::tag_updateSingleItem, updateSingleItem);
				if (updateSingleItem)
					itemAttributes |= kMenuItemAttrUpdateSingleItem;

				bool autoDisable = false;
				itemReader.ReadOptionalBool(NibConstants::tag_autoDisable, autoDisable);
				if (autoDisable)
					itemAttributes |= kMenuItemAttrAutoDisable;
				
#endif // 10.3 or later

				MenuItemIndex newItemIndex = 0;
				status = AppendMenuItemTextWithCFString(menu, itemTitle, itemAttributes, cmdID, &newItemIndex);
				PPx_ThrowIfOSError_(status, "AppendMenuItemTextWithCFString failed");

				if (status == noErr)
				{
					CFString keyEquivString("");
					if (itemReader.ReadOptionalString(NibConstants::tag_keyEquivalent, keyEquivString) and
						keyEquivString.GetLength())
					{
						UInt16 keyEquiv = keyEquivString.GetCharacterAt(0);
						if (std::islower(keyEquiv))
							keyEquiv = std::toupper(keyEquiv);
						status = SetMenuItemCommandKey(menu, newItemIndex, useVirtualKey, keyEquiv);
						PPx_ThrowIfOSError_(status, "SetMenuItemCommandKey failed");
					}
					UInt32 keyEquivModifiers = kMenuNoModifiers;
					if (itemReader.ReadOptionalInt(NibConstants::tag_keyEquivalentModifier, keyEquivModifiers))
					{
						// Interface Builder uses some strange constants here
						UInt8 modifiers = kMenuNoModifiers;
						if (keyEquivModifiers & 0x020000)
							modifiers |= kMenuShiftModifier;
						if (keyEquivModifiers & 0x080000)
							modifiers |= kMenuOptionModifier;
						if (keyEquivModifiers & 0x040000)
							modifiers |= kMenuControlModifier;
						if (not (keyEquivModifiers & 0x100000))
							modifiers |= kMenuNoCommandModifier;

						status = SetMenuItemModifiers(menu, newItemIndex, modifiers);
						PPx_ThrowIfOSError_(status, "SetMenuItemModifiers failed");
					}
				}
			}
		}
		outValue = menu.Release();
		return true;
	}
	return false;

}


// ---------------------------------------------------------------------------
//	ReadRequiredMenu
/**
	+++
	
	@param	inName			Key name of item
	@param	outValue		+++												*/

void
NibReader::ReadRequiredMenu(
	CFStringRef	inName,
	MenuRef&	outValue) const
{
	if (not ReadOptionalMenu(inName, outValue))
		PPx_Throw_(DataError, err_MissingData, "Required value not found");
}


// ---------------------------------------------------------------------------
//	ReadCommonValues
/**
	+++
	
	@param	xxxxx		+++												*/

void
NibReader::ReadCommonValues(
	const View*	/* inSuperView */,
	HIRect&		outFrame,
	bool&		outIsVisible,
	bool&		outIsEnabled) const
{
	CFString tempString;
	ReadRequiredString(NibConstants::tag_viewFrame, tempString);
	NibDecoderUtils::SetHIRectFromString(tempString, outFrame);

#if 0
// ### If the window does not have compositing turned on, IB uses tag_bounds
// instead of tag_viewFrame.
	if (ReadOptionalString(NibConstants::tag_viewFrame, tempString))
	{
		NibDecoderUtils::SetHIRectFromString(tempString, outFrame);
	}
	else
	{
		ReadRequiredString(NibConstants::tag_bounds, tempString);
		Rect bounds;
		NibDecoderUtils::SetQDRectFromString(tempString, bounds);
		ViewUtils::QDToHIRect(bounds, outFrame);
		if (inSuperView)
		{
			OSStatus status;
			HIViewRef contentViewRef;
			status = ::GetRootControl(::GetControlOwner(inSuperView->GetSysView()),
				&contentViewRef);
			PPx_ThrowIfOSError_(status, "GetRootControl failed");
			
			// Convert the window content view relative bounds to a super view relative HIRect
			status = ::HIViewConvertRect(&outFrame, contentViewRef, inSuperView->GetSysView());
			PPx_ThrowIfOSError_(status, "HIViewConvertRect failed");
		}
	}
#endif

	bool isHidden = false;
	ReadOptionalBool(NibConstants::tag_hidden, isHidden);
	outIsVisible = not isHidden;

	outIsEnabled = true;
	ReadOptionalBool(NibConstants::tag_enabled, outIsEnabled);
}

// ---------------------------------------------------------------------------

//	ReadOptionalControlInfo
/**
	+++
	
	@param	inView		+++												*/

void
NibReader::ReadOptionalControlInfo( View* inView ) const
{
	if (inView == NULL)
		PPx_Throw_(LogicError, err_BadParam, "inView is NULL");

	OSStatus status;
	
	
	OSType command = 0;
	if (ReadOptionalOSType(NibConstants::tag_command, command))
	{
		status = ::SetControlCommandID(inView->GetSysView(), command);
		PPx_ThrowIfOSError_(status, "SetControlCommandID failed");
	}
	
	ControlID controlID = { 0, 0 };
	bool setControlID = false;
	setControlID = ReadOptionalOSType(NibConstants::tag_controlSignature, controlID.signature);
	setControlID |= ReadOptionalInt(NibConstants::tag_controlID, controlID.id);
	if (setControlID)
	{
		status = ::SetControlID(inView->GetSysView(), &controlID);
		PPx_ThrowIfOSError_(status, "SetControlID failed");
		
		// Set the View's ID from controlID.id
		if (controlID.id != 0)
			inView->SetID(controlID.id);
	}

	CFXMLTree propertiesTree;
	if (ReadOptionalElement(NibConstants::elem_dictionary, NibConstants::tag_auxiliaryDictionary, propertiesTree))
	{
		CFIndex childCount = propertiesTree.GetChildCount();
		if (childCount != 8)
			PPx_Throw_(DataError, err_XMLFormat, "The 'auxiliaryDictionary' must have 8 children");
		
		const CFStringRef kCommaString = CFSTR(",");

		// ### This could get us into trouble if IB changes its output.
		// We're assuming that the arrays are always at the same indicies:
		//		dict[1] = creators array
		//		dict[3] = tags array
		//		dict[5] = types array
		//		dict[7] = values array
		CFXMLTree creatorsTree(propertiesTree.GetChildAtIndex(1));
		CFXMLTree tagsTree(propertiesTree.GetChildAtIndex(3));
		CFXMLTree typesTree(propertiesTree.GetChildAtIndex(5));
		CFXMLTree valuesTree(propertiesTree.GetChildAtIndex(7));

		childCount = creatorsTree.GetChildCount();
		PPx_ThrowIf_(childCount != tagsTree.GetChildCount(), DataError, err_XMLFormat, "number of children do not match");
		PPx_ThrowIf_(childCount != typesTree.GetChildCount(), DataError, err_XMLFormat, "number of children do not match");
		PPx_ThrowIf_(childCount != valuesTree.GetChildCount(), DataError, err_XMLFormat, "number of children do not match");
		
		for (CFIndex index = 0; index < childCount; index++)
		{
			UInt32 propertySize = 0;
			const void* propertyData = NULL;

			OSType propertyCreator = 0;
			CFXMLTree childCreatorTree(creatorsTree.GetChildAtIndex(index));
			CFString propertyCreatorString(XMLTreeBrowser::GetValue<CFString>(childCreatorTree));
			if (propertyCreatorString.IsValid())
				propertyCreator = propertyCreatorString.Get4CharCodeValue();

			OSType propertyTag = 0;
			CFXMLTree childTagTree(tagsTree.GetChildAtIndex(index));
			CFString propertyTagString(XMLTreeBrowser::GetValue<CFString>(childTagTree));
			if (propertyTagString.IsValid())
				propertyTag = propertyTagString.Get4CharCodeValue();

			CFXMLTree childValueTree(valuesTree.GetChildAtIndex(index));
			CFString propertyValueString(XMLTreeBrowser::GetValue<CFString>(childValueTree));

			SInt32 propertyType = 0;
			CFXMLTree childTypeTree(typesTree.GetChildAtIndex(index));
			CFString propertyTypeString(XMLTreeBrowser::GetValue<CFString>(childTypeTree));
			if (propertyTypeString.IsValid())
				propertyType = propertyTypeString.GetNumericValue<SInt32>();
			
			switch (propertyType)
			{
				case 0: // Unicode String
				{
					CFStringRef stringValue = propertyValueString;
					status = ::SetControlProperty(inView->GetSysView(), propertyCreator, propertyTag,
						sizeof(CFStringRef), &stringValue);
					PPx_ThrowIfOSError_(status, "SetControlProperty failed");
					break;
				}
				case 1: // C String
				{
					std::string stringValue;
					propertyValueString.GetString(stringValue, kCFStringEncodingASCII);
					status = ::SetControlProperty(inView->GetSysView(), propertyCreator, propertyTag,
						stringValue.size(), stringValue.c_str());
					PPx_ThrowIfOSError_(status, "SetControlProperty failed");
					break;
				}
				case 2: // Boolean
				{
					Boolean booleanValue = false;
					if ((propertyValueString == CFSTR("true")) or
						(propertyValueString == NibConstants::value_TRUE))
						booleanValue = true;
					status = ::SetControlProperty(inView->GetSysView(), propertyCreator, propertyTag,
						sizeof(Boolean), &booleanValue);
					PPx_ThrowIfOSError_(status, "SetControlProperty failed");
					break;
				}
				case 3: // CFRange
				{
					CFRange rangeValue = { 0, 0 };
					CFArray<CFStringRef> stringsArray(::CFStringCreateArrayBySeparatingStrings(NULL, propertyValueString, kCommaString), retain_No);
					if (stringsArray.IsValid() and (stringsArray.GetCount() >= 2))
					{
						rangeValue.location = ::CFStringGetIntValue(stringsArray.GetValueAt(0));
						rangeValue.length = ::CFStringGetIntValue(stringsArray.GetValueAt(1));
					}
					status = ::SetControlProperty(inView->GetSysView(), propertyCreator, propertyTag,
						sizeof(CFRange), &rangeValue);
					PPx_ThrowIfOSError_(status, "SetControlProperty failed");
					break;
				}
				case 4: // float
				{
					float floatValue = propertyValueString.GetNumericValue<float>();
					status = ::SetControlProperty(inView->GetSysView(), propertyCreator, propertyTag,
						sizeof(float), &floatValue);
					PPx_ThrowIfOSError_(status, "SetControlProperty failed");
					break;
				}
				case 5: // OSType
				{
					OSType ostypeValue = propertyValueString.Get4CharCodeValue();
					status = ::SetControlProperty(inView->GetSysView(), propertyCreator, propertyTag,
						sizeof(OSType), &ostypeValue);
					PPx_ThrowIfOSError_(status, "SetControlProperty failed");
					break;
				}
				case 6: // SInt32
				{
					SInt32 longValue = propertyValueString.GetNumericValue<SInt32>();
					status = ::SetControlProperty(inView->GetSysView(), propertyCreator, propertyTag,
						sizeof(SInt32), &longValue);
					PPx_ThrowIfOSError_(status, "SetControlProperty failed");
					break;
				}
				case 7: // UInt32
				{
					UInt32 ulongValue = propertyValueString.GetNumericValue<UInt32>();
					status = ::SetControlProperty(inView->GetSysView(), propertyCreator, propertyTag,
						sizeof(UInt32), &ulongValue);
					PPx_ThrowIfOSError_(status, "SetControlProperty failed");
					break;
				}
				case 8: // HIPoint
				{
					HIPoint pointValue = { 0, 0 };
					CFArray<CFStringRef> stringsArray(::CFStringCreateArrayBySeparatingStrings(NULL, propertyValueString, kCommaString), retain_No);
					if (stringsArray.IsValid() and (stringsArray.GetCount() >= 2))
					{
						pointValue.x = ::CFStringGetDoubleValue(stringsArray.GetValueAt(0));
						pointValue.y = ::CFStringGetDoubleValue(stringsArray.GetValueAt(1));
					}
					status = ::SetControlProperty(inView->GetSysView(), propertyCreator, propertyTag,
						sizeof(HIPoint), &pointValue);
					PPx_ThrowIfOSError_(status, "SetControlProperty failed");
					break;
				}
				case 9: // HISize
				{
					HISize sizeValue = { 0, 0 };
					CFArray<CFStringRef> stringsArray(::CFStringCreateArrayBySeparatingStrings(NULL, propertyValueString, kCommaString), retain_No);
					if (stringsArray.IsValid() and (stringsArray.GetCount() >= 2))
					{
						sizeValue.width = ::CFStringGetDoubleValue(stringsArray.GetValueAt(0));
						sizeValue.height = ::CFStringGetDoubleValue(stringsArray.GetValueAt(1));
					}
					status = ::SetControlProperty(inView->GetSysView(), propertyCreator, propertyTag,
						sizeof(HISize), &sizeValue);
					PPx_ThrowIfOSError_(status, "SetControlProperty failed");
					break;
				}
				case 10: // HIRect
				{
					HIRect rectValue = { 0, 0, 0, 0 };
					CFArray<CFStringRef> stringsArray(::CFStringCreateArrayBySeparatingStrings(NULL, propertyValueString, kCommaString), retain_No);
					if (stringsArray.IsValid() and (stringsArray.GetCount() >= 2))
					{
						rectValue.origin.x = ::CFStringGetDoubleValue(stringsArray.GetValueAt(0));
						rectValue.origin.y = ::CFStringGetDoubleValue(stringsArray.GetValueAt(1));
						rectValue.size.width = ::CFStringGetDoubleValue(stringsArray.GetValueAt(2));
						rectValue.size.height = ::CFStringGetDoubleValue(stringsArray.GetValueAt(3));
					}
					status = ::SetControlProperty(inView->GetSysView(), propertyCreator, propertyTag,
						sizeof(HIRect), &rectValue);
					PPx_ThrowIfOSError_(status, "SetControlProperty failed");
					break;
				}
				default:
				{
					PPx_SignalString_("unknown property type");
					break;
				}
			}
		}
	}
}


// ---------------------------------------------------------------------------
//	ReadOptionalHelpInfo
/**
	+++
	
	@param	inView		+++												*/

void
NibReader::ReadOptionalHelpInfo( View* inView ) const
{
	if (inView == NULL)
		PPx_Throw_(LogicError, err_BadParam, "inView is NULL");

	OSStatus status;
	Rect absHotRect = { 0, 0, 0, 0 };
	HMHelpContentRec helpContent = { kMacHelpVersion, absHotRect, kHMDefaultSide };

	CFString helpTagText("");
	if (ReadOptionalString(NibConstants::tag_helpTagText, helpTagText))
	{
		helpContent.content[kHMMinimumContentIndex].contentType = kHMCFStringContent;
		helpContent.content[kHMMinimumContentIndex].u.tagCFString = helpTagText;
	}
	else
		helpContent.content[kHMMinimumContentIndex].contentType = kHMNoContent;
	
	CFString helpTagExtendedText("");
	if (ReadOptionalString(NibConstants::tag_helpTagExtendedText, helpTagExtendedText))
	{
		helpContent.content[kHMMaximumContentIndex].contentType = kHMCFStringContent;
		helpContent.content[kHMMaximumContentIndex].u.tagCFString = helpTagExtendedText;
	}
	else
		helpContent.content[kHMMaximumContentIndex].contentType = kHMNoContent;
	
	ReadOptionalInt(NibConstants::tag_helpTagDisplaySide, helpContent.tagSide);

	status = ::HMSetControlHelpContent(inView->GetSysView(), &helpContent);
	PPx_ThrowIfOSError_(status, "HMSetControlHelpContent failed");
}


// ---------------------------------------------------------------------------
//	ReadOptionalLayoutInfo
/**
	+++
	
	@param	inView		+++												*/

void
NibReader::ReadOptionalLayoutInfo( View* inView ) const
{
	if (inView == NULL)
		PPx_Throw_(LogicError, err_BadParam, "inView is NULL");

#if (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_3)

	OSStatus status;
	CFXMLTree layoutInfoTree;
	if (ReadOptionalElement(NibConstants::elem_object, NibConstants::tag_layoutInfo, layoutInfoTree))
	{
		const HISideBinding kHISideNoBinding = { NULL, kHILayoutBindNone, 0.0 };
		const HIBinding kHINoBinding = { kHISideNoBinding, kHISideNoBinding, kHISideNoBinding, kHISideNoBinding };
		HILayoutInfo layoutInfo = { kHILayoutInfoVersionZero, kHINoBinding,
			{ { NULL, 0.0 }, { NULL, 0.0 } },
			{ { NULL, kHILayoutPositionNone }, { NULL, kHILayoutPositionNone } } };
		
		NibReader layoutReader(layoutInfoTree);

		layoutReader.ReadOptionalInt(NibConstants::tag_bindingTopKind, layoutInfo.binding.top.kind);
		layoutReader.ReadOptionalInt(NibConstants::tag_bindingBottomKind, layoutInfo.binding.bottom.kind);
		layoutReader.ReadOptionalInt(NibConstants::tag_bindingLeftKind, layoutInfo.binding.left.kind);
		layoutReader.ReadOptionalInt(NibConstants::tag_bindingRightKind, layoutInfo.binding.right.kind);
		
		status = ::HIViewSetLayoutInfo(inView->GetSysView(), &layoutInfo);
		PPx_ThrowIfOSError_(status, "HIViewSetLayoutInfo failed");
	}
	
	bool layoutSuspended = false;
	if (ReadOptionalBool(NibConstants::tag_layoutSuspended, layoutSuspended) and layoutSuspended)
	{
		status = ::HIViewSuspendLayout(inView->GetSysView());
		PPx_ThrowIfOSError_(status, "HIViewSuspendLayout failed");
	}

#endif // 10.3 or later
}


// ---------------------------------------------------------------------------
//	ReadOptionalSubViews
/**
	+++
	
	@param	inView		+++												*/

void
NibReader::ReadOptionalSubViews( View* inView ) const
{
	if (inView == NULL)
		PPx_Throw_(LogicError, err_BadParam, "inView is NULL");

	CFXMLTree subViewsTree;
	if (not ReadOptionalElement(NibConstants::elem_array, NibConstants::tag_subviews, subViewsTree))
		return;

	CFIndex childCount = subViewsTree.GetChildCount();
	for (CFIndex index = 0; index < childCount; index++)
	{
		CFXMLTree childTree(subViewsTree.GetChildAtIndex(index));
		CFXMLNode childNode(childTree.GetNode());
		if ((childNode.GetTypeCode() == kCFXMLNodeTypeElement) and
			(childNode.GetString() == NibConstants::elem_reference))
		{
			CFXMLElement elementInfo(childNode);
			CFString idRefValue(elementInfo.GetAttributeValue(NibConstants::attr_idRef));
			if (idRefValue.IsValid())
			{
				CFXMLTree rootTree(mXMLTree.FindRoot());
				if (rootTree.GetChildCount() != 2)
					PPx_Throw_(DataError, err_XMLFormat, "XML document must have exactly two main elements");

				CFXMLTree ibObjectData(rootTree.GetChildAtIndex(1));
				if (ibObjectData.GetNode().GetString() != NibConstants::elem_object)
					PPx_Throw_(DataError, err_XMLFormat, "The second element is not an 'object'");

				NibReader objectDataReader(ibObjectData);
				
				CFXMLTree allObjects;
				objectDataReader.ReadRequiredElement(NibConstants::elem_array, NibConstants::value_allObjects, allObjects);
				
				CFIndex objectsChildCount = allObjects.GetChildCount();
				for (CFIndex objectsIndex = 0; objectsIndex < objectsChildCount; objectsIndex++)
				{
					CFXMLTree objectsChildTree(allObjects.GetChildAtIndex(objectsIndex));
					CFXMLNode objectsChildNode(objectsChildTree.GetNode());
					if ((objectsChildNode.GetTypeCode() == kCFXMLNodeTypeElement) and
						(objectsChildNode.GetString() == NibConstants::elem_object))
					{
						CFXMLElement objectsElementInfo(objectsChildNode);
						CFString idValue(objectsElementInfo.GetAttributeValue(NibConstants::attr_id));
						if (idValue.IsValid() and (idValue == idRefValue))
						{
							if ((objectsChildNode.GetTypeCode() == kCFXMLNodeTypeElement) and
								(objectsChildNode.GetString() == NibConstants::elem_object))
							{
								CFXMLElement childElem(objectsChildNode);
								CFString classStr(childElem.GetAttributeValue(NibConstants::attr_class));
								if (classStr.IsValid())
								{
									Persistent* object = NibRegistrar::CreateObject(classStr, inView, objectsChildTree);
								}
								else
									PPx_SignalString_("Could not get the 'class' attribute of the subview");
							}
							break;
						}
					}
				}
			}
		}
		else if ((childNode.GetTypeCode() == kCFXMLNodeTypeElement) and
			(childNode.GetString() == NibConstants::elem_object))
		{
			CFXMLElement childElem(childNode);
			CFString classStr(childElem.GetAttributeValue(NibConstants::attr_class));
			if (classStr.IsValid())
			{
				Persistent* object = NibRegistrar::CreateObject(classStr, inView, childTree);
			}
			else
				PPx_SignalString_("Could not get the 'class' attribute of the subview");
		}
		else
		{
			PPx_SignalString_("unknown subview data");
		}
	}
}

} // namespace PPx
