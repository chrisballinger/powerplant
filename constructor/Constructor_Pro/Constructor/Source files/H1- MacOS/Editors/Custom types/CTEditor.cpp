// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	CTEditor.cpp				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 12/09/96
//     $Date: 2006/04/12 08:48:30 $
//  $History: CTEditor.cpp $
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 03/18/97   Time: 15:26
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Custom types
//	Removed RFResource-specific code.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:08
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Custom types
//	Improved commenting.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/04/97   Time: 19:03
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Custom types
//	Finished adaptation to refactored RM/RF classes.
//	
//	*****************  Version 3  *****************
//	User: Andrew       Date: 2/04/97    Time: 3:00p
//	Updated in $/ConstructorWin/Sources
//	Added MSVC prefix file
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:42
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Custom types
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 12/10/96   Time: 14:22
//	Created in $/Constructor/Source files/Editors/Custom types
//	Baseline CTYP editor.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "CTEditor.h"

	// Core : Headers
#include "Constructor.menu.h"

	// Core : Data model : Core objects
#include "DMReanimator.h"

	// Core : Data model : Utility classes
#include "DMIterator.h"

	// Core : Display classes (Eric) : Window positioning
#include "LWindowPositioner.h"

	// Core : Editors : Generic editor
#include "RESession.h"
//#include "StResourceContext.h"
#include <UResourceMgr.h>

	// MacOS : Editors : Custom types
#include "CTModelAttribute.h"
#include "CTModelObject.h"
#include "CTModelValueLabel.h"
#include "CTOutlineTable.h"

	// MacOS : Resource manager
#include "RFResource.h"


// ===========================================================================
//		* Class variables
// ===========================================================================

LMenu*	CTEditor::sCtypMenu = nil;


// ===========================================================================
//		* CTYP resource format description
// ===========================================================================
//	Adapted from UReanimator.cp.

typedef	SInt32 TagID;

enum {
	tag_ObjectData		= 'objd',
	tag_BeginSubs		= 'begs',
	tag_EndSubs			= 'ends',
	tag_Include			= 'incl',
	tag_ClassAlias		= 'dopl',
	tag_Comment			= 'comm',
	tag_End				= 'end.',
	
	object_Null			= 'null'
};


// ===========================================================================

#pragma mark *** CTEditor ***

// ---------------------------------------------------------------------------
//		* CTEditor()
// ---------------------------------------------------------------------------
//	Constructor

CTEditor::CTEditor(
	LCommander*		inSuper,
	RESession&		inSession)

: REEditor(inSuper, inSession)

{

	// Validate pointers.

	ValidateObject_(inSuper);
	ValidateObject_(&inSession);

	// Register Txtr editor classes.
	
	RegisterCTClasses();

	// Build type list.
	
	mTypeList = CTTypeList::GetTypeList();

}


// ---------------------------------------------------------------------------
//		* ~CTEditor
// ---------------------------------------------------------------------------
//	Destructor

CTEditor::~CTEditor()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** editor override hooks

// ---------------------------------------------------------------------------
//		* ReadResourceDataSelf									[protected]
// ---------------------------------------------------------------------------
//	Read the CTYP resource.

void
CTEditor::ReadResourceDataSelf()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mPrimaryResource);

	// Build input streams for the two resources.
	
	LHandleStream ctypStream(mPrimaryResource->GetResData());

	// Make sure the CTYP version is correct.
	
	SInt16 ctypVersion = 0;
	ctypStream >> ctypVersion;
	if (ctypVersion != 1)
		return;
	
	// Parse the stream.
	
	CTModelObjectPtr rootObject;
	ReadObjectFromCTYPTags(rootObject, nil, ctypStream);
	
	// Check for 'end.' tag.
	
	FourCharCode endTag;
	ctypStream >> endTag;
	if (endTag != tag_End)
		return;
	
	// Okay, it was valid. Return it.

	mDataModelRoot = rootObject;
	
	// Get a selection for this menu editor.
	
	mSession.GetSelectionForEditor(this, mSelection);
	ValidateObject_(mSelection.GetObject());

	if (mSelection->GetRootElement() == nil)
		mSelection->SetRootElement(mDataModelRoot);

}


