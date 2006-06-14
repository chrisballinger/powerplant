// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxBaseView.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:38 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxBaseView.h
	@brief		Basic View subclass
*/

#ifndef H_PPxBaseView
#define H_PPxBaseView
#pragma once

#include <PPxView.h>

namespace PPx {

// ===========================================================================
//	BaseView
/**
	A basic view. BaseView is a concrete View subclass that does nothing
	on its own. You add behavior by installing event handlers.				*/

class	BaseView : public View {
public:
						BaseView();
						
	virtual				~BaseView();
	
	void				Initialize(
								const HIRect&	inFrame,
								OptionBits		inFeatures = features_None);
	
	void				Initialize(
								View*			inSuperView,
								const HIRect&	inFrame,
								bool			inVisible,
								bool			inEnabled,
								OptionBits		inFeatures = features_None);
	
protected:
	virtual OptionBits	GetFeatureFlags() const;

	virtual void		InitState( const DataReader& inReader );
	
	virtual void		WriteState( DataWriter& ioWriter ) const;

private:
	virtual CFStringRef	ClassName() const;
	
	HIViewRef			CreateSysView();
	
private:
	OptionBits			mControlFeatures;	/**< Control feature flags */
};


} // namespace PPx

#endif	// H_PPxBaseView
