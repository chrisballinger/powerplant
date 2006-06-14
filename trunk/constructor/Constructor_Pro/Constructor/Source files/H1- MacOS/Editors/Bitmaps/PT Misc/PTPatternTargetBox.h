// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
#pragma once

#include "PTDraggableTargetBox.h"

class PTPatternTargetBox : public PTDraggableTargetBox
{
	public:
		enum { class_ID = 'PatT' };
		
										PTPatternTargetBox( LStream * );
		virtual							~PTPatternTargetBox();
		static PTPatternTargetBox *		CreateFromStream( LStream * );
	
	protected:
		virtual void					DrawBuffer();
};


