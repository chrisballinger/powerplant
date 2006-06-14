// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
#include "PTActions.h"
#include "SUMarchingAnts.h"

/*==========================================
	PTSelectionAction 
===========================================*/
class PTSelectionAction : public PTTrackingPaintAction
{
	public:
							PTSelectionAction( const SPaintAction & );
		virtual				~PTSelectionAction();

		virtual void 		HandleMouseDown( const SMouseDownEvent &inEvent );

	protected:
		Point				mMouseDownPt;
		Rect				mMouseDownCellR;
		
		Rect				mPrevDrawnRect;
		Rect				mCurrentSelection;
		SUMarchingAnts		mMarchingAnts;	
		Boolean				mUseXorMode;
		
		virtual void		MouseInitial( const SMouseDownEvent &, SInt32 newCol, SInt32 newRow );

		virtual void		MouseStillDown( const SMouseDownEvent &, 
											Point prevMousePt, Point newMousePt,
											SInt32 prevCol, SInt32 prevRow,
											SInt32 newCol, SInt32 newRow );
											 		
		virtual Boolean		MouseFinal( const SMouseDownEvent &, 
											Point prevMousePt, Point newMousePt,
											SInt32 prevCol, SInt32 prevRow,
											SInt32 newCol, SInt32 newRow );

		virtual void		DrawNewRect( const Rect & );
		virtual void		EraseOldRect();
};

