// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	DMElement.h					© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 04/26/96
//	   $Date: 2006/01/18 01:32:35 $
//	$History: DMElement.h $
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 02/20/97   Time: 14:24
//	Updated in $/Constructor/Source files/CO- Core/Data model/Core objects
//	Improved commenting.
//	
//	*****************  Version 9  *****************
//	User: Andrew       Date: 2/03/97    Time: 6:34p
//	Updated in $/ConstructorWin/Includes
//	Added declarations for MSVC++; exported destructor
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:20
//	Updated in $/Constructor/Source files/CO- Core/Data model/Core objects
//	Fixed CR/LF problem
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 12/07/96   Time: 18:22
//	Updated in $/Constructor/Source files/Data model/Core objects
//	Added ShallowClone method to DMElement.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 12/07/96   Time: 14:38
//	Updated in $/Constructor/Source files/Data model/Core objects
//	Fixed a memory leak in Clone macro that would occur if an exception was
//	thrown during the FinishCreate call.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 12/01/96   Time: 02:03
//	Updated in $/Constructor/Source files/Data model/Core objects
//	Changed DMClassID_ macro so that Clone() calls FinishCreate.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/30/96   Time: 19:04
//	Updated in $/Constructor/Source files/Data model/Core objects
//	Improved commenting.
//	
//	*****************  Version 3   *****************
//	User: scouten      Date: 10/23/96   Time: 18:34
//	Checked in '$/Constructor/Source files/Data model/Core objects'
//	Comment: Added DMAbstractClassID_ macro.
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 10/17/96   Time: 23:42
//	Checked in '$/Constructor/Source files/Data model/Core objects'
//	Comment: Replaced ItemControl and CanMakeChanges with Enabled flag.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:29
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// Core : Data model : Core classes
#include "DMPointers.h"

	// PowerPlant : Feature classes
#include <LBroadcaster.h>


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class LStr255;
struct DM_DragPlacement;
typedef FourCharCode DM_AttributeKey;
class DMObject;
class DMAttribute;


// ===========================================================================
//		* DMElement												[abstract]
// ===========================================================================
//
//	DMElement is at the core of the data modelling framework.
//	It is the basic abstraction for all displayable items (objects,
//	attributes, selection). It provides common code for handling
//	update and notification behaviors (thus the LBroadcaster
//	inheritance), and for handling memory management of the data
//	model (thus LSharable).
//
//	NOTE: Several update messages are defined at the end of this header file.
//
// ===========================================================================

class DMElement :	public LSharable,
					public LBroadcaster {

public:
	enum { class_ID = 'elem' };

protected:
							DMElement();
							DMElement(const DMElement& inOriginal);

public:
	virtual					~DMElement();

	// object type information

	virtual DMElement*		Clone() const = 0;
	virtual DMElement*		ShallowClone() const = 0;
	virtual void			SetShallowClone();

	virtual Boolean			IsDerivedFrom(FourCharCode inClassID) const
									{ return inClassID == class_ID; }

	virtual void			GetDisplayableName(
									LStr255&			outTitle) const;

	// object hierarchy
	
	virtual void			FinishCreate();
	
	inline DMElement*		GetSuperElement() const
									{ return mSuperElement; }
	virtual void			SetSuperElement(
									DMElement*			inSuperElement);
	virtual void			SetDeleted(
									Boolean				inDeleted);

	virtual Boolean			IsSubElementOf(
									const DMElement*	inSuperElement) const;
	virtual DMObject*		GetAffectedObject() const;

	// attribute accessors
	
	virtual DMAttribute*	FindAttributeByKey(
									DM_AttributeKey		inKey) const;
	virtual DMAttribute*	FindAttributeByTitle(
									ConstStringPtr		inTitle) const;

	// update notifications/requests

	virtual void			GoingAway();
	virtual void			CollectChanges();
	
	// user interface accessor
	
	virtual void*			FindUserInterfaceObject(
									FourCharCode		inObjectIdentifier) const;

	// modified flag accessors

	virtual Boolean			IsModified() const;
	virtual Boolean			IsSubModified(Boolean		inTentative = false) const;

	virtual void			MakeModified();
	virtual void			ResetModified();

	// enabled flag accessors
	
	virtual Boolean			CanDeleteThis() const;
	virtual Boolean			IsEnabled() const;

	virtual void			Enable();
	virtual void			Disable();

	// data model changes

	virtual Boolean			CanHostElement(
									const DMElement*		inNewElement,
									DM_DragPlacement*		ioPlacement);
	virtual void			HostElement(
									DMElement*				inNewElement,
									const DM_DragPlacement*	inPlacement);

	// update requests from parent

protected:
	virtual void			SuperGoingAway(DMElement*		inSuperElement);
	virtual void			SuperCollectChanges(DMElement*	inSuperElement);
	virtual void			SuperResetModified(DMElement*	inSuperElement);
	virtual void			SuperDeleted(DMElement*			inSuperElement);
	virtual void			SuperEnable(DMElement*			inSuperElement);
	virtual void			SuperDisable(DMElement*			inSuperElement);

	virtual Boolean			SuperCanHostElement(
									const DMElement*		inNewElement,
									DM_DragPlacement*		ioPlacement);

	// modified flag implementation

	virtual void			SubModified();

	// enabled flag implementation

	virtual void			EnableSelf();
	virtual void			DisableSelf();
	virtual void			SendEnabledChangeMessage();

	// update notification

	virtual void			SendUpdateMessage(
									MessageT				inMessage,
									void*					ioParam,
									DMElement*				inSubElement = nil);
	
	// override hooks for update notifications/requests
	
	virtual void			FinishCreateSelf();
	virtual void			GoingAwaySelf();
	virtual void			CollectChangesSelf();
	

	virtual Boolean			CanHostElementSelf(
									const DMElement*		inNewElement,
									DM_DragPlacement*		ioPlacement);

	virtual void			HostElementSelf(
									DMElement*				inNewElement,
									const DM_DragPlacement*	ioPlacement);

	// common initialization

private:
	void					InitElement();
	

	// data members

protected:
	DMElement*				mSuperElement;			// parent of this element in object hierarchy
	ETriState				mEnabled;				// true if this element is editable
	Boolean					mModified;				// true if this element is modified
	Boolean					mSubModified;			// true if subelement of this element is modified

	
	friend class DMContainerMixin;
	friend class DMModFlagSubAction;

};


