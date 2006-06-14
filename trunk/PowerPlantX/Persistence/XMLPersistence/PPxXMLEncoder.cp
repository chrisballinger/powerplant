// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxXMLEncoder.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:25 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxXMLEncoder.h>
#include <map>

namespace PPx {

#ifndef PPx_Doxygen_Parsing				// $ Doxygen Bug Workaround $ v 1.3.3
using namespace XMLConstants;			// Trouble parsing a using directive
#endif

// ---------------------------------------------------------------------------
//	Encoder Registry
//
//	For encoders we need to store 3 pieces of information:
//		type_info the DataObject used to store values of the data type
//		Name for the data type
//		XML Encoder function
//
//	Registry uses the type_info struct as the key, since type_info is
//	accessible via the typeid operator for polymorphic DataObjects.
//	The encoder uses the name of the data type as the tag name for the
//	XML element containing the data value.

namespace {	// unnamed

	typedef std::map<const std::type_info*, XMLEncoder::EncoderInfo>
																RegistryType;
	
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
	Registers an XML encoder function for a data type
	
	@param	inTypeInfo		type_info for the DataObject class used to store
								values of the data type
	@param	inTypeName		Name of the data type. Used as the XML element
								tag for values of the data type
	@param	inEncoderFunc	XML Encoder function							*/

void
XMLEncoder::Register(
	const std::type_info&	inTypeInfo,
	CFStringRef				inTypeName,		
	EncoderFuncT			inEncoderFunc)
{
	EncoderInfo	encoder = {inTypeName, inEncoderFunc};

	GetRegistry().insert(std::make_pair(&inTypeInfo, encoder));
}


// ---------------------------------------------------------------------------
//	Find
/**
	Returns the XML encoder information for a DataObject class
	
	@param	inTypeInfo		type_info for a DataObject class				*/

XMLEncoder::EncoderInfo
XMLEncoder::Find(
	const std::type_info&	inTypeInfo)
{
	RegistryType::const_iterator	pos = GetRegistry().find(&inTypeInfo);
	
	if (pos == GetRegistry().end()) {
		PPx_Throw_(RuntimeError, err_Runtime, "XML Encoder not found");
	}
	
	return pos->second;
}

#pragma mark -

// ---------------------------------------------------------------------------
//	EncodeData< LocalizedString >
/**
	Function template specialization for an XML encoder for a localized string
	
	@param	inDataObject	DataObject containing a localized string
	@param	ioDataTree		Parent XML Tree									*/

template<>
void
XMLEncoderFuncs::EncodeData<LocalizedString>(
	const DataObject&	inDataObject,
	CFXMLTree&			ioDataTree)
{
	const TDataObject<LocalizedString>&	data =
			dynamic_cast<const TDataObject<LocalizedString>&>(inDataObject);
			
		// Localized string stores the table name, if present, as an
		// attribute
	
	if (data.mValue.GetTableName().IsValid()) {
		XMLTreeBuilder::AddAttribute(ioDataTree, attr_Table,
										data.mValue.GetTableName());
	}
	
	ioDataTree.AppendChild(XMLTreeBuilder::MakeText(data.mValue.GetKeyString()));
}


// ---------------------------------------------------------------------------
//	EncodeData< Point >
/**
	Function template specialization for an XML encoder for a Quickdraw Point
	
	@param	inDataObject	DataObject containing a Point value
	@param	ioDataTree		Parent XML Tree									*/

template<>
void
XMLEncoderFuncs::EncodeData<Point>(
	const DataObject&	inDataObject,
	CFXMLTree&			ioDataTree)
{
	const TDataObject<Point>&	data = dynamic_cast<const TDataObject<Point>&>(inDataObject);
	
	XMLEncoder::EncoderInfo	encoderSInt16 = XMLEncoder::Find(typeid(TDataObject<SInt16>));
	
	XMLTreeBuilder::AddChildDataValue(data.mValue.h, field_Point_h, encoderSInt16, ioDataTree);
	XMLTreeBuilder::AddChildDataValue(data.mValue.v, field_Point_v, encoderSInt16, ioDataTree);
}


// ---------------------------------------------------------------------------
//	EncodeData< Rect >
/**
	Function template specialization for an XML encoder for a Quickdraw Rect
	
	@param	inDataObject	DataObject containing a Rect value
	@param	ioDataTree		Parent XML Tree									*/
	
template<>
void
XMLEncoderFuncs::EncodeData<Rect>(
	const DataObject&	inDataObject,
	CFXMLTree&			ioDataTree)
{
	const TDataObject<Rect>&	data = dynamic_cast<const TDataObject<Rect>&>(inDataObject);
	
	XMLEncoder::EncoderInfo	encoderSInt16 = XMLEncoder::Find(typeid(TDataObject<SInt16>));
	
	XMLTreeBuilder::AddChildDataValue(data.mValue.top, field_Rect_top, encoderSInt16, ioDataTree);
	XMLTreeBuilder::AddChildDataValue(data.mValue.left, field_Rect_left, encoderSInt16, ioDataTree);
	XMLTreeBuilder::AddChildDataValue(data.mValue.bottom, field_Rect_bottom, encoderSInt16, ioDataTree);
	XMLTreeBuilder::AddChildDataValue(data.mValue.right, field_Rect_right, encoderSInt16, ioDataTree);
}


// ---------------------------------------------------------------------------
//	EncodeData< CGPoint >
/**
	Function template specialization for an XML encoder for a CGPoint
	
	@param	inDataObject	DataObject containing a CGPoint value
	@param	ioDataTree		Parent XML Tree									*/

template<>
void
XMLEncoderFuncs::EncodeData<CGPoint>(
	const DataObject&	inDataObject,
	CFXMLTree&			ioDataTree)
{
	const TDataObject<CGPoint>&	data = dynamic_cast<const TDataObject<CGPoint>&>(inDataObject);
	
	XMLEncoder::EncoderInfo	encoderFloat = XMLEncoder::Find(typeid(TDataObject<float>));
	
	XMLTreeBuilder::AddChildDataValue(data.mValue.x, field_CGPoint_x, encoderFloat, ioDataTree);
	XMLTreeBuilder::AddChildDataValue(data.mValue.y, field_CGPoint_y, encoderFloat, ioDataTree);
}


// ---------------------------------------------------------------------------
//	EncodeData< CGSize >
/**
	Function template specialization for an XML encoder for a CGSize
	
	@param	inDataObject	DataObject containing a CGSize value
	@param	ioDataTree		Parent XML Tree									*/

template<>
void
XMLEncoderFuncs::EncodeData<CGSize>(
	const DataObject&	inDataObject,
	CFXMLTree&			ioDataTree)
{
	const TDataObject<CGSize>&	data = dynamic_cast<const TDataObject<CGSize>&>(inDataObject);
	
	XMLEncoder::EncoderInfo	encoderFloat = XMLEncoder::Find(typeid(TDataObject<float>));
	
	XMLTreeBuilder::AddChildDataValue(data.mValue.width,  field_CGSize_width,  encoderFloat, ioDataTree);
	XMLTreeBuilder::AddChildDataValue(data.mValue.height, field_CGSize_height, encoderFloat, ioDataTree);
}


// ---------------------------------------------------------------------------
//	EncodeData< CGRect >
/**
	Function template specialization for an XML encoder for a CGRect
	
	@param	inDataObject	DataObject containing a CGRect value
	@param	ioDataTree		Parent XML Tree									*/

template<>
void
XMLEncoderFuncs::EncodeData<CGRect>(
	const DataObject&	inDataObject,
	CFXMLTree&			ioDataTree)
{
	const TDataObject<CGRect>&	data = dynamic_cast<const TDataObject<CGRect>&>(inDataObject);
	
	XMLEncoder::EncoderInfo	encoderCGPoint = XMLEncoder::Find(typeid(TDataObject<CGPoint>));
	XMLTreeBuilder::AddChildDataValue(data.mValue.origin,  field_CGRect_origin,  encoderCGPoint, ioDataTree);
	
	XMLEncoder::EncoderInfo	encoderCGSize = XMLEncoder::Find(typeid(TDataObject<CGSize>));
	XMLTreeBuilder::AddChildDataValue(data.mValue.size,  field_CGRect_size,  encoderCGSize, ioDataTree);
}

#pragma mark -

// ---------------------------------------------------------------------------
//	Declare local functions in namespace

namespace XMLTreeBuilder {

