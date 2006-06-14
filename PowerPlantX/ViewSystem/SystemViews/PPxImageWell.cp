// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxImageWell.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:42 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxImageWell.h>
#include <PPxSerializer.h>
#include <SysCreateView.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	ImageWell														  [public]
/**
	Default constructor */

ImageWell::ImageWell()
{
}


// ---------------------------------------------------------------------------
//	~ImageWell													  [public]
/**
	Destructor */

ImageWell::~ImageWell()
{
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initialize from image well creation parameters
	
	@param	inSuperView		Parent view
	@param	inFrame			Bounds for view, in local coordinates of parent
	@param	inVisible		Whether the view is visible
	@param	inEnabled		Whether the view is enabled
	@param	inContent		Content of image well							*/

void
ImageWell::Initialize(
	View*			inSuperView,
	const HIRect&	inFrame,
	bool			inVisible,
	bool			inEnabled,
	const ControlButtonContentInfo&	inContent)
{
	HIViewRef	viewRef = SysCreateView::ImageWell(inContent);

	View::Initialize(viewRef, inSuperView, inFrame, inVisible, inEnabled);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
ImageWell::ClassName() const
{
	return CFSTR("PPx::ImageWell");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
ImageWell::InitState(
	const DataReader&	inReader)
{
	ControlButtonContentInfo	content;	// Data for Image Well
	content.contentType = kControlNoContent;
	content.u.picture = nil;
	
	inReader.ReadOptional(key_ContentType, content.contentType);
	inReader.ReadOptional(key_ContentResID, content.u.resID);
	
	HIViewRef	viewRef = SysCreateView::ImageWell(content);
	
	View::InitViewState(viewRef, inReader);	// Data for all Views
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
ImageWell::WriteState(
	DataWriter&	ioWriter) const
{
	ControlButtonContentInfo	content;	// Data for Image Well
	GetContentInfo(content);
				
	ioWriter.WriteValue(key_ContentType, content.contentType);
	
											// Write ID for resource content
	if ( (content.contentType > kControlNoContent)  &&
		 (content.contentType <= kControlContentICONRes) ) {
		ioWriter.WriteValue(key_ContentResID, content.u.resID);
	}
	
	View::WriteState(ioWriter);				// Data for all Views
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SetContentInfo												  [public]
/**
	Sets the content inforomation
	
	@param	inContent	Content inforomation								*/

void
ImageWell::SetContentInfo(
	const ControlButtonContentInfo&	inContent)
{
	SetDataTag(kControlImageWellPart, kControlImageWellContentTag,
				sizeof(ControlButtonContentInfo), &inContent);
}


// ---------------------------------------------------------------------------
//	GetContentInfo												  [public]
/**
	Passes back the content inforomation
	
	@param	outContent	Content inforomation								*/

void
ImageWell::GetContentInfo(
	ControlButtonContentInfo&	outContent) const
{
	GetDataTag(kControlImageWellPart, kControlImageWellContentTag,
				sizeof(ControlButtonContentInfo), &outContent);
}


// ---------------------------------------------------------------------------
//	SetImageTransform												  [public]
/**
	Sets the image transform
	
	@param	inTransform		Image transform									*/

void
ImageWell::SetImageTransform(
	IconTransformType	inTransform)
{
	SetDataTag(kControlImageWellPart, kControlImageWellTransformTag,
				sizeof(IconTransformType), &inTransform);
}


// ---------------------------------------------------------------------------
//	GetImageTransform												  [public]
/**
	Returns the image transform
	
	@return Image transform													*/

IconTransformType
ImageWell::GetImageTransform() const
{
	IconTransformType	transform;

	GetDataTag(kControlImageWellPart, kControlImageWellTransformTag,
				sizeof(IconTransformType), &transform);
				
	return transform;
}


// ---------------------------------------------------------------------------
//	SetDragDestinationFlag										  [public]
/**
	Sets whether the image well is a drag destination
	
	@param	inIsDragDestination		Whether it is a drag destination		*/

void
ImageWell::SetDragDestinationFlag(
	bool	inIsDragDestination)
{
	Boolean	isDragDestination = inIsDragDestination;

	SetDataTag(kControlImageWellPart, kControlImageWellIsDragDestinationTag,
				sizeof(Boolean), &isDragDestination);
}


// ---------------------------------------------------------------------------
//	GetDragDestinationFlag										  [public]
/**
	Returns whether the image well is a drag destination
	
	@return Whether the image well is a drag destination					*/

bool
ImageWell::GetDragDestinationFlag() const
{
	Boolean	isDragDestination;

	GetDataTag(kControlImageWellPart, kControlImageWellIsDragDestinationTag,
				sizeof(Boolean), &isDragDestination);
				
	return isDragDestination;
}


} // namespace PPx
