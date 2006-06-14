// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1996 - 1997 Metrowerks Corp. All rights reserved

---------------------------------------<< * >>----------------------------------------

FILE:					GAResizeBar.h

CLASSES:				GAResizeBar

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.06.22

CHANGE HISTORY :

		96.06.22		rtm	Initial creation of file
	
---------------------------------------<< * >>----------------------------------------

	THEORY OF OPERATION

This class is the main hub for delivering a simplified resizable pane system that can
easily be laid out using the Layout Editor within Constructor.

This class is used in conjunction with the GAResizeBorder class and also the GAShadowBorder
class.

This class uses a grayscale appearance as specified in AGA for the rendering including
the default background color.  The color change depending on the state of the bar so
that when it is inactive or disabled it has a different appearance.

It is an LPane subclass that should always be embedded in the view that contains the
resize borders with which it is associated, in most cases this would be another
resize border class - GAResizeBorder.  This works this way so that the tracking
for the resizing process can have a view in which to render its tracking feedback. In
addition the resize bar is an LBroadcaster subclass so that it can broadcast messages
to any listeners associated with it.

The resize bar has a custom pane type that allows the specification of two resize
borders with which it is associated, these are the two views that will be resized
when the resize bar changes position.  The two views are used as follows: the first
one is either the top or left view depending on whether the resize bar is horizontal
or vertical, where horizontal means the bar renders a horizontal resize bar or vise
versa for vertical.  A horizontal bar when moved will result in the associated views
changing size vertically and vise versa for vertical bars.

The resizing mechanism is triggered by the moving of the bar, except when the bar is
disabled.  The way this trigger is implemented is as an override of the LPane method
MoveBy() which in turn calls the method AdjustPanes() to handle the actual resizing
of the associated panes.  Whenever the trigger is fired it also broadcasts a message
which consists of the mResizeMessage specified for the bar and the delta value of a
drag of the bar, where the delta is calculated as the distance between the new and old
locations of the bar.

The above mechanism can be used by a window that wishes to implement smart distribution
of screen real estate between its subpanes, as it can calculate the locations for the
resize bar[s] and then simply move them in order to trigger the resize of the bar's
associated views.

Resize bars have the ability to render their backgrounds or not so they can easily be
placed over other views or between them whatever makes the most sense, they should 
however be embedded in the view into which their associated views are embedded as they
are dependent on the coordinate system of their host for tracking during the resizing
process.

When laying out resize bars in Constructor if they are meant to overlay portion of 
another view they should be moved in the Hierarchy Window such that they are the last
to draw so that they appear over the view.

When rendering the resize bar always renders the actual bar in the center of the pane 
for the given direction of the bar.  This allows the bar to be wide in order to provide
fill with its background color or narrow as needed for different situations.  The 
position of the ends of the actual bar can be controlled using the end insets allowing
the positioning of the bar to be altered. This could be used for instance to provide
space for some other control that was overlaid over a portion of the bar.

The resize bar also support the ability to have its orientation altered at runtime
which will result in the bar and its associated panes being rearanged to meet the new
requirements.  WARNING:  if you use this feature make sure that your resize bars have
been laid out in such a manner that they are spaced between the panes they are resizing
otherwise when their orientation is altered they will setup the bars that way.  If you
need your bars to be overlaid then you will have to modify the SetOrientation() method
which is where this all happens.  The main change involved will be determining the
positioning of bars and their associated panes.

Resize bars also have a couple of utility methods that can be called to extract info
that can be used to save the current location of the resize bar.  This can then later
be used to restore the position of the resize bar.

NOTE:

You do not have to utilize the resize borders adjustment mechanism, it can simply be
turned off by setting the appropriate flag.  The user could then implement an
entirely different mechanism if needed that could be driven from somewhere other than
the resize border.

LAYOUT RULES:

Here are some Constructor layout rules that you should bear in mind when using the 
pane system:

1É	Always try to keep your borders positioned within their superview such that they
butt up to the various edges where appropriate
2É	If using the orientation alteration feature, then layout your panes with the resize
bars between the panes they manipulate, not overlaid over them.
3É	A resize border can only handle the adjusting of panes in one direction, this means
that you have to use embedding in certain cases. 

EXAMPLE: For a typically three list browser the following layout would be used to
implement it.

			+---------------------------GAResizeBorder--ADJUSTS-VERTICALLY----+
			|																						|
		+-----------------------GAResizeBorder--ADJUSTS-HORIZONTALLY------+	|
		|																						|	|
	+--GAResizeBorder--+ +----GAResizeBorder---+	+---GAResizeBorder---+	|	|
	|			  			 |	|							 |	|							|	|	|
	|			1			 |	|				2			 |	|			3				|	|	|
	|						 |	|							 |	|							|	|	|
	|	Non-adjusting	 |	|		Non-adjusting	 |	|	 Non-adjusting		|	|	|
	|						 |	|							 |	|							|	|	|
	|						 |	|							 |	|							|--+	|
	|						 |	|							 |	|							|		|
	+------------------+	+---------------------+ +--------------------+		|
	======GAResizeBar==================================================		|
	+------------------------GAResizeBorder---------------------------+		|
	|																						|		|
	|																						|		|
	|										4												|		|
	|																						|		|
	|								Non-adjusting										|		|
	|																						|		|
	|																						|-----+
	|																						|
	+-----------------------------------------------------------------+	
	
In this illustration there are only two of the borders that are set to handle the
adjusting of panes, the one in the back is the container for the entire browser and 
handles the vertically adjustment of the borders that are embedded within it. It
has the second adjusting border embedded within it as well as the horizontal resize
bar and the lower border (4).  Borders (1), (2), and (3) are in turn embedded within
the second adjustable border, which handles adjusting these borders horizontally, it
also has two vertical resize bars embedded within it between borders (1) and (2), and
between (2) and (3).

The horizontal resize bar has its first pane set to be the invisible border that is
set to adjust horizontally, its second pane is setup to be border (4).

Hope this makes at least some sense!!

The basic concept is that a resize bar only ever manipulates two borders, one on 
either side.  So if you have a situation that requires a more compilcated layout
you have to use embedding to accomplish your requirements while observing this
rule.

4É	NEVER embed something like a palette [on the top or the left edge] inside one of
the invisible resize borders as the resizing feedback might be thrown off.  Instead I
would recommend embedding the palette in the window or a view into which the border
has been embedded with allowance for the palette. There is no problem embedding anything
within a border that does not handle adjusting panes [which means it does not have any
resize bars embedded within it].

5É The other tricky thing with this system is correctly establishing the appropriate
bindings between the various panes.  Panes on the bottom and left edges should always
have all of their bindings turned on.  All other panes should have any edge that touches
a window edge turned on as well as any edge that touches any edge of a view into which
they are embedded that has the corresponding edge turned on.

EXAMPLE: using the above illustration

	BORDER 1 - top, left, bottom
	BORDER 2 - top, bottom
	BORDER 3 - top, left, bottom, and right - so that it grows with the window
	BORDER 4 - top, left, bottom, and right - so that it grows with the window
	MAIN Adjustable Border - top, left, bottom, and right
	SECOND Adjustable Border - top, left, right
	
	
FUTURES:

1) Potentially implement a version of the resize bar that can have a relationship with
multiple views so that it could be used to construct a multipane browser.  NOT NEEDED
SEE BELOW!

This might not really be needed as there are other ways of getting around the problem 
of additional panes 
Example: using embedding so that a three pane browser would have the upper three panes
embedded in a resize border that itself contained the three panes, the horizontal 
resize bar in this case would still be manipulating only two views.  The one above 
containing the three panes and the large one below.

2) Specification of the background color through the custom pane type.

