// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMDragSelector.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 02/12/96
//	   $Date: 2006/01/18 01:32:39 $
//	$History: DMDragSelector.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:14
//	Updated in $/Constructor/Source files/CO- Core/Data model/Utilities
//	Improved commenting.
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 01/24/97   Time: 17:22
//	Checked in '$/Constructor/Source files/CO- Core/Data model/Utilities'
//	Comment: Fixed CR/LF problem
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:43
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Core objects
#include "DMPointers.h"

	// PowerPlant : CA : Arrays
#include <LSharableArray.h>

	// PowerPlant : CA : Features
#include <LMarqueeTask.h>


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class DMElement;
class DMSelection;
class LView;


// ===========================================================================
//		* DMDragSelector										[abstract]
// ===========================================================================
//
//	DMDragSelector is a subclass of LMarqueeReceiver that helps in
//	implementing drag selection with the data model classes. Subclasses
//	of DMDragSelector must override the CheckElements method. This
//	method should call FoundElement for each element which is displayed
//	in the view to determine whether each object is selected or
//	unselected by the new marquee rectangle.
//
//	Subclasses need not be concerned with shift-selection or when to
//	redraw the selection. This behavior is provided by DMDragSelector.
//
// ===========================================================================

class DMDragSelector : public LMarqueeReceiver {

public:
							DMDragSelector(
									LView&				inView,
									DMSelection*		inSelection);
	virtual					~DMDragSelector();

	// overrides for selection behavior

	virtual Boolean			CheckSelectionChanged(
									const LMarqueeTask&	inMarqueeTask,
									const Rect&			inOldMarqueeRect,
									const Rect&			inNewMarqueeRect);
	virtual void			UpdateSelection(
									const LMarqueeTask&	inMarqueeTask,
									const Rect&			inOldMarqueeRect,
									const Rect&			inNewMarqueeRect);
	virtual void			FocusMarqueeArea();
	virtual LView*			GetTargetView() const;

	// selection testing

	virtual void			CheckElements(
									const Rect&			inOldMarqueeRect,
									const Rect&			inNewMarqueeRect) = 0;
	void					FoundElement(
									DMElement*			inElement,
									Boolean				inIsInSelectionRect);

	// configuration
	
	virtual void			RecordInitialSelection(
									Boolean				inInvertCurrentSelection);


protected:
	LView&					mView;						// the host view
	DMSelectionPtr			mSelection;					// the selection object being modified

	LSharableArray			mAddToSelection;			// objects to be added to selection
	LSharableArray			mRemoveFromSelection;		// objects to be removed from selection
	LSharableArray			mInvertSelection;			// objects originally in selection

};
