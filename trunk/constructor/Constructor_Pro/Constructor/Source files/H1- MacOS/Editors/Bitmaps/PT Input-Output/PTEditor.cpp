// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PTEditor.cp								©1995-96 Metrowerks Inc. All rights reserved.
// ===========================================================================

/*
	96.11.26	ebs		rewrote for CW11 support
	96.08.28	ebs		support for locked files
	96.08.05	ebs		support for cmd_PaintWindowSwap
	96.08.04	ebs		checked changed flag in CollectChangesSelf so the file isn't
							marked dirty after closing an unchanged window.
						removed some unused RegisterClass() calls
	96.07.31	ebs		SICN support
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "PT_Headers.h"

#include "SUColorPane.h"
#include "SUPatternPane.h"
#include "SUErrors.h"

#include "PTFamilyView.h"
#include "PTCursorView.h"
#include "PTCIconView.h"
#include "PTPictureView.h"
#include "PTPatternView.h"
#include "PTTargetBox.h"
#include "PTDraggableTargetBox.h"
#include "PTPatternTargetBox.h"
#include "PTResizeImageAction.h"
#include "PTEditor.h"

#include "REEditor.h"
#include "DMObject.h"
#include "DMReanimator.h"
#include "LWindowPositioner.h"
#include "RESession.h"
#include "RFResource.h"
//#include "StResourceContext.h"
#include <UResourceMgr.h>

/*=================================
	Constructor
===================================*/
PTEditor::PTEditor( LCommander* inSuper, RESession& inSession)
	: REEditor(inSuper, inSession)
{
	// Validate pointers.
	ValidateObject_(inSuper);
	ValidateObject_(&inSession);

	// Register Txtr editor classes.
	RegisterPTClasses();

	// is this needed ???
	PostAction(nil);
	
	mPaintView = nil;
	mFileIsLocked = false;
}

/*=================================
	Destructor
===================================*/
PTEditor::~PTEditor()
{
}

/*===============================================
	ObeyCommand
	
	Description:
	Just listens for a message from the paint view that something has
	  been modified and relays that message to other classes.
================================================*/
Boolean PTEditor::ObeyCommand( CommandT inCommand, void *ioParam )
{
	switch( inCommand )
	{
		case cmd_PaintModified:
			if ( ioParam != NULL )
				SetModified();
			return( true );
	}
	
	return( REEditor::ObeyCommand( inCommand, ioParam ) );
}

/*===============================================
	EnableSelf
	
	Description:
	Called when the file becomes unlocked or during initialization.
	May be called before the paint window is open.
================================================*/
void PTEditor::EnableSelf()
{	
	if ( mFileIsLocked )
	{
		mFileIsLocked = false;

		if ( mPaintView )
			mPaintView->SetLockFlag( false );
	}
}

/*==================================
	DisableSelf
	
	Note: 
	Called when the file is locked (during our initialization).
	May be called before the paint window is open, so just save
	  the value passed in.
====================================*/
void PTEditor::DisableSelf()
{
	mFileIsLocked = true;
}


// ---------------------------------------------------------------------------
//		* IsModifiedSelf										[protected]
// ---------------------------------------------------------------------------
//	Override hook. Return true if there are unsaved changes to the resource.
//	GHD Bug fix

Boolean
PTEditor::IsModifiedSelf()
{
	return ( mPaintView && mPaintView->GetChangedFlag() );
}


/*===============================================
	ResetModifiedSelf
	
	Called when the file has been successfully saved.
================================================*/
void PTEditor::ResetModifiedSelf()
{
	if ( mPaintView && mPaintView->GetChangedFlag() )
		mPaintView->SetChangedFlag( false );
}