// ---------------------------------------------------------------------------
//		* WriteResourceDataSelf									[protected]
// ---------------------------------------------------------------------------
//	Write a CTYP resource.

void
CTEditor::WriteResourceDataSelf()
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mDataModelRoot.GetObject());
	ValidateObject_(mPrimaryResource);
	
	// Sanity check: Be sure we have a primary resource that's a CTYP.
	
//	ThrowIf_(mPrimaryResource->GetResType() != 'CTYP');
	
	// Build output stream for the resource.
	
	LHandleStream ctypStream;
	
	// Write the CTYP version number.
	
	SInt16 ctypVersion = 1;
	ctypStream << ctypVersion;
	
	// Write the CTYP tab format.
	
	WriteCTYPTags((CTModelObject*) (mDataModelRoot.GetObject()), ctypStream);

	// Write the 'end.' tag.
	
	FourCharCode endTag = tag_End;
	ctypStream << endTag;

	// Spill the streams to resources.
	
	mPrimaryResource->SetResData(ctypStream.GetDataHandle());
	
}


// ---------------------------------------------------------------------------
//		* OpenEditorWindowSelf									[protected]
// ---------------------------------------------------------------------------
//	Create an editor window for this CTYP resource.

LWindow*
CTEditor::OpenEditorWindowSelf()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mDataModelRoot.GetObject());

	// Create the window.

	LWindow* window = nil;
	{
		StApplicationContext appContext;
		window = LWindow::CreateWindow(6000, this);			//! TEMPORARY: magic number
		ValidateObject_(window);
	}

	// Set the window title to the resource name.
	
	LStr255 title;
	mSession.GetWindowTitleForEditor(this, title);
	window->SetDescriptor(title);

	// Attach custom types view.

	CTOutlineTable* ctypView = (dynamic_cast<CTOutlineTable*>
				(window->FindPaneByID('CTOV')));

	ValidateObject_(ctypView);
	ctypView->SetSelection(mSelection);
	ctypView->SetElement(mDataModelRoot);

	// Position & show the window.

	window->DoSetZoom(true);
	window->ProcessCommand(cmd_PositionWindow);
	window->Show();

	return window;

}


// ===========================================================================

#pragma mark -
#pragma mark ** CTYP parser

// ---------------------------------------------------------------------------
//		* ReadObjectFromCTYPTags								[protected]
// ---------------------------------------------------------------------------
//	Read a single object from the CTYP resource. Then look for subobjects.

void
CTEditor::ReadObjectFromCTYPTags(
	CTModelObjectPtr&	inObjectPtr,
	CTModelObject*		inParentObject,
	LStream&			inCTYPStream)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mTypeList.GetObject());

	// First, try to read this object.
	
	inObjectPtr = nil;

	FourCharCode tag = 0;
	Boolean stop = false;
	
	while (inObjectPtr == nil) {
		
		// Read the tag.
	
		inCTYPStream >> tag;
		switch (tag) {
			
			// Comment: Ignore for now. We'll use it later.
			
			case tag_Comment: {
				SInt32 commentLength = 0;
				inCTYPStream >> commentLength;
				inCTYPStream.SetMarker(commentLength, streamFrom_Marker);
				break;
			}
			
			// Object data: Try to build an object.

			case tag_ObjectData: {

				// Get the data length and record the mark.

				SInt32 dataLength;
				inCTYPStream >> dataLength;
				SInt32 dataStart = inCTYPStream.GetMarker();
				
				// Get class ID.
				
				ClassIDT classID;
				inCTYPStream >> classID;
				
				// See if we know anything about this type.
				
				CTModelObject* prototype = mTypeList->FindPrototype(classID);
				ValidateObject_(prototype);
				
				// We do, build an instance of it.
				
				inObjectPtr = (CTModelObject*) (prototype->Clone());

				// Read object data.
				
				inObjectPtr->ReadStreamData(&inCTYPStream, 'CTYP');
				
				// See if there's any leftover data.
				
				SInt32 leftoverSize = (dataStart + dataLength) - inCTYPStream.GetMarker();
				if (leftoverSize < 0)
					Throw_(1);			// we overshot, something's really hosed
				if (leftoverSize > 0) {
					inCTYPStream.SetMarker(leftoverSize, streamFrom_Marker);
					//! TEMPORARY: just skip the data
				}
				
				break;
			}
		
			// Anything else: Unread the tag and exit.
			
			default:
				inCTYPStream.SetMarker(-4, streamFrom_Marker);
				stop = true;
		}
	
		if (stop)
			break;

	}
	
	// Got the object, see if there are subobjects.
	
	inCTYPStream >> tag;
	if (tag != tag_BeginSubs) {
		inCTYPStream.SetMarker(-4, streamFrom_Marker);
		return;
	}
	
	// There are subobjects. Read them.
	
	if (inObjectPtr == nil)
		inObjectPtr = inParentObject;

	ValidateObject_(inObjectPtr.GetObject());
	ReadSubObjectsFromCTYPTags(inObjectPtr, inCTYPStream);

}


