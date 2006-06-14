// Copyright ©2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
#pragma once

#include "PTActions.h"


class PTCutAction : public PTAction
{
	public:
						PTCutAction( const SPaintAction & );
		virtual			~PTCutAction();
		
		virtual void	DoIt();
};

class PTCopyAction : public PTAction
{
	public:
						PTCopyAction( const SPaintAction & );
		virtual			~PTCopyAction();

		virtual void	DoIt();
};

class PTPasteAction : public PTAction
{
	public:
						PTPasteAction( const SPaintAction & );
		virtual			~PTPasteAction();

		virtual void	DoIt();
};


class CClearAction : public PTAction
{
	public:
						CClearAction( const SPaintAction & );
		virtual			~CClearAction();

		virtual void	DoIt();
};

