// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				PT_Resources.h

CLASSES:			

DESCRIPTION:		Resource id values for icon editor

AUTHOR:				Eric Shapiro

CREATION DATE :		96.4.25

CHANGE HISTORY :

		96.11.12	ebs		Text cursor added
		96.4.25		ebs		Integrated into Robin's sources
	
---------------------------------------<< ¥ >>----------------------------------------
*/

#pragma once

/*==========================================
	Icon Editor Views
===========================================*/
const ResIDT	PPob_IconSuiteEditor		= 9000;
const ResIDT	PPob_CICNEditor				= 9001;
const ResIDT	PPob_ICONEditor				= 9002;
const ResIDT	PPob_PictEditor				= 9003;
const ResIDT	PPob_CursorEditor			= 9004;
const ResIDT	PPob_ColorCursorEditor		= 9005;
const ResIDT	PPob_PatternEditor			= 9006;
const ResIDT	PPob_PixPatEditor			= 9007;

/*==========================================
	Undo strings
		Redo strings are at 9000.
		Undo strings are at 9001.
===========================================*/
const ResIDT	STRx_PaintUndo		= 	9000;
const SInt16	str_UndoLine		=	1;
const SInt16	str_UndoBucket		=	2;
const SInt16	str_UndoRect		=	3;
const SInt16	str_UndoRoundRect	=	4;
const SInt16	str_UndoOval		=	5;
const SInt16	str_UndoEraser		=	6;
const SInt16	str_UndoPen			=	7;
const SInt16	str_UndoMove		=	8;
const SInt16	str_UndoFlip		=	9;
const SInt16	str_UndoRotate		=	10;
const SInt16	str_UndoGeneric		=	11;
const SInt16	str_UndoEraseAll	=	12;
const SInt16	str_UndoCut			=	13;
const SInt16	str_UndoPaste		=	14;
const SInt16	str_UndoClear		=	15;
const SInt16	str_UndoTransparent	=	16;
const SInt16	str_UndoDrag		=	17;
const SInt16	str_UndoHotSpot		= 	18;
const SInt16	str_UndoRecolor		=	19;
const SInt16	str_UndoResize		=	20;
const SInt16	str_UndoDeleteImage	=	21;
const SInt16	str_UndoText		=	22;

/*===================================================
	Misc
====================================================*/
const ResIDT	Txtr_PaintFont		=	9000;		// default font/size/style for text tool
const ResIDT	RidL_ToolList		=	9100;		// note: Constructor may create own Ridl 9000 -- don't conflict
const ResIDT	MenuID_PaintOptions	=	9000;
const ResIDT	MenuID_PaintColors	=	9001;
const ResIDT	MenuID_PaintFont	=	9002;
const ResIDT	MenuID_PaintStyle	=	9003;

const ResIDT	ColorTable_IconColors = 9000;

const ResIDT	STRx_PaintEdit		=	9002;
const SInt16	str_PaintCut		=	1;
const SInt16	str_PaintCopy		=	2;
const SInt16	str_PaintPaste		=	3;
const SInt16	str_PaintClear		=	4;

/*===================================================
	These are the image types we support
====================================================*/
const OSType	ImageType_LargeIcon				= 'ICON';

const OSType	ImageType_LargeIconWithMask		= 'ICN#';
const OSType	ImageType_LargeIconMask			= 'ICN#';
const OSType	ImageType_Large4BitIcon			= 'icl4';
const OSType	ImageType_Large8BitIcon			= 'icl8';

const OSType	ImageType_SmallIconWithMask		= 'ics#';
const OSType	ImageType_SmallIconMask			= 'ics#';
const OSType	ImageType_Small4BitIcon			= 'ics4';
const OSType	ImageType_Small8BitIcon			= 'ics8';

const OSType	ImageType_MiniIconWithMask		= 'icm#';
const OSType	ImageType_MiniIconMask			= 'icm#';
const OSType	ImageType_Mini4BitIcon			= 'icm4';
const OSType	ImageType_Mini8BitIcon			= 'icm8';

const OSType	ImageType_ColorIcon				= 'cicn';
const OSType	ImageType_Picture				= 'PICT';

const OSType	ImageType_Pattern				= 'PAT ';
const OSType	ImageType_PixPat				= 'ppat';
const OSType	ImageType_Cursor				= 'CURS';
const OSType	ImageType_ColorCursor			= 'crsr';

const OSType	ResourceType_Region				= 'RgN ';	// for clipboard (register this ???)

/*==========================================
	Cursors
===========================================*/
const ResIDT	CURS_Lasso			=	9000;
const ResIDT	CURS_Eraser			=	9001;
const ResIDT	CURS_Pen			=	9002;
const ResIDT	CURS_Dropper		=	9003;
const ResIDT	CURS_Hand			=	9004;
const ResIDT	CURS_Plus			=	9005;
const ResIDT	CURS_Bucket			=	9006;
const ResIDT	CURS_Text			=	9007;
const ResIDT	CURS_HotSpot		=	9011;
