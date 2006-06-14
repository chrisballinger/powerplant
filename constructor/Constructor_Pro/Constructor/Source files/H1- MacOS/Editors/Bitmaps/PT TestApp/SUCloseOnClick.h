// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
class SUCloseOnClick : public LAttachment, public LBroadcaster
{
	public:
								SUCloseOnClick( const LWindow *closeThisOne );
		virtual		Boolean		Execute( MessageT inMessage, void *ioParam );

		static 		void		CreateSplashWindow( ResIDT inRezID, LCommander *inSuper );

	protected:
		LWindow		*mWindowToClose;
};

