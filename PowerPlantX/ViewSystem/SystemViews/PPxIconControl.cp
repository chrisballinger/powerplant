// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxIconControl.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:41 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxIconControl.h>
#include <PPxSerializer.h>
#include <SysCreateView.h>

namespace PPx {

namespace {		// unnamed namespace 

	// -----------------------------------------------------------------------
	//	Key names for persistent data

	const CFStringRef	key_DontTrack	= CFSTR("do not track");
}


// ---------------------------------------------------------------------------
//	IconControl													  [public]
/**
	Default constructor */

IconControl::IconControl()
{
	mDontTrack = false;
}


// ---------------------------------------------------------------------------
//	~IconControl													  [public]
/**
	Destructor */

IconControl::~IconControl()
{
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initialize from icon control creation parameters
	
	@param	inSuperView		Parent view
	@param	inFrame			Bounds for view, in local coordinates of parent
	@param	inVisible		Whether the view is visible
	@param	inEnabled		Whether the view is enabled
	@param	inContent		Content of icon control
	@param	inDontTrack		Whether to not track mouse downs in the control	*/

void
IconControl::Initialize(
	View*							inSuperView,
	const HIRect&					inFrame,
	bool							inVisible,
	bool							inEnabled,
	const ControlButtonContentInfo&	inContent,
	bool							inDontTrack)
{
	mDontTrack = inDontTrack;
	
	HIViewRef	viewRef = SysCreateView::IconControl(
								inContent, inDontTrack);
	
	View::Initialize(viewRef, inSuperView, inFrame, inVisible, inEnabled);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
IconControl::ClassName() const
{
	return CFSTR("PPx::IconControl");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
IconControl::InitState(
	const DataReader&	inReader)
{
	ControlButtonContentInfo	content;	// Data for Icon Control
	
	content.contentType = kControlNoContent;
	content.u.iconRef   = nil;
	
	inReader.ReadOptional(key_ContentType, content.contentType);
	inReader.ReadOptional(key_ContentResID, content.u.resID);
	inReader.ReadOptional(key_DontTrack, mDontTrack);

	HIViewRef	viewRef = SysCreateView::IconControl(
								content, mDontTrack);
	
	View::InitViewState(viewRef, inReader);	// Data for all Views
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
IconControl::WriteState(
	DataWriter&	ioWriter) const
{
											// Data for Icon Control
	ControlButtonContentInfo	content;

	GetDataTag(kControlButtonPart, kControlIconContentTag,
				sizeof(content), &content, nil);

	ioWriter.WriteValue(key_ContentType, content.contentType);
											// Write ID for resource content
	if ( (content.contentType > kControlNoContent)  &&
		 (content.contentType <= kControlContentICONRes) ) {
		ioWriter.WriteValue(key_ContentResID, content.u.resID);
	}

	ioWriter.WriteValue(key_DontTrack, mDontTrack);
	
	View::WriteState(ioWriter);				// Data for all Views
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SetIconTransform												  [public]
/**
	Sets the icon transform
	
	@param	inTransform		Icon transform									*/

void
IconControl::SetIconTransform(
	IconTransformType	inTransform)
{
	SetDataTag(kControlIconPart, kControlIconTransformTag,
				sizeof(IconTransformType), &inTransform);
}


// ---------------------------------------------------------------------------
//	GetIconTransform												  [public]
/**
	Returns the icon transform
	
	@return Icon transform													*/

IconTransformType
IconControl::GetIconTransform() const
{
	IconTransformType	transform;

	GetDataTag(kControlIconPart, kControlIconTransformTag,
				sizeof(IconTransformType), &transform);
				
	return transform;
}


// ---------------------------------------------------------------------------
//	SetIconAlignment												  [public]
/**
	Sets the icon alignment
	
	@param	inAlignment		Icon alignemnt									*/

void
IconControl::SetIconAlignment(
	IconAlignmentType	inAlignment)
{
	SetDataTag(kControlIconPart, kControlIconAlignmentTag,
				sizeof(IconAlignmentType), &inAlignment);
}


// ---------------------------------------------------------------------------
//	GetIconAlignment												  [public]
/**
	Returns the icon alignment
	
	@return Icon alignment													*/

IconAlignmentType
IconControl::GetIconAlignment() const
{
	IconAlignmentType	alignment;

	GetDataTag(kControlIconPart, kControlIconAlignmentTag,
				sizeof(IconAlignmentType), &alignment);
				
	return alignment;
}


// ---------------------------------------------------------------------------
//	SetIconResourceID												  [public]
/**
	Sets the resource ID for the icon
	
	@param	inResID		Resource ID for the icon							*/

void
IconControl::SetIconResourceID(
	SInt16	inResID)
{
	SetDataTag(kControlIconPart, kControlIconResourceIDTag,
				sizeof(SInt16), &inResID);
}


// ---------------------------------------------------------------------------
//	GetIconResourceID												  [public]
/**
	Returns the resource ID for the icon
	
	@return Resource ID for the icon										*/

SInt16
IconControl::GetIconResourceID() const
{
	SInt16	resID;

	GetDataTag(kControlIconPart, kControlIconResourceIDTag,
				sizeof(SInt16), &resID);
				
	return resID;
}


// ---------------------------------------------------------------------------
//	SetContentInfo												  [public]
/**
	Sets the content inforomation
	
	@param	inContent	Content inforomation								*/

void
IconControl::SetContentInfo(
	const ControlButtonContentInfo&	inContent)
{
	SetDataTag(kControlIconPart, kControlIconContentTag,
				sizeof(ControlButtonContentInfo), &inContent);
}


// ---------------------------------------------------------------------------
//	GetContentInfo												  [public]
/**
	Passes back the content inforomation
	
	@param	outContent	Content inforomation								*/

void
IconControl::GetContentInfo(
	ControlButtonContentInfo&	outContent) const
{
	GetDataTag(kControlIconPart, kControlIconContentTag,
				sizeof(ControlButtonContentInfo), &outContent);
}


} // namespace PPx
