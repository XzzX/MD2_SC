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
			COLLISION_WITH_TOP_WALL 	= -6  ///<collision of particle mI with top wall
		};
		double	mTime;
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

		Event(const double time, const int i, const int j);
};

#endif
