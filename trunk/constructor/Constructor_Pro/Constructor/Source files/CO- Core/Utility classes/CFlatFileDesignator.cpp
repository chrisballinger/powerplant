// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	CFlatFileDesignator.cpp			©2000 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//	A customized Navigation Services save dialog that puts up an option for
//	the user to specify whether to save the resource file as flatened.
//
//	Author:  Isaac Wankerl

#include "CFlatFileDesignator.h"

#include <LString.h>
#include <UDesktop.h>
#include <UModalDialogs.h>


#if PRAGMA_STRUCT_ALIGN
    #pragma options align=mac68k
#endif


struct FlatFileUserData
{
	Boolean							wantsFlatFile;
	Boolean							usingAquaLayout;
};
typedef struct FlatFileUserData			FlatFileUserData;


#if PRAGMA_STRUCT_ALIGN
    #pragma options align=reset
#endif


// ---------------------------------------------------------------------------
//	Globals

ControlHandle	gFlatCheckboxControl = nil;
SInt16			gLastTryRight;
SInt16			gLastTryBottom;


// ---------------------------------------------------------------------------
//	Macros and Resource IDs

#define	kFlatCheckboxTop			10
#define	kFlatCheckboxLeft			20
#define	kFlatCheckboxBottom			28
#define	kFlatCheckboxRight			260

#define	kFlatCheckboxAquaTop		10
#define	kFlatCheckboxAquaLeft		20
#define	kFlatCheckboxAquaBottom		30
#define	kFlatCheckboxAquaRight		260

#define	kCustomPaneWidth			kFlatCheckboxLeft + kFlatCheckboxRight
#define	kCustomPaneHeight			kFlatCheckboxTop + kFlatCheckboxBottom

#define	kCustomPaneAquaWidth		kFlatCheckboxAquaLeft + kFlatCheckboxAquaRight
#define	kCustomPaneAquaHeight		kFlatCheckboxAquaTop + kFlatCheckboxAquaBottom

const ResIDT	STR_SaveFlatFile			= 24000;
const SInt16	str_FlatFileCheckboxTitle	= 1;


// ---------------------------------------------------------------------------
//	¥ CFlatFileDesignator								  [public]
// ---------------------------------------------------------------------------

CFlatFileDesignator::CFlatFileDesignator()
{
	mFlatFile = false;
}


// ---------------------------------------------------------------------------
//	¥ ~CFlatFileDesignator								  [public]
// ---------------------------------------------------------------------------

CFlatFileDesignator::~CFlatFileDesignator()
{
}


// ---------------------------------------------------------------------------
//	¥ AskDesignateFlatFile								  [public]
// ---------------------------------------------------------------------------
//	Most of the code taken directly from
//	UNavServicesDialogs::LFileDesignator::AskDesignateFile
//	with modifications for a custom event filter and handling
//	user data.

bool
CFlatFileDesignator::AskDesignateFlatFile(
	ConstStringPtr	inDefaultName,
	Boolean			inSuggestFlatened)
{
	UNavServicesDialogs::StNavEventUPP	eventUPP(FlatFileNavEventProc);

	LString::CopyPStr(inDefaultName, mNavOptions.savedFileName);

	mNavReply.SetDefaultValues();

	AEDesc*		defaultLocationDesc = nil;
	if (not mDefaultLocation.IsNull()) {
		defaultLocationDesc = mDefaultLocation;

		if (mSelectDefault) {
			mNavOptions.dialogOptionFlags |= kNavSelectDefaultLocation;
		} else {
			mNavOptions.dialogOptionFlags &= ~kNavSelectDefaultLocation;
		}
	}
	
	// Allow users to save files inside of bundles
	mNavOptions.dialogOptionFlags |= kNavSupportPackages | kNavAllowOpenPackages;

	UDesktop::Deactivate();

	OSErr	err;
	SInt32	result;
	FlatFileUserData	navUserData;
	
	navUserData.wantsFlatFile = inSuggestFlatened;
	navUserData.usingAquaLayout = false;
	
	err = ::Gestalt(gestaltMenuMgrAttr, &result);
	if ((err == noErr) and (result & gestaltMenuMgrAquaLayoutMask))
	{
		navUserData.usingAquaLayout = true;
	}


	err = ::NavPutFile(
						defaultLocationDesc,
						mNavReply,
						&mNavOptions,
						eventUPP,
						mFileType,
						mFileCreator,
						&navUserData);			// User Data

	UDesktop::Activate();

	if ( (err != noErr) && (err != userCanceledErr) ) {
		Throw_(err);
	}

	if (mNavReply.IsValid())
	{
		mFlatFile = navUserData.wantsFlatFile;
	}
	
	return mNavReply.IsValid();
}


// ---------------------------------------------------------------------------
//	¥ IsFlatened										  [public]
// ---------------------------------------------------------------------------

bool
CFlatFileDesignator::IsFlatened() const
{
	return mFlatFile;
}




#pragma mark -

// ---------------------------------------------------------------------------
//	¥ FlatFileNavEventProc											  [static]
// ---------------------------------------------------------------------------
//	Event filter callback routine for Navigation Services

