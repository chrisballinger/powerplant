// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:					SUSparePort.cp

CLASSES:				SUSparePort

DESCRIPTION:		Implementation file for Object

AUTHOR:				Eric Shapiro

CREATION DATE :	96.4.18

CHANGE HISTORY :

		96.4.18		ebs	Integrated into Robin's sources

---------------------------------------<< ¥ >>-----------------------------------------
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "SUSparePort.h"

/*==========================================
	static variables
===========================================*/
Boolean 		SUSparePort::sInitted = false;
#if PP_Target_Carbon
CGrafPtr		SUSparePort::sPortPtr = NULL;
#else
GrafPort 		SUSparePort::sPort;
#endif

/*==========================================
	GetSparePort
===========================================*/
GrafPtr SUSparePort::GetSparePort()
{
	if ( !sInitted )
	{
#if PP_Target_Carbon
		sPortPtr = CreateNewPort();
#else
		::OpenCPort( (CGrafPort*)&sPort );
		ThrowIfMemFail_( sPort.visRgn );		// see old tech note
#endif
		sInitted = true;
	}

#if PP_Target_Carbon
	return sPortPtr;
#else
	return( &sPort );
#endif
}

/*==========================================
	Cleanup
===========================================*/
void SUSparePort::Cleanup()
{
	if ( sInitted )
	{
#if PP_Target_Carbon
		DisposePort(sPortPtr);
#else
		ClosePort( &sPort );
#endif
		sInitted = false;
	}
}