// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	CFlatFileDesignator.h			©2000 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	A customized Navigation Services save dialog that puts up an option for
//	the user to specify whether to save the resource file as flatened.
//
//	Author:  Isaac Wankerl

#ifndef _H_CFlatFileDesignator
#define _H_CFlatFileDesignator
#pragma once

#include <UNavServicesDialogs.h>


// ---------------------------------------------------------------------------

class CFlatFileDesignator : public UNavServicesDialogs::LFileDesignator
{
public:
						CFlatFileDesignator();

						~CFlatFileDesignator();

	bool				AskDesignateFlatFile(
								ConstStringPtr		inDefaultName,
								Boolean				inSuggestFlatened = false);

	bool				IsFlatened() const;

protected:
	Boolean				mFlatFile;

};

	pascal void	FlatFileNavEventProc(
						NavEventCallbackMessage		inSelector,
						NavCBRecPtr					ioParams,
						NavCallBackUserData			ioUserData);



#endif // _H_CFlatFileDesignator
