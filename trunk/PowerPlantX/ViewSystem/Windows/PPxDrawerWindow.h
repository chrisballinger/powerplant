// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxDrawerWindow.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:48 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxDrawerWindow.h
	@brief		A drawer which slides out from an edge of a parent window
*/

#ifndef H_PPxDrawerWindow
#define H_PPxDrawerWindow
#pragma once

#include <PPxWindow.h>

namespace PPx {

// ===========================================================================
//	DrawerWindow
/**
	A drawer which slides out from an edge of a parent window */

class	DrawerWindow : public Window {
public:
						DrawerWindow();
						
	void				Initialize(
								WindowAttributes	inWindAttrs,
								Window*				inParentWindow);

									// Get/Set properties							
	void				SetParentWindow( Window* inParent );
	
	Window*				GetParentWindow() const;

	void				SetPreferredEdge( OptionBits inPreferredEdge );
						
	OptionBits			GetPreferredEdge() const;
	
	OptionBits			GetCurrentEdge() const;
	
	WindowDrawerState	GetDrawerState() const;
	
	void				SetDrawerOffsets(
								float	inLeadingOffset,
								float	inTrailingOffset);
								
	void				GetDrawerOffsets(
								float&	outLeadingOffset,
								float&	outTrailingOffset) const;
								
									// Display
	void				Toggle();
	
	void				OpenDrawer(
								OptionBits	inPreferredEdge,
								bool		inAsync);
								
	void				CloseDrawer( bool inAsync );

protected:	
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;
								
private:
	virtual CFStringRef	ClassName() const;
};


} // namespace PPx

#endif	// H_PPxDrawerWindow
