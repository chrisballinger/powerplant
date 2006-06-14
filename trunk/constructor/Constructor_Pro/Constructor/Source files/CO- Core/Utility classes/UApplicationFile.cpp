// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	UApplicationFile.cpp		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 02/29/96
//     $Date: 2006/01/18 01:33:27 $
//	$History: UApplicationFile.cpp $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/21/97   Time: 11:16
//	Updated in $/Constructor/Source files/CO- Core/Utility classes
//	Improved commenting.
//	
//	*****************  Version 3   *****************
//	User: Andrew       Date: 02/04/97   Time: 21:07
//	Checked in '$/ConstructorWin/Sources'
//	Comment: Initial merge of changes for MSVC build
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

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "UApplicationFile.h"


// ===========================================================================
//		* Class variables
// ===========================================================================

FSSpec UApplicationFile::sAppFileSpec;


// ===========================================================================

#pragma mark *** UApplicationFile ***

// ---------------------------------------------------------------------------
//		* FindApplicationFile									[static]
// ---------------------------------------------------------------------------
//	Should be called once while the application is starting up.
//	Identifies the application's file location.

void
UApplicationFile::FindApplicationFile()
{
	
	ProcessInfoRec infoRec;
	Str63 name;

	infoRec.processInfoLength = sizeof(ProcessInfoRec);
	infoRec.processName = name;
	infoRec.processAppSpec = &sAppFileSpec;
	
	ProcessSerialNumber PSN;
	PSN.highLongOfPSN = 0;
	PSN.lowLongOfPSN = kCurrentProcess;
	
	ThrowIfOSErr_(::GetProcessInformation(&PSN, &infoRec));

}


// ---------------------------------------------------------------------------
//		* GetApplicationFile									[static]
// ---------------------------------------------------------------------------
//	Call at any time to determine the application's file location.

void
UApplicationFile::GetApplicationFile(
	FSSpec& outAppFileSpec)
{
	outAppFileSpec = sAppFileSpec;
}
