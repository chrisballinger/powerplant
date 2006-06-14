// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxOptions.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:10 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxOptions.h
	@brief		Conditional compilation options
*/

//	The basic format of most of the options is:
//
//		#ifndef Option_Name
//			#define Option_Name		Default_Value
//		#endif
//
//		#if Option_Name
//			// Define symbols for the option being ON
//		#else
//			// Define symbols for the option being OFF
//		#endif
//
//	If you wish to set the option yourself, you should place the appropriate
//	#define of the Option_Name in a header file that gets #include'd before
//	this one, normally your project prefix file or precompiled header.

#ifndef H_PPxOptions
#define H_PPxOptions
#pragma once

// ---------------------------------------------------------------------------
/**	Master Debugging Switch */

#ifndef PPx_Debug
	#define PPx_Debug					1
#endif


// ---------------------------------------------------------------------------
/**	Debugging Exceptions */

#ifndef PPx_Debug_Exceptions
	#define	PPx_Debug_Exceptions		PPx_Debug
#endif


// ---------------------------------------------------------------------------
/**	Debugging Signals */

#ifndef PPx_Debug_Signals
	#define PPx_Debug_Signals			PPx_Debug
#endif


// ---------------------------------------------------------------------------
/**	Verifying Parameters */

#ifndef PPx_Verify_Parameters
	#define PPx_Verify_Parameters		PPx_Debug
#endif


// ---------------------------------------------------------------------------
/**	Apple Debugging Utilities (see Debugging.h) */

#ifndef DEBUG
	#define	DEBUG						PPx_Debug
#endif

#endif	// H_PPxOptions