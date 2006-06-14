// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	SysAEDesc.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:33 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <SysAEDesc.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	AutoAEDesc
/**
	Default constructor */

AutoAEDesc::AutoAEDesc()
{
	::AEInitializeDesc(&mAEDesc);
	mIsOwner = true;
}


// ---------------------------------------------------------------------------
//	AutoAEDesc
/**
	Constructs from an existing AEDsc. Caller retains ownership of the AEDesc.
	
	@param	inDesc		AEDesc to use										*/

AutoAEDesc::AutoAEDesc(
	const AEDesc&	inDesc)
{
	mIsOwner = false;
	mAEDesc  = inDesc;
}


// ---------------------------------------------------------------------------
//	AutoAEDesc
/**
	Copy constructor. Object being copied transfers its ownership rights for
	the AEDesc to this object. That is, if inOther was the owner, this
	object becomes the owner. If inOther was not the owner, this object
	is not the owner either.												*/

AutoAEDesc::AutoAEDesc(
	const AutoAEDesc& inOther)
{
	mIsOwner = inOther.mIsOwner;
	mAEDesc	 = inOther.Release();
}


// ---------------------------------------------------------------------------
//	AutoAEDesc
/**
	Constructs an AEDesc with a particular DescType and data
	
	@param	inType		DescType to use for the AEDesc
	@param	inData		Pointer to the data for the AEDesc
	@param	inSize		Byte count of the data for the AEDesc				*/

AutoAEDesc::AutoAEDesc(
	DescType		inType,
	const void*		inData,
	SInt32			inSize)
{
	::AEInitializeDesc(&mAEDesc);
	mIsOwner = true;

	OSErr	err = ::AECreateDesc(inType, inData, inSize, &mAEDesc);
	PPx_ThrowIfOSError_(err, "AECreateDesc failed");
}


// ---------------------------------------------------------------------------
//	~AutoAEDesc
/**
	Destructor */

AutoAEDesc::~AutoAEDesc()
{
	DisposeDesc();
}


// ---------------------------------------------------------------------------
//	operator =
/**
	Assignment operator. Object being copied transfers its ownership rights
	for the AEDesc to this object. That is, if inOther was the owner, this
	object becomes the owner. If inOther was not the owner, this object
	does not become the owner.												*/

AutoAEDesc&
AutoAEDesc::operator = (
	const AutoAEDesc&	inOther)
{
	if (&inOther != this) {
		DisposeDesc();
		mIsOwner = inOther.mIsOwner;
		mAEDesc  = inOther.mAEDesc;
	}

	return *this;
}


// ---------------------------------------------------------------------------
//	Release
/**
	Releases ownership of the AEDesc. If this object was the owner of the
	AEDesc, caller becomes the new owner of the AEDesc.
	
	@return AEDesc used by this object										*/

AEDesc
AutoAEDesc::Release() const
{
	mIsOwner = false;
	return mAEDesc;
}


// ---------------------------------------------------------------------------
//	Reset
/**
	Resets this object by disposing its AEDesc and initializing it to
	a null descriptor														*/

void
AutoAEDesc::Reset()
{
	DisposeDesc();
	::AEInitializeDesc(&mAEDesc);
}


// ---------------------------------------------------------------------------
//	Reset
/**
	Resets AEDesc of this object to the input one, disposing of its
	current AEDesc Caller retains ownership of the AEDesc.
	
	@param	inDesc	AEDesc to use										*/

void
AutoAEDesc::Reset(
	const AEDesc&	inDesc)
{
	if (mAEDesc.dataHandle != inDesc.dataHandle)  {
		DisposeDesc();
		mAEDesc  = inDesc;
		mIsOwner = false;
	}
}


// ---------------------------------------------------------------------------
//	Adopt
/**
	Takes ownership of an AEDesc. Disposes of its current AEDesc.
	
	@param	inDesc	AEDesc to adopt											*/

void
AutoAEDesc::Adopt(
	AEDesc&	inDesc)
{
	if (mAEDesc.dataHandle != inDesc.dataHandle)  {
	
		DisposeDesc();
		mAEDesc  = inDesc;
		mIsOwner = true;
	}
}


// ---------------------------------------------------------------------------
//	IsOwner
/**
	Returns whether this object owns its AEDesc
	
	@return Whether this object owns its AEDesc								*/

bool
AutoAEDesc::IsOwner() const
{
	return mIsOwner;
}
	

// ---------------------------------------------------------------------------
//	DisposeDesc

