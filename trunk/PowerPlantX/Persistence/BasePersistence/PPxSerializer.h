// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxSerializer.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:21 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxSerializer.h
	@brief		Routines for reading and writing state information for
				Persistent objects to flattened data structures
*/

#ifndef H_PPxSerializer
#define H_PPxSerializer
#pragma once

#include <PPxDataObject.h>
#include <PPxMemoryUtils.h>

#include <deque>
#include <map>
#include <vector>

namespace PPx {

class	Persistent;

// ---------------------------------------------------------------------------
//	Type Definitions

								// Unique type for a stoage ID number
struct ObjectStorageIDStruct { };
typedef IntegerType<ObjectStorageIDStruct, UInt32>		ObjectStorageIDT;

								// Dictionary of data objects identified
								//   by string names
typedef	std::map<CFString, AutoRetained<DataObject> >	KeyDataMap;

								// Look-up table of object pointers and
								//   storage ID numbers
typedef std::map<const Persistent*, ObjectStorageIDT>	ObjectIDMap;

								// Queue of Persistent object pointers
typedef std::deque<const Persistent*>					ObjectQueue;


						/**	Stores data describing a Persistent object */
struct	ObjectDescriptor {
	Persistent*					objectPtr;		/**< Pointer to object */
	ObjectStorageIDT			storageID;		/**< ID number in descriptor list */
	CFString					className;		/**< Name of class as a string */
	AutoRefCount<KeyDataMap>	objectState;	/**< Dictionary of state data */
};	

typedef	std::vector<ObjectDescriptor>		ObjectDescriptorList;


// ===========================================================================
//	Serializer
/**
	Functions for reading and writing state information for Peristent
	objects to flattened data structures									*/

namespace Serializer {

	std::auto_ptr<Persistent>			
						DescriptorsToObjects(
								ObjectDescriptorList&	ioDescriptors);

	void				ObjectsToDescriptors(
								const Persistent*		inRootObject,
								ObjectDescriptorList&	outDescriptors);

} // namespace Serializer


// ===========================================================================
//	DataReader
/**
	A data dictionary for reading state information. The items in the
	dictionary are (key name, data object) pairs.							*/

class	DataReader {
public:
						DataReader(
								KeyDataMap&				inKeyDataMap,
								ObjectDescriptorList&	inDescriptors);
								
	bool				ContainsKey( CFStringRef inKey ) const;
								
	template <typename TData>
	bool				ReadOptional(
								CFStringRef		inKey,
								TData&			outValue) const;
								
	bool				ReadOptional(
								CFStringRef		inKey,
								CFString&		outString) const;

	template <typename TData>
	void				ReadRequired(
								CFStringRef		inKey,
								TData&			outValue) const;
	
	template <class TData, class TOutputIterator>
	bool				ReadContainer(
								CFStringRef			inKey,
								TOutputIterator		outDestination) const;

	template <class TPersistent>
	bool				ReadObjectValue(
								CFStringRef		inKey,
								TPersistent&	outObject) const;
								
	template <class TPersistent, class TOutputIterator>
	bool				ReadObjectContainer(
								CFStringRef			inKey,
								TOutputIterator		outDestination) const;

private:
	DataObject*			ReadData( CFStringRef inKey ) const;

	Persistent*			ReadObject( CFStringRef inKey ) const;
	
	Persistent*			FindObjectByID( ObjectStorageIDT inStorageID ) const;

