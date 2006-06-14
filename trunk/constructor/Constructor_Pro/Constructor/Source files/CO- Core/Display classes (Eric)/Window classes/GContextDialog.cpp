// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	GContextDialog.cpp			© 1995-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/15/95
//     $Date: 2006/01/18 01:32:46 $
//	$History: GContextDialog.cpp $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:57
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Window classes
//	Improved commenting.
//	
//	*****************  Version 3   *****************
//	User: Andrew       Date: 02/04/97   Time: 21:01
//	Checked in '$/ConstructorWin/Sources'
//	Comment: Initial merge with MSVC build
//	
//	*****************  Version 2   *****************
//	User: scouten      Date: 01/24/97   Time: 17:24
//	Checked in '$/Constructor/Source files/CO- Core/Display classes (Eric)/Window classes'
//	Comment: Fixed CR/LF problem
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:45
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "GContextDialog.h"

	// Core : Resource manager
//#include "StResourceContext.h"
#include <UResourceMgr.h>


// ===========================================================================

#pragma mark *** GContextDialog ***

// ---------------------------------------------------------------------------
//		* GContextDialog(LStream*)
// ---------------------------------------------------------------------------
//	Stream constructor

GContextDialog::GContextDialog(
	LStream* inStream)

: LDialogBox(inStream)
{
}


// ---------------------------------------------------------------------------
//		* ~GContextDialog
// ---------------------------------------------------------------------------
//	Destructor

GContextDialog::~GContextDialog()
{
}


// ===========================================================================

#pragma mark -
#pragma mark ** overrides to enforce resource context

// ---------------------------------------------------------------------------
//		* Activate
// ---------------------------------------------------------------------------

void
GContextDialog::Activate()
{
	StApplicationContext appContext;
	LWindow::Activate();
}


// ---------------------------------------------------------------------------
//		* Deactivate
// ---------------------------------------------------------------------------

void
GContextDialog::Deactivate()
{
	StApplicationContext appContext;
	LWindow::Deactivate();
}


// ---------------------------------------------------------------------------
//		* UpdatePort
// ---------------------------------------------------------------------------

void
GContextDialog::UpdatePort()
{
	StApplicationContext appContext;
	LWindow::UpdatePort();
}


// ---------------------------------------------------------------------------
//		* HandleClick
// ---------------------------------------------------------------------------

void
GContextDialog::HandleClick(
	const EventRecord&	inMacEvent,
	SInt16				inPart)
{
	StApplicationContext appContext;
	LWindow::HandleClick(inMacEvent, inPart);
}
