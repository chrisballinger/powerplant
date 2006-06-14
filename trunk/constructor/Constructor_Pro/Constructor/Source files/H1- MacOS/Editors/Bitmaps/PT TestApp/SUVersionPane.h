// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
#include "LCaption.h"
#include "SUMiscUtils.h"

/*====================================
	SUVersionPane
=====================================*/
class SUVersionPane : LCaption
{
	public:

	enum { class_ID = 'VERS' };
	
	static SUVersionPane*	CreateVersionStream(LStream *inStream);

							SUVersionPane();		// are these needed ???
							SUVersionPane( LStream * );
		virtual				~SUVersionPane();		
	
	protected:
		virtual void		FinishCreateSelf();
};
