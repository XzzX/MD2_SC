#include	"MDSystem.h"

#include	"Configuration.hpp"
#include	"Particle.hpp"
#include	"Event.h"

MDSystem::MDSystem(Configuration& config):
	mBoxWidth(config.mBoxWidth),
	mBoxHeight(config.mBoxHeight),
	mSystemTime(0.0){

	Particle	dummy;
	dummy.mPosition = Vector(5.0,5.0,0.0);
	dummy.mSpeed = Vector(3.0,1.5, 0.0);
	mParticleVector.push_back(dummy);

	dummy.mPosition = Vector(8.0,5.0,0.0);
	dummy.mSpeed = Vector(-2.0,2.0, 0.0);
	mParticleVector.push_back(dummy);

	dummy.mPosition = Vector(5.0,2.0,0.0);
	dummy.mSpeed = Vector(-2.0,-2.0, 0.0);
	mParticleVector.push_back(dummy);

	dummy.mPosition = Vector(5.0,8.0,0.0);
	dummy.mSpeed = Vector(2.0,2.0, 0.0);
	mParticleVector.push_back(dummy);
}

void MDSystem::UpdateParticles(){
	for (unsigned int i=0; i<GetNumberOfParticles(); i++)
		mParticleVector[i].MoveToTime(mSystemTime);
}

void	MDSystem::MoveToNextEvent(){
	Event	event(mSystemTime + 0.01, -1, Event::UPDATE);

	double t;
	for (unsigned int i=0; i<GetNumberOfParticles(); i++){
		t = mParticleVector[i].CalcCollisionTimeWithWall(Vector(0.0,0.0,0.0), Vector(0.0,1.0,0.0));
		if ( (t>mSystemTime) && (t<event.mTime) ) {
			event.mTime = t;
			event.mI = i;
			event.mJ = Event::COLLISION_WITH_BOTTOM_WALL;
		}
		t = mParticleVector[i].CalcCollisionTimeWithWall(Vector(0.0,0.0,0.0), Vector(1.0,0.0,0.0));
		if ( (t>mSystemTime) && (t<event.mTime) ) {
			event.mTime = t;
			event.mI = i;
			event.mJ = Event::COLLISION_WITH_LEFT_WALL;
		}
		t = mParticleVector[i].CalcCollisionTimeWithWall(Vector(0.0,mBoxHeight,0.0), Vector(0.0,-1.0,0.0));
		if ( (t>mSystemTime) && (t<event.mTime) ) {
			event.mTime = t;
			event.mI = i;
			event.mJ = Event::COLLISION_WITH_TOP_WALL;
		}
		t = mParticleVector[i].CalcCollisionTimeWithWall(Vector(mBoxWidth,0.0,0.0), Vector(-1.0,0.0,0.0));
		if ( (t>mSystemTime) && (t<event.mTime) ) {
			event.mTime = t;
			event.mI = i;
			event.mJ = Event::COLLISION_WITH_RIGHT_WALL;
		}
		for (unsigned int j=i+1; j<GetNumberOfParticles(); j++){
			t = mParticleVector[i].CalcCollisionTimeWithParticle(mParticleVector[j], mSystemTime);
			if ( (t>mSystemTime) && (t<event.mTime) ) {
				event.mTime = t;
				event.mI = i;
				event.mJ = j;
			}
		}
	}

	EvaluateEvent(event);
}

void	MDSystem::EvaluateEvent(const Event& event){
	mSystemTime = event.mTime;

	if (event.mJ>=0){
		mParticleVector[event.mI].MoveToTime(mSystemTime);
		mParticleVector[event.mJ].MoveToTime(mSystemTime);
		mParticleVector[event.mI].CollideWithParticle(mParticleVector[event.mJ]);
		return;
	}
	switch	(event.mJ){
		case	Event::UPDATE:
			UpdateParticles();
			break;
		case	Event::COLLISION_WITH_BOTTOM_WALL:
			mParticleVector[event.mI].MoveToTime(mSystemTime);
			mParticleVector[event.mI].CollideWithWall(Vector(0.0,0.0,0.0), Vector(0.0,1.0,0.0));
			break;
		case	Event::COLLISION_WITH_LEFT_WALL:
			mParticleVector[event.mI].MoveToTime(mSystemTime);
			mParticleVector[event.mI].CollideWithWall(Vector(0.0,0.0,0.0), Vector(1.0,0.0,0.0));
			break;
		case	Event::COLLISION_WITH_TOP_WALL:
			mParticleVector[event.mI].MoveToTime(mSystemTime);
			mParticleVector[event.mI].CollideWithWall(Vector(0.0,mBoxHeight,0.0), Vector(0.0,-1.0,0.0));
			break;
		case	Event::COLLISION_WITH_RIGHT_WALL:
			mParticleVector[event.mI].MoveToTime(mSystemTime);
			mParticleVector[event.mI].CollideWithWall(Vector(mBoxWidth,0.0,0.0), Vector(-1.0,0.0,0.0));
			break;
	}
}
