// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				SaveResFile.cp

CLASSES:			StSaveResFile

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

#include "SUSaveResFile.h"

/*===================================
	Constructor #1
====================================*/
StSaveResFile::StSaveResFile()
{
	mOldResFileID = ::CurResFile();
}

/*===================================
	Constructor #2
====================================*/
StSaveResFile::StSaveResFile( Int32 rezFileID )
{
	mOldResFileID = ::CurResFile();
	::UseResFile( rezFileID );
}

/*===================================
	Destructor
====================================*/
StSaveResFile::~StSaveResFile()
{
	::UseResFile( mOldResFileID );
}

