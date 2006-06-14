// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
class PTPaintView;

/*===================================
	PTColorTableChoice
====================================*/
class PTColorTableChoice
{
	public:
						PTColorTableChoice( PTPaintView *, Boolean inUsesIconColors );
						~PTColorTableChoice();
					
		Boolean			FindCommandStatus( SInt32 inDepth, CommandT inCommand, 
										Boolean &outEnabled, Boolean &outUsesMark, 
										UInt16 &outMark, Str255 outName );
										
		Boolean			ObeyCommand( SInt32 inDepth, CommandT inCommand );
		void			ImageChanged( SInt32 inNewDepth );
		CTabHandle		GetCurrentTable();
	
	protected:
		CommandT		mTableCommands[ 33 ];	// we use the bit depth as an index
		
		CTabHandle		mCurrentTable;
		PTPaintView		*mPaintView;
		
		void			InitTableCommands( Boolean inIcon );
		Boolean			MapCommandToTableID( CommandT, ResIDT * );
		void			ChangePopupColorTables( ResIDT inResID );
		void			DisposeCurrentTable();
};
