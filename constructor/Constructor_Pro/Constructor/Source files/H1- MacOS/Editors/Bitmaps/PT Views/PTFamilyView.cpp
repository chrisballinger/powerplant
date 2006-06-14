// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				PaintFamilyView.cp

PROJECT:			Constructor Icon Editor

CLASSES:			PTFamilyView

DESCRIPTION:		This code is for painting ICON resources
					
AUTHOR:				Eric Shapiro

CREATION DATE :		96.6.8

CHANGE HISTORY :

		97.02.21	ebs		SaveAsResource rewritten to be compatible with new
							resource code. 
		96.11.26	ebs		Add include RFResource.h (CW11)
		96.08.07	ebs		Removed call to GetDecentZoomFactor (routine is gone now).
							Added GetZoomFactor() method instead.
		96.08.05	ebs		Support for mSamplePaneList
		96.08.03	ebs		Support for "unused" flag in target panes
					ebs		Changed default pane to the first one with an image
		96.07.22	ebs		New target boxes
		96.6.8		ebs		Started

---------------------------------------<< ¥ >>-----------------------------------------
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "PT_Headers.h"
#include "PTFamilyView.h"
#include "SUSaveResFile.h"
#include "SUColorUtils.h"
#include "PTDraggableTargetBox.h"
#include "PTGraphicConversions.h"
#include "SUFileUtils.h"

#include "RFMap.h"
#include "RFResource.h"

/*====================================
	OpenPaintWindow
=====================================*/
PTFamilyView* PTFamilyView::OpenPaintWindow( ResIDT inPPobID, RFMap *inMap, ResIDT inResID )
{
	PTFamilyView		*theView = nil;

	try
	{
		theView = (PTFamilyView*) PTFamilyView::CreatePaintWindow( inPPobID );
		theView->InitializeFromResource( inMap, inResID );
		
		#ifdef ICON_STAND_ALONE
		theView->Show();
		#endif
	}
	catch( ... )
	{
		delete theView;
		throw;
	}
	
	return( theView );
}

/*====================================
	Constructor
=====================================*/
PTFamilyView::PTFamilyView( LStream *inStream ) : PTPaintView( inStream )
{
		// our refCon field has the resource id of our family information
	ResIDT infoID = this->GetUserCon();
	mFamilyListH = (PaintFamilyListH) SUFileUtils::GetAppResource( resType_PaintFamilyInfo, infoID );
	::DetachResource( (Handle) mFamilyListH );
}

/*====================================
	Destructor
=====================================*/
PTFamilyView::~PTFamilyView()
{
	SUMiscUtils::DisposeHandle( mFamilyListH );
}

/*====================================
	CreateFamilyView
=====================================*/
PTFamilyView* PTFamilyView::CreateFromStream( LStream *inStream )
{
	return new PTFamilyView( inStream );
}

/*====================================
	FinishCreateSelf
=====================================*/
void PTFamilyView::FinishCreateSelf()
{
	PTPaintView::FinishCreateSelf();
	
	#ifdef EBS_OUT
		// default to 8-bit large color icon
	if ( (**mFamilyListH).defaultPane )
	{
		mCurrentSamplePane = (PTDraggableTargetBox*) this->FindPaneByID( (**mFamilyListH).defaultPane );
		ThrowIfNil_( mCurrentSamplePane );
		mCurrentSamplePane->SetTarget( true, redraw_Dont );
	}
	#endif
}

