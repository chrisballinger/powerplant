// Copyright й2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyrightй 1996 Metrowerks Corp. All rights reserved

---------------------------------------<< е >>-----------------------------------------

FILE:					GAMWBanner.cp

CLASSES:				GAMWBanner

DESCRIPTION:		Implementation file for GAMWBanner

AUTHOR:				Robin Mair

CREATION DATE :	96.06.06

CHANGE HISTORY :

	98.01.21		mak	Implemented ApplyForeAndBackColors & GetForeAndBackColors so that
							sub views can depend on the proper background being set.
	97.02.08		rtm	Changed the rendering of the banner so that it is now drawn
	97.02.08		rtm	Changed the rendering of the banner so that it is now drawn
							in a dimmed state when it is deactivated, this was done to
							make it consistent with the way the grayscale classes work. It
							also makes the banner a little less conspicuous when in the
							background.
	97.02.03		rtm	Fixed a problem when drawing in B&W where the face of the banner
							was not getting painted which left things behind when the banner
							was resized.
	97.01.06		rtm	Fixed a bug when drawing across multiple devices where the screen
							depth was the same.
	96.11.22		rtm	Changed the rendering of the strips so that the yellow now has 
							some orange in it so that it looks more like the logo colors.
	
	96.06.06		rtm	Initial creation of file

---------------------------------------<< е >>-----------------------------------------
*/

#define SPINNING_BANNERS	1

#if PP_Target_Carbon
	#define MW_SUPPORTS_APPEARANCE_110 1
#endif

	// Prefix file for MSVC build
#ifdef WINVER
	#include "ctor.h"
#endif

#include "GAMWBanner.h"

// * GRAYSCALE HEADERS
#include <UGraphicsUtilities.h>
#include <UGAColorRamp.h>

#if SPINNING_BANNERS

struct SBannerRec {
	GAMWBanner		*banner;
	Rect			localFrame;
};

LArray GAMWBanner::sBannerList(sizeof(SBannerRec));

#endif

#ifdef MW_SUPPORTS_APPEARANCE_110

class UBannerEnvironment {
public:
	static void		InitEnvironment();

	static SInt16	GetMinimumDeviceDepth();
	static Boolean	HasColorDevice();
	
	static Boolean	HasPlatinumTheme();

	static void		UpdateDeviceInfo();
	static void		UpdateThemeInfo();

private:
	static 	Boolean			sIsInited;
	static	SInt16			sMinimumDeviceDepth;
	static 	Boolean			sHasColorDevice;
	static	Boolean			sHasPlatinumTheme;
};

Boolean			UBannerEnvironment::sIsInited = false;
SInt16			UBannerEnvironment::sMinimumDeviceDepth = 0;
Boolean			UBannerEnvironment::sHasColorDevice = false;
Boolean			UBannerEnvironment::sHasPlatinumTheme = false;

class StMWThemeDrawingState
{
	public:
		StMWThemeDrawingState();
		~StMWThemeDrawingState();

		void					Save();
		void					Restore();

		static void				Normalize();

	private:
		ThemeDrawingState		mThemeDrawingState;
};

class StDefaultDrawLoop {
public:
						StDefaultDrawLoop(
								const Rect	&inLocalRect);
	virtual				~StDefaultDrawLoop();
						
	virtual Boolean		NextDepth(
								SInt16		&outDepth);
								
	GDHandle			CurrentDevice() const { return mCurrentDevice; }
	SInt16				CurrentDepth() const { return mCurrentDepth; }
	Boolean 			CurrentDeviceHasColor() const;

	static SInt16		GetDefaultDeviceDepth();
	static Boolean		GetDefaultDeviceHasColor();

	static StDefaultDrawLoop	*GetCurrentDeviceLoop() { return sCurrentDrawLoop; }

protected:
	StClipRgnState	mClipState;
	Rect			mGlobalRect;
	GDHandle		mCurrentDevice;
	SInt16			mCurrentDepth;
	Boolean			mIsNestedLoop;

	static StDefaultDrawLoop	*sCurrentDrawLoop;
};

StDefaultDrawLoop		*StDefaultDrawLoop::sCurrentDrawLoop = nil;

void
UBannerEnvironment::InitEnvironment()
{
	if (sIsInited == false)
	{
		UpdateDeviceInfo();
		UpdateThemeInfo();
		sIsInited = true;
	}
}

