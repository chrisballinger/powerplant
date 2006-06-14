// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxGrayBox.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:39 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxGrayBox.h>
#include <PPxSerializer.h>
#include <PPxViewUtils.h>

namespace PPx {

namespace {

	const CFStringRef		key_StrokeGray	= CFSTR("stroke gray");
	const CFStringRef		key_StrokeAlpha	= CFSTR("stroke alpha");
	const CFStringRef		key_FillGray	= CFSTR("fill gray");
	const CFStringRef		key_FillAlpha	= CFSTR("fill alpha");
}


// ---------------------------------------------------------------------------
//	GrayBox															  [public]
/**
	Default constructor */

GrayBox::GrayBox()
{
	mStrokeGray		= 1.0;
	mStrokeAlpha	= 1.0;
	mFillGray		= 0.5;
	mFillAlpha		= 1.0;
}


// ---------------------------------------------------------------------------
//	Initialize														  [public]
/**
	Initializes from parameters
	
	@param	inSuperView		Parent view
	@param	inFrame			Bounds for view, in local coordinates of parent
	@param	inVisible		Whether the view is visible
	@param	inEnabled		Whether the view is enabled
	@param	inStrokeGray	Gray level for drawing frame
	@param	inStrokeAlpha	Alpha level for drawing frame
	@param	inFillGray		Gray level for filling box
	@param	inFillAlpha		Alpha level for filling box
	@param	inFeatures		Control features supported by this view			*/

void
GrayBox::Initialize(
	View*			inSuperView,
	const HIRect&	inFrame,
	bool			inVisible,
	bool			inEnabled,
	float			inStrokeGray,
	float			inStrokeAlpha,
	float			inFillGray,
	float			inFillAlpha,
	OptionBits		inFeatures)
{
	mStrokeGray		= inStrokeGray;
	mStrokeAlpha	= inStrokeAlpha;
	mFillGray		= inFillGray;
	mFillAlpha		= inFillAlpha;

	BaseView::Initialize(inSuperView, inFrame, inVisible, inEnabled,
								inFeatures);
}


// ---------------------------------------------------------------------------
//	FinishInit													   [protected]
/**
	Finishes initialization */

void
GrayBox::FinishInit()
{
	ControlDrawDoer::Install(GetSysEventTarget());
}


// ---------------------------------------------------------------------------
//	ClassName														 [private]
/**
	Returns the name of the class
	
	@return	Name of the class												*/

CFStringRef
GrayBox::ClassName() const
{
	return CFSTR("GrayBox");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
GrayBox::InitState(
	const DataReader&	inReader)
{
	inReader.ReadOptional(key_StrokeGray,	mStrokeGray);
	inReader.ReadOptional(key_StrokeAlpha,	mStrokeAlpha);
	inReader.ReadOptional(key_FillGray,		mFillGray);
	inReader.ReadOptional(key_FillAlpha,	mFillAlpha);
	
	BaseView::InitState(inReader);
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
GrayBox::WriteState(
	DataWriter&	ioWriter) const
{
	ioWriter.WriteValue(key_StrokeGray,		mStrokeGray);
	ioWriter.WriteValue(key_StrokeAlpha,	mStrokeAlpha);
	ioWriter.WriteValue(key_FillGray,		mFillGray);
	ioWriter.WriteValue(key_FillAlpha,		mFillAlpha);

	BaseView::WriteState(ioWriter);
}


// ---------------------------------------------------------------------------
//	DoControlDraw												   [protected]
/**
	Draws the gray box
	
	@param	ioEvent			CarbonEvent for control draw
	@param	inControl		ControlRef for the view
	@param	inPartCode		Part of the view to draw
	@param	inClipRgn		Clipping region
	@param	inContext		CGContext for drawing
	
	@return	Status of drawing event. Always returns noErr.					*/

OSStatus
GrayBox::DoControlDraw(
	SysCarbonEvent&		/* ioEvent */,
	ControlRef			/* inControl */,
	ControlPartCode		/* inPartCode */,
	RgnHandle			/* inClipRgn */,
	CGContextRef		inContext)
{
	HIRect	frame;
	GetLocalFrame(frame);
	
	if (inContext != nil) {				// Draw using Core Graphics
	
		::CGContextSetGrayFillColor(inContext, mFillGray, mFillAlpha);
		::CGContextFillRect(inContext, frame);

		::CGContextSetGrayStrokeColor(inContext, mStrokeGray, mStrokeAlpha);
		::CGContextStrokeRect(inContext, frame);
		
	} else {							// Draw using QuickDraw
		Rect	qdFrame;
		ViewUtils::HIToQDRect(frame, qdFrame);
		
		RGBColor	rgbGray;
		rgbGray.red		= mFillGray * 65535;
		rgbGray.green	= mFillGray * 65535;
		rgbGray.blue	= mFillGray * 65535;
		
		::RGBForeColor(&rgbGray);
		::PaintRect(&qdFrame);
		
		rgbGray.red		= mStrokeGray * 65535;
		rgbGray.green	= mStrokeGray * 65535;
		rgbGray.blue	= mStrokeGray * 65535;
		
		::RGBForeColor(&rgbGray);
		::FrameRect(&qdFrame);
	}

	return noErr;
}


} // namespace PPx