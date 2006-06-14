// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	StDMSetup.h					© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 04/27/96
//	   $Date: 2006/01/18 01:32:37 $
//	$History: StDMSetup.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/31/97   Time: 12:00
//	Updated in $/Constructor/Source files/CO- Core/Data model/Core objects
//	Added registration for DMResourceRefAttribute.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:21
//	Updated in $/Constructor/Source files/CO- Core/Data model/Core objects
//	Fixed CR/LF problem
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 12/08/96   Time: 11:22
//	Updated in $/Constructor/Source files/Data model/Core objects
//	Upgraded to Clint's drop 12/07/96. Improved commenting.
//
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:30
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once


// ===========================================================================
//		* StDMSetup
// ===========================================================================
//
//	This is a stack-based class to set up and tear down the data
//	modelling classes. Currently it creates a DMRegistrar object and
//	initializes it by registering the common object and attribute classes.
//	The preferred use of this object is from the main() method of the
//	application.
//
// ===========================================================================

class StDMSetup {

public:
							StDMSetup();
							~StDMSetup();
	
protected:
	void					RegisterCommonElements();	

};
