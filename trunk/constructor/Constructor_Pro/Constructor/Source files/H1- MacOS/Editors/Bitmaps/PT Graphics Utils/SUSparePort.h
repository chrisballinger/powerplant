// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:					SUSparePort.h

CLASSES:				SUSparePort

DESCRIPTION:		

AUTHOR:				Eric Shapiro

CREATION DATE :	96.4.18

CHANGE HISTORY :

		96.4.18		ebs	Integrated into Robin's sources
	
---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

	It is often convenient for Mac software to have a spare port
	available. For example:
	
	  OpenPicture(), OpenPoly(), OpenRgn(), etc, require ports
	  Getting a string's width requires a port
	  etc, etc
	
	Rather than creating a port every time we need one, we'll just
	create one and keep it around.

	Note: 
	The constructor for this object creates a port, so you shouldn't
	use one of these as a global variable or as a member of application,
	since the Mac toolbox won't have been initted yet.
---------------------------------------<< ¥ >>----------------------------------------
*/

#pragma once

class SUSparePort
{
	public:
		static GrafPtr			GetSparePort();
		static void				Cleanup();

	protected:
#if PP_Target_Carbon
		static CGrafPtr			sPortPtr;
#else
		static GrafPort			sPort;
#endif
		static Boolean			sInitted;
};