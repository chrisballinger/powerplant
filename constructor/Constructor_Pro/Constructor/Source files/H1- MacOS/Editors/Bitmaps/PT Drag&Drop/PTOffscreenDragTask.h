// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.

class SUOffscreen;

const unsigned long		DragFlavor_Offscreen = 'DOFF';		// need to register this ???


class PTOffscreenDragTask : public LDragTask
{
	public:
							PTOffscreenDragTask( const EventRecord &, const Rect &inRect, SUOffscreen * );
		virtual				~PTOffscreenDragTask();
		
	protected:
#ifdef WINVER
		S_PASCAL(OSErr)		 SendProc( FlavorType, void *refCon, ItemReference, DragReference );
#else
		static pascal OSErr	 SendProc( FlavorType, void *refCon, ItemReference, DragReference );
#endif
};


