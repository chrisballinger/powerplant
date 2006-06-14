// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PPDebug_New.cp		©1997-1999 Metrowerks Inc. All rights reserved
//	Original Author: John C. Daub
// ===========================================================================
//	Overrides for C++ memory allocation mechanisms, to work better with
//	PowerPlant. Includes use of DebugNew and Spotlight as well (via the
//	Debugging Classes).
//
//	Contains support for: new, new[], new(nothrow), new[](nothrow), delete,
//	and delete[]. Also contains support for the DebugNew versions of new
//	that take file and line arguments. A new_handler implementation is
//	provided as well.

#include <PP_Debug.h>
#include <UOnyx.h>

#include "UMemoryUtils.h"
#include "Constructor.new.h"


// ===========================================================================
//	¥ MW standard operator new configuration
// ===========================================================================

#undef new
#include <new>
#include <stdlib.h>

void PP_NewHandler() throw(PP_STD::bad_alloc);	// prototype

	// NEWMODE_MALLOC seems to be the fastest allocator. Furthermore
	// using the MSL's malloc as the ultimate allocator allows you to
	// browse your new (malloc) heaps in ZoneRanger 2.x
#undef NEWMODE
#define NEWMODE NEWMODE_MALLOC


// ===========================================================================
//	¥ DebugNew configuration
// ===========================================================================

// this will generate some linker warnings about multiple definitions
// (between this and the MSL runtime libraries). FYI, the MW Linkers always
// use what they find first in the "Link Order" (or Segments) page of the
// project window.. top to bottom. However, regardless of order, the MW
// Linkers ALAWYS take source over libraries if they find duplicates.

#if PP_Debug && PP_DebugNew_Support
	#define DEBUG_NEW_NO_GLOBAL_OPERATORS 1		// suppress DebugNew's global
												// new/delete operators
	#include "DebugNew.cp"
#endif


// ===========================================================================


// ---------------------------------------------------------------------------
//	¥ AllowAllocation								[static]
// ---------------------------------------------------------------------------
//	Determines if it's ok to allocate memory or not. Typical use would
//	be to check the GrowZone, check for critical sections, etc. Returns
//	true if it's ok to proceed with the allocation.

static bool	AllowAllocation();

static bool
AllowAllocation()
{
	return	(not StInterruptSection::InInterruptSection() &&
				(StCriticalSection::InCriticalSection() ||
				not UMemoryUtils::MemoryIsLow())
			);
}


#pragma mark -
#pragma mark === operator new ===

// ---------------------------------------------------------------------------
//	¥ operator new(size_t) throw(bad_alloc)
// ---------------------------------------------------------------------------
//	Overriden to test for low-memory conditions.

void*
operator new(
	PP_CSTD::size_t	size) throw (PP_STD::bad_alloc)
{
	StSpotlightDisable_();
	
	void* ptr = nil;
	if (AllowAllocation())
	{
		for(;;)
		{
			#if PP_Debug && PP_DebugNew_Support && (DEBUG_NEW >= DEBUG_NEW_BASIC)
				ptr = DebugNewDoAllocate(size, 0, 0, malloc, false);
			#else
				ptr = malloc(size);
			#endif
			
			if (ptr != NULL)
				break;
			else
				PP_NewHandler();
		}
	}
	
	return ptr;
}


// ---------------------------------------------------------------------------
//	¥ operator new(size_t, nothrow_t&) throw()
// ---------------------------------------------------------------------------
//	Overriden to test for low-memory conditions.

void*
operator new(
	PP_CSTD::size_t				size,
	const PP_STD::nothrow_t& 	nt) throw ()
{
#pragma unused(nt)	// Keeps the compiler quiet

	StSpotlightDisable_();
	
	void* ptr = nil;
	if (AllowAllocation())
	{
		#if PP_Debug && PP_DebugNew_Support && (DEBUG_NEW >= DEBUG_NEW_BASIC)
			ptr = DebugNewDoAllocate(size, 0, 0, malloc, false);
		#else
			ptr = malloc(size);
		#endif
	}
	
	return ptr;
}


