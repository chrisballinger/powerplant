// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VPDataModel.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 06/17/96
//     $Date: 2006/01/18 01:33:54 $
//	$History: VPDataModel.h $
//	
//	*****************  Version 10  *****************
//	User: scouten      QDate: 02/18/97   Time: 19:38
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Improved commenting.
//	
//	*****************  Version 9  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:43
//	Updated in $/Constructor/Source files/H1- MacOS/Editors/Views/Data model
//	Fixed CR/LF problem
//	
//	*****************  Version 8  *****************
//	User: scouten      QDate: 12/08/96   Time: 17:49
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Fixed a syntax error.
//	
//	*****************  Version 7  *****************
//	User: scouten      QDate: 12/08/96   Time: 17:22
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added property keys for LRadioGroup.
//	
//	*****************  Version 6  *****************
//	User: scouten      QDate: 12/08/96   Time: 16:44
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added pPaneListInitialItemText property key for LListBox.
//	
//	*****************  Version 5  *****************
//	User: scouten      QDate: 12/08/96   Time: 15:17
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added new attributes for LWindow and LListBox.
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 11/20/96   Time: 20:13
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added pPaneSubObjectList property key.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 11/17/96   Time: 21:39
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Removed pPaneWindowProcMenu property key.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/17/96   Time: 10:51
//	Updated in $/Constructor/Source files/Editors/Views/PowerPlant/Data model
//	Added property keys for GA classes.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/31/96   Time: 13:59
//	Created
//	Comment: 
//	
// ===========================================================================

#pragma once


// ===========================================================================
//		* Constructor property types
// ===========================================================================

enum {

