// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PIResourceRefItem.cpp		   © 1997 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 01/30/97
//     $Date: 2006/01/18 01:33:23 $
//	$History: PIResourceRefItem.cpp $
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 02/28/97   Time: 11:49
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Added support for ValuesOnly.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 02/21/97   Time: 11:15
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Improved commenting.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/18/97   Time: 15:40
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Merged in 2.4.1 updates.
//	
//	*****************  Version 4  *****************
//	User: Andrew       Date: 2/06/97    Time: 1:45p
//	Updated in $/ConstructorWin/Sources
//	Initial changes for building with MSVC: prefix file, inherited keyword
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/04/97   Time: 17:47
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Finished adaptation to refactored RM/RF classes.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 02/01/97   Time: 13:43
//	Updated in $/Constructor/Source files/CO- Core/Property inspector
//	Fixed some problems with the RF/RM class refactoring.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 01/31/97   Time: 11:50
//	Created
//	Comment: Added class.
//
// ===========================================================================

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "PIResourceRefItem.h"

	// Core : Data model : Attribute classes
#include "DMResourceRefAttribute.h"

	// Core : Data model : Undo-redo
#include "StDMAttributeSetter.h"

	// Core : Display classes (Eric) : Outline table
#include "OVTable.h"

	// Core : Documents : Generic resource file
#include "RSResource.h"
#include "RSContainer.h"

	// Core : Editors : Generic editor
#include "RESession.h"
#include "REEditor.h"
#include "RETypeEntry.h"

	// Core : Resource manager
#include "RMIdentifier.h"

	// Core : Utilities
#include "UMemoryUtils.h"

	// MacOS : Resource manager
#include "RFMap.h"
#include "RFResource.h"

	// PowerPlant : CA : Outline table
#include <LOutlineTable.h>
#include <LPushButton.h>

	// MSL Headers
#include <new> //for bad_alloc

using namespace std;

// ===========================================================================

#pragma mark *** PIResourceRefItem ***

// ---------------------------------------------------------------------------
//		* PIResourceRefItem()
// ---------------------------------------------------------------------------
//	Constructor

PIResourceRefItem::PIResourceRefItem()
{
	mButton = nil;
	mMap = nil;
	mRefAttribute = nil;
}



// ===========================================================================

#pragma mark -
#pragma mark ** configuration

// ---------------------------------------------------------------------------
//		* FinishCreate											[protected]
// ---------------------------------------------------------------------------
//	Create button to edit resource.

void
PIResourceRefItem::FinishCreate()
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mOutlineTable);

	// Load button from PPob resource.
	
	STableCell cell(FindRowForItem(), 2);
	mButton = (dynamic_cast<LPushButton*>(CreatePaneForCell(cell, 21107)));
	ValidateObject_(mButton);
	mButton->MoveBy(0, 1, false);

	UpdateButtonTitle();	
	
	// Place mButton in right edge of cell.

	Rect bounds;	
	mOutlineTable->GetLocalCellRect(cell, bounds);
	
	// Set up listener relationship.
	
	mButton->AddListener(this);
}


// ---------------------------------------------------------------------------
//		* SetElementSelf										[protected]
// ---------------------------------------------------------------------------
//	Make sure this is a resource ref attribute, and grab the RFMap.

void
PIResourceRefItem::SetElementSelf(
	DMElement*	inElement)
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(inElement);

	//call the base class SetElementSelf
	PIStringItem::SetElementSelf(inElement);

	// Make sure we have a resource ref attribute.

	mRefAttribute = dynamic_cast<DMResourceRefAttribute*>(inElement);
	ValidateObject_(mRefAttribute);	
	
	
	mMap = GetContainer()->GetRFMap();
	

	//listen to the map so that if our target changes we can update the button
	mMap->AddListener(this);
}


// ===========================================================================

#pragma mark -
#pragma mark ** drawing & editing

