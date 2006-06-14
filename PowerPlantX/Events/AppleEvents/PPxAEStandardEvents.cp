// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxAEStandardEvents.cp
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:12 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#include <PPxAEStandardEvents.h>

namespace PPx {

// ===========================================================================
//	AERunApplicationDoer

OSStatus
AERunApplicationDoer::DoAppleEvent(
	const AutoAEDesc&	inAppleEvent,
	AutoAEDesc&			outAEReply)
{
	return DoAERunApplication(inAppleEvent, outAEReply);
}


// ===========================================================================
//	AEReopenApplicationDoer

OSStatus
AEOpenDocumentsDoer::DoAppleEvent(
	const AutoAEDesc&	inAppleEvent,
	AutoAEDesc&			outAEReply)
{
	return DoAEOpenDocuments(inAppleEvent, outAEReply);
}


// ===========================================================================
//	AEPrintDocumentsDoer

OSStatus
AEPrintDocumentsDoer::DoAppleEvent(
	const AutoAEDesc&	inAppleEvent,
	AutoAEDesc&			outAEReply)
{
	return DoAEPrintDocuments(inAppleEvent, outAEReply);
}


// ===========================================================================
//	AEReopenApplicationDoer

OSStatus
AEReopenApplicationDoer::DoAppleEvent(
	const AutoAEDesc&	inAppleEvent,
	AutoAEDesc&			outAEReply)
{
	return DoAEReopenApplication(inAppleEvent, outAEReply);
}


// ===========================================================================
//	AEQuitApplicationDoer

OSStatus
AEQuitApplicationDoer::DoAppleEvent(
	const AutoAEDesc&	inAppleEvent,
	AutoAEDesc&			outAEReply)
{
	return DoAEQuitApplication(inAppleEvent, outAEReply);
}


// ===========================================================================
//	AECloneDoer

OSStatus
AECloneDoer::DoAppleEvent(
	const AutoAEDesc&	inAppleEvent,
	AutoAEDesc&			outAEReply)
{
	return DoAEClone(inAppleEvent, outAEReply);
}


// ===========================================================================
//	AECloseDoer

OSStatus
AECloseDoer::DoAppleEvent(
	const AutoAEDesc&	inAppleEvent,
	AutoAEDesc&			outAEReply)
{
	return DoAEClose(inAppleEvent, outAEReply);
}


// ===========================================================================
//	AECountElementsDoer

OSStatus
AECountElementsDoer::DoAppleEvent(
	const AutoAEDesc&	inAppleEvent,
	AutoAEDesc&			outAEReply)
{
	return DoAECountElements(inAppleEvent, outAEReply);
}


// ===========================================================================
//	AECreateElementDoer

OSStatus
AECreateElementDoer::DoAppleEvent(
	const AutoAEDesc&	inAppleEvent,
	AutoAEDesc&			outAEReply)
{
	return DoAECreateElement(inAppleEvent, outAEReply);
}


// ===========================================================================
//	AEDeleteDoer

OSStatus
AEDeleteDoer::DoAppleEvent(
	const AutoAEDesc&	inAppleEvent,
	AutoAEDesc&			outAEReply)
{
	return DoAEDelete(inAppleEvent, outAEReply);
}


// ===========================================================================
//	AEDoObjectsExistDoer

OSStatus
AEDoObjectsExistDoer::DoAppleEvent(
	const AutoAEDesc&	inAppleEvent,
	AutoAEDesc&			outAEReply)
{
	return DoAEDoObjectsExist(inAppleEvent, outAEReply);
}


// ===========================================================================
//	AEGetDataDoer

OSStatus
AEGetDataDoer::DoAppleEvent(
	const AutoAEDesc&	inAppleEvent,
	AutoAEDesc&			outAEReply)
{
	return DoAEGetData(inAppleEvent, outAEReply);
}


// ===========================================================================
//	AEMoveDoer

OSStatus
AEMoveDoer::DoAppleEvent(
	const AutoAEDesc&	inAppleEvent,
	AutoAEDesc&			outAEReply)
{
	return DoAEMove(inAppleEvent, outAEReply);
}


// ===========================================================================
//	AESaveDoer

OSStatus
AESaveDoer::DoAppleEvent(
	const AutoAEDesc&	inAppleEvent,
	AutoAEDesc&			outAEReply)
{
	return DoAESave(inAppleEvent, outAEReply);
}


// ===========================================================================
//	AESelectDoer

OSStatus
AESelectDoer::DoAppleEvent(
	const AutoAEDesc&	inAppleEvent,
	AutoAEDesc&			outAEReply)
{
	return DoAESelect(inAppleEvent, outAEReply);
}


// ===========================================================================
//	AESetDataDoer

OSStatus
AESetDataDoer::DoAppleEvent(
	const AutoAEDesc&	inAppleEvent,
	AutoAEDesc&			outAEReply)
{
	return DoAESetData(inAppleEvent, outAEReply);
}


} // namespace PPx