										// Forbid copy and assignment
						DataReader( const DataReader& );
	DataReader&			operator = (const DataReader& );
private:
	KeyDataMap&				mKeyDataMap;
	ObjectDescriptorList&	mObjectDescriptors;
};


// ---------------------------------------------------------------------------
//	ReadOptional
/**
	Template function for reading the value for an optional item from
	the dictionary. Template parameter is the data type of the item.
	
	@param	inKey		Key name of item
	@param	outValue	Value of the item
	
	@return	Whether an item with the key name exists
	
	outValue is unchanged if the key item does not exist.
	
	Throws an exception if key item does exist, but has a type different
	from TData																*/

template <typename TData>
bool
DataReader::ReadOptional(
	CFStringRef			inKey,
	TData&				outValue) const
{
	typedef TDataObject<TData>	DataObjectT;

	DataObject*	data = ReadData(inKey);
	
	if (data == nil) {				// No data with specified key
		return false;
	}
	
	DataObjectT*	typedData = dynamic_cast<DataObjectT*>(data);
	
									// Check that types match
	PPx_ThrowIfNil_(typedData, DataError, err_DataFormat,
					"Value type is different from that requested");
		
	outValue = typedData->mValue;
	
	return true;
}


// ---------------------------------------------------------------------------
//	ReadRequired
/**
	Template function for reading the value for a required item from
	the dictionary. Template parameter is the data type of the item.
	
	@param	inKey		Key name of item
	@param	outValue	Value of the item
	
	The same as ReadOptional except that it throws an exception if
	an item with the key name does not exist.								*/
	

template <typename TData>
void
DataReader::ReadRequired(
	CFStringRef			inKey,
	TData&				outValue) const
{
	if (not ReadOptional(inKey, outValue)) {
		PPx_Throw_(DataError, err_MissingData, "Required value not found");
	}
}


// ---------------------------------------------------------------------------
//	ReadContainer
/**
	Template function for reading from the dictionary an item that is
	a list of values.
	
	TData is a template parameter for the data type of the item.
	TOutputIterator is a template parameter for the iterator type.

	@param	inKey			Key name of item
	@param	outDestination	Output iterator for storing data values
	
	@return	Whether an item with the key name exists
	
	Copies a list of TData values using the output iterator if the
	key item exists.
	
	Throws an exception if the key item exists but is not a list
	of TData items.															*/

template <class TData, class TOutputIterator>
bool
DataReader::ReadContainer(
	CFStringRef			inKey,
	TOutputIterator		outDestination) const
{
	typedef TDataVector<TData>	DataVectorT;

	DataObject*	data = ReadData(inKey);
	
	if (data == nil) {
		return false;
	}
	
	DataVectorT*	dataVector = dynamic_cast<DataVectorT*>(data);
	
	PPx_ThrowIfNil_(dataVector, DataError, err_DataFormat,
					"Value type in not a vector");
	
	std::copy( dataVector->mDataValues.begin(),
			   dataVector->mDataValues.end(),
			   outDestination );
			   
	return true;
}


// ---------------------------------------------------------------------------
//	ReadObjectValue
/**
	Template function for reading from the dictionary an item that is
	a pointer to a Persistent object
	
	TPersistent is a template parameter for the Persitent object type,
	which must be a pointer to a subclass of Persistent.
								
	@param	inKey			Key name of item
	@param	outObject		Pointer to persistent object
	
	@return Whether an item with the key name exists
	
	Sets outObject to nil if the key item does not exist.
	
	Throws an exception if the key item exists but is not a TPersistent
	object
*/

template <class TPersistent>
bool
DataReader::ReadObjectValue(
	CFStringRef		inKey,
	TPersistent&	outObject) const
{
	outObject = SafeDynamicCast<TPersistent>(ReadObject(inKey));

	return (outObject != nil);
}


// ---------------------------------------------------------------------------
//	ReadObjectContainer
/**
	Template function for reading from the dictionary an item that is
	a list of pointers to Persistent objects
	
	TPersistent is a template parameter for the Persitent object type,
	which must be a pointer to a subclass of Persistent.
	
	TOutputIterator is a template parameter for the iteration type

	@param	inKey			Key name of item
	@param	outDestination	Output iterator for storing object pointers
	
	@return Whether an item with the key name exists
	
	Copies a list of TPersistent pointers using the output iterator if
	the key item exists.
	
	Throws an exception if the key item exists but is not a list
	of TPersistent objects.													*/
	
template <class TPersistent, class TOutputIterator>
bool
DataReader::ReadObjectContainer(
	CFStringRef			inKey,
	TOutputIterator		outDestination) const
{
	typedef TDataVector<ObjectStorageIDT>	DataVectorT;
	typedef std::vector<ObjectStorageIDT>	ObjectIDList;

	DataObject*	data = ReadData(inKey);
	
	if (data == nil) {
		return false;
	}
	
	DataVectorT*	objectIDs = dynamic_cast<DataVectorT*>(data);
	
	PPx_ThrowIfNil_(objectIDs, DataError, err_DataFormat,
					"Value type is not an object ID vector");
	
	for ( ObjectIDList::const_iterator iter = objectIDs->mDataValues.begin(),
									   last = objectIDs->mDataValues.end();
		  iter != last; ++iter ) {
	
		*outDestination = SafeDynamicCast<TPersistent>
								(FindObjectByID(*iter));
								
		++outDestination;
	};
	
	return true;
}

#pragma mark -

// ===========================================================================
//	DataWriter
/**
	A data dictionary for writing state information. The items in the
	dictionary are (key name, data object) pairs.							*/

class	DataWriter {
public:
						DataWriter(
								KeyDataMap&			inKeyDataMap,
								ObjectIDMap&		inObjectsProcessed,
								ObjectQueue&		inObjectsToWrite);
								
