// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	OVPipeDrawer.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/21/96
//     $Date: 2006/01/18 01:32:44 $
//  $History: OVPipeDrawer.h $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:36
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Outline table
//	Improved commenting.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:24
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Outline table
//	Fixed CR/LF problem
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/24/96   Time: 21:45
//	Updated in $/Constructor/Source files/Display classes (Eric)/Outline table
//	Finished implementation of pipe drawing.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/22/96   Time: 09:00
//	Updated in $/Constructor/Source files/Display classes (Eric)/Outline table
//	Fixed a missing class declaration.
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/22/96   Time: 08:56
//	Created in $/Constructor/Source files/Display classes (Eric)/Outline table
//	Added class.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Core objects
#include "DMSelection.h"

	// Core : Data model : Utilities
#include "DMListener.h"


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class LOutlineItem;
class OVTable;


// ===========================================================================
//		* OVPipeDrawer
// ===========================================================================
//
//	OVPipeDrawer draws "pipes" that represent the inheritance tree in
//	an outline. It is designed to sit on top of an OVTable.
//
// ===========================================================================

class OVPipeDrawer :	public LPane,
						public DMListener,
						public LPeriodical {

public:
	enum { class_ID = 'OVPD' };

	static LPane*			CreateFromStream(LStream* inStream)
									{ return new OVPipeDrawer(inStream); }

							OVPipeDrawer(LStream* inStream);
	virtual					~OVPipeDrawer();

	// drawing behaviors

protected:
	virtual void			DrawSelf();
	
	virtual void			DrawPipes(
									SInt16				inDepth,
									Boolean				inShowSelection);
	virtual void			DrawPipeSegment(
									const Rect&			inCellFrame,
									SInt16				inItemLeftEdge,
									SInt16				inDepth,
									SInt16				inNestingDepth,
									Boolean				inSideStick,
									Boolean				inSideStickDark,
									Boolean				inBottomStick,
									Boolean				inBottomStickDark);
	virtual void			SetPenColor(
									SInt32				inPixelDepth,
									SInt32				inColorSelector,
									Boolean				inDarken);
	virtual void			UpdatePipeCenters(
									LOutlineItem*		inItem,
									SInt16				inStopDepth);
	
	// selection refresh
	
	virtual void			SelectionChanged(
									DM_SelectionChanged* inMessage);
	virtual void			SpendTime(
									const EventRecord&	inMacEvent);

	// configuration
	
	virtual void			FinishCreateSelf();


	// data members

protected:
	OVTable*				mOVTable;				// outline table we're drawing in
	DMSelectionPtr			mSelection;				// table selection
	
	SInt16					mPipeCenters[32];		// horizontal center of pipes for each depth

};