// ---------------------------------------------------------------------------
//	¥ operator new[](size_t) throw(bad_alloc)
// ---------------------------------------------------------------------------
//	Overriden to test for low-memory conditions.

void*
operator new[](
	PP_CSTD::size_t	size) throw (PP_STD::bad_alloc)
{
	StSpotlightDisable_();
	
	void* ptr = nil;
	if (AllowAllocation())
	{
		for(;;)
		{
			#if PP_Debug && PP_DebugNew_Support && (DEBUG_NEW >= DEBUG_NEW_BASIC)
				ptr = DebugNewDoAllocate(size, 0, 0, malloc, true);
			#else
				ptr = malloc(size);
			#endif
			
			if (ptr != NULL)
				break;
			else
				PP_NewHandler();
		}
	}
	
	return ptr;
}


// ---------------------------------------------------------------------------
//	¥ operator new[](size_t, nothrow_t&) throw()
// ---------------------------------------------------------------------------
//	Overriden to test for low-memory conditions.

void*
operator new[](
	PP_CSTD::size_t				size,
	const PP_STD::nothrow_t&	nt) throw ()
{
#pragma unused(nt)	// Keeps the compiler quiet

	StSpotlightDisable_();
	
	void* ptr = nil;
	if (AllowAllocation())
	{
		#if PP_Debug && PP_DebugNew_Support && (DEBUG_NEW >= DEBUG_NEW_BASIC)
			ptr = DebugNewDoAllocate(size, 0, 0, malloc, true);
		#else
			ptr = malloc(size);
		#endif
	}
	
	return ptr;
}


#pragma mark === leaks new ===

#if PP_Debug && PP_DebugNew_Support && (DEBUG_NEW == DEBUG_NEW_LEAKS)

// Prototypes
void*	operator new(PP_CSTD::size_t size, const char* file, int line) throw(PP_STD::bad_alloc);
void*	operator new(PP_CSTD::size_t size, const PP_STD::nothrow_t& nt, const char* file, int line) throw();
void*	operator new[](PP_CSTD::size_t size, const char* file, int line) throw(PP_STD::bad_alloc);
void*	operator new[](PP_CSTD::size_t size, const PP_STD::nothrow_t& nt, const char* file, int line) throw();


// ---------------------------------------------------------------------------
//	¥ operator new(size_t, char*, int) throw(bad_alloc)
// ---------------------------------------------------------------------------
//	Only used when DebugNew is active. Same as new(size_t) but records
//	the location of the allocation.

void*
operator new(
	PP_CSTD::size_t		size,
	const char*			file,
	int					line) throw (PP_STD::bad_alloc)
{
#pragma unused (file, line)	// Quiet down the non-debug builds.

	StSpotlightDisable_();
	
	void* ptr = nil;
	if (AllowAllocation())
	{
		for(;;)
		{
			#if PP_Debug && PP_DebugNew_Support && (DEBUG_NEW >= DEBUG_NEW_BASIC)
				ptr = DebugNewDoAllocate(size, file, line, malloc, false);
			#else
				ptr = malloc(size);
			#endif
			
			if (ptr != NULL)
				break;
			else
				PP_NewHandler();
		}
	}
	
	return ptr;
}


// ---------------------------------------------------------------------------
//	¥ operator new(size_t, char*, int) throw()
// ---------------------------------------------------------------------------
//	Only used when DebugNew is active. Same as new(size_t) nothrow but records
//	the location of the allocation. You'll need to use the DebugNew NEW_NOTHROW
//	macro to invoke this method (instead of the usual NEW). See DebugNew.h
//	for more information.

