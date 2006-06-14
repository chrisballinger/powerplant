// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	UCustomTextTraits.h			© 2001 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	This file is designed to allow for CodeWarrior preference panel developers
//	to build their UI using specific text traits without actually storing the
//	text trait resources in their resource file.  The idea behind this is that
//	the panels will use the IDE's text traits.
//
//	To use this feature, create a folder named "Text Traits" and place it next
//	to the Constructor application.  Any items inside this folder will be
//	scanned for 'Txtr' resources which Constructor will use for displaying in
//	the views.

#pragma once

	// PowerPlant : PP headers
#include <PP_Prefix.h>




// ===========================================================================
//		* SCustomTextTraitsResInfo
// ===========================================================================
//	Based on PIPopupLabel.cpp's SResInfo

struct SCustomTextTraitsResInfo {
	ResIDT			mResID;
	Str255			mResName;
};


typedef	pascal	void (*TxtrFilterProcPtr) (const SInt16	inRefNum,
											  Boolean*	outQuitFlag,
											  void*		ioUserData);

#define CallTxtrFilterProc(userRoutine, inRefNum, outQuitFlag, ioUserData) \
		(*(userRoutine))((inRefNum), (outQuitFlag), (ioUserData))



class UCustomTextTraits
{

public:
	static TextTraitsH		FindTextTraitsByID(
									ResIDT		inTextTraitsID);

	static void				FindCustomTextTraits(
									LArray&		ioResInfo);

protected:
	static OSErr			FindTextTraitsFolder(
									SInt16*		outFoundVRefNum,
									SInt32*		outFoundDirID);

	static void				ScanFilesInFolder(
									SInt16				inVRefNum,
									SInt32				inDirID,
									TxtrFilterProcPtr	inTxtrFilter,
									void*				ioUserData);

};

	pascal  void 		FindTextTraitsByIDFilter(
									const SInt16	inRefNum,
									Boolean*		outQuitFlag,
									void*			ioUserData);

	pascal  void 		FindAllTextTraitsFilter(
									const SInt16	inRefNum,
									Boolean*		outQuitFlag,
									void*			ioUserData);
