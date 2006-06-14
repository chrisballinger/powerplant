// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1996 Metrowerks Corp. All rights reserved

---------------------------------------<< ¥ >>-----------------------------------------

FILE:					LPattern.cp

CLASSES:				LPattern

DESCRIPTION:		Implementation file for LPattern

AUTHOR:				Robin Mair

CREATION DATE :	96.08.21

CHANGE HISTORY :

		96.06.28		rtm	Initial creation of file

---------------------------------------<< ¥ >>-----------------------------------------
*/

#include "LPattern.h"
#include "UGAColorRamp.h"


//=====================================================================================
// CONSTANTS
//=====================================================================================

//=====================================================================================
// DEFINES
//=====================================================================================

#pragma mark === CLASS: LPattern

//=====================================================================================
// ¥¥ STREAM CREATION
//-------------------------------------------------------------------------------------
// LPattern::CreateLPatternStream	
//-------------------------------------------------------------------------------------
//

LPattern*	
LPattern::CreateLPatternStream ( LStream *inStream )
{

	return ( new LPattern ( inStream ));
	
}	// LPattern::CreateLPatternStream


#pragma mark -
#pragma mark === INITIALIZATION

//=====================================================================================
// CLASS:	LPattern
//=====================================================================================

//=====================================================================================
// ¥¥ INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// LPattern::LPattern					---<<¥ CONSTRUCTOR ¥>>---
//-------------------------------------------------------------------------------------

LPattern::LPattern ()
{

	mPixPatID = 0;
	
}	//	LPattern::LPattern


//-------------------------------------------------------------------------------------
// LPattern::LPattern					---<<¥ CONSTRUCTOR ¥>>---
//-------------------------------------------------------------------------------------

LPattern::LPattern (	LStream*	inStream ) : LPane( inStream )
{
	
	// ¥ We read this but don't do anything with it
	*inStream >> mPixPatID;

}	//	LPattern::LPattern


//-------------------------------------------------------------------------------------
// LPattern::~LPattern				---<<¥ DESTRUCTOR ¥>>---
//-------------------------------------------------------------------------------------

LPattern::~LPattern ()
{
}	//	LPattern::~LPattern


#pragma mark -
#pragma mark === DRAWING

//=====================================================================================
// ¥¥ DRAWING
//-------------------------------------------------------------------------------------
// LPattern::DrawSelf
//-------------------------------------------------------------------------------------

void
LPattern::DrawSelf ()
{

	StColorPenState	thePenState;
	
	Rect localFrame;
	CalcLocalFrameRect ( localFrame );
	PixPatHandle pixPatH = GetPixPat ( mPixPatID );
	if ( pixPatH )
	{
		::FillCRect ( &localFrame, pixPatH );
		::ReleaseResource ( (Handle)pixPatH );
	}
		
}	//	LPattern::DrawSelf


