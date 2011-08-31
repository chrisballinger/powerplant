// Copyright �2005, 2006 Freescale Semiconductor, Inc.
// Please see the License for the specific language governing rights and
// limitations under the License.
// ===========================================================================
//	LListener.cp				PowerPlant 2.2.2	�1993-2005 Metrowerks Inc.
// ===========================================================================
//
//	An abstract mix-in class that works with LBroadcaster class to implement
//	dependencies. A Listener receives messages from its Broadcasters.
//
//	�� Public Interface
//	� Construction:
//		LListener();
//			Default constructor. Listener has no Broadcasters
//
//	� Linking to Broadcasters:
//		Link a Listener to a Broadcaster by sending an AddListener message
//		to a Broadcaster. [see LBroadcaster]
//
//	� Responding to Messages:
//		void	ListenToMessage(MessageT inMessage, void *ioParam);
//			Derived classes *must* override this function (it is a
//			pure virtual function in LListener).
//			LLBroadcaster::BroadcastMessage calls this function.
//
//		Example Implementation:
//
//		{
//			switch (inMessage) {
//
//				case msg_NameChanged:	// ioParam is a StringPtr
//					DoNameChanged((StringPtr) ioParam);
//					break;
//
//				case msg_NewValue:		// ioParam is a long*
//					DoNewValue(*(long *) ioParam);
//					break;
//			}
//		}
//
//		The programmer can define message constants and associated
//		meanings for the ioParam parameter.
//
//		A Broadcaster always sends an msg_BroadcasterDied message, with
//		a pointer to itself as the parameter, before it is deleted.
//
//	� Listening State:
//		void	StopListening();
//		void	StartListening();
//		Boolean	IsListening();
//			Turn off/on, Inspect listening state. A Listener that is
//			not listening does not receive messages from its Broadcasters.
//			Use Stop/Start Listening to temporarily alter dependencies.

#ifdef PowerPlant_PCH
#include PowerPlant_PCH
#endif

#include <LListener.h>
#include <LBroadcaster.h>
#if !PP_Uses_STL_Containers
#include <TArrayIterator.h>
#endif

PP_Begin_Namespace_PowerPlant


// ---------------------------------------------------------------------------
//	� LListener								Default Constructor		  [public]
// ---------------------------------------------------------------------------

LListener::LListener()
{
	mIsListening = true;
}


// ---------------------------------------------------------------------------
//	� LListener								Copy Constructor		  [public]
// ---------------------------------------------------------------------------
//	Makes a shallow copy; Broadcaster links are not copied.

LListener::LListener(
	const LListener&	inOriginal)
{
	mIsListening = inOriginal.mIsListening;
}


// ---------------------------------------------------------------------------
//	� ~LListener							Destructor				  [public]
// ---------------------------------------------------------------------------

LListener::~LListener()
{
#if PP_Uses_STL_Containers
	std::list<LBroadcaster*>::iterator	iter = mBroadcasters.begin();
	while (iter != mBroadcasters.end()) {
		LBroadcaster *	theBroadcaster = (*iter);
		++iter;
		theBroadcaster->RemoveListener(this);
	}
#else
	TArrayIterator<LBroadcaster*> iterator(mBroadcasters);
	LBroadcaster*	theBroadcaster;
	while (iterator.Next(theBroadcaster)) {
		theBroadcaster->RemoveListener(this);
	}
#endif
}


// ---------------------------------------------------------------------------
//	� HasBroadcaster												  [public]
// ---------------------------------------------------------------------------
//	Return whether a Listener has the specified Broadcaster

bool
LListener::HasBroadcaster(
	LBroadcaster*	inBroadcaster)
{
#if PP_Uses_STL_Containers
	bool			found = false;
	std::list<LBroadcaster*>::iterator	iter = mBroadcasters.begin();
	while (iter != mBroadcasters.end()) {
		if ((*iter) == inBroadcaster) {
			found = true;
			break;
		} else {
			++iter;
		}
	}
	return found;
#else
	return (mBroadcasters.FetchIndexOf(inBroadcaster) != LArray::index_Bad);
#endif
}


// ---------------------------------------------------------------------------
//	� AddBroadcaster											   [protected]
// ---------------------------------------------------------------------------
//	Add a Broadcaster to a Listener
//
//	You should not call this function directly. LBroadcaster::AddListener
//	will call this function
//		Right:	theBroadcaster->AddListener(theListener);
//		Wrong:	theListener->AddBroadcaster(theBroadcaster);

void
LListener::AddBroadcaster(
	LBroadcaster*	inBroadcaster)
{
#if PP_Uses_STL_Containers
	mBroadcasters.push_back(inBroadcaster);
#else
	mBroadcasters.AddItem(inBroadcaster);
#endif
}


// ---------------------------------------------------------------------------
//	� RemoveBroadcaster											   [protected]
// ---------------------------------------------------------------------------
//	Remove a Broadcaster from a Listener
//
//	You should not call this function directly. LBroadcaster::RemoveListener
//	will call this function
//		Right:	theBroadcaster->RemoveListener(theListener);
//		Wrong:	theListener->RemoveBroadcaster(theBroadcaster);

void
LListener::RemoveBroadcaster(
	LBroadcaster*	inBroadcaster)
{
#if PP_Uses_STL_Containers
	mBroadcasters.remove(inBroadcaster);
#else
	mBroadcasters.Remove(inBroadcaster);
#endif
}


PP_End_Namespace_PowerPlant