pascal void
FlatFileNavEventProc(
	NavEventCallbackMessage		inSelector,
	NavCBRecPtr					ioParams,
	NavCallBackUserData			ioUserData)
{
	try
	{
		switch (inSelector)
		{
			case kNavCBEvent:
			{
				if (ioParams->eventData.eventDataParms.event->what == mouseDown)
				{
					Point			where;
					ControlHandle	control;
					ControlPartCode	partCode;
					
					if (gFlatCheckboxControl == nil)
						break;

					where = ioParams->eventData.eventDataParms.event->where;
					GlobalToLocal(&where);
					partCode = FindControl(where, ioParams->window, &control);
					if ((control == gFlatCheckboxControl) and (partCode == kControlCheckBoxPart))
					{
						SetControlValue(gFlatCheckboxControl, !GetControlValue(gFlatCheckboxControl));
					}
				}
				else
				{
					UModalAlerts::ProcessModalEvent(*(ioParams->eventData.eventDataParms.event));
				}
				break;
			}

			case kNavCBCustomize:
			{
				SInt16	width;
				SInt16	height;

				if (((FlatFileUserData*)ioUserData)->usingAquaLayout)
				{
					width = kCustomPaneAquaWidth;
					height = kCustomPaneAquaHeight;
				}
				else
				{
					width = kCustomPaneWidth;
					height = kCustomPaneHeight;
				}
				
				// Here we negotiate with NavServ to set the size of our custom pane area
				if ((ioParams->customRect.right == 0) and (ioParams->customRect.bottom == 0))
				{
					// This is our first time through, lets tell NavServ what we want
					ioParams->customRect.right = ioParams->customRect.left + width;
					ioParams->customRect.bottom = ioParams->customRect.top + height;
				}
				else
				{
					// Negotiate the size to use
					if (gLastTryRight != ioParams->customRect.right)
						if (ioParams->customRect.right < ioParams->customRect.left + width)
							ioParams->customRect.right = ioParams->customRect.left + width;

					if (gLastTryBottom != ioParams->customRect.bottom)
						if (ioParams->customRect.bottom < ioParams->customRect.top + height)
							ioParams->customRect.bottom = ioParams->customRect.top + height;
				}
				
				// Save our last size so we can re-negotiate
				gLastTryRight = ioParams->customRect.right;
				gLastTryBottom = ioParams->customRect.bottom;

				break;
			}

			case kNavCBStart:
			{
				if (gFlatCheckboxControl == nil)
				{
					LStr255	checkboxTitle(STR_SaveFlatFile, str_FlatFileCheckboxTitle);
					Rect	bounds;
					Boolean	initialValue = ((FlatFileUserData*)ioUserData)->wantsFlatFile;

					if (((FlatFileUserData*)ioUserData)->usingAquaLayout)
					{
						SetRect(&bounds, kFlatCheckboxAquaLeft, kFlatCheckboxAquaTop,
									kFlatCheckboxAquaRight, kFlatCheckboxAquaBottom);
					}
					else
					{
						SetRect(&bounds, kFlatCheckboxLeft, kFlatCheckboxTop,
									kFlatCheckboxRight, kFlatCheckboxBottom);
					}
					
					gFlatCheckboxControl = NewControl(ioParams->window, &bounds, checkboxTitle,
								true, initialValue, kControlCheckBoxUncheckedValue,
								kControlCheckBoxCheckedValue, checkBoxProc, 0);
				}
				if (gFlatCheckboxControl != nil)
				{
					OSErr	err;
					
					err = NavCustomControl(ioParams->context, kNavCtlAddControl, gFlatCheckboxControl);
				}
				break;
			}

			case kNavCBTerminate:
			{
				if (gFlatCheckboxControl == nil)
					break;
					
				if (GetControlValue(gFlatCheckboxControl) == kControlCheckBoxCheckedValue)
				{
					((FlatFileUserData*)ioUserData)->wantsFlatFile = true;
				}
				else
				{
					((FlatFileUserData*)ioUserData)->wantsFlatFile = false;
				}
				
				DisposeControl(gFlatCheckboxControl);
				gFlatCheckboxControl = nil;
				break;
			}

			case kNavCBAdjustRect:
			{
				SInt16	width;
				SInt16	height;
				Rect	oldBounds;
				Rect	newBounds = ioParams->customRect;
				
				if (gFlatCheckboxControl == nil)
					break;

				GetControlBounds(gFlatCheckboxControl, &oldBounds);
				width = oldBounds.right - oldBounds.left;
				height = oldBounds.bottom - oldBounds.top;
				
				if (((FlatFileUserData*)ioUserData)->usingAquaLayout)
				{
					newBounds.left += kFlatCheckboxAquaLeft;
					newBounds.top += kFlatCheckboxAquaTop;
					newBounds.right = newBounds.left + width;
					newBounds.bottom = newBounds.top + height;
				}
				else
				{
					newBounds.left += kFlatCheckboxLeft;
					newBounds.top += kFlatCheckboxTop;
					newBounds.right = newBounds.left + width;
					newBounds.bottom = newBounds.top + height;
				}
				
				SetControlBounds(gFlatCheckboxControl, &newBounds);
				InvalWindowRect(ioParams->window, &newBounds);

				break;
			}
		}
	}
	catch (...)
	{
		// Can't throw back through the Toolbox
	}
}


