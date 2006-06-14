// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:					SUMiscUtils.h

CLASSES:				SUMiscUtils

DESCRIPTION:		

AUTHOR:				Eric Shapiro

CREATION DATE :	96.4.18

CHANGE HISTORY :

		96.4.18		ebs	Integrated into Robin's sources
	
---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

	This class is just a container for a bunch of utility routines.
	
---------------------------------------<< ¥ >>----------------------------------------
*/

#pragma once

#include "SU_Types.h"
#include "SU_Constants.h"
#include "SU_Macros.h"

/*============================================
	A bunch of random utility functions by Eric Shapiro consolidated
==============================================*/
class SUMiscUtils
{
	public:

		/***************************************
			Memory
		****************************************/
		static void			ClearMemory( void *, MemSizeT inNumBytes );
		static void			SetMemory( void *p, UInt8 toWhat, MemSizeT numBytes );
		static Handle		DuplicateHandle( Handle source );
		static void			DisposeHandle( void *h );
		
		/***************************************
			Mac stuff
		****************************************/
		static Boolean		IsShiftKey();
		static Boolean		IsOptionKey();
		static void			GetWindowGlobalBounds( WindowPtr inWindow, Rect & );
		
		/***************************************
			PowerPlant
		****************************************/
		static void			SetPort( GrafPtr );
		static void			SetGWorld( CGrafPtr, GDHandle );
		
		static void			GetMouseRelativeToPort( Point * );
		static void			GetMouseRelativeToView( LPane *, Point * );
		
		static LView*		GetTopView( LPane *inPane );
		static LPane*		FindSibblingPaneByID( LPane *, PaneIDT );

		static void 		LinkListenerToControls( LListener *, LView *, ResIDT );
		static void			RedrawPaneAsIndicated( LPane *, ERedrawOptions );
		static LWindow*		CreatePPWindow( ResIDT inWindowID );
		
		static GDHandle		FindDeviceViaPoint( Point inGlobalPt );
		static void			ForceRectToScreen( const Rect &inRect, Rect *outRect, GDHandle inScreen = nil );
};


