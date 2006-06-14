// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	LAMPopupButtonImp.h			PowerPlant 2.2.2	©1997-2005 Metrowerks Inc.
// ===========================================================================

#ifndef _H_LAMPopupButtonImp
#define _H_LAMPopupButtonImp
#pragma once

#include <LAMControlImp.h>

#if PP_Uses_Pragma_Import
	#pragma import on
#endif

PP_Begin_Namespace_PowerPlant

// ---------------------------------------------------------------------------

class	LAMPopupButtonImp : public LAMControlImp {
public:
						LAMPopupButtonImp( LStream* inStream = nil );

	virtual				~LAMPopupButtonImp();

	virtual SInt16		FindHotSpot( Point inPoint ) const;

	virtual Boolean		TrackHotSpot(
								SInt16			inHotSpot,
								Point			inPoint,
								SInt16			inModifiers);

	virtual void		DrawSelf();

	virtual void		PostSetValue();

	virtual void		SetDataTag(
								SInt16			inPartCode,
								FourCharCode	inTag,
								Size			inDataSize,
								void*			inDataPtr);
								
	virtual void		FocusImp();

protected:
	virtual void		MakeMacControl(
								ConstStringPtr	inTitle,
								SInt32			inRefCon);

	virtual MenuHandle	GetMacMenuH() const;
	
	virtual void		AlignControlRect();

protected:
	ResType			mMenuHandleTag;
};

PP_End_Namespace_PowerPlant


#if PP_Uses_Pragma_Import
	#pragma import reset
#endif

#endif
