// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RMIdentifier.h				   © 1997 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 02/02/97
//     $Date: 2006/01/18 01:33:25 $
//  $History: RMIdentifier.h $
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 02/03/97   Time: 10:04
//	Created in $/Constructor/Source files/CO- Core/Resource manager
//	Added class.
//	
// ===========================================================================

#pragma once


// ===========================================================================
//		* RMIdentifier
// ===========================================================================
//
//	RMIdentifier encapsulates the idea of the "ordinal or name" identifier
//	that is used throughout the Win32 resource file format. This class is
//	is built as an basic identifier that can be shared between MacOS
//	and Win32 resource file objects and only supports numeric IDs. The
//	class RWIdentifier, which derives from this class, adds the behaviors
//	for parsing the Unicode strings used by Windows.
//
//	This class is built with the assumption that an ID never changes once
//	it is created, so there are no "setters" in this class' interface.
//
// ===========================================================================

class RMIdentifier {

public:
							RMIdentifier(SInt32 inOrdinal = 0);
							RMIdentifier(const RMIdentifier& inOriginal);
	virtual					~RMIdentifier();

	// value accessors

	inline					operator SInt32() const
									{ return mOrdinal; }

	inline SInt32			GetNumericID() const
									{ return mOrdinal; }
	virtual void			GetStringID(
									LStr255& outStringID) const;
	virtual Boolean			IsNumericID() const;

	// comparisons
	
	virtual Boolean			IsMatchFor(SInt32 inNumericID) const;
	virtual Boolean			IsMatchFor(const LStr255& inStringID) const;
	virtual Boolean			IsMatchFor(const RMIdentifier& inIdentifier) const;


	// data members

protected:
	SInt32					mOrdinal;				// ordinal value of this ID
	
};
