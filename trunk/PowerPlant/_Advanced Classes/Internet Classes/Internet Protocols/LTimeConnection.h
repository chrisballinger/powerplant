// Copyright �2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	LTimeConnection.h			PowerPlant 2.2.2	�1998-2005 Metrowerks Inc.
// ===========================================================================

#ifndef _H_LTimeConnection
#define _H_LTimeConnection
#pragma once

#include <LInternetProtocol.h>

#if PP_Uses_Pragma_Import
	#pragma import on
#endif

PP_Begin_Namespace_PowerPlant


// ---------------------------------------------------------------------------

class LTimeConnection : public LInternetProtocol {
public:
	static const UInt16	kDefaultTimePort = 37;
	
						LTimeConnection(LThread& inThread);
						
	virtual				~LTimeConnection();

	UInt32				GetTimeFrom(
								ConstStr255Param	inRemoteHost,
								UInt16				inPort = kDefaultTimePort);

protected:
	virtual void		WaitResponse( UInt32& outNetTime );
};

PP_End_Namespace_PowerPlant

#if PP_Uses_Pragma_Import
	#pragma import reset
#endif

#endif
