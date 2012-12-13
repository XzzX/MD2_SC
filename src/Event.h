#ifndef EVENT_H
#define EVENT_H

class	Event{
	public:
		enum {
			NONE 						= -1,
			UPDATE						= -2,
			COLLISION_WITH_LEFT_WALL 	= -3,
			COLLISION_WITH_RIGHT_WALL 	= -4,
			COLLISION_WITH_BOTTOM_WALL 	= -5,
			COLLISION_WITH_TOP_WALL 	= -6
		};

		double	mTime;
		int		mI;
		int		mJ;
		Event*	mNextI;
		Event*	mPrevI;
		Event*	mNextJ;
		Event*	mPrevJ;

		Event(const double time, const int i, const int j);
};

#endif
