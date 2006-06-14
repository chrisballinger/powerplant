// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*
	96.11.26	ebs		Changed GetCenteredBufferArea() since GABox no longer has
							an mBorderRect instance variable.
	96.08.28	ebs		Added GetDropEnable/SetDropEnable/ItemIsAcceptable (lock file support)
	96.08.08	ebs		support for saving initial size & min pane size
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "SUOffscreen.h"
#include "SUColorCursorCache.h"
#include "SUMiscUtils.h"
#include "SUSaveGWorld.h"
#include "PTDraggableTargetBox.h"
#include "PT_Resources.h"
#include "PTOffscreenDragTask.h"

/*=================================
	UserCon flags (stored in userCon of pane)
==================================*/
const UInt32	targetFlag_IsMask		= 0x0001;
const UInt32	targetFlag_CanDelete	= 0x0002;

/*=================================
	if we let the pane get too small, the text is cut off horizontally.
	this should probably be read from a resource???
==================================*/
const UInt16	kMinTargetWidth = 40;
const UInt16	kMinTargetHeight = 40;

/*=================================
	Constructor
==================================*/
PTDraggableTargetBox::PTDraggableTargetBox( LStream *inStream )
		: PTTargetBox( inStream ), PTImageDragMixin( this->GetMacPort(), this )
{
	mCursorCache = SUColorCursorCache::GetCurrentCache();
	mBuffer = nil;
	mIsUsed = true;						// assume we're not deleted

	UInt32	refCon = this->GetUserCon();
	mIsMask = (refCon & targetFlag_IsMask) ? true : false;
	mCanBeDeleted = (refCon & targetFlag_CanDelete) ? true : false;
	
		// these are for resizing support
	this->GetFrameSize( mInitialPaneSize );
	this->GetFrameLocation( mInitialPaneLocation );
	mMinPaneSize.width = kMinTargetWidth;
	mMinPaneSize.height = kMinTargetHeight;
	
	mDropEnabled = true;
}

/*=================================
	Destructor
==================================*/
PTDraggableTargetBox::~PTDraggableTargetBox()
{
	if ( mBuffer )
		delete mBuffer;
}


/*=================================
	CreateFromStream
==================================*/
PTDraggableTargetBox *PTDraggableTargetBox::CreateFromStream( LStream *inStream )
{
	return new PTDraggableTargetBox( inStream );
}

/*=================================
	AdjustCursorSelf
==================================*/
void PTDraggableTargetBox::AdjustCursorSelf( Point, const EventRecord & )
{
	if ( mCursorCache && mBuffer )
		mCursorCache->SetColorCursor( CURS_Hand );
}

/*=================================
	ClickSelf
==================================*/
void PTDraggableTargetBox::ClickSelf( const SMouseDownEvent &inMouseDown )
{
	/***********************************************
		if drag & drop isn't present or the user just clicks
		the mouse quickly without moving it, broadcast a message
	***********************************************/
	if ( !LDragAndDrop::DragAndDropIsPresent() )
	{
		this->BroadcastMessage( cmd_TargetBoxClicked, this );
		return;
	}

		// wait for mouse to move a little -- if it doesn't, broadcast
	if ( !::WaitMouseMoved( inMouseDown.macEvent.where ) )
	{
		this->BroadcastMessage( cmd_TargetBoxClicked, this );
		return;
	}
	
	/***********************************************
		the user moved the mouse while it was down...
		perform a drag operation
	***********************************************/
	if ( mBuffer )
	{
		Rect	localR;
		
		this->FocusDraw();
		this->CalcLocalFrameRect( localR );
		PTOffscreenDragTask	theDragTask( inMouseDown.macEvent, localR, mBuffer );
	}
}

/*=================================
	SetBuffer
	
	Note:
	Once this call is made, this object takes complete control of the buffer.
==================================*/
void PTDraggableTargetBox::SetBuffer( SUOffscreen *inBuffer, ERedrawOptions inRedraw )
{
	if ( mBuffer == inBuffer ) return;		// drag to ourselves
	
		// delete current buffer (if any)
	if ( mBuffer ) delete mBuffer;
	mBuffer = inBuffer;
	this->RedrawBufferAsIndicated( inRedraw );
}

