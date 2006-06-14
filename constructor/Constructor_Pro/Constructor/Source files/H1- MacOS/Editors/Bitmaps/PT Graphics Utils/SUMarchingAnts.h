// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
#pragma once

class SUMarchingAnts
{
	public:
							SUMarchingAnts();
		virtual				~SUMarchingAnts() { }
							
		virtual Boolean		TimeToDraw();
		virtual void		GetPattern( Pattern * );
		virtual void		IncrementPattern();
		
		virtual UInt32		GetDelay();
		virtual void		SetDelay( UInt32 inTicks );
		
	protected:
		UInt32				mDelay;
		static Pattern		sAntPattern;
		static UInt32		sNextMarchTime;
};

