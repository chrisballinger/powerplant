// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*
	this is similar to UMarchingAnts in PowerPlant, but is a little
	more flexible (at the expense of some ease of use).
	
	this is better suited to selection rectangles and areas because
	you can draw & erase in xor mode since this code doesn't automatically
	change the pattern.
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "SUMarchingAnts.h"

/*===========================================
	constants
============================================*/
const SInt16	kDefaultMarchInterval = 3;

/*===========================================
	statics
============================================*/
Pattern	SUMarchingAnts::sAntPattern = {  0xF8, 0xF1, 0xE3, 0xC7,
								  	   	 0x8F, 0x1F, 0x3E, 0x7C };
								  	   	 
UInt32	SUMarchingAnts::sNextMarchTime = 0;

/*===========================================
	Constructor
============================================*/
SUMarchingAnts::SUMarchingAnts()
{
	mDelay = kDefaultMarchInterval;
}

/*===========================================
	GetDelay
============================================*/
UInt32 SUMarchingAnts::GetDelay()
{
	return( mDelay );
}

/*===========================================
	SetDelay
============================================*/
void SUMarchingAnts::SetDelay( UInt32 inDelay )
{
	mDelay = inDelay;		// will take effect next time we draw
}


/*===========================================
	TimeToDraw
============================================*/
Boolean SUMarchingAnts::TimeToDraw()
{
	return( (UInt32) TickCount() > sNextMarchTime );
}

/*===========================================
	GetPattern
============================================*/
void SUMarchingAnts::GetPattern( Pattern *outPattern )
{
	*outPattern = sAntPattern;
}

/*===========================================
	IncrementPattern
============================================*/
void SUMarchingAnts::IncrementPattern()
{
	unsigned char	lastRow = sAntPattern.pat[7];
	for (SInt16 i = 7; i > 0; i--) 
		sAntPattern.pat[i] = sAntPattern.pat[i - 1];
	sAntPattern.pat[0] = lastRow;

	sNextMarchTime = (UInt32) TickCount() + mDelay;
}

