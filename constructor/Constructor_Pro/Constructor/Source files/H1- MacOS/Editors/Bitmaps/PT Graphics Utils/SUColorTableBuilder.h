// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
#pragma once

#include "SU_Types.h"

class SUOffscreen;
class SUColorHash;

class SUColorTableBuilder
{
	public:
		/*********************************************
			this is the only routine many will need
		*********************************************/
		static SUOffscreen *	DownSampleBuffer( SUOffscreen *inSource, SInt32 inMaxDepth,
													Boolean inIncludeBW, Boolean inMinimizeTable, 
													Boolean biggerInverseTable );
													
		/*********************************************
			constructor & destructor
		*********************************************/
								SUColorTableBuilder( Boolean inIncludeBW = false );
		virtual					~SUColorTableBuilder();
		
		/*********************************************
			changing the settings
		*********************************************/
		virtual void			SetBuffer( SUOffscreen * );
		virtual void			SetIncludeBW( Boolean );
		virtual void			SetMinimizeTable( Boolean );
		virtual void			SetPictUtilMethod( short inMethod );
		virtual void			SetLargerInverseTable( Boolean );
	
		/*********************************************
			retrieving information
		*********************************************/
		virtual SInt32			GetColorCount();
		virtual CTabHandle		GetColorTable( SInt32 inMaxDepth, SInt32 *outDepth );
		virtual SUOffscreen *	CreateDownSampledBuffer( SInt32 inMaxDepth );
	
															
	protected:
		SUOffscreen	*			mBuffer;
		SUColorHash *			mColorHash;
		
		Boolean					mIncludeBW;
		Boolean					mMinimizeTable;
		Boolean					mLargerInverseTable;
		short					mPictUtilMethod;
		
		Boolean					mScanIsDone;
		Boolean					mUseCopyBits;
		
		virtual void			DoScan();
};


