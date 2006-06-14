// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxXMLEncoder.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:25 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file	PPxXMLEncoder.h
	@brief	Functions for converting information in Data Objects to
			XML Trees														*/

#ifndef H_PPxXMLEncoder
#define H_PPxXMLEncoder
#pragma once

#include <PPxDataObject.h>
#include <PPxLocalizedString.h>
#include <PPxXMLConstants.h>
#include <PPxXMLSerializer.h>
#include <SysCFXMLNode.h>
#include <SysCFXMLTree.h>

#include <typeinfo>

namespace PPx {

// ===========================================================================
//	XMLEncoder
/**
	Maintains a table which maps XML encoder functions to data types.
	An XML encoder function converts information in a DataObject to
	an XML Tree.
	
	You must register an encoder for every data type that you wish to
	write to XML descriptions												*/

namespace XMLEncoder {

						/** Encoder function signature */
	typedef	void		(*EncoderFuncT)(
								const DataObject&	inDataObject,
								CFXMLTree&			ioDataTree);

						/** Data stored for each registered enccoder type */
	struct	EncoderInfo {
		CFStringRef		typeName;		/**< XML element tag name */
		EncoderFuncT	encoderFunc;	/**< Encoder function pointer */
	};
								
	void				Register(
								const std::type_info&	inTypeInfo,
								CFStringRef				inTypeName,
								EncoderFuncT			inEncoderFunc);
								
	EncoderInfo			Find( const std::type_info& inTypeInfo );
}


// ===========================================================================
//	XMLTreeBuilder
/**
	Utility functions for building XML Trees containing data values */

namespace XMLTreeBuilder {

	CFXMLTree		MakeElement( CFStringRef inElemTag );

	CFXMLTree		MakeElement(
							CFStringRef			inElemTag,
							const CFStringRef*	inAttrNames,
							const CFStringRef*	inAttrValues,
							CFIndex				inAttrCount);

	CFXMLTree		MakeElement(
							CFStringRef		inElemTag,
							CFStringRef		inAttrName,
							CFStringRef		inAttrValue);
							
	CFXMLTree		MakePersistentElement(
							ObjectStorageIDT	inStorageID,
							CFStringRef			inClassName);
							
	void			AddAttribute(
							CFXMLTree&		ioXMLTree,
							CFStringRef		inAttrName,
							CFStringRef		inAttrValue);
	
	template <typename TData>
	void			AddChildDataValue(
							const TData&		inValue,
							CFStringRef			inName,
							const XMLEncoder::EncoderInfo&	inEncoder,
							CFXMLTree&			ioParent);
							
	CFXMLTree		MakeTextString( const CFString& inString );
							
	template <typename TData>
	CFXMLTree		MakeText( const TData& inData );
	
	CFXMLTree		MakeText( const CFString& inString );
	
	CFXMLTree		MakeText( bool inBool );
					
	void			FormatDescriptorsTree( CFXMLTree& inXMLTree );
	
	CFXMLTree		MakeWhitespace( CFStringRef inWhitespace );
}


// ---------------------------------------------------------------------------
//	AddChildDataValue
/**
	Template function for adding data as a child XML tree of another tree
	TData is a template parameter for the value type
	
	@param	inValue		Data value
	@param	inName		Name of data item
	@param	inEncoder	XML Encoder information for the data type
	@param	ioParent	Parent XML Tree										*/

template <typename TData>
void
XMLTreeBuilder::AddChildDataValue(
	const TData&					inValue,
	CFStringRef						inName,
	const XMLEncoder::EncoderInfo&	inEncoder,
	CFXMLTree&						ioParent)
{
	CFXMLTree	childTree(XMLTreeBuilder::MakeElement(
											(inEncoder.typeName),
											XMLConstants::attr_Name, inName) );
	(inEncoder.encoderFunc)(TDataObject<TData>(inValue), childTree);
	ioParent.AppendChild(childTree);
}


// ---------------------------------------------------------------------------
//	MakeText
/**
	Template function for making an XML Tree with a node containing
	the text representation of a value
	TData is a template parameter for the value type
	
	@param	inData		Data value
	
	@return	XML Tree with a node containing the value as text				*/

template <typename TData>
CFXMLTree
XMLTreeBuilder::MakeText(
	const TData&	inData)
{
	CFString	numberStr;
	numberStr.AssignNumericValue(inData);
	
	return MakeTextString(numberStr);
}


// ===========================================================================
//	XMLEncoderFuncs
/**
	XML Encoder functions for common data types */

namespace	XMLEncoderFuncs {

