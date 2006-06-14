// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxServiceEvents.cp
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:15 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
//
//	Handlers for CarbonEvents of class kEventClassService
//
//	Each handler class implements two functions: Install and DoEvent.
//
//	Install calls the inherited EventDoer::Install function, passing the
//	appropriate event class and kind for the event.
//
//	DoEvent extracts the required and optional parameters from the event,
//	calls the "Do" function, then stores any output parameters back into
//	the event. Subclasses must implement the "Do" function, which is declared
//	as pure virtual.

#include <PPxServiceEvents.h>
#include <SysEventParam.h>

namespace PPx {

// ===========================================================================
//	ServiceCopyDoer

OSStatus
ServiceCopyDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	ScrapRef	scrap;
	
	SysEventParam::Get(ioEvent, kEventParamScrapRef, scrap);

	return DoServiceCopy(ioEvent, scrap);
}


// ===========================================================================
//	ServicePasteDoer

OSStatus
ServicePasteDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	ScrapRef	scrap;
	
	SysEventParam::Get(ioEvent, kEventParamScrapRef, scrap);

	return DoServicePaste(ioEvent, scrap);
}


// ===========================================================================
//	ServiceGetTypesDoer

OSStatus
ServiceGetTypesDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	CFMutableArrayRef	copyTypes;
	CFMutableArrayRef	pasteTypes;
	
	SysEventParam::Get(ioEvent, kEventParamServiceCopyTypes, copyTypes);
	SysEventParam::Get(ioEvent, kEventParamServicePasteTypes, pasteTypes);

	return DoServiceGetTypes(ioEvent, copyTypes, pasteTypes);
}


// ===========================================================================
//	ServicePerformDoer

OSStatus
ServicePerformDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	ScrapRef		scrap;
	CFStringRef		messageName;
	CFStringRef		userData;
	
	SysEventParam::Get(ioEvent, kEventParamScrapRef, scrap);
	SysEventParam::Get(ioEvent, kEventParamServiceMessageName, messageName);
	SysEventParam::Get(ioEvent, kEventParamServiceUserData, userData);

	return DoServicePerform(ioEvent, scrap, messageName, userData);
}


} // namespace PPx
