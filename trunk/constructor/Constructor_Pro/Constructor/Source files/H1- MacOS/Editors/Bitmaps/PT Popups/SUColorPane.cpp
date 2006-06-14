// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*
	96.12.11	ebs		Added support for option-click for color picker
	96.11.25	ebs		Read data from stream rather than using refcon
*/

	// Prefix file for Windows build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "SUColorPane.h"
#include "SUColorPopup.h"
#include "SUColorUtils.h"
#include "SUMiscUtils.h"
#include "SUPopupArtist.h"

/*===============================================
	SUColorPane
================================================*/
SUColorPane::SUColorPane( LStream *inStream ) : LPane( inStream )
{
	RGBColor	anRGB;
	
	mColorTable = nil;	
	mClippedRgn = nil;
	
	/*
		read in the default color and whether we clip (from the PPob resource).
		note: Constructor for CW10 used the userCon field for the color -- CW11
				uses the stream data instead.
	*/
	*inStream >> anRGB.red;
	*inStream >> anRGB.green;
	*inStream >> anRGB.blue;
	mCurrentColor = SUColorUtils::RGBToColor32( anRGB );
	
	*inStream >> mClipsToSibblings;
	*inStream >> mUsePickerOption;
}

/*===============================================
	CreateColorStream
================================================*/
SUColorPane *SUColorPane::CreateFromStream( LStream *inStream )
{
	return new SUColorPane( inStream );
}


/*===============================================
	FinishCreateSelf
================================================*/
void SUColorPane::FinishCreateSelf()
{
		// (note: this is non-dynamic, but ok since we don't move panes around)
	this->CalcClipRegionForOverlap();
}

/*===============================================
	Destructor
================================================*/
SUColorPane::~SUColorPane()
{
	if ( mClippedRgn )
		::DisposeRgn( mClippedRgn );
		
	this->DisposeCurrentTable();
}

/*===============================================
	SUColorPane::CalcClipRegionForOverlap
================================================*/
void SUColorPane::CalcClipRegionForOverlap()
{
	// see if we are behind any other views and set our clip region

	Rect		ourRect, peerRect, dummyRect;
	RgnHandle	peerRgn = nil;
	
		// assume we don't clip at all -- get our entire region	
	mClippedRgn = ::NewRgn();
	ThrowIfMemFail_( mClippedRgn );

	peerRgn = ::NewRgn();
	ThrowIfMemFail_( peerRgn );
	
	CalcPortFrameRect( ourRect );
	::RectRgn( mClippedRgn, &ourRect );

	LView	*parentView = this->GetSuperView();

	if ( mClipsToSibblings )
	{
			// iterate through our parent view's list of panes
			// should we make this a recursive search ???
		LPane	*peerItem;
		
		LArrayIterator	anIterator( parentView->GetSubPanes() );
		
			// loop until we're done with the list or (more likely) we hit ourselves
			// since we draw on top of everything after us
		while( anIterator.Next( &peerItem ) && (peerItem != this) )
		{
			peerItem->CalcPortFrameRect( peerRect );
			if ( ::SectRect( &ourRect, &peerRect, &dummyRect ) )
			{
				::RectRgn( peerRgn, &peerRect );
				::DiffRgn( mClippedRgn, peerRgn, mClippedRgn );
			}
		}
	}
	
		// when we draw, the origin is the top/left of our superview,
		//  so offset the region accordingly
	parentView->CalcPortFrameRect( dummyRect );
	::OffsetRgn( mClippedRgn, -dummyRect.left, -dummyRect.top );
	::DisposeRgn( peerRgn );
}

/*===============================================
	SetColor
================================================*/
void SUColorPane::SetColor( Color32 inColor, ERedrawOptions inRedrawHow )
{
	/*
		only redraw if we need to (prevents flicker)
	*/
	if ( !SUColorUtils::EqualColor32( inColor, mCurrentColor ) )
	{
		mCurrentColor = inColor;

		if ( inRedrawHow == redraw_Now )
			this->DrawSwatch();				// prevent flicker -- just redraw swatch
		else
			SUMiscUtils::RedrawPaneAsIndicated( this, inRedrawHow );

		this->BroadcastMessage( this->GetPaneID(), (void*) inColor );
	}
}

