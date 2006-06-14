// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxSerializer.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:21 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxSerializer.h>
#include <PPxLocalizedString.h>
#include <PPxPersistent.h>
#include <PPxPrimaryBundle.h>
#include <PPxRegistrar.h>

namespace PPx {


// ---------------------------------------------------------------------------
//	DescriptorsToObjects
/**
	Creates Persistent objects from a list of object descriptors
	
	@param	ioDescriptors	List of object descriptors
	
	@return The first Persistent object created from the list
	
	An object descriptor contains the class name and a data dictionary
	of state information for a persistent object. This function
	iterates over a list of object descriptors, creating and initializing
	a Persistent object based on each descriptor.
	
	The process has three steps: (1) contruction via new and the default
	constructor; (2) call to InitPersistent with the data dictionary of
	state information; (3) call to FinishInitPersistent.
	
	The first object should be the root of an object hierarchy. The return
	value is a std::auto_ptr containing a pointer to the first object,
	meaning the caller receives ownership of it.
	
	This reverses the processes of the ObjectsToDescriptors function.
*/

std::auto_ptr<Persistent>
Serializer::DescriptorsToObjects(
	ObjectDescriptorList&	ioDescriptors)
{
	if (ioDescriptors.empty()) {
		return std::auto_ptr<Persistent>(nil);
	}

	CFString	className;
	ObjectDescriptorList::iterator	iter;
	ObjectDescriptorList::iterator	last = ioDescriptors.end();
	
		// Iterate over all descriptors and create each Persistent
		// object based on its class name string
	
	for (iter = ioDescriptors.begin(); iter != last; ++iter) {
		  
		DataReader	reader(*(iter->objectState), ioDescriptors);
		
		iter->objectPtr = Registrar::CreateObject(iter->className);
	}
	
		// Now that all objects have been created, iterate a second
		// time to let each Persistent object read its state data
	
	for (iter = ioDescriptors.begin(); iter != last; ++iter) {
		  
		DataReader	reader(*(iter->objectState), ioDescriptors);
		iter->objectPtr->InitPersistent(reader);
	}
	
		// Finally, iterate a third time to let each Persistent object
		// finish initializing
	
	for (iter = ioDescriptors.begin(); iter != last; ++iter) {
		  
		DataReader	reader(*(iter->objectState), ioDescriptors);
		iter->objectPtr->FinishInitPersistent();
	}
	
		// Return value is a pointer to the first Persistent object
		
	return std::auto_ptr<Persistent>(ioDescriptors[0].objectPtr);
}


// ---------------------------------------------------------------------------
//	ObjectsToDescriptors
/**
	Stores Persistent objects as a list of object descriptors
	
	@param	inRootObject	Persistent object to store
	@param	outDescriptors	List of object descriptors
	
	This function puts the Persistent object passes as a parameter into
	a first in, first out deque. It then begins a loop were it removes
	the first object from the deque, creates an object descriptor with
	a data dictionary, adds the descriptor to a list, then tells the
	object to write its state to the data dictionary.
	
	In the course of writing its state, an object may request that
	other Persistent objects write their state. Such objects are added
	to the deque. The loop continues until the deque is empty. In this
	way, an entire object hierarchy is flattened into a list of object
	descriptors.
*/

void
Serializer::ObjectsToDescriptors(
	const Persistent*		inRootObject,
	ObjectDescriptorList&	outDescriptors)
{
		// Each object writes its state using (key, data) pairs, and is
		// numbered sequentially in the order written. For data which is a
		// pointer to another persistent object, we write the object's
		// storage ID number.
		//
		// Once we write an object, we add it to a map of objects already
		// processed. The "key" for the map is the object pointer, and the
		// "value" is the storage ID. Given an object pointer, the DataWriter
		// can quickly lookup whether the object has already been processed.
		//
		// When requested to write an object not already written, we add a
		// pointer to the object to a queue of objects to write.

	ObjectIDMap		objectsProcessed;
	ObjectQueue		objectsToWrite;
	
	ObjectStorageIDT	storageID = 1;	// Number objects sequentially
										//   First object written has ID 1
										
		// Add the root object to the queue of objects to write and
		// the map of objects processed, then loop over the queue until
		// it is empty. On a given iteration, the queue will grow if
		// the current object stores pointers to other persistent objects.
		// The queue is empty when we have written all objects in the
		// dependency graph.

	objectsToWrite.push_back(inRootObject);
	objectsProcessed.insert(std::make_pair(inRootObject, storageID));
										
	while (not objectsToWrite.empty()) {
										// Get object from front of queue
		const Persistent*	object = objectsToWrite.front();
		objectsToWrite.pop_front();
		
										// Fill in descriptor record for
										//   the object
		ObjectDescriptor	objDesc;
		objDesc.objectPtr	= const_cast<Persistent*>(object);
		objDesc.storageID	= storageID++;
		objDesc.className.AttachRef(object->GetClassName(), retain_Yes);
		objDesc.objectState.Reset( new KeyDataMap );
			
										// Add descriptor to our list
		outDescriptors.push_back(objDesc);

			// Write data for the object into the data map. Pointers to
			// other persistent objects referenced by the object will be
			// added to the "objects to write" queue.
			
		DataWriter	writer( *objDesc.objectState,
							objectsProcessed,
							objectsToWrite );
							
		object->WritePersistent(writer);
	}
}

#pragma mark -

// ---------------------------------------------------------------------------
//	DataReader														  [public]
/**
	Constructs from a data dictionary and list of object descriptors
	
	@param	inKeyDataMap	Dictionary of (key, data object) pairs
	@param	inDescriptors	List of object descriptors						*/

DataReader::DataReader(
	KeyDataMap&				inKeyDataMap,
	ObjectDescriptorList&	inDescriptors)
	