	void	ReplaceSubstring(
					const CFString&	inSubstring,
					const CFString&	inReplacement,
					CFString&		ioTargetString);
					
	void	IndentLevel(
					CFXMLTree&			inFirstTree,
					SInt16				inLevel);
}


// ---------------------------------------------------------------------------
//	MakeElement
/**
	Creates an XML Tree with an element node
	
	@param	inElemTag	Name of element tag
	
	@return	XML Tree with a node having the element tag						*/

CFXMLTree
XMLTreeBuilder::MakeElement(
	CFStringRef		inElemTag)
{
	CFXMLElementInfo	elemInfo = { nil, nil, false };
	
	return CFXMLTree( CFXMLNode(elemInfo, inElemTag) );
}


// ---------------------------------------------------------------------------
//	MakeElement
/**
	Creates an XML Tree with an element node that has a list of attributes
	
	@param	inElemTag		Name of element tag
	@param	inAttrNames		Array of attribute names
	@param	inAttrValues	Array of attribute values
	@param	inAttrCount		Number of attributes
	
	@return	XML Tree with a node having the element tag and attributes		*/

CFXMLTree
XMLTreeBuilder::MakeElement(
	CFStringRef			inElemTag,
	const CFStringRef*	inAttrNames,
	const CFStringRef*	inAttrValues,
	CFIndex				inAttrCount)
{
	CFXMLElementInfo	elemInfo;
	
	CFDictionary<CFStringRef, CFStringRef>	attrsDict(
									inAttrNames, inAttrValues, inAttrCount,
									&kCFTypeDictionaryKeyCallBacks,
									&kCFTypeDictionaryValueCallBacks);
	CFArray<CFStringRef>	namesArray(inAttrNames, inAttrCount,
									&kCFTypeArrayCallBacks);

	elemInfo.attributes		= attrsDict;
	elemInfo.attributeOrder	= namesArray;
	elemInfo.isEmpty		= false;
	
	return CFXMLTree( CFXMLNode(elemInfo, inElemTag) );
}


// ---------------------------------------------------------------------------
//	MakeElement
/**
	Creates an XML Tree with an element node that has one attributes
	
	@param	inElemTag		Name of element tag
	@param	inAttrName		Naem of attribute
	@param	inAttrValue		Value of attribute
	
	@return	XML Tree with a node having the element tag and attribute		*/

CFXMLTree
XMLTreeBuilder::MakeElement(
	CFStringRef		inElemTag,
	CFStringRef		inAttrName,
	CFStringRef		inAttrValue)
{
	return MakeElement(inElemTag, &inAttrName, &inAttrValue, 1);
}


// ---------------------------------------------------------------------------
//	MakePersistentElement
/**
	Creates an XML Tree with an element node for a Persistent object
	
	@param	inStorageID		Storage ID number of the Persistent object
	@param	inClassName		Name of the class of the Persistent object
	
	@return	XML Tree with an element node for a Persistent object
	
	The element tag name specifies a Persistent object and the stoarge ID
	and class name are attributes											*/

CFXMLTree
XMLTreeBuilder::MakePersistentElement(
	ObjectStorageIDT	inStorageID,
	CFStringRef			inClassName)
{
	CFString		idStr;
	idStr.AssignNumericValue(inStorageID.Get());

	CFStringRef		attrNames[2] = { attr_StorageID, attr_Class };
	CFStringRef		attrValues[2] = { idStr, inClassName };

	return MakeElement(elem_Persistent, attrNames, attrValues, 2);
}


// ---------------------------------------------------------------------------
//	AddAttribute
/**
	Adds an attribute to an XML Tree with an element node
	
	@param	ioXMLTree		Tree to which to add attribute
	@param	inAttrName		Attribute name
	@param	inAttrValue		Attribute value
	
	@note The tree must not have a parent or children. We have this
	limitation because we have to destroy the existing tree and make a new
	one in order to add an attribute to its node. The Toolbox API does
	not have a means for modifying the node of an existing XML tree.		*/

void
XMLTreeBuilder::AddAttribute(
	CFXMLTree&		ioXMLTree,
	CFStringRef		inAttrName,
	CFStringRef		inAttrValue)
{
	CFXMLNode	node(ioXMLTree.GetNode());
		
	if (node.GetTypeCode() != kCFXMLNodeTypeElement) {
		PPx_Throw_(RuntimeError, err_Runtime,
					"XML tree node is not an element");
	}
	
	if (ioXMLTree.GetParent().IsValid() or (ioXMLTree.GetChildCount() > 0)) {
		PPx_Throw_(RuntimeError, err_Runtime,
					"Can't add attributes to a tree with a parent or children");
	}
												
										// Get existing attributes of node
	const CFXMLElementInfo*	elemInfo = (const CFXMLElementInfo*)
												::CFXMLNodeGetInfoPtr(node);
	
	CFDictionary<CFStringRef, CFStringRef>
							attrsDict(elemInfo->attributes, retain_Yes);
	CFArray<CFStringRef>	namesArray(elemInfo->attributeOrder, retain_Yes);
	
	if (elemInfo->attributes == nil) {	// Node has no attributes, so we
										//   create a new dictionary and
										//   array to store our attribute
	
		attrsDict = CFDictionary<CFStringRef, CFStringRef>(
										&inAttrName, &inAttrValue, 1,
										&kCFTypeDictionaryKeyCallBacks,
										&kCFTypeDictionaryValueCallBacks);
										
		namesArray = CFArray<CFStringRef>(&inAttrName, 1, &kCFTypeArrayCallBacks);
		
	} else {							// Node has attributes, so we add ours
		attrsDict.AddValue(inAttrName, inAttrValue);
		namesArray.AppendValue(inAttrName);
	}
										// Create a new node that has our
										//   added attribute
	CFXMLElementInfo	addedInfo;
	addedInfo.attributes		= attrsDict.DetachRef();
	addedInfo.attributeOrder	= namesArray.DetachRef();
	addedInfo.isEmpty			= false;
	
										// Create a new tree from our new node.
										// This destroys the existing tree.
	ioXMLTree = CFXMLTree( CFXMLNode(addedInfo, node.GetString()) );
}


// ---------------------------------------------------------------------------
//	MakeTextString
/**
	Creates an XML Tree with a text node
	
	@param	inString	Text to put in the node
	
	@return	XML Tree with a text node										*/

CFXMLTree
XMLTreeBuilder::MakeTextString(
	const CFString&	inString)
{
	return CFXMLTree( CFXMLNode(inString) );
}


// ---------------------------------------------------------------------------
//	Local function for replacing all occurrences of a substring within
//	a string with a replacement string (global search and replace)
//
//	$ Comment $ Move to a utility namespace so others can use this function

void
XMLTreeBuilder::ReplaceSubstring(
	const CFString&	inSubstring,
	const CFString&	inReplacement,
	CFString&		ioTargetString)
{
	CFRange	searchRange = {0, ioTargetString.GetLength()};
	
	while (searchRange.length > 0) {
	
		CFRange	foundRange;
	
		if ( ::CFStringFindWithOptions( ioTargetString, inSubstring,
										searchRange, 0, &foundRange ) ) {
										
			 ::CFStringReplace( ioTargetString.UseMutableRef(),
			 					foundRange, inReplacement );
			 					
			 searchRange.location = foundRange.location + inReplacement.GetLength();
			 searchRange.length = ioTargetString.GetLength() - searchRange.location;
			
		} else {
			break;
		}
	}
}


// ---------------------------------------------------------------------------
//	MakeText
/**
	Creates an XML Tree with a text node containing a string
	
	@param	inString	String to put into text node
	
	@return	XML Tree with a text node
	
	This function handles escaping the five special characters (ampersand,
	apostrophe, greater than, less than, and quotes)						*/

CFXMLTree
XMLTreeBuilder::MakeText(
	const CFString&	inString)
{
	CFString	escapedString(inString);

	ReplaceSubstring(CFSTR("&"),  CFSTR("&amp;"),  escapedString);
	ReplaceSubstring(CFSTR("'"),  CFSTR("&apos;"), escapedString);
	ReplaceSubstring(CFSTR(">"),  CFSTR("&gt;"),   escapedString);
	ReplaceSubstring(CFSTR("<"),  CFSTR("&lt;"),   escapedString);
	ReplaceSubstring(CFSTR("\""), CFSTR("&quot;"), escapedString);

	return MakeTextString(escapedString);
}


// ---------------------------------------------------------------------------
//	MakeText
/**
	Creates an XML Tree with a text node containing a bool value
	
	@param	inBool		bool value
	
	@return	XML Tree with a text node
	
	bool value is written as "true" or "false"								*/

CFXMLTree
XMLTreeBuilder::MakeText(
	bool	inBool)
{
	CFString	boolStr = inBool ? bool_True : bool_False;

	return MakeTextString(boolStr);
}


// ---------------------------------------------------------------------------
//	IndentLevel
//
//	Indents all nodes at the same level within an XML Tree

void
XMLTreeBuilder::IndentLevel(
	CFXMLTree&			inFirstTree,
	SInt16				inLevel)
{
	CFXMLTree	tree(inFirstTree);
	
	do {

		if (tree.GetNode().GetTypeCode() == kCFXMLNodeTypeElement) {
		
			CFXMLTree	childTree(tree.GetFirstChild());
			
			if (childTree.IsValid()) {

				if (childTree.GetNode().GetTypeCode() == kCFXMLNodeTypeElement) {

					tree.PrependChild( MakeWhitespace(whitespace_NewLineTabs[inLevel+1]) );
					IndentLevel(childTree, inLevel+1);
					tree.AppendChild( MakeWhitespace(whitespace_NewLineTabs[inLevel]) );
				}
			}
		}
		
		CFXMLTree	siblingTree(tree.GetNextSibling());
		
		if (siblingTree.IsValid()) {
			tree.InsertSibling( MakeWhitespace(whitespace_NewLineTabs[inLevel]) );
		}
		
		tree = siblingTree;
	
	} while (tree.IsValid());
}


// ---------------------------------------------------------------------------
//	FormatDescriptorsTree
/**
	Adds new line and tab character whitespace to the XML Tree
	
	@param	inDescTree		XML Tree containing object descriptors
	
	New lines and tabs make the XML easier to read when output as text		*/

void
XMLTreeBuilder::FormatDescriptorsTree(
	CFXMLTree&	inDescTree)
{
	CFXMLTree	firstPersist(inDescTree.GetFirstChild());
	IndentLevel(firstPersist, 0);
	
	inDescTree.PrependChild( MakeWhitespace(whitespace_NewLineTabs[0]) );
	inDescTree.AppendChild( MakeWhitespace(whitespace_NewLineTabs[0]) );
	inDescTree.InsertSibling( MakeWhitespace(whitespace_NewLineTabs[0]) );
}


// ---------------------------------------------------------------------------
//	MakeWhitespace
/**
	Creates an XML Tree with a whitespace node
	
	@param	inWhitespace	Whitespace charactes
	
	@return	XML Tree wiht a whitespace node									*/

CFXMLTree
XMLTreeBuilder::MakeWhitespace(
	CFStringRef		inWhitespace)
{
	return CFXMLTree( CFXMLNode(kCFXMLNodeTypeWhitespace, inWhitespace, nil) );
}


} // namepsace PPx