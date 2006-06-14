// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VETypeEntry.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 06/12/96
//     $Date: 2006/01/18 01:33:12 $
//	$History: VETypeEntry.cpp $
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
//	User: Andrew       Date: 2/04/97    Time: 9:09p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:29
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

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "VETypeEntry.h"

	// Core : Utilities
#include "UIconUtilities.h"


// ===========================================================================

#pragma mark *** VETypeEntry ***

// ---------------------------------------------------------------------------
//		* VETypeEntry()
// ---------------------------------------------------------------------------
//	Default constructor

VETypeEntry::VETypeEntry()
{
	InitVETypeEntry();
}


// ---------------------------------------------------------------------------
//		* VETypeEntry(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor. Stream data for VETypeEntry is:
//
//		[DMObject data]
//		Str255:				Class name
//		Int16:				Icon ID for class icon
//		Boolean:			True if class is abstract (i.e. may not be
//									created at runtime)
//		OSType:				Class ID (needed for Constructor, and possibly
//									by framework)
//		OSType:				Class ID of parent class
//		Int16:				Default width of object when dragged
//		Int16:				Default height of object when dragged

VETypeEntry::VETypeEntry(
	LStream*	inStream)

: DMObject(inStream)

{

	// Clear fields.

	InitVETypeEntry();

	// Read stream data.

	SInt16 iconID;
	(*inStream)	>> (StringPtr) mClassName
				>> iconID
				>> mIsAbstractClass
				>> mClassID
				>> mParentClassID
				>> mDragWidth
				>> mDragHeight
				>> mMustBeRoot;
	
	// Fetch the icon, if there is one.
	
	if (iconID != 0)
		mClassIcon = UIconUtilities::Get1DetachedIconSuite(UIconUtilities::GetRemappedIconID(iconID), svAllSmallData);

}


// ---------------------------------------------------------------------------
//		* VETypeEntry(VETypeEntry&)
// ---------------------------------------------------------------------------
//	Copy constructor

VETypeEntry::VETypeEntry(
	const VETypeEntry&	inOriginal)

: DMObject(inOriginal)

{

	// Validate pointers.
	
	ValidateObject_(&inOriginal);

	// Do common initialization.

	InitVETypeEntry();

	// Copy variables.

	mClassName = inOriginal.mClassName;
			//? duplicate class icon?
	
	mClassID = inOriginal.mClassID;
	mParentClassID = inOriginal.mParentClassID;
	mDragHeight = inOriginal.mDragHeight;
	mDragWidth = inOriginal.mDragWidth;
	mMustBeRoot = inOriginal.mMustBeRoot;
	mIsAbstractClass = inOriginal.mIsAbstractClass;
	mIsBuiltIn = inOriginal.mIsBuiltIn;

	ValidateObject_(inOriginal.mPrototype.GetObject());
	mPrototype = (dynamic_cast<VEModelObject*>(inOriginal.mPrototype->Clone()));
			//? copy mRuntimeSuperClass & mDataSuperClass ?

}


// ---------------------------------------------------------------------------
//		* ~VETypeEntry
// ---------------------------------------------------------------------------
//	Destructor

VETypeEntry::~VETypeEntry()
{
	if (mClassIcon != nil) {
#if PP_Target_Carbon
		ThrowIfNil_(mClassIcon);
#else
		ValidateHandle_(mClassIcon);
#endif
		::DisposeIconSuite(mClassIcon, true);
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** creation functions

// ---------------------------------------------------------------------------
//		* CreateInstance
// ---------------------------------------------------------------------------
//	Return a data model object that is an instance of the class we describe.

void
VETypeEntry::CreateInstance(
	VEModelObjectPtr&	outInstance) const
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mPrototype.GetObject());
	
	// Create an instance.

	outInstance = nil;
	if (mPrototype != nil) {
		outInstance = (VEModelObject*) (mPrototype->Clone());
		if (outInstance != nil) {
			ValidateObject_(outInstance.GetObject());
			outInstance->SetTypeEntry((const_cast<VETypeEntry*>(this)));
			outInstance->ResizeTo(mDragWidth, mDragHeight);
		}
	}
}


// ---------------------------------------------------------------------------
//		* CreateDrawingAgent
// ---------------------------------------------------------------------------
//	Create a drawing agent for the class we describe.

VEDrawingAgent*
VETypeEntry::CreateDrawingAgent(
	VEModelObject*	inObject) const
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inObject);

	// I hate function pointers!

	VEDrawingAgent* agent = nil;
	
	if (mDrawingAgentCreatorFunc != nil)
		agent = (*mDrawingAgentCreatorFunc)();
	else if (mRuntimeSuperClass != nil)
		agent = mRuntimeSuperClass->CreateDrawingAgent(inObject);

	return agent;

}


// ===========================================================================

#pragma mark -
#pragma mark ** accessors

// ---------------------------------------------------------------------------
//		* GetClassIcon
// ---------------------------------------------------------------------------
//	Return an icon-suite handle for this class' class icon. Use the
//	superclass' icon if this class doesn't have one.

