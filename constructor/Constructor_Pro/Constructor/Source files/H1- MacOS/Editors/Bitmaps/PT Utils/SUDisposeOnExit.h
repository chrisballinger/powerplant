// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				SUDisposeOnExit.h

CLASSES:			StDisposeHandle

DESCRIPTION:		

AUTHOR:				Eric Shapiro

CREATION DATE :		96.4.18

CHANGE HISTORY :

		96.4.18		ebs	Integrated into Robin's sources
	
---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

	This stack-based class is just a handy way of making sure a handle 
	gets disposed of when a routine is exitted.

---------------------------------------<< ¥ >>----------------------------------------
*/

#pragma once

class StDisposeHandle
{
	public:
						StDisposeHandle( Handle );
		virtual			~StDisposeHandle();
		
	protected:
		Handle			mHandle;
};

