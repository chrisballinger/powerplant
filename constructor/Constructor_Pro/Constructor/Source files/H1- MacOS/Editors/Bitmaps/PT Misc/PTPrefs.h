// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.


class PTPrefs
{
	public:
		static void			Initialize();
		static void			Save();
	
		static Boolean		Use32BitBuffersForPicts();
		static Boolean		Use32BitBuffersForOthers();
		static Boolean		UsePalettesForWindows();
		static Boolean		DefaultToFullColorTable();
		
	protected:
		static void			GetDefaultPrefs( struct PTPreferenceRec * );
};

