// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
#include "PTActions.h"

class PTDraggableTargetBox;

class PTTargetClickedAction : public PTAction
{
	public:
								PTTargetClickedAction( const SPaintAction &inAction, 
														PTDraggableTargetBox * );
		virtual					~PTTargetClickedAction();
		
		virtual void			DoIt();
		
	protected:
		PTDraggableTargetBox	*mTargetBoxHit;
};

