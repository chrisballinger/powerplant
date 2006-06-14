// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				PaintViewConstants.h

PROJECT:			Constructor Icon Editor

CLASSES:			

DESCRIPTION:		Command & Pane id values for icon editor

AUTHOR:				Eric Shapiro

CREATION DATE :		96.4.25

CHANGE HISTORY :

		96.11.18	ebs		Added msg_TextActionDied
		96.11.11	ebs		Text edit tool
		96.4.25		ebs		Integrated into Robin's sources
	
---------------------------------------<< ¥ >>----------------------------------------
*/

#pragma once

/*===================================================
	These are both the Pane ID values and the command id's associated
	with the various drawing tools. The ones underneath don't have panes.
====================================================*/
const PaneIDT	Tool_None				= 0;
const PaneIDT	Tool_Lasso				= 'LASS';
const PaneIDT	Tool_Selection			= 'SELE';
const PaneIDT	Tool_Eraser				= 'ERAS';
const PaneIDT	Tool_Pen				= 'PEN ';
const PaneIDT	Tool_Dropper			= 'EYED';
const PaneIDT	Tool_Bucket				= 'BUCK';
const PaneIDT	Tool_Line				= 'LINE';
const PaneIDT	Tool_Rect				= 'RECT';
const PaneIDT	Tool_FilledRect			= 'FREC';
const PaneIDT	Tool_RoundRect			= 'RREC';
const PaneIDT	Tool_FilledRoundRect	= 'FRRE';
const PaneIDT	Tool_Oval				= 'OVAL';
const PaneIDT	Tool_FilledOval			= 'FOVA';
const PaneIDT	Tool_ForegroundColor	= 'FORE';
const PaneIDT	Tool_BackgroundColor	= 'BACK';		
const PaneIDT	Tool_Pattern			= 'PAT ';
const PaneIDT	Tool_Rotate				= 'ROTA';
const PaneIDT	Tool_BlackAndWhite		= 'BWBW';
const PaneIDT	Tool_SwapColors			= 'SWAP';
const PaneIDT	Tool_HotSpot			= 'HOTS';
const PaneIDT	Tool_Text				= 'TEXT';

const PaneIDT	PaneID_BoxAroundCanvas	= 'BITE';
const PaneIDT	PaneID_SampleWell		= 'SAMP';

const CommandT	cmd_MoveSelection		= 'MOVE';
const CommandT	cmd_EraseAll			= 'ERAL';
const CommandT	cmd_FlipVertical		= 'FLPV';
const CommandT	cmd_FlipHorizontal		= 'FLPH';
const CommandT	cmd_RotateClockwise		= 'RTCL';
const CommandT	cmd_RotateCounter		= 'RTCC';
const CommandT	cmd_Transparent			= 'TRAN';

const CommandT	cmd_DragImage			= 'DRAG';
const CommandT	cmd_PaintModified		= 'PMOD';
const CommandT	cmd_RecolorCurrentImage	= 'PTRC';
const CommandT	cmd_ResizeImage			= 'PTSZ';
const CommandT	cmd_DeleteImage			= 'PTDL';

const MessageT	msg_TextActionDied		= 'TxDi';
const CommandT	cmd_FirstFontSize		= 9100;		// font sizes are 9100-cmdID
const CommandT	cmd_LastFontSize		= 9300;		//  (and within this range)
const CommandT	cmd_OtherFontSize		= 9100;		//  "Other..."

enum ENudgeDirection{ nudge_Up, nudge_Down, nudge_Left, nudge_Right };
