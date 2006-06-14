// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	Copyright© 1996 Metrowerks Corp. All rights reserved

---------------------------------------<< ¥ >>----------------------------------------

FILE:					GAMWBanner.h

CLASSES:				GAMWBanner

DESCRIPTION:		

AUTHOR:				Robin Mair

CREATION DATE :	96.06.06		

CHANGE HISTORY :

		96.06.06		rtm	Initial creation of file
	
---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

This class is used to render the Metrowerks banner as designed for the new UI for the
IDE.  It renders a 3D effect banner that has the MW construction stripes on the left
edge. It uses colors similar to those specified for Apple's Grayscale Appearance.


---------------------------------------<< ¥ >>----------------------------------------
*/

#ifndef _H_GAMWBanner
#define _H_GAMWBanner

#ifdef MW_USES_PRAGMA_ONCE
#pragma once
#endif

// ¥ POWERPLANT HEADERS
#include <LView.h>


//==================================================================================== 
//	CONSTANTS
//==================================================================================== 

//==================================================================================== 
//	FORWARD DECLARATIONS
//==================================================================================== 

class LPeriodical;

//==================================================================================== 
//	FUNCTIONS
//==================================================================================== 

//==================================================================================== 
//	GAMWBanner
//==================================================================================== 
	
class GAMWBanner : public LView 
{
	
	//====<< ¥ CLASS ID ¥ >>===========================================================

public:

	enum	{ class_ID = 'Gban' };
	
	
	//====<< ¥ FIELDS ¥ >>=============================================================
											
	//====<< ¥ METHODS ¥ >>============================================================
		
	//----<< ¥ INITIALIZATION & DISPOSAL ¥ >>------------------------------------------

public:

							GAMWBanner 			();									//	¥ Constructor
							GAMWBanner 			( LStream*	inStream );			//	¥ Constructor
							~GAMWBanner 		();									//	¥ Destructor
										

	//----<< ¥ ACTIVATION ¥ >>---------------------------------------------------------

	virtual	void		ActivateSelf		();										// ¥ OVERRIDE
	virtual	void		DeactivateSelf		();										// ¥ OVERRIDE
	virtual	void		EnableSelf			();										// ¥ OVERRIDE
	virtual	void		DisableSelf			();										// ¥ OVERRIDE
	
	
	//----<< ¥ DRAWING ¥ >>------------------------------------------------------------

	virtual	void		DrawSelf				();										//	¥ OVERRIDE

	virtual void		GetForeAndBackColors(
							RGBColor		*outForeColor,
							RGBColor		*outBackColor) const;
	virtual void		ApplyForeAndBackColors() const;

#ifdef MW_SUPPORTS_APPEARANCE_110
	void				DrawAppearanceBanner();
#endif

	void				DrawStripe(const Rect &inRect, SInt16 inBitDepth);


#if SPINNING_BANNERS
	virtual void		Click				(SMouseDownEvent &inMouseEvent);

private:
	static void			BuildBannerList(LArray &inArray);
	static void			FindAllMWBanners(LView *inView, LArray &inArray);
	
	static LPeriodical	*sBannerSpinner;
	static LArray		sBannerList;
	
	friend class BannerSpinner;
#endif

};	

#endif	// _H_GAMWBanner