/*===============================================
	GetColor
================================================*/
Color32 SUColorPane::GetColor()
{
	return( mCurrentColor );
}

/*===============================================
	GetColorTable
================================================*/
CTabHandle SUColorPane::GetColorTable()
{
	return( mColorTable );
}

/*===============================================
	SetColorTable
	
	Note:
	Nil is ok if you want to disassociate a table with this pane.
================================================*/
void SUColorPane::SetColorTable( CTabHandle inTable, Boolean inChangeColorToo, ERedrawOptions inRedrawHow )
{
	Handle	copyOfTable = inTable ? SUMiscUtils::DuplicateHandle((Handle)inTable) : nil;

	this->DisposeCurrentTable();
	mColorTable = (CTabHandle) copyOfTable;
	
		// change the currently displayed color if it's not in the
		// new table
	if ( mColorTable && inChangeColorToo )
	{
		UInt8		newIndex;
		RGBColor 	oldRGB = SUColorUtils::Color32ToRGB( mCurrentColor );

		if ( !SUColorUtils::IsColorInTable( mColorTable, oldRGB, &newIndex ) )
		{
			RGBColor	newRGB = (**mColorTable).ctTable[ newIndex ].rgb;
			this->SetColor( SUColorUtils::RGBToColor32(newRGB), inRedrawHow );
		}
	}
}

/*===============================================
	DisposeCurrentTable
================================================*/
void SUColorPane::DisposeCurrentTable()
{
	if ( mColorTable )
	{
		::DisposeCTable( mColorTable );
		mColorTable = nil;
	}
}

/*==================================================
	DrawSelf
	
	Note:
	ColorPanes often overlap. This wouldn't be a problem except that we need
	to redraw the pane outside of the event loop if the application tells us
	to change our color (while the dropper tool is being used, for example).
	
	So we clip.
===================================================*/
void SUColorPane::DrawSelf()
{
	Rect				r, swatchR;
	StClipRgnState		aClipObject( mClippedRgn );		// clip us to prevent overlap
	
	CalcLocalFrameRect( r );
	SUPopupArtist::DrawPopup( r, false /* not hilited */, IsEnabled() );

	SUPopupArtist::CalculateSwatchArea( r, &swatchR );
	this->DrawSwatchSelf( swatchR );
}


/*==================================================
	ClickSelf
===================================================*/
void SUColorPane::ClickSelf( const SMouseDownEvent &inEvent )
{
	Rect		portR, localR;
	Point		pt;
	Color32		colorChosen;
	
	this->CalcPortFrameRect( portR );
	this->CalcLocalFrameRect( localR );

		// find the bottom/left of the control in global coordinates
	this->PortToGlobalPoint( topLeft(portR) );
	this->PortToGlobalPoint( botRight(portR) );
	pt.h = portR.left;
	pt.v = portR.bottom;

		// hilite the control
	this->DrawPopup( true /* hilited */, true /* enabled */ );
	
		// prepare for drawing, clip to prevent overlap with other color pane
	this->FocusDraw();
	StClipRgnState		aClipObject( mClippedRgn );	
	
	if ( !mColorTable || (mUsePickerOption && (inEvent.macEvent.modifiers & optionKey)) )
	{
		if ( this->DoAppleColorPicker( &colorChosen ) )
			this->SetColor( colorChosen, redraw_Dont );
	}
	else
	{
			// show the popup & track the user's actions
		SInt32	defaultItem = this->GetColorIndex( mCurrentColor );
		SInt32	newColorIndex = SUColorPopup::DoColorPopup( pt, mColorTable, defaultItem );
		
			// save the new value (broadcast, redraw, etc)
			// note: ColorIndex is only a Byte and newColor may be -1, so watch out
		if ( newColorIndex != -1 )
		{
				// convert newColorIndex to a Color32
			RGBColor	colorChosenRGB = (**mColorTable).ctTable[ newColorIndex ].rgb;
			colorChosen = SUColorUtils::RGBToColor32( colorChosenRGB );

			this->SetColor( colorChosen, redraw_Dont );
		}
	}

		// unhilite the control
	this->DrawPopup( false /* not hilited */, true /* enabled */ );
}

