// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxCreateView.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:46 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxCreateView.h
	@brief		Template functions for creating PPx views
	
	The template functions in this file create and initialize an object of
	a View subclass. There are nine versions of the function, taking
	from zero to eight parameters beyond the four standard parameters.

	Via the magic of template parameter type deduction, these functions
	suffice to create any view. For example,
	
	@code
	
	PPx:ChasingArrows*	chasers = PPx::CreateView<PPx::ChasingArrows>(
										PPx::superView_None,
										frame,
										PPx::visible_Yes,
										PPx::enabled_Yes);
										
	PPx::CheckBox*		checker = PPx::CreateView<PPx::CheckBox>(
										PPx::superView_None,
										frame,
										PPx::visible_Yes,
										PPx::enabled_No,
										CFSTR("Check Box Title");
										PPx::value_Off,
										true);		// auto toggle
	@endcode
*/

#ifndef H_PPxCreateView
#define H_PPxCreateView
#pragma once

#include <PPxMemoryUtils.h>
#include <PPxView.h>
#include <memory>

namespace PPx {

// ---------------------------------------------------------------------------

const View*	const	superView_None	= nil;	/**< Nil superview */


// ---------------------------------------------------------------------------

template <class TView>
TView*
CreateView(
	View*			inSuperView,
	const HIRect&	inFrame,
	bool			inVisible,
	bool			inEnabled)
{
	std::auto_ptr<TView>	theView = CreateNew<TView>();
	
	theView->Initialize(inSuperView, inFrame, inVisible, inEnabled);
	
	theView->FinishInitPersistent();

	return theView.release();
}


// ---------------------------------------------------------------------------

template <class TView, typename T1>
TView*
CreateView(
	View*			inSuperView,
	const HIRect&	inFrame,
	bool			inVisible,
	bool			inEnabled,
	T1				in1)
{
	std::auto_ptr<TView>	theView = CreateNew<TView>();
	
	theView->Initialize(inSuperView, inFrame, inVisible, inEnabled,
						in1);
	
	theView->FinishInitPersistent();

	return theView.release();
}


// ---------------------------------------------------------------------------

template <class TView, typename T1, typename T2>
TView*
CreateView(
	View*			inSuperView,
	const HIRect&	inFrame,
	bool			inVisible,
	bool			inEnabled,
	T1				in1,
	T2				in2)
{
	std::auto_ptr<TView>	theView = CreateNew<TView>();
	
	theView->Initialize(inSuperView, inFrame, inVisible, inEnabled,
						in1, in2);
	
	theView->FinishInitPersistent();

	return theView.release();
}


// ---------------------------------------------------------------------------

template <class TView, typename T1, typename T2, typename T3>
TView*
CreateView(
	View*			inSuperView,
	const HIRect&	inFrame,
	bool			inVisible,
	bool			inEnabled,
	T1				in1,
	T2				in2,
	T3				in3)
{
	std::auto_ptr<TView>	theView = CreateNew<TView>();
	
	theView->Initialize(inSuperView, inFrame, inVisible, inEnabled,
						in1, in2, in3);
	
	theView->FinishInitPersistent();

	return theView.release();
}


// ---------------------------------------------------------------------------

template <class TView, typename T1, typename T2, typename T3, typename T4>
TView*
CreateView(
	View*			inSuperView,
	const HIRect&	inFrame,
	bool			inVisible,
	bool			inEnabled,
	T1				in1,
	T2				in2,
	T3				in3,
	T4				in4)
{
	std::auto_ptr<TView>	theView = CreateNew<TView>();
	
	theView->Initialize(inSuperView, inFrame, inVisible, inEnabled,
						in1, in2, in3, in4);
	
	theView->FinishInitPersistent();

	return theView.release();
}


// ---------------------------------------------------------------------------

template <class TView, typename T1, typename T2, typename T3, typename T4,
					   typename T5>
TView*
CreateView(
	View*			inSuperView,
	const HIRect&	inFrame,
	bool			inVisible,
	bool			inEnabled,
	T1				in1,
	T2				in2,
	T3				in3,
	T4				in4,
	T5				in5)
{
	std::auto_ptr<TView>	theView = CreateNew<TView>();
	
	theView->Initialize(inSuperView, inFrame, inVisible, inEnabled,
						in1, in2, in3, in4, in5);
	
	theView->FinishInitPersistent();

	return theView.release();
}


// ---------------------------------------------------------------------------

template <class TView, typename T1, typename T2, typename T3, typename T4,
					   typename T5, typename T6>
TView*
CreateView(
	View*			inSuperView,
	const HIRect&	inFrame,
	bool			inVisible,
	bool			inEnabled,
	T1				in1,
	T2				in2,
	T3				in3,
	T4				in4,
	T5				in5,
	T6				in6)
{
	std::auto_ptr<TView>	theView = CreateNew<TView>();
	
	theView->Initialize(inSuperView, inFrame, inVisible, inEnabled,
						in1, in2, in3, in4, in5, in6);
	
	theView->FinishInitPersistent();

	return theView.release();
}


// ---------------------------------------------------------------------------

template <class TView, typename T1, typename T2, typename T3, typename T4,
					   typename T5, typename T6, typename T7>
TView*
CreateView(
	View*			inSuperView,
	const HIRect&	inFrame,
	bool			inVisible,
	bool			inEnabled,
	T1				in1,
	T2				in2,
	T3				in3,
	T4				in4,
	T5				in5,
	T6				in6,
	T7				in7)
{
	std::auto_ptr<TView>	theView = CreateNew<TView>();
	
	theView->Initialize(inSuperView, inFrame, inVisible, inEnabled,
						in1, in2, in3, in4, in5, in6, in7);
	
	theView->FinishInitPersistent();

	return theView.release();
}


// ---------------------------------------------------------------------------

template <class TView, typename T1, typename T2, typename T3, typename T4,
					   typename T5, typename T6, typename T7, typename T8>
TView*
CreateView(
	View*			inSuperView,
	const HIRect&	inFrame,
	bool			inVisible,
	bool			inEnabled,
	T1				in1,
	T2				in2,
	T3				in3,
	T4				in4,
	T5				in5,
	T6				in6,
	T7				in7,
	T8				in8)
{
	std::auto_ptr<TView>	theView = CreateNew<TView>();
	
	theView->Initialize(inSuperView, inFrame, inVisible, inEnabled,
						in1, in2, in3, in4, in5, in6, in7, in8);
	
	theView->FinishInitPersistent();

	return theView.release();
}


} // namespace PPx

#endif	// H_PPxCreateView
