// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*
 *	File Name:			Version.h
 *	File Purpose:		Common repository for build numbers to be used in Version.r
 *						and PropertyList.plc
 *
 *	Author:				Isaac Wankerl
 *	Last Revision:		5/4/2001
 *
 *  Copyright © 2001 Metrowerks Corporation.  All rights reserved.
 */

#ifndef __PLIST__
	// Only do this stuff if using the Rez compiler, not the Property List compiler
	#include "MacTypes.r"
	
	#define BUILD_JAPANESE		0
	
	#define MAJORREVISION		2
	#define MINORREVISION		5
	#define MINORFIX			8
	#define RELEASETYPE			release
	#define RELEASEVERSION		0
#endif

#define VERSION_STRING		"2.5.8"

#define LONG_VERSION_STRING	"v" VERSION_STRING ", a Metrowerks CodeWarrior Component"
#define COPYRIGHT_STRING	"© 1993-2005 Metrowerks Corporation"
#define GET_INFO_STRING		"Constructor " LONG_VERSION_STRING ", " COPYRIGHT_STRING

// icw -- Note that the 'TEXT' (1450) in AboutCarbon.rsrc needs a copyright date update.
// JWW -- Note that the 'TEXT' (1450) in AboutClassic.rsrc needs a copyright date update, too. :-)