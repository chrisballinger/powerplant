// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxQuickdrawUtils.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:37 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxQuickdrawUtils.h>

namespace PPx {

// ---------------------------------------------------------------------------
//	GrafPortSaver													  [public]
/**
	Constructor
	
	@param	inPort		GrafPtr to make the current port
	
	Saves the current port upon entry, and sets the current port to the
	input GrafPtr if it is not nil											*/

GrafPortSaver::GrafPortSaver(
	GrafPtr	inPort)
{
	::GetPort(&mSavedPort);					// Save cuurent GrafPort
	
	if (inPort != nil) {
		::SetPort(inPort);
	}
}


// ---------------------------------------------------------------------------
//	~GrafPortSaver													  [public]
/**
	Desstructor
	
	Restores the port that was current when the contructor was called		*/

GrafPortSaver::~GrafPortSaver()
{
	if (mSavedPort != nil) {
		::SetPort(mSavedPort);
	}
}


} // namespace PPx