	: mKeyDataMap(inKeyDataMap),
	  mObjectDescriptors(inDescriptors)
{
}


// ---------------------------------------------------------------------------
//	ContainsKey														  [public]
/**
	Returns whether a data object with a certain key is in the DataReader
	
	@param	inKey	Key name of item
	
	@return Whether an item wih the key name exists							*/

bool
DataReader::ContainsKey(
	CFStringRef	inKey) const
{
	return (mKeyDataMap.find(inKey) != mKeyDataMap.end());
}


// ---------------------------------------------------------------------------
//	ReadOptional
/**
	Reads an optional string item from the dictionary
	
	@param	inKey		Key name of item
	@param	outString	String item
	
	This overload handles strings which may be localized. For a localized
	string, the string in the dictionary is a key in a lookup table of
	translated strings.
*/

bool
DataReader::ReadOptional(
	CFStringRef		inKey,
	CFString&		outString) const
{
	DataObject*	data = ReadData(inKey);
	
	if (data == nil) {
		return false;
	}
	
										// Read as a regular string
	typedef	TDataObject<CFString>	StringDataT;
	
	StringDataT*	stringData = dynamic_cast<StringDataT*>(data);
	
	if (stringData != nil) {
		outString = stringData->mValue;
		return true;
	}
	
										// Item is not a regular string.
										// Try reading as a localized string.
	typedef	TDataObject<LocalizedString>	LocalDataT;
		
	LocalDataT*	localData = dynamic_cast<LocalDataT*>(data);
		
	PPx_ThrowIfNil_(localData, DataError, err_DataFormat, "Value type is not a string");
	
	outString = localData->mValue.GetLocalizedString();
	
	return true;
}


// ---------------------------------------------------------------------------
//	ReadData														 [private]
/**
	Returns data object for the specified key name
	
	@param	inKey	Key name of item
	
	@return	Pointer to DataObject for the key item							*/

DataObject*
DataReader::ReadData(
	CFStringRef	inKey) const
{
	KeyDataMap::const_iterator	iter = mKeyDataMap.find(inKey);
	
	return (iter == mKeyDataMap.end()) ? nil : iter->second.Get();
}


// ---------------------------------------------------------------------------
//	ReadObject														 [private]
/**
	Returns the Persistent object for the specified key name
	
	@param	inKey	Key name of item
	
	@return	Pointer to Persistent object for the key item

	Throws and exception if the key item is not a Persistent object			*/

Persistent*
DataReader::ReadObject(
	CFStringRef	inKey) const
{
	DataObject*	data = ReadData(inKey);
	
	if (data == nil) {					// No data with specified key
		return nil;
	}
	
	TDataObject<ObjectStorageIDT>*	persistData =
		dynamic_cast<TDataObject<ObjectStorageIDT>*>(data);
	
	if (persistData == nil) {			// Data value is not an object
		PPx_Throw_(DataError, err_DataFormat, "Attempt to read non-object value as an object");
	}
	
	return FindObjectByID(persistData->mValue);
}


namespace { // unnamed

	// -----------------------------------------------------------------------
	//	Search criterion for finding object by storage ID number
	
