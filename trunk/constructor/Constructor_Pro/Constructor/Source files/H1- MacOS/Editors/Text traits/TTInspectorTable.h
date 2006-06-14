// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	TTInspectorTable.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 10/29/96
//	   $Date: 2006/01/18 01:33:53 $
//	$History: TTInspectorTable.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:18
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Text traits
//	Improved commenting.
//
//	*****************  Version 2   *****************
//	User: scouten      Date: 01/24/97   Time: 17:43
//	Checked in '$/Constructor/Source files/H1- MacOS/Editors/Text traits'
//	Comment: Fixed CR/LF problem
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/30/96   Time: 18:33
//	Created
//	Comment: Baseline Txtr editor.
//	
// ===========================================================================

#pragma once

	// Core : Property inspector
#include "PIInspectorTable.h"


// ===========================================================================
//		* TTInspectorTable
// ===========================================================================
//
//	A variation on the regular property inspector designed for use in the
//	text traits editor.
//
// ===========================================================================

class TTInspectorTable : public PIInspectorTable {

public:
	enum { class_ID = 'TXIT' };

	static LView*			CreateFromStream(LStream* inStream)
									{ return new TTInspectorTable(inStream); }

							TTInspectorTable(LStream* inStream);
	virtual					~TTInspectorTable();

	// element accessors

	virtual void			SetElement(DMElement* inElement);

	// drawing behaviors

protected:
	virtual void			DrawSelf();
	virtual OVItem*			CreateItemForElementSelf(DMElement* inElement);

	// configuration

	virtual void			FinishCreateSelf();

};
