// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	RETypeList.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 10/09/96
//	   $Date: 2006/01/18 01:33:07 $
//	$History: RETypeList.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 17:06
//	Updated in $/Constructor/Source files/CO- Core/Editors/00- Generic editor
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:29
//	Updated in $/Constructor/Source files/CO- Core/Editors/00- Generic editor
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/05/96   Time: 15:21
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

class RETypeEntry;


// ===========================================================================
//		* RETypeList
// ===========================================================================
//
//	RETypeList contains the list of all known resource types. It is used
//	by the resource editor shell to get information about each of the
//	resource types and to create appropriate editors for each resource
//	type.
//
//	This object is intended to serve as a singleton.
//
// ===========================================================================

class RETypeList {

protected:
							RETypeList();
	virtual					~RETypeList();

	// singleton accessor
	
public:
	static RETypeList*		GetTypeList();
	static void				ReleaseTypeList();
	
	// type entry lookup
	
	virtual RETypeEntry*	FindResTypeEntry(ResType inResType) const;
	inline const LArray&	GetResTypeEntries() const
									{ return mResTypeEntries; }

	// type specification builders

protected:	
	virtual void			ScanAtLaunch();

	virtual void			ScanCurrentResourceFile(
									const FSSpec&	inSourceFile);

	virtual void			MakeSpecFromRSCP(
									LStream&		inStream,
									const FSSpec&	inSourceFile);


	// data members

protected:
	LArray					mResTypeEntries;		// resource type entries (sorted by sort sequence)


	// class variables

protected:
	static RETypeList*		sTypeList;				// pointer to singleton class list
	
};


// ===========================================================================
//		* RETypeSortComparator
// ===========================================================================
//
//	RETypeSortComparator is used by RETypeList to keep the RETypeEntry
//	objects sorted by their sort sequence.
//
// ===========================================================================

class RETypeSortComparator : public LComparator {

public:
							RETypeSortComparator() {}
	virtual					~RETypeSortComparator() {}

	virtual SInt32			Compare(
									const void*			inItemOne,
									const void* 		inItemTwo,
									UInt32				inSizeOne,
									UInt32				inSizeTwo) const;
	virtual Boolean			IsEqualTo(
									const void*			inItemOne,
									const void* 		inItemTwo,
									UInt32				inSizeOne,
									UInt32				inSizeTwo) const;

};

