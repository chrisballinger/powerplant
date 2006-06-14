// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxSegmentedView.cp
//
//	Copyright © 2004-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:45 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxSegmentedView.h>
#include <PPxSerializer.h>
#include <PPxViewUtils.h>
#include <SysCreateView.h>

// The system segmented view is only available on Mac OS 10.3 or later
#if (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_3)

namespace PPx {

namespace {

	// -----------------------------------------------------------------------
	//	Key names for persistent data

	//const CFStringRef	key_TabSize			= CFSTR("tab size");
}


// ---------------------------------------------------------------------------
//	SegmentedView													  [public]
/**
	Default constructor */

SegmentedView::SegmentedView()
{
}


// ---------------------------------------------------------------------------
//	~SegmentedView													  [public]
/**
	Destructor */

SegmentedView::~SegmentedView()
{
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initialize from segmented view creation parameters
	
	@param	inSuperView			Parent view
	@param	inFrame				Bounds for view, in local coords of parent
	@param	inVisible			Whether the view is visible
	@param	inEnabled			Whether the view is enabled					*/

void
SegmentedView::Initialize(
	View*			inSuperView,
	const HIRect&	inFrame,
	bool			inVisible,
	bool			inEnabled)
{
	HIViewRef	viewRef = SysCreateView::SegmentedView();

	View::Initialize(viewRef, inSuperView, inFrame, inVisible, inEnabled);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
SegmentedView::ClassName() const
{
	return CFSTR("PPx::SegmentedView");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
SegmentedView::InitState(
	const DataReader&	inReader)
{
	// ### Not Finished
	HIViewRef	viewRef = SysCreateView::SegmentedView();
	
	View::InitViewState(viewRef, inReader);	// Data for all Views
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
SegmentedView::WriteState(
	DataWriter&	ioWriter) const
{
	// ### Not Finished
	View::WriteState(ioWriter);				// Data for all Views
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SetSegmentCount													  [public]
/**
	Sets the number of segments for the view
	
	@param	inSegmentCount	Number of segments for the view					*/

void
SegmentedView::SetSegmentCount( UInt32 inSegmentCount )
{
	OSStatus	status = ::HISegmentedViewSetSegmentCount(
								GetSysView(), inSegmentCount);

	PPx_ThrowIfOSError_(status, "HISegmentedViewSetSegmentCount failed");
}


// ---------------------------------------------------------------------------
//	GetSegmentCount													  [public]
/**
	Gets the number of segments of the view
	
	@return Number of segments of the view									*/

UInt32
SegmentedView::GetSegmentCount() const
{
	return ::HISegmentedViewGetSegmentCount(GetSysView());
}


// ---------------------------------------------------------------------------
//	SetSegmentBehavior												  [public]
/**
	Sets the behavior of a particular segment of the view
	
	@param	inIndex		+++
	@param	inBehavior	+++					*/

void
SegmentedView::SetSegmentBehavior(
	UInt32				inIndex,
	HISegmentBehavior	inBehavior)
{
	OSStatus	status = ::HISegmentedViewSetSegmentBehavior(
								GetSysView(), inIndex, inBehavior);

	PPx_ThrowIfOSError_(status, "HISegmentedViewSetSegmentBehavior failed");
}


// ---------------------------------------------------------------------------
//	GetSegmentBehavior												  [public]
/**
	Gets +++
	
	@return +++									*/

HISegmentBehavior
SegmentedView::GetSegmentBehavior( UInt32 inIndex ) const
{
	return ::HISegmentedViewGetSegmentBehavior(GetSysView(), inIndex);
}


// ---------------------------------------------------------------------------
//	ChangeSegmentAttributes											  [public]
/**
	Changes +++
	
	@param	inSegmentCount	+++					*/

void
SegmentedView::ChangeSegmentAttributes(
	UInt32		inIndex,
	OptionBits	inAttributesToSet,
	OptionBits	inAttributesToClear)
{
	OSStatus	status = ::HISegmentedViewChangeSegmentAttributes(
								GetSysView(), inIndex, inAttributesToSet,
								inAttributesToClear);

	PPx_ThrowIfOSError_(status, "HISegmentedViewChangeSegmentAttributes failed");
}


// ---------------------------------------------------------------------------
//	GetSegmentAttributes											  [public]
/**
	Gets +++
	
	@return +++									*/

OptionBits
SegmentedView::GetSegmentAttributes( UInt32 inIndex ) const
{
	return ::HISegmentedViewGetSegmentAttributes(GetSysView(), inIndex);
}


// ---------------------------------------------------------------------------
//	SetSegmentValue													  [public]
/**
	Sets +++
	
	@param	inSegmentCount	+++					*/

void
SegmentedView::SetSegmentValue(
	UInt32	inIndex,
	SInt32	inValue)
{
	OSStatus	status = ::HISegmentedViewSetSegmentValue(
								GetSysView(), inIndex, inValue);

	PPx_ThrowIfOSError_(status, "HISegmentedViewSetSegmentValue failed");
}


// ---------------------------------------------------------------------------
//	GetSegmentValue													  [public]
/**
	Gets +++
	
	@return +++									*/

SInt32
SegmentedView::GetSegmentValue( UInt32 inIndex ) const
{
	return ::HISegmentedViewGetSegmentValue(GetSysView(), inIndex);
}


// ---------------------------------------------------------------------------
//	SetSegmentEnabled												  [public]
/**
	Sets +++
	
	@param	inIndex		+++
	@param	inBehavior	+++					*/

void
SegmentedView::SetSegmentEnabled(
	UInt32	inIndex,
	bool	inEnabled)
{
	OSStatus	status = ::HISegmentedViewSetSegmentEnabled(
								GetSysView(), inIndex, inEnabled);

	PPx_ThrowIfOSError_(status, "HISegmentedViewSetSegmentEnabled failed");
}


// ---------------------------------------------------------------------------
//	IsSegmentEnabled												  [public]
/**
	Gets +++
	
	@return +++									*/

bool
SegmentedView::IsSegmentEnabled( UInt32 inIndex ) const
{
	return ::HISegmentedViewIsSegmentEnabled(GetSysView(), inIndex);
}


// ---------------------------------------------------------------------------
//	SetSegmentCommand												  [public]
/**
	Sets +++
	
	@param	inIndex		+++
	@param	inBehavior	+++					*/

void
SegmentedView::SetSegmentCommand(
	UInt32	inIndex,
	UInt32	inCommand)
{
	OSStatus	status = ::HISegmentedViewSetSegmentCommand(
								GetSysView(), inIndex, inCommand);

	PPx_ThrowIfOSError_(status, "HISegmentedViewSetSegmentCommand failed");
}


// ---------------------------------------------------------------------------
//	GetSegmentCommand												  [public]
/**
	Gets +++
	
	@return +++									*/

UInt32
SegmentedView::GetSegmentCommand( UInt32 inIndex ) const
{
	return ::HISegmentedViewGetSegmentCommand(GetSysView(), inIndex);
}


// ---------------------------------------------------------------------------
//	SetSegmentLabel												  [public]
/**
	Sets +++
	
	@param	inIndex		+++
	@param	inBehavior	+++					*/

void
SegmentedView::SetSegmentLabel(
	UInt32		inIndex,
	CFStringRef	inLabel)
{
	OSStatus	status = ::HISegmentedViewSetSegmentLabel(
								GetSysView(), inIndex, inLabel);

	PPx_ThrowIfOSError_(status, "HISegmentedViewSetSegmentLabel failed");
}


// ---------------------------------------------------------------------------
//	GetSegmentLabel												  [public]
/**
	Gets +++
	
	@return +++									*/

CFString
SegmentedView::GetSegmentLabel( UInt32 inIndex ) const
{
	CFStringRef	label = nil;

	OSStatus	status = ::HISegmentedViewCopySegmentLabel(
								GetSysView(), inIndex, &label);
					
	PPx_ThrowIfOSError_(status, "HISegmentedViewCopySegmentLabel failed");
				
	return CFString(label, retain_No);
}


// ---------------------------------------------------------------------------
//	SetSegmentContentWidth											  [public]
/**
	Sets +++
	
	@param	inIndex		+++
	@param	inBehavior	+++					*/

void
SegmentedView::SetSegmentContentWidth(
	UInt32	inIndex,
	bool	inAutoCalculateWidth,
	float	inWidth)
{
	OSStatus	status = ::HISegmentedViewSetSegmentContentWidth(
								GetSysView(), inIndex, inAutoCalculateWidth,
								inWidth);

	PPx_ThrowIfOSError_(status, "HISegmentedViewSetSegmentContentWidth failed");
}


// ---------------------------------------------------------------------------
//	GetSegmentContentWidth											  [public]
/**
	Gets +++
	
	@return +++									*/

float
SegmentedView::GetSegmentContentWidth(
	UInt32	inIndex,
	bool*	outAutoCalculated) const
{
	Boolean	autoCalculated;
	float	width = ::HISegmentedViewGetSegmentContentWidth(
							GetSysView(), inIndex, &autoCalculated);

	if (outAutoCalculated)
		*outAutoCalculated = autoCalculated;
	
	return width;
}


// ---------------------------------------------------------------------------
//	SetSegmentImage													  [public]
/**
	Sets +++
	
	@param	inIndex		+++
	@param	inBehavior	+++					*/

void
SegmentedView::SetSegmentImage(
	UInt32							inIndex,
	const HIViewImageContentInfo&	inImage)
{
	OSStatus	status = ::HISegmentedViewSetSegmentImage(
								GetSysView(), inIndex, &inImage);

	PPx_ThrowIfOSError_(status, "HISegmentedViewSetSegmentImage failed");
}


// ---------------------------------------------------------------------------
//	GetSegmentImageContentType										  [public]
/**
	Gets +++
	
	@return +++									*/

HIViewImageContentType
SegmentedView::GetSegmentImageContentType( UInt32 inIndex ) const
{
	return ::HISegmentedViewGetSegmentImageContentType(GetSysView(), inIndex);
}


// ---------------------------------------------------------------------------
//	CopySegmentImage												  [public]
/**
	Gets +++
	
	@return +++									*/

void
SegmentedView::CopySegmentImage(
	UInt32					inIndex,
	HIViewImageContentInfo&	ioImage) const
{
	OSStatus	status = ::HISegmentedViewCopySegmentImage(
								GetSysView(), inIndex, &ioImage);
					
	PPx_ThrowIfOSError_(status, "HISegmentedViewCopySegmentImage failed");
}


} // namespace PPx

#endif // 10.3 or later
