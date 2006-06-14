// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxAttachment.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:20 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxAttachment.h>
#include <PPxSerializer.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	Key name for persistent data

namespace {
	const CFStringRef	key_AttachmentID = CFSTR("attachment id");
}


// ---------------------------------------------------------------------------
//	InitState													   [protected]
/**
	Initializes state from a data dictionary
	
	@param	inReader	Data dictionary from which to read persistent data	*/

void
Attachment::InitState(
	const DataReader&	inReader)
{
	ObjectIDT	theID = objectID_None;
	inReader.ReadOptional(key_AttachmentID, theID);
	
	SetID(theID);
}


// ---------------------------------------------------------------------------
//	WriteState													   [protected]
/**
	Writes state to a data dictionary

	@param	ioWriter	Data dictionary to which to write persistent data	*/

void
Attachment::WriteState(
	DataWriter&	ioWriter) const
{
	ObjectIDT	theID = GetID();
	
	if (theID != objectID_None) {
		ioWriter.WriteValue(key_AttachmentID, theID);
	}
}


} // namespace PPx
