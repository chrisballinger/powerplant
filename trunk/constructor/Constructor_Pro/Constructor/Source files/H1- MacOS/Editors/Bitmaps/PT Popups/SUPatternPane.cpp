// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*
	96.10.31	ebs		Changed to use PAT# within Constructor rather than System 
						(per squeegee@concentric.net feature request)
	96.08.01	ebs		Fixed hiliting when clicked on	
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "SUMiscUtils.h"
#include "SUPatternPane.h"
#include "SUPatternPopup.h"
#include "SUFileUtils.h"
#include "SUPopupArtist.h"
#include "SUColorUtils.h"

/*===============================================
	SUPatternPane
================================================*/
SUPatternPane::SUPatternPane( LStream *inStream ) : LPane( inStream )
{
	mForeColor = kBlackColor;
	mBackColor = kWhiteColor;
	mCurrentIndex = 0;
	mPatternListH = nil;
	
	/*
		the refCon is formatted as follows:
			the low word has the PAT# resource id (-1 if no default).
			the high word has 0 -> application file, 1 -> system file
		
	*/
	SInt32		theRefCon = this->GetUserCon();
	ResIDT		theResID = LoWord( theRefCon );
	ResFileIDT	theFileID;
	
	if ( theResID != -1 )
	{
		if ( HiWord( theRefCon ) == 0 )
			theFileID = SUFileUtils::GetMainResourceFile();
		else
			theFileID = kSystemResFile;
			
		this->SetPatternList( theFileID, theResID, redraw_Dont );
	}	
}

/*===============================================
	CreateColorStream
================================================*/
SUPatternPane *SUPatternPane::CreateFromStream( LStream *inStream )
{
	return new SUPatternPane( inStream );
}


/*===============================================
	Destructor
================================================*/
SUPatternPane::~SUPatternPane()
{
	SUMiscUtils::DisposeHandle( mPatternListH );
}

/*===============================================
	GetPatternIndex
================================================*/
SInt32 SUPatternPane::GetPatternIndex()
{
	return( mCurrentIndex );
}

/*===============================================
	SetPatternIndex
================================================*/
void SUPatternPane::SetPatternIndex( SInt32 inIndex, ERedrawOptions inRedraw )
{
	if ( inIndex != mCurrentIndex )
	{
		mCurrentIndex = inIndex;
		SUMiscUtils::RedrawPaneAsIndicated( this, inRedraw );
	}
	
	this->BroadcastMessage( this->GetPaneID(), (void*) inIndex );
}

/*===============================================
	GetCurrentPattern
================================================*/
void SUPatternPane::GetCurrentPattern( Pattern *outPattern )
{
	ThrowIfNil_( mPatternListH );
	ThrowIf_( (mCurrentIndex < 0) || (mCurrentIndex >= (**mPatternListH).numPatterns) );

	*outPattern = (**mPatternListH).patterns[ mCurrentIndex ];
}

/*===============================================
	SetPatternList
================================================*/
void SUPatternPane::SetPatternList( ResFileIDT inFileID, ResIDT inResID, ERedrawOptions inRedraw )
{
	Boolean		wasLoaded;
	
	Handle h = SUFileUtils::GetResource( inFileID, 'PAT#', inResID, &wasLoaded );
	this->SetPatternList( (SPatternListH) h, inRedraw );
	
	if ( !wasLoaded )
		SUMiscUtils::DisposeHandle( h );
}

/*===============================================
	SetPatternList
================================================*/
void SUPatternPane::SetPatternList( SPatternListH inHandle, ERedrawOptions inRedraw )
{
	if ( inHandle )
		inHandle = (SPatternListH) SUMiscUtils::DuplicateHandle( (Handle) inHandle );
	
		// get rid of old pattern list (if any)
	if ( mPatternListH )
		SUMiscUtils::DisposeHandle( mPatternListH );
		
		// save the new list
	mPatternListH = inHandle;

		// change the current pattern to number 0 -- this will broadcast our new value
	this->SetPatternIndex( 0, redraw_Dont );
	
		// redraw if needed
	SUMiscUtils::RedrawPaneAsIndicated( this, inRedraw );
}

/*===============================================
	SetColors
================================================*/
void SUPatternPane::SetColors( Color32 inFore, Color32 inBack, ERedrawOptions inRedraw )
{
	mForeColor = SUColorUtils::Color32ToRGB( inFore );
	mBackColor = SUColorUtils::Color32ToRGB( inBack );
	SUMiscUtils::RedrawPaneAsIndicated( this, inRedraw );
}

/*==================================================
	DrawSelf
===================================================*/
void SUPatternPane::DrawSelf()
{
	Rect				r, swatchR;
	
	CalcLocalFrameRect( r );
	SUPopupArtist::DrawPopup( r, false /* not hilited */, IsEnabled() );

	SUPopupArtist::CalculateSwatchArea( r, &swatchR );
	this->DrawSwatchSelf( swatchR );
}

/*==================================================
	ClickSelf
===================================================*/
void SUPatternPane::ClickSelf( const SMouseDownEvent & )
{
	if ( !mPatternListH ) return;
	
	Rect	r;
	Point	pt;

		// find the bottom/left of the control
	this->CalcPortFrameRect( r );
	this->PortToGlobalPoint( topLeft(r) );
	this->PortToGlobalPoint( botRight(r) );
	pt.h = r.left;
	pt.v = r.bottom;
	
		// hilite the control
	this->DrawPopup( true /* hilited */, true /* enabled */ );

		// show the popup & track the user's actions
	SInt32 newIndex = SUPatternPopup::DoPatternPopup( pt, mPatternListH, mCurrentIndex,
										mForeColor, mBackColor );

		// note: ColorIndex is only a Byte and newColor may be -1, so watch out
	if ( newIndex != -1 )
		this->SetPatternIndex( newIndex );

		// unhilite the control
	this->DrawPopup( false /* not hilited */, true /* enabled */ );
}

/*==================================================
	DrawPopup
===================================================*/
void SUPatternPane::DrawPopup( Boolean inHilited, Boolean inEnabled )
{
	Rect	localR, swatchR;

	this->FocusDraw();
	this->CalcLocalFrameRect( localR );
	
	SUPopupArtist::DrawPopup( localR, inHilited, inEnabled );
	
	SUPopupArtist::CalculateSwatchArea( localR, &swatchR );
	this->DrawSwatchSelf( swatchR );
}

/*==================================================
	DrawSwatch
===================================================*/
void SUPatternPane::DrawSwatch()
{
	Rect	localR, swatchR;
	
	this->FocusDraw();
	this->CalcLocalFrameRect( localR );

	SUPopupArtist::CalculateSwatchArea( localR, &swatchR );	
	this->DrawSwatchSelf( swatchR );
}

/*==================================================
	DrawSwatchSelf
===================================================*/
void SUPatternPane::DrawSwatchSelf( const Rect &swatchR )
{
	if ( !mPatternListH || (mCurrentIndex < 0) || (mCurrentIndex >= (**mPatternListH).numPatterns) ) return;
	
	StColorPenState		oldPenState;
	oldPenState.Normalize();
	
	StHandleLocker		aLock( (Handle) mPatternListH );
	
	::PenNormal();
	::RGBForeColor( &mForeColor );
	::RGBBackColor( &mBackColor );
	::PenPat( &(**mPatternListH).patterns[ mCurrentIndex ] );
	::PaintRect( &swatchR );
}
	