void*
operator new(
	PP_CSTD::size_t				size,
	const PP_STD::nothrow_t&	nt,
	const char*					file,
	int							line) throw ()
{
#pragma unused (nt, file, line)	// Quiet down the non-debug builds.

	StSpotlightDisable_();
	
	void* ptr = nil;
	if (AllowAllocation())
	{
		for(;;)
		{
			#if PP_Debug && PP_DebugNew_Support && (DEBUG_NEW >= DEBUG_NEW_BASIC)
				ptr = DebugNewDoAllocate(size, file, line, malloc, false);
			#else
				ptr = malloc(size);
			#endif
			
			if (ptr != NULL)
				break;
			else
				PP_NewHandler();
		}
	}
	
	return ptr;
}


// ---------------------------------------------------------------------------
//	¥ operator new[](size_t, char*, int) throw(bad_alloc)
// ---------------------------------------------------------------------------
//	Only used when DebugNew is active. Same as new[](size_t) but records
//	the location of the allocation.

void*
operator new[](
	PP_CSTD::size_t	size,
	const char*		file,
	int				line) throw (PP_STD::bad_alloc)
{
#pragma unused (file, line)	// Quiet down the non-debug builds.

	StSpotlightDisable_();
	
	void* ptr = nil;
	if (AllowAllocation())
	{
		for(;;)
		{
			#if PP_Debug && PP_DebugNew_Support && (DEBUG_NEW >= DEBUG_NEW_BASIC)
				ptr = DebugNewDoAllocate(size, file, line, malloc, true);
			#else
				ptr = malloc(size);
			#endif
			
			if (ptr != NULL)
				break;
			else
				PP_NewHandler();
		}
	}
	
	return ptr;
}


// ---------------------------------------------------------------------------
//	¥ operator new[](size_t, char*, int) throw()
// ---------------------------------------------------------------------------
//	Only used when DebugNew is active. Same as new[](size_t) nothrow but records
//	the location of the allocation. You'll need to use the DebugNew NEW_NOTHROW
//	macro to invoke this method (instead of the usual NEW). See DebugNew.h
//	for more information.

void*
operator new[](
	PP_CSTD::size_t				size,
	const PP_STD::nothrow_t&	nt,
	const char*					file,
	int							line) throw ()
{
#pragma unused (nt, file, line)	// Quiet down the non-debug builds.

	StSpotlightDisable_();
	
	void* ptr = nil;
	if (AllowAllocation())
	{
		for(;;)
		{
			#if PP_Debug && PP_DebugNew_Support && (DEBUG_NEW >= DEBUG_NEW_BASIC)
				ptr = DebugNewDoAllocate(size, file, line, malloc, true);
			#else
				ptr = malloc(size);
			#endif
			
			if (ptr != NULL)
				break;
			else
				PP_NewHandler();
		}
	}
	
	return ptr;
}


#endif // PP_Debug && PP_DebugNew_Support && (DEBUG_NEW == DEBUG_NEW_LEAKS)


#pragma mark -
#pragma mark === operator delete ===

// ---------------------------------------------------------------------------
//	¥ operator delete
// ---------------------------------------------------------------------------
//	Overridden to disable Spotlight memory checks.

void
operator delete(
	void*	ptr) throw()
{
#if PP_Debug && PP_DebugNew_Support && (DEBUG_NEW >= DEBUG_NEW_BASIC)

	StSpotlightDisable_();
	DebugNewDoFree(ptr, free, false);

#else

	free(ptr);

#endif
}


// ---------------------------------------------------------------------------
//	¥ operator delete[]
// ---------------------------------------------------------------------------
//	Overridden to disable Spotlight memory checks.

void
operator delete[](
	void*	ptr) throw()
{
#if PP_Debug && PP_DebugNew_Support && (DEBUG_NEW >= DEBUG_NEW_BASIC)

	StSpotlightDisable_();
	DebugNewDoFree(ptr, free, true);

#else

	free(ptr);

#endif
}


// ---------------------------------------------------------------------------
//	¥ operator delete
// ---------------------------------------------------------------------------
//	Overridden to disable Spotlight memory checks.

void
operator delete(
	void*	ptr, const char*, int) throw()
{
#if PP_Debug && PP_DebugNew_Support && (DEBUG_NEW >= DEBUG_NEW_BASIC)

	StSpotlightDisable_();
	DebugNewDoFree(ptr, PP_CSTD::free, false);

#else

	::free(ptr);

#endif
}


