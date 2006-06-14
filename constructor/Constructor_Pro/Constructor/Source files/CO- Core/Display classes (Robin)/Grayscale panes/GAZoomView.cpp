// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1996 Metrowerks Corp. All rights reserved

---------------------------------------<< * >>-----------------------------------------

FILE:					GAZoomView.cp

CLASSES:				GAZoomView

DESCRIPTION:		Implementation file for GAZoomView

AUTHOR:				Robin Mair

CREATION DATE :	96.11.23

CHANGE HISTORY :

		96.11.23		rtm	Initial creation of file

---------------------------------------<< * >>-----------------------------------------
*/

#include "GAZoomView.h"

// * GRAYSCALE HEADERS

// * POWERPLANT HEADERS


//=====================================================================================
// CONSTANTS
//=====================================================================================

//=====================================================================================
// DEFINES
//=====================================================================================

#pragma mark === CLASS: GAZoomView

//=====================================================================================
// ** STREAM CREATION
//-------------------------------------------------------------------------------------
// GAZoomView::CreateGAZoomViewStream	
//-------------------------------------------------------------------------------------
//

GAZoomView*	
GAZoomView::CreateGAZoomViewStream ( LStream *inStream )
{

	return ( new GAZoomView ( inStream ));
	
}	// GAZoomView::CreateGAZoomViewStream


#pragma mark -
#pragma mark === INITIALIZATION

//=====================================================================================
// CLASS:	GAZoomView
//=====================================================================================

//=====================================================================================
// ** INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// GAZoomView::GAZoomView					---<<* CONSTRUCTOR *>>---
//-------------------------------------------------------------------------------------

GAZoomView::GAZoomView ()
{
}	//	GAZoomView::GAZoomView


//-------------------------------------------------------------------------------------
// GAZoomView::GAZoomView					---<<* CONSTRUCTOR *>>---
//-------------------------------------------------------------------------------------

GAZoomView::GAZoomView (	LStream*	inStream ) : LView ( inStream )
{
}	//	GAZoomView::GAZoomView


//-------------------------------------------------------------------------------------
// GAZoomView::~GAZoomView				---<<* DESTRUCTOR *>>---
//-------------------------------------------------------------------------------------

GAZoomView::~GAZoomView ()
{
}	//	GAZoomView::~GAZoomView

