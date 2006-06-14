// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxAttachable.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:20 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxAttachable.h>
#include <PPxAttachment.h>
#include <PPxSerializer.h>

namespace PPx {

namespace {

	// -----------------------------------------------------------------------
	//	Key names for persistent data

	const CFStringRef		key_Attachments = CFSTR("attachments");
}


// ---------------------------------------------------------------------------
//	Attachable														  [public]
/**
	Default constructor */

Attachable::Attachable()
{
	mAttachments = nil;
}


// ---------------------------------------------------------------------------
//	Attachable														  [public]
/**
	Copy constructor */

Attachable::Attachable(
	const Attachable&	/* inOriginal */)
{
	mAttachments = nil;						// Shallow copy
}


// ---------------------------------------------------------------------------
//	~Attachable														  [public]
/**
	Destructor */

Attachable::~Attachable()
{
	RemoveAllAttachments();
}


// ---------------------------------------------------------------------------
//	operator =														  [public]
/**
	Assignment operator */

Attachable&
Attachable::operator = (
	const Attachable&	inSource)
{
	if (this != &inSource) {
		RemoveAllAttachments();					// Shallow copy
	}
	
	return *this;
}


// ---------------------------------------------------------------------------
//	AddAttachment													  [public]
/**
	Adds an Attachment
	
	@param	inAttachment	Attachment object to add
	
	The Attachable takes ownership of the Attachment and is responsible for
	deleting it.															*/

void
Attachable::AddAttachment(
	Attachment*		inAttachment)
{
	if (mAttachments == nil) {
		mAttachments = new std::vector<Attachment*>;
	}
	
	mAttachments->push_back(inAttachment);
}


// ---------------------------------------------------------------------------
//	RemoveAttachment												  [public]
/**
	Removes an Attachment
	
	@param	inAttachment	Attachment object to remove
	
	Attachable gives up ownership of the Attachment, so the caller is
	resposible for deleting it.												*/

void
Attachable::RemoveAttachment(
	Attachment*		inAttachment)
{
	if (mAttachments != nil) {
		mAttachments->erase( remove( mAttachments->begin(),
									 mAttachments->end(),
									 inAttachment ),
					  		 mAttachments->end() );
	}
}


// ---------------------------------------------------------------------------
//	RemoveAllAttachments											  [public]
/**
	Removes and deletes all Attachments */

void
Attachable::RemoveAllAttachments()
{
		// Delete objects from back to front of the Attachments list.
		// Remove Attachment from list before deleting it.
		
	if (mAttachments != nil) {

		while (mAttachments->size() > 0) {
			Attachment*	theAttachment = mAttachments->back();
			mAttachments->pop_back();
			delete theAttachment;
		}
		
		delete mAttachments;
		mAttachments = nil;
	}
}


// ---------------------------------------------------------------------------
//	FindAttachmentByID												  [public]
/**
	Returns the Attachment with the specified object ID
	
	@param	inID	Object ID of Attachment to find
	
	@return	Pointer to Attachment object
	
	Return value is nil if there is no Attachment with the specified ID		*/

Attachment*
Attachable::FindAttachmentByID(
	ObjectIDT	inID) const
{
	Attachment*		theAttachment = nil;
	
	std::vector<Attachment*>::const_iterator	iter;
	
	iter = std::find_if( mAttachments->begin(), mAttachments->end(),
						 std::bind2nd( std::mem_fun(&Identifiable::HasID),
						 			   inID ) );
						 			   
	if (iter != mAttachments->end()) {
		theAttachment = *iter;
	}
	
	return theAttachment;
}


// ---------------------------------------------------------------------------
//	ReadAttachments													  [public]
/**
	Reads Attachment objects from a DataReader
	
	@param	inReader	DataReader from which to get Attahcments
	
	@note Attachable is not a subclass of Persistent. An Attachable subclass
	that also inherits from Persistent should call ReadAttahcments from
	its InitState function.												*/

void
Attachable::ReadAttachments(
	const DataReader&	inReader)
{
	if (inReader.ContainsKey(key_Attachments)) {

		if (mAttachments == nil) {
			mAttachments = new std::vector<Attachment*>;
		}
	
		inReader.ReadObjectContainer<Attachment*>(
										key_Attachments,
										std::back_inserter(*mAttachments) );
	}
}


// ---------------------------------------------------------------------------
//	WriteAttachments												  [public]
/**
	Writes Attachment objects to a DataWriter
	
	@param	ioWriter	DataWriter into which to put Attachments
	
	@note Attachable is not a subclass of Persistent. An Attachable subclass
	that also inherits from Persistent should call WriteAttachmens from
	its WriteState function.												*/

void
Attachable::WriteAttachments(
	DataWriter&	ioWriter) const
{
	if (mAttachments != nil) {
		ioWriter.WriteObjectContainer( key_Attachments,
									   mAttachments->begin(),
									   mAttachments->end() );
	}
}


} // namespace PPx