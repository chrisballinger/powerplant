// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxXMLDecoder.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:25 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxXMLDecoder.h>
#include <PPxXMLConstants.h>
#include <map>

namespace PPx {

#ifndef PPx_Doxygen_Parsing				// $ Doxygen Bug Workaround $ v 1.3.3
using namespace XMLConstants;			// Trouble parsing a using directive
#endif

// ---------------------------------------------------------------------------
//	Decoder Registry

namespace { // unnamed

	typedef std::map<CFString, XMLDecoder::DecoderFuncT>	RegistryType;
	
	RegistryType&
	GetRegistry()
	{
		static RegistryType		sRegistry;
		return sRegistry;
	}
}


// ---------------------------------------------------------------------------
//	Register
/**
	Registers an XML decoder function for a data type
	
	@param	inTypeName		Name of data type
	@param	inDecoderFunc	XML Decoder function							*/

void
XMLDecoder::Register(
	CFStringRef				inTypeName,		
	DecoderFuncT			inDecoderFunc)
{
	GetRegistry().insert(std::make_pair(CFString(inTypeName), inDecoderFunc));
}


// ---------------------------------------------------------------------------
//	Find
/**
	Returns the XML decoder function for a data type
	
	@param	inTypeName		Name of data type
	
	@return	XML Decorder function for the data type							*/

XMLDecoder::DecoderFuncT
XMLDecoder::Find(
	CFStringRef		inTypeName)
{
	RegistryType::const_iterator	pos = GetRegistry().find(CFString(inTypeName));
	
	if (pos == GetRegistry().end()) {
		PPx_Throw_(RuntimeError, err_Runtime, "XML Decoder not found");
	}
	
	return pos->second;
}

#pragma mark -

// ---------------------------------------------------------------------------
//	DecodeData< LocalizedString >
/**
	Function template specialization for an XML Decoder for a localized
	string.
	
	@param	inDataTree	XML tree containing data for a localized string		*/

template <>
AutoRetained<DataObject>
XMLDecoderFuncs::DecodeData<LocalizedString>(
	const CFXMLTree&	inDataTree)
{
	CFXMLElement	elem(inDataTree.GetNode());
	
	return AutoRetained<DataObject>(
				new TDataObject<LocalizedString>(
					LocalizedString( XMLTreeBrowser::GetValue<CFString>(inDataTree),
									 elem.GetAttributeValue(attr_Table) ) ) );
}


// ---------------------------------------------------------------------------
//	DecodeData< Point >
/**
	Function template specialization for an XML Decoder for a Quickdraw Point
	
	@param	inDataTree	XML Tree containing data for a Point
	
	@return	DataObject containing a Point value								*/

template <>
AutoRetained<DataObject>
XMLDecoderFuncs::DecodeData<Point>(
	const CFXMLTree&	inDataTree)
{
	Point	thePoint = { 0, 0 };
	
	XMLTreeBrowser::GetFieldValue(inDataTree, field_Point_h, thePoint.h);
	XMLTreeBrowser::GetFieldValue(inDataTree, field_Point_v, thePoint.v);
	
	return AutoRetained<DataObject>( new TDataObject<Point>(thePoint) );
}


// ---------------------------------------------------------------------------
//	DecodeData< Rect >
/**
	Function template specialization for an XML Decoder for a Quickdraw Rect
	
	@param	inDataTree	XML Tree containing data for a Rect
	
	@return	DataObject containing a Rect value								*/

template <>
AutoRetained<DataObject>
XMLDecoderFuncs::DecodeData<Rect>(
	const CFXMLTree&	inDataTree)
{
	Rect	theRect = { 0, 0, 0, 0 };
	
	XMLTreeBrowser::GetFieldValue(inDataTree, field_Rect_top,    theRect.top);
	XMLTreeBrowser::GetFieldValue(inDataTree, field_Rect_left,   theRect.left);
	XMLTreeBrowser::GetFieldValue(inDataTree, field_Rect_bottom, theRect.bottom);
	XMLTreeBrowser::GetFieldValue(inDataTree, field_Rect_right,  theRect.right);
	
	return AutoRetained<DataObject>( new TDataObject<Rect>(theRect) );
}


// ---------------------------------------------------------------------------
//	DecodeData< CGPoint >
/**
	Function template specialization for an XML Decoder for a CGPoint
	
	@param	inDataTree	XML Tree containing data for a CGPoint
	
	@return	DataObject containing a CGPoint value							*/

template <>
AutoRetained<DataObject>
XMLDecoderFuncs::DecodeData<CGPoint>(
	const CFXMLTree&	inDataTree)
{
	CGPoint	thePoint = { 0, 0 };
	
	XMLTreeBrowser::GetFieldValue(inDataTree, field_CGPoint_x, thePoint.x);
	XMLTreeBrowser::GetFieldValue(inDataTree, field_CGPoint_y, thePoint.y);
	
	return AutoRetained<DataObject>( new TDataObject<CGPoint>(thePoint) );
}


// ---------------------------------------------------------------------------
//	DecodeData< CGSize >
/**
	Function template specialization for an XML Decoder for a CGSize
	
	@param	inDataTree	XML Tree containing data for a CGSize
	
	@return	DataObject containing a CGSize value							*/

template <>
AutoRetained<DataObject>
XMLDecoderFuncs::DecodeData<CGSize>(
	const CFXMLTree&	inDataTree)
{
	CGSize	theSize = { 0, 0 };
	
	XMLTreeBrowser::GetFieldValue(inDataTree, field_CGSize_width, theSize.width);
	XMLTreeBrowser::GetFieldValue(inDataTree, field_CGSize_height, theSize.height);
	
	return AutoRetained<DataObject>( new TDataObject<CGSize>(theSize) );
}


// ---------------------------------------------------------------------------
//	DecodeData< CGRect >
/**
	Function template specialization for an XML Decoder for a CGRect
	
	@param	inDataTree	XML Tree containing data for a CGRect
	
	@return	DataObject containing a CGRect value							*/

template <>
AutoRetained<DataObject>
XMLDecoderFuncs::DecodeData<CGRect>(
	const CFXMLTree&	inDataTree)
{
	CGRect	theRect = { {0, 0}, {0, 0} };
	
	XMLTreeBrowser::GetFieldValue(inDataTree, field_CGRect_origin, theRect.origin);
	XMLTreeBrowser::GetFieldValue(inDataTree, field_CGRect_size, theRect.size);
	
	return AutoRetained<DataObject>( new TDataObject<CGRect>(theRect) );
}


// ---------------------------------------------------------------------------
//	GetValue< CFString >
/**
	Function template specialization for getting a string value from
	an XML Tree
	
	@param	inDataTree	XML Tree containing data for a string
	
	@return	CFString object
	
	This function takes care of mapping the 5 escape sequences (&amp,
	&apos, &gt, &lt, &quot) to the correspoding charaters.					*/

template <>
CFString
XMLTreeBrowser::GetValue<CFString>(
	const CFXMLTree&	inDataTree)
{
	CFString	result("");

	CFIndex	childCount = inDataTree.GetChildCount();
	
	for (CFIndex child = 0; child < childCount; ++child) {
	
		CFXMLTree	childTree(inDataTree.GetChildAtIndex(child));
		CFXMLNode	childNode(childTree.GetNode());
		
		if (childNode.GetTypeCode() == kCFXMLNodeTypeText) {
			result = result + childNode.GetString();
		
		} else if (childNode.GetTypeCode() == kCFXMLNodeTypeEntityReference) {
			
			CFString	entityStr(childNode.GetString());
			
			char	escapedChar[2] = { 0, 0 };
			if (entityStr == CFSTR("amp")) {
				escapedChar[0] = '&';
			} else if (entityStr == CFSTR("apos")) {
				escapedChar[0] = '\'';
			} else if (entityStr == CFSTR("gt")) {
				escapedChar[0] = '>';
			} else if (entityStr == CFSTR("lt")) {
				escapedChar[0] = '<';
			} else if (entityStr == CFSTR("quot")) {
				escapedChar[0] = '"';
			}
			
			if (escapedChar[0] != 0) {
				result.Append(escapedChar);
			}
		}
	}
	
	return result;
}


// ---------------------------------------------------------------------------
//	GetValue< bool >
/**
	Function template specialization for getting a bool value from an XML Tree
	
	@param	inDataTree	XML Tree containing data for a bool
	
	@return	bool value														*/

template <>
bool
XMLTreeBrowser::GetValue<bool>(
	const CFXMLTree&	inDataTree)
{
	CFString	boolStr( GetValue<CFString>(inDataTree) );
	
	return (boolStr == bool_True);	// Assume any other string is false
}


// ---------------------------------------------------------------------------
//	GetStructField
/**
	Returns XML Tree for a named field within the XML Tree for a struct
	
	@param	inStructTree	XML Tree containing a struct
	@param	inFieldName		Name of the field
	
	@return	XML Tree containing the field									*/

CFXMLTree
XMLTreeBrowser::GetStructField(
	const CFXMLTree&	inStructTree,
	const CFString&		inFieldName)
{
	CFXMLTree	fieldTree(inStructTree.GetFirstChild());
	
	while (fieldTree.IsValid()) {
	
		CFXMLElement	fieldElem(fieldTree.GetNode());
		
		if (inFieldName == fieldElem.GetAttributeValue(attr_Name)) {
			break;
		}
		
		fieldTree = CFXMLTree(fieldTree.GetNextSibling());
	}
	
	return fieldTree;
}


} // namepsace PPx