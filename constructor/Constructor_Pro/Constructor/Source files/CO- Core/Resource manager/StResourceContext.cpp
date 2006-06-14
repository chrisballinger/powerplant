// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	StResourceContext.cpp		© 1994-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 12/19/94
//     $Date: 2006/04/12 08:48:13 $
//	$History: StResourceContext.cpp $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 03/19/97   Time: 11:52
//	Updated in $/Constructor/Source files/CO- Core/Resource manager
//	Fixed bug in Exit that caused the Save() and Exit() pair to fail.
//	Symptom was that an incorrect resource fork would be left on top.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/21/97   Time: 11:16
//	Updated in $/Constructor/Source files/CO- Core/Resource manager
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: Andrew       Date: 2/04/97    Time: 9:06p
//	Updated in $/ConstructorWin/Sources
//	Initial merge of changes for MSVC build
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/23/97   Time: 14:06
//	Updated in $/Constructor/Source files/CO- Core/Resource manager
//	Cleaned up source in preparation for refactoring.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 02:09
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

// ===========================================================================

#include "StResourceContext.h"


// ===========================================================================
//		* Class variables
// ===========================================================================

Int16 StResourceContext::sAppContext = ::CurResFile();
StResourceContext* StResourceContext::sCurrentContext = nil;


// ===========================================================================

#pragma mark *** StResourceContext ***

// ---------------------------------------------------------------------------
//		* StResourceContext
// ---------------------------------------------------------------------------
//	Constructor, specifying the resource context to enter by using a refnum.
//	Destructor will revert to the existing resource context.

StResourceContext::StResourceContext(
	Int16 inContext)
{
	mThisContext = inContext;
	mIsValid = (mThisContext != ResourceContext_NoFile);
	mInContext = false;
	mSavedContext = false;
	mSaveContext = nil;
	Enter();
}


// ---------------------------------------------------------------------------
//		* ~StResourceContext
// ---------------------------------------------------------------------------
//	Make sure that CurResFile is returned to its initial state.

StResourceContext::~StResourceContext()
{
	Exit();
}


// ===========================================================================

#pragma mark -
#pragma mark ** CurResFile changers

// ---------------------------------------------------------------------------
//		* Enter
// ---------------------------------------------------------------------------
//	Enter this resource file's context.

void
StResourceContext::Enter()
{

	// Update current context pointer.
	
	mSaveContext = sCurrentContext;
	sCurrentContext = this;
	mInContext = true;
	mPrevContext = ::CurResFile();

	// Set new context (if necessary).
	
	if (IsValid())
		::UseResFile(mThisContext);

}


// ---------------------------------------------------------------------------
//		* Exit
// ---------------------------------------------------------------------------
//	Leave this resource file's context & go back to whatever we had before.

void
StResourceContext::Exit()
{

	// Update current context pointer.

	if (InContext() && (mSaveContext)) {
		sCurrentContext = mSaveContext;
		mSaveContext = nil;
	}
	
	// If we switched the resource context, switch it back.
	
	if (mSavedContext || (IsValid() && InContext()))
		::UseResFile(mPrevContext);

	// Now remember that we left context.

	mInContext = false;
	
}


// ---------------------------------------------------------------------------
//		* Save
// ---------------------------------------------------------------------------
//	Save the current resource context so when we Exit()
//	the current context is restored.

void
StResourceContext::Save()
{

	// Make sure we don't already have a context sitting around.
	
	Exit();
	
	// Now save the current context.

	mPrevContext = ::CurResFile();
	mThisContext = -1;
	mSavedContext = true;
	mIsValid = false;

}


// ===========================================================================

#pragma mark -
#pragma mark ** status accessors

// ---------------------------------------------------------------------------
//		* CurrentContextIsValid									[static]
// ---------------------------------------------------------------------------

Boolean
StResourceContext::CurrentContextIsValid()
{
	if (!sCurrentContext)
		return false;
	return sCurrentContext->IsValid();
}


// ---------------------------------------------------------------------------
//		* GetCurrentRefNum										[static]
// ---------------------------------------------------------------------------
//	Return the refnum for the current resource file context.

Int16
StResourceContext::GetCurrentRefNum()
{
	if (!sCurrentContext)
		return 0;
	return sCurrentContext->mThisContext;
}
