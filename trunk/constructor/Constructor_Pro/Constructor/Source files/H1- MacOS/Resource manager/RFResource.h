// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RFResource.h				   © 1997 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 01/23/97
//     $Date: 2006/01/18 01:34:07 $
//  $History: RFResource.h $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/19/97   Time: 20:08
//	Updated in $/Constructor/Source files/H1- MacOS/Resource manager
//	Improved commenting.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:51
//	Updated in $/Constructor/Source files/H1- MacOS/Resource manager
//	Finished adaptation to refactored RM/RF classes.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/31/97   Time: 15:20
//	Updated in $/Constructor/Source files/H1- MacOS/Resource manager
//	Upgraded to John Cortell's revisions.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/27/97   Time: 14:00
//	Updated in $/Constructor/Source files/H1- MacOS/Resource manager
//	Removed the multi-file capability. It was never used anyway.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 01/24/97   Time: 15:03
//	Created in $/Constructor/Source files/H1- MacOS/Resource manager
//	Refactored the RF* classes.
//	
// ===========================================================================

#pragma once

	// Core : Resource manager
#include "RMResource.h"


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class RFMap;
class RFType;


// ===========================================================================
//		* RFResource
// ===========================================================================
//
//	RFResource is the object representation of a single resource
//	in a resource file (or map). It provides accessors for resource
//	data, attributes, and ID.
//
//	Note that you cannot change the resource ID or type of an RMResource.
//	Rather you must make a copy of the resource and delete the original.
//	The DuplicateResource method is provided as a helper for this purpose.
//
// ===========================================================================

class RFResource : public RMResource {

protected:
							RFResource(
									RFType&				inResType,
									ResIDT				inResID);
							RFResource(
									RFType&				inResType,
									Handle				inResourceH);
							~RFResource();

	// resource attribute accessors

public:
	virtual Boolean			IsMatchFor(
									const RMIdentifier&	inResType,
									const RMIdentifier&	inResID) const;
	virtual Boolean			IsMatchFor(
									ResType				inResType,
									ResIDT				inResID) const;

	virtual RMIdentifier*	GetResTypeRM() const;
	virtual RMIdentifier*	GetResIDRM() const;

	inline SInt16			GetResAttrs() const
									{ return mResAttrs; }
	inline ResIDT			GetResID() const
									{ return mResID; }
	ResType					GetResType() const;

	virtual StringPtr		GetResName(Str255 outResName) const;
	virtual Handle			GetResData();
	
	RFMap&					GetRFMap() const;
	inline RFType&			GetRFType() const
									{ return mResType; }

	virtual Boolean			IsModified() const;

	// resource info changers

	virtual void			SetResData(Handle inResData);
	void					SetResAttrs(SInt16 inResAttrs);
	void					SetResName(ConstStringPtr inResName);

	RFResource*				DuplicateResource(
									ResType				inResType,
									ResIDT				inDuplicateResID);

	virtual void			DeleteResource();

	// resource file updating

protected:
	virtual void			UpdateComplete();
	virtual void			UpdateRF();

	// internal implementation details

private:
	void					InitRFResource(
									ResIDT				inResID);
	void					SetResNameInternal(
									ConstStringPtr		inResName);

	SInt16					GetResourceContext();
	void					CacheResName();


	// data members

protected:
	RFType&					mResType;					// resource type entry
	Handle					mResData;					// cached resource data (only if modified)
	ResIDT					mResID;						// resource ID
	SInt16					mResAttrs;					// resource attributes
	StringHandle			mResName;					// resource name
	Boolean					mResAttrsChanged;			// true if resource attributes have changed

	friend class RFType;

};
