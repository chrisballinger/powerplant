// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.

class PTResizeDialog
{
	public:
		static Boolean	DoResizeDialog( SInt32 inWidth, SInt32 inMinWidth, SInt32 inMaxWidth,
										SInt32 inHeight, SInt32 inMinHeight, SInt32 inMaxHeight,
										SInt32 *outWidth, SInt32 *outHeight );

	protected:
		static void		ShowResizeError( SInt32 inMinWidth, SInt32 inMaxWidth,
										 SInt32 inMinHeight, SInt32 inMaxHeight );
};