// ---------------------------------------------------------------------------
//		* GetDrawContentsSelf									[protected]
// ---------------------------------------------------------------------------
//	Overriden to cut the label text short so there's room for the button.

void
PIResourceRefItem::GetDrawContentsSelf(
	const STableCell&		inCell,
	SOutlineDrawContents&	ioDrawContents)
{

	// Validate pointers.

	ValidateThis_();

	// Do normal draw contents...

	PIStringItem::GetDrawContentsSelf(inCell, ioDrawContents);
	
	// ... except that we truncate the label text to make space for
	// the botton.
	
	if (inCell.col == 2)
		ioDrawContents.ioCellBounds.right -= 28;
	
}


// ---------------------------------------------------------------------------
//		* SetValueFromAttributeSelf								[protected]
// ---------------------------------------------------------------------------
//	Update button to match 

void
PIResourceRefItem::SetValueFromAttributeSelf()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mButton);
	ValidateObject_(mRefAttribute);
	
	// Update IPE display.

	PIStringItem::SetValueFromAttributeSelf();
	
	// Update the button
	
	UpdateButtonTitle();
	
}


// ---------------------------------------------------------------------------
//		* DisableSelf											[protected]
// ---------------------------------------------------------------------------
//	Disable the button.

void
PIResourceRefItem::DisableSelf()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mButton);
	
	// Disable the button.

	mButton->Disable();

}


// ---------------------------------------------------------------------------
//		* EnableSelf											[protected]
// ---------------------------------------------------------------------------
//	Enable the button.

void
PIResourceRefItem::EnableSelf()
{

	// Validate pointers.

	ValidateThis_();
	ValidateObject_(mButton);
	
	// Enable the button.

	UpdateButtonTitle();

}


// ---------------------------------------------------------------------------
//		* ListenToMessage										[protected]
// ---------------------------------------------------------------------------
//	We listen for the button's messages, and also for changes in the resource map

void
PIResourceRefItem::ListenToMessage(
	MessageT	inMessage,
	void*		ioParam)
{

	ValidateThis_();

	switch (inMessage) {
		case msg_CreateRef:
		{
			//The user pushed the "Create..." button, so create the resource
			
			//get the REType for the reference

			RETypeEntry* typeEntry = RETypeEntry::FindResTypeEntry(mRefAttribute->GetResourceType());
			ValidateObject_(typeEntry);

			//the name is "Resource created for <attribute name>
			LStr255 newName(21001,3);
			LStr255 attrName;
			mAttribute->GetTitle(attrName);
			newName += attrName;

			RSContainer *container = GetContainer();

			// Copy master resource suite.  We assume the master resource ID is 1 (bad)
	
			container->CopyMasterResourceSet(typeEntry, 1, mRefAttribute->GetIntValue(), newName);


			//Ick.  Since we are going to fall through and edit the resource immediately, 
			//we have to flush the deleted editors, because if during creation of the resource
			//an editor was temporarily created (for instance to update resID info), then the
			//below GetEditorForResource call will return that dying editor, and on the
			//next SpendTime after we return, our new editor will get deleted.
			
			container->GetSession()->FlushDeletedEditors();
			
			//fall through so we edit the newly created resource
		}
		case msg_EditRef:
		{
			//The user pushed the "Edit..." button, so get the session and edit the resource
			
			RSContainer *container = GetContainer();
			
			RESession* session = container->GetSession();
			
			//The session will do all the real work
			REEditor* editor = session->GetEditorForResource(
				mRefAttribute->GetResourceType(), mRefAttribute->GetIntValue(), true);

			if (editor != nil) {

				ValidateObject_(editor);

				// Got an editor, ask editor to make a window.
					
				try {
				
					LWindow* window = editor->OpenEditorWindow();
					if (window != nil) {
					
						// Got a window, bring window to front.
					
						ValidateObject_(window);
						window->Select();
	
					}
				}
				catch (const LException& err) {
					session->TearDownEditor(editor);
					if (err.GetErrorCode() == memFullErr)
						UMemoryUtils::ShowOutOfMemoryAlert();
					break;
				}
				catch (bad_alloc) {
					session->TearDownEditor(editor);
					UMemoryUtils::ShowOutOfMemoryAlert();
					break;
				}
				catch (...) {
					session->TearDownEditor(editor);
					break;
				}
			}
		
			break;
		}

		case Resource_Added:
		case Resource_Removed:
		{
			//We find out if the changed resource is our target resource,
			//and if so we update the button
			
			ValidateObject_(mButton);
			ValidateObject_(mRefAttribute);
			
			RMMap_ChangeMessage* message = (RMMap_ChangeMessage*) ioParam;
			ThrowIfNil_(message);
			
			RFResource* theRFResource = (dynamic_cast<RFResource*>(message->mResource));
			ValidateObject_(theRFResource);
			
			if ((theRFResource->GetResID() == mRefAttribute->GetIntValue())&&
				(theRFResource->GetResType() == mRefAttribute->GetResourceType()))
			{	
				if (inMessage == Resource_Added)
					SetButtonEdit(); 	//our resource has been added
				else
					SetButtonCreate();	//our resource has been removed
			}
			break;
		}
		default:
			PIStringItem::ListenToMessage(inMessage,ioParam);
		break;
	}
}


