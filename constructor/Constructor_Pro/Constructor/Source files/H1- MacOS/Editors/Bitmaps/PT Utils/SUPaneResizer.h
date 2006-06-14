// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
#pragma once

class SUPaneResizer
{
	public:
	/*
		static void		ResizeAndMovePanes( LView *inParent, ResIDT inResID, 
											Int32 inDefaultWidth, Int32 inDefaultHeight,
											Int32 inNewWidth, Int32 inNewHeight, 
											Boolean inUpdate = true );
	*/
											
		static void		ResizeWindowIfNeeded( LWindow *inWindow, SInt32 hMargin, SInt32 vMargin );
		static void		GetMaxContainedSize( LView *inWindow, SInt32 &oWidth, SInt32 &oHeight );
};

