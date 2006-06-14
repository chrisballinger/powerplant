// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxTabView.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:45 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxTabView.h>
#include <PPxSerializer.h>
#include <PPxViewUtils.h>
#include <SysCreateView.h>

namespace PPx {

namespace {

	// -----------------------------------------------------------------------
	//	Key names for persistent data

	const CFStringRef	key_TabSize			= CFSTR("tab size");
	const CFStringRef	key_TabDirection	= CFSTR("tab direction");
	const CFStringRef	key_TabCount		= CFSTR("tab count");
	const CFStringRef	key_TabNames		= CFSTR("tab names");
	const CFStringRef	key_TabIconIDs		= CFSTR("tab icon ids");
	const CFStringRef	key_TabEnabled		= CFSTR("tab enabled");
}


// ---------------------------------------------------------------------------
//	TabView															  [public]
/**
	Default constructor */

TabView::TabView()
{
	mTabSize		= kControlTabSizeLarge;
	mTabDirection	= kControlTabDirectionNorth;
}


// ---------------------------------------------------------------------------
//	~TabView														  [public]
/**
	Destructor */

TabView::~TabView()
{
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initialize from chasing arrows creation parameters
	
	@param	inSuperView			Parent view
	@param	inFrame				Bounds for view, in local coords of parent
	@param	inVisible			Whether the view is visible
	@param	inEnabled			Whether the view is enabled
	@param	inTabSize			Size of tabs (normal or small)
	@param	inTabDirection		Dirction of tabs (north, south, east, west)
	@param	inTabCount			Number of tabs
	@param	inTabEntries		Data for each tab							*/

void
TabView::Initialize(
	View*					inSuperView,
	const HIRect&			inFrame,
	bool					inVisible,
	bool					inEnabled,
	ControlTabSize			inTabSize,
	ControlTabDirection		inTabDirection,
	UInt16					inTabCount,
	const ControlTabEntry*	inTabEntries)
{
	mTabSize		= inTabSize;
	mTabDirection	= inTabDirection;

	HIViewRef	viewRef = SysCreateView::TabView(
								inTabSize, inTabDirection, inTabCount,
								inTabEntries);

	View::Initialize(viewRef, inSuperView, inFrame, inVisible, inEnabled);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
TabView::ClassName() const
{
	return CFSTR("PPx::TabView");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
TabView::InitState(
	const DataReader&	inReader)
{
										// Data for Tab View
	inReader.ReadOptional(key_TabSize,		mTabSize);
	inReader.ReadOptional(key_TabDirection,	mTabDirection);
	
	SInt32	initialValue = 1;
	inReader.ReadOptional(key_InitValue,	initialValue);
	
	SInt32	tabCount = 0;
	inReader.ReadOptional(key_TabCount,		tabCount);
	
										// Each tab has a name, icon ID,
										//   and enabled flag
	std::vector<CFString>	names;
	inReader.ReadContainer<CFString>(key_TabNames, std::back_inserter(names));
	
	PPx_ThrowIf_( (names.size() != tabCount), DataError, err_XMLFormat,
				  "Number of tab names does not match tab count" );
	
	std::vector<SInt16>		iconIDs;
	inReader.ReadContainer<SInt16>(key_TabIconIDs, std::back_inserter(iconIDs));
	
	PPx_ThrowIf_( (iconIDs.size() != tabCount), DataError, err_XMLFormat,
				  "Number of tab icon IDs does not match tab count" );
	
	std::vector<bool>		enabledFlags;
	inReader.ReadContainer<bool>(key_TabEnabled, std::back_inserter(enabledFlags));
	
	PPx_ThrowIf_( (enabledFlags.size() != tabCount), DataError, err_XMLFormat,
				  "Number of tab enabled flags does not match tab count" );
	
	std::vector<ControlTabEntry>	tabEntries;
	
	for (long i = 0; i < tabCount; ++i) {
		
		ControlButtonContentInfo	contentInfo = { kControlNoContent };
		if (iconIDs[i] != 0) {
			contentInfo.contentType = kControlContentIconSuiteRes;
			contentInfo.u.resID = iconIDs[i];
		}
		
		ControlTabEntry	entry = { &contentInfo, names[i], enabledFlags[i] };
		tabEntries.push_back(entry);
	}
	
	HIViewRef	viewRef = SysCreateView::TabView(
								mTabSize, mTabDirection, tabCount,
								&tabEntries[0]);
	
	View::InitViewState(viewRef, inReader);	// Data for all Views
	
	SetValue(initialValue);
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
TabView::WriteState(
	DataWriter&	ioWriter) const
{
	SInt32	tabCount = GetMaxValue();		// Data for Tab View
									
	ioWriter.WriteValue(key_TabSize,		mTabSize);
	ioWriter.WriteValue(key_TabDirection,	mTabDirection);
	ioWriter.WriteValue(key_InitValue,		GetValue());
	ioWriter.WriteValue(key_TabCount,		tabCount);
	
	std::vector<CFString>	names;
	std::vector<SInt16>		iconIDs;
	std::vector<bool>		enabledFlags;

	names.reserve(tabCount);				// Each tab has name, icon ID,
	iconIDs.reserve(tabCount);				//   and enabled flag
	enabledFlags.reserve(tabCount);
	
	OSStatus	status;
	
	for (SInt32 i = 1; i <= tabCount; ++i) {
		
		ControlTabInfoRecV1	tabInfo = { kControlTabInfoVersionOne, 0, nil };
		status = GetDataTag(i, kControlTabInfoTag, sizeof(tabInfo), &tabInfo);

		PPx_ThrowIfOSError_(status, "Could not get tab information");
		
		names.push_back(CFString(tabInfo.name, retain_No));
		iconIDs.push_back(tabInfo.iconSuiteID);
		
		Boolean	isEnabled = false;
		status = GetDataTag(i, kControlTabEnabledFlagTag, sizeof(isEnabled), &isEnabled);
		
		PPx_ThrowIfOSError_(status, "Could not get tab enabled state");
		
		enabledFlags.push_back(isEnabled);
	}
	
	ioWriter.WriteContainer(key_TabNames, names.begin(), names.end());
	ioWriter.WriteContainer(key_TabIconIDs, iconIDs.begin(), iconIDs.end());
	ioWriter.WriteContainer(key_TabEnabled, enabledFlags.begin(), enabledFlags.end());
		
	View::WriteState(ioWriter);				// Data for all Views
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SetThemeFontID													  [public]
/**
	Sets the theme font ID
	
	@param	inFont		Theme font ID to use for text						*/

void
TabView::SetThemeFontID(
	ThemeFontID		inFont)
{
	ViewUtils::SetControlThemeFontID(this, inFont);
}


} // namespace PPx
