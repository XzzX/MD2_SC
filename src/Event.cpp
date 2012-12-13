#include	"Event.h"

Event::Event(const double time, const int i, const int j):
	mTime(time),
	mI(i),
	mJ(j),
	mNextI(0),
	mPrevI(0),
	mNextJ(0),
	mPrevJ(0){

}