	bool	HasStorageID(
					const ObjectDescriptor&	inObjDesc,
					ObjectStorageIDT		inStorageID)
				{
					return inStorageID == inObjDesc.storageID;
				}
				
} // unnamed namespace


// ---------------------------------------------------------------------------
//	FindObjectByID													 [private]
/**
	Returns the Persistent object with the specified storage ID
	
	@param	inStorageID		Storage ID for the object to find
	
	@return	Pointer to Persistent object									*/

Persistent*
DataReader::FindObjectByID(
	ObjectStorageIDT	inStorageID) const
{
	if (inStorageID == 0) {				// Zero ID means nil object
		return nil;
	}
	
		// In most cases, storage ID numbers are sequential starting
		// at one. So, we check if the object descriptor at index
		// of storageID - 1 has the target storage ID.
	
	if (inStorageID <= mObjectDescriptors.size()) {
		ObjectDescriptor	objDesc = mObjectDescriptors[inStorageID - 1];
		if (inStorageID == objDesc.storageID) {
			return objDesc.objectPtr;
		}
	}
	
		// Storage ID numbers aren't sequential. We fall back on
		// a linear search for the target storage ID.
		
#if __MWERKS__
	ObjectDescriptorList::const_iterator	iter =
		std::find_if( mObjectDescriptors.begin(), mObjectDescriptors.end(),
					  std::bind2nd(std::ptr_fun(HasStorageID), inStorageID) );
#else
	// ### GCC doesn't like the code above and I haven't figured out why
	ObjectDescriptorList::const_iterator	iter;
	for (iter = mObjectDescriptors.begin(); iter != mObjectDescriptors.end(); iter++) {
		if (HasStorageID(*iter, inStorageID)) {
			break;
		}
	}
#endif
					  
	if (iter != mObjectDescriptors.end()) {
		return iter->objectPtr;
	}
	
	PPx_Throw_(DataError, err_DataFormat, "Undefined object storage ID");
	
	return nil;
}

#pragma mark -

// ---------------------------------------------------------------------------
//	DataWriter														  [public]
/**
	Constructor
	
	@param	inKeyDataMap		Data dictionary of (key, data object) pairs
	@param	inObjectsProcessed	Map of objects already processed
	@param	inObjectsToWrite	Queue of objects still to write				*/

DataWriter::DataWriter(
	KeyDataMap&		inKeyDataMap,
	ObjectIDMap&	inObjectsProcessed,
	ObjectQueue&	inObjectsToWrite)
	
	: mKeyDataMap(inKeyDataMap),
	  mObjectsProcessed(inObjectsProcessed),
	  mObjectsToWrite(inObjectsToWrite)
{
}


// ---------------------------------------------------------------------------
//	WriteObjectValue												  [public]
/**
	Writes a Persistent object to the data dictionary
	
	@param	inKey		Key name for item
	@param	inObject	Pointer to a Persistent object						*/

void
DataWriter::WriteObjectValue(
	CFStringRef			inKey,
	const Persistent*	inObject)
{
	if (inObject != nil) {
		ObjectStorageIDT	storageID = WriteObject(inObject);
		WriteData(inKey, new TDataObject<ObjectStorageIDT>(storageID));
	}
}


// ---------------------------------------------------------------------------
//	WriteObject							 							  [public]
/**
	Queues a Persistent object for writing
	
	@param	inObject	Pointer to a Persistent object
	
	@return	Storage ID for the object
	
	Unlike WriteObjectValue, this function does not write a key item
	for the object in the data dictionary. Call this function when you
	want to write an object, but do not later need to get back a pointer
	to the object when restoring state.
	
	For exmaple, if two objects P and Q have pointers to each other, you
	could have P call WriteObject for Q, and P call WriteObjectValue
	for Q. When reading, P would do nothing about Q, and Q would call
	ReadObjectValue for P and then make some other calls that would
	associate Q with P.
*/

ObjectStorageIDT
DataWriter::WriteObject(
	const Persistent*	inObject)
{
	if (inObject == nil) {		// We do not write nil objects
		return 0;
	}
								// Check if we've already processed the object
	ObjectIDMap::const_iterator	iter = mObjectsProcessed.find(inObject);
	
	if (iter != mObjectsProcessed.end()) {
								// Object is already processed. Its storage
								//   ID is the "value" of the pair item
		return iter->second;
	}

								// Put object in write queue
	mObjectsToWrite.push_back(inObject);
	
								// Storage IDs are sequential, so ID for
								//   this object is one more than the
								//   number of objects already processed
	ObjectStorageIDT	storageID = mObjectsProcessed.size() + 1;
	
								// Put object in map of those already
								//   processed so that we don't write
								//   the same object multiple times
	mObjectsProcessed.insert(std::make_pair(inObject, storageID));
	
	return storageID;								
}


// ---------------------------------------------------------------------------
//	WriteData														 [private]
/**
	Writes a (key, data object) pair to the data dictionary
	
	@param	inKey		Key name of item
	@param	inData		Pointer to DataObject								*/

void
DataWriter::WriteData(
	CFStringRef		inKey,
	DataObject*		inData)
{
	AutoRetained<DataObject>	data(inData);
	mKeyDataMap.insert(std::make_pair(CFString(inKey), data));
}


} // namespace PPx
