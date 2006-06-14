// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
#include "PTActions.h"
#include "PTDraggableTargetBox.h"

/*=================================
	PTDragImgeToTargetAction
==================================*/
class PTDragImageToTargetAction : public PTAction
{
	public:
								PTDragImageToTargetAction( const SPaintAction &inAction, 
														SImageDropOnTargetBox * );
		virtual					~PTDragImageToTargetAction();
		virtual void			DoIt();
		
	protected:
		SImageDropOnTargetBox	mDropInfo;
		
		virtual void			GenerateMask( SUOffscreen *inSource, SUOffscreen *inDest );
};

