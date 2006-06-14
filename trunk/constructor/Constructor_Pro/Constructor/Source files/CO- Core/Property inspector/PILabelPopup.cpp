// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PILabelPopup.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 08/12/96
//     $Date: 2006/01/18 01:33:23 $
//	$History: PILabelPopup.cpp $
//	
//	*****************  Version 11  *****************
//	User: scouten      Date: 2/28/97    Time: 2:13p
//	Updated in $/ConstructorWin/Core/Source
//	Fixed a build error on VC++.
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 02/28/97   Time: 10:27
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Fixed an error in SetValue that caused it to misread resource IDs that
//	didn't contain a resource name.
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 02/21/97   Time: 11:15
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Improved commenting.
//	
//	*****************  Version 8  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:05p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 02/01/97   Time: 14:57
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Isolated access to RF* classes behind Constructor_UseRF macro.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:32
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Fixed CR/LF problem
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 12/22/96   Time: 13:02
//	Updated in $/Constructor/Source files/Property inspector
//	Rolled in a bug fix from Clint.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 12/21/96   Time: 16:12
//	Updated in $/Constructor/Source files/Property inspector
//	Switched from LGAPopup to LGAMiniArrowPopup in PI window. (Bug fix
//	#1132.)
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 12/13/96   Time: 14:12
//	Updated in $/Constructor/Source files/Property inspector
//	Added support for resource list popups.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 10/31/96   Time: 17:40
//	Updated in $/Constructor/Source files/Property inspector
//	Fixed current item highlighting.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 02:09
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "PILabelPopup.h"

	// Core : Data model : Attributes
#include "DMContainerAttribute.h"
#include "DMIterator.h"
#include "DMLabelledIntegerAttribute.h"
#include "DMResListAttribute.h"
#include "DMValueLabel.h"

	// Core : Documents : Generic resource file
#include "RSResource.h"

	// MacOS : Resource manager
#if Constructor_UseRF
	#include "RFMap.h"
	#include "RFResource.h"
	#include "RFType.h"
#endif

#include "UCustomTextTraits.h"

	// PowerPlant : CA : Array classes
#include <LFastArrayIterator.h>


// ===========================================================================
//		* SResInfo
// ===========================================================================
//	Used internally

struct SResInfo {
	ResIDT			mResID;
	Str255			mResName;
};


// ===========================================================================

#pragma mark *** PILabelPopup ***

// ---------------------------------------------------------------------------
//		* PILabelPopup()
// ---------------------------------------------------------------------------
//	Constructor

PILabelPopup::PILabelPopup(
	LStream*	inStream)

: LGAMiniArrowPopup(inStream)

{
	mAttribute = nil;
	mAttributeValue = 0;
}


// ---------------------------------------------------------------------------
//		* ~PILabelPopup
// ---------------------------------------------------------------------------
//	Destructor

PILabelPopup::~PILabelPopup()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** value accessors

// ---------------------------------------------------------------------------
//		* SetAttributeValue
// ---------------------------------------------------------------------------
//	Takes an value from the attribute and maps it to the popup menu item
// 	which displays that value's label (if any).

void
PILabelPopup::SetAttributeValue(
	SInt32	inAttributeValue)
{

	// Validate pointers.

	ValidateThis_();

	// Sanity check: Make sure we have an attribute and value labels.
	
	ValidateObject_(mAttribute);

	DMContainerAttribute* labels = mAttribute->GetValueLabels();
	ValidateObject_(labels);
	
	mAttributeValue = inAttributeValue;

	// Scan the value label list until we find this value.
	
	DMFastIterator iter(labels->GetSubElements());
	while (iter.NextElement()) {
	
		DMValueLabel* label = (dynamic_cast<DMValueLabel*>(iter.CurrentElement()));
		ValidateObject_(label);

		if (mAttribute->GetIntValue() == label->GetValue()) {
		
			// Suppress value-change message, since we're usually getting
			// called when the attribute's value has already changed.
		
			StopBroadcasting();
			LGAMiniArrowPopup::SetValue(iter.GetCurrentIndex());
			StartBroadcasting();
			return;
		}
	}
	
	// If we fall through to here, there's no label for this value.
	// Make sure the popup doesn't highlight any item.
	
	SetValue(0);

}


// ---------------------------------------------------------------------------
//		* SetValue
// ---------------------------------------------------------------------------
//	Overriden to map popup menu item number to actual attribute value.