inline SInt16
UBannerEnvironment::GetMinimumDeviceDepth()
{
	return sMinimumDeviceDepth;
}

inline Boolean
UBannerEnvironment::HasColorDevice()
{
	return sHasColorDevice;
}

inline Boolean
UBannerEnvironment::HasPlatinumTheme()
{
	return sHasPlatinumTheme;
}

void
UBannerEnvironment::UpdateDeviceInfo()
{
	sMinimumDeviceDepth = max_Int16;
	sHasColorDevice = false;

	GDHandle macDeviceH = ::GetDeviceList();
	while (macDeviceH)
	{
//		#if HOSTOS != MSDOS
		SInt16 bitDepth = (**((**macDeviceH).gdPMap)).pixelSize;
//		#else
//		SInt16 bitDepth = ::GetDevicePixelDepth(macDeviceH);
//		#endif
		
		if (bitDepth < sMinimumDeviceDepth)
			sMinimumDeviceDepth = bitDepth;

		if (::TestDeviceAttribute(macDeviceH, gdDevType))
			sHasColorDevice = true;

		macDeviceH = ::GetNextDevice(macDeviceH);
	}

	Assert_(sMinimumDeviceDepth < max_Int16);
}

void
UBannerEnvironment::UpdateThemeInfo()
{
	if (UEnvironment::HasFeature(env_HasAppearance11))
	{
		sHasPlatinumTheme = false;
		
		Collection collection = NewCollection();
		
		OSErr err = noErr;
		if (::GetTheme(collection) == noErr)
		{
			FSSpec macSpec = {0, 0, "\p"};
			SInt32 itemSize = sizeof(macSpec.name);
			err = ::GetCollectionItem(collection, kThemeAppearanceFileNameTag, 0, &itemSize, macSpec.name);
			if (err == noErr)
			{
				err = ::FindFolder(kOnSystemDisk, kThemesFolderType, kDontCreateFolder, &macSpec.vRefNum,
					&macSpec.parID);
				if (err == noErr)
				{
					FInfo finderInfo;
					err = ::FSpGetFInfo(&macSpec, &finderInfo);
					if (err == noErr)
						sHasPlatinumTheme = (finderInfo.fdType == kThemePlatinumFileType);
				}
			}
		}
		
		::DisposeCollection(collection);
	}
}

StMWThemeDrawingState::StMWThemeDrawingState()
{
	if (UEnvironment::IsAppearanceRunning())
		::GetThemeDrawingState(&mThemeDrawingState);
}

StMWThemeDrawingState::~StMWThemeDrawingState()
{
	if (UEnvironment::IsAppearanceRunning())
		::SetThemeDrawingState(mThemeDrawingState, true);
}

void					StMWThemeDrawingState::Save()
{
	if (UEnvironment::IsAppearanceRunning())
	{
		::DisposeThemeDrawingState(mThemeDrawingState);
		::GetThemeDrawingState(&mThemeDrawingState);
	}
}

void					StMWThemeDrawingState::Restore()
{
	if (UEnvironment::IsAppearanceRunning())
		::SetThemeDrawingState(mThemeDrawingState, false);
}

void					StMWThemeDrawingState::Normalize()
{
	if (UEnvironment::IsAppearanceRunning())
		::NormalizeThemeDrawingState();
}

StDefaultDrawLoop::StDefaultDrawLoop(
	const Rect	&inLocalRect)
{
	mGlobalRect = inLocalRect;			// Convert to Global coords
	::LocalToGlobal(&topLeft(mGlobalRect));
	::LocalToGlobal(&botRight(mGlobalRect));
	
	mCurrentDevice = nil;
	mCurrentDepth = 0;
	mIsNestedLoop = false;
}


StDefaultDrawLoop::~StDefaultDrawLoop()
{
	if (sCurrentDrawLoop == this)
		sCurrentDrawLoop = NULL;
}


