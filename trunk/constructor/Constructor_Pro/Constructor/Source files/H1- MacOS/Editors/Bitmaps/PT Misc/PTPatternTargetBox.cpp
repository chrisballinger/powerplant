// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*
	ebs		96.11.26	Changed DrawBuffer() since GetBorderRect() no longer exists
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "SUOffscreen.h"
#include "SURegionUtils.h"
#include "SUSaveGWorld.h"
#include "PTPatternTargetBox.h"

/*=============================
	CreateFromStream
==============================*/
PTPatternTargetBox *PTPatternTargetBox::CreateFromStream( LStream *inStream )
{
	return new PTPatternTargetBox( inStream );
}

/*=============================
	Constructor (LStream *)
==============================*/
PTPatternTargetBox::PTPatternTargetBox( LStream *inStream ) :
	PTDraggableTargetBox( inStream )
{
}

/*=============================
	Destructor
==============================*/
PTPatternTargetBox::~PTPatternTargetBox()
{
}

/*=============================
	DrawSelf
==============================*/
void PTPatternTargetBox::DrawBuffer()
{
	StSaveGWorld		aSaver;
	StColorPenState		aPenState;
	aPenState.Normalize();
	
	if ( !mBuffer ) return;
	Rect  sourceR, portR, destR, borderR, localR;
	
	GrafPtr	destPort = this->GetMacPort();
	if ( !destPort ) return;
	
	//borderR = this->GetBorderRect();
	this->CalcBorderRect( borderR );
	
	SInt32	destWidth = borderR.right - borderR.left - 12;
	SInt32	destHeight = borderR.bottom - borderR.top - 12;
	
	SInt32	imageWidth = mBuffer->GetWidth();
	SInt32	imageHeight = mBuffer->GetHeight();
	if ( (imageWidth == 0) || (imageHeight == 0) ) return;
	::SetRect( &sourceR, 0, 0, imageWidth, imageHeight );
	
	this->GetCenteredBufferArea( destWidth, destHeight, &localR, &portR );

	if ( !this->IsUsed() )
	{
		this->FocusDraw();
		aPenState.Normalize();
		Pattern thePattern;
		UQDGlobals::GetLightGrayPat(&thePattern);
		::PenPat( &thePattern );
		::PaintRect( &localR );
		return;
	}
	
		// find out how many rows & columns to draw to fill in the space
		// for the pattern. round upward to the next whole number and clip
		// to prevent overdrawing the area we're drawing to
	SInt32 numRows = destHeight / imageHeight;
	if ( destHeight % imageHeight ) ++numRows;
	
	SInt32 numCols = destWidth / imageWidth;
	if ( destWidth % imageWidth ) ++numCols;
	
	/*
		the CopyTo() method below clears the clip region, so the only
		way to clip is to pass a mask region to CopyTo
	*/
	RgnHandle	maskRgn = nil;
	
	try
	{	
		maskRgn = SURegionUtils::NewRegionFromRect( portR );
		for ( SInt32 rowCount = 0; rowCount < numRows; rowCount++ )
		{
			destR.left = portR.left;
			destR.right = destR.left + imageWidth;
			destR.top = portR.top + imageHeight * rowCount;
			destR.bottom = destR.top + imageHeight;
		
			for ( SInt32 colCount = 0; colCount < numCols; colCount++ )
			{
				mBuffer->CopyTo( destPort, &destR, nil, srcCopy, maskRgn );
				::OffsetRect( &destR, imageWidth, 0 );
			}
		}
	}
	catch( ... )
	{
		// don't throw errors because it would cause an infinite recursion
	}
	
	SURegionUtils::DisposeRegion( maskRgn );
}