void
PILabelPopup::SetValue(
	SInt32	inValue)
{

	// Validate pointers.

	ValidateThis_();

	// Sanity check: Make sure we have an attribute and value labels.
	
	ValidateObject_(mAttribute);

	DMContainerAttribute* labels = mAttribute->GetValueLabels();
	ValidateObject_(labels);
	
	// Now grab the value label whose index == the item number.
	
	DMElement* element;
	if (labels->GetSubElements().FetchItemAt(inValue, &element)) {

		ValidateObject_(element);

		// Got it. Set our value (and thus the attribute's) to
		// the value of the popup.
	
		DMValueLabel* label = (dynamic_cast<DMValueLabel*>(element));
		ValidateObject_(label);
		
		mAttributeValue = label->GetValue();
		
	}
	else {
		
		// It's beyond the end of the list. Try parsing as a resource ID.
		
		LStr255 idStr;
		::GetMenuItemText(GetMacMenuH(), inValue, idStr);
		
		SInt32 index = idStr.Find(':');
		if (index == 0)
			index = idStr.Length() + 1;

		idStr[(UInt8)0] = index - 1;
		
		// JCD 980624 - THis is an additional fix for MW00374 (see first
		// the fix in BuildResListPopup()).
		//
		// Due to that fix, we need to "remove" the opt-hyphen character
		// and replace it with a real negative sign to get the proper
		// value. Else "-1234" becomes "1234" in this conversion.

		#if Constructor_ForMacOS	// only necessary on Mac
			if ( (idStr.Length() > 0) && (idStr[1] == 0xD0) ) {
				idStr[1] = 0x2D;
			}
		#endif
				
		mAttributeValue = (SInt32) idStr;
		
	}

	// Pass the value through to control.

	LGAMiniArrowPopup::SetValue(inValue);

}


// ===========================================================================

#pragma mark -
#pragma mark ** configuration

// ---------------------------------------------------------------------------
//		* SetAttribute
// ---------------------------------------------------------------------------
//	Read the initial value from the attribute.

void
PILabelPopup::SetAttribute(
	DMLabelledIntegerAttribute*	inAttribute)
{
	
	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inAttribute);

	// Stash the attribute pointer.

	mAttribute = inAttribute;
	
	// Update popup menu.
	
	SetMacMenuH(LoadPopupMenuH());

	// Make sure the popup value matches the attribute.
	// Prevent value-change message from being sent because we
	// aren't really changing the attribute's value.

	StopBroadcasting();
	SetValue(0);
	SetAttributeValue(inAttribute->GetIntValue());
	StartBroadcasting();
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** dynamic popup menu

// ---------------------------------------------------------------------------
//		* LoadPopupMenuH										[protected]
// ---------------------------------------------------------------------------
//	Get the popup menu handle. Add items to the menu to reflect the
//	value labels.

MenuHandle
PILabelPopup::LoadPopupMenuH() const
{

	// Validate pointers.
	
	ValidateThis_();

	// Get the (hopefully) empty menu.

	MenuHandle theMenu = ::GetMenu(mPopupMenuResID);
#if PP_Target_Carbon
	ThrowIf_(theMenu == nil);
#else
	ValidateHandle_((Handle) theMenu);
#endif

	// If no attribute, bail out now.
	
	if (mAttribute == nil)
		return theMenu;
	ValidateObject_((const_cast<PILabelPopup*>(this))->mAttribute);
	
	// If menu isn't empty, yank all the items out of it.

	SInt16 menuItemCount = ::CountMenuItems(theMenu);
	while (menuItemCount--) {
		::DeleteMenuItem(theMenu, 1);
	}

	// Now add menu items for each attribute.
	
	DMContainerAttribute* labels = mAttribute->GetValueLabels();
	ValidateObject_(labels);

	DMFastIterator iter(labels->GetSubElements());
	while (iter.NextElement()) {
	
		DMValueLabel* label = (dynamic_cast<DMValueLabel*>(iter.CurrentElement()));
		ValidateObject_(label);

		// Get the label string. Special case for "-": make a divider item.
		
		LStr255 labelStr;
		label->GetLabel(labelStr);
		
		if (labelStr.CompareTo((StringPtr)"\001-") == 0)			//! TEMPORARY: magic strings
			::AppendMenu(theMenu, (StringPtr)"\001-");
		else {
		
			// It's not a divider, add menu text.
		
			::AppendMenu(theMenu, (StringPtr)"\001x");			//! TEMPORARY: magic string
			::SetMenuItemText(theMenu, ::CountMenuItems(theMenu), labelStr);
			
			// See if this is the current value of the attribute.
			// If so, make sure it gets marked.
			
			if (mAttribute->GetIntValue() == label->GetValue()) {
				(const_cast<PILabelPopup*>(this))->mMaxValue = 16384;
				(const_cast<PILabelPopup*>(this))->LGAMiniArrowPopup::SetValue(::CountMenuItems(theMenu));
			}

		}
	}

	// If this is a resource list attribute, grab the list of resources.
	
#if Constructor_UseRF
	DMResListAttribute* resListAttr = (dynamic_cast<DMResListAttribute*>((const_cast<PILabelPopup*>(this))->mAttribute));
	if (resListAttr != nil)
		BuildResListPopup(theMenu, resListAttr);
#endif

	// Return the newly-formed menu.
	
	return theMenu;

}