// ===========================================================================
//		* Update messages
// ===========================================================================
//	A data element (DMElement) is an LBroadcaster. User interface objects
//	which act on this object should make themselves listeners to the element
//	so they can be made aware of changes to the object.


// ---------------------------------------------------------------------------
//	* Going away message:
//		This message indicates that the object is about to be deleted.
//		All user interface objects should delete themselves.

const MessageT Element_GoingAway = 'dead';
	// ioParam = DMElement*

// ---------------------------------------------------------------------------
//	* Collect changes message:
//		This message indicates that the object is about to be saved to disk.
//		All user interface objects should record any pending changes (such
//		as a partially completed editfield entry).

const MessageT Element_CollectChanges = 'colÆ';
	// ioParam = DMElement*

// ---------------------------------------------------------------------------
//	* Collect tentative changes message:
//		This message is sent in response to a FindCommandStatus request
//		for the Save command. All user interface objects should return
//		true in the mIsChanged member of this struct if there are changes
//		pending (such as a partially completed editfield entry).
//		These changes should NOT be recorded.

const MessageT Element_CollectTentativeChanges = 'chkÆ';
	// ioParam = DM_CollectTentativeChanges*

struct DM_CollectTentativeChanges {
	DMElement*		mElement;
	Boolean			mIsChanged;
};

// ---------------------------------------------------------------------------
//	* Modified flag changed message:
//		This message is sent whenever the value of mModified or mSubModified
//		is changing. Listeners may want to update their display based on this
//		information.

const MessageT Element_ModifiedFlagChanged = 'modÆ';
	// ioParam = DMElement*

// ---------------------------------------------------------------------------
//	* Element enabled message:
//		This message is sent just after an element becomes enabled or
//		disabled.

const MessageT Element_EnabledChange = 'enaÆ';
	// ioParam = DMElement*

// ---------------------------------------------------------------------------
//	* Find UI object
//		This message is sent whenever an element (or its client) wishes to
//		locate a user interface object related to the element.
//		Listeners should return their address if this message is received.

const MessageT Element_FindUIObject = 'fuio';
	// ioParam = DM_FindUIObject*

struct DM_FindUIObject {
	const DMElement*	mElement;
	FourCharCode		mObjectIdentifier;
	void*				mObjectPointer;
};


// ===========================================================================
//		* Drag and drop placement description
// ===========================================================================

struct DM_DragPlacement {
	DMElement*		mHostElement;			// object which will host dragged element
	UInt32			mInsertionIndex;		// sequence within this container (if applicable)
	Boolean			ioInsertAsDuplicate;	// true if object is copied, false if local move
};


// ===========================================================================
//		* Streaming setup macros
// ===========================================================================
//
//	The DMClassID_ macro should be used in the header of every concrete
//	DMElement subclass. It provides the class ID registration and create from
//	stream function.
//
// ===========================================================================

#define DMAbstractClassID_(classID, className, parentClassName)				\
	public:																	\
	enum { class_ID = classID };											\
	virtual Boolean		IsDerivedFrom(FourCharCode inClassID) const			\
							{ return (inClassID == class_ID)				\
							|| (parentClassName::IsDerivedFrom(inClassID)); }


#define DMClassIDNoClone_(classID, className, parentClassName)				\
	DMAbstractClassID_(classID, className, parentClassName);				\
	static DMElement*	CreateFromStream(LStream* inStream)					\
							{ return new className(inStream); }				\


#define DMClassID_(classID, className, parentClassName)						\
	DMClassIDNoClone_(classID, className, parentClassName);					\
	virtual DMElement*	Clone() const										\
							{ DMElement* clone = new className(*this);		\
							  try { clone->FinishCreate(); }				\
							  catch(...) { delete clone; throw; }			\
							  return clone; }								\
	virtual DMElement*	ShallowClone() const								\
							{ DMElement* clone = new className(*this);		\
							  clone->SetShallowClone();						\
							  try { clone->FinishCreate(); }				\
							  catch(...) { delete clone; throw; }			\
							  return clone; }