Boolean
StDefaultDrawLoop::NextDepth(
	SInt16	&outDepth)
{
	outDepth = 0;

	if (sCurrentDrawLoop && (sCurrentDrawLoop != this))
	{
		if (mIsNestedLoop)
			mCurrentDevice = nil;
		else
		{
			mIsNestedLoop = true;
			mCurrentDevice = sCurrentDrawLoop->CurrentDevice();
			outDepth = sCurrentDrawLoop->CurrentDepth();
		}
	}
	else
	{
		sCurrentDrawLoop = this;

		if (mCurrentDevice == nil) {		// First call. Start at beginning
											//   of GDevice list.
			mCurrentDevice = ::GetDeviceList();
			
		} else {							// Subsequent call	
			mCurrentDevice = ::GetNextDevice(mCurrentDevice);
		}
		
		while (mCurrentDevice != nil) {
		
				// Find active screen device that intersects our
				// target rectangle
		
			if (UDrawingUtils::IsActiveScreenDevice(mCurrentDevice)) {
				Rect	deviceRect = (**mCurrentDevice).gdRect;
				Rect	intersection;
				
				if (::SectRect(&mGlobalRect, &deviceRect, &intersection)) {
				
						// Set clipping region to the intersection of the target
						// rectangle, the screen rectangle, and the original
						// clipping region

					::GlobalToLocal(&topLeft(intersection));
					::GlobalToLocal(&botRight(intersection));
					mClipState.ClipToIntersection(intersection);
//				#if HOSTOS == MSDOS
//					outDepth = ::GetDevicePixelDepth(mCurrentDevice);
//				#else
					outDepth = (**((**mCurrentDevice).gdPMap)).pixelSize;
//				#endif
					break;				// Exit device loop
				}
			}
			
			mCurrentDevice = ::GetNextDevice(mCurrentDevice);
		}
	}
	
	mCurrentDepth = outDepth;
	return (mCurrentDevice != nil);
}

inline Boolean
StDefaultDrawLoop::CurrentDeviceHasColor() const
{
	Assert_(mCurrentDevice);
	return (::TestDeviceAttribute(mCurrentDevice, gdDevType));
}

SInt16
StDefaultDrawLoop::GetDefaultDeviceDepth()
{
	SInt16 defaultDepth = UBannerEnvironment::GetMinimumDeviceDepth();

	if (sCurrentDrawLoop)
		defaultDepth = sCurrentDrawLoop->CurrentDepth();

	return defaultDepth;
}

Boolean
StDefaultDrawLoop::GetDefaultDeviceHasColor()
{
	Boolean defaultHasColor = UBannerEnvironment::HasColorDevice();

	if (sCurrentDrawLoop)
		defaultHasColor = sCurrentDrawLoop->CurrentDeviceHasColor();

	return defaultHasColor;
}

#endif

//=====================================================================================
// CONSTANTS
//=====================================================================================

#ifdef MW_USES_PRAGMA_MARK
#pragma mark === CLASS: GAMWBanner
#pragma mark -
#endif

//=====================================================================================
// CLASS:	GAMWBanner
//=====================================================================================

#ifdef MW_USES_PRAGMA_MARK
#pragma mark -
#pragma mark === INITIALIZATION & DESTRUCTION
#endif

//=====================================================================================
// ее INITIALIZATION & DESTRUCTION
//-------------------------------------------------------------------------------------
// GAMWBanner::GAMWBanner					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------

GAMWBanner::GAMWBanner ()
{
#ifdef MW_SUPPORTS_APPEARANCE_110
	UBannerEnvironment::InitEnvironment();
#endif
}	//	GAMWBanner::GAMWBanner


//-------------------------------------------------------------------------------------
// GAMWBanner::GAMWBanner					---<<е CONSTRUCTOR е>>---
//-------------------------------------------------------------------------------------

GAMWBanner::GAMWBanner (	LStream*	inStream ) : LView ( inStream )
{
#ifdef MW_SUPPORTS_APPEARANCE_110
	UBannerEnvironment::InitEnvironment();
#endif
	
#if SPINNING_BANNERS
	SBannerRec rec;
	rec.banner = this;
	rec.banner->CalcLocalFrameRect(rec.localFrame);
	::MacInsetRect ( &rec.localFrame, 2, 2 );
	rec.localFrame.right = rec.localFrame.left + 5;

	sBannerList.InsertItemsAt(1, LArray::index_Last, &rec);
#endif
}	//	GAMWBanner::GAMWBanner


//-------------------------------------------------------------------------------------
// GAMWBanner::~GAMWBanner				---<<е DESTRUCTOR е>>---
//-------------------------------------------------------------------------------------

