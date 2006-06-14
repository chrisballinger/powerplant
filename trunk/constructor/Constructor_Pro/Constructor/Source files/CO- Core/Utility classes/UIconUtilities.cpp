// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	UIconUtilities.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 10/09/96
//	   $Date: 2006/04/12 08:48:30 $
//	$History: UIconUtilities.cpp $
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 03/05/97   Time: 09:56
//	Updated in $/Constructor/Source files/CO- Core/Utility classes
//	Silenced "unused parameter" warnings for CW Win32 build.
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 02/28/97   Time: 11:51
//	Updated in $/Constructor/Source files/CO- Core/Utility classes
//	Changed several instances of "#if Constructor_ForMacOS" to "#if
//	Constructor_MacOSHost".
//	
//	*****************  Version 7  *****************
//	User: Andrew       Date: 2/21/97    Time: 3:37p
//	Updated in $/ConstructorWin/Sources
//	added ifdefs for a few more PP-only icon calls
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 02/19/97   Time: 19:32
//	Updated in $/Constructor/Source files/CO- Core/Utility classes
//	Added a quick hack for Windows version. Improved commenting.
//	
//	*****************  Version 5  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:07p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:33
//	Updated in $/Constructor/Source files/CO- Core/Utility classes
//	Fixed CR/LF problem
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/20/96   Time: 11:49
//	Updated in $/Constructor/Source files/Utility classes
//	Added GetIconSuiteFromMap function.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/03/96   Time: 15:55
//	Updated in $/Constructor/Source files/Utility classes
//	Corrected commenting errors.
//
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 02:10
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "UIconUtilities.h"
	
	// MacOS : Resource manager
#ifndef WINVER
	#include "RFMap.h"
	#include "RFResource.h"
#endif


// ===========================================================================

#pragma mark *** UIconUtilities ***

// ---------------------------------------------------------------------------
//		* Get1DetachedIconSuite									[static]
// ---------------------------------------------------------------------------
//	Similar to the Toolbox call GetIconSuite, except that it loads from the
//	topmost resource fork only, and it detaches all resource handles before
//	returning the suite.

Handle
UIconUtilities::Get1DetachedIconSuite(
	ResIDT				inResID,
	IconSelectorValue	inSelector)
{

#ifdef WINVER

	// On Windows, we make the simplifying assumption that the
	// icon handle won't be disposed. This is because Altura doesn't
	// implement the NewIconSuite or AddIconToSuite functions.

	Handle outHandle = nil;
	ThrowIfOSErr_(::GetIconSuite(&outHandle, inResID, inSelector));
	return outHandle;

#else

	// Create the icon suite.
	
	Handle iconSuite = nil;
	ThrowIfOSErr_(::NewIconSuite(&iconSuite));
#if PP_Target_Carbon
	ThrowIfNil_(iconSuite);
#else
	ValidateHandle_(iconSuite);
#endif
	
	// Attempt to grab the requested icon suite members.
	// Use a preprocessor macro to avoid copy/paste errors.
	
	#define AddIfSelected_(resType, selector) \
		if (inSelector & selector) \
			foundAny |= UIconUtilities::Add1IconToSuite( \
								iconSuite, resType, inResID)

	Boolean foundAny = false;

	AddIfSelected_('ICN#', svLarge1Bit);
	AddIfSelected_('icl4', svLarge4Bit);
	AddIfSelected_('icl8', svLarge8Bit);

	AddIfSelected_('ics#', svSmall1Bit);
	AddIfSelected_('ics4', svSmall4Bit);
	AddIfSelected_('ics8', svSmall8Bit);
	
	AddIfSelected_('icm#', svMini1Bit);
	AddIfSelected_('icm4', svMini4Bit);
	AddIfSelected_('icm8', svMini8Bit);
	
	// If we didn't find any resources, kill the icon suite.
	// Otherwise, it's ready to go.
	
	if (foundAny)
		return iconSuite;
	else {
#if PP_Target_Carbon
		ThrowIfNil_(iconSuite);
#else
		ValidateHandle_(iconSuite);
#endif
		::DisposeIconSuite(iconSuite, true);
		return nil;
	}
#endif
}


