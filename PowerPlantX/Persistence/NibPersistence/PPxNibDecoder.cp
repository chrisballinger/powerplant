// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxNibDecoder.cp
//
//	Copyright © 2004-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:21 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxNibDecoder.h>
#include <map>
#include <PPxFile.h>
#include <PPxPrimaryBundle.h>
#include <PPxViewUtils.h>
#include <SysCFBundle.h>
#include <SysCFURL.h>

namespace PPx {

namespace NibDecoder {

	CFXMLTree			GetNibXMLTree( CFStringRef inNibName );

} // namespace NibDecoder


// ---------------------------------------------------------------------------
//	CreateWindowFromNib
/**
	This is the non-template version of CreateWindowFromNib() that will create
	a new window of type PPx::Window.
	
	@param	inNibName		Name of the nib file (without the .nib extension)
							to load the window from
	@param	inWindowName	Name of the window object's name as created in
							Interface Builder
	
	@return	Pointer to newly created Window									*/

Window*
NibDecoder::CreateWindowFromNib(
	CFStringRef	inNibName,
	CFStringRef	inWindowName)
{
	return CreateWindowFromNib<Window>(inNibName, inWindowName);
}


// ---------------------------------------------------------------------------
//	GetNibXMLTree
/**
	+++
	
	@param	inNibName		Name of the nib file (without the .nib extension)
							to load the XML tree from
	
	@return	The XML tree of the data within the nib file					*/

CFXMLTree
NibDecoder::GetNibXMLTree( CFStringRef inNibName )
{
	// We will map the nib files to keep from having to read the data from disk
	// multiple times.
	static std::map<CFString, CFXMLTree> sLoadedNibs;
	
	std::map<CFString, CFXMLTree>::const_iterator iter = sLoadedNibs.find(inNibName);
	if (iter != sLoadedNibs.end())
		return iter->second;

	const CFString objectsFileName("objects.xib");

	CFBundle mainBundle(PrimaryBundle::Instance(), retain_Yes);
	PPx_ThrowIf_((not mainBundle.IsValid()), RuntimeError, err_MacOS, "Can't find main application bundle.");

	CFURL nibURL(::CFBundleCopyResourceURL(mainBundle, inNibName, CFSTR("nib"), NULL), retain_No);
	PPx_ThrowIf_((not nibURL.IsValid()), RuntimeError, err_MacOS, "Can't find the nib resource.");
	FSObject nibLoc(nibURL);

	if (not nibLoc.IsFolder())
		PPx_ThrowOSError_(errFSNotAFolder, "The found nib is not a document wrapper (folder)");
	
	// Get the objects.xib data file
	FSObject xmlLoc(nibLoc.UseRef(), objectsFileName);
	if (not xmlLoc.Exists())
		PPx_ThrowOSError_(fnfErr, "The objects.xib file can't be found within the nib file.  This may not be a Carbon-based nib.");
	
	// Read in the XML data
	CFURL xmlURL(xmlLoc.UseRef());
	SInt32 errorCode;
	CFDataRef xmlDataTemp = NULL;
	bool created = ::CFURLCreateDataAndPropertiesFromResource(NULL, xmlURL, &xmlDataTemp, NULL, NULL, &errorCode);
	CFData xmlData(xmlDataTemp, retain_No);
	if (not created or (xmlDataTemp == NULL))
		PPx_ThrowOSError_(errorCode, "CFURLCreateDataAndPropertiesFromResource failed");

	CFXMLTree nibXMLTree(xmlData, NULL, kCFXMLParserSkipWhitespace, NULL);
	sLoadedNibs.insert(std::make_pair(inNibName, nibXMLTree));
	
	return nibXMLTree;
}


// ---------------------------------------------------------------------------
//	GetWindowTreeData
/**
	+++
	
	@param	inNibName		Name of the nib file (without the .nib extension)
							to load the window from
	@param	inWindowName	Name of the window object's name as created in
							Interface Builder
	@param	outWindowTree	On return, this will be a valid CFXMLTree of the
							window tree data which is suitable for creating
							a NibReader from
	@param	outRootControlTree
							On return, this may be a valid CFXMLTree of the
							window's root control tree data.  You should only
							create a NibReader from it if it is valid.
	
	@return	void									*/

void
NibDecoder::GetWindowTreeData(
	CFStringRef	inNibName,
	CFStringRef	inWindowName,
	CFXMLTree&	outWindowTree,
	CFXMLTree&	outRootControlTree)
{
	PPx_BadParamIfNil_(inNibName);
	PPx_BadParamIfNil_(inWindowName);

	CFXMLTree nibTree = GetNibXMLTree(inNibName);
	if (nibTree.GetChildCount() != 2)
		PPx_Throw_(DataError, err_XMLFormat, "XML document must have exactly two main elements");

	CFXMLTree ibObjectData(nibTree.GetChildAtIndex(1));
	if (ibObjectData.GetNode().GetString() != NibConstants::elem_object)
		PPx_Throw_(DataError, err_XMLFormat, "The second element is not an 'object'");

	NibReader objectDataReader(ibObjectData);
	
	// Get the "allObjects" array of the nib file
	CFXMLTree allObjectsArray;
	objectDataReader.ReadRequiredElement(NibConstants::elem_array,
		NibConstants::value_allObjects, allObjectsArray);

	// Get the "nameTable" dictionary of the nib file
	CFXMLTree nameTableDictionary;
	objectDataReader.ReadRequiredElement(NibConstants::elem_dictionary,
		NibConstants::value_nameTable, nameTableDictionary);
	
	// Find the window name from the nameTable and then grab the idRef
	// from the next child
	CFString windowIDRef;
	
	CFIndex childCount = nameTableDictionary.GetChildCount();
	for (CFIndex index = 0; index < childCount - 1; index++)
	{
		CFXMLTree childTree(nameTableDictionary.GetChildAtIndex(index));
		CFXMLNode childNode(childTree.GetNode());
		if ((childNode.GetTypeCode() == kCFXMLNodeTypeElement) and
			(childNode.GetString() == NibConstants::elem_string))
		{
			if (XMLTreeBrowser::GetValue<CFString>(childTree) == inWindowName)
			{
				CFXMLTree nextChildTree(nameTableDictionary.GetChildAtIndex(index + 1));
				CFXMLNode nextChildNode(nextChildTree.GetNode());
				if ((nextChildNode.GetTypeCode() == kCFXMLNodeTypeElement) and
					(nextChildNode.GetString() == NibConstants::elem_reference))
				{
					CFXMLElement elementInfo(nextChildNode);
					CFString idRefValue(elementInfo.GetAttributeValue(NibConstants::attr_idRef));
					if (idRefValue.IsValid())
					{
						windowIDRef = idRefValue;
						break;
					}
				}
			}
		}
	}
	
	if (not windowIDRef.IsValid())
		PPx_Throw_(DataError, err_XMLFormat, "Could not find the 'idRef' of the window");

	// Initialize outWindowTree so we can test for validity later
	outWindowTree = CFXMLTree();

	childCount = allObjectsArray.GetChildCount();
	for (CFIndex index = 0; index < childCount; index++)
	{
		CFXMLTree childTree(allObjectsArray.GetChildAtIndex(index));
		CFXMLNode childNode(childTree.GetNode());
		if ((childNode.GetTypeCode() == kCFXMLNodeTypeElement) and
			(childNode.GetString() == NibConstants::elem_object))
		{
			CFXMLElement elementInfo(childNode);
			CFString idValue(elementInfo.GetAttributeValue(NibConstants::attr_id));
			if (idValue.IsValid() and (idValue == windowIDRef))
			{
				CFString classValue(elementInfo.GetAttributeValue(NibConstants::attr_class));
				if (not classValue.IsValid() or (classValue != CFSTR("IBCarbonWindow")))
					PPx_Throw_(DataError, err_XMLFormat, "The object of the window's 'id' is not a 'IBCarbonWindow' class");
				
				outWindowTree = childTree;
				break;
			}
		}
	}

	if (not outWindowTree.IsValid())
		PPx_Throw_(DataError, err_XMLFormat, "Could not find the window");


	// Find the root control.  First look for 'object' and then check for
	// 'reference' if that isn't found.
	//
	//	<object name="rootControl" class="IBCarbonRootControl" id="181">
	//		<string name="bounds">0 0 242 475 </string>
	//	</object>
	//
	//	<reference name="rootControl" idRef="283"/>
	
	// Initialize outRootControlTree so we can test for validity later
	outRootControlTree = CFXMLTree();

	NibReader windowReader(outWindowTree);
	if (not windowReader.ReadOptionalElement(NibConstants::elem_object, NibConstants::value_rootControl, outRootControlTree))
	{
		CFString rootControlID;
		CFXMLTree rootControlRefTree;
		if (windowReader.ReadOptionalElement(NibConstants::elem_reference, NibConstants::value_rootControl, rootControlRefTree))
		{
			CFXMLNode childNode(rootControlRefTree.GetNode());

			CFXMLElement elementInfo(childNode);
			CFString idRefValue(elementInfo.GetAttributeValue(NibConstants::attr_idRef));
			if (idRefValue.IsValid())
			{
				rootControlID = idRefValue;
			}
		}
		if (rootControlID.IsValid())
		{
			childCount = allObjectsArray.GetChildCount();
			for (CFIndex index = 0; index < childCount; index++)
			{
				CFXMLTree childTree(allObjectsArray.GetChildAtIndex(index));
				CFXMLNode childNode(childTree.GetNode());
				if ((childNode.GetTypeCode() == kCFXMLNodeTypeElement) and
					(childNode.GetString() == NibConstants::elem_object))
				{
					CFXMLElement elementInfo(childNode);
					CFString idValue(elementInfo.GetAttributeValue(NibConstants::attr_id));
					if (idValue.IsValid() and (idValue == rootControlID))
					{
						outRootControlTree = childTree;
						break;
					}
				}
			}
		}
	}
}


} // namespace PPx
