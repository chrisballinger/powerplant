// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	UApplicationFile.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 02/29/96
//     $Date: 2006/01/18 01:33:27 $
//	$History: UApplicationFile.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/21/97   Time: 11:16
//	Updated in $/Constructor/Source files/CO- Core/Utility classes
//	Improved commenting.
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 01/24/97   Time: 17:33
//	Checked in '$/Constructor/Source files/CO- Core/Utility classes'
//	Comment: Fixed CR/LF problem
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 02:10
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

	// PowerPlant : PP headers
#include <PP_Prefix.h>


// ===========================================================================
//		* UApplicationFile
// ===========================================================================
//
//	The UApplicationFile class contains helper functions to find the
//	application's file. It depends on being initialized at startup
//	by having the FindApplicationFile method called.
//
// ===========================================================================

class UApplicationFile {

public:
	static void				FindApplicationFile();
	static void				GetApplicationFile(FSSpec& outAppFileSpec);

protected:
	static FSSpec			sAppFileSpec;

};
