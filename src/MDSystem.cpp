#include	"MDSystem.h"

#include	"Configuration.hpp"
#include	"Particle.hpp"

MDSystem::MDSystem(Configuration& config):
	mBoxWidth(config.mBoxWidth),
	mBoxHeight(config.mBoxHeight),
	mSystemTime(0.0){

	Particle	dummy;
	dummy.mPosition = Vector(5.0,5.0,0.0);
	dummy.mSpeed = Vector(2.0,0.9, 0.0);
	mParticleVector.push_back(dummy);
}

void MDSystem::UpdateParticles(){
	for (unsigned int i=0; i<GetNumberOfParticles(); i++)
		mParticleVector[i].MoveToTime(mSystemTime);
}

void	MDSystem::MoveToNextEvent(){
	double nextEventTime = mSystemTime + 0.01;
	double t;
	for (unsigned int i=0; i<GetNumberOfParticles(); i++){
		t = mParticleVector[i].CalcCollisionTimeWithWall(Vector(0.0,0.0,0.0), Vector(0.0,1.0,0.0));
		if ((t>0.0)&&(t<nextEventTime)) nextEventTime = t;
		t = mParticleVector[i].CalcCollisionTimeWithWall(Vector(0.0,0.0,0.0), Vector(1.0,0.0,0.0));
		if ((t>0.0)&&(t<nextEventTime)) nextEventTime = t;
		t = mParticleVector[i].CalcCollisionTimeWithWall(Vector(0.0,mBoxHeight,0.0), Vector(0.0,-1.0,0.0));
		if ((t>0.0)&&(t<nextEventTime)) nextEventTime = t;
		t = mParticleVector[i].CalcCollisionTimeWithWall(Vector(mBoxWidth,0.0,0.0), Vector(-1.0,0.0,0.0));
		if ((t>0.0)&&(t<nextEventTime)) nextEventTime = t;
	}
	mSystemTime = nextEventTime;
	UpdateParticles();
}