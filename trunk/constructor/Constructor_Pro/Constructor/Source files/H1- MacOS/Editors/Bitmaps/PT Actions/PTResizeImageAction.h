// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
#include "PTActions.h"
#include "PTResizer.h"

class PTResizeImageAction : public PTAction
{
	public:
							PTResizeImageAction( const SPaintAction & );
		virtual 			~PTResizeImageAction();
		
		virtual void		DoIt();
		
	protected:
		PTResizer			mBeforeState, mAfterState;
		Boolean				mCancelledResize;
		virtual void		UndoSelf();
		virtual void		RedoSelf();
};

