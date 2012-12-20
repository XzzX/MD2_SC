#include	"Event.h"

#include	<iostream>

Event::Event(const double time, const int i, const int j):
	mTime(time),
	mParent(0),
	mI(i),
	mJ(j),
	mNextI(0),
	mPrevI(0),
	mNextJ(0),
	mPrevJ(0),
	mRight(0),
	mLeft(0){

}

Event*	Event::InsertEvent(Event* event){
	if (mTime >= event->mTime){
		if (mLeft==0){
			mLeft = event;
			event->mParent = this;
			return event;
		}else return mLeft->InsertEvent(event);
	} else {
		if (mRight==0){
			mRight = event;
			event->mParent = this;
			return event;
		}else return mRight->InsertEvent(event);
	}
}

void	Event::Print(){
	std::cout << "#event id:\t\t" << this << std::endl;
	std::cout << "#mTime:\t\t" << mTime << std::endl;
	std::cout << "#mParent:\t\t" << mParent << std::endl;
	std::cout << "#mLeft:\t\t" << mLeft << std::endl;
	std::cout << "#mRight:\t\t" << mRight << std::endl;
	std::cout << "#mI:\t\t" << mI << std::endl;
	std::cout << "#mNextI:\t\t" << mNextI << std::endl;
	std::cout << "#mPrevI:\t\t" << mPrevI << std::endl;
	std::cout << "#mJ:\t\t" << mJ << std::endl;
	std::cout << "#mNextJ:\t\t" << mNextJ << std::endl;
	std::cout << "#mPrevJ:\t\t" << mPrevJ << std::endl;
	std::cout << std::endl;
	if (mLeft!=0) mLeft->Print();
	if (mRight!=0) mRight->Print();
}
