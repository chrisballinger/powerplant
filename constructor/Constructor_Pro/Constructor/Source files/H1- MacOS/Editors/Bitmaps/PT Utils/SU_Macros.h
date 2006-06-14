// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
#pragma once

/*=======================================
	Helpful macros
========================================*/
#ifndef MIN
	#define MIN(a,b) ( (a)<(b) ? a : b )
#endif

#ifndef MAX
	#define MAX(a,b) ( (a)<(b) ? b : a )
#endif

#ifndef ABS
	#define ABS(x)   ( (x) < 0 ?  -(x) : x )
#endif