GAMWBanner::~GAMWBanner ()
{
#if SPINNING_BANNERS
	SInt32 indexToRemove = 0;
	SInt32 count = sBannerList.GetCount();
	SInt32 index;
	for(index = 1; index <= count; index++)
		if(this == ((SBannerRec*) sBannerList.GetItemPtr(index))->banner)
		{
			indexToRemove = index;
			break;	
		}
	
	if(indexToRemove != 0)
		sBannerList.RemoveItemsAt(1, indexToRemove);
#endif
}	//	GAMWBanner::~GAMWBanner


#ifdef MW_USES_PRAGMA_MARK
#pragma mark -
#pragma mark === ACTIVATION
#endif

//=====================================================================================
// ее ACTIVATION
//-------------------------------------------------------------------------------------
// GAMWBanner::ActivateSelf
//-------------------------------------------------------------------------------------

void			
GAMWBanner::ActivateSelf	()
{
	
	Refresh ();
	
}	//	GAMWBanner::ActivateSelf


//-------------------------------------------------------------------------------------
// GAMWBanner::DeactivateSelf
//-------------------------------------------------------------------------------------

void			
GAMWBanner::DeactivateSelf	()
{
	
	Refresh ();
		
}	//	GAMWBanner::DeactivateSelf

//-------------------------------------------------------------------------------------
// GAMWBanner::EnableSelf
//-------------------------------------------------------------------------------------
void			
GAMWBanner::EnableSelf	()
{
	
	Refresh ();
	
}	//	GAMWBanner::ActivateSelf


//-------------------------------------------------------------------------------------
// GAMWBanner::DisableSelf
//-------------------------------------------------------------------------------------

void			
GAMWBanner::DisableSelf	()
{
	
	Refresh ();
		
}	//	GAMWBanner::DeactivateSelf


#ifdef MW_USES_PRAGMA_MARK
#pragma mark -
#pragma mark === DRAWING
#endif

//=====================================================================================
// ее DRAWING
//-------------------------------------------------------------------------------------
// GAMWBanner::DrawSelf
//-------------------------------------------------------------------------------------

void
GAMWBanner::DrawSelf ()
{
#ifdef MW_SUPPORTS_APPEARANCE_110
	if (UEnvironment::IsAppearanceRunning() && !UBannerEnvironment::HasPlatinumTheme())
		DrawAppearanceBanner();
	else
#endif
	{
		StColorPenState	thePenState;
		thePenState.Normalize();
		
		RGBColor	tempColor;
		
		// е Get the frame rect and calculate the width and height of the pane
		Rect	frameRect, adornRect;
		CalcLocalFrameRect ( frameRect );
		adornRect = frameRect;
		
		// е Start by framing the banner in black
		tempColor = UGAColorRamp::GetBlackColor();
		::RGBForeColor ( &tempColor );
		::MacFrameRect ( &frameRect );
		
		// е Setup a device loop so that we can handle drawing at the correct bit depth
		StDeviceLoop	theLoop ( frameRect );
		SInt16				depth;
		while ( theLoop.NextDepth ( depth )) 
		{
		
			StColorPenState::Normalize ();
			
			// е BLACK & WHITE
			if ( depth < 4 )
			{
				// е Start by framing the banner in black
				tempColor = UGAColorRamp::GetBlackColor();
				::RGBForeColor ( &tempColor );
				::MacFrameRect ( &frameRect );
		
				// е Paint the content area of the banner
				tempColor = UGAColorRamp::GetWhiteColor();
				::RGBForeColor (&tempColor);
				::MacInsetRect ( &frameRect, 1, 1 );
				::PaintRect ( &frameRect );			
			}
			// е COLOR
			else
			{
				// е Start by framing the banner in black
				tempColor = IsEnabled () && IsActive () ? UGAColorRamp::GetBlackColor()
													: UGAColorRamp::GetColor(colorRamp_Gray7);
				::RGBForeColor ( &tempColor );
				::MacFrameRect ( &frameRect );
		
				// е Start by filling the banner with the face color
				tempColor = IsEnabled () && IsActive () ? 
										UGAColorRamp::GetColor(colorRamp_Gray4)
											: UGAColorRamp::GetColor(colorRamp_Gray3);
				::RGBForeColor ( &tempColor );
				::MacInsetRect ( &frameRect, 1, 1 );
				::PaintRect ( &frameRect );
				
				// е We only handle the rendering of the shading when we are
				// enabled and active
				if ( IsEnabled () && IsActive ())
				{
					RGBColor tempColor;
					// е SHADING
					// е LIGHT EDGE
// JWW - I didn't bother to snag the GetXPlatColor from the IDE since this is mac only
//					GetXPlatColor(tempColor, COLOR_3DHILIGHT, colorRamp_White);
					tempColor = UGAColorRamp::GetColor(colorRamp_White);
					::RGBForeColor ( &tempColor );
					::MoveTo ( frameRect.left, frameRect.bottom - 2 );
					::MacLineTo ( frameRect.left, frameRect.top );
					::MacLineTo ( frameRect.right - 2, frameRect.top );
					
					// е SHADOW EDGE
// JWW - I didn't bother to snag the GetXPlatColor from the IDE since this is mac only
//					GetXPlatColor(tempColor, COLOR_3DSHADOW, colorRamp_Gray7);
					tempColor = UGAColorRamp::GetColor(colorRamp_Gray7);
					::RGBForeColor ( &tempColor );
					::MoveTo ( frameRect.left + 1, frameRect.bottom - 1 );
					::MacLineTo ( frameRect.right - 1, frameRect.bottom - 1 );
					::MacLineTo ( frameRect.right - 1, frameRect.top + 1 );
				}
				
				// е Restore the rect
				::MacInsetRect ( &frameRect, -1, -1 );
				
			}

			// е Now draw the stripe adornment
			DrawStripe(adornRect, depth);

			// е Restore the adorn rect to the original frame rect
			adornRect = frameRect;
		}
	}		
}	//	GAMWBanner::DrawSelf

