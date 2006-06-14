// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxUserPane.h
//
//	Copyright © 2004-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:46 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxUserPane.h
	@brief		A system user pane
*/

#ifndef H_PPxUserPane
#define H_PPxUserPane
#pragma once

#include <PPxView.h>

namespace PPx {

// ===========================================================================
//	UserPane
/**
	A system user pane */

class	UserPane : public View {
public:
						UserPane();
						
	virtual				~UserPane();
	
	void				Initialize(
								View*			inSuperView,
								const HIRect&	inFrame,
								bool			inVisible,
								bool			inEnabled,
								UInt32			inFeatures);
	
protected:	
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;
								
private:
	virtual CFStringRef	ClassName() const;
};


} // namespace PPx

#endif	// H_PPxUserPane
