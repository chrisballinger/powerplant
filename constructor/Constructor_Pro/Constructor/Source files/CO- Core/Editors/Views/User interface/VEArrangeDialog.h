// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	VEArrangeDialog.h			© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 03/22/96
//     $Date: 2006/01/18 01:33:14 $
//	$History: VEArrangeDialog.h $
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/20/97   Time: 18:26
//	Updated in $/Constructor/Source files/CO- Core/Editors/Views/User interface
//	Improved commenting.
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 11/13/96   Time: 13:36
//	Updated in $/Constructor/Source files/Editors/Views/+ Generic view editor
//	Rearranged view editor classes.
//	
//	*****************  Version 1   *****************
//	User: scouten      Date: 10/16/96   Time: 01:59
//	Created
//	Comment: Baseline source 15 October 1996.
//	
// ===========================================================================

#pragma once

#include <LDialogBox.h>


// ===========================================================================
//		* Forward class declarations
// ===========================================================================

class LEditText;

	
// ===========================================================================
//		* Enums for alignment options
// ===========================================================================

enum EAlignHorizontal {
	alignHoriz_None							= 'hnon',
	alignHoriz_AlignLeft					= 'alft',
	alignHoriz_AlignVerticalCenters			= 'ahrz',
	alignHoriz_AlignRight					= 'argt',
	alignHoriz_DistributeHorizontal			= 'shrz',
	alignHoriz_DistributeHorizInContainer	= 'shct',
	alignHoriz_UserDefinedSpread			= 'husd',
	alignHoriz_Value						= 'hval'
};

enum EAlignVertical {
	alignVert_None							= 'vnon',
	alignVert_AlignTop						= 'atop',
	alignVert_AlignHorizontalCenters		= 'avrt',
	alignVert_AlignBottom					= 'abtm',
	alignVert_DistributeVertical			= 'svrt',
	alignVert_DistributeVertInContainer		= 'svct',
	alignVert_UserDefinedSpread				= 'vusd',
	alignVert_Value							= 'vval'
};


// ===========================================================================
//		* Structure that describes current alignment
// ===========================================================================

#pragma options align=mac68k		// GHD: This data gets written to disk
									// Enforce 68K data alignment
struct SAlignmentData {
	EAlignHorizontal	alignHorizontal;
	SInt16				horizSpread;
	EAlignVertical		alignVertical;
	SInt16				vertSpread;
};

typedef struct SAlignmentData SAlignmentData, *SAlignmentDataP, **SAlignmentDataH;

#pragma options align=reset


// ===========================================================================
//		* VEArrangeDialog
// ===========================================================================
//
//	VEArrangeDialog runs the Align Objects... dialog in the view editor.
//	It coordinates the drawing of the preview objects (the buttons that
//	simulate the change in alignment that is being proposed) and issues
//	the necessary commands to cause the alignment to take place once
//	the command is approved.
//
// ===========================================================================
	
class VEArrangeDialog : public LDialogBox {
	
public:
	enum { class_ID = 'ARRG' };
	
	static LPane*			CreateFromStream(LStream* inStream)
									{ return new VEArrangeDialog(inStream); }

							VEArrangeDialog(LStream* inStream);
	virtual					~VEArrangeDialog();

	// accessors

	inline EAlignHorizontal	GetHorizontalAlignment() const
									{ return mHorizontalAlignment; }
	virtual void			SetHorizontalAlignment(
									EAlignHorizontal inAlign);

	inline EAlignVertical	GetVerticalAlignment() const
									{ return mVerticalAlignment; }
	virtual void			SetVerticalAlignment(
									EAlignVertical inAlign);

	SInt16					GetHorizontalSpread() const;
	virtual void			SetHorizontalSpread(SInt16 inSpread);

	SInt16					GetVerticalSpread() const;
	virtual	void			SetVerticalSpread(SInt16 inSpread);
												
	virtual	void			GetArrangementData(
									SAlignmentData&			outAlignData) const;
	virtual void			SetArrangementData(
									const SAlignmentData&	inAlignData);

	virtual void			SaveArrangementData();

	// drawing behaviors

protected:
	virtual void			DrawSelf();
	virtual void			DrawHorizontalArrangement();
	virtual void			DrawVerticalArrangement();

	// preview setup

	virtual void			ListenToMessage(
									MessageT			inMessage, 
									void*				ioParam);
	virtual void			UpdatePreview();
	
	// initialization

	virtual void			FinishCreateSelf();


	// data members
	
protected:	
	EAlignHorizontal		mHorizontalAlignment;		// horizontal alignment setting
	EAlignVertical			mVerticalAlignment;			// vertical alignment setting

	LView*					mPreview;					// view that displays the preview
	LEditText*				mHorizSpreadValue;			// horizontal spread value
	LEditText*				mVertSpreadValue;			// vertical spread value
	LPane*					mSample1;					// first sample pane
	LPane*					mSample2;					// second sample pane
	LPane*					mSample3;					// third sample pane
	
	Point					mSample1Loc;				// original location of sample one
	Point					mSample2Loc;				// original location of sample two
	Point					mSample3Loc;				// original location of sample three
														
};