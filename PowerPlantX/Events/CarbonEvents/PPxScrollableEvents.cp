// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxScrollableEvents.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:15 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
//
//	Handlers for CarbonEvents of class kEventClassScrollable
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

#include <PPxScrollableEvents.h>
#include <SysEventParam.h>

namespace PPx {

// ===========================================================================
//	ScrollableGetInfoDoer

OSStatus
ScrollableGetInfoDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	HISize		imageSize;
	HISize		viewSize;
	HISize		lineSize;
	HIPoint		origin;

	OSStatus	status = DoScrollableGetInfo(ioEvent, imageSize,
												viewSize, lineSize, origin);
												
	if (status == noErr) {
		SysEventParam::Set(ioEvent, kEventParamImageSize, imageSize);
		SysEventParam::Set(ioEvent, kEventParamViewSize,  viewSize);
		SysEventParam::Set(ioEvent, kEventParamLineSize,  lineSize);
		SysEventParam::Set(ioEvent, kEventParamOrigin,	  origin);
	}
	
	return status;
}


// ===========================================================================
//	ScrollableInfoChangedDoer

OSStatus
ScrollableInfoChangedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	return DoScrollableInfoChanged(ioEvent);
}


// ===========================================================================
//	ScrollableScrollToDoer

OSStatus
ScrollableScrollToDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	HIPoint		location;
	
	SysEventParam::Get(ioEvent, kEventParamOrigin, location);
	
	return DoScrollableScrollTo(ioEvent, location);
}


} // namespace PPx