/*=================================
	SetRawBuffer
	
	Note:
	Most callers will use the SetBuffer() call instead.
==================================*/
void PTDraggableTargetBox::SetRawBuffer( SUOffscreen *inBuffer )
{
	mBuffer = inBuffer;
}

/*=================================
	RedrawBufferAsIndicated
	
	Description:
	Redraws just the buffer area of the pane either right now or by posting
	  an update event. 
	This is useful for preventing flicker -- it doesn't redraw the pane frame or title.
==================================*/
void PTDraggableTargetBox::RedrawBufferAsIndicated( ERedrawOptions inRedraw )
{
	StSaveGWorld		aSaver;
	StColorPenState		save2;
	save2.Normalize();
	
	if ( inRedraw == redraw_Now )
	{
		this->DrawBuffer();	
	}
	else if ( (inRedraw == redraw_Later) && mBuffer )
	{
		Rect	portR;
		this->GetCenteredBufferArea( mBuffer->GetWidth(), mBuffer->GetHeight(), nil, &portR );
		this->CalcPortFrameRect(portR);
		this->InvalPortRect( &portR );
	}
}

/*=================================
	CopyBufferFrom
==================================*/
void PTDraggableTargetBox::CopyBufferFrom( SUOffscreen *inBuffer, ERedrawOptions inRedraw )
{
	if ( !mBuffer ) return;
	mBuffer->CopyFrom( inBuffer );

	this->SetUsedFlag( true, redraw_Dont );		// we have an image -- not empty any more	
	this->RedrawBufferAsIndicated( inRedraw );
}

/*=================================
	GetBuffer
==================================*/
SUOffscreen *PTDraggableTargetBox::GetBuffer()
{
	return mBuffer;
}

/*=================================
	DrawSelf
==================================*/
void PTDraggableTargetBox::DrawSelf()
{
	PTTargetBox::DrawSelf();
	this->DrawBuffer();
}


/*=================================
	DrawBuffer
==================================*/
void PTDraggableTargetBox::DrawBuffer()
{	
	StSaveGWorld		aSaver;
	StColorPenState		aPenState;
	
	if ( !mBuffer ) return;
	
	GrafPtr		macPort = this->GetMacPort();
	Rect		localR, portR;
	
	if ( !macPort ) return;
	this->GetCenteredBufferArea( mBuffer->GetWidth(), mBuffer->GetHeight(), &localR, &portR );
	
	if ( this->IsUsed() )
		mBuffer->CopyTo( macPort, &portR );
	else
	{
		this->FocusDraw();
		aPenState.Normalize();
		Pattern thePattern;
		UQDGlobals::GetLightGrayPat(&thePattern);
		::PenPat( &thePattern );
		::PaintRect( &localR );
	}
}

/*=================================
	HandleDroppedOffscreen
==================================*/
void PTDraggableTargetBox::HandleDroppedOffscreen( 	SUOffscreen *inBuffer, 
													DragReference inDragRef, 
													DragAttributes inDragAttribs )
{
	SImageDropOnTargetBox		info;
	
	info.thePicture = nil;
	info.theBuffer = inBuffer;
	info.thePane = this;
	info.theDragRef = inDragRef;
	info.theDragAttribs = inDragAttribs;
	
	this->BroadcastMessage( cmd_ImageDroppedOnTargetBox, &info );
}

/*=================================
	HandleDroppedPicture
==================================*/
void PTDraggableTargetBox::HandleDroppedPicture( 	PicHandle inPicture, 
													DragReference inDragRef, 
													DragAttributes inDragAttribs )
{
	SImageDropOnTargetBox		info;
	
	info.thePicture = inPicture;
	info.theBuffer = nil;
	info.thePane = this;
	info.theDragRef = inDragRef;
	info.theDragAttribs = inDragAttribs;
	
	this->BroadcastMessage( cmd_ImageDroppedOnTargetBox, &info );
}

