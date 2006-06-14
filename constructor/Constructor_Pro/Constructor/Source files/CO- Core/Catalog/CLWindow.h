// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	CLWindow.h					© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/26/96
//     $Date: 2006/01/18 01:32:29 $
//  $History: CLWindow.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/20/97   Time: 13:24
//	Updated in $/Constructor/Source files/CO- Core/Catalog
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:19
//	Updated in $/Constructor/Source files/CO- Core/Catalog
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/30/96   Time: 17:55
//	Created in $/Constructor/Source files/Catalog
//	Added class.
//	
// ===========================================================================

#pragma once

	// Core : Display classes (Eric) : Window classes
#include "GReferencedWindow.h"

	// Core : Editors : Views : Data model
#include "VETypeList.h"


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class CLDisplayClassTable;
class LPageController;
class LIconTextTable;

// ===========================================================================
//		* CLWindow
// ===========================================================================
//
//	CLWindow runs the catalog window.
//
// ===========================================================================

class CLWindow :	public GReferencedWindow,
					public LListener {

public:
	enum { class_ID = 'CLwi' };

	static LPane*			CreateFromStream(LStream* inStream)
									{ return new CLWindow(inStream); }

							CLWindow(LStream* inStream);
	virtual					~CLWindow();

	// window creation

	static void				OpenCatalogWindow();

	static LWindow*			GetCatalogWindow()
									{ return sCatalogWindow; }

	// configuration

protected:
	static void				RegisterCLClasses();

	virtual void			FinishCreateSelf();
	virtual void			ConfigureNewPageTitles();
	virtual void			ConfigurePageTitles();
	
	// page switching
	
	virtual void			ListenToMessage(
									MessageT		inMessage,
									void*			ioParam);
	virtual void			InstallCatalogPage(
									SInt16			inIndex);

	// window settings

	virtual void			RecordCatalogPrefs();


	// data members

protected:
	LPageController*		mPageController;			// the page controller
	LIconTextTable*			mIconTextTable;				// the icon text table
	CLDisplayClassTable*	mClassTable;				// table that shows the display classes
	LControl*				mAlphaToggle;				// alpha/hierarchical toggle
	VETypeListPtr			mTypeList;					// the type list who provides catalog entries
	DMSelectionPtr			mSelection;					// selected items in catalog


	// class variables

protected:
	static LWindow*			sCatalogWindow;				// global singleton window

};