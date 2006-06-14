// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// This is the prefix file for constructor

#ifndef _H_CPrefix
#define _H_CPrefix

#pragma warning( disable : 4237 )	// true/false usage
#pragma warning( disable : 4068 )   // Unknown #pragma
#pragma warning( disable : 4244 )   // Possible loss of precision
#pragma warning( disable : 4200 )   // Zero size array
#pragma	warning( disable : 4005 )   // Marco redefinition
#pragma	warning( disable : 4018 )   // signed/unsigned mismatch
#pragma	warning( disable : 4250 )   // inherit via dominance
#pragma warning( disable : 4146 )   // unary minus operator applied to unsigned type
#pragma warning( disable : 4309 )   // truncation of constant value
#pragma warning( disable : 4101 )   // unreference local varaible
#pragma warning( disable : 4761 )   // integral size mismatch in argument

#define __METROWERKS__
//#define Compile4DLL	1
//#define _PP_DLL

#define assignCString(dest,val) (strcpy(dest,val))

//#pragma message("far")
#include <asiport.h>
//#pragma message("point")
#include <strings.h>	// for c2pstr
//#pragma message("station")
#include <stringc.h>	// for strcpy
//#pragma message("was")
#include <PP_Types.h>
//#pragma message("lame")
#ifdef C4Mac
 #include "Constructor-Mac-pc.h"
#else
#ifdef C4Win
// #include "Constructor-Win-pc.h"
#else
#ifdef C4Java
 #include "Constructor-Java-pc.h"
#endif
#endif
#endif
//#pragma message("ya?")

inline void SwapShort( short& num ) { num = (num << 8)| ((num >> 8)&0x00ff); };
inline short SwappedShort( short num ) { SwapShort(num); return num; };
inline void SwapLong( long& num ) { num = ((SwappedShort(LoWord(num))) << 16) | (SwappedShort(HiWord(num))); };
inline long SwappedLong( long num ) { SwapLong(num); return num; };

#ifdef WINVER
#define WINSWAPS(x) SwapShort(x)
#define WINSWAPL(x) SwapLong(x)
#define WINSWAPUS(x) (x=SwappedShort((short)x))
#define WINSWAPUL(x) (x=SwappedLong((long)x))
#endif

#endif		// _H_CPrefix


