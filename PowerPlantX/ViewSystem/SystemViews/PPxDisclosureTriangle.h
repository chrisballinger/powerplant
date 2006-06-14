// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxDisclosureTriangle.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:41 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxDisclosureTriangle.h
	@brief		A system disclosure triangle control
*/

#ifndef H_PPxDisclosureTriangle
#define H_PPxDisclosureTriangle
#pragma once

#include <PPxView.h>

namespace PPx {

// ===========================================================================
//	DisclosureTriangle
/**
	A system disclosure triangle control */

class	DisclosureTriangle : public View {
public:
						DisclosureTriangle();
						
	virtual				~DisclosureTriangle();
	
	void				Initialize(
								View*			inSuperView,
								const HIRect&	inFrame,
								bool			inVisible,
								bool			inEnabled,
								ControlDisclosureTriangleOrientation inOrientation,
								CFStringRef		inTitle,
								SInt32			inInitialValue,
								bool			inDrawTitle,
								bool			inAutoToggle);
	
protected:	
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;
								
private:
	virtual CFStringRef	ClassName() const;
	
private:
									/** Pointing left or right */
	ControlDisclosureTriangleOrientation	mOrientation;
	bool			mDrawTitle;		/**< Whether to draw a text title */
	bool			mAutoToggle;	/**< Toggle between open/closed when clickec */
};


} // namespace PPx

#endif	// H_PPxDisclosureTriangle