	template <typename TData>
	void				EncodeData(
								const DataObject&	inDataObject,
								CFXMLTree&			ioDataTree);
								
	template <typename TData>
	void				EncodeVector(
								const DataObject&	inDataVector,
								CFXMLTree&			ioDataTree);
	
	template <>
	void				EncodeData<LocalizedString>(
								const DataObject&	inDataObject,
								CFXMLTree&			ioDataTree);
								
	template <>
	void				EncodeData<Point>(
								const DataObject&	inDataObject,
								CFXMLTree&			ioDataTree);
								
	template <>
	void				EncodeData<Rect>(
								const DataObject&	inDataObject,
								CFXMLTree&			ioDataTree);
								
	template <>
	void				EncodeData<CGPoint>(
								const DataObject&	inDataObject,
								CFXMLTree&			ioDataTree);
								
	template <>
	void				EncodeData<CGSize>(
								const DataObject&	inDataObject,
								CFXMLTree&			ioDataTree);
								
	template <>
	void				EncodeData<CGRect>(
								const DataObject&	inDataObject,
								CFXMLTree&			ioDataTree);
}


// ---------------------------------------------------------------------------
//	EncodeData
/**
	Template function which stores a value from a DataObject as a child
	XML Tree of a another tree
	TData is a template parameter for the value type
	
	@param	inDataObject	DataObject containing the value
	@param	ioDataTree		Parent XML Tree
	
	This is the general case for the function template which assumes the
	value is a simple type which can be represented as a single text item.
	Create template specializations for complex types such as structs.		*/

template <typename TData>
void
XMLEncoderFuncs::EncodeData(
	const DataObject&	inDataObject,
	CFXMLTree&			ioDataTree)
{
	const TDataObject<TData>&	data = dynamic_cast<const TDataObject<TData>&>(inDataObject);
	
	ioDataTree.AppendChild(XMLTreeBuilder::MakeText(data.mValue));
}


// ---------------------------------------------------------------------------
//	EncodeVector
/**
	Template function which stores a vector of values from a DataObject as a
	child XML Tree of a another tree
	TData is a template parameter for the value type
	
	@param	inDataVector	DataObject containing the vector of values
	@param	ioDataTree		Parent XML Tree									*/

template <typename TData>
void
XMLEncoderFuncs::EncodeVector(
	const DataObject&	inDataVector,
	CFXMLTree&			ioDataTree)
{
	const TDataVector<TData>&	dataObject = dynamic_cast<const TDataVector<TData>&>(inDataVector);
	
		// Find encoder for TData
		
	XMLEncoder::EncoderInfo	itemEncoder = XMLEncoder::Find(typeid(TDataObject<TData>));
		
	typedef	typename std::vector<TData>::const_iterator	IteratorT;
		
	for ( IteratorT iter = dataObject.mDataValues.begin(),
					last = dataObject.mDataValues.end();
		  iter != last; ++iter ) {
		  
		CFXMLTree	itemTree(XMLTreeBuilder::MakeElement(itemEncoder.typeName));
		
		(itemEncoder.encoderFunc)(TDataObject<TData>(*iter), itemTree);
		
		ioDataTree.AppendChild(itemTree);
	}
}


} // namepsace PPx

#endif	// H_PPxXMLEncoder
