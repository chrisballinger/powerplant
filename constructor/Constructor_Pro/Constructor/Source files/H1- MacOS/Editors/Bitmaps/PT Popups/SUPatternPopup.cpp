// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "SUPatternPopup.h"
#include "SU_Macros.h"				// for MIN
#include "SU_Constants.h"			// for kBlackColor, kWhiteColor

/*=========================================
	SUColorPopup::DoPatternPopup
	
	Description:
	This is the main (and only) entry point into the Pattern Popup code.
	It opens the popup window, handles user selection, and returns the
		item chosen by the user (0..n-1) or -1 if nothing.
==========================================*/
SInt32 SUPatternPopup::DoPatternPopup( Point inTopLeft, SPatternListH inPatterns, SInt32 defaultItem,
										const RGBColor &inFore, const RGBColor &inBack )
{
	SUPatternPopup		thePopup( inPatterns, inFore, inBack );		// note: object on stack
	
	return( thePopup.DoPopup( inTopLeft, defaultItem ) );
}

/*=========================================
	SUPatternPopup Constructor
==========================================*/
SUPatternPopup::SUPatternPopup( SPatternListH inPatterns, const RGBColor &inFore, const RGBColor &inBack )
{
	mPatternList = inPatterns;
	mForeColor = inFore;
	mBackColor = inBack;
}

/*=========================================
	SUPatternPopup Destructor
==========================================*/
SUPatternPopup::~SUPatternPopup()
{
}

/*=========================================
	Prepare
	
	Must set: mItems, mRows, mCols, mFirstitemRect, mCellWidth, mCellHeight
==========================================*/
void SUPatternPopup::Prepare( Point inTopLeft )
{
	ThrowIfNil_( mPatternList );
	
	mItems = (**mPatternList).numPatterns;
	
	mCols = MIN( mItems, 16 );				// MAX of 16 items across
	mRows = 1 + (mItems - 1) / mCols;

	mCellWidth = 16;
	mCellHeight = 16;
	SetRect( &mFirstItemRect, 1, 1, mCellWidth+1, mCellHeight+1 );
	
	mWindowRect.left = inTopLeft.h;
	mWindowRect.right = mWindowRect.left + mCols * mCellWidth + 2;
	mWindowRect.top = inTopLeft.v;
	mWindowRect.bottom = mWindowRect.top + mRows * mCellHeight + 2;
}

/*=========================================
	SUPatternPopup::DrawInitialState
==========================================*/
void SUPatternPopup::DrawInitialState()
{
	ThrowIfNil_( mPatternList );
	
	StColorPenState		aPenState;
	aPenState.Normalize();

	for ( SInt32 count = 0; count < mItems; count++ )
		this->DrawOneCell( count );
}

/*=========================================
	SUPatternPopup::DrawOneCell
==========================================*/
void SUPatternPopup::DrawOneCell( SInt32 inIndex, Boolean inHilited )
{
	Rect		r, itemR;

	if ( !mPatternList ) return;
	
	if ( !this->GetItemRect( inIndex, &itemR ) )
		return;
	r = itemR;

	if ( inHilited )
	{
		::RGBForeColor( &kWhiteColor );
		::FrameRect( &r );
		
		::RGBForeColor( &kBlackColor );
		::InsetRect( &r, -1, -1 );
		::FrameRect( &r );
	}
	else
	{
		::RGBForeColor( &kBlackColor );
		::FrameRect( &r );
		
		::RGBForeColor( &kWhiteColor );
		::InsetRect( &r, -1, -1 );
		::FrameRect( &r );
	}
	
		// draw the pattern swatch itself
	Pattern aPat = (**mPatternList).patterns[ inIndex ];
	::PenPat( &aPat );
	::RGBForeColor( &mForeColor );
	::RGBBackColor( &mBackColor );
	::InsetRect( &itemR, 1, 1 );
	::PaintRect( &itemR );

		// leave the pattern as black, color as black
	Pattern thePattern;
	UQDGlobals::GetBlackPat(&thePattern);
	::PenPat( &thePattern );
	::RGBForeColor( &kBlackColor );
	::RGBBackColor( &kWhiteColor );
}

#ifdef OLDWAY
/*=========================================
	SUPatternPopup::DrawOneCell
==========================================*/
void SUPatternPopup::DrawOneCell( Int32 inIndex, Boolean  )
{
	Rect		r;
	Pattern		aPat;

	this->GetItemRect( inIndex, &r );
	--r.right;
	--r.bottom;

	::RGBForeColor( &kBlackColor );
	::FrameRect( &r );
	

	::InsetRect( &r, 1, 1 );
	::PaintRect( &r );
}
#endif

