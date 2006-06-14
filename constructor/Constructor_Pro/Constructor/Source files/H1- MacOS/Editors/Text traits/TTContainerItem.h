// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	TTContainerItem.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	 Created: 10/29/96
//	   $Date: 2006/01/18 01:33:53 $
//	$History: TTContainerItem.h $
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
//	User: scouten      Date: 10/30/96   Time: 18:29
//	Created
//	Comment: Baseline Txtr editor.
//	
// ===========================================================================

#pragma once

	// Core : Property inspector
#include "PIContainerItem.h"


// ===========================================================================
//		* TTContainerItem
// ===========================================================================
//
//	TTContainerItem is a specialized container used in the Txtr
//	"property inspector." It does not have a flip-down triangle, and
//	it does not draw the row adornments that are typically drawn.
//
// ===========================================================================

class TTContainerItem : public PIContainerItem {

public:
							TTContainerItem();
	virtual					~TTContainerItem();

	// disclosure triangle

	virtual void			Collapse();
	virtual Boolean			CanExpand() const;
	virtual Boolean			CalcLocalDisclosureTriangleRect(
									Rect&					outTriangleRect);

	// drawing behaviors
	
protected:
	virtual void			GetDrawContentsSelf(
									const STableCell&		inCell,
									SOutlineDrawContents&	ioDrawContents);
	virtual void			DrawRowAdornments(
									const Rect&				inLocalRowRect);

};