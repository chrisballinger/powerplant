// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxViewUtils.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:48 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxViewUtils.h>
#include <PPxView.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	SetControlThemeFontID
/**
	Specify the theme font ID for drawing text in a view
	
	@param	inView		Pointer to View object
	@param	inFontID	Theme font ID
	
	@note The view must support the data tag kControlFontStyleTag
*/

void
ViewUtils::SetControlThemeFontID(
	View*			inView,
	ThemeFontID		inFontID)
{
	ControlFontStyleRec		styleRec;
	
	styleRec.flags	= kControlUseThemeFontIDMask;
	styleRec.font	= inFontID;
	
	inView->SetDataTag(kControlEntireControl, kControlFontStyleTag,
							sizeof(ControlFontStyleRec), &styleRec);
}


// ---------------------------------------------------------------------------
//	GetControlThemeFontID
/**
	Get the theme font ID used for drawing text in a view
	
	@param	inView		Pointer to View object
	@param	outFontID	Theme font ID
	
	@return	Whetherthe view uses a theme font ID. If true, outFontID is
			the theme font ID used. If false, outFontID is unchanged.
*/

bool
ViewUtils::GetControlThemeFontID(
	View*			inView,
	ThemeFontID&	outFontID)
{
	ControlFontStyleRec		styleRec;

	inView->GetDataTag(kControlEntireControl, kControlFontStyleTag,
							sizeof(ControlFontStyleRec), &styleRec);
							
	if (styleRec.flags & kControlUseThemeFontIDMask) {
		outFontID = styleRec.font;
		return true;
	}
	
	return false;
}

#pragma mark -

// ---------------------------------------------------------------------------
//	QDToHIPoint
/**
	Converts a Quickdraw Point to a HIPoint
	
	@param	inQDPoint		Quickdraw Point
	@param	outHIPoint		Corresponding HIPoint							*/

void
ViewUtils::QDToHIPoint(
	const Point&	inQDPoint,
	HIPoint&		outHIPoint)
{
	outHIPoint.x = inQDPoint.h;
	outHIPoint.y = inQDPoint.v;
}


// ---------------------------------------------------------------------------
//	HIToQDPoint
/**
	Converts a HIPoint to a Quickdraw Point
	
	@param	inHIPoint		HIPoint
	@param	outQDPoint		Corresponding Quickdraw Point
	
	Truncates the HIPoint coordinates from 32 to 16 bits					*/

void
ViewUtils::HIToQDPoint(
	const HIPoint&	inHIPoint,
	Point&			outQDPoint)
{
	outQDPoint.h = inHIPoint.x;
	outQDPoint.v = inHIPoint.y;
}


// ---------------------------------------------------------------------------
//	QDToHIRect
/**
	Converts a Quickdraw Rect to a HIRect
	
	@param	inQDRect		Quickdraw Rect
	@param	outHIRect		Corresponding HIRect							*/

void
ViewUtils::QDToHIRect(
	const Rect&		inQDRect,
	HIRect&			outHIRect)
{
	outHIRect.origin.x = inQDRect.left;
	outHIRect.origin.y = inQDRect.top;
	outHIRect.size.width = inQDRect.right - inQDRect.left;
	outHIRect.size.height = inQDRect.bottom - inQDRect.top;
}


// ---------------------------------------------------------------------------
//	HIToQDRect
/**
	Converts a HIRect to a Quickdraw REct
	
	@param	inHIRect		HIRect
	@param	outQDRect		Corresponding Quickdraw Rect
	
	Truncates the HIPoint coordinates from 32 to 16 bits					*/

void
ViewUtils::HIToQDRect(
	const HIRect&	inHIRect,
	Rect&			outQDRect)
{
	outQDRect.left = inHIRect.origin.x;
	outQDRect.top = inHIRect.origin.y;
	outQDRect.right = inHIRect.origin.x + inHIRect.size.width;
	outQDRect.bottom = inHIRect.origin.y + inHIRect.size.height;
}

#pragma mark -

// ---------------------------------------------------------------------------
//	CGContextSaver
/**
	Constructor from a CGContextRef
	
	@param	inContext	CGContext to save									*/

CGContextSaver::CGContextSaver(
	CGContextRef	inContext)
{
	mCGContext = nil;
	Save(inContext);
}


// ---------------------------------------------------------------------------
//	~CGContextSaver
/**
	Destructor. Restores saved CGContext									*/

CGContextSaver::~CGContextSaver()
{
	Restore();
}


// ---------------------------------------------------------------------------
//	Get
/**
	Returns the CGContext
	
	@return	Returns the CGContext */

CGContextRef
CGContextSaver::Get() const
{
	return mCGContext;
}


// ---------------------------------------------------------------------------
//	Save
/**
	Saves the input CGContext and restores the one formerly being saved
	
	@param	inContext	CGContext to save									*/

void
CGContextSaver::Save(
	CGContextRef	inContext)
{
	if (mCGContext != nil) {
		Restore();
	}
	
	::CGContextSaveGState(inContext);
	mCGContext = inContext;
}


// ---------------------------------------------------------------------------
//	Restore
/**
	Restores the CGContext that was being saved								*/

void
CGContextSaver::Restore()
{
	if (mCGContext != nil) {
		::CGContextRestoreGState(mCGContext);
		mCGContext = nil;
	}
}


} // namespace PPx