// ---------------------------------------------------------------------------
//	е GetForeAndBackColors											  [public]
// ---------------------------------------------------------------------------

void
GAMWBanner::GetForeAndBackColors(
	RGBColor	*outForeColor,
	RGBColor	*outBackColor) const
{
	if (outForeColor != nil) {
		*outForeColor = Color_Black;
	}
	
	if (outBackColor != nil) {
		*outBackColor = IsEnabled () && IsActive () ? 
									UGAColorRamp::GetColor(colorRamp_Gray4)
										: UGAColorRamp::GetColor(colorRamp_Gray3);
	}
}

//------------------------------------------------------------------------------------
//	LBanner::ApplyForeAndBackColors
//------------------------------------------------------------------------------------
//
//	The banner's face color is set as the background color

void
GAMWBanner::ApplyForeAndBackColors() const
{
#ifdef MW_SUPPORTS_APPEARANCE_110
	if (UEnvironment::IsAppearanceRunning() && !UBannerEnvironment::HasPlatinumTheme())
	{
		LView *focusView = GetInFocusView();
		if (focusView)
		{
			Rect localFrame;
			focusView->CalcLocalFrameRect(localFrame);

			ThemeDrawState drawState = kThemeStateInactive;
			if (IsActive() && IsEnabled())
				drawState = kThemeStateActive;

			::ApplyThemeBackground(kThemeBackgroundPlacard, &localFrame, drawState, StDefaultDrawLoop::GetDefaultDeviceDepth(),
				StDefaultDrawLoop::GetDefaultDeviceHasColor());
		}
		else
			LView::ApplyForeAndBackColors();
	}
	else
#endif
	{
		RGBColor	foreColor;
		RGBColor	backColor;
		GetForeAndBackColors(&foreColor, &backColor);
		::RGBForeColor(&foreColor);
		::RGBBackColor(&backColor);
	}
}

