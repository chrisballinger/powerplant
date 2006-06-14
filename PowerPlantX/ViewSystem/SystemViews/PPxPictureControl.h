// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxPictureControl.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:42 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxPictureControl.h
	@brief		A system picture control
*/

#ifndef H_PPxPictureControl
#define H_PPxPictureControl
#pragma once

#include <PPxView.h>

namespace PPx {

// ===========================================================================
//	PictureControl
/**
	A system picture control */

class	PictureControl : public View {
public:
						PictureControl();
						
	virtual				~PictureControl();
	
	void				Initialize(
								View*			inSuperView,
								const HIRect&	inFrame,
								bool			inVisible,
								bool			inEnabled,
								SInt16			inPictResID,
								PicHandle		inPictureHandle,
								bool			inDontTrack);
	
	void				SetPicture( PicHandle inPicture );
	
	PicHandle			GetPicture() const;
	
protected:	
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;
								
private:
	virtual CFStringRef	ClassName() const;

private:
	SInt16		mPictResID;		/**< PICT resource ID */
	bool		mDontTrack;		/**< Whether to track mouse when clicked */
};


} // namespace PPx

#endif	// H_PPxPictureControl
