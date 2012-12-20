#include	"EventCalendar.h"

#include	<assert.h>

#include	"Event.h"

EventCalendar::EventCalendar(const unsigned int numberOfParticles) :
	mRootEvent(new Event(-123.123456789, Event::NONE, Event::NONE)),
	mEventPool(0){
	mEventsWithParticle.resize(numberOfParticles, 0);
}

EventCalendar::~EventCalendar(){
	while (mRootEvent->mLeft != 0) RemoveEvent(mRootEvent->mLeft);
	while (mRootEvent->mRight != 0) RemoveEvent(mRootEvent->mRight);
	delete mRootEvent;
	Event* dummy = mEventPool;
	while (dummy!=0){
		mEventPool = mEventPool->mNextI;
		delete dummy;
		dummy = mEventPool;
	}
}

Event*	EventCalendar::CreateEvent(const double t, const int a, const int b){
	if (mEventPool == 0) mEventPool = new Event(t, a, b);

	Event* nextPoolEvent = mEventPool->mNextI;

	mEventPool->mTime = t;
	mEventPool->mI = a;
	if (a>=0){
		if (mEventsWithParticle[a] == 0){
			mEventsWithParticle[a] = mEventPool;
			mEventPool->mNextI = mEventPool;
			mEventPool->mPrevI = mEventPool;
		}else{
			mEventPool->mNextI = mEventsWithParticle[a];
			if (mEventsWithParticle[a]->mI == a){
				mEventPool->mPrevI = mEventsWithParticle[a]->mPrevI;
				mEventsWithParticle[a]->mPrevI = mEventPool;
				if (mEventPool->mPrevI->mI == a){
					mEventPool->mPrevI->mNextI = mEventPool;
				}else{
					assert(mEventPool->mPrevI->mJ == a);
					mEventPool->mPrevI->mNextJ = mEventPool;
				}
			}else{
				assert(mEventsWithParticle[a]->mJ == a);
				mEventPool->mPrevI = mEventsWithParticle[a]->mPrevJ;
				mEventsWithParticle[a]->mPrevJ = mEventPool;
				if (mEventPool->mPrevI->mI == a){
					mEventPool->mPrevI->mNextI = mEventPool;
				}else{
					assert(mEventPool->mPrevI->mJ == a);
					mEventPool->mPrevI->mNextJ = mEventPool;
				}
			}
		}
	}
	mEventPool->mJ = b;
	if (b>=0){
		if (mEventsWithParticle[b] == 0){
			mEventsWithParticle[b] = mEventPool;
			mEventPool->mNextJ = mEventPool;
			mEventPool->mPrevJ = mEventPool;
		}else{
			mEventPool->mNextJ = mEventsWithParticle[b];
			if (mEventsWithParticle[b]->mI == b){
				mEventPool->mPrevJ = mEventsWithParticle[b]->mPrevI;
				mEventsWithParticle[b]->mPrevI = mEventPool;
				if (mEventPool->mPrevJ->mI == b){
					mEventPool->mPrevJ->mNextI = mEventPool;
				}else{
					assert(mEventPool->mPrevJ->mJ == b);
					mEventPool->mPrevJ->mNextJ = mEventPool;
				}
			}else{
				assert(mEventsWithParticle[b]->mJ == b);
				mEventPool->mPrevJ = mEventsWithParticle[b]->mPrevJ;
				mEventsWithParticle[b]->mPrevJ = mEventPool;
				if (mEventPool->mPrevJ->mI == b){
					mEventPool->mPrevJ->mNextI = mEventPool;
				}else{
					assert(mEventPool->mPrevJ->mJ == b);
					mEventPool->mPrevJ->mNextJ = mEventPool;
				}
			}
		}
	}
	mEventPool->mParent = 0;
	mEventPool->mLeft = 0;
	mEventPool->mRight = 0;
	Event*	dummy = mEventPool;
	///possibly mNextI == 0
	mEventPool = nextPoolEvent;
	return dummy;
}

/**
Removes an event from the tree and corrects all connections if it concerns a particle
@param event pointer to an event
**/
void	EventCalendar::RemoveEvent(Event* event){
	//new vertical connection
	if (event->mParent->mLeft == event){
		event->mParent->mLeft = event->mLeft;
		if (event->mLeft!=0)
			event->mLeft->mParent = event->mParent;
		if (event->mRight!=0)
			mRootEvent->InsertEvent(event->mRight);
	} else {
		assert(event->mParent->mRight == event);
		event->mParent->mRight = event->mRight;
		if (event->mRight!=0)
			event->mRight->mParent = event->mParent;
		if (event->mLeft!=0)
			mRootEvent->InsertEvent(event->mLeft);
	}
	//new horizontal connection if event is a particle
	if (event->mI >= 0){
		if (event->mNextI == event) {
			mEventsWithParticle[event->mI] = 0;
			assert(event->mPrevI == event);
		}else{
			if (event->mNextI->mI == event->mI){
				event->mNextI->mPrevI = event->mPrevI;
			}else{
				assert(event->mNextI->mJ == event->mI);
				event->mNextI->mPrevJ = event->mPrevI;
			}
			if (event->mPrevI->mI == event->mI){
				event->mPrevI->mNextI = event->mNextI;
			}else{
				assert(event->mPrevI->mJ == event->mI);
				event->mPrevI->mNextJ = event->mNextI;
			}
			if (mEventsWithParticle[event->mI] = event) mEventsWithParticle[event->mI] = event->mNextI;
		}
	}

	if (event->mJ >= 0){
		if (event->mNextJ == event) {
			mEventsWithParticle[event->mJ] = 0;
			assert(event->mPrevJ == event);
		}else{
			if (event->mNextJ->mI == event->mJ){
				event->mNextJ->mPrevI = event->mPrevJ;
			}else{
				assert(event->mNextJ->mJ == event->mJ);
				event->mNextJ->mPrevJ = event->mPrevJ;
			}
			if (event->mPrevJ->mI == event->mJ){
				event->mPrevJ->mNextI = event->mNextJ;
			}else{
				assert(event->mPrevJ->mJ == event->mJ);
				event->mPrevJ->mNextJ = event->mNextJ;
			}
			if (mEventsWithParticle[event->mJ] = event) mEventsWithParticle[event->mJ] = event->mNextJ;
		}
	}
	//saving memory of event for later use
	event->mNextI = mEventPool;
	mEventPool = event;
}

/**
Removes all events which contain particle pid.
@param pid id of the particle
**/
void	EventCalendar::RemoveAllEventsWithParticle(const int pid){
	if (pid>=0)
		while (mEventsWithParticle[pid] != 0) RemoveEvent(mEventsWithParticle[pid]);
}

Event*	EventCalendar::NextEvent(){
	Event*	dummy = mRootEvent;
	if (dummy->mRight != 0)
		dummy = dummy->mRight;
	while (dummy->mLeft!=0) dummy = dummy->mLeft;

	return dummy;
}

/**
Takes an event from storage list (mEventPool) or creates a new one.
Sets the correct data and puts it in the tree.
@param t time of the event
@param a first parameter
@param b second parameter
@return returns the newly created event
**/
Event*	EventCalendar::InsertEvent(const double t, const int a, const int b){
	return mRootEvent->InsertEvent( CreateEvent(t, a, b) );
}

void	EventCalendar::Print(){
	mRootEvent->Print();
}