// ---------------------------------------------------------------------------
//		* UpdateButtonTitle											[protected]
// ---------------------------------------------------------------------------
//	Set the title of the button based on the existence of the target resource

void
PIResourceRefItem::UpdateButtonTitle()
{
	ValidateThis_();
	ValidateObject_(mButton);
	SInt32 id = mRefAttribute->GetIntValue();
	
	//find out if the target resource exists
	RFResource* targetResource = mMap->FindResource(mRefAttribute->GetResourceType(), id, false);

	if ((targetResource == nil)||(targetResource->IsDeleted())) 
	{
		SetButtonCreate(); 	//nope, change the button to say "create..."
		//if the id is zero, disable the button, so we don't create a resource
		//with ID zero
		if (!id)
			mButton->Disable();
		else
			mButton->Enable();
	}
	else	
	{
		SetButtonEdit();	//it exists, make it say "edit..."
		mButton->Enable();  //and enable it
	}
}

// ---------------------------------------------------------------------------
//		* SetButtonCreate											[protected]
// ---------------------------------------------------------------------------
//	Make the button say "Create..." and set its message

void 
PIResourceRefItem::SetButtonCreate()
{
	LStr255 createString(21001,2);
	mButton->SetDescriptor(createString);
	mButton->SetValueMessage(msg_CreateRef);
}


// ---------------------------------------------------------------------------
//		* SetButtonEdit												[protected]
// ---------------------------------------------------------------------------
//	Make the button say "Edit..." and set its message

void 
PIResourceRefItem::SetButtonEdit()
{
	LStr255 editString(21001,1);
	mButton->SetDescriptor(editString);
	mButton->SetValueMessage(msg_EditRef);
}

RSContainer*
PIResourceRefItem::GetContainer()
{
	//skanky hack to get the RSContainer; 
	//walk the data model up to the RSResource and ask it for it's RSContainer,
	//or if we find an RSContainer on the way, use it

	DMElement* ancestor = mAttribute;
	RSResource* resource = nil;
	RSContainer *container = nil;
	
	while (ancestor != nil) {
		ValidateObject_(ancestor);
		resource = (dynamic_cast<RSResource*>(ancestor));
		if (resource != nil)
			break;

		container = (dynamic_cast<RSContainer*>(ancestor));
		if (container != nil)
			break;

		ancestor = ancestor->GetSuperElement();
	}
	if (resource)
		container = resource->GetRSContainer();

	ValidateObject_(container);
	return container;
}