// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxConstants.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:10 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxConstants.h
	@brief		Declarations of commonly used constants
*/

#ifndef H_PPxConstants
#define H_PPxConstants
#pragma once

#include <PPxTypes.h>
#include <CoreFoundation/CoreFoundation.h>
															/** PowerPlantX */
namespace PPx {

// ---------------------------------------------------------------------------
// 	Common Values

const UInt32		attributes_None		= 0;
const OptionBits	options_None		= 0;
const OptionBits	features_None		= 0;

const bool			visible_Yes			= true;
const bool			visible_No			= false;

const bool			enabled_Yes			= true;
const bool			enabled_No			= false;

const SInt32		value_Off			= 0;
const SInt32		value_On			= 1;
const SInt32		value_Mixed			= 2;

const SInt32		value_Closed		= 0;
const SInt32		value_Open			= 1;

const bool			clear_Yes			= true;
const bool			clear_No			= false;

const bool			retain_Yes			= true;
const bool			retain_No			= false;

const ObjectIDT		objectID_None 		= 0;
const CommandIDT	commandID_None		= 0;


// ---------------------------------------------------------------------------
//	CoreFoundation string encodings

extern const CFStringEncoding	encoding_System;
const CFStringEncoding			encoding_Invalid = kCFStringEncodingInvalidId;


// ---------------------------------------------------------------------------
//	CoreFoundation strings

extern const CFStringRef	cfString_Empty;

					// Pascal string with zero length byte
extern const unsigned char	pString_Empty[];


// ---------------------------------------------------------------------------
//	CoreFoundation sizes, positions, and ranges

const CFIndex			cfSize_Unlimited	= 0;

const CFIndex			cfIndex_First		= CFIndex(0);
const CFIndex			cfIndex_Last		= CFIndex(0x7FFFFFFF);
const CFIndex			cfIndex_BeforeStart	= CFIndex(-2);
const CFIndex			cfIndex_AfterEnd	= CFIndex(-3);
const CFIndex			cfIndex_Bad			= CFIndex(-4);

extern const CFRange	cfRange_All;
extern const CFRange	cfRange_Empty;


// ---------------------------------------------------------------------------
//	File Manager constants

const SInt16		refNum_Invalid = -1;
const SInt32		dirID_Invalid = 0;


// ---------------------------------------------------------------------------
//	Custom event class identifiers

							/** Custom event class for processing a command */
const EventClassT	eventClass_ProcessCommand	= 'Pcmd';

							/** Custom event class for updating the status
								of a command */
const EventClassT	eventClass_UpdateCmdStatus	= 'Pups';

							/** Custom PPx event class */
const EventClassT	eventClass_PPx				= 'PPxE';


							/** Target is being deleted.
							
								kEventParamDirectObject is the
									system EventTargetRef of the target
									being deleted.							*/
const EventKindT	eventKind_TargetDeleted		= 'TDel';



} // namespace PPx

#endif // H_PPxConstants
