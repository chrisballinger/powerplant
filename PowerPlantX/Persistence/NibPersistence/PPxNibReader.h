// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxNibReader.h
//
//	Copyright © 2004-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:24 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxNibReader.h
	@brief		Routines for reading and state information from a XML tree
				constructed from a Carbon-based nib file.
*/

#ifndef H_PPxNibReader
#define H_PPxNibReader
#pragma once

#include <PPxNibConstants.h>
#include <PPxView.h>
#include <PPxXMLDecoder.h>
#include <SysCFString.h>
#include <SysCFXMLTree.h>


namespace PPx {

// ===========================================================================
//	NibReader
/**
	A class for reading state information from a XML tree constructed from
	a Carbon-based nib file.												*/

class	NibReader {
public:
						NibReader( const CFXMLTree& inTree );

	bool				ReadOptionalElement(
								CFStringRef	inElemType,
								CFStringRef	inName,
								CFXMLTree&	outValue) const;

	void				ReadRequiredElement(
								CFStringRef	inElemType,
								CFStringRef	inName,
								CFXMLTree&	outValue) const;

	template <typename TData>
	bool				ReadOptionalInt(
								CFStringRef	inName,
								TData&		outValue) const;

	template <typename TData>
	void				ReadRequiredInt(
								CFStringRef	inName,
								TData&		outValue) const;

	template <typename TData>
	bool				ReadOptionalSignedInt(
								CFStringRef	inName,
								TData&		outValue) const;

	template <typename TData>
	void				ReadRequiredSignedInt(
								CFStringRef	inName,
								TData&		outValue) const;
	
	template <typename TData>
	bool				ReadOptionalUnsignedInt(
								CFStringRef	inName,
								TData&		outValue) const;
	
	template <typename TData>
	void				ReadRequiredUnsignedInt(
								CFStringRef	inName,
								TData&		outValue) const;
	
	bool				ReadOptionalString(
								CFStringRef	inName,
								CFString&	outValue) const;

	void				ReadRequiredString(
								CFStringRef	inName,
								CFString&	outValue) const;

	bool				ReadOptionalBool(
								CFStringRef	inName,
								bool&		outValue) const;

	void				ReadRequiredBool(
								CFStringRef	inName,
								bool&		outValue) const;

	bool				ReadOptionalBoolean(
								CFStringRef	inName,
								Boolean&	outValue) const;

	void				ReadRequiredBoolean(
								CFStringRef	inName,
								Boolean&	outValue) const;

	bool				ReadOptionalOSType(
								CFStringRef	inName,
								OSType&		outValue) const;

	void				ReadRequiredOSType(
								CFStringRef	inName,
								OSType&		outValue) const;

	bool				ReadOptionalFloat(
								CFStringRef	inName,
								float&		outValue) const;

	void				ReadRequiredFloat(
								CFStringRef	inName,
								float&		outValue) const;

	bool				ReadOptionalMenu(
								CFStringRef	inName,
								MenuRef&	outValue) const;

	void				ReadRequiredMenu(
								CFStringRef	inName,
								MenuRef&	outValue) const;

	void				ReadCommonValues(
								const View*	inSuperView,
								HIRect&		outFrame,
								bool&		outIsVisible,
								bool&		outIsEnabled) const;

	void				ReadOptionalControlInfo( View* inView ) const;

	void				ReadOptionalHelpInfo( View* inView ) const;

	void				ReadOptionalLayoutInfo( View* inView ) const;

	void				ReadOptionalSubViews( View* inView ) const;

private:
										// Forbid copy and assignment
						NibReader( const NibReader& );
	NibReader&			operator=( const NibReader& );

private:
	CFXMLTree			mXMLTree;
};


// ---------------------------------------------------------------------------
//	ReadOptionalInt
/**
	+++
	
	@param	inName			Key name of item
	@param	outValue		+++

	@return Whether an item with the key name exists						*/

template <typename TData>
bool
NibReader::ReadOptionalInt(
	CFStringRef	inName,
	TData&		outValue) const
{
	if (ReadOptionalSignedInt(inName, outValue) or
		ReadOptionalUnsignedInt(inName, outValue))
	{
		return true;
	}
	return false;
}

// ---------------------------------------------------------------------------
//	ReadRequiredInt
/**
	+++
	
	@param	inName			Key name of item
	@param	outValue		+++												*/

template <typename TData>
void
NibReader::ReadRequiredInt(CFStringRef inName, TData& outValue) const
{
	if (not ReadOptionalInt(inName, outValue))
		PPx_Throw_(DataError, err_MissingData, "Required value not found");
}

// ---------------------------------------------------------------------------
//	ReadOptionalSignedInt
/**
	+++
	
	@param	inName			Key name of item
	@param	outValue		+++

	@return Whether an item with the key name exists						*/

template <typename TData>
bool
NibReader::ReadOptionalSignedInt(CFStringRef inName, TData& outValue) const
{
	CFXMLTree intTree;
	if (ReadOptionalElement(NibConstants::elem_int, inName, intTree))
	{
		CFString intStr(XMLTreeBrowser::GetValue<CFString>(intTree));
		if (intStr.IsValid())
		{
			outValue = intStr.GetNumericValue<TData>();
			return true;
		}
		PPx_SignalString_("A problem occurred getting the value");
		return false;
	}
	return false;
}

// ---------------------------------------------------------------------------
//	ReadRequiredSignedInt
/**
	+++
	
	@param	inName			Key name of item
	@param	outValue		+++												*/

template <typename TData>
void
NibReader::ReadRequiredSignedInt(CFStringRef inName, TData& outValue) const
{
	if (not ReadOptionalSignedInt(inName, outValue))
		PPx_Throw_(DataError, err_MissingData, "Required value not found");
}

// ---------------------------------------------------------------------------
//	ReadOptionalUnsignedInt
/**
	+++
	
	@param	inName			Key name of item
	@param	outValue		+++

	@return Whether an item with the key name exists						*/

template <typename TData>
bool
NibReader::ReadOptionalUnsignedInt(CFStringRef inName, TData& outValue) const
{
	CFXMLTree intTree;
	if (ReadOptionalElement(NibConstants::elem_unsigned_int, inName, intTree))
	{
		CFString intStr(XMLTreeBrowser::GetValue<CFString>(intTree));
		if (intStr.IsValid())
		{
			outValue = intStr.GetNumericValue<TData>();
			return true;
		}
		PPx_SignalString_("A problem occurred getting the value");
		return false;
	}
	return false;
}

// ---------------------------------------------------------------------------
//	ReadRequiredUnsignedInt
/**
	+++
	
	@param	inName			Key name of item
	@param	outValue		+++												*/

template <typename TData>
void
NibReader::ReadRequiredUnsignedInt(CFStringRef inName, TData& outValue) const
{
	if (not ReadOptionalUnsignedInt(inName, outValue))
		PPx_Throw_(DataError, err_MissingData, "Required value not found");
}

} // namespace PPx

#endif // H_PPxNibReader
