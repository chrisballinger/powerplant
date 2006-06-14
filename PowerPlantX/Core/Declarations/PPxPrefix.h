// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxPrefix.h
//
//	Copyright © 2002-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:10 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxPrefix.h
	@brief		Top-level header file for PowerPlant X.
	
	To ensure that PowerPlant X and compiler options are set properly, the
	first #include for every file should be PPxPrefix.h or some header file
	that #include's this file.												*/

#ifndef H_PPxPrefix
#define H_PPxPrefix
#pragma once

// ---------------------------------------------------------------------------
/** PowerPlant X version number */

#define		PPx_Version		0x01102000	// Version 1.1.0 development


// ---------------------------------------------------------------------------
//	Core Headers

#include <PPxOptions.h>
#include <PPxConstants.h>
#include <PPxExceptions.h>


#endif	// H_PPxPrefix