// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	GWindowBorder.h				   © 1996 Metrowerks Inc. All rights reserved.
// ===========================================================================

#pragma once

#include <LView.h>


// ===========================================================================
//		¥ GWindowBorder
// ===========================================================================

class GWindowBorder : public LView {

public:
	enum { class_ID = 'Wbdr' };

	static LView*			CreateFromStream(LStream*	inStream)
									{ return new GWindowBorder(inStream); }

							GWindowBorder(
									LStream*			inStream);
	virtual					~GWindowBorder();

	virtual void			Activate();
	virtual void			Deactivate();

protected:
	virtual void			DrawSelf();
	virtual void			DrawInset(
									Int16				inPixelDepth);
	virtual void			RefreshInset();

	virtual void			FinishCreateSelf();

	virtual void			ActivateSelf();
	virtual void			DeactivateSelf();
	virtual void			ShowSelf();


	LPane*					mInsetSubPane;
	PaneIDT					mInsetSubPaneID;
	Boolean					mGrowBoxOuterNotch;
	Boolean					mGrowBoxInnerNotch;
	Boolean					mFrameInsetPane;
	Boolean					mHasBeenShown;
	Boolean					mDrawFrame;

};