// ---------------------------------------------------------------------------
//		* SetPopupMinMaxValues									[protected]
// ---------------------------------------------------------------------------
//	Overriden to allow zero as a value (i.e. unmarked).

void
PILabelPopup::SetPopupMinMaxValues()
{

	// Validate pointers.

	ValidateThis_();
	
	// Allow unmarked popup.

	LGAMiniArrowPopup::SetPopupMinMaxValues();
	mMinValue = 0;

}


// ---------------------------------------------------------------------------
//		* BuildResListPopup										[protected]
// ---------------------------------------------------------------------------
//	Called by LoadPopupMenuH when the attribute is a resource list attribute.
//	Adds the IDs and names of all resources of the specified type to
//	the popup.

void
PILabelPopup::BuildResListPopup(
	MenuHandle			inMenu,
	DMResListAttribute*	inResListAttr) const
{
	
#if Constructor_UseRF

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inResListAttr);
#if PP_Target_Carbon
	ThrowIf_(inMenu == nil);
#else
	ValidateHandle_((Handle) inMenu);
#endif
	
	// Skanky hack #2: Look up for an RSContainer.
	// We shouldn't have this knowledge, but it's the
	// only way to get an RFMap.
	
	RSResource* resource = nil;
	DMElement* ancestor = inResListAttr;
	
	while (ancestor != nil) {
		ValidateObject_(ancestor);
		resource = (dynamic_cast<RSResource*>(ancestor));
		if (resource != nil)
			break;
		ancestor = ancestor->GetSuperElement();
	}
	
	if (resource == nil)
		return;
	
	// Look for resource map.
	
	RFMap* map = resource->GetRFMap();
	ValidateObject_(map);

	// Get resource type. Special case for text traits: add system font.
	
	ResType resType = inResListAttr->GetResType();
	if (resType == 'Txtr')
		::AppendMenu(inMenu, (StringPtr)"\0160: System font");		//! TEMPORARY: read from STR#

	// Build array of resources. Special case for icon suites.
	
	LArray resources(sizeof (SResInfo));

	if (resType == 'ICN#') {
		AddResources(map, resources, 'ICN#');
		AddResources(map, resources, 'icl8');
		AddResources(map, resources, 'icl4');
		AddResources(map, resources, 'ics#');
		AddResources(map, resources, 'ics8');
		AddResources(map, resources, 'ics4');
		AddResources(map, resources, 'icm#');
		AddResources(map, resources, 'icm8');
		AddResources(map, resources, 'icm4');
	}
	else
		AddResources(map, resources, resType);

	// icw -- hack for building the IDE pref panels
	if (resType == 'Txtr')
	{
		SCustomTextTraitsResInfo	txtrResInfo;

		LArray	txtrResInfoArray(sizeof(SCustomTextTraitsResInfo));
		UCustomTextTraits::FindCustomTextTraits(txtrResInfoArray);
		if (txtrResInfoArray.GetCount() > 0)
		{
			SResInfo		resInfo;
			LArrayIterator	iterTxtrResInfo(txtrResInfoArray);

			while (iterTxtrResInfo.Next(&txtrResInfo))
			{
				LArrayIterator	iterMainList(resources);
				bool			foundDup = false;

				while (iterMainList.Next(&resInfo))
				{
					if (resInfo.mResID == txtrResInfo.mResID)
					{
						foundDup = true;
						break;
					}
				}
				
				if (foundDup)
				{
					continue;
				}
				
				// Not a duplicate, add it to the list
				resInfo.mResID = txtrResInfo.mResID;
				LString::CopyPStr(txtrResInfo.mResName, resInfo.mResName);
				resources.InsertItemsAt(1, LArray::index_Last, &resInfo);
			}
		}
	}
	
	// If no resources, bail out now.
	
	if (resources.GetCount() == 0)
		return;

	// If there were any labels, insert a divider line here.
	
	if (::CountMenuItems(inMenu) != 0)
		::AppendMenu(inMenu, (StringPtr)"\001-");
	
	// Sort the resources array.
	
	resources.SetComparator(new PIResListComparator);
	resources.Sort();
	
	// Add the resources.
	
	LArrayIterator iter(resources);
	SResInfo resInfo;
	
	while (iter.Next(&resInfo)) {
		
		LStr255 menuText((SInt32) resInfo.mResID);
		
		// JCD 980611 - before we proceed, we need to check if the
		// menuText begins with a hyphen (ASCII 0x2D). If it does,
		// the Menu Manager will display a divider line instead of a
		// negative number, which is not what we want. So we will
		// convert the hyphen to an extended character (ASCII 0xD0)
		// which looks like a hyphen but the Menu Manager will ignore it.
		// This fixes bug MW00374.
	
	#if Constructor_ForMacOS	// only necessary on Mac
		if ( (menuText.Length() > 0) && (menuText[1] == 0x2D) ) {
			menuText[1] = 0xD0;
		}
	#endif
		
		if (resInfo.mResName[0] != 0) {
			menuText += (StringPtr)"\002: ";				//! TEMPORARY: read from STR#
			menuText += resInfo.mResName;
		}
		
		::AppendMenu(inMenu, (StringPtr)"\001x");			//! TEMPORARY: magic string
		::SetMenuItemText(inMenu, ::CountMenuItems(inMenu), menuText);
		
		if (mAttribute->GetIntValue() == resInfo.mResID) {
			(const_cast<PILabelPopup*>(this))->mMaxValue = 16384;
			(const_cast<PILabelPopup*>(this))->LGAMiniArrowPopup::SetValue(::CountMenuItems(inMenu));
		}

	}

