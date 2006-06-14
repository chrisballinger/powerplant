// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	CAGenericTarget.h			© 1994-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 08/05/94
//     $Date: 2006/01/18 01:32:28 $
//	$History: CAGenericTarget.h $
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 03/06/97   Time: 11:42
//	Updated in $/Constructor/Source files/CO- Core/Application
//	Finished conversion to PDDocument class.
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 03/06/97   Time: 11:38
//	Updated in $/Constructor/Source files/CO- Core/Application
//	Changed return type of MakeEmptyProject and MakeProjectFromFile from
//	RSDocument* to PDDocument*.
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 02/01/97   Time: 14:08
//	Updated in $/Constructor/Source files/CO- Core/Application
//	Added Windows application object.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 12/05/96   Time: 19:20
//	Updated in $/Constructor/Source files/Application
//	Removed Display Classes command.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 11/30/96   Time: 17:25
//	Updated in $/Constructor/Source files/Application
//	Added catalog to RememberOpenWindows.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/27/96   Time: 09:17
//	Updated in $/Constructor/Source files/Application
//	Removed catalog document behaviors. Added support for new catalog
//	window.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/24/96   Time: 23:29
//	Updated in $/Constructor/Source files/Application
//	Finished support for alignment palette.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/13/96   Time: 10:34
//	Updated in $/Constructor/Source files/Application
//	Moved OpenDisplayClassWindow from target-specific application objects
//	to CAGenericTarget.
//
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:20
//	Created
//	Comment: Generic application (common code to all targets)
//
// ===========================================================================

#pragma once

	// PowerPlant : Commander classes
#include <LDocApplication.h>

#if Constructor_Debug
	#include <LDebugMenuAttachment.h>
#endif

// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class LWindow;
class PDDocument;


// ===========================================================================
//		* CAGenericTarget										[abstract]
// ===========================================================================
//
//	CAGenericTarget is the main Constructor application class.
//	It contains default behavior for file opening and global windows.
//	It provides simple override hooks for target-specific behaviors.
//
// ===========================================================================

class CAGenericTarget : public LDocApplication {

public:
							CAGenericTarget();
	virtual					~CAGenericTarget();

	// menu commands

	virtual void			FindCommandStatus(
									CommandT			inCommand,
									Boolean&			outEnabled,
									Boolean&			outUsesMark,
									UInt16&				outMark,
									Str255				outName);
	virtual Boolean			ObeyCommand(
									CommandT			inCommand,
									void*				ioParam);

	// initialization behaviors
	
	virtual void			Run();
	virtual void			MakeMenuBar();
/*
	// debugging behaviors
	
	virtual void			UseIdleTime(
									const EventRecord&	inMacEvent);
*/

	// low-memory overrides

	virtual void			DispatchEvent(
									const EventRecord&	inMacEvent);
	virtual void			SendAEQuit();

	// new file behaviors
	
protected:
	virtual LModelObject*	MakeNewDocument();

	// opening existing files

	virtual void			SendAEOpenDoc(FSSpec&		inFileSpec);
	virtual void			OpenDocument(FSSpec*		inMacFSSpec);

	virtual void			DoStdFileOpenDialog(void);
	
	virtual	void			DoCantOpenFileDialog(
									ConstStringPtr		inFilename,
									ExceptionCode		inError );
																	
	// project file behaviors
	
	virtual PDDocument*		MakeEmptyProject() = 0;
	virtual PDDocument*		MakeProjectFromFile(
									const FSSpec&		inFileSpec) = 0;

	virtual void			ChooseDocument() = 0;

	// resource type list behaviors
	
	virtual void			MakeResTypeList();

	// display class list behaviors
	
	virtual void			MakeTypeList() = 0;

	// global windows
	
	virtual void			RememberOpenWindows();
	virtual void			RememberOpenWindowsSelf(
									LStream&			inCommandStream);

	virtual void			ReopenGlobalWindows();
	
	virtual Boolean			AllowSubRemoval(
									LCommander*			inSub);

	// about box

	virtual void			ShowAboutBox();


	// data members

protected:
	
	virtual	void			Initialize();

	LWindow*				mAlignmentPalette;				// global alignment palette window
	
	#if Constructor_Debug
		LDebugMenuAttachment	*mDebugMenuAttachment;
	#endif

	virtual void		CatchException( const LException& inException );

};