	template <typename TData>
	void				WriteValue(
								CFStringRef		inKey,
								const TData&	inValue);
								
	template <typename TInputIterator>
	void				WriteContainer(
								CFStringRef		inKey,
								TInputIterator	inFirst,
								TInputIterator	inLast);
								
	void				WriteObjectValue(
								CFStringRef			inKey,
								const Persistent*	inObject);
								
	ObjectStorageIDT	WriteObject( const Persistent*	inObject );

	template <typename TInputIterator>			
	void				WriteObjectContainer(
								CFStringRef				inKey,
								TInputIterator			inFirst,
								TInputIterator			inLast);

private:								
	void				WriteData(
								CFStringRef		inKey,
								DataObject*		inData);

										// Forbid copy and assignment
						DataWriter( const DataWriter& );
	DataWriter&			operator = (const DataWriter& );								
													
private:
	KeyDataMap&			mKeyDataMap;
	ObjectIDMap&		mObjectsProcessed;
	ObjectQueue&		mObjectsToWrite;
};


// ---------------------------------------------------------------------------
//	WriteValue
/**
	Template function for writing a value in the dictionary.
	TData is a template parameter for the value type.
	
	@param	inKey		Key name of item
	@param	inValue		Value of item										*/
	
template <typename TData>
void
DataWriter::WriteValue(
	CFStringRef		inKey,
	const TData&	inValue)
{
	WriteData(inKey, new TDataObject<TData>(inValue));
}


// ---------------------------------------------------------------------------
//	WriteContainer
/**
	Template function for writing a list of values in the dictionary
	TInputIterator is a template parameter for the iterator type.
	
	@param	inKey			Key name of item
	@param	inFirst			Input iterator for first value to write
	@param	inLast			Input iterator for last value to write			*/

template <typename TInputIterator>
void
DataWriter::WriteContainer(
	CFStringRef		inKey,
	TInputIterator	inFirst,
	TInputIterator	inLast)
{
	typedef typename std::iterator_traits<TInputIterator>::value_type DataT;

	if (inFirst != inLast) {
		WriteData(inKey, new TDataVector<DataT>(inFirst, inLast));
	}
}


// ---------------------------------------------------------------------------
//	WriteObjectContainer
/**
	Template function for writing a list of pointers to Persistent objects
	TInputIterator is a template parameter for the iterator type.
	
	@param	inKey			Key name of item
	@param	inFirst			Input iterator for first value to write
	@param	inLast			Input iterator for last value to write			*/

template <typename TInputIterator>
void
DataWriter::WriteObjectContainer(
	CFStringRef				inKey,
	TInputIterator			inFirst,
	TInputIterator			inLast)
{
	typedef	TDataVector<ObjectStorageIDT>	ObjectIDList;
	
	if (inFirst != inLast) {			// Do nothing if range is empty
	
		std::auto_ptr<ObjectIDList>	objectIDs(new ObjectIDList);

		for (TInputIterator iter = inFirst; iter != inLast; ++iter) {
			objectIDs->mDataValues.push_back(WriteObject(*iter));
		}

		WriteData(inKey, objectIDs.release());
	}
}


} // namespace PPx

#endif // H_PPxSerializer
