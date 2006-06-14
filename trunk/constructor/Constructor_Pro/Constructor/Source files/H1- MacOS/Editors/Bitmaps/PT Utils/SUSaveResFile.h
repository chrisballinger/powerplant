// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				SUSaveResFile.h

CLASSES:			StSaveResFile

DESCRIPTION:		

AUTHOR:				Eric Shapiro

CREATION DATE :		96.4.18

CHANGE HISTORY :

		96.4.18		ebs	Integrated into Robin's sources
	
---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

	This stack-based class is used to save/restore the current
	resource file.
	
	Note:
	Be sure not to close the current resource file within a routine that
	uses this class, since we will restore a now-invalid resource file id
	upon exit.
	
---------------------------------------<< ¥ >>----------------------------------------
*/

#pragma once

class StSaveResFile
{
	public:
						StSaveResFile();
						StSaveResFile( SInt32 rezFileID );		// save old & set new
		virtual			~StSaveResFile();
	
	protected:
		SInt32			mOldResFileID;
};

