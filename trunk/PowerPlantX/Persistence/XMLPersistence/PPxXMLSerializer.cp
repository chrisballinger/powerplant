// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxXMLSerializer.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:26 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxXMLSerializer.h>
#include <PPxPersistent.h>
#include <PPxXMLConstants.h>
#include <PPxXMLDecoder.h>
#include <PPxXMLEncoder.h>
#include <SysCFArray.h>
#include <SysCFData.h>
#include <SysCFDictionary.h>
#include <SysCFString.h>

namespace PPx {

#ifndef PPx_Doxygen_Parsing				// $ Doxygen Bug Workaround $ v 1.3.3
using namespace XMLConstants;			// Trouble parsing a using directive
#endif


namespace { // unnamed

	const CFStringRef	xml_TargetName			= CFSTR("xml");
	const CFStringRef	xml_ProcessingInst		= CFSTR("version=\"1.0\" encoding=\"UTF-8\"");

	const CFStringRef	elem_ObjectDescriptors	= CFSTR("ObjectDescriptors");
	
	const CFStringRef	attr_Version			= CFSTR("version");
	const CFStringRef	value_VersionOne		= CFSTR("1");
}


// ---------------------------------------------------------------------------
//	XMLDataToObjects
/**
	Creates a hierarchy of persistent objects from XML data
	
	@param	inXMLData	Unicode text containing XML descriptions of objects
	
	@return	Root of persistent objects hierarchy
	
	Caller assumes ownership of the object hierarchy						*/

std::auto_ptr<Persistent>
XMLSerializer::XMLDataToObjects(
	const CFData&	inXMLData)
{
	ObjectDescriptorList	descriptors;
	
	XMLDataToDescriptors(inXMLData, descriptors);
	return Serializer::DescriptorsToObjects(descriptors);
}


// ---------------------------------------------------------------------------
//	XMLDataToDescriptors
/**
	Creates a list of object descriptors from XML data
	
	@param	inXMLData		Unicode text containing XML descriptions of objects
	@param	outDescriptors	List of object descriptors						*/

void
XMLSerializer::XMLDataToDescriptors(
	const CFData&			inXMLData,
	ObjectDescriptorList&	outDescriptors)
{
	CFXMLTree	xmlTree(inXMLData);		// Create XML Tree from XML text
	
		// Document must have two top-level elements: xml and ObjectDescritpor
	
	if (xmlTree.GetChildCount() != 2) {
		PPx_Throw_(DataError, err_XMLFormat, "XML document must have exactly two main elements");
	}
	
		// Second element must be ObjectDescriptor tag
	
	CFXMLTree	descriptorTree(xmlTree.GetChildAtIndex(1));
	
	if (descriptorTree.GetNode().GetString() != elem_ObjectDescriptors) {
		PPx_Throw_(DataError, err_XMLFormat, "XML document's second element tag must be 'ObjectDescriptor'");
	}
	
		// Process each Persistent object
	
	CFIndex		objectCount = descriptorTree.GetChildCount();
	
	for (int i = 0; i < objectCount; ++i) {
		CFXMLTree	persistTree(descriptorTree.GetChildAtIndex(i));
		CFXMLNode	persistNode(persistTree.GetNode());
		
		if (persistNode.GetString() != elem_Persistent) {
			PPx_Throw_(DataError, err_XMLFormat, "Tag for object must be 'Persistent'");
		}
		
		if ( persistNode.GetTypeCode() != kCFXMLNodeTypeElement) {
			PPx_Throw_(DataError, err_XMLFormat, "Persistent node must be an XML elemeent");
		}
		
		CFXMLElement	persistElem(persistNode);
		CFString		storageIDStr(persistElem.GetAttributeValue(attr_StorageID));
		
			// Fill in Object Descriptor for the Persistent object
		
		ObjectDescriptor	objDesc;
		objDesc.objectPtr	= nil;
		objDesc.storageID	= storageIDStr.GetNumericValue<UInt32>();
		objDesc.className.AttachRef(persistElem.GetAttributeValue(attr_Class), retain_Yes);
		objDesc.objectState.Reset( new KeyDataMap );
		
		outDescriptors.push_back(objDesc);
		
			// Put each data item into KeyDataMap
		
		CFIndex		dataCount = persistTree.GetChildCount();
		
		for (int i = 0; i < dataCount; ++i) {
			CFXMLTree	dataTree(persistTree.GetChildAtIndex(i));
			CFXMLNode	dataNode(dataTree.GetNode());
			
			XMLDecoder::DecoderFuncT	decoder = XMLDecoder::Find(dataNode.GetString());
			
			AutoRetained<DataObject>	dataObject( (*decoder)(dataTree) );
			
			CFXMLElement	dataElem(dataNode);
			
			(objDesc.objectState)->insert( std::make_pair(dataElem.GetAttributeValue(attr_Name), dataObject) );
		}
	}
}

#pragma mark -

// ---------------------------------------------------------------------------
//	ObjectsToXMLData
/**
	Creates XML data describing a hierarchy of persistent objects
	
	@param	inRootObject	Root of persistent objects hierarchy
	@param	outXMLData		XML data describing the objects					*/

void
XMLSerializer::ObjectsToXMLData(
	const Persistent*	inRootObject,
	CFData&				outXMLData)
{
	ObjectDescriptorList	descriptors;
	
	Serializer::ObjectsToDescriptors(inRootObject, descriptors);
	DescriptorsToXMLData(descriptors, outXMLData);
}


// ---------------------------------------------------------------------------
//	DescriptorsToXMLData
/**
	Creates XML data from a list of object descriptors
	
	@param	inDescriptors	List of persistent object descriptors
	@param	outXMLData		XML data describing the objects					*/

void
XMLSerializer::DescriptorsToXMLData(
	const ObjectDescriptorList&	inDescriptors,
	CFData&						outXMLData)
{
		// Structure of our XML Tree
		//
		//		Document Node
		//			XML Processing Instruction Node
		//			ObjectDescriptors Element Node
		//				Persistent Element Node 1
		//				Persistent Element Node 2
		//					...
		//				Persistent Element Node n

		// For our root Document node, we don't store the source URL in the
		// XML file and we always specify UTF-8 encoding.
	
	CFXMLDocumentInfo	docInfo;
	docInfo.sourceURL	= nil;
	docInfo.encoding	= kCFStringEncodingUTF8;
	CFXMLNode			docNode(docInfo);
	CFXMLTree			xmlTree(docNode);
	
		// Add XML Processing Instruction node
	
	CFXMLProcessingInstructionInfo	procInstInfo = { xml_ProcessingInst };
	xmlTree.AppendChild( CFXMLTree( CFXMLNode(procInstInfo, xml_TargetName) ) );
	xmlTree.AppendChild(XMLTreeBuilder::MakeWhitespace(whitespace_NewLineTabs[0]));
	
		// Add Element node for the list of Object Descriptors
	
	CFXMLTree	objDescTree( XMLTreeBuilder::MakeElement( elem_ObjectDescriptors, attr_Version, value_VersionOne) );
	xmlTree.AppendChild(objDescTree);
	
		// Add Element node for each Persistent Object Descriptor
		
	for ( ObjectDescriptorList::const_iterator objIter = inDescriptors.begin(),
											   objLast = inDescriptors.end();
		  objIter != objLast; ++objIter ) {
		  
		ObjectDescriptor	objDesc = *objIter;
		  
		CFXMLTree	persistTree( XMLTreeBuilder::MakePersistentElement(
		  								objDesc.storageID,
		  								objDesc.className) );
		  								
		  		// Add node for each data item to Persistent object tree
		  								
		for ( KeyDataMap::const_iterator dataIter = (objDesc.objectState)->begin(),
		  								 dataLast = (objDesc.objectState)->end();
		  		dataIter != dataLast; ++dataIter ) {
		  		
		 	XMLEncoder::EncoderInfo	encoder = XMLEncoder::Find(typeid(*(dataIter->second.Get())));
		 	
		 	CFXMLTree	dataTree(XMLTreeBuilder::MakeElement(encoder.typeName, attr_Name, dataIter->first));
		 	(encoder.encoderFunc)(*(dataIter->second.Get()), dataTree);
	 	
		 	persistTree.AppendChild(dataTree);
		}

		objDescTree.AppendChild(persistTree);
	}
	
	XMLTreeBuilder::FormatDescriptorsTree(objDescTree);
	
	outXMLData = xmlTree.GetXMLData();
}


} // namespace PPx
