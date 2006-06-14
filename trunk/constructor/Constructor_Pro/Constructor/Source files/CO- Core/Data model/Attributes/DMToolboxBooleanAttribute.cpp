// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMToolboxBooleanAttribute.cpp
//								© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 06/13/96
//	   $Date: 2006/04/12 08:47:44 $
//	$History: DMToolboxBooleanAttribute.cpp $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/28/97   Time: 11:16
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Pilot #ifdef. Pilot likes 16-bit booleans, but uses 1 and 0 rather than
//	the Mac's 0x01FF.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:15
//	Updated in $/Constructor/Source files/CO- Core/Data model/Attributes
//	Improved commenting.
//
//	*****************  Version 3   *****************
//	User: Andrew       Date: 02/04/97   Time: 21:01
//	Checked in '$/ConstructorWin/Sources'
//	Comment: Initial merge with MSVC build
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 01/24/97   Time: 17:20
//	Checked in '$/Constructor/Source files/CO- Core/Data model/Attributes'
//	Comment: Fixed CR/LF problem
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:29
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "DMToolboxBooleanAttribute.h"

	// PowerPlant : File & stream
#include <LStream.h>


// ===========================================================================

#pragma mark *** DMToolboxBooleanAttribute ***

// ---------------------------------------------------------------------------
//		* DMToolboxBooleanAttribute()
// ---------------------------------------------------------------------------
//	Default constructor

DMToolboxBooleanAttribute::DMToolboxBooleanAttribute()
{
	mValueBytes = 1;
}


// ---------------------------------------------------------------------------
//		* DMToolboxBooleanAttribute(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. Stream data for DMToolboxBooleanAttribute is:
//
//		[DMIntegerAttribute data]
//		Int16:				Number of bytes to write (1 or 2)

DMToolboxBooleanAttribute::DMToolboxBooleanAttribute(
	LStream* inStream)

: DMIntegerAttribute(inStream)

{
	(*inStream)	>> mValueBytes;
	
	SignalIf_(mValueBytes < 1);
	SignalIf_(mValueBytes > 2);
	
}


// ---------------------------------------------------------------------------
//		* DMToolboxBooleanAttribute(DMToolboxBooleanAttribute&)
// ---------------------------------------------------------------------------
//	Copy constructor

DMToolboxBooleanAttribute::DMToolboxBooleanAttribute(
	const DMToolboxBooleanAttribute& inOriginal)

: DMIntegerAttribute(inOriginal)

{
	ValidateObject_(&inOriginal);
	mValueBytes = inOriginal.mValueBytes;
}


// ---------------------------------------------------------------------------
//		* ~DMToolboxBooleanAttribute
// ---------------------------------------------------------------------------
//	Destructor

DMToolboxBooleanAttribute::~DMToolboxBooleanAttribute()
{
	ValidateThis_();
}


// ===========================================================================

#pragma mark -
#pragma mark ** raw data streaming

// ---------------------------------------------------------------------------
//		* ReadStreamDataSelf
// ---------------------------------------------------------------------------
//	Read Boolean value and convert it to a one or zero value.

void
DMToolboxBooleanAttribute::ReadStreamDataSelf(
	LStream* inStream)
{

	ValidateThis_();

	// Figure out how many bytes to read for this value.

	SInt32 tempValue;
	switch (mValueBytes) {
		case 1:
			UInt8 value8;
			*inStream >> value8;
			tempValue = value8;
			break;
		case 2:
			UInt16 value16;
			*inStream >> value16;
			tempValue = value16;
			break;
		default:
			SignalCStr_("Bad value size.");
	}

	// Set the value. SetIntValue is a bottleneck.
	
	SetIntValue(tempValue != 0 ? 1 : 0);

}


// ---------------------------------------------------------------------------
//		* WriteStreamDataSelf
// ---------------------------------------------------------------------------
//	Write Boolean value as 255 (true) or 0 (false).

void
DMToolboxBooleanAttribute::WriteStreamDataSelf(
	LStream* inStream) const
{

	ValidateThis_();

	// Figure out how many bytes will be read for this value.

	switch (mValueBytes) {
		case 1:
			UInt8 value8;
			value8 = mValue != 0 ? 0xFF : 0;
			*inStream << value8;
			break;
		case 2:
			UInt16 value16;
#if Constructor_ForPilot
			value16 = mValue != 0 ? 1 : 0;
#else
			value16 = mValue != 0 ? 0x01FF : 0;
#endif
			*inStream << value16;
			break;
		default:
			SignalCStr_("Bad value size.");
	}
}