#ifdef MW_SUPPORTS_APPEARANCE_110
void
GAMWBanner::DrawAppearanceBanner()
{
	StMWThemeDrawingState saveDrawingState;
	StMWThemeDrawingState::Normalize();

	ThemeDrawState drawState = kThemeStateInactive;
	if (IsActive() && IsEnabled())
		drawState = kThemeStateActive;

	Rect localFrame;
	CalcLocalFrameRect(localFrame);

	::DrawThemePlacard(&localFrame, drawState);

	DrawStripe(localFrame, StDefaultDrawLoop::GetDefaultDeviceDepth());

#if 0
	// е Get the frame rect and calculate the width and height of the pane
	Rect	frameRect, adornRect;
	CalcLocalFrameRect ( frameRect );
	adornRect = frameRect;
	
	// е Setup a device loop so that we can handle drawing at the correct bit depth
	StDeviceLoop	theLoop ( frameRect );
	Int16				depth;
	while ( theLoop.NextDepth ( depth )) 
	{
		StMWThemeDrawingState::Normalize ();
		
		::SetThemePen(kThemeBrushBlack, depth, true);
		::MacFrameRect ( &frameRect );

		ApplyForeAndBackColors();
		::MacInsetRect ( &frameRect, 1, 1 );
		::EraseRect ( &frameRect );

		// е We only handle the rendering of the shading when we are
		// enabled and active
		bool isActive = IsActive() && IsEnabled();
		bool hasColor = theLoop.CurrentDeviceHasColor();
		if (isActive)
		{
			// е SHADING
			// е LIGHT EDGE
			::SetThemePen(kThemeBrushBevelActiveLight, depth, hasColor);
			::MoveTo ( frameRect.left, frameRect.bottom - 2 );
			::MacLineTo ( frameRect.left, frameRect.top );
			::MacLineTo ( frameRect.right - 2, frameRect.top );

			// е SHADOW EDGE
			::SetThemePen(kThemeBrushBevelActiveDark, depth, hasColor);
			::MoveTo ( frameRect.left + 1, frameRect.bottom - 1 );
			::MacLineTo ( frameRect.right - 1, frameRect.bottom - 1 );
			::MacLineTo ( frameRect.right - 1, frameRect.top + 1 );
		}
		
		// е Restore the rect
		::MacInsetRect ( &frameRect, -1, -1 );

		// е Now draw the stripe adornment
		DrawStripe(adornRect, depth);

		// е Restore the adorn rect to the original frame rect
		adornRect = frameRect;
	}
#endif
}
#endif

void
GAMWBanner::DrawStripe(const Rect &inRect, SInt16 inBitDepth)
{
	StColorPenState thePenState;
	StColorPenState::Normalize ();
	
	Rect adornRect = inRect;
	::MacInsetRect ( & adornRect, 2, 2 );
	adornRect.right = adornRect.left + 5;
	RGBColor tempColor = (inBitDepth < 4) ? UGAColorRamp::GetBlackColor () : IsEnabled () && 
			IsActive () ? UGAColorRamp::GetBlackColor () : UGAColorRamp::GetColor(colorRamp_Gray7);
	::RGBForeColor ( &tempColor );
	::PaintRect ( &adornRect );
	adornRect.bottom -= 1;
	
	// е Setup the stripe color
	RGBColor yellow = { 65535, 65535, 0 };
	RGBColor mediumOrange = { 65535, 52428, 0 };
	RGBColor darkOrange = { 52428, 39321, 0 };
	RGBColor	white = UGAColorRamp::GetWhiteColor();
	
	// е Now draw the actual stripes, before we draw we are going to setup the
	// clipping region to be the inersection of the current clip and the rect we
	// are drawing into, in case the drawing tries to spill over if the user has
	// used an odd size for the banner
	// DDK 991022: For the new Windows build, we don't have the Mac-style pen sizes; that is
	// we can't create a 1x3 size pen; we can only set the width, regardless of direction
	// To address that, instead of drawing 1 pixel with a 1x3 pen, which actually draws 3 pixels,
	// we simply draw 3 pixels with 1x1 pen. The net effect is the same, and we don't rely on
	// the PenSize
	{
		StClipRgnState saveClip;
		saveClip.ClipToIntersection ( adornRect );
		SInt16 height = UGraphicUtils::RectHeight ( adornRect );
		SInt16 start = 1;
		::PenSize ( 1, 1 );
		while ( start < height ) 
		{
			tempColor =  (inBitDepth < 4) ? white : IsEnabled () && IsActive () ? yellow :
												UGraphicUtils::Lighten ( yellow ) ;
			::RGBForeColor ( &tempColor );
			
			for (short i = 0; i < 3; ++i)
			{
				::MoveTo ( adornRect.left + 1, adornRect.top + start + i );
				::MacLineTo ( adornRect.left + 1, adornRect.top + start + i );
			}
			
			tempColor =  (inBitDepth < 4) ? white : IsEnabled () && IsActive () ? mediumOrange :
												UGraphicUtils::Lighten ( mediumOrange ) ;
			::RGBForeColor ( &tempColor );
			
			for (short i = 0; i < 3; ++i)
			{
				::MoveTo ( adornRect.left + 2, adornRect.top + start + 1 + i);
				::MacLineTo ( adornRect.left + 2, adornRect.top + start + 1 + i);
			}
			
			tempColor =  (inBitDepth < 4) ? white : IsEnabled () && IsActive () ? darkOrange :
												UGraphicUtils::Lighten ( darkOrange ) ;
			::RGBForeColor ( &tempColor );

			for (short i = 0; i < 3; ++i)
			{
				::MoveTo ( adornRect.left + 3, adornRect.top + start + 2 + i);
				::MacLineTo ( adornRect.left + 3, adornRect.top + start + 2 + i);
			}
			
			start += 6;
		}
	}
}

