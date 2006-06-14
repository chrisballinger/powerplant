// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxFrameAdapter.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:48 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxFrameAdapter.h
	@brief		Classes for adjucting the frame of a view
*/

#ifndef H_PPxFrameAdapter
#define H_PPxFrameAdapter
#pragma once

#include <PPxPersistent.h>

namespace PPx {

// ===========================================================================
//	FrameAdapter
/**
	Abstract class for adjusting the frame of a view when its container
	frame changes size														*/

class	FrameAdapter : public Persistent {
public:
	void				AdaptFrame(
								const HIRect&	inOldSuperFrame,
								const HIRect&	inNewSuperFrame,
								HIRect&			ioFrame) const;

private:
	virtual void		DoAdaptFrame(
								const HIRect&	inOldSuperFrame,
								const HIRect&	inNewSuperFrame,
								HIRect&			ioFrame) const = 0;
};


// ---------------------------------------------------------------------------
//	AdaptFrame
/**
	Non-virtual public wrapper for DoAdaptFrame */

inline void
FrameAdapter::AdaptFrame(
	const HIRect&	inOldSuperFrame,
	const HIRect&	inNewSuperFrame,
	HIRect&			ioFrame) const
{
	DoAdaptFrame(inOldSuperFrame, inNewSuperFrame, ioFrame);
}


// ===========================================================================
//	BindingsFrameAdapter
/**
	Adjusts a view frame based on whether its sides are bound to the
	corresponding sides of its container frame								*/

class	BindingsFrameAdapter : public FrameAdapter {
public:
						BindingsFrameAdapter();
						
	virtual				~BindingsFrameAdapter();
	
	void				SetBindings(
								bool	inBindLeft,
								bool	inBindTop,
								bool	inBindRight,
								bool	inBindBottom);
protected:
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;

private:
	virtual CFStringRef	ClassName() const;
	
	virtual void		DoAdaptFrame(
								const HIRect&	inOldSuperFrame,
								const HIRect&	inNewSuperFrame,
								HIRect&			ioFrame) const;

private:
	bool		mBindLeft;
	bool		mBindTop;
	bool		mBindRight;
	bool		mBindBottom;
};


} // namespace PPx

#endif	// H_PPxFrameAdapter
