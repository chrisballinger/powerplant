// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	CAPreferencesFile.cpp		© 1995-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 01/21/95
//     $Date: 2006/01/18 01:32:28 $
//  $History: CAPreferencesFile.cpp $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 13:19
//	Updated in $/Constructor/Source files/CO- Core/Application
//	Improved commenting.
//
//	*****************  Version 3   *****************
//	User: Andrew       Date: 02/04/97   Time: 11:46
//	Checked in '$/ConstructorWin/Sources'
//	Comment: Added MSVC-hosted prefix file
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 01/24/97   Time: 17:18
//	Checked in '$/Constructor/Source files/CO- Core/Application'
//	Comment: Fixed CR/LF problem
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:28
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "CAPreferencesFile.h"

	// Core : Headers
#include "Constructor.file.h"


// ===========================================================================
//		* Class variables
// ===========================================================================

CAPreferencesFile* CAPreferencesFile::sApplicationPrefs = nil;


// ===========================================================================

#pragma mark *** CAPreferencesFile ***

// ---------------------------------------------------------------------------
//		* CAPreferencesFile
// ---------------------------------------------------------------------------
//	Constructor

CAPreferencesFile::CAPreferencesFile(
	ConstStr255Param inPrefsFileName)

: LPreferencesFile(inPrefsFileName, true)

{

	// If this is the first prefs file opened, it belongs to the app.
	
	if (sApplicationPrefs == nil)
		sApplicationPrefs = this;

	// Open the resource file, but be sure to save old ref num.

	StResourceContext saveContext;
	saveContext.Save();

	try {
		OpenOrCreateResourceFork(fsRdWrPerm, Type_CreatorCode, Type_Preferences, smRoman);
	}
	catch(...) {
		// Couldn't open prefs file. Oh, well...
	}
}


// ---------------------------------------------------------------------------
//		* ~CAPreferencesFile
// ---------------------------------------------------------------------------
//	Destructor

CAPreferencesFile::~CAPreferencesFile()
{
}


// ---------------------------------------------------------------------------
//		* GetApplicationPrefs									[static]
// ---------------------------------------------------------------------------
//	Return a pointer to the preferences file object.

CAPreferencesFile&
CAPreferencesFile::GetApplicationPrefs()
{
	return *sApplicationPrefs;
}


// ===========================================================================
//	Access to these methods is private so that we don't inadvertantly bypass the
//	EnterModuleContext / ExitModuleContext methods.
// ===========================================================================

// ---------------------------------------------------------------------------
//		* OpenOrCreateResourceFork								[private]
// ---------------------------------------------------------------------------

SInt16
CAPreferencesFile::OpenOrCreateResourceFork(
	SInt16		inPrivileges,
	OSType		inCreator,
	OSType		inFileType,
	ScriptCode	inScriptCode)
{
	return LPreferencesFile::OpenOrCreateResourceFork(inPrivileges, inCreator, inFileType, inScriptCode);
}


// ---------------------------------------------------------------------------
//		* OpenResourceFork										[private]
// ---------------------------------------------------------------------------

SInt16
CAPreferencesFile::OpenResourceFork(
	SInt16 inPrivileges)
{
	return LFile::OpenResourceFork(inPrivileges);
}


// ---------------------------------------------------------------------------
//		* CloseResourceFork										[private]
// ---------------------------------------------------------------------------

void
CAPreferencesFile::CloseResourceFork()
{
	LFile::CloseResourceFork();
}


// ===========================================================================

#pragma mark -
#pragma mark *** StPreferencesContext ***

// ---------------------------------------------------------------------------
//		* StPreferencesContext()
// ---------------------------------------------------------------------------
//	Constructor. Sets CurResFile to preferences file. Destructor will restore
//	to previous file.

StPreferencesContext::StPreferencesContext()

: StResourceContext(CAPreferencesFile::GetApplicationPrefs().GetResourceForkRefNum())

{
}
