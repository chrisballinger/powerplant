// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxMiscellaneousEvents.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:15 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxMiscellaneousEvents.h
	@brief		Event handlers for Volume, Appearance, and Text Field
				Carbon Events
*/
	// ### Tablet events not implemented
	
#ifndef H_PPxMiscellaneousEvents
#define H_PPxMiscellaneousEvents
#pragma once

#include <PPxEventDoer.h>

namespace PPx {

// ===========================================================================
/**	Notification that a volume has been mounted */

class	VolumeMountedDoer : public SpecificEventDoer<
										kEventClassVolume,
										kEventVolumeMounted> {
protected:
	virtual OSStatus	DoVolumeMounted(
								SysCarbonEvent&	ioEvent,
								FSVolumeRefNum	inVolumeRefNum) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Notification that a volume has been unmounted */

class	VolumeUnmountedDoer : public SpecificEventDoer<
										kEventClassVolume,
										kEventVolumeUnmounted> {
protected:
	virtual OSStatus	DoVolumeUnmounted(
								SysCarbonEvent&	ioEvent,
								FSVolumeRefNum	inVolumeRefNum) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


// ===========================================================================
/**	Notification that the scroll bar variant has changed */

class	AppearanceScrollBarVariantChangedDoer : public SpecificEventDoer<
															kEventClassAppearance,
															kEventAppearanceScrollBarVariantChanged> {
protected:
	virtual OSStatus	DoAppearanceScrollBarVariantChanged(
								SysCarbonEvent&	ioEvent,
								SInt16			inVariant ) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};


#if (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_3)

// ===========================================================================
/**	Notifies that text in an editable text field has been accepted */

class	TextAcceptedDoer : public SpecificEventDoer<
										kEventClassTextField,
										kEventTextAccepted> {
protected:
	virtual OSStatus	DoTextAccepted(
								SysCarbonEvent&	ioEvent,
								ControlRef		inControl) = 0;

private:
	virtual OSStatus	DoEvent( SysCarbonEvent& ioEvent );
};

#endif // 10.3 or later

} // namespace PPx

#endif	// H_PPxMiscellaneousEvents
