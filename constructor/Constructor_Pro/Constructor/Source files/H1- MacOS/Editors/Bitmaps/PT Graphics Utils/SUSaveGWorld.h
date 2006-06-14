// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				SUSaveGWorld.h

CLASSES:			StSaveGWorld

DESCRIPTION:		

AUTHOR:				Eric Shapiro

CREATION DATE :		96.4.18

CHANGE HISTORY :

		96.4.18		ebs	Integrated into Robin's sources
	
---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

	This stack-based class is used to save/restore the current
	GWorld within a function or block of code.
	
	Note:
	Be careful if you use this class in a function that disposes of
	GWorlds. You must make sure you don't dispose of the current
	one because this object will have no way of knowing and will
	blindly SetGWorld() back to it on exit.
	
---------------------------------------<< ¥ >>----------------------------------------
*/

#pragma once

class StSaveGWorld
{
	public:
						StSaveGWorld();
						StSaveGWorld( GWorldPtr inWorld );		// save old & set new
		virtual			~StSaveGWorld();
		
	protected:
		CGrafPtr		mPort;
		GDHandle		mDevice;
};