#else
	#pragma unused (inMenu)
	#pragma unused (inResListAttr)
#endif

}


// ---------------------------------------------------------------------------
//		* AddResources											[protected]
// ---------------------------------------------------------------------------
//	Called by BuildResListPopup. Finds all resources of a single ResType
//	and adds them to a temporary array of SResInfo that's used to build the
//	popup.

void
PILabelPopup::AddResources(
	RFMap*		inMap,
	LArray&		ioResources,
	ResType		inResType) const
{

#if Constructor_UseRF

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inMap);
	
	// See if there is a res type entry.
	
	RFType* type = inMap->FindResType(inResType, false);
	if (type == nil)
		return;
	
	ValidateSimpleObject_(type);
	
	// Iterate through the resource list.
	
	LArrayIterator iter(type->GetResources());
	RFResource* resource;
	
	while (iter.Next(&resource)) {

		// Skip deleted resources.
		
		ValidateSimpleObject_(resource);
		if (resource->IsDeleted())
			continue;

		// Get resource info.
		
		ResIDT resID = resource->GetResID();
		Str255 resName;
		resource->GetResName(resName);
		
		// See if an entry already exists in the menu for this resource.
	
		LArrayIterator dupCheck(ioResources);
		SResInfo resInfo;
		Boolean foundDup = false;
		
		while (dupCheck.Next(&resInfo)) {
			if (resInfo.mResID == resID) {
				foundDup = true;
				break;
			}
		}
		
		if (foundDup)
			continue;
		
		// No duplicate, add it to this list.
		
		resInfo.mResID = resID;
		LString::CopyPStr(resName, resInfo.mResName);
		
		ioResources.InsertItemsAt(1, LArray::index_Last, &resInfo);
	
	}

#else
	#pragma unused (inMap)
	#pragma unused (ioResources)
	#pragma unused (inResType)
#endif

}


// ===========================================================================

#pragma mark -
#pragma mark *** PIResListComparator ***

// ---------------------------------------------------------------------------
//		* PIResListComparator
// ---------------------------------------------------------------------------
//	Given an array of SResInfo, compare two entries by their resource ID.

SInt32
PIResListComparator::Compare(
	const void*		inItemOne,
	const void* 	inItemTwo,
	UInt32			/* inSizeOne */,
	UInt32			/* inSizeTwo */) const
{
	const SResInfo* info1 = (const SResInfo*) inItemOne;
	const SResInfo* info2 = (const SResInfo*) inItemTwo;
	
	return info1->mResID - info2->mResID;
}
