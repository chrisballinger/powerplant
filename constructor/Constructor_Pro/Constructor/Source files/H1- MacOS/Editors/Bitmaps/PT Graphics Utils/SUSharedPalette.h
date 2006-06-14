// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				SUSharedPalette.h

CLASSES:			SUSharedPalette

DESCRIPTION:		

AUTHOR:				Eric Shapiro

CREATION DATE :		96.4.18

CHANGE HISTORY :

		96.4.18		ebs	Integrated into Robin's sources
	
---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

	This class is a wrapper for a Mac Palette that can be shared between
	more than one port.  Unlike color tables, palettes attached to windows,
	gworlds, and ports are not duplicated by the toolbox. In order to clean
	up after ourselves (esp in a plug-in), we need to keep track of the
	number of users of a given palette and delete it only when they are all
	through.
	
	An alternative would be just to allocate one palette per offscreen buffer,
	but this wastes time and RAM, especially since we have several offscreens 
	per window).
	
---------------------------------------<< ¥ >>----------------------------------------
*/

#pragma once

#include "SURefCountMixin.h"

/*=============================
	SUSharedPalette
==============================*/
class SUSharedPalette : public SURefCountMixin
{
	public:
								SUSharedPalette( CTabHandle, GrafPtr inPort = nil, short inUpdates = pmNoUpdates );
		virtual					~SUSharedPalette();
		virtual PaletteHandle	GetPalette();
		
		virtual void			AttachToPort( GrafPtr inPort, short inUpdates = pmNoUpdates );
		
	protected:
		PaletteHandle			mPaletteH;
};

