// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxNibDecoderUtils.h
//
//	Copyright © 2004-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:23 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#ifndef H_PPxNibDecoderUtils
#define H_PPxNibDecoderUtils
#pragma once

#include <Carbon/Carbon.h>

namespace PPx {

class View;

namespace NibDecoderUtils {

	void				SetQDRectFromString(
								CFStringRef	inString,
								Rect&		outRect);

	void				SetHIRectFromString(
								CFStringRef	inString,
								HIRect&		outRect);

	void				SetViewFont(
								View*		inView,
								ControlSize	inControlSize);

	void				SetViewSizeAndFont(
								View*		inView,
								ControlSize	inControlSize);

} // namespace NibDecoderUtils

} // namespace PPx

#endif	// H_PPxNibDecoderUtils

