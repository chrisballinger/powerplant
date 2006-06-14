// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*
	96.08.29	ebs		16 color popups now show as 2 rows of 8 instead of 12 & 4
*/

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "SUColorPopup.h"
#include "SU_Constants.h"
#include "SU_Macros.h"

const SInt32	kMargins			= 1;

/*=========================================
	SUColorPopup::DoColorPopup
	
	Description:
	This is the main (and only) entry point into the Color Popup code.
	It opens the popup window, handles user selection, and returns the
		item chosen by the user (0..n-1) or -1 if nothing.
==========================================*/
SInt32 SUColorPopup::DoColorPopup( Point inTopLeft, CTabHandle inTable, SInt32 defaultItem )
{
	SUColorPopup		thePopup( inTable );		// note: object on stack
	
	return( thePopup.DoPopup( inTopLeft, defaultItem ) );
}

/*=========================================
	SUColorPopup Constructor
==========================================*/
SUColorPopup::SUColorPopup( CTabHandle inTable )
{
	mColorTable = inTable;
}

/*=========================================
	SUColorPopup Destructor
==========================================*/
SUColorPopup::~SUColorPopup()
{
}

/*=========================================
	SUColorPopup::Prepare
	
	Must set: mItems, mRows, mCols, mFirstitemRect, mCellWidth, mCellHeight, mWindowRect
==========================================*/
void SUColorPopup::Prepare( Point inTopLeft )
{
	ThrowIfNil_( mColorTable );
	
	mItems = (**mColorTable).ctSize + 1;
	
	/*
		give the popup a nice look
	*/
	//mCols = ((Int32) sqrt( mItems )) * 2;		// gives a nice rectangular look

	if ( mItems <= 16 )
		mCols = MIN( mItems, 8 );
	else if ( mItems <= 36 )
		mCols = MIN( mItems, 12 );
	else
		mCols = MIN( mItems, 32 );
	
	mRows = 1 + (mItems - 1) / mCols;

	if ( mItems <= 36 )
	{
		mCellWidth = 21;		// includes space between cells
		mCellHeight = 21;
	}
	else
	{
		mCellWidth = 11;
		mCellHeight = 11;
	}
	

	::SetRect( &mFirstItemRect, kMargins, kMargins, 
				kMargins + mCellWidth, kMargins + mCellHeight );
	
	mWindowRect.left = inTopLeft.h + 3;
	mWindowRect.right = mWindowRect.left + kMargins * 2 + mCols * mCellWidth;
	mWindowRect.top = inTopLeft.v;
	mWindowRect.bottom = mWindowRect.top + kMargins * 2 + mRows * mCellHeight;
}

/*=========================================
	SUColorPopup::DrawInitialState
==========================================*/
void SUColorPopup::DrawInitialState()
{
	for ( SInt32 count = 0; count < mItems; count++ )
		this->DrawOneCell( count );
}

/*=========================================
	SUColorPopup::DrawOneCell
==========================================*/
void SUColorPopup::DrawOneCell( SInt32 inIndex, Boolean inHilited )
{
	Rect		r, itemR;

	if ( !mColorTable ) return;

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
	
		// draw the color swatch itself
	RGBColor	theRGB = (**mColorTable).ctTable[ inIndex ].rgb;
	::RGBForeColor( &theRGB );
	::InsetRect( &itemR, 1, 1 );
	::PaintRect( &itemR );
}


