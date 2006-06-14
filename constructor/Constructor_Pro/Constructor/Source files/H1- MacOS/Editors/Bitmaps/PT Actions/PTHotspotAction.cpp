// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*
		96.08.28	ebs		Support for locked files
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "PT_Headers.h"
#include "PTActions.h"

PTHotSpotAction::PTHotSpotAction( const SPaintAction &inSettings )
	: PTTrackingPaintAction( inSettings, str_UndoHotSpot )
{
	this->ThrowIfFileLocked();
}

PTHotSpotAction::~PTHotSpotAction()
{
}

void PTHotSpotAction::MouseStillDown( const SMouseDownEvent &, 
										Point, Point,
										SInt32, SInt32,
										SInt32 newCol, SInt32 newRow )
{
	PTCanvas *		theCanvas = mSettings.theCanvas;
	SUOffscreen *	theBuffer = mSettings.currentBuffer;

	Point	oldSpot = theCanvas->GetHotSpot();
	
	if ( ((oldSpot.h != newCol) || (oldSpot.v != newRow)) &&
			(newCol >= 0) && (newCol < theBuffer->GetWidth()) &&
		 	(newRow >= 0) && (newRow < theBuffer->GetHeight()) )
	{
		Point	newSpot = { newRow, newCol };
		theCanvas->SetHotSpot( newSpot );
		
			// canvas doesn't redraw automatically, so do so here
		theCanvas->DrawOnePixel( oldSpot.h, oldSpot.v, theBuffer );
		theCanvas->DrawOnePixel( newSpot.h, newSpot.v, theBuffer );
	}
}