Handle
VETypeEntry::GetClassIcon() const
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Use our class icon, if we have one. Otherwise, ask
	// parent class for its icon.

	if (mClassIcon != nil) {
#if PP_Target_Carbon
		ThrowIfNil_(mClassIcon);
#else
		ValidateHandle_(mClassIcon);
#endif
		return mClassIcon;
	}
	else if (mRuntimeSuperClass != nil) {
		ValidateObject_(const_cast<VETypeEntry*>(this)->mRuntimeSuperClass);
		return mRuntimeSuperClass->GetClassIcon();
	}
	else
		return nil;

}


// ---------------------------------------------------------------------------
//		* GetShortClassName
// ---------------------------------------------------------------------------
//	Return a shortened version of the class name. In Java, this is
//	used to return the unqualified class name.

void
VETypeEntry::GetShortClassName(
	LStr255&	outClassName) const
{
	GetClassName(outClassName);
}


// ---------------------------------------------------------------------------
//		* GetDragSize
// ---------------------------------------------------------------------------
//	Return the prefered size of this object when dragged.

void
VETypeEntry::GetDragSize(
	SDimension32&	outDragSize) const
{

	// Validate pointers.

	ValidateThis_();
	
	// Return drag size.
	
	outDragSize.width = mDragWidth;
	outDragSize.height = mDragHeight;

}


// ---------------------------------------------------------------------------
//		* TypeDerivesFrom
// ---------------------------------------------------------------------------
//	Return true if this class is derived from the class whose ID is
//	named.

Boolean
VETypeEntry::TypeDerivesFrom(
	ClassIDT	inClassID) const
{

	// Validate pointers.

	if (inClassID == mClassID)
		return true;
	if (mRuntimeSuperClass != nil) {
		ValidateObject_(const_cast<VETypeEntry*>(this)->mRuntimeSuperClass);
		return mRuntimeSuperClass->TypeDerivesFrom(inClassID);
	}
	else
		return false;

}


// ===========================================================================

#pragma mark -
#pragma mark ** type entry change notifications

// ---------------------------------------------------------------------------
//		* SetReplacementEntry									[protected]
// ---------------------------------------------------------------------------
//	Replace this type entry with a new one for the same class.

void
VETypeEntry::SetReplacementEntry(
	VETypeEntry*	inReplacement)
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inReplacement);
	
	// Stash forwarding link.
	
	mReplacementClass = inReplacement;
	
	// Tell everyone about it.
	
	VE_TypeEntryChange changeMessage;
	changeMessage.mOldType = this;
	changeMessage.mNewType = inReplacement;
	BroadcastMessage(TypeEntry_Replaced, &changeMessage);
	
}


// ---------------------------------------------------------------------------
//		* ListenToMessage										[protected]
// ---------------------------------------------------------------------------
//	Listen for notification that an ancestor of this class has changed.

void
VETypeEntry::ListenToMessage(
	MessageT	inMessage,
	void*		ioParam) 
{

	switch (inMessage) {
		
		case TypeEntry_Replaced:
			VE_TypeEntryChange* changeMsg = (VE_TypeEntryChange*) ioParam;
			TypeEntryChanged(changeMsg);
			break;
		
	}
}


// ---------------------------------------------------------------------------
//		* TypeEntryChanged										[protected]
// ---------------------------------------------------------------------------
//	A type entry has changed. If it's an parent of this class, update
//	the pointer links.

void
VETypeEntry::TypeEntryChanged(
	VE_TypeEntryChange*	inChangeMessage)
{

	// Validate pointers.
	
	ValidateThis_();
	
	// See what type object changed.
	
	if (inChangeMessage->mOldType == mRuntimeSuperClass) {
		mRuntimeSuperClass = inChangeMessage->mNewType;
		mRuntimeSuperClass->AddListener(this);
	}
	
	if (inChangeMessage->mNewType == mDataSuperClass) {
		mDataSuperClass = inChangeMessage->mNewType;
		mDataSuperClass->AddListener(this);
		
		//! TEMPORARY: should rebuild prototype object!
	
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** initialization

// ---------------------------------------------------------------------------
//		* FinishCreateSelf										[protected]
// ---------------------------------------------------------------------------
//	Make sure there's a container attribute which can hold the catalog
//	subclasses.

void
VETypeEntry::FinishCreateSelf()
{

	// Validate pointers.
	
	ValidateThis_();
	
	// Make sure there's a catalog entry for subclasses.
	
	DMContainerAttributePtr subclasses;
	subclasses = new DMContainerAttribute;
	
	subclasses->FinishCreate();
	subclasses->SetKey(attrKey_SubClasses);
	subclasses->SetIncludeInDisplay(false);

	// Add it to our list.
	
	InsertElementAt(LArray::index_Last, subclasses, true);

}


// ---------------------------------------------------------------------------
//		* InitVETypeEntry										[private]
// ---------------------------------------------------------------------------
//	Common code for VETypeEntry constructors.

void
VETypeEntry::InitVETypeEntry()
{
	mClassID = mParentClassID = 0;
	mDragHeight = mDragWidth = 0;
	mMustBeRoot = false;
	mClassIcon = nil;
	mIsBuiltIn = false;
	mIsAbstractClass = false;
	mDrawingAgentCreatorFunc = nil;
}