// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				SaveGWorld.cp

CLASSES:			StSaveGWorld

DESCRIPTION:		Implementation file for Object

AUTHOR:				Eric Shapiro

CREATION DATE :		96.4.18

CHANGE HISTORY :

		96.4.18		ebs	Integrated into Robin's sources

---------------------------------------<< ¥ >>-----------------------------------------
*/

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "SUSaveGWorld.h"
#include "SUMiscUtils.h"			// for SetPort & SetGDevice

/*===================================
	Constructor #1
====================================*/
StSaveGWorld::StSaveGWorld()
{
	::GetGWorld( &mPort, &mDevice );
}

/*===================================
	Constructor #2
====================================*/
StSaveGWorld::StSaveGWorld( GWorldPtr inWorld )
{
	::GetGWorld( &mPort, &mDevice );
	SUMiscUtils::SetGWorld( inWorld, nil );
}

/*===================================
	Destructor
====================================*/
StSaveGWorld::~StSaveGWorld()
{
	if ( mPort )
		SUMiscUtils::SetGWorld( mPort, mDevice );
}
