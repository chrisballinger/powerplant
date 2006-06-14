// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	CAPrintingAttachment.h		© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 12/16/96
//     $Date: 2006/01/18 01:32:42 $
//  $History: CAPrintingAttachment.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 01/30/97   Time: 18:36
//	Updated in $/Constructor/Source files/CO- Core/Display classes (Eric)/Miscellaneous
//	Fixed ExecuteSelf so that it calls FindCommandStatus. (Bug fix #1259.)
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

	// PowerPlant : Features
#include <LAttachment.h>


// ===========================================================================
//		* CAPrintingAttachment
// ===========================================================================
//
//	CAPrintingAttachment provides the standard printing behaviors.
//
// ===========================================================================

class CAPrintingAttachment : public LAttachment {

public:
	enum { class_ID = 'lprt' };

	static void*			CreateFromStream(LStream* inStream)
									{ return new CAPrintingAttachment(inStream); }

							CAPrintingAttachment(LStream* inStream);
	virtual					~CAPrintingAttachment();

	// event dispatching

protected:
	virtual void			ExecuteSelf(
									MessageT			inMessage,
									void*				ioParam);
	virtual void			FindCommandStatus(
									SCommandStatus*		inCommandStatus);

	// printing commands
	
	virtual void			DoPageSetup();
	virtual void			DoPrint();
	virtual void			DoPrintOne();
	
	virtual void			DoPrintJob();
	

	// data members

protected:
	ResIDT					mPrintoutPPobID;			// PPob containing the printout format
	PaneIDT					mPrintoutPlaceholderID;		// placeholder in the printout format
	PaneIDT					mPrintoutTbarPlaceholderID;	// placeholder for title bar
	
	PaneIDT					mPrintedPaneID;				// pane (in window) that gets printed
	PaneIDT					mPrintedTitleBarID;			// pane (in window) that holds title bar


	// class variables

protected:
	static LPrintSpec		sPrintRecordSpec;			// default print record

};