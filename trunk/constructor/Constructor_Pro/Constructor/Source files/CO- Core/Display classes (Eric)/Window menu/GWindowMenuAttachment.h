// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	GWindowMenuAttachment.h		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 08/14/96
//     $Date: 2006/01/18 01:32:47 $
//	$History: GWindowMenuAttachment.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:58
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Window menu
//	Improved commenting.
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 01/24/97   Time: 17:25
//	Checked in '$/Constructor/Source files/CO- Core/Display classes (Eric)/Window menu'
//	Comment: Fixed CR/LF problem
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:45
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// Core : Display classes (Eric) : Window menu
#include "LWindowMenuAttachment.h"


// ===========================================================================
//		* GWindowMenuAttachment
// ===========================================================================
//
//	GWindowMenuAttachment is a special version of LWindowMenuAttachment
//	which indents the window titles in the Window menu so that they
//	represent the data model hierarchy. Intended for use only in Constructor.
//
// ===========================================================================

class GWindowMenuAttachment : public LWindowMenuAttachment {

public:
	enum { class_ID = 'Gwmn' };

	static void*			CreateFromStream(LStream*	inStream)
									{ return new GWindowMenuAttachment(inStream); }

							GWindowMenuAttachment();
							GWindowMenuAttachment(
									LStream*			inStream);
	virtual					~GWindowMenuAttachment();

	// menu configuration

protected:
	virtual void			FindCommandStatus(
									SCommandStatus*		inCommandStatus);
	virtual void			InstallMenu();

	virtual void			ExecuteSelf(
									MessageT			inMessage,
									void*				ioParam);


	// data members

protected:
	Boolean					mTopLevelWindow;			// true if this a top-level window
	SInt16					mNestingLevel;				// nesting level in object hierarchy

};
