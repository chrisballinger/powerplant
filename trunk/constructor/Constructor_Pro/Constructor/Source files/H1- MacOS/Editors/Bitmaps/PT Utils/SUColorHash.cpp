// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
/*
	96.12.04	ebs		Fixed FindAssociatedValueIndex() -- check for empty slot first
						Fixed FixBlackAndWhite() for non-minimized ctables (not used or tested, though)
*/

	// Prefix file for MSVC build
#ifdef WINVER
  #include "ctor.h"
#endif

#include "SU_Constants.h"
#include "SUColorHash.h"
#include "SUColorUtils.h"

/*
	note: for speed, most of the methods are inline -- see SUColorHash.h
	
	white & black are put into the first & last elements in the table
	when a color table is built. it is optional whether to force them
	into the table even if they didn't exist previously.
*/

/*==========================================
	Constructor
===========================================*/
SUColorHash::SUColorHash( Boolean forceBW )
{
	for ( SInt32 count = 0; count < kHashArrayLimit; count++ )
		mHashArray[ count ].theColor = kEmptyColorSlot;

	mRecent1.theColor = kEmptyColorSlot;
	mRecent2.theColor = kEmptyColorSlot;
	mUniquePixels = 0;
	mBWCompleted = false;
	
	if ( forceBW )
	{
		this->AddColorToHash( kWhiteColor32 );
		this->AddColorToHash( kBlackColor32 );
	}
}

/*==========================================
	Destructor
===========================================*/
SUColorHash::~SUColorHash()
{
}


/*==========================================
	BuildColorTable
===========================================*/
CTabHandle SUColorHash::BuildColorTable( Boolean inMinimal )
{
	CTabHandle	theTable;
	
	this->FixBlackAndWhite( inMinimal );
	
	if ( inMinimal )
		theTable = SUColorUtils::NewColorTableByEntries( mUniquePixels );
	else
	{
			// this will round the color table up to the "normal"
			// number of colors rather than the minimal one
		SInt32	depth = SUColorUtils::NumColorsToDepth( mUniquePixels );
		theTable = SUColorUtils::NewColorTableByDepth( depth );
	}
	
	::HLock( (Handle) theTable );
	CTabPtr		p = *theTable;
	
	
	/*
		the associated value in mHashArray has the color index
	*/
	SInt32  tableCount = 0;

	for ( SInt32 hashCount = 0; hashCount < kHashArrayLimit; hashCount++ )
	{
		Color32		theColor = mHashArray[hashCount].theColor;
		
		if ( theColor != kEmptyColorSlot )
		{
			if ( tableCount++ >= mUniquePixels )
				break;

			SInt32		associatedValue = mHashArray[hashCount].associatedNumber;
			p->ctTable[ associatedValue ].rgb = SUColorUtils::Color32ToRGB( theColor );
		}
	}	
	
	::HUnlock( (Handle) theTable );
	return( theTable );
}

/*==========================================
	FindAssociatedValue
===========================================*/
SInt32 SUColorHash::FindAssociatedValueIndex( SInt32 inAssoc )
{
	for ( long count = 0; count < kHashArrayLimit; count++ )
		if ( (mHashArray[count].theColor != kEmptyColorSlot) && 
			 (mHashArray[count].associatedNumber == inAssoc) )
		{
			return( count );
		}
		
	return -1;
}


#ifdef PREV_VERSION_XXX
/*==========================================
	FindAssociatedValue
===========================================*/
Int32 SUColorHash::FindAssociatedValueIndex( Int32 inAssoc )
{
	for ( long count = 0; count < kHashArrayLimit; count++ )
		if ( mHashArray[count].associatedNumber == inAssoc )
			return( count );
		
	return -1;
}
#endif

