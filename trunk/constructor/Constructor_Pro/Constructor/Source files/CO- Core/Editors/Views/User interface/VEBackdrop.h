// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VEBackdrop.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 06/06/96
//     $Date: 2006/01/18 01:33:15 $
//	$History: VEBackdrop.h $
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 02/20/97   Time: 18:27
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Improved commenting.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:30
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Fixed CR/LF problem
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 11/20/96   Time: 14:27
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Added CalcLocalBackdropFrame method.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/17/96   Time: 21:44
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Merged with VEBasicBackdrop class.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/16/96   Time: 23:11
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/+ User interface
//	Converted VEBackdrop from a pure abstract class to an LPane.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/13/96   Time: 13:36
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor
//	Rearranged view editor classes.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:59
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Utilities
#include "DMListener.h"

	// Core : Editors : Views : Data model
#include "VEModelObject.h"


// ===========================================================================
//		* VEBackdrop
// ===========================================================================
//
//	VEBackdrop is a base class for panes that display the backdrop
//	in a layout view. (The backdrop is the area between the edge of
//	the layout view itself and the edge of the root object's
//	display pane.) It provides a default behavior of drawing a simple
//	box around the root view.
//
// ===========================================================================

class VEBackdrop :	public LPane,
					public DMListener {

public:
							VEBackdrop();
	virtual					~VEBackdrop();

	// backdrop drawing

	virtual void			DrawSelf();

	// backdrop location

	virtual Boolean			CalcLocalFrameForRootObject(
									Rect&				outFrameRect);
	virtual Boolean			CalcLocalBackdropFrame(
									Rect&				outFrameRect);
	virtual void			GetBackdropOverscan(
									SDimension32&		outTopLeft,
									SDimension32&		outBotRight);

	// configuration
	
protected:
	virtual void			SetRootObject(
									VEModelObject*		inRootObject);

	// update notifications
	
	virtual void			ValueChanged(
									DM_ValueChange*		inMessage);
	virtual void			RootAttributeChanged(
									DMAttribute*		inAttribute);

	// drawing helpers
	
	virtual void			ShadeDesktopArea(
									const Rect&			inRootFrameRect);
	virtual RgnHandle		MakeBackdropRegion(
									const Rect&			inWindowFrameArea);


	// data members

protected:
	VEModelObjectPtr		mRootObject;				// root pane in this display


	friend class VELayoutView;

};
