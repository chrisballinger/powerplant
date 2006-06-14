// Copyright �2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	PP_MacHeaders.h				PowerPlant 2.2.2	�1996-2005 Metrowerks Inc.
// ===========================================================================

	// Use PowerPlant-specific Precompiled header

#if __MACH__
	#include <PP_MSLMacHeadersMach-O++>

#elif __POWERPC__
	#include <PP_MacHeadersPPC++>

#elif __CFM68K__
	#include <PP_MacHeadersCFM68K++>
	
#else
	#include <PP_MacHeaders68K++>
#endif

#include <PP_PCH_Options.h>		// Check for compatibility with pch options