/*====================================
	InitializeFromResource
=====================================*/
void PTFamilyView::InitializeFromResource( RFMap *inMap, ResIDT inResID )
{
	StSaveGWorld		aSaver2;
	SInt32				numTypes = GetFamilyMemberCount();
	
	/*********************************************
		initialize each family member 
	**********************************************/
	for ( SInt32 count = 1; count <= numTypes; count++ )
	{
		PaintFamilyMember	memberInfo;
		
		GetFamilyMemberInfo( count, &memberInfo );
		
			// initialize the main image
		Boolean hasImage = this->InitializeOneMember( 
									inMap, memberInfo.resourceType, inResID, 
									memberInfo.width, memberInfo.height, memberInfo.depth,
									0, memberInfo.rowBytes, memberInfo.samplePaneID, false );
			
			// initialize the mask (ICN#, etc)
		if ( memberInfo.maskRowBytes != 0 )
			this->InitializeOneMember( inMap, memberInfo.resourceType, inResID,
									memberInfo.width, memberInfo.height, 1,
									memberInfo.maskOffset, memberInfo.maskRowBytes, 
									memberInfo.maskSamplePaneID, true );

		/*********************************************
			default to the first non-empty resource we find
		*********************************************/
		if ( hasImage && !mCurrentSamplePane )
			mCurrentSamplePane = (PTDraggableTargetBox*) this->FindPaneByID( memberInfo.samplePaneID );
	}

	/*********************************************
		need to listen to the sample panes (drag & drop, etc)
	*********************************************/
	this->BecomeListenerTo( mNumSamplePanes, mSamplePaneList );

	/*********************************************
		if the entire family is empty, default to whichever target pane
		is indicated by the resource
	*********************************************/
	if ( !mCurrentSamplePane )
	{
		mCurrentSamplePane = (PTDraggableTargetBox*) this->FindPaneByID( (**mFamilyListH).defaultPane );
		ThrowIfNil_( mCurrentSamplePane );
	}
	
	mCurrentSamplePane->SetTarget( true, redraw_Dont );
	this->SetImage( mCurrentSamplePane->GetBuffer(), PTResize_Canvas );
}

/*====================================
	InitializeOneMember
=====================================*/
Boolean PTFamilyView::InitializeOneMember( RFMap *inMap, ResType inResType, ResIDT inResID,
											SInt32 inWidth, SInt32 inHeight, SInt32 inDepth,
											SInt32 inOffset, SInt32 inRowBytes,
											PaneIDT inSamplePaneID, Boolean /* inIsMask */ )
{
	StSaveResFile 	aSaver;
	SUOffscreen		*theBuffer = nil;
	Boolean			isUsed = false;
	
	/*
		icon families use the standard color table for their depth
	*/
	CTabHandle		theTable = SUColorUtils::GetColorTable( inDepth );

	try
	{
		/******************************************
			create the offscreen buffer
		*******************************************/
		theBuffer = SUOffscreen::CreateBuffer( inWidth, inHeight, inDepth, theTable );
		
		/******************************************
			if we have a resource, load the data into the offscreen buffer
		*******************************************/
		RFResource *theResource = inMap->FindResource( inResType, inResID, false );
		if ( theResource )
		{
			Handle h = theResource->GetResData();
			if ( h )
			{
				::HLock( h );
				theBuffer->CopyFromRawData( (UInt8*) *h + inOffset, inRowBytes );
				SUMiscUtils::DisposeHandle( h );
				isUsed = true;
			}
		}

		/******************************************
			initialize the sample pane
		*******************************************/
		PTDraggableTargetBox *thePane = (PTDraggableTargetBox*) this->FindPaneByID( inSamplePaneID );
		ThrowIfNil_( thePane );
		
		mSamplePaneList[ mNumSamplePanes++ ] = thePane;		
		thePane->SetUsedFlag( isUsed, redraw_Dont );		// true if the resource exists
		thePane->SetBuffer( theBuffer, redraw_Dont );
		theBuffer = nil;									// so we don't delete it below
	}
	catch( ... )
	{
		if ( theBuffer )
			delete theBuffer;
		if ( theTable )
			::DisposeCTable( theTable );
		throw;
	}
	
	if ( theTable )
		::DisposeCTable( theTable );
		
	return( isUsed );
}