void
AutoAEDesc::DisposeDesc()
{
	if (mIsOwner and (mAEDesc.dataHandle != nil)) {
		::AEDisposeDesc(&mAEDesc);
	}
}


// ---------------------------------------------------------------------------
//	GetCount
/**
	Returns the number of items contained by its AEDesc
	
	@return Number of items contained by its AEDesc							*/

SInt32
AutoAEDesc::GetCount() const
{
	SInt32	count;
	OSErr	err = ::AECountItems(&mAEDesc, &count);
	PPx_ThrowIfOSError_(err, "AECountItems failed");
	
	return count;
}


// ---------------------------------------------------------------------------
//	GetNthDesc
/**
	Returns a copy of a contained descriptor referred to by index number
	
	@param	inIndex			Index of descriptor to get
	@param	inDesiredType	Desired type for descriptor
	
	@return AutuAEDesc for the indexed descriptor							*/

AutoAEDesc
AutoAEDesc::GetNthDesc(
	SInt32		inIndex,
	DescType	inDesiredType) const
{
	AEKeyword	theKeyword;
	
	return GetNthDesc(inIndex, inDesiredType, theKeyword);
}


// ---------------------------------------------------------------------------
//	GetNthDesc
/**
	Returns a copy of a contained descriptor referred to by index
	number and passes back its keywork name
	
	@param	inIndex			Index of descriptor to get
	@param	inDesiredType	Desired type for descriptor
	@param	outKeyword		Keyword name for the indexed descriptor
	
	@return AutuAEDesc for the indexed descriptor							*/

AutoAEDesc
AutoAEDesc::GetNthDesc(
	SInt32		inIndex,
	DescType	inDesiredType,
	AEKeyword&	outKeyword) const
{
	AEDesc		resultDesc;

	OSErr	err = ::AEGetNthDesc( &mAEDesc, inIndex, inDesiredType,
								  &outKeyword, &resultDesc );
	PPx_ThrowIfOSError_(err, "AEGetNthDesc failed");
	
	return AutoAEDesc(resultDesc);
}


// ---------------------------------------------------------------------------
//	GetRequiredParamDesc
/**
	Gets a required descriptor parameter referred to by keywork name
	
	@param	inKeyword		Keyword name for parameter
	@param	inDesiredType	Desired type for descriptor
	
	@return AutuAEDesc for the keyword named descriptor
	
	Throws an exception it fails to get the parameter					*/
	
AutoAEDesc
AutoAEDesc::GetRequiredParamDesc(
	AEKeyword	inKeyword,
	DescType	inDesiredType) const
{
	AEDesc		resultDesc;

	OSErr	err = ::AEGetParamDesc( &mAEDesc, inKeyword, inDesiredType,
									&resultDesc );
	PPx_ThrowIfOSError_(err, "AEGetParamDesc failed");
	
	return AutoAEDesc(resultDesc);
}


// ---------------------------------------------------------------------------
//	GetOptionalParamDesc
/**
	Gets an optional descriptor parameter referred to by keywork name
	
	@param	inKeyword		Keyword name for parameter
	@param	inDesiredType	Desired type for descriptor
	
	@return AutuAEDesc for the keyword named descriptor
	
	Returned AEDesc is a null descriptor if the parameter does not exist	*/

AutoAEDesc
AutoAEDesc::GetOptionalParamDesc(
	AEKeyword	inKeyword,
	DescType	inDesiredType) const
{
	AEDesc		resultDesc;
	::AEInitializeDesc(&resultDesc);

	OSErr	err = ::AEGetParamDesc( &mAEDesc, inKeyword, inDesiredType,
									&resultDesc );
	
	return AutoAEDesc(resultDesc);
}


// ---------------------------------------------------------------------------
//	GetAttributeDesc
/**
	Gets the descriptor for a keyword named attribute
	
	@param	inKeyword		Keyword name for attribute
	@param	inDesiredType	Desired type for descriptor
	
	@return AutuAEDesc for the keyword named attribute
	
	Throws an exception it fails to get the attribute						*/

AutoAEDesc
AutoAEDesc::GetAttributeDesc(
	AEKeyword	inKeyword,
	DescType	inDesiredType) const
{
	AEDesc		resultDesc;

	OSErr	err = ::AEGetAttributeDesc( &mAEDesc, inKeyword, inDesiredType,
										&resultDesc );
	PPx_ThrowIfOSError_(err, "AEGetAttributeDesc failed");
	
	return AutoAEDesc(resultDesc);
}


} // namespace PPx
