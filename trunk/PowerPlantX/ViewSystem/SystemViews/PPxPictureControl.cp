// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxPictureControl.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:42 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxPictureControl.h>
#include <PPxSerializer.h>
#include <SysCreateView.h>

namespace PPx {

namespace {		// unnamed namespace 

	// -----------------------------------------------------------------------
	//	Key names for persistent data

	const CFStringRef	key_DontTrack	= CFSTR("do not track");
}


// ---------------------------------------------------------------------------
//	PictureControl												  [public]
/**
	Default constructor */

PictureControl::PictureControl()
{
	mPictResID = 0;
	mDontTrack = false;
}


// ---------------------------------------------------------------------------
//	~PictureControl												  [public]
/**
	Destructor */

PictureControl::~PictureControl()
{
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initialize from icon control creation parameters
	
	@param	inSuperView			Parent view
	@param	inFrame				Bounds for view, in local coords of parent
	@param	inVisible			Whether the view is visible
	@param	inEnabled			Whether the view is enabled
	@param	inPictResID			PICT resource ID of picture to display
	@param	inPictureHandle		Handle to picture to dispaly
	@param	inDontTrack			Whether not to track mouse downs			*/

void
PictureControl::Initialize(
	View*				inSuperView,
	const HIRect&		inFrame,
	bool				inVisible,
	bool				inEnabled,
	SInt16				inPictResID,
	PicHandle			inPictureHandle,
	bool				inDontTrack)
{
	mPictResID = inPictResID;
	mDontTrack = inDontTrack;
	
	HIViewRef	viewRef = SysCreateView::PictureControl(
								inPictResID, inPictureHandle, inDontTrack);
	
	View::Initialize(viewRef, inSuperView, inFrame, inVisible, inEnabled);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
PictureControl::ClassName() const
{
	return CFSTR("PPx::PictureControl");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
PictureControl::InitState(
	const DataReader&	inReader)
{
	inReader.ReadOptional(key_ContentResID, mPictResID);
	inReader.ReadOptional(key_DontTrack, mDontTrack);
	
	HIViewRef	viewRef = SysCreateView::PictureControl(
								mPictResID, nil, mDontTrack);
	
	View::InitViewState(viewRef, inReader);	// Data for all Views
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
PictureControl::WriteState(
	DataWriter&	ioWriter) const
{
											// Data for Picture Control
	ioWriter.WriteValue(key_ContentResID, mPictResID);
	ioWriter.WriteValue(key_DontTrack, mDontTrack);
	
	View::WriteState(ioWriter);				// Data for all Views
}

#pragma mark -

// ---------------------------------------------------------------------------
//	SetPicture													  [public]
/**
	Sets the picture
	
	@param	inPicture	PicHandle for the picture							*/

void
PictureControl::SetPicture(
	PicHandle	inPicture)
{
	SetDataTag(kControlPicturePart, kControlPictureHandleTag,
				sizeof(PicHandle), &inPicture);
				
	mPictResID = 0;							// No longer using PICT resource
}


// ---------------------------------------------------------------------------
//	GetPicture													  [public]
/**
	Returns the PicHandle for the picture
	
	@return	PicHandle for the picture										*/

PicHandle
PictureControl::GetPicture() const
{
	PicHandle	picture;

	GetDataTag(kControlPicturePart, kControlPictureHandleTag,
				sizeof(PicHandle), &picture);
				
	return picture;
}


} // namespace PPx