/*==================================================
	DoAppleColorPicker
===================================================*/
Boolean SUColorPane::DoAppleColorPicker( Color32 *outColor )
{
	Point		pt = { -1, -1 };			// picker will choose location
	RGBColor	currentRGB = SUColorUtils::Color32ToRGB( mCurrentColor );
	RGBColor	newRGB;
	Boolean		colorChanged = false;
	
	UDesktop::Deactivate();
	
	if ( ::GetColor( pt, (StringPtr)"", &currentRGB, &newRGB ) )
		if ( !SUColorUtils::EqualRGB( currentRGB, newRGB ) )
		{
			*outColor = SUColorUtils::RGBToColor32( newRGB );
			colorChanged = true;
		}

	UDesktop::Activate();
	return( colorChanged );
}

/*==================================================
	DrawPopup
===================================================*/
void SUColorPane::DrawPopup( Boolean inHilited, Boolean inEnabled )
{
	Rect	localR, swatchR;

	this->FocusDraw();
	this->CalcLocalFrameRect( localR );
	
	StClipRgnState	aClip( mClippedRgn );		// don't draw on overlapped popup
	
	SUPopupArtist::DrawPopup( localR, inHilited, inEnabled );
	
	SUPopupArtist::CalculateSwatchArea( localR, &swatchR );
	this->DrawSwatchSelf( swatchR );
}

/*==================================================
	DrawSwatch
===================================================*/
void SUColorPane::DrawSwatch()
{
	Rect	localR, swatchR;
	
	this->FocusDraw();
	this->CalcLocalFrameRect( localR );

	StClipRgnState	aClip( mClippedRgn );		// don't draw on overlapped popup
	SUPopupArtist::CalculateSwatchArea( localR, &swatchR );
	
	this->DrawSwatchSelf( swatchR );
}

/*==================================================
	DrawSwatchSelf
===================================================*/
void SUColorPane::DrawSwatchSelf( const Rect &swatchR )
{
	RGBColor swatchRGB = SUColorUtils::Color32ToRGB( mCurrentColor );
	
	::PenNormal();
	::RGBForeColor( &swatchRGB );
	::PaintRect( &swatchR );
}

/*==================================================
	PointIsInFrame
	
	Notes:
	This is here because the user can click in the overlapped portion of 2
	  panes and PP selects the wrong one by default.
	Incoming points are in port coordinates.
	mClippedRgn is in local (superview relative) coordinates.
===================================================*/
Boolean SUColorPane::PointIsInFrame( SInt32 inHoriz, SInt32 inVert) const
{
	Point	pt = { inVert, inHoriz };
	
	if ( LPane::PointIsInFrame( inHoriz, inVert ) )
	{
		PortToLocalPoint( pt );	
		if ( ::PtInRgn( pt, mClippedRgn ) )
			return( true );
	}
	
	return( false );
}

/*==================================================
	GetColorIndex
===================================================*/
SInt32 SUColorPane::GetColorIndex( Color32 inColor )
{
	if ( !mColorTable ) return( -1 );
	
	RGBColor	theRGB = SUColorUtils::Color32ToRGB( inColor );
	UInt8		index;
	
	if ( SUColorUtils::IsColorInTable( mColorTable, theRGB, &index ) )
		return( index );
	
	return( -1 );
}

/*==================================================
	AllowPickerOption
===================================================*/
void SUColorPane::AllowPickerOption( Boolean inOption )
{
	mUsePickerOption = inOption;
}