/*=================================
	GetCenteredBufferArea
	
	Description:
	Returns a rectangle centered on mBorderRect, in port coordinates (for CopyBits).
==================================*/
void PTDraggableTargetBox::GetCenteredBufferArea( SInt32 inWidth, SInt32 inHeight, 
												Rect *outLocalRect, Rect *outPortRect )
{
	Rect	borderR;
	this->CalcBorderRect( borderR );
	
	SInt32	paneWidth = borderR.right - borderR.left;
	SInt32	paneHeight = borderR.bottom - borderR.top;	

	Rect	localR;
	localR.left = borderR.left + (paneWidth - inWidth)/2;
	localR.right = localR.left + inWidth;
	localR.top = borderR.top + (paneHeight - inHeight)/2;
	localR.bottom = localR.top + inHeight;
		
	if ( outLocalRect )
		*outLocalRect = localR;
		
	if ( outPortRect )
	{
		*outPortRect = localR;
		LocalToPortPoint( topLeft(*outPortRect) );
		LocalToPortPoint( botRight(*outPortRect) );
	}
}

#ifdef OLD_VERSION_CW10
/*=================================
	GetCenteredBufferArea
	
	Description:
	Returns a rectangle centered on mBorderRect, in port coordinates (for CopyBits).
==================================*/
void PTDraggableTargetBox::GetCenteredBufferArea( Int32 inWidth, Int32 inHeight, 
												Rect *outLocalRect, Rect *outPortRect )
{
	Int32	paneWidth = mBorderRect.right - mBorderRect.left;
	Int32	paneHeight = mBorderRect.bottom - mBorderRect.top;	

	Rect	localR;
	localR.left = mBorderRect.left + (paneWidth - inWidth)/2;
	localR.right = localR.left + inWidth;
	localR.top = mBorderRect.top + (paneHeight - inHeight)/2;
	localR.bottom = localR.top + inHeight;
		
	if ( outLocalRect )
		*outLocalRect = localR;
		
	if ( outPortRect )
	{
		*outPortRect = localR;
		LocalToPortPoint( topLeft(*outPortRect) );
		LocalToPortPoint( botRight(*outPortRect) );
	}
}
#endif

/*=================================
	IsMask
==================================*/
Boolean PTDraggableTargetBox::IsMask()
{
	return mIsMask;
}

/*=================================
	SetMask
==================================*/
void PTDraggableTargetBox::SetMask( Boolean inMask )
{
	mIsMask = inMask;
}

/*=================================
	IsUsed
==================================*/
Boolean PTDraggableTargetBox::IsUsed()
{
	return( mIsUsed );
}

/*=================================
	SetUsedFlag
==================================*/
void PTDraggableTargetBox::SetUsedFlag( Boolean inUsed, ERedrawOptions inRedraw )
{
	if ( mIsUsed == inUsed ) return;
	
	mIsUsed = inUsed;
	this->RedrawBufferAsIndicated( inRedraw );
}

/*=================================
	CanBeDeleted
==================================*/
Boolean PTDraggableTargetBox::CanBeDeleted()
{
	return( mCanBeDeleted );
}

/*=================================
	SetCanBeDeleted
==================================*/
void PTDraggableTargetBox::SetCanBeDeletedFlag( Boolean inValue )
{
	mCanBeDeleted = inValue;
}

/*=================================
	GetInitialPaneSize
==================================*/
void PTDraggableTargetBox::GetInitialPaneSize( SDimension16 *outSize )
{
	*outSize = mInitialPaneSize;
}

/*=================================
	GetMinPaneSize
==================================*/
void PTDraggableTargetBox::GetMinPaneSize( SDimension16 *outSize )
{
	*outSize = mMinPaneSize;
}

/*=================================
	GetInitialPaneLocation
==================================*/
void PTDraggableTargetBox::GetInitialPaneLocation( SPoint32 *outLoc )
{
	*outLoc = mInitialPaneLocation;
}

/*===================================
	ItemIsAcceptable
====================================*/
Boolean PTDraggableTargetBox::ItemIsAcceptable( DragReference inDragRef, ItemReference inItemRef )
{
	if ( !mDropEnabled )
		return( false );
	
	return( PTImageDragMixin::ItemIsAcceptable( inDragRef, inItemRef ) );
}

/*===================================
	GetDropEnable
====================================*/
Boolean PTDraggableTargetBox::GetDropEnable()
{
	return( mDropEnabled );
}

/*===================================
	GetDropEnable
====================================*/
void PTDraggableTargetBox::SetDropEnable( Boolean inEnable )
{
	mDropEnabled = inEnable;
}
