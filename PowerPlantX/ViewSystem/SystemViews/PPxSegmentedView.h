// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxSegmentedView.h
//
//	Copyright © 2004-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:45 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxSegmentedView.h
	@brief		A system segmented view
*/

#ifndef H_PPxSegmentedView
#define H_PPxSegmentedView
#pragma once

#include <PPxView.h>

// The system segmented view is only available on Mac OS 10.3 or later
#if (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_3)

namespace PPx {

// ===========================================================================
//	SegmentedView
/**
	A system segmented view */

class	SegmentedView : public View {
public:
						SegmentedView();
						
	virtual				~SegmentedView();
	
	void				Initialize(
								View*			inSuperView,
								const HIRect&	inFrame,
								bool			inVisible,
								bool			inEnabled);
	
	void				SetSegmentCount( UInt32 inSegmentCount );

	UInt32				GetSegmentCount() const;

	void				SetSegmentBehavior(
								UInt32				inIndex,
								HISegmentBehavior	inBehavior);

	HISegmentBehavior	GetSegmentBehavior( UInt32 inIndex ) const;

	void				ChangeSegmentAttributes(
								UInt32		inIndex,
								OptionBits	inAttributesToSet,
								OptionBits	inAttributesToClear);

	OptionBits			GetSegmentAttributes( UInt32 inIndex ) const;
	
	void				SetSegmentValue(
								UInt32	inIndex,
								SInt32	inValue);
	
	SInt32				GetSegmentValue( UInt32 inIndex ) const;

	void				SetSegmentEnabled(
								UInt32	inIndex,
								bool	inEnabled);
	
	bool				IsSegmentEnabled( UInt32 inIndex ) const;
	
	void				SetSegmentCommand(
								UInt32	inIndex,
								UInt32	inCommand);
	
	UInt32				GetSegmentCommand( UInt32 inIndex ) const;
	
	void				SetSegmentLabel(
								UInt32		inIndex,
								CFStringRef	inLabel);
	
	CFString			GetSegmentLabel( UInt32 inIndex ) const;

	void				SetSegmentContentWidth(
								UInt32	inIndex,
								bool	inAutoCalculateWidth,
								float	inWidth);
	
	float				GetSegmentContentWidth(
								UInt32	inIndex,
								bool*	outAutoCalculated = nil) const;
	
	void				SetSegmentImage(
								UInt32							inIndex,
								const HIViewImageContentInfo&	inImage);
	
	HIViewImageContentType
						GetSegmentImageContentType( UInt32 inIndex ) const;
	
	void				CopySegmentImage(
								UInt32					inIndex,
								HIViewImageContentInfo&	ioImage) const;
	
protected:	
	virtual void		InitState( const DataReader& inReader);
	
	virtual void		WriteState( DataWriter& ioWriter ) const;
								
private:
	virtual CFStringRef	ClassName() const;
};


} // namespace PPx

#endif // 10.3 or later

#endif	// H_PPxSegmentedView