#if SPINNING_BANNERS

const UInt32	kRotationDelayTicks		=	1;
const SInt16	kSecretModifierKeys		=	(cmdKey | optionKey);

LPeriodical*	GAMWBanner::sBannerSpinner = NULL;


class BannerSpinner : public LPeriodical 
{
public:
					BannerSpinner();
	virtual void	SpendTime(const EventRecord &inEvent);

private:
	LArray			mBannerList;
	SInt16			mStripeOffset;
	UInt32			mLastRotationTime;
};

BannerSpinner::BannerSpinner()
	: mBannerList(sizeof(SBannerRec))
{
	mStripeOffset = 1;
	mLastRotationTime = 0;
}

void BannerSpinner::SpendTime(const EventRecord & /* inEvent */)
{
	UInt32 time = TickCount();
	if(time - mLastRotationTime < kRotationDelayTicks)
		return;
	
	mLastRotationTime = time;

	GrafPtr		savePort;
	::GetPort(&savePort);

	RGBColor black = { 0, 0, 0 };
	// е Setup the stripe colors
	RGBColor yellow = { 65535, 65535, 0 };
	RGBColor mediumOrange = { 65535, 52428, 0 };
	RGBColor darkOrange = { 52428, 39321, 0 };
	
	LArrayIterator iter(GAMWBanner::sBannerList, LArrayIterator::from_Start);
	SBannerRec rec;

	while(iter.Next(&rec))
	{
		if(rec.banner->IsVisible() && rec.banner->FocusDraw())
		{

			StColorPenState save;
			StColorPenState::Normalize();

			::PenSize(1, 3);

			Rect s = rec.localFrame;
			::MacInsetRect(&s, 1, 1);
			StClipRgnState clip(s);

			RGBForeColor(&black);
			::PaintRect(&s);
			
			// е Now draw the actual stripes, before we draw we are going to setup the
			// clipping region to be the inersection of the current clip and the rect we
			// are drawing into, in case the drawing tries to spill over if the user has
			// used an odd size for the banner
			SInt16 height = s.bottom - s.top;
			SInt16 start = -height + mStripeOffset;
			while ( start < height ) 
			{
				::RGBForeColor(&yellow);
				::MoveTo(s.left + 0, s.top + start);
				::Line(0, 0);
				::RGBForeColor(&mediumOrange);
				::MoveTo(s.left + 1, s.top + start + 1);
				::Line(0, 0);
				::RGBForeColor(&darkOrange);
				::MoveTo(s.left + 2, s.top + start + 2);
				::Line(0, 0);
				start += 6;
			}
		}
	}

	mStripeOffset += 1;
	if(mStripeOffset == 6)
		mStripeOffset = 0;
	
	::MacSetPort(savePort);
	LView::OutOfFocus(NULL);
}
#endif

void GAMWBanner::Click(SMouseDownEvent &inMouseEvent)
{
	Point local = inMouseEvent.whereLocal;
	PortToLocalPoint(local);

	if((inMouseEvent.macEvent.modifiers & kSecretModifierKeys) == kSecretModifierKeys)
	{
		Rect	poleRect;
	
		CalcLocalFrameRect(poleRect);
		::MacInsetRect ( &poleRect, 2, 2 );
		poleRect.right = poleRect.left + 5;

		if(MacPtInRect(local, &poleRect))
		{
			if(sBannerSpinner == NULL)
			{
				sBannerSpinner = new BannerSpinner;
				sBannerSpinner->StartRepeating();
			}
			else
			{
				delete sBannerSpinner;
				sBannerSpinner = NULL;
			}
			
			return;
		}
	}

	LView::Click(inMouseEvent);
}
