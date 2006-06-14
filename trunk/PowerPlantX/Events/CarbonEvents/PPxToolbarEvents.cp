// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxToolbarEvents.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:15 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxToolbarEvents.h>
#include <SysEventParam.h>

#include <Carbon/Carbon.h>

namespace PPx {

// ===========================================================================
//	ToolbarGetDefaultIdentifiersDoer

OSStatus
ToolbarGetDefaultIdentifiersDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	HIToolbarRefType	toolbarRef;
	CFMutableArrayRef	identifiers;
	
	SysEventParam::Get(ioEvent, kEventParamToolbar, toolbarRef);
	SysEventParam::Get(ioEvent, kEventParamMutableArray, identifiers);
	
	return DoToolbarGetDefaultIdentifiers(ioEvent, toolbarRef, identifiers);
}


// ===========================================================================
//	ToolbarGetAllowedIdentifiersDoer

OSStatus
ToolbarGetAllowedIdentifiersDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	HIToolbarRefType	toolbarRef;
	CFMutableArrayRef	identifiers;
	
	SysEventParam::Get(ioEvent, kEventParamToolbar, toolbarRef);
	SysEventParam::Get(ioEvent, kEventParamMutableArray, identifiers);
	
	return DoToolbarGetAllowedIdentifiers(ioEvent, toolbarRef, identifiers);
}


// ===========================================================================
//	ToolbarCreateItemWithIdentifierDoer

OSStatus
ToolbarCreateItemWithIdentifierDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	HIToolbarRefType	toolbarRef;
	CFStringRef			identifier;
	CFTypeRef			configData = nil;
	
	SysEventParam::Get(ioEvent, kEventParamToolbar, toolbarRef);
	SysEventParam::Get(ioEvent, kEventParamToolbarItemIdentifier, identifier);
	
	SysEventParam::GetOptional(ioEvent, kEventParamToolbarItemConfigData, configData);
	
	HIToolbarItemRefType	createdItem;
	
	OSStatus	status = DoToolbarCreateItemWithIdentifier(ioEvent, toolbarRef,
											identifier, configData, createdItem);
											
	if (status == noErr) {
		SysEventParam::Set(ioEvent, kEventParamToolbarItem, createdItem);
	}
	
	return status;
}


// ===========================================================================
//	ToolbarCreateItemFromDragDoer

OSStatus
ToolbarCreateItemFromDragDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	HIToolbarRefType	toolbarRef;
	DragRef				drag;
	
	SysEventParam::Get(ioEvent, kEventParamToolbar, toolbarRef);
	SysEventParam::Get(ioEvent, kEventParamDragRef, drag);
	
	HIToolbarItemRefType	createdItem;
	
	OSStatus	status = DoToolbarCreateItemFromDrag(ioEvent, toolbarRef,
														drag, createdItem);
											
	if (status == noErr) {
		SysEventParam::Set(ioEvent, kEventParamToolbarItem, createdItem);
	}
	
	return status;
}


} // namespace PPx