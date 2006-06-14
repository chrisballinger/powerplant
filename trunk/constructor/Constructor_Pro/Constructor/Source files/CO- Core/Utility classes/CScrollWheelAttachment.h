// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*
File:		CScrollWheelAttachment.h
Contact:	Richard Buckle, Sailmaker Software Ltd
			<mailto:richardb@sailmaker.co.uk>
			<http://www.sailmaker.co.uk/>
Version:	1.0.2
Purpose:	Attachment to add Carbon scroll wheel support to windows
Status:		Public domain
*/

/*
*********** Change history  *************
1.0.2	Conditionalised for correct behaviour on PowerPlant 2.2
1.0.1	No longer requires modifications to LScroller and LScrollerView.
1.0.1	Respects the magnitude of the mouse wheel delta -- it can
		exceed +/-1 if you really thrash the mouse wheel.
1.0.1	Put Carbon-specific code inside #if PP_Target_Carbon blocks.

*********** Licensing  *************
This code is placed "as is" in the public domain in the hope that it may be useful.
You may use and modify it free of charge, however it comes with no warranty whatsoever.
It is your responsibility to determine its fitness for use and no liability, whether 
express or implied, will be accepted.
I would however appreciate being advised of any bug fixes or enhancements that 
you make.

*********** Credits  *************
The core code was generously provided by Matthew Vosburgh. 
I then added LScrollerView support and wrapped it as an attachment.

*********** Requirements  *************
PowerPlant version of CW7 early access or later.

*********** Usage *************
0. 	The changes to LScroller and LScrollerView that version 1.0 required are no longer
	necessary. If you made these changes, you can keep them or undo them as you see fit.

1.	Add the following files to your project:
	CScrollWheelAttachment.cp
	LEventHandler.cp
	
2.	In your applications's Initialize() override, call:
	AddAttachment( new CScrollWheelAttachment );

*********** Remarks *************
It seems to me that there is an awful lot of common code between LScroller and LScrollerView
that could profitably be factored up into a common abstract base class.
*/

// Modified quite a bit by Isaac Wankerl to get the scrolling to behave
// as Apple's Mac OS X apps behave (i.e. Finder, iTunes, Project Builder).

#pragma once

#include <LAttachment.h>
#if PP_Target_Carbon
	#include <TEventHandler.h>
#endif	

class	CScrollWheelAttachment : public LAttachment {
public:
	enum	{kScrollWheelFactor = 3}; // semi-official guideline 

					CScrollWheelAttachment();
	virtual			~CScrollWheelAttachment();

protected:
#if PP_Target_Carbon
	OSStatus		HandleScroll(
								EventHandlerCallRef	inCallRef,
								EventRef			inEventRef);
	bool			DoScrollEvent(
									Boolean isVertical, 
									SInt32 delta, 
									Point mouseLoc,
									UInt32	inModifiers);
#endif	

private:
#if PP_Target_Carbon
	TEventHandler<CScrollWheelAttachment>	mScrollHandler;
#endif	
	
	// disallowed methods
	CScrollWheelAttachment( CScrollWheelAttachment& );
	CScrollWheelAttachment& operator= (const CScrollWheelAttachment&);
};