// ---------------------------------------------------------------------------
//		* ReadSubObjectsFromCTYPTags							[protected]
// ---------------------------------------------------------------------------
//	Read content between 'begs' and 'ends' tags in a CTYP resource.

void
CTEditor::ReadSubObjectsFromCTYPTags(
	CTModelObject*	inParentObject,
	LStream&		inCTYPStream)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inParentObject);
	ValidateObject_(mTypeList.GetObject());

	// Read tags until the end tag is reached.

	FourCharCode tag = 0;
	
	while (1) {
	
		// Read tags for subobjects.
	
		inCTYPStream >> tag;
		switch (tag) {
		
			// End subs: Go back up in nesting level.
		
			case tag_EndSubs:
				return;
			
			// Any object tag: Read a subobject and see where it fits in.
			
			case tag_ObjectData:
			case tag_ClassAlias: {
				inCTYPStream.SetMarker(-4, streamFrom_Marker);

				CTModelObjectPtr subObject;
				ReadObjectFromCTYPTags(subObject, inParentObject, inCTYPStream);
				
				if (subObject == nil)
					break;
				
				DMContainerAttribute* host = inParentObject->GetAttributeList();
				if (host != nil) {
					ValidateObject_(host);
					host->InsertElementAt(LArray::index_Last, subObject, true);
				}

				break;
			}

			// Comment: Ignore for now. We'll use it later.
			
			case tag_Comment: {
				SInt32 commentLength = 0;
				inCTYPStream >> commentLength;
				inCTYPStream.SetMarker(commentLength, streamFrom_Marker);
				break;
			}
			
			// Anything else: Unread the tag and exit.
			
			default:
				inCTYPStream.SetMarker(-4, streamFrom_Marker);
				return;
		}
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** CTYP generator

// ---------------------------------------------------------------------------
//		* WriteCTYPTags											[protected]
// ---------------------------------------------------------------------------
//	Write CTYP tags, which look suspiciously like PPob tags.

void
CTEditor::WriteCTYPTags(
	CTModelObject*	inObject,
	LHandleStream&	inCTYPStream)
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inObject);

	// Write the data for this object.
	// The stream must be random access since we back up to write the length.
	
	TagID theTag = tag_ObjectData;
	inCTYPStream << theTag;
	
	SInt32 length = 0;
	inCTYPStream << length;

	SInt32 dataStart = inCTYPStream.GetMarker();
	inCTYPStream << inObject->mTypeID;

	inObject->WriteStreamData(&inCTYPStream, 'CTYP');

	length = inCTYPStream.GetMarker() - dataStart;
	inCTYPStream.SetMarker(dataStart - sizeof (length), streamFrom_Start);
	inCTYPStream << length;
	inCTYPStream.SetMarker(dataStart + length, streamFrom_Start);

	// Now write stream data for any attachments or subpanes we may have.
	
	DMContainerAttribute* attachments =
			(dynamic_cast<DMContainerAttribute*>(inObject->FindAttributeByKey('cATL')));

	WriteCTYPTagsForSubs(attachments, inCTYPStream);

}


// ---------------------------------------------------------------------------
//		* WriteCTYPTagsForSubs									[protected]
// ---------------------------------------------------------------------------
//	Write tags for subobjects of a CTYP data model object.

