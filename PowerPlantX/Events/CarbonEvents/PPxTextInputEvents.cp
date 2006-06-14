// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxTextInputEvents.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:15 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxTextInputEvents.h>
#include <SysEventParam.h>
#include <vector>

namespace PPx {

// ===========================================================================
//	TextInputUpdateActiveInputAreaDoer

OSStatus
TextInputUpdateActiveInputAreaDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	return DoTextInputUpdateActiveInputArea(ioEvent);
}


// ===========================================================================
//	TextInputUnicodeForKeyEventDoer

OSStatus
TextInputUnicodeForKeyEventDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	UInt32					dataSize = 0;
	std::vector<UniChar>	buffer;

	OSStatus	status = SysEventParam::GetSizeOptional(ioEvent,
								kEventParamTextInputSendText, dataSize);

	if ((status == noErr) and (dataSize > 0)) {
		buffer = std::vector<UniChar>(dataSize / sizeof(UniChar));

		status = ::GetEventParameter(ioEvent, kEventParamTextInputSendText,
									 typeUnicodeText, nil, dataSize, nil,
									 &buffer[0]);
		PPx_ThrowIfOSError_(status, "GetEventParameter failed");
	}

	return DoTextInputUnicodeForKeyEvent(ioEvent, buffer.size(), &buffer[0]);
}


// ===========================================================================
//	TextInputOffsetToPosDoer

OSStatus
TextInputOffsetToPosDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	SInt32		textOffset;

	SysEventParam::Get(ioEvent, kEventParamTextInputSendTextOffset, textOffset);
	
	Point		globalPoint;
	
	OSStatus	status = DoTextInputOffsetToPos(ioEvent, textOffset, globalPoint);
	
	if (status == noErr) {
		SysEventParam::Set(ioEvent, kEventParamTextInputReplyPoint, globalPoint);
	}
	
	return status;
}


// ===========================================================================
//	TextInputPosToOffsetDoer

OSStatus
TextInputPosToOffsetDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	Point		globalPoint;

	SysEventParam::Get(ioEvent, kEventParamTextInputSendCurrentPoint, globalPoint);
	
	SInt32		regionClass;
	SInt32		textOffset;
	
	OSStatus	status = DoTextInputPosToOffset(ioEvent, globalPoint, regionClass,
												textOffset);
	
	if (status == noErr) {
		SysEventParam::Set(ioEvent, kEventParamTextInputReplyRegionClass, regionClass);
		SysEventParam::Set(ioEvent, kEventParamTextInputReplyTextOffset, textOffset);
	}
	
	return status;
}


// ===========================================================================
//	TextInputShowHideBottomWindowDoer

OSStatus
TextInputShowHideBottomWindowDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	return DoTextInputShowHideBottomWindow(ioEvent);
}


// ===========================================================================
//	TextInputGetSelectedTextDoer

OSStatus
TextInputGetSelectedTextDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	return DoTextInputGetSelectedText(ioEvent);
}


// ===========================================================================
//	TextInputUnicodeTextDoer

OSStatus
TextInputUnicodeTextDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	UniCharType	theUnichar;

	SysEventParam::Get(ioEvent, kEventParamTextInputSendText, theUnichar);

	return DoTextInputUnicodeText(ioEvent, theUnichar);
}


} // namespace PPx