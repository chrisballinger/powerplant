// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxAEStandardEvents.h
//
//	Copyright © 2003-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:12 $
//	$Revision: 1.1.1.1 $
// ===========================================================================
/**
	@file		PPxAEStandardEvents.h
	@brief		Handlers for events in the Apple Event Standard Suite
*/

#ifndef H_PPxAEStandardEvents
#define H_PPxAEStandardEvents
#pragma once

#include <PPxAppleEventDoer.h>

namespace PPx {

// ===========================================================================
//	Run Application
/**
	Handles notification the application was launched directly and not
	from opening a document													*/

class	AERunApplicationDoer : public SpecificAppleEventDoer<
											kCoreEventClass,
											kAEOpenApplication> {
protected:								
	virtual OSStatus	DoAERunApplication(
								const AutoAEDesc&	inAppleEvent,
								AutoAEDesc&			outAEReply) = 0;

private:					
	virtual OSStatus	DoAppleEvent( 
								const AutoAEDesc&	inAppleEvent,
								AutoAEDesc&			outAEReply);
};


// ===========================================================================
//	Open Documents
/**
	Handles request to open a list of documents */

class	AEOpenDocumentsDoer : public SpecificAppleEventDoer<
											kCoreEventClass,
											kAEOpenDocuments> {
protected:
	virtual OSStatus	DoAEOpenDocuments(
								const AutoAEDesc&	inAppleEvent,
								AutoAEDesc&			outAEReply) = 0;
						
private:					
	virtual OSStatus	DoAppleEvent( 
								const AutoAEDesc&	inAppleEvent,
								AutoAEDesc&			outAEReply);
};


// ===========================================================================
//	Print Documents
/**
	Handles request to print a list of documents */

class	AEPrintDocumentsDoer : public SpecificAppleEventDoer<
											kCoreEventClass,
											kAEPrintDocuments> {
protected:
	virtual OSStatus	DoAEPrintDocuments(
								const AutoAEDesc&	inAppleEvent,
								AutoAEDesc&			outAEReply) = 0;
						
private:					
	virtual OSStatus	DoAppleEvent( 
								const AutoAEDesc&	inAppleEvent,
								AutoAEDesc&			outAEReply);
};


// ===========================================================================
//	Reopen Application
/**
	Handles notification that an already running application has been
	reactivated from the Finder												*/

class	AEReopenApplicationDoer : public SpecificAppleEventDoer<
											kCoreEventClass,
											kAEReopenApplication> {
protected:
	virtual OSStatus	DoAEReopenApplication(
								const AutoAEDesc&	inAppleEvent,
								AutoAEDesc&			outAEReply) = 0;
						
private:					
	virtual OSStatus	DoAppleEvent( 
								const AutoAEDesc&	inAppleEvent,
								AutoAEDesc&			outAEReply);
};


// ===========================================================================
//	Quit Application
/**
	Handles request to quit the application */

class	AEQuitApplicationDoer : public SpecificAppleEventDoer<
											kCoreEventClass,
											kAEQuitApplication> {
protected:								
	virtual OSStatus	DoAEQuitApplication(
								const AutoAEDesc&	inAppleEvent,
								AutoAEDesc&			outAEReply) = 0;
	
private:					
	virtual OSStatus	DoAppleEvent( 
								const AutoAEDesc&	inAppleEvent,
								AutoAEDesc&			outAEReply);
};


// ===========================================================================
//	Clone
/**
	Duplicates an item
	
	@note The programming command is "clone", but the user terminology
	is "duplicate".															*/

class	AECloneDoer : public SpecificAppleEventDoer<
											kAECoreSuite,
											kAEClone> {
protected:								
	virtual OSStatus	DoAEClone(
								const AutoAEDesc&	inAppleEvent,
								AutoAEDesc&			outAEReply) = 0;
	
private:					
	virtual OSStatus	DoAppleEvent( 
								const AutoAEDesc&	inAppleEvent,
								AutoAEDesc&			outAEReply);
};


// ===========================================================================
//	Close
/**
	Closes an item */

class	AECloseDoer : public SpecificAppleEventDoer<
											kAECoreSuite,
											kAEClose> {
protected:								
	virtual OSStatus	DoAEClose(
								const AutoAEDesc&	inAppleEvent,
								AutoAEDesc&			outAEReply) = 0;
	
private:					
	virtual OSStatus	DoAppleEvent( 
								const AutoAEDesc&	inAppleEvent,
								AutoAEDesc&			outAEReply);
};


// ===========================================================================
//	Count Elements
/**
	Counts the number of items */

class	AECountElementsDoer : public SpecificAppleEventDoer<
											kAECoreSuite,
											kAECountElements> {
protected:								
	virtual OSStatus	DoAECountElements(
								const AutoAEDesc&	inAppleEvent,
								AutoAEDesc&			outAEReply) = 0;
	
private:					
	virtual OSStatus	DoAppleEvent( 
								const AutoAEDesc&	inAppleEvent,
								AutoAEDesc&			outAEReply);
};


// ===========================================================================
//	Create Element
/**
	Creates a new item
	
	@note The programming command is "create element", but the user
	terminology is "make new" */

class	AECreateElementDoer : public SpecificAppleEventDoer<
											kAECoreSuite,
											kAECreateElement> {
protected:								
	virtual OSStatus	DoAECreateElement(
								const AutoAEDesc&	inAppleEvent,
								AutoAEDesc&			outAEReply) = 0;
	
private:					
	virtual OSStatus	DoAppleEvent( 
								const AutoAEDesc&	inAppleEvent,
								AutoAEDesc&			outAEReply);
};


// ===========================================================================
//	Delete
/**
	Deletes an item */

class	AEDeleteDoer : public SpecificAppleEventDoer<
											kAECoreSuite,
											kAEDelete> {
protected:								
	virtual OSStatus	DoAEDelete(
								const AutoAEDesc&	inAppleEvent,
								AutoAEDesc&			outAEReply) = 0;
	
private:					
	virtual OSStatus	DoAppleEvent( 
								const AutoAEDesc&	inAppleEvent,
								AutoAEDesc&			outAEReply);
};


// ===========================================================================
//	Do Objects Exists
/**
	Checks whether an item exists */

class	AEDoObjectsExistDoer : public SpecificAppleEventDoer<
											kAECoreSuite,
											kAEDoObjectsExist> {
protected:								
	virtual OSStatus	DoAEDoObjectsExist(
								const AutoAEDesc&	inAppleEvent,
								AutoAEDesc&			outAEReply) = 0;
	
private:					
	virtual OSStatus	DoAppleEvent( 
								const AutoAEDesc&	inAppleEvent,
								AutoAEDesc&			outAEReply);
};


// ===========================================================================
//	Get Data
/**
	Gets the data for an item */

class	AEGetDataDoer : public SpecificAppleEventDoer<
											kAECoreSuite,
											kAEGetData> {
protected:								
	virtual OSStatus	DoAEGetData(
								const AutoAEDesc&	inAppleEvent,
								AutoAEDesc&			outAEReply) = 0;
	
private:					
	virtual OSStatus	DoAppleEvent( 
								const AutoAEDesc&	inAppleEvent,
								AutoAEDesc&			outAEReply);
};


// ===========================================================================
//	Move
/**
	Moves an item */

class	AEMoveDoer : public SpecificAppleEventDoer<
											kAECoreSuite,
											kAEMove> {
protected:								
	virtual OSStatus	DoAEMove(
								const AutoAEDesc&	inAppleEvent,
								AutoAEDesc&			outAEReply) = 0;
	
private:					
	virtual OSStatus	DoAppleEvent( 
								const AutoAEDesc&	inAppleEvent,
								AutoAEDesc&			outAEReply);
};


// ===========================================================================
//	Save
/**
	Saves an item */

class	AESaveDoer : public SpecificAppleEventDoer<
											kAECoreSuite,
											kAESave> {
protected:								
	virtual OSStatus	DoAESave(
								const AutoAEDesc&	inAppleEvent,
								AutoAEDesc&			outAEReply) = 0;
	
private:					
	virtual OSStatus	DoAppleEvent( 
								const AutoAEDesc&	inAppleEvent,
								AutoAEDesc&			outAEReply);
};


// ===========================================================================
//	Select
/**
	Selects an item */

class	AESelectDoer : public SpecificAppleEventDoer<
											kAEMiscStandards,
											kAESelect> {
protected:								
	virtual OSStatus	DoAESelect(
								const AutoAEDesc&	inAppleEvent,
								AutoAEDesc&			outAEReply) = 0;
	
private:					
	virtual OSStatus	DoAppleEvent( 
								const AutoAEDesc&	inAppleEvent,
								AutoAEDesc&			outAEReply);
};


// ===========================================================================
//	Set Data
/**
	Sets the data for an item */

class	AESetDataDoer : public SpecificAppleEventDoer<
											kAECoreSuite,
											kAESetData> {
protected:								
	virtual OSStatus	DoAESetData(
								const AutoAEDesc&	inAppleEvent,
								AutoAEDesc&			outAEReply) = 0;
	
private:					
	virtual OSStatus	DoAppleEvent( 
								const AutoAEDesc&	inAppleEvent,
								AutoAEDesc&			outAEReply);
};


} // namespace PPx

#endif	// H_PPxAEStandardEvents
