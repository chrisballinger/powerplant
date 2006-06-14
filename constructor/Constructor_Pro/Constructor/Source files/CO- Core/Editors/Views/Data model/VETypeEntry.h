// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VETypeEntry.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 06/12/96
//     $Date: 2006/01/18 01:33:12 $
//	$History: VETypeEntry.h $
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 03/19/97   Time: 14:53
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/Data model
//	Eliminated dangling pointers in VETypeEntry. (Bug fix #1310.)
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 02/20/97   Time: 18:26
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/Data model
//	Improved commenting.
//	
//	*****************  Version 6  *****************
//	User: Andrew       Date: 2/03/97    Time: 6:31p
//	Updated in $/ConstructorWin/Includes
//	Rearranged declarations for compilation by MSVC++
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:30
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/Data model
//	Fixed CR/LF problem
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 12/01/96   Time: 02:06
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/Data model
//	Changed mRuntimeSuperClass and mDataSuperClass to be simple pointers.
//	This avoids a circular reference, which is a Bad Thing.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/30/96   Time: 18:24
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor/Data model
//	Added SubClasses container attribute and FinishCreateSelf method.
//	(Supports catalog.)
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

	// Core : Data model : Attributes
#include "DMContainerAttribute.h"

	// Core : Editors : Views : Data model
#include "VEModelObject.h"

	// PowerPlant : Pane classes
#include <LPane.h>


// ===========================================================================
//		* Helper types
// ===========================================================================

class VETypeEntry;
class VEDrawingAgent;
class VEModelObject;

struct VE_TypeEntryChange;

typedef TSharablePtr<VETypeEntry> VETypeEntryPtr;
typedef TSharablePtr<VEModelObject> VEModelObjectPtr;
typedef VEDrawingAgent* (*VE_DrawAgentCreatorFunc)();


// ===========================================================================
//		* VETypeEntry
// ===========================================================================
//
//	VETypeEntry contains the description of a single class in a
//	framework's display class hierarchy. It includes the class name,
//	a 16x16 icon for the class, and a data model prototype for instances
//	of the class.
//
// ===========================================================================

class VETypeEntry : public DMObject,
					public LListener {

	// data model description

	DMClassID_				('vete', VETypeEntry, DMObject);
	DMContainerAttribute_	('SCLS', SubClasses);

	// constructor/destructor

public:
							VETypeEntry();
							VETypeEntry(LStream* inStream);
							VETypeEntry(const VETypeEntry& inOriginal);
	virtual					~VETypeEntry();

	// creation functions
	
	virtual void			CreateInstance(
									VEModelObjectPtr&	outInstance) const;
	virtual VEDrawingAgent*	CreateDrawingAgent(
									VEModelObject*		inObject) const;

	// accessors
	
	Handle					GetClassIcon() const;
	inline void				GetClassName(LStr255&		outClassName) const
									{ outClassName = mClassName; }
	inline ClassIDT			GetClassID() const
									{ return mClassID; }
	inline ClassIDT			GetParentClassID() const
									{ return mParentClassID; }
	
	virtual void			GetShortClassName(
									LStr255&			outClassName) const;

	virtual void			GetDragSize(
									SDimension32&		outDragSize) const;

	inline VETypeEntry*		GetRuntimeSuperClass() const
									{ return mRuntimeSuperClass; }
	inline VETypeEntry*		GetDataSuperClass() const
									{ return mDataSuperClass; }
	inline Boolean			IsBuiltIn() const
									{ return mIsBuiltIn; }
	inline Boolean			IsAbstractClass() const
									{ return mIsAbstractClass; }
	inline Boolean			MustBeRoot() const
									{ return mMustBeRoot; }

	virtual Boolean			TypeDerivesFrom(
									ClassIDT			inClassID) const;

	// type entry change notifications
	
protected:
	virtual void			ListenToMessage(
									MessageT			inMessage,
									void*				ioParam);
	virtual void			TypeEntryChanged(
									VE_TypeEntryChange*	inChangeMessage);

	virtual void			SetReplacementEntry(
									VETypeEntry*		inReplacement);

	// initialization

	virtual void			FinishCreateSelf();

private:
	void					InitVETypeEntry();


	// data members

protected:
	LStr255					mClassName;
	Handle					mClassIcon;
	ClassIDT				mClassID;					// internal class ID
	ClassIDT				mParentClassID;				// parent class' ID

	SInt16					mDragWidth;					// size of default drag rectangle
	SInt16					mDragHeight;
	Boolean					mMustBeRoot;				// true if this class must be at the root of PPob
	Boolean					mIsBuiltIn;
	Boolean					mIsAbstractClass;
	
	VEModelObjectPtr		mPrototype;
	VETypeEntry*			mRuntimeSuperClass;
	VETypeEntry*			mDataSuperClass;
	VETypeEntryPtr			mReplacementClass;
	VE_DrawAgentCreatorFunc	mDrawingAgentCreatorFunc;


	friend class VETypeList;

};


// ===========================================================================
//		* Update notifications
// ===========================================================================

// ---------------------------------------------------------------------------
//	* Class definition replaced:
//		This message is sent when a new type entry replaces an existing
//		type entry. Listeners (subclasses and instances) should change
//		pointers to refer to the new definition.

const MessageT TypeEntry_Replaced = 'typ∆';
	// ioParam = VE_TypeEntryChange*

struct VE_TypeEntryChange {
	VETypeEntry*	mOldType;
	VETypeEntry*	mNewType;
};
