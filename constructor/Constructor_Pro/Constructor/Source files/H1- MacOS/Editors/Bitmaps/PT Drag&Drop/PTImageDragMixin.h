// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.

#pragma once

class SUOffscreen;

class PTImageDragMixin : public LDragAndDrop
{
	public:
							PTImageDragMixin( GrafPtr, LPane * );
		
	protected:
		virtual void		HandleDroppedPicture( PicHandle, DragReference = 0, DragAttributes = 0 ) = 0;
		virtual void		HandleDroppedOffscreen( SUOffscreen *, DragReference = 0, DragAttributes = 0 ) = 0;

		virtual Boolean		ItemIsAcceptable( DragReference, ItemReference );
		virtual void		ReceiveDragItem( DragReference, DragAttributes, ItemReference, Rect & );
		virtual void		HandleDroppedPictureFile( const FSSpec &, DragReference = 0, DragAttributes = 0 );

		virtual void		HiliteDropArea( DragReference );
		virtual void		UnhiliteDropArea( DragReference );
};

