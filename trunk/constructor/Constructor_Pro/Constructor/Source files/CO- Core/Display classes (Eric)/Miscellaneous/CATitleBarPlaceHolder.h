// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	CATitleBarPlaceHolder.h		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 12/16/96
//     $Date: 2006/01/18 01:32:42 $
//  $History: CATitleBarPlaceHolder.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/20/97   Time: 15:25
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Miscellaneous
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:23
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Miscellaneous
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 12/17/96   Time: 00:14
//	Created in $/Constructor/Source files/Display classes (Eric)/Miscellaneous
//	Added class.
//	
// ===========================================================================

#pragma once

	// PowerPlant : Panes
#include <LPlaceHolder.h>


// ===========================================================================
//		* CATitleBarPlaceHolder
// ===========================================================================

class CATitleBarPlaceHolder : public LPlaceHolder {

public:
	enum { class_ID = 'CTBR' };

	static void*			CreateFromStream(LStream* inStream)
									{ return new CATitleBarPlaceHolder(inStream); }

							CATitleBarPlaceHolder(LStream* inStream);
	virtual					~CATitleBarPlaceHolder();

	virtual void			DrawSelf();
	virtual Boolean			ScrollToPanel(const PanelSpec& inPanel);

};
