// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "PTImageDragMixin.h"
#include "SUOffscreen.h"
#include "PT_Resources.h"			// for ImageType_Picture
#include "PTOffscreenDragTask.h"			// for DragFlavor_Offscreen
#include "SUSaveGWorld.h"
#include "SUMiscUtils.h"
#include "PTGraphicConversions.h"
#include "SURegionUtils.h"

/*=================================
	Constructor
==================================*/
PTImageDragMixin::PTImageDragMixin( GrafPtr inPort, LPane *inPane )
			: LDragAndDrop( GetWindowFromPort(inPort), inPane )
{
}


/*=================================
	ItemIsAcceptable
==================================*/
Boolean PTImageDragMixin::ItemIsAcceptable( DragReference inDragRef, ItemReference inItemRef )
{
	OSErr				err = noErr;
	FlavorFlags			theFlags;
	HFSFlavor			hfsInfo;
	PromiseHFSFlavor	promiseInfo;
	
		// do we need to check the flags for anything ???
		// is the promise code correct ???
	
	err = ::GetFlavorFlags( inDragRef, inItemRef, DragFlavor_Offscreen, &theFlags );
	if ( !err ) return( true );
	
	err = ::GetFlavorFlags( inDragRef, inItemRef, ImageType_Picture, &theFlags );
	if ( !err ) return( true );
	
	long numBytes = sizeof( hfsInfo );
	err = ::GetFlavorData( inDragRef, inItemRef, flavorTypeHFS, (void*) &hfsInfo, &numBytes, 0 );
	if ( !err && (hfsInfo.fileType == ImageType_Picture) ) return( true );
	
	numBytes = sizeof( promiseInfo );
	err = ::GetFlavorData( inDragRef, inItemRef, flavorTypePromiseHFS, (void*) &promiseInfo, &numBytes, 0 );
	if ( !err && (promiseInfo.fileType == ImageType_Picture) ) return( true );

	return( false );
}


/*=================================
	ReceiveDragItem
==================================*/
void PTImageDragMixin::ReceiveDragItem( DragReference inDragRef, DragAttributes inAttribs,
										ItemReference inItemRef, Rect & )
{
	OSErr			err = noErr;
	long			numBytes;
	StSaveGWorld	aSaver;
	
	/*****************************************
		first see if we get a SUOffscreen (internal drag)
	*****************************************/
	err = ::GetFlavorDataSize( inDragRef, inItemRef, DragFlavor_Offscreen, &numBytes );
	if ( !err && (numBytes == sizeof(SUOffscreen*)) )
	{
		SUOffscreen		*draggedBuffer = nil;
		
		err = ::GetFlavorData( inDragRef, inItemRef, DragFlavor_Offscreen, &draggedBuffer, &numBytes, 0 );
		if ( err ) return;
		
		this->HandleDroppedOffscreen( draggedBuffer, inDragRef, inAttribs );
		return;
	}
	
	
	/*****************************************
		next check for a PICT
	*****************************************/
	err = ::GetFlavorDataSize( inDragRef, inItemRef, ImageType_Picture, &numBytes );
	if ( !err && (numBytes >= static_cast<long>(sizeof(Picture))) )
	{
		PicHandle thePict = (PicHandle) ::NewHandle( numBytes );
		ThrowIfMemFail_( thePict );

		HLock( (Handle) thePict );
		err = ::GetFlavorData( inDragRef, inItemRef, ImageType_Picture, (void*) *thePict, &numBytes, 0 );
		if ( err )
		{
			SUMiscUtils::DisposeHandle( (Handle) thePict );
			Throw_( err );
		}
		
		HUnlock( (Handle) thePict );
		
		this->HandleDroppedPicture( thePict, inDragRef, inAttribs );
		SUMiscUtils::DisposeHandle( thePict );
		return;
	}
	
	
	/*****************************************
		next check for a PICT file
	*****************************************/
	HFSFlavor			hfsInfo;
	numBytes = sizeof( hfsInfo );
	err = ::GetFlavorData( inDragRef, inItemRef, flavorTypeHFS, (void*) &hfsInfo, &numBytes, 0 );
	if ( !err )
		this->HandleDroppedPictureFile( hfsInfo.fileSpec, inDragRef, inAttribs );
		
	if ( err )
		Throw_( err );
}

/*=================================
	HandleDroppedPictureFile
==================================*/
void PTImageDragMixin::HandleDroppedPictureFile( const FSSpec &inSpec, DragReference inDragRef,
												DragAttributes inAttribs )
{
	PicHandle		thePict = nil;
	
	try
	{
		thePict = PTGraphicConversions::GetPictFromFile( inSpec );
		this->HandleDroppedPicture( thePict, inDragRef, inAttribs );
	}
	catch( ... )
	{
		SUMiscUtils::DisposeHandle( (Handle) thePict );
		throw;
	}
	
	SUMiscUtils::DisposeHandle( (Handle) thePict );
}

/*=================================
	HiliteDropArea
	
	Description:
	Hilites the drag area by drawing a few xor-ed rectangles around mPane.
	The default hiliting doesn't show up very well at all for our panes
	  so this one is somewhat thicker and drawn outside of the pane itself.
==================================*/
void PTImageDragMixin::HiliteDropArea( DragReference )
{
	Rect	r;
	mPane->CalcLocalFrameRect( r );	

	StColorPenState			thePen;
	thePen.Normalize();
	::PenMode( patXor );

	#ifdef xxxxx
	//RGBColor	aGray = { 56797, 56797, 56797 };
	RGBColor	aGray = { 0xFFFF, 0xFFFF, 0xFFFF };
	RGBForeColor( &aGray );
	#endif
	
		// don't want any clipping since we draw outside of the pane	
	Rect	bigClipR = { -15000, -15000, 15000, 15000 };
	StClipRgnState	aClip( bigClipR );		// want to draw outside of the pane too
	
		// draw some rectangles
	#define kThickness		3
	for ( long count = 0; count < kThickness; count++ )
	{
		::FrameRect( &r );
		::InsetRect( &r, -1, -1 );
	}
}

/*=================================
	UnhiliteDropArea
==================================*/
void PTImageDragMixin::UnhiliteDropArea( DragReference inDragRef )
{
	this->HiliteDropArea( inDragRef );		// since it xors
}

