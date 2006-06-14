// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RMResource.h				© 1995-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 10/09/95
//     $Date: 2006/01/18 01:33:25 $
//	$History: RMResource.h $
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:46
//	Updated in $/Constructor/Source files/CO- Core/Resource manager
//	Finished adaptation to refactored RM/RF classes.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/31/97   Time: 15:24
//	Updated in $/Constructor/Source files/CO- Core/Resource manager
//	Upgraded to John Cortell's revisions.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/27/97   Time: 14:01
//	Updated in $/Constructor/Source files/CO- Core/Resource manager
//	Removed the multi-file capability. It was never used anyway.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 15:08
//	Updated in $/Constructor/Source files/CO- Core/Resource manager
//	Refactored the RF* classes.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/23/97   Time: 14:06
//	Updated in $/Constructor/Source files/CO- Core/Resource manager
//	Cleaned up source in preparation for refactoring.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 02:09
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class RMIdentifier;
class RMMap;
class RMType;


// ===========================================================================
//		* RMResource											[abstract]
// ===========================================================================
//
//	RMResource is the object representation of a single resource
//	in a resource file (or map). It is not platform-specific, and so 
//	much of the detail for getting at resource identity, etc., is contained
//	in the concrete subclasses of RMResource.
//
// ===========================================================================

class RMResource {

protected:
							RMResource(RMType& inResType);
	virtual					~RMResource();

	// resource attribute accessors

public:
	virtual Boolean			IsMatchFor(
									const RMIdentifier&	inResType,
									const RMIdentifier&	inResID) const = 0;

	virtual RMIdentifier*	GetResTypeRM() const = 0;
	virtual RMIdentifier*	GetResIDRM() const = 0;

	virtual StringPtr		GetResName(Str255 outResName) const = 0;
	virtual Handle			GetResData() = 0;

	virtual Boolean			IsModified() const;
	inline Boolean			IsDeleted() const
									{ return mResDeleted; }

	// resource map accessors

	inline RMType&			GetRMType() const
									{ return mResType; }
	RMMap&					GetRMMap() const;

	// resource info changers

	virtual void			SetResName(ConstStringPtr inResName) = 0;
	virtual void			SetResData(Handle inResData) = 0;

	virtual void			DeleteResource();

	// internal implementation details

protected:
	virtual void			SendRsrcMessage(MessageT inMessage);
	
	virtual void			UpdateRF();
	virtual void			UpdateComplete();
	

	// data members

protected:
	RMType&					mResType;				// the resource type entry
	Boolean					mResDeleted;			// true if resource is deleted

	friend class RMType;

};
