// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
#include "PTPaintView.h"

typedef struct
{
	LPane			*thePane;
	SInt32			top, left;
	SInt32			width, height;
	SUOffscreen 	*buffer;
} PTSavedPaneInfo;

class PTResizer
{
	public:
							PTResizer();
		virtual				~PTResizer();
		
		virtual void		Initialize( PTPaintView * );
		virtual void		RestoreWindowState();
		virtual	void		DeleteAllBuffers();

	protected:
		PTPaintView			*mPaintView;
		Rect				mWindowBounds;
		SUOffscreen			*mImageBuffer;
		
		PTSavedPaneInfo		mSampleWellInfo;
		
		SInt32				mNumSamplePanes;
		PTSavedPaneInfo		mSamplePaneInfo[ kMaxSamplePanes ];
		
		virtual void		GetPaneInfoByID( PaneIDT, Boolean isTargetBox, PTSavedPaneInfo * );
		virtual void		GetPaneInfo( LPane *, Boolean isTargetBox, PTSavedPaneInfo * );
		virtual void		SetPaneInfo( PTSavedPaneInfo & );
};

