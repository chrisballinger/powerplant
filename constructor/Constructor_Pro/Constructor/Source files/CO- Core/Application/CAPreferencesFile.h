// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	CAPreferencesFile.h			© 1995-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 01/21/95
//     $Date: 2006/01/18 01:32:28 $
//  $History: CAPreferencesFile.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/20/97   Time: 13:19
//	Updated in $/Constructor/Source files/CO- Core/Application
//	Improved commenting.
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

#pragma once

	// Core : Resource manager
//#include "StResourceContext.h"
#include <UResourceMgr.h>

	// PowerPlant : File & stream classes
#include <LPreferencesFile.h>


// ===========================================================================
//		* CAPreferencesFile
// ===========================================================================
//
//	CAPreferencesFile is Constructor's preferences file. When created,
//	it attempts to open a preferences file in the system preferences
//	folder. If that fails, it attempts to create a new (empty)
//	preferences file. The file remains open until this object
//	is deleted (usually when Constructor is closed down).
//
// ===========================================================================

class CAPreferencesFile : public LPreferencesFile {

public:
								CAPreferencesFile(
										ConstStr255Param	inPrefsFileName);
	virtual						~CAPreferencesFile();

	// global accessor

	static CAPreferencesFile&	GetApplicationPrefs();

	// overrides to hide file behaviors

private:
	virtual SInt16				OpenOrCreateResourceFork(
										SInt16				inPrivileges,
										OSType				inCreatorCode,
										OSType				inFileType,
										ScriptCode			inScriptCode);

	virtual SInt16				OpenResourceFork(SInt16		inPrivileges);
	virtual void				CloseResourceFork();


	static CAPreferencesFile*	sApplicationPrefs;

};


// ===========================================================================
//		* StPreferencesContext
// ===========================================================================
//
//	Stack-based class to enter & exit the global preferences
//	file's resource context.
//
// ===========================================================================

class StPreferencesContext : public StResourceContext {

public:
	StPreferencesContext();

};
