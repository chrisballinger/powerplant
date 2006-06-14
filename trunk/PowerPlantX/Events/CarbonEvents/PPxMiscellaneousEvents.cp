// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxMiscellaneousEvents.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:14 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxMiscellaneousEvents.h>
#include <SysEventParam.h>

namespace PPx {

// ===========================================================================
//	VolumeMountedDoer

OSStatus
VolumeMountedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	FSVolumeRefNumType		volumeRefNum;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, volumeRefNum);
	
	return DoVolumeMounted(ioEvent, volumeRefNum);
}


// ===========================================================================
//	VolumeUnmountedDoer

OSStatus
VolumeUnmountedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	FSVolumeRefNumType		volumeRefNum;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, volumeRefNum);
	
	return DoVolumeUnmounted(ioEvent, volumeRefNum);
}


// ===========================================================================
//	AppearanceScrollBarVariantChangedDoer

OSStatus
AppearanceScrollBarVariantChangedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	SInt16		variant;
	
	SysEventParam::Get(ioEvent, kEventParamNewScrollBarVariant, variant);
	
	return DoAppearanceScrollBarVariantChanged(ioEvent, variant);
}


#if (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_3)

// ===========================================================================
//	TextAcceptedDoer

OSStatus
TextAcceptedDoer::DoEvent(
	SysCarbonEvent&	ioEvent)
{
	ControlRef		control;
	
	SysEventParam::Get(ioEvent, kEventParamDirectObject, control);

	return DoTextAccepted(ioEvent, control);
}

#endif // 10.3 or later


} // namespace PPx