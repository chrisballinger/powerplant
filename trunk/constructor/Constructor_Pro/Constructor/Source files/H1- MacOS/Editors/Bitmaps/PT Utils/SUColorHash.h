// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.

#include "SU_Types.h"
#include "SU_Macros.h"

/*==========================================
	SUColorHash
	
	Note:
	Most of these routines are inline because they are called for
	every pixel in large buffers. This is one area where the overhead
	of a virtual method might really be noticeable.
===========================================*/

enum {
	err_HashFull		= 'eHsF'
};

class SUColorHash
{
	public:
								SUColorHash( Boolean forceBW = false );
		virtual					~SUColorHash();
	
		inline		void		AddColorToHash( Color32 );				
		inline		SInt32		LookupAssociatedNumber( Color32 );
		inline 		SInt32		GetColorCount();
		
					CTabHandle	BuildColorTable( Boolean inMinimizeTable = true );
					void		FixBlackAndWhite( Boolean inMinimizeTable = true );
				
	protected:

		#define kHashArrayLimit		1024
		#define kEmptyColorSlot		0xFFFFFFFF		// bad because hi byte is set
		#define kInvalidAssociate	-1
		
		typedef struct
		{
			Color32		theColor;
			SInt32		associatedNumber;
		} HashEntry;
		
		
		HashEntry				mHashArray[ kHashArrayLimit ];
		HashEntry				mRecent1, mRecent2;
		SInt32					mUniquePixels;
		Boolean					mBWCompleted;
	
		inline 		SInt32		GetHashStartingPoint( Color32 );
		inline 		Boolean		GetIndex( Color32, SInt32 *outIndex );
		
		inline 		SInt32		IsRecentOne( Color32 );
		inline 		void		AddToRecentOne( Color32, SInt32 inAssociate );
					SInt32 		FindAssociatedValueIndex( SInt32 inAssoc );
};


/*==========================================
	AddColorToHash
===========================================*/
inline void SUColorHash::AddColorToHash( Color32 inColor )
{
	inColor &= 0x00FFFFFF;			// clear hi byte because kEmptyColorSlot might conflict
	
		// check the recent ones for speed first
	if ( IsRecentOne( inColor ) != kInvalidAssociate )
		return;
	
		// find the slot number
	SInt32		index;
	
	if ( GetIndex( inColor, &index ) )
	{
			// already in table, but keep as a recent one for speed
		this->AddToRecentOne( inColor, mHashArray[index].associatedNumber );
	}
	else
	{
			// not in table yet -- put it there
		mHashArray[ index ].theColor = inColor;
		mHashArray[ index ].associatedNumber = mUniquePixels;

			// keep track of recent ones
		this->AddToRecentOne( inColor, mUniquePixels++ );
		
			// and we're "dirty" now (B&W in wrong place)
		mBWCompleted = false;
	}
}

/*==========================================
	IsRecentOne
	
	This is used both for lookups and adding elements to the array.
===========================================*/
inline SInt32 SUColorHash::IsRecentOne( Color32 inColor )
{
	if ( mRecent1.theColor == inColor )
		return( mRecent1.associatedNumber );
	
	if ( mRecent2.theColor == inColor )
		return( mRecent2.associatedNumber );
		
	return( kInvalidAssociate );
}

/*==========================================
	AddToRecentOne
===========================================*/
inline void SUColorHash::AddToRecentOne( Color32 inColor, SInt32 inAssociate )
{
	mRecent2 = mRecent1;
	mRecent1.theColor = inColor;
	mRecent1.associatedNumber = inAssociate;
}

/*==========================================
	LookupAssociatedNumber
===========================================*/
inline SInt32 SUColorHash::LookupAssociatedNumber( Color32 inColor )
{
	SInt32	associatedNumber;
	SInt32	theIndex;
	
	inColor &= 0x00FFFFFF;
	
	associatedNumber = this->IsRecentOne( inColor );
	if ( associatedNumber != kInvalidAssociate ) return( associatedNumber );
	
	if ( this->GetIndex( inColor, &theIndex ) )
	{
		associatedNumber = mHashArray[ theIndex ].associatedNumber;

			// keep track of recent ones for speed 
		this->AddToRecentOne( inColor, associatedNumber );
		
			// return the requested number
		return( associatedNumber );
	}
	
	return( kInvalidAssociate );
}

/*==========================================
	GetHashStartingPoint
===========================================*/
inline SInt32 SUColorHash::GetHashStartingPoint( Color32 inColor )
{
	/****************************************
		Start searching the color list based on a hash formula.
		(the low 3 bits will tend to change rapidly, so use the
		low 3 bits of red & green + the low 2 bits of blue as an
		8-bit index). 
	****************************************/
	SInt32 hashStartingPoint = 
						( ((inColor & 0x000F0000) >> 10)	|
						  ((inColor & 0x00000700) >> 5) 	|
						  (inColor & 0x00000007) );
						  
	return( MIN( hashStartingPoint, kHashArrayLimit-1 ) );
}

/*==========================================
	GetIndex
===========================================*/
inline Boolean SUColorHash::GetIndex( Color32 inColor, SInt32 *outIndex )
{
	SInt32	hashStartingPoint = this->GetHashStartingPoint( inColor );
	SInt32	hashCount = hashStartingPoint;

	do
	{
		if ( hashCount >= kHashArrayLimit ) 
			hashCount = 0;					// wrap around to start of list
	
			// if we find an empty slot...
		if ( mHashArray[ hashCount ].theColor == kEmptyColorSlot )
		{
			*outIndex = hashCount;
			return( false );
		}
		
			// if it's already in the list...
		if ( mHashArray[ hashCount ].theColor == inColor )
		{
			*outIndex = hashCount;
			return( true );
		}

		++hashCount;
	} while( hashCount != hashStartingPoint );	// stop if we've looped all the way
	
		// table is full -- this shouldn't happen

	LException::Throw(err_HashFull);
	
	return( false );				// keep compiler happy
}

/*==========================================
	GetColorCount
===========================================*/
inline SInt32 SUColorHash::GetColorCount()
{
	return mUniquePixels;
}

