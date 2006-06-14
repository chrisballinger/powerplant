// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				PTSnapTo.h

PROJECT:			Constructor Icon Editor

CLASSES:			PTSnapTo

DESCRIPTION:		

AUTHOR:				Eric Shapiro

CREATION DATE :		96.4.18

CHANGE HISTORY :

		97.07.02	ebs		Added SnapTo() routine
		96.4.18		ebs		Integrated into Robin's sources
	
---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

	This class is used to snap a point to a 45 or 90 degree
	relative to another point. It can be used separately from
	the Icon Editor.
	
---------------------------------------<< ¥ >>----------------------------------------
*/

#pragma once

class PTSnapTo
{
	public:
		enum ESnapTo { kDontSnap, kSnapTo45, kSnapTo90, kSnapToRect };
		
					PTSnapTo();
	
		void		SetAngle( ESnapTo angle );
		
		void 		SnapToPoint( Point startPt, Point currentPt, Point *outPt );	
		void		SnapTo( SInt32 origH, SInt32 origV, SInt32 newH, SInt32 newV, 
							SInt32 *outH, SInt32 *outV );
		
	protected:
		Boolean		mSnappingHorizontal;
		Boolean		mHaveMovedSinceMouseDown;
		ESnapTo		mSnapAngle;
		
		void		SnapTo45( Point startPt, Point currentPt, Point *outPt );
		void		SnapTo90( Point startPt, Point currentPt, Point *outPt );
		void		SnapToRect( Point startPt, Point currentPt, Point *outPt );
};