/*==========================================
	FixBlackAndWhite
	
	Description:
	White needs slot #0 in the color table. Black needs the last slot. We'll
	find whatever colors are currently using those pixel values and swap
	the pixel values whatever black & white have been given.
===========================================*/
void SUColorHash::FixBlackAndWhite( Boolean inMinimalTable )
{
	if ( mBWCompleted ) return;			// already fixed it
	
	SInt32	depth = SUColorUtils::NumColorsToDepth( mUniquePixels );
	SInt32	maxColorsForDepth = SUColorUtils::DepthToNumColors( depth );
	SInt32	blackAssocValue = inMinimalTable ? (mUniquePixels - 1) : (maxColorsForDepth - 1);

	SInt32	whiteIndex, blackIndex, assocIndex;
	
	/*
		if white is in the table, put it at the beginning
	*/
	if ( this->GetIndex( kWhiteColor32, &whiteIndex ) )
	{
		assocIndex = this->FindAssociatedValueIndex( 0 );
		if ( (assocIndex != -1) && (assocIndex != whiteIndex) )
		{
				// swap the associated values
			mHashArray[ assocIndex ].associatedNumber = mHashArray[ whiteIndex ].associatedNumber;
			mHashArray[ whiteIndex ].associatedNumber = 0;
		}
	}
	
	/*
		if black is in the table, put it at the end
	*/
	if ( this->GetIndex( kBlackColor32, &blackIndex ) )
	{
		assocIndex = this->FindAssociatedValueIndex( blackAssocValue );
		if ( assocIndex == -1 )
		{
				// nobody is using the desired value for black -- this happens if we
				// don't minimize the table and don't completely fill it.
				// just give black the pixel value it wants
			mHashArray[ blackIndex ].associatedNumber = blackAssocValue;
		}
		else if ( assocIndex != blackIndex )
		{
				// swap the associated (pixel) values
			mHashArray[ assocIndex ].associatedNumber = mHashArray[ blackIndex ].associatedNumber;
			mHashArray[ blackIndex ].associatedNumber = blackAssocValue;
		}
	}
	
	/*
		clear the cache since we've changed some values
	*/
	mRecent1.theColor = kEmptyColorSlot;
	mRecent2.theColor = kEmptyColorSlot;

	mBWCompleted = true;
}

#ifdef OLDWAY_OUT
/*==========================================
	FixBlackAndWhite
	
	Description:
	Changes the values associated with black and white to the specified
		numbers.
		
	The reason we can't do this "as we go" is because we don't know what
	value is correct for black until the table is complete.
===========================================*/
void SUColorHash::FixBlackAndWhite( Boolean inMinimalTable )
{
	if ( mBWCompleted ) return;

	if ( !mForceBW ) return;
	
	/*
		white is already at the correct spot -- index value 0 -- since
		it was added to the table first.
		
		black was added next (index value 1), and we need to change its
		associated value from 1 to 255 (for an 8-bit table) or mUniquePixels - 1
		in the general sense.

		so find the color whose associated value is mUniquePixels - 1 and
		swap that pixel value with black.
		
	*/
	Int32	depth = SUColorUtils::NumColorsToDepth( mUniquePixels );
	Int32	maxColorsForDepth = SUColorUtils::DepthToNumColors( depth );
	
		// if the table isn't full, then the black value won't
		// have been used yet -- just change the associated 
		// value for black and return
	if ( !inMinimalTable && (mUniquePixels < maxColorsForDepth) )
	{
		mHashArray[1].associatedNumber = maxColorsForDepth - 1;
		mBWCompleted = true;
		return;
	}
	
		// if we're minimizing the table or the table is full
		// we need to scan it to find out who is borrowing
		// the associated value that black needs.
		// once we find it, swap it with black
	Int32 lookingFor = maxColorsForDepth - 1;
	for ( long count = 0; count < kHashArrayLimit; count++ )
		if ( mHashArray[count].associatedNumber == lookingFor )
		{
			mHashArray[count].associatedNumber = 1;			// move this to pixelValue 1
			mHashArray[1].associatedNumber = lookingFor;	// and black to the end
			mBWCompleted = true;
			return;		
		}
}
#endif