		// LPane
	pPaneID						= 'ID  ',			// pID
	pObjectClassAlias			= 'cALI',
	pPaneWidth					= 'pWID',
	pPaneHeight					= 'pHGT',
	pPaneVisible				= 'pVIS',
	pPaneEnabled				= 'pENA',
	pPaneBoundLeft				= 'pBDL',
	pPaneBoundTop				= 'pBDT',
	pPaneBoundRight				= 'pBDR',
	pPaneBoundBottom			= 'pBDB',
	pPaneLeftOrigin				= 'pLFT',
	pPaneTopOrigin				= 'pTOP',
	pPaneUserConstant			= 'pUSR',
	pPaneSuperview				= 'pSPV',
	pPaneDescriptor				= 'pDES',			// (generic alias)
	pPaneTextTraits				= 'pTXR',			// (generic alias)
	pPaneToolboxRefCon			= 'pREF',			// (generic alias)
	pPaneAttachmentsList		= 'pATX',
		// LView
	pPaneImageWidth				= 'pIMX',
	pPaneImageHeight			= 'pIMY',
	pPaneScrollPosHoriz			= 'pSPX',
	pPaneScrollPosVert			= 'pSPY',
	pPaneScrollUnitHoriz		= 'pSUX',
	pPaneScrollUnitVert			= 'pSUY',
	pPaneReconcileOverhang		= 'pROH',
	pPaneSubViewList			= 'pSUB',
	pPaneSubObjectList			= 'pSOB',			// list of attributes & subpanes (for hierarchy view)
		// LWindow
	pPaneWINDResourceID			= 'pW--',
	pPaneWindowLayer			= 'pLYR',
	pPaneWindowModal			= 'pMDL',
	pPaneWindowFloating			= 'pFLT',
	pPaneWindowRegular			= 'pWRG',
	pPaneWindowCloseBox			= 'pCBX',
	pPaneWindowMgrCloseBox		= 'pCBZ',
	pPaneWindowMgrVisible		= 'pXVS',			//* 2.2a2: BUG FIX #568: WIND resource visible flag
	pPaneWindowTitleBar			= 'pTBR',
	pPaneWindowResizable		= 'pRSZ',
	pPaneWindowSizeBox			= 'pSBX',
	pPaneWindowZoomable			= 'pZOM',
	pPaneWindowInitiallyVisible	= 'pVIS',			// pPaneVisible
	pPaneWindowEnabled			= 'pENA',			// pPaneEnabled
	pPaneWindowTargetable		= 'pTAR',
	pPaneWindowGetSelectClick	= 'pSCL',
	pPaneWindowHideOnSuspend	= 'pHSP',
	pPaneWindowDelaySelect		= 'pDLS',
	pPaneWindowEraseOnUpdate	= 'pERS',
	pPaneWindowMinimumWidth		= 'pMNX',
	pPaneWindowMinimumHeight	= 'pMNY',
	pPaneWindowMaximumWidth		= 'pMXX',
	pPaneWindowMaximumHeight	= 'pMXY',
	pPaneWindowStandardWidth	= 'pSTX',
	pPaneWindowStandardHeight	= 'pSTY',
	pPaneWindowUserConstant		= 'pUSR',			// pPaneUserConstant
	pPaneWindowTop				= 'pTOP',			// pPaneTopOrigin
	pPaneWindowLeft				= 'pLFT',			// pPaneLeftOrigin
	pPaneWindowBottom			= 'pBOT',
	pPaneWindowRight			= 'pRGT',
	pPaneWindowProc				= 'pWDF',
	pPaneWindowRefCon			= 'pREF',			// pPaneToolboxRefCon
	pPaneWindowTitle			= 'pDES',			// pPaneDescriptor
	pPaneWindowAutoPosition		= 'pWAP',
	pPaneWindowHasWCTB			= 'pWCT',			//* 2.3.1b4: SUGGESTION #1004
	pPaneWindowContentColor		= 'pWCC',			//* 2.3.1b1: SUGGESTION #938
		// LDialogBox
	pPaneDialogDefaultButtonID	= 'pDFL',
	pPaneDialogCancelButtonID	= 'pCAN',
		// LPrintout
	pPanePrintoutWidth			= 'pWID',			// pPaneWidth
	pPanePrintoutHeight			= 'pHGT',			// pPaneHeight
	pPanePrintoutActive			= 'pACT',
	pPanePrintoutEnabled		= 'pENA',			// pPaneEnabled
	pPanePrintoutNumberDown		= 'pPND',
		// LCaption
	pPaneText					= 'pDES',			// pPaneDescriptor
		// LControl
	pPaneValueMessage			= 'pMSG',
	pPaneValue					= 'pVAL',
	pPaneMinimumValue			= 'pMIN',
	pPaneMaximumValue			= 'pMAX',
		// LStdControl
	pPaneControlKind			= 'pCKD',
	pPaneControlTitle			= 'pDES',			// pPaneDescriptor
	pPaneControlRefCon			= 'pREF',			// pPaneToolboxRefCon
		// LStdCheckBox
	pPaneCheckBoxOn				= 'pON ',
	pPaneCheckBoxOff			= 'pOFF',
		// LStdPopupMenu
	pPaneMenuTitleBold			= 'pBLD',
	pPaneMenuTitleUnderline		= 'pUND',
	pPaneMenuTitleItalic		= 'pITA',
	pPaneMenuTitleOutline		= 'pOUT',
	pPaneMenuTitleShadow		= 'pSHA',
	pPaneMenuTitleCondensed		= 'pCON',
	pPaneMenuTitleExtended		= 'pEXT',
	pPaneMenuTitleJustification	= 'pJUS',
	pPaneMenuTitleLeft			= 'pLFT',
	pPaneMenuTitleCenter		= 'pCTR',
	pPaneMenuTitleRight			= 'pRGT',
	pPaneMenuTitleWidth			= 'pTWX',
	pPaneMenuResourceID			= 'pMNU',
	pPaneMenuAddResMenu			= 'pARM',
	pPaneMenuFixedWidth			= 'pFIX',
	pPaneMenuInitialMenuItem	= 'pIMI',
		// LTextButton
	pPaneButtonTitle			= 'pDES',			// pPaneDescriptor
	pPaneButtonSelectedBold		= 'pBLD',			// pMenuTitleBold
	pPaneButtonSelectedUnderline = 'pUND',			// pMenuTitleUnderline
	pPaneButtonSelectedItalic	= 'pITA',			// pMenuTitleItalic
	pPaneButtonSelectedOutline	= 'pOUT',			// pMenuTitleOutline
	pPaneButtonSelectedShadow	= 'pSHA',			// pMenuTitleShadow
	pPaneButtonSelectedCondensed = 'pCON',			// pMenuTitleCondensed
	pPaneButtonSelectedExtended	= 'pEXT',			// pMenuTitleExtended
		// LButton
	pPaneButtonGraphicsType		= 'pBGT',
	pPaneButtonNormalGraphicID	= 'pNG#',
	pPaneButtonPushedGraphicID	= 'pPG#',
		// LToggleButton
	pPaneButtonOnGraphicID		= 'pG1#',
	pPaneButtonOnClickedGraphicID = 'pC1#',
	pPaneButtonOffGraphicID		= 'pG0#',
	pPaneButtonOffClickedGraphicID = 'pC0#',
	pPaneButtonTransitionGraphicID = 'pTR#',
		// LEditField
	pPaneEditInitialText		= 'pDES',			// pPaneDescriptor
	pPaneEditMaxChars			= 'pMXC',
	pPaneEditHasBox				= 'pBOX',
	pPaneEditWordWrap			= 'pWWR',
	pPaneEditAutoScroll			= 'pASC',
	pPaneEditTextBuffer			= 'pTBF',
	pPaneEditOutlineHighlight	= 'pOHL',
	pPaneEditInlineInput		= 'pINL',
	pPaneEditUsesTextServices	= 'pTXS',
	pPaneEditKeyFilter			= 'pKYF',
		// LListBox
	pPaneListHorizScrollBar		= 'pSBX',
	pPaneListVertScrollBar		= 'pSBY',
	pPaneListGrowBox			= 'pGBX',
	pPaneListFocusBox			= 'pFBX',
	pPaneListDoubleClickMsg		= 'pMSG',			// pPaneValueMessage
	pPaneListLDEFID				= 'pLDF',
	pPaneListInitialItems		= 'pITM',
	pPaneListInitialItemText	= 'pITX',
		// LScroller
	pPaneScrollLeftIndent		= 'pSLI',
	pPaneScrollRightIndent		= 'pSRI',
	pPaneScrollTopIndent		= 'pSTI',
	pPaneScrollBottomIndent		= 'pSBI',
	pPaneScrollingViewID		= 'pSVW',
		// LTable
	pPaneTableNumberRows		= 'pNCY',
	pPaneTableNumberCols		= 'pNCX',
	pPaneTableRowHeight			= 'pSZY',
	pPaneTableColumnWidth		= 'pSZX',
	pPaneCellDataSize			= 'pCDS',
		// LTextEdit
	pPaneTextEditMultiStyle		= 'pEMS',
	pPaneTextEditEditable		= 'pEDT',
	pPaneTextEditSelectable		= 'pEDS',
	pPaneTextEditWordWrap		= 'pWWR',			// pPaneEditWordWrap
	pPaneTextEditInitialTextID	= 'pTID',
		// LPicture
	pPanePictureID				= 'pPIC',
		// LPlaceHolder
	pPaneOccupantAlignmentHoriz	= 'pOAX',
	pPaneOccupantAlignmentVert	= 'pOAY',
		// LGABox
	pPaneBoxHasTitle			= 'pgHT',
	pPaneBoxHasBorder			= 'pgHB',
	pPaneBoxBorderStyle			= 'pgBS',
	pPaneBoxTitlePosition		= 'pgTP',
		// LGAIconButton
	pPaneIconControlMode		= 'pgCM',
	pPaneIconResource			= 'pgIS',
	pPaneIconSizeSelector		= 'pgIZ',
	pPaneIconPositionSelector	= 'pgIP',
	pPaneIconHiliteIcon			= 'pgHI',
	pPaneIconOffsetOnHilite		= 'pgOF',
		// LGAIconSuiteControl
	pPaneIconHandleClick		= 'pgHC',
		// LGACmdIconButton
	pPaneIconCommandNumber		= 'pgCM',
		// LGAColorSwatch
	pPaneColorSwatchColor		= 'pgSC',
		// LGADisclosureTriangle
	pPaneDisclosureLeftTri		= 'pgLT',
		// LGAFocusBox
	pPaneFocusInsetPaneID		= 'pgIZ',
	pPaneFocusCommanderID		= 'pgCI',
	pPanePaintBorderFace		= 'pgPF',
	pPaneFrameInsetSubPane		= 'pgFI',
	pPaneNotchInsetSubPane		= 'pgNI',
	pPaneNotchFaceForGrowbox	= 'pgNF',
	pPaneNotchWidthHeight		= 'pgNW',
	pPaneCanBeFocused			= 'pgCF',
	pPaneHasFocus				= 'pgHF',
		// LRadioGroup
	pPaneRadioButtonList		= 'pRAD',
	pPaneRadioButtonID			= 'pRBT',
		// LAttachment
	pPaneAttachmentMessage		= 'pMSG',
	pPaneExecuteHost			= 'pXHO',
	pPaneHostOwnsMe				= 'pOWN',
	
	pRebuildMe					= 'pRMe',	// Generic Key to force a rebuild
		// LPushButton
	pDefaultButton				= 'pDfB',
		// LProgressBar
	pPBarIndeterminate			= 'pDet'

};
