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

DESCRIPTION:		Misc typedefs

AUTHOR:				Eric Shapiro

CREATION DATE :		96.4.25

CHANGE HISTORY :

		96.4.25		ebs		Integrated into Robin's sources
	
---------------------------------------<< ¥ >>----------------------------------------
*/

#pragma once

typedef UInt32	PTResizeOptions;

const UInt32	PTResize_None			= 0x00;
const UInt32	PTResize_Canvas			= 0x01;
const UInt32	PTResize_MoveSampleBox	= 0x02;
const UInt32	PTResize_Window			= 0x04;
const UInt32	PTResize_All			= 0xFF;

