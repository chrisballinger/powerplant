// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
#include "PTActions.h"

class PTRecolorAction : public PTAction
{
	public:
							PTRecolorAction( const SPaintAction &inAction, CTabHandle inTable );
		virtual				~PTRecolorAction();
		
		virtual void		DoIt();

	protected:
		CTabHandle			mTable;

		virtual CTabHandle	CreateResizedTable( CTabHandle inSource );
};

