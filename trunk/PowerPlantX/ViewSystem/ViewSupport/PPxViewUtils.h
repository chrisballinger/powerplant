// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxViewUtils.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:48 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxViewUtils.h
	@brief		Class and functions for working with Views
*/

#ifndef H_PPxViewUtils
#define H_PPxViewUtils
#pragma once

#include <PPxPrefix.h>

#include <Carbon/Carbon.h>

namespace PPx {

class	View;

// ===========================================================================
//	ViewUtils
/**
	Utility functions for working with Views */

namespace ViewUtils {
									// Using Theme Fonts
	void	SetControlThemeFontID(
					View*			inViewRef,
					ThemeFontID		inFontID );
					
	bool	GetControlThemeFontID(
					View*			inViewRef,
					ThemeFontID&	outFontID );
					
					
									// QD <--> CG Geometry Converters
	void	QDToHIPoint(
					const Point&	inQDPoint,
					HIPoint&		outHIPoint);
					
	void	HIToQDPoint(
					const HIPoint&	inHIPoint,
					Point&			outQDPoint);
					
	void	QDToHIRect(
					const Rect&		inQDRect,
					HIRect&			outHIRect);
					
	void	HIToQDRect(
					const HIRect&	inHIRect,
					Rect&			outQDRect);
}


// ===========================================================================
//	CGContextSaver
/**
	Saves and restores a Core Graphics context */

class	CGContextSaver {
public:
					CGContextSaver( CGContextRef inContext );
			
					~CGContextSaver();
	
	CGContextRef	Get() const;
			
	void			Save( CGContextRef inContext );
	
	void			Restore();
			
private:
	CGContextRef	mCGContext;
};
		


} // namespace PPx

#endif	// H_PPxViewUtils
