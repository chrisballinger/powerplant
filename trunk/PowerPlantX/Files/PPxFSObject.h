// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxFSObject.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:19 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxFSObject.h
	@brief		Wrapper for FSRef and related File Manager and MoreFiles X
				functions
*/

#ifndef H_PPxFSObject
#define H_PPxFSObject
#pragma once

#include <PPxPrefix.h>
#include <SysCFString.h>

#include "MoreFilesX.h"

namespace PPx {

	class CFURL;
	class Folder;


// ===========================================================================
//	FSObject
/**
	Wrapper for a system file reference (FSRef) and related File Manager
	and MoreFiles X functions.
	
	FSObject supports describing an entity that does not yet exist. FSRef
	does not have this feature, but FSSpec, the former standard OS file
	description, does have this feature.
	
	Entities that do not exist are described by their name and parent folder.
	The File and Folder subclasses of FSObject have CreateOnDisk() functions
	to create an entity. FSObject throws an exception if you attempt to
	perform a file system operation on an entity that does not exist.		*/

class	FSObject {
public:
									// Constructors
						FSObject();

	explicit			FSObject( const FSRef& inFSRef );

						FSObject(
								const FSRef&		inParentRef,
								const CFString&		inName);

						FSObject(
								const FSRef&		inParentRef,
								const HFSUniStr255&	inName);

						FSObject(
								FSVolumeRefNum		inVRefNum,
								SInt32				inParentDirID,
								const CFString&		inName);

						FSObject(
								FSVolumeRefNum		inVRefNum,
								SInt32				inParentDirID,
								const HFSUniStr255&	inName);

						FSObject(
								const FSSpec&		inFSSpec,
								CFStringEncoding	inNameEncoding = encoding_System);

	explicit			FSObject( CFURLRef inURL );

						FSObject(
								const CFString&		inAbsolutePath,
								CFURLPathStyle		inPathStyle = kCFURLPOSIXPathStyle);

						FSObject(
								const CFString&		inRelativePath,
								const FSRef&		inBaseDir,
								CFURLPathStyle		inPathStyle = kCFURLPOSIXPathStyle);

						FSObject( const FSObject& inOriginal );

	virtual				~FSObject();

									// Assignment operators
	FSObject&			operator = ( const FSObject& inSource );

	FSObject&			operator = ( const FSRef& inFSRef );

									// State accessors
	const FSRef&		UseRef() const;

	bool				IsValid() const;

	bool				Exists() const;

									// Comparison
	bool				IsEqualTo( const FSObject& inOther ) const;

	bool				IsEqualTo( const FSRef& inFSRef ) const;

	OSStatus			CompareTo( const FSObject& inOther ) const;

	OSStatus			CompareTo( const FSRef& inFSRef ) const;

									// Getting file system information
	void				GetName( HFSUniStr255& outName ) const;

	CFString			GetName() const;

	CFString			GetPath( CFURLPathStyle inPathStyle = kCFURLPOSIXPathStyle ) const;

	FSVolumeRefNum		GetVolume() const;

	void				GetParent( FSRef& outParentRef ) const;

	void				GetParent( FSObject &outParent ) const;

	SInt32				GetParentDirID() const;

	void				GetFSSpec(
								FSSpec&				outSpec,
								CFStringEncoding	inNameEncoding = encoding_System) const;

	CFURL				GetURL() const;

	void				GetCatalogInfo(
								FSCatalogInfoBitmap	inWhichInfo,
								FSCatalogInfo&		outCatInfo) const;

	void				SetCatalogInfo(
								FSCatalogInfoBitmap		inWhichInfo,
								const FSCatalogInfo&	inCatInfo);

	bool				IsFile() const;

	bool				IsFolder() const;

	OSStatus			CheckLock() const;

	void				SetIsLocked( bool inLock );
	
									// Getting/Setting Finder information
	void				GetFinderInfo(
								FinderInfo*			outFinderInfo,
								ExtendedFinderInfo*	outExtFinderInfo = nil,
								bool*				outIsFolder = nil) const;

	void				SetFinderInfo(
								const FinderInfo*			inFinderInfo,
								const ExtendedFinderInfo*	inExtFinderInfo = nil);

	UInt16				GetFinderFlags() const;

	void				ChangeFinderFlags(
								bool		inSetFlags,
								UInt32		inFlagsToChange);

									// File system operations
	void				Rename(	const HFSUniStr255&	inName,
								TextEncoding 		inEncodingHint = kTextEncodingUnknown);

	void				Rename(	const CFString&	inName,
								TextEncoding inEncodingHint = kTextEncodingUnknown);

	void				Delete();

	void				DeleteContainer();

	void				DeleteContainerContents();

									// State change operations
	void				Update();

	void				Invalidate();

private:
	FSRef&				GetRef();
	
	void				InitFromVolDirName(
								FSVolumeRefNum			inVRefNum,
								SInt32					inParentDirID,
								const HFSUniStr255&		inName);
	
	void				InitFromURL( CFURLRef inURLRef );

private:
	enum EState {
		ref_Invalid,
		ref_Exists,
		ref_ParentAndName
	};

	FSRef				mFSRef;		/**< System file reference */
	CFString			mName;		/**< Name of file */
	EState				mState;		/**< State of file reference */
};


// -----------------------------------------------------------------------
//	IsValid
/**
	Returns whether the FSObject refers to a valid file system item
	
	@return Whether the FSObject refers to a valid file system item		*/

inline bool
FSObject::IsValid() const
{
	return (mState != ref_Invalid);
}


// -----------------------------------------------------------------------
//	Exists
/**
	Returns whether the FSObject refers to an existing file system item
	
	@return Whether the FSObject refers to an existing file system item	*/

inline bool
FSObject::Exists() const
{
	return (mState == ref_Exists);
}


// -----------------------------------------------------------------------
//	IsEqualTo
/**
	Returns whether the FSObject is equal to another FSObject
	
	@param	inOther		FSObject to which to compare
	
	@return Whether the FSObject is equal to the other FSObject			*/

inline bool
FSObject::IsEqualTo(const FSObject &inOther) const
{
	return (CompareTo(inOther) == noErr);
}


// -----------------------------------------------------------------------
//	IsEqualTo
/**
	Returns whether the FSObject is equal to a FSRef
	
	@param	inFSRef		FSRef to which to compare
	
	@return Whether the FSObject refers to the same item as the FSRef	*/

inline bool
FSObject::IsEqualTo(const FSRef &inFSRef) const
{
	return (CompareTo(inFSRef) == noErr);
}


} // namespace PPx

#endif // H_PPxFSObject
