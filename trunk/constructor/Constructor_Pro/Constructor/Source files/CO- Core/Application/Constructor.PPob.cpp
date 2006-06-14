// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	Constructor.PPob.cpp		© 1994-97 Metrowerks Inc. All rights reserved.
// ===========================================================================

#include "Constructor.PPob.h"

	// Core : Display classes (Eric) : Pane classes
#include "GActiveScroller.h"
#include "GScrollingTextPane.h"

	// Core : Display classes (Eric) : Miscellaneous
#include "CAPrintingAttachment.h"
#include "CATitleBarPlaceHolder.h"

	// Core : Display classes (Eric) : Outline table
#include "OVPipeDrawer.h"

	// Core : Display classes (Eric) : Window classes
#include "GContextDialog.h"
#include "GReferencedWindow.h"

	// Core : Display classes (Eric) : Window menu
#include "GWindowMenuAttachment.h"

	// Core : Display classes (Eric) : Window positioning
#include "GWindowPositioner.h"

	// Core : Display classes (Robin) : Grayscale futures
#include "LGAMiniArrowPopup.h"
#include "LGASBToggleIconButton.h"

	// Core : Display classes (Robin) : Grayscale panes
#include "GAMWBanner.h"
#include "GAShadowBorder.h"

	// Core : Display classes (Robin) : Page controller
#include "LPageController.h"
#include "LIconTextTable.h"

	// Core : Display classes (Shapiro) : Color popup
#include "SUColorPane.h"

	// PowerPlant : Pane classes
#include <LGroupBox.h>

	// PowerPlant : Utility classes
#include <URegistrar.h>

	// PowerPlant : CA : Pane classes
#include <LPageNumberCaption.h>

	// PowerPlant : CA : Text editing
#include <LBroadcasterEditField.h>

	// PowerPlant : GA : Grayscale controls
#include <LGACheckbox.h>
#include <LGAIconButton.h>
#include <LGAPopup.h>
#include <LGAPushButton.h>
#include <LGATextButton.h>

	// PowerPlant : GA : Grayscale panes
#include <LGABox.h>
#include <LGACaption.h>
#include <LGADialogBox.h>
#include <LGAEditField.h>
#include <LGAFocusBorder.h>
#include <LGAPrimaryGroup.h>
#include <LGASecondaryBox.h>
#include <LGASeparator.h>

#include <UControlRegistry.h>

#include "VPLBevelButton.h"
#include "VPLIconControl.h"
#include "VPLImageWell.h"
#include "VPLPictureControl.h"
#include "VPLPopupButton.h"
#include "LGAPopupGroupBoxImp.h"
#include "LPopupGroupBox.h"
#include "VPLPushButton.h"

// ---------------------------------------------------------------------------
//		* RegisterAllConstructorPPClasses
// ---------------------------------------------------------------------------
//	Register all the pane types that we use in Constructor.
//	NOTE: Some built-in classes are rerouted to special subclasses
//	that we need throughout Constructor.

