// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
#pragma once

class SUOffscreen;
class PTPaintSelection;

class PTPaintSelection
{
	public:
								PTPaintSelection();
		virtual					~PTPaintSelection();

			// getting the region		
		virtual RgnHandle		GetRegion();
		virtual RgnHandle		GetCopyOfRegion();

			// changing the region
		virtual void			SelectAll( SUOffscreen * );
		virtual void			SelectNone();
		
		virtual void			SetSelection( SUOffscreen *, RgnHandle );
		virtual void			SetSelection( SUOffscreen *, const Rect & );
		virtual void			SetSelection( PTPaintSelection * );
		
			// change the region, but do not affect the bits
		virtual void			SetRawSelection( RgnHandle );
		
			// copy/paste
		virtual void			CopyToClipboard();
		virtual void			PasteFromClipboard( SUOffscreen *parentOffscreen );
		virtual void			PastePicture( SUOffscreen *parentOffscreen, PicHandle, RgnHandle = nil );
		virtual void 			PasteOffscreenBuffer( SUOffscreen *inParentBuffer, SUOffscreen *inBuffer, RgnHandle = nil );
		
			// display
		virtual void			DrawInto( SUOffscreen * );
		
			// this is used by flip & rotation code
		virtual SUOffscreen		*GetImageBuffer();
				
			// misc
		virtual Boolean			IsEmpty();
		virtual Boolean			PointInSelection( SInt32 left, SInt32 top );
		virtual void			Offset( SInt32 dh, SInt32 dv );
		virtual void			MoveTo( SInt32 h, SInt32 v );
		
	protected:
		SUOffscreen				*mSelectionBuffer;
		RgnHandle				mRegion;
		
		void					DisposeBuffer();
};