void
CTEditor::WriteCTYPTagsForSubs(
	DMContainerAttribute*	inAttributes,
	LHandleStream&			inCTYPStream)
{
	
	// Validate pointers.
	
	ValidateThis_();
	
	// See if there are any subobjects to write home about. :-)

	if (inAttributes == nil)
		return;

	ValidateObject_(inAttributes);
	if (inAttributes->GetSubElements().GetCount() == 0)
		return;
	
	// We have subobjects. Write the begin subs tag.
	
	TagID theTag = tag_BeginSubs;
	inCTYPStream << theTag;
	
	// Iterate through the subobjects first.
	
	DMFastIterator attachments(inAttributes->GetSubElements());
	while (attachments.NextElement()) {
		CTModelObject* attachment = (dynamic_cast<CTModelObject*>(attachments.CurrentElement()));
		ValidateObject_(attachment);
		WriteCTYPTags(attachment, inCTYPStream);
	}
	
	// Write the end subs tag.
	
	theTag = tag_EndSubs;
	inCTYPStream << theTag;
	
}


// ===========================================================================

#pragma mark -
#pragma mark ** dynamic menus

// ---------------------------------------------------------------------------
//		* PutOnDuty												[protected]
// ---------------------------------------------------------------------------
//	Add the CTYP editor menu.

void
CTEditor::PutOnDuty( LCommander* /* inNewTarget */)
{

	// Validate pointers.
	
	ValidateThis_();

	// Load the menu resources if this hasn't been done already.

	{
		StApplicationContext appContext;

		if (sCtypMenu == nil)
		{
			sCtypMenu = new LMenu(6000);
			
#if PP_Target_Carbon
			SInt16 theIntegerIndex = sCtypMenu->IndexFromCommand('intv');
			SInt16 thePStringIndex = sCtypMenu->IndexFromCommand('pstr');
			SInt16 theRGBIndex = sCtypMenu->IndexFromCommand('rgbc');
			SInt16 theResourceIndex = sCtypMenu->IndexFromCommand('resl');
			
			::SetMenuItemModifiers(sCtypMenu->GetMacMenuH(), theIntegerIndex, kMenuShiftModifier);
			::SetMenuItemModifiers(sCtypMenu->GetMacMenuH(), thePStringIndex, kMenuShiftModifier);
			::SetMenuItemModifiers(sCtypMenu->GetMacMenuH(), theRGBIndex, kMenuShiftModifier);
			::SetMenuItemModifiers(sCtypMenu->GetMacMenuH(), theResourceIndex, kMenuShiftModifier);
#endif
		}
	}
	
	// Install these menus in the menu bar.
	
	LMenuBar* theBar = LMenuBar::GetCurrentMenuBar();
	ValidateObject_(theBar);
	ValidateSimpleObject_(sCtypMenu);

	theBar->InstallMenu(sCtypMenu, menu_Windows);
	
}


// ---------------------------------------------------------------------------
//		* TakeOffDuty											[protected]
// ---------------------------------------------------------------------------
//	Remove the CTYP editor menu.

void
CTEditor::TakeOffDuty()
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateSimpleObject_(sCtypMenu);

	// Remove menus from the menu bar.

	LMenuBar* theBar = LMenuBar::GetCurrentMenuBar();
	ValidateObject_(theBar);
	
	theBar->RemoveMenu(sCtypMenu);

}


// ===========================================================================

#pragma mark -
#pragma mark ** class registration

// ---------------------------------------------------------------------------
//		* RegisterCTClasses								[static, protected]
// ---------------------------------------------------------------------------
//	Register pane and data model classes used by the CTYP editor.

void
CTEditor::RegisterCTClasses()
{

	// Do this only once.
	
	static Boolean initialized = false;
	if (initialized)
		return;
	initialized = true;

	// Register custom type view classes.

	URegistrar::RegisterClass(CTOutlineTable::class_ID, (ClassCreatorFunc) CTOutlineTable::CreateFromStream);

	// Register custom type editor data model classes.

	DMReanimator* reanimator = DMReanimator::GetReanimator();
	ValidateObject_(reanimator);

	reanimator->RegisterClass(CTModelAttribute::class_ID, CTModelAttribute::CreateFromStream);
	reanimator->RegisterClass(CTModelObject::class_ID, CTModelObject::CreateFromStream);
	reanimator->RegisterClass(CTModelValueLabel::class_ID, CTModelValueLabel::CreateFromStream);

}