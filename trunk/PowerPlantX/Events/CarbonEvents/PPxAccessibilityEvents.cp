// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxAccessibilityEvents.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:12 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxAccessibilityEvents.h>
#include <SysEventParam.h>

namespace PPx {

// ===========================================================================
//	AccessibleGetChildAtPointDoer

OSStatus
AccessibleGetChildAtPointDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	AXUIElementRef	accessible;
	HIPoint			globalPoint;
	
	SysEventParam::Get(ioEvent, kEventParamAccessibleObject, accessible);
	SysEventParam::Get(ioEvent, kEventParamMouseLocation, globalPoint);
	
	AXUIElementRef	child = nil;
	
	OSStatus	status = DoAccessibleGetChildAtPoint(
							ioEvent, accessible, globalPoint, child);
							
	if ((status == noErr) and (child != nil)) {
		SysEventParam::Set(ioEvent, kEventParamAccessibleChild, child);
	}
	
	return status;
}


// ===========================================================================
//	AccessibleGetFocusedChildDoer

OSStatus
AccessibleGetFocusedChildDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	AXUIElementRef	accessible;
	
	SysEventParam::Get(ioEvent, kEventParamAccessibleObject, accessible);
	
	AXUIElementRef	child;
	
	OSStatus	status = DoAccessibleGetFocusedChild(
							ioEvent, accessible, child);
							
	if ((status == noErr) and (child != nil)) {
		SysEventParam::Set(ioEvent, kEventParamAccessibleChild, child);
	}
	
	return status;
}


// ===========================================================================
//	AccessibleGetAllAttributeNamesDoer

OSStatus
AccessibleGetAllAttributeNamesDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	AXUIElementRef		accessible;
	CFMutableArrayRef	attrNames;
	
	SysEventParam::Get(ioEvent, kEventParamAccessibleObject, accessible);
	SysEventParam::Get(ioEvent, kEventParamAccessibleAttributeNames, attrNames);
	
	return DoAccessibleGetAllAttributeNames(ioEvent, accessible, attrNames);
}


// ===========================================================================
//	AccessibleGetNamedAttributeDoer

OSStatus
AccessibleGetNamedAttributeDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	AXUIElementRef		accessible;
	CFStringRef			attrName;
	
	SysEventParam::Get(ioEvent, kEventParamAccessibleObject, accessible);
	SysEventParam::Get(ioEvent, kEventParamAccessibleAttributeName, attrName);
	
	return DoAccessibleGetNamedAttribute(ioEvent, accessible, attrName);
}


// ===========================================================================
//	AccessibleSetNamedAttributeDoer

OSStatus
AccessibleSetNamedAttributeDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	AXUIElementRef		accessible;
	CFStringRef			attrName;
	
	SysEventParam::Get(ioEvent, kEventParamAccessibleObject, accessible);
	SysEventParam::Get(ioEvent, kEventParamAccessibleAttributeName, attrName);
	
	return DoAccessibleSetNamedAttribute(ioEvent, accessible, attrName);
}


// ===========================================================================
//	AccessibleIsNamedAttributeSettableDoer

OSStatus
AccessibleIsNamedAttributeSettableDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	AXUIElementRef		accessible;
	CFStringRef			attrName;
	
	SysEventParam::Get(ioEvent, kEventParamAccessibleObject, accessible);
	SysEventParam::Get(ioEvent, kEventParamAccessibleAttributeName, attrName);
	
	bool	isSettable;
	
	OSStatus	status = DoAccessibleIsNamedAttributeSettable(
									ioEvent, accessible, attrName, isSettable);
	
	if (status == noErr) {
		SysEventParam::Set(ioEvent, kEventParamAccessibleAttributeSettable,
								(Boolean) isSettable);
	}
							
	return status;
}


// ===========================================================================
//	AccessibleGetAllActionNamesDoer

OSStatus
AccessibleGetAllActionNamesDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	AXUIElementRef		accessible;
	CFMutableArrayRef	actionNames;
	
	SysEventParam::Get(ioEvent, kEventParamAccessibleObject, accessible);
	SysEventParam::Get(ioEvent, kEventParamAccessibleActionNames, actionNames);
	
	return DoAccessibleGetAllActionNames(ioEvent, accessible, actionNames);
}


// ===========================================================================
//	AccessiblePerformNamedActionDoer

OSStatus
AccessiblePerformNamedActionDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	AXUIElementRef		accessible;
	CFStringRef			actionName;
	
	SysEventParam::Get(ioEvent, kEventParamAccessibleObject, accessible);
	SysEventParam::Get(ioEvent, kEventParamAccessibleActionName, actionName);
	
	return DoAccessiblePerformNamedAction(ioEvent, accessible, actionName);
}


// ===========================================================================
//	AccessibleGetNamedActionDescriptionDoer

OSStatus
AccessibleGetNamedActionDescriptionDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	AXUIElementRef		accessible;
	CFStringRef			actionName;
	CFMutableStringRef	description;
	
	SysEventParam::Get(ioEvent, kEventParamAccessibleObject, accessible);
	SysEventParam::Get(ioEvent, kEventParamAccessibleActionName, actionName);
	SysEventParam::Get(ioEvent, kEventParamAccessibleActionDescription, description);
	
	
	return DoAccessibleGetNamedActionDescription(
								ioEvent, accessible, actionName, description);
}


} // namespace PPx