/*===============================================
	SaveAsResource
	
	Note:
	This has changed in post CW11. We can no longer delete all of the resources
	  within an icon suite, even temporarily within this routine. Thus we loop
	  twice, first creating and later deleting them. If they are all gone, we'll
	  create an ICN# anyway because otherwise our view itself is deallocated by
	  the resource listeners.
================================================*/
void PTFamilyView::SaveAsResource( RFMap *inMap, ResIDT inResID )
{
	PaintFamilyMember		memberInfo;
	SInt32					numIcons = this->GetFamilyMemberCount();
	PTDraggableTargetBox	*mainPane, *maskPane;
	SUOffscreen				*mainBuffer, *maskBuffer;

		// post CW11 stuff
	SInt32					numActiveImages = 0;
	SUOffscreen				*icnBuffer = nil, *icnMaskBuffer = nil;
	PaintFamilyMember		icnInfo;
	PTDraggableTargetBox	*icnPane = nil;
	
	/*************************************
		first create all of the resources we need to
	**************************************/
	SInt32 count;
	for ( count = 1; count <= numIcons; count++ )
	{
		this->GetFamilyMemberInfo( count, &memberInfo );
		
		/*************************************
			find the main buffer
		**************************************/
		mainPane = (PTDraggableTargetBox*) this->FindPaneByID( memberInfo.samplePaneID );
		ThrowIfNil_( mainPane );
			
		mainBuffer = mainPane->GetBuffer();
		ThrowIfNil_( mainBuffer );
		
		/*************************************
			find the mask buffer (if any)
		**************************************/
		if ( memberInfo.maskRowBytes == 0 )
		{
			maskPane = nil;
			maskBuffer = nil;
		}
		else
		{
			maskPane = (PTDraggableTargetBox*) this->FindPaneByID( memberInfo.maskSamplePaneID );
			ThrowIfNil_( maskPane );
				
			maskBuffer = maskPane->GetBuffer();
			ThrowIfNil_( maskBuffer );
		}
		
		/*************************************
			keep track of the ICN# one in case all others are deleted
		**************************************/
		if ( memberInfo.samplePaneID == static_cast<SInt32>(ImageType_LargeIconWithMask) )
		{
			icnInfo = memberInfo;
			icnPane = mainPane;
			icnBuffer = mainBuffer;
			icnMaskBuffer = maskBuffer;
		}
		
		/*************************************
			create whatever resources are used
		**************************************/
		if ( mainPane->IsUsed() /*|| (maskPane && maskPane->IsUsed())*/ )
		{
			PTGraphicConversions::SaveOffscreenAsResource( 
								inMap, memberInfo.resourceType, inResID, 
								mainBuffer, memberInfo.rowBytes, 
								maskBuffer, memberInfo.maskOffset, 
								memberInfo.maskRowBytes );
			++numActiveImages;
		}
	}	
	
	/*************************************
		if no active images, we still need to create the ICN# or very
		bad things happen (our window gets disposed of).
	**************************************/
	if ( (numActiveImages == 0) && icnPane && icnBuffer )
	{
		PTGraphicConversions::SaveOffscreenAsResource( 
							inMap, ImageType_LargeIconWithMask, inResID, 
							icnBuffer, icnInfo.rowBytes, 
							icnMaskBuffer, icnInfo.maskOffset, 
							icnInfo.maskRowBytes );
	}
	
	/*************************************
		now delete all of the unused ones
	**************************************/
	for ( count = 1; count <= numIcons; count++ )
	{
		this->GetFamilyMemberInfo( count, &memberInfo );
		
		/*************************************
			find the main buffer
		**************************************/
		mainPane = (PTDraggableTargetBox*) this->FindPaneByID( memberInfo.samplePaneID );
		ThrowIfNil_( mainPane );
			
		mainBuffer = mainPane->GetBuffer();
		ThrowIfNil_( mainBuffer );
		
		/*************************************
			find the mask buffer (if any)
		**************************************/
		if ( memberInfo.maskRowBytes == 0 )
		{
			maskPane = nil;
			maskBuffer = nil;
		}
		else
		{
			maskPane = (PTDraggableTargetBox*) this->FindPaneByID( memberInfo.maskSamplePaneID );
			ThrowIfNil_( maskPane );
				
			maskBuffer = maskPane->GetBuffer();
			ThrowIfNil_( maskBuffer );
		}
		
		if ( !mainPane->IsUsed() && ((numActiveImages > 0) || (memberInfo.resourceType != ImageType_LargeIconWithMask)) )
		{
			RFResource *theResource = inMap->FindResource( memberInfo.resourceType, inResID, false );
			if ( theResource )
				theResource->DeleteResource();
		}
	}	

	this->SetChangedFlag( false );
}