// ---------------------------------------------------------------------------
//		* GetIconSuiteFromMap									[static]
// ---------------------------------------------------------------------------
//	Similar to the Toolbox call GetIconSuite, except that it loads from the
//	named resource map only, and it detaches all resource handles before
//	returning the suite.

Handle
UIconUtilities::GetIconSuiteFromMap(
	RFMap*				inRFMap,
	ResIDT				inResID,
	IconSelectorValue	inSelector)
{

#if Constructor_MacOSHost

	// Validate pointers.
	
	ValidateObject_(inRFMap);

	// Create the icon suite.
	
	Handle iconSuite = nil;
	ThrowIfOSErr_(::NewIconSuite(&iconSuite));
#if PP_Target_Carbon
	ThrowIfNil_(iconSuite);
#else
	ValidateHandle_(iconSuite);
#endif
	
	// Attempt to grab the requested icon suite members.
	// Use a preprocessor macro to avoid copy/paste errors.
	
	#define AddFromMapIfSelected_(resType, selector) \
		if (inSelector & selector) \
			foundAny |= UIconUtilities::AddIconToSuiteFromMap( \
								inRFMap, iconSuite, resType, inResID)

	Boolean foundAny = false;

	AddFromMapIfSelected_('ICN#', svLarge1Bit);
	AddFromMapIfSelected_('icl4', svLarge4Bit);
	AddFromMapIfSelected_('icl8', svLarge8Bit);

	AddFromMapIfSelected_('ics#', svSmall1Bit);
	AddFromMapIfSelected_('ics4', svSmall4Bit);
	AddFromMapIfSelected_('ics8', svSmall8Bit);
	
	AddFromMapIfSelected_('icm#', svMini1Bit);
	AddFromMapIfSelected_('icm4', svMini4Bit);
	AddFromMapIfSelected_('icm8', svMini8Bit);
	
	// If we didn't find any resources, kill the icon suite.
	// Otherwise, it's ready to go.
	
	if (foundAny)
		return iconSuite;
	else {
#if PP_Target_Carbon
		ThrowIfNil_(iconSuite);
#else
		ValidateHandle_(iconSuite);
#endif
		::DisposeIconSuite(iconSuite, true);
		return nil;
	}
#else

	// RFMap icon suites not supported on Windows.

	#pragma unused (inRFMap)
	#pragma unused (inResID)
	#pragma unused (inSelector)

	return nil;

#endif

}


// ---------------------------------------------------------------------------
//		* GetCIconFromMap										[static]
// ---------------------------------------------------------------------------
//	Similar to the Toolbox call GetCIcon, except that it loads from the
//	named resource map only, and it detaches all resource handles before
//	returning the icon.

CIconHandle
UIconUtilities::GetCIconFromMap(
	RFMap*				inRFMap,
	ResIDT				inResID)
{

#if Constructor_MacOSHost

	// Validate pointers.
	
	ValidateObject_(inRFMap);
	
	RFResource* iconRsrc = inRFMap->FindResource('cicn', inResID, false);
	
	if (iconRsrc == nil)
		return nil;
	
#ifndef __GNUC__
#warning JWW - Fix this so it actually gets it from the in-memory resource map
#endif
	return ::GetCIcon(inResID);
	
#else

	// RFMap color icons not supported on Windows.

	#pragma unused (inRFMap)
	#pragma unused (inResID)

	return nil;

#endif

}


// ---------------------------------------------------------------------------
//		* Get1DetachedResource									[static]
// ---------------------------------------------------------------------------
//	Used by Add1IconToSuite to fetch a single resource and detach it.

