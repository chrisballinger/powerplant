// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	CTTypeList.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 12/09/96
//     $Date: 2006/01/18 01:33:51 $
//  $History: CTTypeList.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:02
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Custom types
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:42
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Custom types
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 12/10/96   Time: 14:23
//	Created in $/Constructor/Source files/Editors/Custom types
//	Baseline CTYP editor.
//	
// ===========================================================================

#pragma once

	// PowerPlant : Feature classes
#include <LSharable.h>

	// PowerPlant : CA : Array classes
#include <LSharableArray.h>

	// PowerPlant : CA : Feature classes
#include <LSharable.h>


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class CTModelObject;
class CTTypeList;
typedef TSharablePtr<CTTypeList> CTTypeListPtr;


// ===========================================================================
//		* Public exception codes
// ===========================================================================

const ExceptionCode	err_CantMakePrototype	= 'xpro';


// ===========================================================================
//		* CTTypeList
// ===========================================================================

class CTTypeList : public LSharable {

public:
							CTTypeList();
	virtual					~CTTypeList();

	// singleton accessor

	static CTTypeList*		GetTypeList();

	// type entry lookup
	
	virtual CTModelObject*	FindPrototype(ClassIDT inObjectType) const;
	const LSharableArray&	GetPrototypes() const
									{ return mPrototypes; }

	// type specification builders

protected:	
	virtual void			ScanAtLaunch();
	virtual void			MakeSpecFromDSPC(LStream& inStream);


	// data members

protected:
	LSharableArray			mPrototypes;


	// class variables

protected:
	static CTTypeList*		sTypeList;

};