// ---------------------------------------------------------------------------
//	¥ operator delete[]
// ---------------------------------------------------------------------------
//	Overridden to disable Spotlight memory checks.

void
operator delete[](
	void*	ptr, const char*, int) throw()
{
#if PP_Debug && PP_DebugNew_Support && (DEBUG_NEW >= DEBUG_NEW_BASIC)

	StSpotlightDisable_();
	DebugNewDoFree(ptr, PP_CSTD::free, true);

#else

	::free(ptr);

#endif
}


#pragma mark -
#pragma mark === new_handler ===

// ---------------------------------------------------------------------------
//	¥ PP_NewHandler()
// ---------------------------------------------------------------------------
//	This is a new_handler function that attempts to free up memory
//	when new fails to find enough memory. See section 18.4.2.2 of the C++
//	standard, or section 14.4.5 in Stroustrup's 3rd edition.
//
//	It is not installed automatically. You must manually install it via
//	set_new_handler().

#include <LGrowZone.h>

void PP_NewHandler() throw(PP_STD::bad_alloc)
{
		// Ask the GrowZone to free up memory. Since at this point we
		// have no idea how much memory we need, consider any freed
		// memory as a positive result and return so new can try again.
		// If it failed to free any memory or there is no GrowZone at
		// all, throw bad_alloc to signal the failure.
		//
		// This does assume that once DoGrowZone() has been called and
		// if it returns a positive value, that subsequent calls to
		// DoGrowZone() should fail to return a positive value (at least
		// until after this allocation attempt has completed). Not only
		// does this assumption affect this function, but the method
		// used by new to allocate memory (malloc, my_alloc (New.cp),
		// NewPtr, etc.) could potentially call into the system (NewPtr)
		// and cause the GrowZone to again trigger. However the structuring
		// of the implementation of new (at least in the Metrowerks runtimes,
		// see New.cp) should utimately still end up in failure and none
		// of this should truly be an issue.
		//
		// Besides, if you hit a situation like that, you're probably in
		// deep kimchee anyways. ;-)
		
	LGrowZone*	theGZ = LGrowZone::GetGrowZone();
	if (theGZ != nil) {
		if (theGZ->DoGrowZone(max_Int32) > 0) {
			return;
		}
	}
	
	throw PP_STD::bad_alloc();
}

// ===========================================================================

#pragma mark -
#pragma mark ** DebugNew helpers

// ---------------------------------------------------------------------------
//		* BetterDebugNewErrorHandler
// ---------------------------------------------------------------------------
//	A replacement for DebugNew's error handler. Makes it easier to
//	identify the cause of the error in the MW debugger.

#if Constructor_Debug

	void
	BetterDebugNewErrorHandler(short err)
	{
		switch (err) {
			case dbgnewNullPtr:
				SignalCStr_("DebugNew: null pointer");
				break;
				
			case dbgnewTooManyFrees:
				SignalCStr_("DebugNew: more deletes than news");
				break;
				
			case dbgnewPointerOutsideHeap:
				SignalCStr_("DebugNew: delete or validate called for pointer outside application heap");
				break;
				
			case dbgnewFreeBlock:
				SignalCStr_("DebugNew: delete or validate called for free block");
				break;
				
			case dbgnewBadHeader:
				SignalCStr_("DebugNew: unknown block, or block header was overwritten");
				break;
				
			case dbgnewBadTrailer:
				SignalCStr_("DebugNew: block trailer was overwritten");
				break;
				
			case dbgnewBlockNotInList:
				SignalCStr_("DebugNew: block valid but not in block list (internal error)");
				break;
				
			case dbgnewFreeBlockOverwritten:
				SignalCStr_("DebugNew: free block overwritten, could be dangling pointer");
				break;
										
			default:
				SignalCStr_("DebugNew: undefined error");
				break;
		}
	}

#endif