// Copyright �2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PP_Constants.h				PowerPlant 2.2.2	�1993-2005 Metrowerks Inc.
// ===========================================================================

#ifndef _H_PP_Constants
#define _H_PP_Constants
#pragma once

#include <PP_Types.h>

#if PP_Uses_Pragma_Import
	#pragma import on
#endif

PP_Begin_Namespace_PowerPlant

const SInt8		max_Int8		= 127;
const SInt8		min_Int8		= -128;

const UInt8		max_Uint8		= 255;

const SInt16	max_Int16		= 32767;
const SInt16	min_Int16		= -32768;

const SInt32	max_Int32		= 0x7FFFFFFF;	//  2,147,483,647
const SInt32	min_Int32		= 0x80000000;	// -2,147,483,648

const ResIDT	resID_Default	= 32766;
const ResIDT	resID_Undefined	= 32767;

const SInt16	ScrollBar_Size	= 16;

const SInt16	Button_Off		= 0;
const SInt16	Button_On		= 1;
const SInt16	Button_Mixed	= 2;

const UInt32	delay_Feedback	= 8;	// Ticks to delay for visual feedback

					// ID for a Pane which does not exist
const PaneIDT	PaneIDT_Undefined	= -1;

					// ID for a Pane which exists, but has not been
					//   assigned a unique ID number
const PaneIDT	PaneIDT_Unspecified	= -2;

const PaneIDT	PaneIDT_HorizontalScrollBar	= -3;
const PaneIDT	PaneIDT_VerticalScrollBar	= -4;

					// Constants for inRefresh parameter for LPane functions
const bool		Refresh_Yes		= true;
const bool		Refresh_No		= false;

					// Constants for executing AppleEvents
const bool		ExecuteAE_Yes	= true;
const bool		ExecuteAE_No	= false;

					// Constants for recording AppleEvents
const bool		RecordAE_Yes	= true;
const bool		RecordAE_No		= false;

					// Constants for inThrowFail parameters
const bool		Throw_Yes		= true;
const bool		Throw_No		= false;

					// Pascal string with zero length byte
extern const unsigned char	Str_Empty[];

					// Pascal string with 1 character
extern const unsigned char	Str_Dummy[];

extern const Point		Point_00;
extern const Rect		Rect_0000;

const Size		Size_Zero		= 0;
const Handle	Handle_Nil		= 0;
const Ptr		Ptr_Nil			= 0;

PP_End_Namespace_PowerPlant

#if PP_Uses_Pragma_Import
	#pragma import reset
#endif

#endif
