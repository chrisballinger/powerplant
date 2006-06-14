// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:					DisposeOnExit.cp

CLASSES:				StDisposeHandle

DESCRIPTION:		Implementation file for Object

AUTHOR:				Eric Shapiro

CREATION DATE :	96.4.18

CHANGE HISTORY :

		96.4.18		ebs	Integrated into Robin's sources

---------------------------------------<< ¥ >>-----------------------------------------
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "SUDisposeOnExit.h"
#include "SUMiscUtils.h"

/*==================
	Constructor
===================*/
StDisposeHandle::StDisposeHandle( Handle h )
{
	mHandle = h;
}

/*==================
	Destructor
===================*/
StDisposeHandle::~StDisposeHandle()
{
	if ( mHandle )
		SUMiscUtils::DisposeHandle( mHandle );
}

