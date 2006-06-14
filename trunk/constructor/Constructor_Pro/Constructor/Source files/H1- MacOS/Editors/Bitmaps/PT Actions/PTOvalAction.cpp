// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				OvalAction.cp

PROJECT:			Constructor Icon Editor

CLASSES:			COvalAction

DESCRIPTION:		Implementation file for class
					See also RectAction.cp
					
AUTHOR:				Eric Shapiro

CREATION DATE :		96.4.23

CHANGE HISTORY :

		96.4.23		ebs		Integrated into Robin's sources

---------------------------------------<< ¥ >>-----------------------------------------
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "PT_Headers.h"
#include "PTActions.h"

/*=====================================
	Constructor
======================================*/
COvalAction::COvalAction( const SPaintAction &inAction, Boolean inFill ) : 
		PTRectAction( inAction, inFill, str_UndoOval )
{
}

/*=====================================
	Destructor
======================================*/
COvalAction::~COvalAction()
{
}


/*=====================================
	COvalAction::DrawShape
======================================*/
void COvalAction::DrawShape( const Rect &r )
{
	if ( mFilled )
	{
		::PenPat( &mSettings.thePattern );
		::PaintOval( &r );
	}
	else
		::FrameOval( &r );
}

