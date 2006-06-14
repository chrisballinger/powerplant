// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPxNibDecoderFuncs.h
//
//	Copyright © 2004-2005 Metrowerks Corporation.  All rights reserved.
//
//	$Date: 2006/01/18 01:37:23 $
//	$Revision: 1.1.1.1 $
// ===========================================================================

#ifndef H_NibDecoderFuncs
#define H_NibDecoderFuncs
#pragma once

#include <SysCFXMLTree.h>

namespace PPx {

class	Persistent;
class	View;

namespace NibDecoderFuncs {

	Persistent*			CreateBevelButton(
								View*				inSuperView,
								const CFXMLTree&	inTree);

	Persistent*			CreateButton(
								View*				inSuperView,
								const CFXMLTree&	inTree);

	Persistent*			CreateChasingArrows(
								View*				inSuperView,
								const CFXMLTree&	inTree);

	Persistent*			CreateCheckBox(
								View*				inSuperView,
								const CFXMLTree&	inTree);

	Persistent*			CreateClockDate(
								View*				inSuperView,
								const CFXMLTree&	inTree);

	Persistent*			CreateComboBox(
								View*				inSuperView,
								const CFXMLTree&	inTree);

	Persistent*			CreateDBColumnView(
								View*				inSuperView,
								const CFXMLTree&	inTree);

	Persistent*			CreateDBListView(
								View*				inSuperView,
								const CFXMLTree&	inTree);

	Persistent*			CreateEditText(
								View*				inSuperView,
								const CFXMLTree&	inTree);

	Persistent*			CreateGroupBox(
								View*				inSuperView,
								const CFXMLTree&	inTree);

	Persistent*			CreateHIView(
								View*				inSuperView,
								const CFXMLTree&	inTree);

	Persistent*			CreateIcon(
								View*				inSuperView,
								const CFXMLTree&	inTree);

	Persistent*			CreateImageView(
								View*				inSuperView,
								const CFXMLTree&	inTree);

	Persistent*			CreateImageWell(
								View*				inSuperView,
								const CFXMLTree&	inTree);

	Persistent*			CreateLittleArrows(
								View*				inSuperView,
								const CFXMLTree&	inTree);

	Persistent*			CreatePicture(
								View*				inSuperView,
								const CFXMLTree&	inTree);

	Persistent*			CreatePopupArrow(
								View*				inSuperView,
								const CFXMLTree&	inTree);

	Persistent*			CreatePopupButton(
								View*				inSuperView,
								const CFXMLTree&	inTree);

	Persistent*			CreateProgressBar(
								View*				inSuperView,
								const CFXMLTree&	inTree);

	Persistent*			CreateRadioButton(
								View*				inSuperView,
								const CFXMLTree&	inTree);

	Persistent*			CreateRadioGroup(
								View*				inSuperView,
								const CFXMLTree&	inTree);

	Persistent*			CreateRelevanceBar(
								View*				inSuperView,
								const CFXMLTree&	inTree);

	Persistent*			CreateRoundButton(
								View*				inSuperView,
								const CFXMLTree&	inTree);

	Persistent*			CreateScrollBar(
								View*				inSuperView,
								const CFXMLTree&	inTree);

	Persistent*			CreateScrollView(
								View*				inSuperView,
								const CFXMLTree&	inTree);

	Persistent*			CreateSearchField(
								View*				inSuperView,
								const CFXMLTree&	inTree);

	Persistent*			CreateSegmentedView(
								View*				inSuperView,
								const CFXMLTree&	inTree);

	Persistent*			CreateSeparator(
								View*				inSuperView,
								const CFXMLTree&	inTree);

	Persistent*			CreateSlider(
								View*				inSuperView,
								const CFXMLTree&	inTree);

	Persistent*			CreateStaticText(
								View*				inSuperView,
								const CFXMLTree&	inTree);

	Persistent*			CreateTab(
								View*				inSuperView,
								const CFXMLTree&	inTree);

	Persistent*			CreateTextView(
								View*				inSuperView,
								const CFXMLTree&	inTree);

	Persistent*			CreateTriangle(
								View*				inSuperView,
								const CFXMLTree&	inTree);

	Persistent*			CreateUserPane(
								View*				inSuperView,
								const CFXMLTree&	inTree);

} // namespace NibDecoderFuncs

} // namespace PPx

#endif	// H_NibDecoderFuncs