void
RegisterAllConstructorPPClasses()
{

		// Core : Display classes (Eric) : Pane classes
	URegistrar::RegisterClass(GActiveScroller::class_ID, (ClassCreatorFunc) GActiveScroller::CreateFromStream);
	URegistrar::RegisterClass(GScrollingTextPane::class_ID, (ClassCreatorFunc) GScrollingTextPane::CreateFromStream);

		// Core : Display classes (Eric) : Miscellaneous
	URegistrar::RegisterClass(CAPrintingAttachment::class_ID, (ClassCreatorFunc) CAPrintingAttachment::CreateFromStream);
	URegistrar::RegisterClass(CATitleBarPlaceHolder::class_ID, (ClassCreatorFunc) CATitleBarPlaceHolder::CreateFromStream);

		// Core : Display classes (Eric) : Outline classes
	URegistrar::RegisterClass(OVPipeDrawer::class_ID, (ClassCreatorFunc) OVPipeDrawer::CreateFromStream);

		// Core : Display classes (Eric) : Window menu
	URegistrar::RegisterClass(GWindowMenuAttachment::class_ID, (ClassCreatorFunc) GWindowMenuAttachment::CreateFromStream);

		// Core : Display classes (Eric) : Window positioning
	URegistrar::RegisterClass(LWindowPositioner::class_ID, (ClassCreatorFunc) GWindowPositioner::CreateWindowPositionerStream);

	URegistrar::RegisterClass(LGAMiniArrowPopup::class_ID, (ClassCreatorFunc) LGAMiniArrowPopup::CreateLGAMiniArrowPopupStream);
	URegistrar::RegisterClass(LGASBToggleIconButton::class_ID, (ClassCreatorFunc) LGASBToggleIconButton::CreateLGASBToggleIconButtonStream);

		// Core : Display classes (Robin) : Grayscale panes
	RegisterClass_(GAMWBanner);
	URegistrar::RegisterClass(GAShadowBorder::class_ID, (ClassCreatorFunc) GAShadowBorder::CreateGAShadowBorderStream);

		// Core : Display classes (Robin) : Page controller
	RegisterClass_(LPageController);
	RegisterClass_(LIconTextTable);
	RegisterClass_(LEraseAttachment);

		// Core : Display classes (Shapiro) : Color popup
	URegistrar::RegisterClass(SUColorPane::class_ID, (ClassCreatorFunc) SUColorPane::CreateFromStream);

		// PowerPlant : Pane classes
	URegistrar::RegisterClass(LActiveScroller::class_ID, (ClassCreatorFunc) GActiveScroller::CreateFromStream);

	RegisterClass_(LButton);
	RegisterClass_(LCaption);
	RegisterClass_(LCicnButton);

	URegistrar::RegisterClass(LDialogBox::class_ID, (ClassCreatorFunc) GContextDialog::CreateFromStream);

	RegisterClassID_(LBroadcasterEditField, LEditField::class_ID);
	
	RegisterClass_(LGroupBox);
	RegisterClass_(LPane);
	RegisterClass_(LPicture);
	RegisterClass_(LPlaceHolder);
	RegisterClass_(LPrintout);
	RegisterClass_(LStdControl);
	RegisterClass_(LStdButton);
	RegisterClass_(LStdCheckBox);
	RegisterClass_(LStdRadioButton);
	RegisterClass_(LStdPopupMenu);
	RegisterClass_(LView);

	URegistrar::RegisterClass(LWindow::class_ID, (ClassCreatorFunc) GReferencedWindow::CreateFromStream);

		// PowerPlant : Support classes
	RegisterClass_(LRadioGroup);
	RegisterClass_(LTabGroup);

		// PowerPlant : CA : Pane classes
//	URegistrar::RegisterClass(LPageNumberCaption::class_ID, (ClassCreatorFunc) LPageNumberCaption::CreateFromStream);
	RegisterClass_(LPageNumberCaption);

		// PowerPlant : CA : Text editing classes
	RegisterClass_(LBroadcasterEditField);

	RegisterClass_(LGAEditField);
	RegisterClass_(LGAPopup);

		// PowerPlant : GA : Grayscale controls
	RegisterClass_(LGACheckbox);
	RegisterClass_(LGAIconButton);
	RegisterClass_(LGAPushButton);
	RegisterClass_(LGATextButton);

		// PowerPlant : GA : Grayscale panes
	RegisterClass_(LGABox);
	RegisterClass_(LGACaption);
	RegisterClass_(LGADialogBox);
	RegisterClass_(LGAFocusBorder);
	RegisterClass_(LGAPrimaryGroup);
	RegisterClass_(LGASecondaryBox);
	RegisterClass_(LGASeparator);
	
			// Appearance Controls that use resources
	RegisterClassID_( VPLGABevelButtonImp,		LBevelButton::imp_class_ID );
	RegisterClassID_( VPLGAIconControlImp,		LIconControl::imp_class_ID );
	RegisterClassID_( VPLGAImageWellImp,		LImageWell::imp_class_ID );
	RegisterClassID_( VPLGAPictureControlImp,	LPictureControl::imp_class_ID );
	RegisterClassID_( VPLGAPushButtonImp,		LPushButton::imp_class_ID );
	
		// PowerPlant : Appearance Controls

// JWW - Never use GA junk under Carbon
#if !PP_Target_Carbon
	LStr255	gaStr(1099, 4);
	if (gaStr == "\pGA") {
		UControlRegistry::RegisterGAClasses();
	} else
#endif
	{
		UControlRegistry::RegisterClasses();
	}
	
}