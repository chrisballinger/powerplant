// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
#pragma once

#include "PT_Headers.h"

class RFMap;

/*====================================
	Constants
=====================================*/
const ResType	resType_PaintFamilyInfo		= 'PTFM';
//const UInt32	flag_RemoveIfIconErased		= 0x0001;	// not used

/*===========================================
	PaintFamilyMember, PaintFamilyList
	
	These data structures describe how the icon family and ICON
	windows work -- which resources are displayed in which sample
	panes, etc.
	
	This allows us to add/remove additional resource types simply
	by editing the resource rather than changing the code.
	
	Note: everything is a long, so we have no alignment problems (68k vs. PowerPC)
============================================*/
typedef struct
{
	ResType				resourceType;		// icl8, ICN#, etc
	UInt32				flags;				
	SInt32				width, height;		// in pixels
	SInt32				depth, rowBytes;
	PaneIDT				samplePaneID;
	
		// if it has a mask also
	SInt32				maskOffset;			// # bytes into resource where the mask lives
	SInt32				maskRowBytes;
	PaneIDT				maskSamplePaneID;
} PaintFamilyMember;

typedef struct
{
	PaneIDT				defaultPane;		// which one is the initial sample pane
	SInt32				numEntries;			// 1..n
	PaintFamilyMember	members[1];	
} PaintFamilyList, **PaintFamilyListH;


/*====================================
	PTFamilyView
=====================================*/
class PTFamilyView : public PTPaintView
{
	public:
		enum { class_ID = 'PTIF' };

		static PTFamilyView*	OpenPaintWindow( ResIDT inPPobID, RFMap *inMap, ResIDT );
		
		static PTFamilyView*	CreateFromStream( LStream *inStream );
								PTFamilyView( LStream * );
		virtual					~PTFamilyView();
		virtual void			FinishCreateSelf();
		
		virtual void			InitializeFromResource( RFMap *inMap, ResIDT );
		virtual void			SaveAsResource( RFMap *, ResIDT );
	
		SInt32					GetZoomFactor( SInt32, SInt32, Boolean *outShowGrid );
			
		SInt32					GetFamilyMemberCount();
		void					GetFamilyMemberInfo( SInt32 index, PaintFamilyMember * );
	
	protected:
		PaintFamilyListH		mFamilyListH;
		
		virtual Boolean			InitializeOneMember( 
										RFMap *inMap, ResType inResType, ResIDT inResID,
										SInt32 inWidth, SInt32 inHeight, SInt32 inDepth,
										SInt32 inOffset, SInt32 inRowBytes,
										PaneIDT inSamplePaneID, Boolean inIsMask );

};

