// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RETypeEntry.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 10/09/96
//	   $Date: 2006/01/18 01:33:06 $
//	$History: RETypeEntry.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 17:06
//	Updated in $/Constructor/Source files/CO- Core/Editors/00- Generic editor
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:28
//	Updated in $/Constructor/Source files/CO- Core/Editors/00- Generic editor
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/05/96   Time: 15:19
//	Updated in $/Constructor/Source files/Editors/+ Generic editor
//	Resource editor shell overhaul.
//
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:58
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class REEditor;
class RESession;


// ===========================================================================
//		* RETypeEntry
// ===========================================================================
//
//	RETypeEntry contains basic information about a single resource type.
//	It is used by the resource editor shell to control presentation of
//	resources of this type, and to spawn resource editors.
//
// ===========================================================================

class RETypeEntry {

public:
							RETypeEntry();
	virtual					~RETypeEntry();
	
	// resource type locator

	static RETypeEntry*		FindResTypeEntry(ResType inResType);

	// resource type accessors
	
	virtual Boolean			MatchResType(ResType inResType) const;
	
	inline ResType			GetPrimaryResType() const
									{ return mFirstPrimaryType; }
	inline SInt32			CountPrimaryResTypes() const
									{ return mPrimaryResTypeCount; }
	inline const LArray&	GetResTypes() const
									{ return mResTypes; }

	inline void				GetResTypeName(LStr255& outResTypeName) const
									{ outResTypeName = mResTypeName; }
	inline void				GetResFolderName(LStr255& outResFolderName) const
									{ outResFolderName = mResFolderName; }

	inline Handle			GetResTypeIconSuite() const
									{ return mResTypeIcon; }
	inline Handle			GetResFolderIconSuite() const
									{ return mResFolderIcon; }

	inline SInt32			GetSortSequence() const
									{ return mSortSequence; }
	inline Boolean			CreateEmptyFolder() const
									{ return mCreateEmptyFolder; }
	inline Boolean			UpdateResDataOnIDChange() const
									{ return mUpdateResDataOnIDChange; }
	inline ResIDT			GetDefaultNewResID() const
									{ return mDefaultNewResID; }

	// default resource accessors
	
	virtual SInt32			CountDefaultResources() const;
	virtual void			GetIndexedDefaultResName(
									SInt32				inResIndex,
									LStr255&			outResName) const;
	virtual Handle			GetIndexedDefaultResData(
									SInt32				inResIndex,
									ResType				inResType) const;

	// initialization

protected:
	virtual Boolean			InitializeFromRSCP(
									LStream&			inStream);


	// data members

protected:
	ResType					mFirstPrimaryType;			// first entry in primary res types
														//	(stored as optimization for MatchResType)
	LArray					mResTypes;					// all resource types associated with this type
	SInt32					mPrimaryResTypeCount;		// number of primary resource types
														//	(the first _n_ types in mResTypes are primary types)

	Str255					mResTypeName;				// resource type string
	Str255					mResFolderName;				// name for folder of resources

	Handle					mResTypeIcon;				// resource type icon
	Handle					mResFolderIcon;				// resource folder icon
	
	SInt32					mSortSequence;				// sort order for this resource type
	Boolean					mCreateEmptyFolder;			// true to create empty folder in project
														// 	for this resource type
	Boolean					mUpdateResDataOnIDChange;	// true to update resource data when
														//	a resource ID changes
	ResIDT					mDefaultNewResID;			// default ID for new resources of this type

	LArray					mDefaultResIDs;				// handles to default resources
									//! TEMPORARY: implementation of default resources is inefficient...

	friend class RETypeList;

};
