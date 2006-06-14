// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "SURegionUtils.h"
#include "SUSaveGWorld.h"
#include "SUMiscUtils.h"
#include "SUSparePort.h"

RgnHandle SURegionUtils::NewRegionFromRect( const Rect &r )
{
	RgnHandle	result = SURegionUtils::NewRegion();
	
	::RectRgn( result, &r );
	return( result );
}

RgnHandle SURegionUtils::NewRegionFromRect( SInt32 left, SInt32 top, SInt32 right, SInt32 bottom )
{
	Rect	r;
	SetRect( &r, left, top, right, bottom );
	return( SURegionUtils::NewRegionFromRect( r ) );
}

RgnHandle SURegionUtils::NewRegionFromPolygon( PolyHandle inPoly )
{
	StSaveGWorld		aSaver;
	
	SUMiscUtils::SetPort( SUSparePort::GetSparePort() );
	RgnHandle	result = ::NewRgn();
	ThrowIfMemFail_( result );
	
	::OpenRgn();
	if ( inPoly && *inPoly )
		::FramePoly( inPoly );
	::CloseRgn( result );			// how do we check this for errors ???
	
	return( result );
}

RgnHandle SURegionUtils::NewRegionFromRegion( RgnHandle source )
{
	RgnHandle	result = SURegionUtils::NewRegion();

	CopyRgn( source, result );			// how do we check this for errors ???
	return( result );
}

RgnHandle SURegionUtils::NewRegion()
{
	RgnHandle	result = ::NewRgn();
	ThrowIfMemFail_( result );
	return( result );
}

void SURegionUtils::DisposeRegion( RgnHandle &inRegion )
{
	if ( inRegion )
	{
		::DisposeRgn( inRegion );
		inRegion = nil;
	}
}