Handle
UIconUtilities::Get1DetachedResource(
	ResType		inResType,
	ResIDT		inResID)
{
	
	// Attempt to get the resource. If it fails, we assume that
	// there is no such resource.

	Handle returnResource = ::Get1Resource(inResType, inResID);
	if (returnResource == nil)
		return nil;
	
	// Make sure the resource data exists, then detach it.
	
	ValidateHandle_(returnResource);

	::HNoPurge(returnResource);
	ThrowIfMemError_();

	::DetachResource(returnResource);
	ThrowIfResError_();
	
	return returnResource;
	
}

// ---------------------------------------------------------------------------
//		* GetRemappedIconID										[static]
// ---------------------------------------------------------------------------
//	Return an alternate icon ID for use when running under OS X.
ResIDT
UIconUtilities::GetRemappedIconID(
	ResIDT	inResID)
{
	ResIDT outResID = inResID;
	
	if (UEnvironment::GetOSVersion() >= 0x1000)
	{
		Handle theAlternate = ::Get1Resource('ALTI', inResID);
		
		if (theAlternate != NULL)
		{
			ValidateHandle_(theAlternate);
			
			outResID = **((ResIDT **) theAlternate);
			
			::ReleaseResource(theAlternate);
		}
	}
	
	return outResID;
}

// ---------------------------------------------------------------------------
//		* Add1IconToSuite								[static, protected]
// ---------------------------------------------------------------------------
//	Used by Get1DetachedIconSuite to add an individual icon resource to
//	a suite if it is available. Returns true if this icon was found.

Boolean
UIconUtilities::Add1IconToSuite(
	Handle		inIconSuite,
	ResType		inResType,
	ResIDT		inResID)
{

#if Constructor_MacOSHost

	// Get the detached resource if it exists.
	
	Handle iconData = Get1DetachedResource(inResType, inResID);
	
	// If it exists, add it to the suite.
	
	if (iconData != nil) {
#if PP_Target_Carbon
		ThrowIfNil_(inIconSuite);
#else
		ValidateHandle_(inIconSuite);
#endif
		ValidateHandle_(iconData);
		::AddIconToSuite(iconData, inIconSuite, inResType);
	}

	// Return success flag if resource existed.
	
	return iconData != nil;

#else

	// On Windows, this call is not used and not supported.

	#pragma unused (inIconSuite)
	#pragma unused (inResType)
	#pragma unused (inResID)
	
	return false;

#endif

}

// ---------------------------------------------------------------------------
//		* AddIconToSuiteFromMap							[static, protected]
// ---------------------------------------------------------------------------
//	Used by GetIconSuiteFromMap to add an individual icon resource to
//	a suite if it is available. Returns true if this icon was found.

Boolean
UIconUtilities::AddIconToSuiteFromMap(
	RFMap*		inRFMap,
	Handle		inIconSuite,
	ResType		inResType,
	ResIDT		inResID)
{

#if Constructor_MacOSHost

	// Validate pointers.
	
	ValidateObject_(inRFMap);
#if PP_Target_Carbon
	ThrowIfNil_(inIconSuite);
#else
	ValidateHandle_(inIconSuite);
#endif
	
	// Look for the resource object.
	
	RFResource* rsrc = inRFMap->FindResource(inResType, inResID, false);
	if (rsrc == nil)
		return nil;
	ValidateSimpleObject_(rsrc);
	
	// Got the resource object, get its data.
	
	Handle iconData = rsrc->GetResData();
	ValidateHandle_(iconData);
	
	// Add it to the suite.
	
	::AddIconToSuite(iconData, inIconSuite, inResType);

	// Return success flag.
	
	return true;

#else

	// On Windows, this call is not used and not supported.

	#pragma unused (inRFMap)
	#pragma unused (inIconSuite)
	#pragma unused (inResType)
	#pragma unused (inResID)
	
	return false;

#endif

}