3) GAShadowBorder - this class will be subclassed to create GAResizerView which will
have the ability to do proportional distribution of window real estate between its
subpanes, in conjunction with the resize bar. DECIDED TO ADD IT INTO THE
GARESIZEBORDER CLASS INSTEAD!!

4) GAFocusBorder - this might need to be changed so that it can be setup to do no
rendering at all, which is something that would be needed for the three pane example
given in item (1).  PUT THIS IN GARESIZEBORDER AS AN OVERRIDE!!

---------------------------------------<< * >>----------------------------------------
*/

#pragma once


#include "GAResizeBorder.h"


//==================================================================================== 
//	CONSTANTS
//==================================================================================== 

enum
{
	resizeBar_MinWidth			=	3,
	resizeBar_MinInset			=	5,
	resizeBar_MinResize			=	65,
	resizeBar_HResizeCursor		=	30000,
	resizeBar_VResizeCursor		=	30001
};

//==================================================================================== 
//	FORWARD DECLARATIONS
//==================================================================================== 

class LPane;
class	LBroadcaster;
class GAResizeBorder;

//==================================================================================== 
//	FUNCTIONS
//==================================================================================== 

//==================================================================================== 
//	GAResizeBar
//==================================================================================== 
	
class GAResizeBar : public LPane,
							public LBroadcaster
{

	//====<< * CLASS ID * >>===========================================================

public:

	enum { class_ID = 'Grsz' };
	
	
	//====<< * FIELDS * >>=============================================================

protected:	
	
	PaneIDT		mFirstPaneID;		//	ID for the first pane which is either the left or
											//		the top pane depending on whether the bar is a
											//		vertical or horizontal bar
	PaneIDT		mSecondPaneID;		//	ID for the second pane which is either the right or
											//		bottom pane depending on whether the it is a
											//		vertical or horizontal bar
	Point			mEndInsets;			//	Allows inset to be specified for either end of the
											//		the bar where the h & v values apply to the left
											//		and right for a horizontal bar and top and bottom
											//		for a vertical one
	Boolean		mPaintBackground;	//	This flag is used to tell the bar if it should be
											//		responsible for drawing the background color or
											//		not, this can be useful if the bar is actually
											//		being used as a separator between two panes or
											//		if it is just being drawn over another pane that
											//		already is taking care of the background drawing
	MessageT		mResizeMessage;	//	Message to be sent when the bar changes its
											//		position after being dragged
	Boolean		mHorizontal;		//	Flag used to indicate whether the bar is horizontal
											//		or vertically oriented, this is calculated by
											//		looking at the relationship between the width
											//		and height of the bar
	GAResizeBorder*	mFirstPane;			//	Reference to the first pane
	GAResizeBorder*	mSecondPane;		//	Reference to the second pane

	// * This is used internally
	Int16			mInitialMouseOffset;	//	This is the offset of the mouses initial down
												//		when tracking it is potentially used to
												//		correctly calculate the feedback rect this
												//		offset is the distance from either the left
												//		or the top edge of the bar depending on the
												//		bars orientation

private:

	PaneIDT		mZoomCommanderID;	//	ID for the commander to which the border resize
											//		manager has been attached
	
												
												
	//====<< * METHODS * >>============================================================
		
	//----<< * INITIALIZATION & DISPOSAL * >>------------------------------------------

public:

							GAResizeBar 			();								//	* Constructor
							GAResizeBar 			(	LStream*		inStream );	//	* Constructor
	virtual				~GAResizeBar 			();								//	* Desctructor
								
	static	GAResizeBar*	CreateGAResizeBarStream	(	LStream*		inStream );
	
	virtual	void		FinishCreateSelf		();									//	* OVERRIDE
	
	//----<< * ACCESSORS * >>----------------------------------------------------------

	// ** GETTERS
	
	virtual	GAResizeBorder*	GetFirstPane			() const
													{	return mFirstPane;	};
	virtual	GAResizeBorder*	GetSecondPane			() const
													{	return mSecondPane;	};
	virtual	Boolean	IsHorizontal			() const
													{	return mHorizontal;	};
	virtual	Point		GetEndInsets			() const
													{	return mEndInsets;	};
	virtual	Boolean	DoesPaintBackground	() const
													{	return mPaintBackground;	};
													
	virtual	MessageT	GetResizeMessage		() const
													{	return mResizeMessage; };
													
	virtual	ETriState	GetEnabledState	() const
													{	return mEnabled;	};
													
	virtual	void		CalcLocalResizeBarRect	(	Rect	&outRect );
	virtual	void		CalcResizeFeedbackRect	(	Rect	&inResizeRect,
																Int16	inPosition );										
	virtual	void		CalcMinAndMaxConstraints	(	Int16		&outMin,
																	Int16		&outMax );	
	
	// * Utility
	
	virtual	LWindow*	GetContainingWindow		();
	
	// These getters are used to get dimensions for the associated panes
	virtual	Int16		GetFirstPaneWidth			();
	virtual	Int16		GetFirstPaneHeight		();
	virtual	Int16		GetSecondPaneWidth		();
	virtual	Int16		GetSecondPaneHeight		();
	
	virtual	Int16		GetFirstPaneMinWidth		();
	virtual	Int16		GetFirstPaneMinHeight	();
	virtual	Int16		GetSecondPaneMinWidth	();
	virtual	Int16		GetSecondPaneMinHeight	();
	
	virtual	Int16		GetFirstPaneOffset		();
	virtual	Int16		GetSecondPaneOffset		();
	
	virtual	Boolean	IsFirstPaneHeightObscured	();
	virtual	Boolean	IsFirstPaneWidthObscured	();
	virtual	Boolean	IsSecondPaneHeightObscured	();
	virtual	Boolean	IsSecondPaneWidthObscured	();
																										
	// ** SETTERS
	
	virtual	void		SetFirstPane			(	GAResizeBorder*	inFirstPane )
													{	mFirstPane = inFirstPane;	};
	virtual	void		SetSecondPane			(	GAResizeBorder*	inSecondPane )
													{	mSecondPane = inSecondPane;	};
	virtual	void		SetEndInsets			(	Point		inEndInsets,
															Boolean	inRedraw );
	virtual	void		SetPaintBackground	(	Boolean	inPaint,
															Boolean	inRedraw );
	virtual	void		SetOrientation			(	Boolean	inHorizontal,
															Int16		inNewPosition,
															Int16		inBarCount,
															Int16		inCurrBarIndex );
															
	virtual	void		ResetPaneReferences	();
	
	
	//----<< * DRAWING * >>------------------------------------------------------------

	virtual	void		DrawSelf					();									//	* OVERRIDE
	virtual	void		DrawResizeBar			(	Int16		inBitDepth );
	virtual	void		DrawResizeFeedback	( 	Rect	&inFrame,
															Int16	inPosition );
	
	
	//----<< * MOVING * >>-------------------------------------------------------------

	virtual 	void		MoveBy					(	Int32 inHorizDelta, 
															Int32 inVertDelta,
															Boolean inRefresh	);			// * OVERRIDE
	virtual	void		PositionResizeBarAt	(	Int32		inPosition,
															Boolean	inRefresh );
															
															
	//----<< * RESIZE BAR INFO * >>----------------------------------------------------

	virtual 	Handle	GetResizeBarInfo		();
	virtual 	void		SetResizeBarInfo		(	Handle	inResizeBarInfoH );
	

	//----<< * MOUSE TRACKING * >>-----------------------------------------------------

	virtual void		ClickSelf				(	const SMouseDownEvent &inMouseDown );
																								//	* OVERRIDE

	virtual	void		AdjustPanes				(	Int32 inDelta );
	
	
	//----<< * CURSOR TRACKING * >>----------------------------------------------------

	virtual void		AdjustCursorSelf		(	Point inPortPt,
															const EventRecord &inMacEvent );
																								// * OVERRIDE
																								


};	// GAResizeBar
