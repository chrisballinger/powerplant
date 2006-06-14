// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				RefCountedObject.cp

CLASSES:			SURefCountMixin

DESCRIPTION:		Implementation file for Object

AUTHOR:				Eric Shapiro

CREATION DATE :		96.4.18

CHANGE HISTORY :

		96.4.18		ebs	Integrated into Robin's sources

---------------------------------------<< ¥ >>-----------------------------------------
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "SURefCountMixin.h"

/*=============================
	Constructor
==============================*/
SURefCountMixin::SURefCountMixin()
{
	mRefCount = 1;					// since someone created us
}

/*=============================
	Destructor
==============================*/
SURefCountMixin::~SURefCountMixin()
{
}

/*=============================
	IncrementRefCount
==============================*/
void SURefCountMixin::IncrementRefCount()
{
	++mRefCount;
}

/*=============================
	DecrementRefCount
	
	Note:
	Deletes the object if the count goes to zero.
==============================*/
void SURefCountMixin::DecrementRefCount()
{
	--mRefCount;
	if ( mRefCount == 0 )
		delete this;
}
