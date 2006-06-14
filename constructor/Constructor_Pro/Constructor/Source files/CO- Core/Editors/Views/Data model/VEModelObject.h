// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VEModelObject.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 07/02/96
//     $Date: 2006/01/18 01:33:12 $
//	$History: VEModelObject.h $
//	
//	*****************  Version 11  *****************
//	User: scouten      QDate: 03/19/97   Time: 14:53
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/Data model
//	Eliminated dangling pointers in VETypeEntry. (Bug fix #1310.)
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 02/20/97   Time: 18:26
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/Data model
//	Improved commenting.
//	
//	*****************  Version 9  *****************
//	User: Andrew       Date: 2/03/97    Time: 6:29p
//	Updated in $/ConstructorWin/Includes
//	Rearranged declarations for compilation by MSVC++
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 01/30/97   Time: 13:09
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/Data model
//	Added CanDeleteThis override to prevent deleting the root object of a
//	visual hierarchy. (Bug fix #1246.)
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:29
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/Data model
//	Fixed CR/LF problem
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 12/06/96   Time: 18:01
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/Data model
//	Added GetDisplayableObjectType method.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 11/25/96   Time: 15:24
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/Data model
//	Added GetDisplayableName and GetDescriptor methods.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/20/96   Time: 20:12
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/Data model
//	Added IsVisualObject and GetSubObjectList methods.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/17/96   Time: 10:33
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/Data model
//	Added GetPrintableID and IsSizeAttribute methods.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/13/96   Time: 13:37
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor
//	Rearranged view editor classes.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 02:00
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Core objects
#include "DMObject.h"

	// Core : Data model : Utilities
#include "DMListener.h"

	// Core : Editors : Views : Data model
#include "VETypeEntry.h"


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class VEDrawingAgent;
class VETypeEntry;

struct VE_TypeEntryChange;


// ===========================================================================
//		* VEModelObject											[abstract]
// ===========================================================================
//
//	VEModelObject is the basic container for data about a single
//	display object in a view.
//
// ===========================================================================

class VEModelObject :	public DMObject,
						public DMListener {

	// data model description
	
	DMAbstractClassID_		('vemo', VEModelObject, DMObject);
	
	// constructor/destructor

public:
							VEModelObject();
							VEModelObject(LStream* inStream);
							VEModelObject(const VEModelObject& inOriginal);
	virtual					~VEModelObject();
	
	// accessors

	virtual Boolean			IsVisible() const;
	virtual Boolean			IsVisualObject() const;

	virtual VEDrawingAgent*	FindDrawingAgent() const;
	virtual VEModelObject*	GetSuperModelObject() const;

	virtual Boolean			CanDeleteThis() const;

	virtual void			GetDisplayableName(
									LStr255&		outTitle) const;
	virtual void			GetDisplayableObjectType(
									LStr255&		outTitle) const;
	virtual void			GetPrintableID(
									LStr255&		outIDString) const;
	virtual void			GetDescriptor(
									LStr255&		outTitle) const;

	virtual DMContainerAttribute*
							GetSubPaneList() const = 0;
	virtual DMContainerAttribute*
							GetSubObjectList() const;

	// handy pane moving/sizing utilities
	
	virtual void			MoveBy(
									SInt32			inHorizDelta,
									SInt32			inVertDelta) = 0;
	virtual void			ResizeBy(
									SInt32			inHorizDelta,
									SInt32			inVertDelta) = 0;
	virtual void			ResizeTo(
									SInt32			inHorizSize,
									SInt32			inVertSize) = 0;
	
	virtual Boolean			IsSizeAttribute(
									FourCharCode	inAttributeKey) const = 0;
	
	// type definition
	
	inline VETypeEntry*		GetTypeEntry() const
									{ return mTypeEntry; }
	virtual void			SetTypeEntry(
									VETypeEntry*	inTypeEntry);

protected:
	virtual void			ListenToMessage(
									MessageT		inMessage,
									void*			ioParam);
	virtual void			TypeEntryChanged(
									VE_TypeEntryChange*	inChangeMessage);

	// data model changes

	virtual Boolean			CanHostElementSelf(
									const DMElement*	inNewElement,
									DM_DragPlacement*	ioPlacement);


	// data members

protected:
	TSharablePtr<VETypeEntry> mTypeEntry;			// runtime class type description

};


typedef TSharablePtr<VEModelObject> VEModelObjectPtr;
