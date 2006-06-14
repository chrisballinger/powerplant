// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	UMemoryUtils.h				© 1996-97 Metrowerks Inc. All rights reserved.
// ===========================================================================
//
//   Created: 11/29/96
//     $Date: 2006/01/18 01:33:28 $
//  $History: UMemoryUtils.h $
//	
//	*****************  Version 4  *****************
//	User: scouten      QDate: 02/21/97   Time: 11:17
//	Updated in $/Constructor/Source files/CO- Core/Utility classes
//	Improved commenting.
//	
//	*****************  Version 3  *****************
//	User: scouten      QDate: 02/07/97   Time: 15:15
//	Updated in $/Constructor/Source files/CO- Core/Utility classes
//	Removed #include <stdexcept> (again).
//	
//	*****************  Version 2  *****************
//	User: scouten      QDate: 01/24/97   Time: 17:33
//	Updated in $/Constructor/Source files/CO- Core/Utility classes
//	Fixed CR/LF problem
//	
//	*****************  Version 1  *****************
//	User: scouten      QDate: 11/30/96   Time: 17:15
//	Created in $/Constructor/Source files/Utility classes
//	Added class.
//	
// ===========================================================================

#pragma once


// ===========================================================================
//		* UMemoryUtils
// ===========================================================================
//
//	UMemoryUtils contains a handful of routines for responding to low-memory
//	and out-of-memory conditions.
//
// ===========================================================================

class UMemoryUtils {

public:
	static bool				MemoryIsLow();
	
	static void				ShowOutOfMemoryAlert();
	static void				ShowLowMemoryAlert();


	// class variables

protected:
	static bool				sShowingAlert;
	
};


// ===========================================================================
//	¥ StCriticalSection
// ===========================================================================
//	StCriticalSection is a stack-based object that marks a "critical section,"
//	i.e. a section that should not be allowed to fail because memory is
//	running low. Examples of appropriate uses of StCriticalSection are
//	commands that close windows or save files.

class StCriticalSection {

public:
						StCriticalSection()
							{
								mWasInCriticalSection	= sCriticalSection;
								sCriticalSection		= true;
							}
								
	virtual				~StCriticalSection()
							{
								sCriticalSection = mWasInCriticalSection;
							}

	static	bool		InCriticalSection()
							{
								return sCriticalSection;
							}
	

private:

			bool		mWasInCriticalSection;
	static	bool		sCriticalSection;
};


// ===========================================================================
//	¥ StInterruptSection
// ===========================================================================
//	StInterruptSection is a stack-based object that marks an "interrupt
//	section," i.e. a section of code that (could) execute at interrupt time.
//	Useful in memory allocation to prohibit allocation at interrupt time.

class StInterruptSection {

public:
						StInterruptSection()
							{
								mWasInInterruptSection	= sInterruptSection;
								sInterruptSection		= true;
							}
						
	virtual				~StInterruptSection()
							{
								sInterruptSection = mWasInInterruptSection;
							}

	static	bool		InInterruptSection()
							{
								return sInterruptSection;
							}
	

private:
			bool		mWasInInterruptSection;
	static	bool		sInterruptSection;

};