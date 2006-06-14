// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*
	96.07.29	ebs		removed :: before DisposeRoutineDescriptor
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "PTOffscreenDragTask.h"
#include "SUOffscreen.h"
#include "PTGraphicConversions.h"
#include "PT_Resources.h"		// for ImageType_Picture
#include <Drag.h>

/*============================
	Constructor
	
	This is the only routine you need to call.
==============================*/
PTOffscreenDragTask::PTOffscreenDragTask( const EventRecord &inEvent, const Rect &inRect, SUOffscreen *inOffscreen )
						: LDragTask( inEvent )
{
	DragSendDataUPP		sendUPP = nil;
	long				err = noErr;
	
	try
	{
		/************************************
			add the rectangle to the drag ref, or a pixmap is even better
		*************************************/
		Rect	globalRect = inRect;
		::LocalToGlobal(&topLeft(globalRect));
		::LocalToGlobal(&botRight(globalRect));
		AddRectDragItem( (ItemReference) inOffscreen, globalRect );
		
		/************************************
			add the flavors to the drag ref
			 -- we won't translate the offscreen to a PICT unless a receiver requests it
		*************************************/
		err = ::AddDragItemFlavor( mDragRef, (ItemReference) inOffscreen, DragFlavor_Offscreen, &inOffscreen, sizeof(inOffscreen), flavorSenderOnly );	
		ThrowIfOSErr_( err );
		
		err = ::AddDragItemFlavor( mDragRef, (ItemReference) inOffscreen, ImageType_Picture, nil, 0, 0 );	
		ThrowIfOSErr_( err );
		
		/************************************
			add a sendproc that can translate the offscreen to a PICT
		*************************************/
		sendUPP = NewDragSendDataUPP( SendProc );
		ThrowIfMemFail_( sendUPP );
		
		err = ::SetDragSendProc( mDragRef, sendUPP, nil );
		ThrowIfOSErr_( err );
	
		/************************************
			do the drag
		*************************************/
		err = ::TrackDrag( mDragRef, &mEventRecord, mDragRegion );
	}
	catch( ... )
	{
		if ( sendUPP )
			DisposeDragSendDataUPP( sendUPP );
		throw;
	}
	
	if ( sendUPP )
		DisposeDragSendDataUPP( sendUPP );
}

/*============================
	Destructor
==============================*/
PTOffscreenDragTask::~PTOffscreenDragTask()
{
}

/*============================
	SendProc
==============================*/
#ifdef WINVER
OSErr pascal PTOffscreenDragTask::SendProc( FlavorType inFlavor, void * /*inRefCon*/, ItemReference inItemRef, DragReference inDragRef )
#else
pascal OSErr PTOffscreenDragTask::SendProc( FlavorType inFlavor, void * /*inRefCon*/, ItemReference inItemRef, DragReference inDragRef )
#endif
{
	long	err = noErr;
	
	if ( inFlavor != ImageType_Picture )
	{
		return( noTypeErr );
	}

	try
	{
			// convert the offscreen to a PICT
		PicHandle thePict = PTGraphicConversions::OffscreenToPicture( (SUOffscreen*) inItemRef, nil );
		
			// set the drag item to the data
		::HLock( (Handle) thePict );
		err = ::SetDragItemFlavorData( inDragRef, inItemRef, inFlavor, (void*) *thePict, 
						::GetHandleSize( (Handle) thePict ), 0 );
						
			// cleanup
		::KillPicture( thePict );
	}
	catch (const LException& inErr)
	{
		// don't throw error out of callback -- bad things happen
		err = inErr.GetErrorCode();
	}
	catch( long anErr )
	{
		// don't throw error out of callback -- bad things happen
		err = anErr;
	}
	catch (...)
	{
		// don't throw error out of callback -- bad things happen
		err = err_NilPointer;
	}
	
	return( err );
}

