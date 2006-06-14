// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxNibDecoderUtils.cp
//
//	Copyright © 2004-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:23 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxNibDecoderUtils.h>
#include <PPxView.h>
#include <SysCFArray.h>

namespace PPx {

namespace {
	const CFStringRef kSpaceString = CFSTR(" ");
}

// ---------------------------------------------------------------------------
void
NibDecoderUtils::SetQDRectFromString(
	CFStringRef	inString,
	Rect&		outRect)
{
	CFArray<CFStringRef>	stringsArray(
								::CFStringCreateArrayBySeparatingStrings(
								nil, inString, kSpaceString), retain_No);

	PPx_SignalIf_(stringsArray.GetCount() < 4);

	if (stringsArray.IsValid() and (stringsArray.GetCount() >= 4)) {
		outRect.top		= ::CFStringGetIntValue(stringsArray.GetValueAt(0));
		outRect.left	= ::CFStringGetIntValue(stringsArray.GetValueAt(1));
		outRect.bottom	= ::CFStringGetIntValue(stringsArray.GetValueAt(2));
		outRect.right	= ::CFStringGetIntValue(stringsArray.GetValueAt(3));
	}
}

// ---------------------------------------------------------------------------
void
NibDecoderUtils::SetHIRectFromString(
	CFStringRef	inString,
	HIRect&		outRect)
{
	CFArray<CFStringRef>	stringsArray(
								::CFStringCreateArrayBySeparatingStrings(
								nil, inString, kSpaceString), retain_No);

	PPx_SignalIf_(stringsArray.GetCount() < 4);

	if (stringsArray.IsValid() and (stringsArray.GetCount() >= 4)) {
		outRect.origin.x	= ::CFStringGetDoubleValue(stringsArray.GetValueAt(0));
		outRect.origin.y	= ::CFStringGetDoubleValue(stringsArray.GetValueAt(1));
		outRect.size.width	= ::CFStringGetDoubleValue(stringsArray.GetValueAt(2));
		outRect.size.height	= ::CFStringGetDoubleValue(stringsArray.GetValueAt(3));
	}
}

// ---------------------------------------------------------------------------
void
NibDecoderUtils::SetViewFont(
	View*		inView,
	ControlSize	inControlSize)
{
	if (inControlSize != kControlSizeNormal)
	{
		ControlFontStyleRec style = { 0 };
		if (inControlSize == kControlSizeSmall)
		{
			style.flags |= kControlUseFontMask;
			style.font = kControlFontSmallSystemFont;
		}

#if (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_3)

		else if (inControlSize == kControlSizeMini)
		{
			style.flags |= kControlUseFontMask;
			style.font = kControlFontMiniSystemFont;
		}

#endif // 10.3 or later

		OSStatus status = ::SetControlFontStyle(inView->GetSysView(), &style);
		PPx_ThrowIfOSError_(status, "SetControlFontStyle failed");
	}
}

// ---------------------------------------------------------------------------
void
NibDecoderUtils::SetViewSizeAndFont(
	View*		inView,
	ControlSize	inControlSize)
{
	if (inControlSize != kControlSizeNormal)
	{
		inView->SetDataTag(kControlEntireControl, kControlSizeTag,
			sizeof(inControlSize), &inControlSize);

		SetViewFont(inView, inControlSize);
	}
}


} // namespace PPx
