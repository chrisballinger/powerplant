// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	GActiveScroller.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 03/22/96
//     $Date: 2006/01/18 01:32:45 $
//	$History: GActiveScroller.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:43
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Pane classes
//	Improved commenting.
//
//	*****************  Version 2   *****************
//	User: scouten      Date: 01/24/97   Time: 17:24
//	Checked in '$/Constructor/Source files/CO- Core/Display classes (Eric)/Pane classes'
//	Comment: Fixed CR/LF problem
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:45
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// PowerPlant : Pane classes
#include <LActiveScroller.h>


// ===========================================================================
//		* GActiveScroller
// ===========================================================================
//	Scroller that dims its frame when inactive.

class GActiveScroller : public LActiveScroller {

public:
	static GActiveScroller*	CreateFromStream(LStream*	inStream)
									{ return new GActiveScroller(inStream); }

							GActiveScroller(
									LStream*			inStream);
	virtual					~GActiveScroller();

protected:
	virtual void			DrawSelf();
	virtual void			ActivateSelf();

};
