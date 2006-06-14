// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxXMLDecoder.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:25 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file	PPxXMLDecoder.h
	@brief	Functions for converting information in XML Trees to
			Data Objects													*/

#ifndef H_PPxXMLDecoder
#define H_PPxXMLDecoder
#pragma once

#include <PPxDataObject.h>
#include <PPxLocalizedString.h>
#include <PPxPrimaryBundle.h>
#include <SysCFXMLNode.h>
#include <SysCFXMLTree.h>

#include <typeinfo>

namespace PPx {

// ===========================================================================
//	XMLDecoder
/**
	Maintains a table of which maps XML decoder functions to data types.
	An XML decoder function converts information in an XML tree to a
	Data Object. 
	
	You need to registef a decoder for every data type that you wish to
	read from XML descriptions												*/

namespace XMLDecoder {

										/** Decoder function signature */
	typedef AutoRetained<DataObject>	(*DecoderFuncT)(const CFXMLTree&);
												
	void				Register(
								CFStringRef		inTypeName,
								DecoderFuncT	inDecoderFunc);
								
	DecoderFuncT		Find( CFStringRef inTypeName );
}


// ===========================================================================
//	XMLTreeBrowser
/**
	Utility functions for extracting values from XML Trees */

namespace XMLTreeBrowser {

	template <typename TData>
	TData			GetValue( const CFXMLTree& inDataTree );
	
	template <>
	CFString		GetValue( const CFXMLTree& inDataTree );
	
	template <>
	bool			GetValue( const CFXMLTree& inDataTree );
	
	CFXMLTree		GetStructField(
							const CFXMLTree&	inStructTree,
							const CFString&		inFieldName);
							
	template <typename TData>
	bool			GetFieldValue(
							const CFXMLTree&	inStructTree,
							const CFString&		inFieldName,
							TData&				outValue);
}


// ---------------------------------------------------------------------------
//	GetValue
/**
	Template function for getting a value from an XML Tree.
	TData is a template parameter for the type of the value.
	
	@param	inDataTree	XML Tree containing a value
	
	This is the general case for the function template. It assumes
	the value is numeric. You must create template specializations
	for non-numeric data.													*/

template <typename TData>
TData
XMLTreeBrowser::GetValue(
	const CFXMLTree&	inDataTree)
{
	return GetValue<CFString>(inDataTree).GetNumericValue<TData>();
}


// ---------------------------------------------------------------------------
//	GetFieldValue
/**
	Template function for getting a value for the field of a struct from
	an XML Tree.
	TData is a template parameter for the type of the value
	
	@param	inStructTree	XML Tree containing a struct
	@param	inFieldName		Name of the field
	@param	outValue		Value of the field
	
	@return Whether the field data exists in the struct XML Tree			*/

template <typename TData>
bool
XMLTreeBrowser::GetFieldValue(
	const CFXMLTree&	inStructTree,
	const CFString&		inFieldName,
	TData&				outValue)
{
	CFXMLTree	fieldTree( GetStructField(inStructTree, inFieldName) );
	
	bool		fieldExists = false;
	
	if (fieldTree.IsValid()) {
	
		XMLDecoder::DecoderFuncT	decoder = XMLDecoder::Find(fieldTree.GetNode().GetString());
		AutoRetained<DataObject>	dataObject( (*decoder)(fieldTree) );
		const TDataObject<TData>&	fieldObject = dynamic_cast<const TDataObject<TData>&>(*dataObject);

		outValue = fieldObject.mValue;
		fieldExists = true;
	}
	
	return fieldExists;
}


// ===========================================================================
//	XMLDecoderFuncs
/**
	XML Decoder functions for common data types */

namespace XMLDecoderFuncs {

	template <typename TData>
	AutoRetained<DataObject>
						DecodeData( const CFXMLTree& inDataTree );

	template <typename TData>
	AutoRetained<DataObject>
						DecodeVector( const CFXMLTree& inDataTree );
						
	template <>
	AutoRetained<DataObject>
						DecodeData<LocalizedString>( const CFXMLTree& inDataTree );
						
	template <>
	AutoRetained<DataObject>
						DecodeData<Point>( const CFXMLTree& inDataTree );
						
	template <>
	AutoRetained<DataObject>
						DecodeData<Rect>( const CFXMLTree& inDataTree );
						
	template <>
	AutoRetained<DataObject>
						DecodeData<CGPoint>( const CFXMLTree& inDataTree );
						
	template <>
	AutoRetained<DataObject>
						DecodeData<CGSize>( const CFXMLTree& inDataTree );
						
	template <>
	AutoRetained<DataObject>
						DecodeData<CGRect>( const CFXMLTree& inDataTree );
}


// ---------------------------------------------------------------------------
//	DecodeData
/**
	Template function which returns a DataObject containing a value
	extracted from an XML Tree
	
	TData is a template parameter for the value type
	
	@param	inDataTree	XML Tree containing the data value
	
	This is the general case template which assumes the value is a
	simple type (number, bool, or string). Create template specializations
	for complex types such as structs.										*/

template <typename TData>
AutoRetained<DataObject>
XMLDecoderFuncs::DecodeData(
	const CFXMLTree&	inDataTree)
{
	return AutoRetained<DataObject>( new TDataObject<TData>(
							XMLTreeBrowser::GetValue<TData>(inDataTree)) );
}


// ---------------------------------------------------------------------------
//	DecodeVector
/**
	Template function which returns a DataObject containing a vector of
	values extracted from an XML Tree
	
	TData is a template parameter for the value type
	
	@param	inDataTree	XML Tree containing the vector of values			*/ 

template <typename TData>
AutoRetained<DataObject>
XMLDecoderFuncs::DecodeVector(
	const CFXMLTree&	inDataTree)
{
	CFIndex		itemCount = inDataTree.GetChildCount();
	
	AutoRetained< TDataVector<TData> >	vectorData( new TDataVector<TData> );
	
	if (itemCount == 0) {
		return vectorData;
	}
	
	vectorData->mDataValues.reserve(itemCount);
	
		// Get type of first item. All items in a vector must be the same type.
	
	CFXMLTree	childTree(inDataTree.GetFirstChild());
	CFString	itemTypeName(childTree.GetNode().GetString());
	
	XMLDecoder::DecoderFuncT	decoder = XMLDecoder::Find(itemTypeName);
	
	for (int i = 0; i < itemCount; ++i) {
	
		CFXMLTree	itemTree(inDataTree.GetChildAtIndex(i), retain_Yes);
		
		AutoRetained<DataObject>	itemObject( (*decoder)(itemTree) );
		const TDataObject<TData>&	itemData =
						dynamic_cast<const TDataObject<TData>&>(*itemObject);
		
		vectorData->mDataValues.push_back(itemData.mValue);
	}
	
	return vectorData;
}


} // namepsace PPx

#endif	// H_PPxXMLDecoder