#ifdef PRE_CW11_VERSION_DONT_USE
/*===============================================
	SaveAsResource
================================================*/
void PTFamilyView::SaveAsResource( RFMap *inMap, ResIDT inResID )
{
	PaintFamilyMember		memberInfo;
	Int32					numIcons = this->GetFamilyMemberCount();
	PTDraggableTargetBox	*mainPane, *maskPane;
	SUOffscreen				*mainBuffer, *maskBuffer;
	
	for ( Int32 count = 1; count <= numIcons; count++ )
	{
		this->GetFamilyMemberInfo( count, &memberInfo );
		
		/*************************************
			find the main buffer
		**************************************/
		mainPane = (PTDraggableTargetBox*) this->FindPaneByID( memberInfo.samplePaneID );
		ThrowIfNil_( mainPane );
			
		mainBuffer = mainPane->GetBuffer();
		ThrowIfNil_( mainBuffer );
		
		/*************************************
			find the mask buffer (if any)
		**************************************/
		if ( memberInfo.maskRowBytes == 0 )
		{
			maskPane = nil;
			maskBuffer = nil;
		}
		else
		{
			maskPane = (PTDraggableTargetBox*) this->FindPaneByID( memberInfo.maskSamplePaneID );
			ThrowIfNil_( maskPane );
				
			maskBuffer = maskPane->GetBuffer();
			ThrowIfNil_( maskBuffer );
		}
		
		if ( mainPane->IsUsed() /*|| (maskPane && maskPane->IsUsed())*/ )
		{
			PTGraphicConversions::SaveOffscreenAsResource( 
								inMap, memberInfo.resourceType, inResID, 
								mainBuffer, memberInfo.rowBytes, 
								maskBuffer, memberInfo.maskOffset, 
								memberInfo.maskRowBytes );
		}
		else			// delete it if it's unused
		{
			RFResource *theResource = inMap->FindResource( memberInfo.resourceType, inResID, false );
			if ( theResource )
				theResource->DeleteResource();
		}
	}	
	
	this->SetChangedFlag( false );
}
#endif

/*====================================
	GetFamilyMemberCount
=====================================*/
SInt32 PTFamilyView::GetFamilyMemberCount()
{
	return ( (**mFamilyListH).numEntries );
}

/*====================================
	GetFamilyMemberInfo
	
	index is 1..n
=====================================*/
void PTFamilyView::GetFamilyMemberInfo( SInt32 index, PaintFamilyMember *oRec )
{
	ThrowIf_( (index <= 0) || (index > (**mFamilyListH).numEntries) );
	*oRec = (**mFamilyListH).members[ index - 1 ];
}

/*====================================
	GetZoomFactor

	Description:
	Icon suite images are always zoomed the same amount -- for a 32x32 image.
	Otherwise, clicking on different target panes would resize the canvas 
	  and change the zoom factor and that's not very pretty.
=====================================*/
SInt32 PTFamilyView::GetZoomFactor( SInt32, SInt32, Boolean *outShowGrid )
{
	return PTPaintView::GetZoomFactor( 32, 32, outShowGrid );
}

