// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPTypeEntry.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 06/12/96
//	   $Date: 2006/01/18 01:33:56 $
//	$History: VPTypeEntry.h $
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:40
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Improved commenting.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:45
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Fixed CR/LF problem
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/21/97   Time: 14:26
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Added override of CreateInstance to ensure that class ID field gets
//	filled in properly. (Bug fix #1158.)
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/17/96   Time: 11:08
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Improved commenting.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/31/96   Time: 13:59
//	Created
//	Comment: 
//	
// ===========================================================================

#pragma once

	// Core : Editors : Views : Data model
#include "VETypeEntry.h"


// ===========================================================================
//		* VPTypeEntry
// ===========================================================================
//
//	VPTypeEntry contains the description of a single PowerPlant display
//	class.
//
// ===========================================================================

class VPTypeEntry : public VETypeEntry {

	// data model description
	
	DMClassID_				('vpte', VPTypeEntry, VETypeEntry);

	// constructor/destructor

public:
							VPTypeEntry();
							VPTypeEntry(LStream* inStream);
							VPTypeEntry(const VPTypeEntry& inOriginal);
	virtual					~VPTypeEntry();

	// creation functions
	
	virtual void			CreateInstance(
									VEModelObjectPtr&	outInstance) const;

	// common initialization

private:
	void					InitVPTypeEntry();


	// data members

protected:
	Handle					mDefaultPPob;				// default PPob resource


	friend class VPTypeList;

};

typedef TSharablePtr<VPTypeEntry> VPTypeEntryPtr;
