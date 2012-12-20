#ifndef EVENT_H
#define EVENT_H

class	Event{
	public:
		enum {
			NONE 						= -1, ///<do nothing
			UPDATE						= -2, ///<update inherent time of particle mI
			COLLISION_WITH_LEFT_WALL 	= -3, ///<collision of particle mI with left wall
			COLLISION_WITH_RIGHT_WALL 	= -4, ///<collision of particle mI with right wall
			COLLISION_WITH_BOTTOM_WALL 	= -5, ///<collision of particle mI with bottom wall
			COLLISION_WITH_TOP_WALL 	= -6, ///<collision of particle mI with top wall
			COLLISION_WITH_LEFT_CELLWALL 	= -7, ///<collision of particle mI with left cell wall
			COLLISION_WITH_RIGHT_CELLWALL 	= -8, ///<collision of particle mI with right cell wall
			COLLISION_WITH_BOTTOM_CELLWALL 	= -9, ///<collision of particle mI with bottom cell wall
			COLLISION_WITH_TOP_CELLWALL 	= -10  ///<collision of particle mI with top cell wall
		};
		double	mTime;
		///connection to parent event
		Event*	mParent;
		///affected particle id
		int		mI;
		///event code or affected second particle
		int		mJ;
		///connection to next event which relates to particle mI
		Event*	mNextI;
		///connection to previous event which relates to particle mI
		Event*	mPrevI;
		///connection to next event which relates to particle mJ
		Event*	mNextJ;
		///connection to previous event which relates to particle mJ
		Event*	mPrevJ;
		///connection to an event which will occure afterwards
		Event*	mRight;
		///connection to an event which will occure before this one
		Event*	mLeft;

		///creates a new event
		Event(const double time, const int i, const int j);
		///inserts an event into the structure
		Event*	InsertEvent(Event* event);
		///prints the current event for debugging purpose
		void	Print();
};

#endif
