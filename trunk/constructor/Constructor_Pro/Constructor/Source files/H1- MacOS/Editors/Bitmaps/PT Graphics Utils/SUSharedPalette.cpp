// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				SharedPalette.cp

CLASSES:			SUSharedPalette

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

#include "SUSharedPalette.h"

/*========================
	Constructor
=========================*/
SUSharedPalette::SUSharedPalette( CTabHandle inTable, GrafPtr inPort, short inUpdates )
{
	ThrowIfNil_( inTable );
	
	mPaletteH = ::NewPalette( (**inTable).ctSize + 1, inTable, pmTolerant | pmExplicit, 0 );
	ThrowIfMemFail_( mPaletteH );
	
	if ( inPort )
		this->AttachToPort( inPort, inUpdates );
		
	mRefCount = 1;			// (change back from 2 if inPort != nil)
}

/*========================
	Destructor
=========================*/
SUSharedPalette::~SUSharedPalette()
{
	if ( mPaletteH )
		::DisposePalette( mPaletteH );
}

/*========================
	AttachToPort
=========================*/
void SUSharedPalette::AttachToPort( GrafPtr inPort, short inUpdates )
{
	if ( inPort && mPaletteH )
	{
		::NSetPalette( GetWindowFromPort(inPort), mPaletteH, inUpdates );	
		this->IncrementRefCount();
	}
}

/*========================
	GetPalette
=========================*/
PaletteHandle SUSharedPalette::GetPalette()
{
	return( mPaletteH );
}