/*=================================
	OpenEditorWindowSelf
===================================*/
LWindow* PTEditor::OpenEditorWindowSelf()
{
	// Validate pointers.
	ValidateThis_();

	// Create the window.
	StApplicationContext appContext;		// can't hurt

	LWindow	*theWindow = nil;
	
	try
	{
		RFResource *rf = dynamic_cast<RFResource*>(mPrimaryResource);
		ValidateObject_(rf);
		
		switch( rf->GetResType() )
		{
			case ImageType_LargeIcon:
				theWindow = PTFamilyView::OpenPaintWindow( PPob_ICONEditor, 
											&(rf->GetRFMap()), 
											rf->GetResID() );
				break;
			
#if !Constructor_ForPilot			
				// icon family
			case ImageType_LargeIconWithMask:
			case ImageType_Large4BitIcon:
			case ImageType_Large8BitIcon:
			case ImageType_SmallIconWithMask:
			case ImageType_Small4BitIcon:
			case ImageType_Small8BitIcon:
			case ImageType_MiniIconWithMask:
			case ImageType_Mini4BitIcon:
			case ImageType_Mini8BitIcon:
				theWindow = PTFamilyView::OpenPaintWindow( PPob_IconSuiteEditor,
											&(rf->GetRFMap()), 
											rf->GetResID() );
				break;
				
			case ImageType_ColorIcon:
				theWindow = PTCIconView::OpenPaintWindow(  PPob_CICNEditor,
											&(rf->GetRFMap()), 
											rf->GetResID() );
				break;
				
			case ImageType_Cursor:
				theWindow = PTCursorView::OpenPaintWindow( PPob_CursorEditor,
											&(rf->GetRFMap()), ImageType_Cursor,
											rf->GetResID() );
				break;

			case ImageType_ColorCursor:
				theWindow = PTCursorView::OpenPaintWindow( PPob_ColorCursorEditor,
											&(rf->GetRFMap()), ImageType_ColorCursor,
											rf->GetResID() );
				break;
				
			case ImageType_Pattern:
				theWindow = PTPatternView::OpenPaintWindow( PPob_PatternEditor,
											&(rf->GetRFMap()), ImageType_Pattern,
											rf->GetResID() );
				break;
				
			case ImageType_PixPat:
				theWindow = PTPatternView::OpenPaintWindow( PPob_PixPatEditor,
											&(rf->GetRFMap()), ImageType_PixPat,
											rf->GetResID() );
				break;
#endif
				
			case ImageType_Picture:
				theWindow = PTPictureView::OpenPaintWindow( PPob_PictEditor,
											&(rf->GetRFMap()), rf->GetResID() );
				break;
		}

		ThrowIfNil_( theWindow );
		ValidateObject_(theWindow);
		
			// Set the window title to the resource name.
		LStr255 title;
		mSession.GetWindowTitleForEditor(this, title);
		theWindow->SetDescriptor(title);

			// save off the paint view
		mPaintView = dynamic_cast<PTPaintView*>( theWindow );
		ThrowIfNil_( mPaintView );
		
			// if the file is locked, the paint view needs to know
		if ( mFileIsLocked && mPaintView )
			mPaintView->SetLockFlag( true );
					
			// Position & show the window.
		theWindow->ProcessCommand(cmd_PositionWindow);
		theWindow->Show();
	}
	catch (const LException& inErr )
	{
		if ((inErr.GetErrorCode() >= 9000) && (inErr.GetErrorCode() <= 9099))
			SUErrors::DisplayError( inErr.GetErrorCode() );			// remove this
		throw;
	}
	catch( SInt32 theErr )
	{
		if ((theErr >= 9000) && (theErr <= 9099))
			SUErrors::DisplayError( theErr );			// remove this
		throw;
	}
	
	return theWindow;
}

/*===============================================
	ReadResourceDataSelf
	
	Note: we do all of the work in OpenEditorWindowSelf rather than here.
================================================*/
void PTEditor::ReadResourceDataSelf()
{
}

/*===============================================
	CloseEditorWindowSelf
================================================*/
void PTEditor::CloseEditorWindowSelf()
{
	mPaintView = nil;
}

/*===============================================
	WriteResourceDataSelf
================================================*/
void PTEditor::WriteResourceDataSelf()
{
		// Validate pointers.
	ValidateThis_();
	ValidateSimpleObject_(mPrimaryResource);
	
	try
	{
		RFResource *rf = dynamic_cast<RFResource*>(mPrimaryResource);
		ValidateObject_(rf);

		if ( mPaintView )
			mPaintView->SaveAsResource( &rf->GetRFMap(), rf->GetResID() );
	}
	catch(const LException& inErr )
	{
		if ((inErr.GetErrorCode() >= 9000) && (inErr.GetErrorCode() <= 9099))
			SUErrors::DisplayError( inErr.GetErrorCode() );			// remove this
		throw;
	}
	catch( SInt32 theErr )
	{
		if ((theErr >= 9000) && (theErr <= 9099))
			SUErrors::DisplayError( theErr );			// remove this
		throw;
	}
}


/*===============================================
	RegisterClasses
================================================*/
void PTEditor::RegisterPTClasses()
{
	static Boolean sInitialized = false;
	
	if ( sInitialized ) return;
	
		// main paint views
	URegistrar::RegisterClass ( PTFamilyView::class_ID, (ClassCreatorFunc) PTFamilyView::CreateFromStream );

#if !Constructor_ForPilot

	URegistrar::RegisterClass ( PTCIconView::class_ID, (ClassCreatorFunc) PTCIconView::CreatePaintCIconStream );
	URegistrar::RegisterClass ( PTCursorView::class_ID, (ClassCreatorFunc) PTCursorView::CreateFromStream );
	URegistrar::RegisterClass ( PTPatternView::class_ID, (ClassCreatorFunc) PTPatternView::CreateFromStream );
#endif
	URegistrar::RegisterClass ( PTPictureView::class_ID, (ClassCreatorFunc) PTPictureView::CreateFromStream );		
		// helpers
	URegistrar::RegisterClass ( PTCanvas::class_ID, (ClassCreatorFunc) PTCanvas::CreatePaintCanvasStream );
	URegistrar::RegisterClass ( SUColorPane::class_ID, (ClassCreatorFunc) SUColorPane::CreateFromStream );
	URegistrar::RegisterClass ( SUPatternPane::class_ID, (ClassCreatorFunc) SUPatternPane::CreateFromStream );
		
		// target boxes
	URegistrar::RegisterClass( PTTargetBox::class_ID, (ClassCreatorFunc) PTTargetBox::CreateFromStream );
	URegistrar::RegisterClass( PTDraggableTargetBox::class_ID, (ClassCreatorFunc) PTDraggableTargetBox::CreateFromStream );
	URegistrar::RegisterClass( PTPatternTargetBox::class_ID, (ClassCreatorFunc) PTPatternTargetBox::CreateFromStream );
	
	sInitialized = true;
}

