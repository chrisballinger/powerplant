// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMValueLabel.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 06/13/96
//	   $Date: 2006/01/18 01:32:33 $
//	$History: DMValueLabel.cpp $
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

#include "DMValueLabel.h"

	// PowerPlant : File & stream
#include <LStream.h>


// ===========================================================================

#pragma mark *** DMValueLabel ***

// ---------------------------------------------------------------------------
//		* DMValueLabel()
// ---------------------------------------------------------------------------
//	Constructor

DMValueLabel::DMValueLabel()
{
	mValue = 0;
}


// ---------------------------------------------------------------------------
//		* DMValueLabel(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. Stream data for DMToolboxBooleanAttribute is:
//
//		[DMIntegerAttribute data]
//		Int32:				Value to be labelled
//		Pascal string:		Label for this value

DMValueLabel::DMValueLabel(
	LStream* inStream)

{
	(*inStream)	>> mValue
				>> (StringPtr) mLabel;
}


// ---------------------------------------------------------------------------
//		* DMValueLabel(DMValueLabel&)
// ---------------------------------------------------------------------------
//	Copy constructor

DMValueLabel::DMValueLabel(
	const DMValueLabel& inOriginal)
{
	ValidateObject_(&inOriginal);

	mValue = inOriginal.mValue;
	mLabel = inOriginal.mLabel;
}


// ---------------------------------------------------------------------------
//		* ~DMValueLabel
// ---------------------------------------------------------------------------
//	Destructor

DMValueLabel::~DMValueLabel()
{
	ValidateThis_();
}


// ===========================================================================

#pragma mark -
#pragma mark ** accessors

// ---------------------------------------------------------------------------
//		* MatchLabel
// ---------------------------------------------------------------------------
//	Return true if the input string matches the label string.
//	The comparison is not case sensitive.

Boolean
DMValueLabel::MatchLabel(
	ConstStringPtr inLabel) const
{
	ValidateThis_();
	return mLabel.CompareTo(inLabel) == 0;
}
