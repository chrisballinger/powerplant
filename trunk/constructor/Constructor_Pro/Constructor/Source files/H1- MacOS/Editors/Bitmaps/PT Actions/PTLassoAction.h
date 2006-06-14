// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
#include "PTActions.h"
#include "SUMarchingAnts.h"

class PTLassoAction : public PTTrackingPaintAction
{
	public:
						PTLassoAction( const SPaintAction & );
		virtual			~PTLassoAction();
		
		virtual void 	HandleMouseDown( const SMouseDownEvent &inEvent );
		virtual void	DoIt();			// for "lasso all"
		
	protected:
		#define kMaxPoints	5000
		SInt32				mNumPoints;
		Point				*mPointArray;
		
		SInt32				mImageWidth, mImageHeight;
		Rect				mTopLeftPixelRect;
		SInt32				mCellWidth, mSpaceBetweenCells, mTotalCellWidth;
		SUMarchingAnts		mMarchingAnts;	
		//Boolean				mAntsDrawn;
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

		virtual void		FindClosestNode( const Point &, SInt32 *outH, SInt32 *outV );
		
		virtual Boolean 	AddPointToArray( SInt32 newCol, SInt32 newRow );
		virtual void		RemovePointFromArray();
		
			// drawing the array as the user moves the mouse
		//virtual void		XorArray();
		//virtual void		XorOneSegment( SInt32 inSegmentNo );
		
		virtual void		DrawSegment( SInt32 inSegmentNo );
		virtual void		DrawArray();
		virtual void		AnimateArray();
		
			// after the mouse is releases, these are used
		virtual RgnHandle	ConvertArrayToRegion();
		virtual void		RestrictRegion( RgnHandle );
		virtual Boolean		IsArrayLinear();
		virtual void		CopyAndDownSample( SUOffscreen *src, SUOffscreen *dst, RgnHandle selRgn );
};

