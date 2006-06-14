// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPDrawingAgent.cpp			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 06/17/96
//     $Date: 2006/01/18 01:33:57 $
//  $History: VPDrawingAgent.cpp $
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 02/28/97   Time: 10:33
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Added a quick and dirty hack to CreateDisplayObject to ensure that
//	items get created in the default superview. (Set superview field of
//	PPob to -1.)
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 01/24/97   Time: 15:00
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Drawing agents
//	Updated for refactored RFMap classes.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 12/13/96   Time: 15:51
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Updated to Clint's drop 12/13/96.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 12/06/96   Time: 18:38
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Updated CreateDisplayObject to bring user document to front while
//	building the object.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/19/96   Time: 14:29
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Drawing agents
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/17/96   Time: 11:19
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Layout view
//	Added resource map listening and Txtr update behaviors.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/31/96   Time: 13:59
//	Created
//	Comment: 
//	
// ===========================================================================

#include "VPDrawingAgent.h"

	// Core : Data model : Attributes
#include "DMIntegerAttribute.h"

	// Core : Editors : Views : User interface
#include "VELayoutView.h"

	// Core : Resource manager
//#include "StResourceContext.h"
#include <UResourceMgr.h>

	// MacOS : Editors : Views : Data model
#include "VPDataModel.h"

	// MacOS : Resource manager
#include "RFMap.h"
#include "RFResource.h"


// ===========================================================================

#pragma mark *** VPDrawingAgent ***

// ---------------------------------------------------------------------------
//		* VPDrawingAgent()
// ---------------------------------------------------------------------------
//	Constructor

VPDrawingAgent::VPDrawingAgent()
{
}


// ---------------------------------------------------------------------------
//		* ~VPDrawingAgent
// ---------------------------------------------------------------------------
//	Destructor

VPDrawingAgent::~VPDrawingAgent()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** drawing behaviors

// ---------------------------------------------------------------------------
//		* CreateDisplayObject
// ---------------------------------------------------------------------------
//	Create the PowerPlant class for this model object. Relies on the
//	pure virtual function CreateFromStrea to create the appropriate class,
//	then does some extra configuration on it.

void
VPDrawingAgent::CreateDisplayObject(
	LView*	inSuperView) 
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(inSuperView);
	ValidateObject_(mLayoutView);
	ValidateObject_(mModelObject.GetObject());

	// Create a PPob data stream by asking the data model object
	// to walk its members and stream out any attribute which uses
	// the 'PPob' stream. This is wonderfully easy. :-)
	
	LHandleStream ppobStream(::NewHandle(256));			// preallocate enough space for most streams
	ppobStream.SetMarker(0, streamFrom_Start);
	
	mModelObject->WriteStreamData(&ppobStream, 'PPob');
	ppobStream.SetMarker(0, streamFrom_Start);

	// Quick and dirty hack: Replace superview with -1 to ensure
	// object gets built correctly.
	
	SInt32 value = -1;

	ppobStream.SetMarker(26, streamFrom_Start);
	ppobStream << value;
	ppobStream.SetMarker(0, streamFrom_Start);

	// Prepare to create the display object.
	// We try as much as possible to reproduce the environment
	// that occurs when the pane is created via UReanimator and
	// its stream constructor.

	LPane::SetDefaultView(inSuperView);
	LCommander::SetDefaultCommander(mLayoutView);
	try {
		RFMap* map = mLayoutView->GetRFMap();
		ValidateObject_(map);

		StResourceContext docContext(map->GetResourceContext());
		
		mDisplayPane = CreateFromStream(&ppobStream);
	}
	catch (...) {
		// ignore errors
	}
}


// ===========================================================================

#pragma mark -
#pragma mark ** resource map update notifications

// ---------------------------------------------------------------------------
//		* ListenToMap											[protected]
// ---------------------------------------------------------------------------
//	Returns true if this drawing agent needs to listen for changes from
//	the resource map. Default method looks for Txtr attribute and says yes
//	if one if found.

Boolean
VPDrawingAgent::ListenToMap()
{
	
	// Validate pointers.
	
	ValidateThis_();
	ValidateObject_(mModelObject.GetObject());

	// If there is a text traits attribute, we listen for change messages.
	
	DMAttribute* txtrAttr = mModelObject->FindAttributeByKey(pPaneTextTraits);
	return (txtrAttr != nil);

}


// ---------------------------------------------------------------------------
//		* ResourceChanged										[protected]
// ---------------------------------------------------------------------------
//	Notification that a resource has been added, removed, or changed.
//	Default looks for 'Txtr' resource changes and calls TxtrChanged if so.

void
VPDrawingAgent::ResourceChanged(
	RMResource*		inResource)
{

	// Validate pointers.
	
	ValidateThis_();
	ValidateSimpleObject_(inResource);

	// Convert this to a Mac resource object.
	
	RFResource* changedRF = (dynamic_cast<RFResource*>(inResource));
	ValidateObject_(changedRF);

	// See if a text traits resource was affected.
	
	if (changedRF->GetResType() == 'Txtr') {
	
		// Yes, it was. Look for the model object's txtr attribute.
	
		ValidateObject_(mModelObject.GetObject());
		DMAttribute* txtrAttr = mModelObject->FindAttributeByKey(pPaneTextTraits);
		if (txtrAttr != nil) {
		
			// Got it. See if the ID matches.
			
			DMIntegerAttribute* txtrIntAttr = (dynamic_cast<DMIntegerAttribute*>(txtrAttr));
			ValidateObject_(txtrIntAttr);
			
			if (txtrIntAttr->GetIntValue() == changedRF->GetResID())
				TxtrChanged();
		
		}
	}
}


// ---------------------------------------------------------------------------
//		* TxtrChanged											[protected]
// ---------------------------------------------------------------------------
//	The Txtr resource that this pane uses has changed. Redraw or rebuild the
//	pane as appropriate. Default method just redraws.

void
VPDrawingAgent::TxtrChanged()
{
	RefreshDisplayObject();
}
