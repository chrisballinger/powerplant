// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*
	96.08.08	ebs		Added support for minimum size		
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "SU_Macros.h"
#include "SUPaneResizer.h"
#include "SUMiscUtils.h"
#include "SUFileUtils.h"

const ResType	ResType_PaneResizer	= 'RSIZ';

/*====================================
	PaneResizeList
	
	This is the resource format of the pane resizer record.
	
	Note: everything is a long so we don't have alignment problems (68k vs. PowerPC)
=====================================*/
typedef struct
{
	PaneIDT			paneID;
	
		// these are multipliers -- we move/resize by the size changed multiplied
		// by these values. if they are zero, there is no change.
	SInt32			hResize;
	SInt32			vResize;
	SInt32			hMove;
	SInt32			vMove;
	
		// items are never resized smaller than their minimum amount
	SInt32			hMin;			
	SInt32			vMin;
} PaneResizeRec;

typedef struct
{
	SInt32			numEntries;
	PaneResizeRec	entries[ 1 ];
} PaneResizeList, **PaneResizeListH;


#ifdef NOT_USED_ANYMORE
/*====================================
	ResizeAndMovePanes
	
	NOTE:
	This is no longer used in the icon editor. Remove ???
	
	Parameters:
	inResID					-	RSIZ resource id which describes how things are moved
	inDefaultWidth & Height	- 	the default size of the image displayed in the pane
	inNewWidth & Height		-	the new size of the image displayed in the pane
	
	Note:
	The sizes are _not_ necessarily the pane size, but rather the size of the
	image displayed within the pane. This way, the PPob resources can be resized
	without affecting the code.
=====================================*/
void SUPaneResizer::ResizeAndMovePanes( LView *inParent, ResIDT inResID, 
										Int32 inDefaultWidth, Int32 inDefaultHeight,
										Int32 inNewWidth, Int32 inNewHeight, 
										Boolean inUpdate )
{
	PaneResizeListH		h = nil;
	
	try
	{
		h = (PaneResizeListH) SUFileUtils::GetAppResource( ResType_PaneResizer, inResID );
		::HLock( (Handle) h );
	
		for ( Int32 count = 0; count < (**h).numEntries; count++ )
		{
			PaneResizeRec	*p = &(**h).entries[ count ];
			
			LPane *thePane = inParent->FindPaneByID( p->paneID );
			ThrowIfNil_( thePane );
			
			/*************************************************
				resize the pane, limited to the minimum size
			*************************************************/
			if ( p->hResize || p->vResize )
			{
				Int32	newWidth = inDefautlWidth - (inNewWidth - inDefaultWidth) * p->hResize;			
				Int32	newHeight = inDefaultHeight - (inNewHeight - inDefaultHeight) * p->vResize;
			
				newWidth = MAX( p->hMin, newWidth );
				newHeight = MAX( p->vMin, newHeight );
				
				thePane->ResizeFrameTo( newWidth, newHeight );
			}

			/*************************************************
				move the pane
			*************************************************/
			if ( p->hMove || p->vMove )
				thePane->MoveBy( dh * p->hMove, dv * p->vMove, inUpdate );
		}
	}
	catch( ... )
	{
		SUMiscUtils::DisposeHandle( (Handle) h );
		throw;
	}
}
#endif

/*====================================
	ResizeWindowIfNeeded
	
	Description:
	Resizes the passed window to be big enough for all enclosing frames.
	
	Note:
	Only looks at 1st-level panes/views, not enclosed ones.
=====================================*/
void SUPaneResizer::ResizeWindowIfNeeded( LWindow *inWindow, SInt32 hMargin, SInt32 vMargin )
{
	SInt32		oldWidth, oldHeight;
	SInt32		newWidth, newHeight;
	
	SDimension16	oldSize;
	inWindow->GetFrameSize( oldSize );
	oldWidth = oldSize.width;
	oldHeight = oldSize.height;
	
	SUPaneResizer::GetMaxContainedSize( inWindow, newWidth, newHeight );
	newWidth += hMargin;
	newHeight += vMargin;
	
	if ( (newWidth != oldWidth) || (newHeight != oldHeight) )
	{
			// resize the view
		SInt32	dh = MAX( 0, newWidth - oldWidth );
		SInt32	dv = MAX( 0, newHeight - oldHeight );
		inWindow->ResizeFrameBy( dh, dv, true );
		
			// resize the window itself
		Rect	r;
		SUMiscUtils::GetWindowGlobalBounds( GetWindowFromPort(inWindow->GetMacPort()), r );
		r.right = r.left + newWidth;
		r.bottom = r.top + newHeight;
		inWindow->DoSetBounds( r );
	}
}

/*====================================
	GetMaxContainedSize
=====================================*/
void SUPaneResizer::GetMaxContainedSize( LView *inView, SInt32 &oWidth, SInt32 &oHeight )
{
	SInt32			maxRight = 0, maxBottom = 0;
	LPane			*thePane;

	LArrayIterator	anIterator( inView->GetSubPanes() );

	while ( anIterator.Next( &thePane ) ) 
	{
		SPoint32		frameLoc;
		SDimension16	frameSize;
		
		thePane->GetFrameLocation( frameLoc );
		thePane->GetFrameSize( frameSize );
		
		maxRight = MAX( maxRight, frameLoc.h + frameSize.width );
		maxBottom = MAX( maxBottom, frameLoc.v + frameSize.height );
	}

	oWidth = maxRight;
	oHeight = maxBottom;
}


