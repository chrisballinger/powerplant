// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxXMLSerializer.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:26 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxXMLSerializer.h
	@brief		Functions for converting between XML data and persistent
				objects
*/

#ifndef H_PPxXMLSerializer
#define H_PPxXMLSerializer
#pragma once

#include <PPxSerializer.h>
#include <PPxPrimaryBundle.h>

namespace PPx {

class CFData;

// ===========================================================================
//	XMLSerializer
/**
	Functions for converting between XML data and persistent objects		*/

namespace XMLSerializer {
								/** Resource type (and file extension) for
									XML object descriptor data */
	const CFStringRef	xml_ResourceType	= CFSTR("pobj");
	
						
						// Decoding from XML
						
	template <class TRoot>
	std::auto_ptr<TRoot>
					ResourceToObjects( CFStringRef inFileName );
					
	std::auto_ptr<Persistent>
					XMLDataToObjects( const CFData& inXMLData );

	void			XMLDataToDescriptors(
							const CFData&			inXMLData,
							ObjectDescriptorList&	outDescriptors);

						// Encoding to XML

	void			ObjectsToXMLData(
							const Persistent*		inRootObject,
							CFData&					outXMLData);

	void			DescriptorsToXMLData(
							const ObjectDescriptorList&	inDescriptors,
							CFData&						outXMLData);
}


// ---------------------------------------------------------------------------
//	ResourceToObjects
/**
	Creates a hierarchy of persistent objects from XML data stored in
	a resource file of the primary bundle. Template parameter is the
	type of the root object.
	
	@param	inResourceName	Name of resource file
	
	@return	Root of persistent objects hierarchy
	
	By convention, XML data files have a .pobj extension and are stored
	within the resource folder of the primary bundle. The resource name
	should be the file name without the .pobj extension.
	
	The default system search algorithm for bundled resources is used to
	find the proper localized version of the resource file based on the
	user's language preferences.
	
	Caller assumes ownership of the object hierarchy.						*/

template <class TRoot>
std::auto_ptr<TRoot>
XMLSerializer::ResourceToObjects(
	CFStringRef		inResourceName)
{
										// Get XML from resource
    CFData  xmlData( PrimaryBundle::GetResourceData( inResourceName,
                                                     xml_ResourceType ) );
                                                     
										// Make objects from XML
	std::auto_ptr<Persistent> root = XMLSerializer::XMLDataToObjects(xmlData);
	
		// Cast root to requested type. If dynamic cast fails, we have
		// a type mismatch and throw an exception. The destructor for
		// the auto_ptr will delete the root object.

	TRoot*	requestedRoot = dynamic_cast<TRoot*>(root.get());
	PPx_ThrowIfNil_(requestedRoot, RuntimeError, err_DynamicCast,
					"Root object is not the requested type");
	
		// All is well. Release root from its auto_ptr and pass back
		// requestedRoot in a temporary auto_ptr.
	
	root.release();
	
	return std::auto_ptr<TRoot>(requestedRoot);
}


} // namespace PPx

#endif // H_PPxXMLSerializer