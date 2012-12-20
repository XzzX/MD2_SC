#ifndef EVENTCALENDAR_H
#define EVENTCALENDAR_H

#include	<vector>

class	Event;

class	EventCalendar{
	private:
		///vector which stores an anchor for every pid 
		std::vector<Event*>	mEventsWithParticle;
		///base element of the tree
		Event*	mRootEvent;
		///pool for unused events that can be reused
		Event*	mEventPool;

		///returns a new event with given parameters
		Event*	CreateEvent(const double t, const int a, const int b);

	public:
		///inserts a new event into the tree
		Event*	InsertEvent(const double t, const int a, const int b);
		///removes a given element from the tree
		void	RemoveEvent(Event* event);
		///removes all events corresponding to a given pid
		void	RemoveAllEventsWithParticle(const int pid);
		///returns the next event
		Event*	NextEvent();
		///prints the tree to cout for debuggin purpose
		void	Print(); 

		///creates a tree for numberOfParticles different particles
		EventCalendar(const unsigned int numberOfParticles);
		///frees all storage of the tree
		~EventCalendar();
};


#endif
