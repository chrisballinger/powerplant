// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	Constructor.file.h			© 1995-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//	Description of Constructor files on disk
//
//   Created: 03/30/95
//     $Date: 2006/01/18 01:32:26 $
//	$History: Constructor.file.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/28/97   Time: 10:44
//	Updated in $/Constructor/Source files/CO- Core/00- Headers
//	Added Pilot file types.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/18/97   Time: 18:17
//	Updated in $/Constructor/Source files/CO- Core/00- Headers
//	Improved commenting.
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 01/24/97   Time: 17:18
//	Checked in '$/Constructor/Source files/CO- Core/00- Headers'
//	Comment: Fixed CR/LF problem
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:28
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once


// ===========================================================================
//		* File type codes
// ===========================================================================

#if Constructor_ForJava
	const OSType Type_CreatorCode	= 'MWCJ';
#elif Constructor_ForPilot
	const OSType Type_CreatorCode	= 'MWCP';
#else
	const OSType Type_CreatorCode	= 'MWC2';
#endif

const OSType Type_PilotDocument		= 'PLob';
const OSType Type_JavaDocument		= 'JPob';
const OSType Type_MacOSDocument		= 'rsrc';
const OSType Type_Preferences		= 'pref';
const OSType Type_Catalog			= 'CATF';
