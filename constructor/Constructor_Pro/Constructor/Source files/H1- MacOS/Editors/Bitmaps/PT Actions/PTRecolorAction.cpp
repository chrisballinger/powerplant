// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*
		96.08.28	ebs		Support for locked files

	note:
	QuickDraw does a really bad job of recoloring. We should probably write our
	own code instead. (maybe for 2.0???)
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "PT_Headers.h"
#include "PTRecolorAction.h"

const SInt32	kRecolorDepth = 8;

PTRecolorAction::PTRecolorAction( const SPaintAction &inAction, CTabHandle inTable )
				: PTAction( inAction, str_UndoRecolor )
{
	this->ThrowIfFileLocked();

	/*********************************************
		make sure we have enough RAM for this operation since
		the inverse table we build isn't small
	**********************************************/
	UInt32	total, contig;
	::PurgeSpace( (SInt32*) &total, (SInt32*) &contig );
	if ( (contig < 50000) || (total < 50000) )
		LException::Throw( memFullErr );

	mTable = this->CreateResizedTable( inTable );
}

PTRecolorAction::~PTRecolorAction()
{
	if ( mTable )
		::DisposeCTable( mTable );
}

void PTRecolorAction::DoIt()
{
	if ( mTable )
	{
		SUOffscreen		*currentBuffer = mSettings.currentBuffer;
		
		mSettings.thePaintView->CopyToUndo();
		mSettings.thePaintView->SelectNone();
	
		/*
			create a new buffer the same size as the image but with
			a depth of 8-bits. use the specified color table as the
			color table.
		*/
		SInt32	width = currentBuffer->GetWidth();
		SInt32	height = currentBuffer->GetHeight();
		
		SUOffscreen *newBuffer = SUOffscreen::CreateBuffer( width, height, kRecolorDepth, mTable );

		/*
			increase the inverse table size from 4 to 5 bits.
			this gives substantially better color matching.
		*/
		newBuffer->IncreaseInverseTableSize();
		
		/*
			Copy the image to the 8-bit buffer to downsample the colors 
			and then back to the original 32-bit one
		*/
		newBuffer->CopyFrom( currentBuffer );		
		currentBuffer->CopyFrom( newBuffer );
		delete newBuffer;
		
			// update the canvas
		mSettings.theCanvas->DrawFrom( currentBuffer );
	}

	this->PostAsAction();
}

/*
	creates a new ColorTable for kRecolorDepth and copies the rgb values from
	a provided source table.
	
	note:
	this is needed because inTable might have fewer entries than 256 and QD
	doesn't like this. is this really true here ???
*/
CTabHandle PTRecolorAction::CreateResizedTable( CTabHandle inTable )
{
	CTabHandle		destTable = SUColorUtils::NewColorTableByDepth( kRecolorDepth );
	
	StHandleLocker	lock1( (Handle) inTable );
	StHandleLocker	lock2( (Handle) destTable );
	
	CTabPtr			sourceP = *inTable;
	CTabPtr			destP = *destTable;
	
	SInt32			sourceEntries = sourceP->ctSize + 1;
	SInt32			destEntries = destP->ctSize + 1;
	
	RGBColor		fillRGB = sourceP->ctTable[ sourceP->ctSize ].rgb;
	
	/*
		put an rgbcolor into every slot in the table. if we're out of source
		rgb values, just duplicate the last value from the source over and over.
	*/
	for ( SInt32 count = 0; count < destEntries; count++ )
	{
		if ( count < sourceEntries )
			destP->ctTable[ count ].rgb = sourceP->ctTable[ count ].rgb;
		else
			destP->ctTable[ count ].rgb = fillRGB;
	}
	
	/*
		put white & black where they belong
	*/
	SUColorUtils::FixColorTableBW( destTable, false /* don't force them */ );
	
	return( destTable );
}
