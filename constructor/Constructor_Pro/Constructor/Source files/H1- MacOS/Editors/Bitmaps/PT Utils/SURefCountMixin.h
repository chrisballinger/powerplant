// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*	
---------------------------------------<< ¥ >>----------------------------------------

	Copyright© 1996 Metrowerks Corp. All rights reserved.
	This file is a confidential trade secret of Metrowerks Corp.
		
---------------------------------------<< ¥ >>----------------------------------------

FILE:				SURefCountMixin.h

CLASSES:			SURefCountMixin

DESCRIPTION:		

AUTHOR:				Eric Shapiro

CREATION DATE :		96.4.18

CHANGE HISTORY :

		96.4.18		ebs	Integrated into Robin's sources
	
---------------------------------------<< ¥ >>----------------------------------------

	THEORY OF OPERATION

	This class can be used as a mix-in for objects than can be
	shared from other objects. When the reference count goes
	to zero, the object is deleted.
	
---------------------------------------<< ¥ >>----------------------------------------
*/

#pragma once

class SURefCountMixin
{
	public:
							SURefCountMixin();
		virtual				~SURefCountMixin();
		
		virtual void		IncrementRefCount();
		virtual void		DecrementRefCount();
		
	protected:
		UInt32				mRefCount;
};

