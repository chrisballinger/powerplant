// Copyright �2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< � >>----------------------------------------

	Copyright� 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< � >>----------------------------------------

FILE:				PTUndoer.h

CLASSES:			PTUndoer

DESCRIPTION:		

AUTHOR:				Eric Shapiro

CREATION DATE :		96.4.18

CHANGE HISTORY :

		96.4.18		ebs	Integrated into Robin's sources
	
---------------------------------------<< � >>----------------------------------------

	THEORY OF OPERATION

	This class is the same as LUndoer, but it supports one additional
	message of use to the Icon Editor: "Get Last Command".
	
	This allows the Icon Editor to query the previous command to see
	if it is a "Move" command, since multiple move commands undo to
	the very first one, not the most recent one.
	
---------------------------------------<< � >>----------------------------------------
*/
 
#pragma once

const CommandT		cmd_GetLastCommand		= 'LCMD';


class PTUndoer : public LUndoer
{
	public:
		virtual void ExecuteSelf( MessageT inMessage, void *ioParam );
};

