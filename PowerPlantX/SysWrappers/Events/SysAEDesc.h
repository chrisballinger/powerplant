// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysAEDesc.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:33 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		SysAEDesc.h
	@brief		Classes and functions for working with Apple Events
*/

#ifndef H_SysAEDesc
#define H_SysAEDesc
#pragma once

#include <SysEventParam.h>

#include <ApplicationServices/ApplicationServices.h>

namespace PPx {

// ===========================================================================
//	AutoAEDesc
/**
	Wrapper for a system Apple Event descriptor. Implements single owner,
	shared use semantics													*/

class	AutoAEDesc {
public:
						AutoAEDesc();
						
	explicit			AutoAEDesc( const AEDesc& inDesc );
						
						AutoAEDesc( const AutoAEDesc& inOther );
						
						AutoAEDesc(
								DescType		inType,
								const void*		inData,
								SInt32			inSize);

						~AutoAEDesc();
						
	AutoAEDesc&			operator = ( const AutoAEDesc& inOther );
	
	AEDesc&				GetRef();
	
	const AEDesc&		GetRef() const;
	
	AEDesc*				GetPtr();
	
	const AEDesc*		GetPtr() const;
	
	AEDesc				Release() const;
	
	void				Reset();
	
	void				Reset( const AEDesc& inDesc );
	
	void				Adopt( AEDesc& inDesc );
	
	bool				IsOwner() const;


							// Getting the type of the descriptor
							
	DescType			GetDescType() const;
	
	bool				IsNull() const;
	
	bool				IsList() const;
	
	bool				IsRecord() const;
	
	bool				IsAppleEvent() const;
	

							// Extracting elements from Lists
							
	SInt32				GetCount() const;
	
	template <typename TItem>
	void				GetNthItem(
								SInt32		inIndex,
								TItem&		outItem) const;
								
	AutoAEDesc			GetNthDesc(
								SInt32		inIndex,
								DescType	inDesiredType = typeWildCard) const;
							

							// Extracting elements from Records
	
	template <typename TItem>
	void				GetNthItem(
								SInt32		inIndex,
								AEKeyword&	outKeyword,
								TItem&		outItem) const;
								
	AutoAEDesc			GetNthDesc(
								SInt32		inIndex,
								DescType	inDesiredType,
								AEKeyword&	outKeyword) const;
								
	template <typename TParam>
	void				GetRequiredParam(
								AEKeyword	inKeyword,
								TParam&		outParam) const;
								
	template <typename TParam>
	void				GetOptionalParam(
								AEKeyword	inKeyword,
								TParam&		outParam) const;
								
	AutoAEDesc			GetRequiredParamDesc(
								AEKeyword	inKeyword,
								DescType	inDesiredType = typeWildCard) const;
								
	AutoAEDesc			GetOptionalParamDesc(
								AEKeyword	inKeyword,
								DescType	inDesiredType = typeWildCard) const;
								
								
							// Extracting attribtes from Apple Events
							
	template <typename TAttribute>
	void				GetAttribute(
								AEKeyword	inKeyword,
								TAttribute&	outAttribute) const;
								
	AutoAEDesc			GetAttributeDesc(
								AEKeyword	inKeyword,
								DescType	inDesiredType = typeWildCard) const;

private:
	void				DisposeDesc();
								
private:
	AEDesc			mAEDesc;
	mutable bool	mIsOwner;
};


// ---------------------------------------------------------------------------

inline	AEDesc&			AutoAEDesc::GetRef()		{ return mAEDesc; }

inline	const AEDesc&	AutoAEDesc::GetRef() const	{ return mAEDesc; }

inline	AEDesc*			AutoAEDesc::GetPtr()		{ return &mAEDesc; }

inline	const AEDesc*	AutoAEDesc::GetPtr() const	{ return &mAEDesc; }


// ---------------------------------------------------------------------------

inline DescType
AutoAEDesc::GetDescType() const
{
	return mAEDesc.descriptorType;
}


inline	bool
AutoAEDesc::IsNull() const
{
	return (mAEDesc.descriptorType == typeNull);
}


inline	bool
AutoAEDesc::IsList() const
{
	return (mAEDesc.descriptorType == typeAEList);
}


inline	bool
AutoAEDesc::IsRecord() const
{
	return ::AECheckIsRecord(&mAEDesc);
}


inline	bool
AutoAEDesc::IsAppleEvent() const
{
	return (mAEDesc.descriptorType == typeAppleEvent);
}


// ---------------------------------------------------------------------------

template <typename TItem>
void
AutoAEDesc::GetNthItem(
	SInt32		inIndex,
	TItem&		outItem) const
{
	AEKeyword	theKeyword;
	return GetNthItem(inIndex, theKeyword, outItem);
}


// ---------------------------------------------------------------------------

template <typename TItem>
void
AutoAEDesc::GetNthItem(
	SInt32		inIndex,
	AEKeyword&	outKeyword,
	TItem&		outItem) const
{
	typedef SysEventParam::Traits<TItem>	DataTraits;

	DescType	actualType;
	Size		actualSize;

	OSErr	err = ::AEGetNthPtr( &mAEDesc, inIndex,
								 DataTraits::type,
								 &outKeyword, &actualType, (Ptr) &outItem,
								 sizeof(TItem),  &actualSize );
	PPx_ThrowIfOSError_(err, "AEGetNthPtr failed");
}


// ---------------------------------------------------------------------------

template <typename TParam>
void
AutoAEDesc::GetRequiredParam(
	AEKeyword	inKeyword,
	TParam&		outParam) const
{
	typedef SysEventParam::Traits<TParam>	DataTraits;

	DescType	actualType;
	Size		actualSize;

	OSErr	err = ::AEGetParamPtr( &mAEDesc, inKeyword,
								   DataTraits::type,
								   &actualType, (Ptr) &outParam,
								   sizeof(TParam), &actualSize );
	PPx_ThrowIfOSError_(err, "AEGetParamPtr failed");
}


// ---------------------------------------------------------------------------

template <typename TParam>
void
AutoAEDesc::GetOptionalParam(
	AEKeyword	inKeyword,
	TParam&		outParam) const
{
	typedef SysEventParam::Traits<TParam>	DataTraits;

	DescType	actualType;
	Size		actualSize;

	OSErr	err = ::AEGetParamPtr( &mAEDesc, inKeyword,
								   DataTraits::type,
								   &actualType, (Ptr) &outParam,
								   sizeof(TParam), &actualSize );
}


// ---------------------------------------------------------------------------

template <typename TAttribute>
void
AutoAEDesc::GetAttribute(
	AEKeyword	inKeyword,
	TAttribute&	outAttribute) const
{
	typedef SysEventParam::Traits<TAttribute>	DataTraits;

	DescType	actualType;
	Size		actualSize;

	OSErr	err = ::AEGetAttributePtr( &mAEDesc, inKeyword,
									   DataTraits::type,
									   &actualType, (Ptr) &outAttribute,
									   sizeof(TAttribute),  &actualSize );
	PPx_ThrowIfOSError_(err, "AEGetAttributePtr failed");
}


} // namespace PPx

#endif	// H_SysAEDesc
