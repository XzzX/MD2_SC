#include	"MDSystem.h"

#include	"Configuration.hpp"
#include	"Particle.hpp"
#include	"Event.h"

MDSystem::MDSystem(Configuration& config):
	mSystemTime(0.0){

	mRNG.seed(config.mRandomSeed);

	if (config.mLatticeType == onlyone) InitParticlesOne(config);
	if (config.mLatticeType == rectangular) InitParticlesRectangular(config);

	mBoxWidth = config.mBoxWidth;
	mBoxHeight = config.mBoxHeight;
}

double	MDSystem::RandomUniform(const double min, const double max){
	boost::random::uniform_01<> uni;
	return	uni(mRNG) * (max-min) + min;
}

/**
Initialises one particle
**/
void MDSystem::InitParticlesOne(Configuration& config){
	///not really one but testing configuration ;-)
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

/**
Tries to initialise m_config.number_particles particles in a rectangular pattern.
**/
void MDSystem::InitParticlesRectangular(Configuration& config){
    //axial_ratio = v/h

	unsigned int	numa(floor(sqrt(static_cast<double>(config.mNumberOfParticles))));
    unsigned int	numb(numa);

	Vector	a(1.0 * config.mLatticeSpace, 0, 0);
    Vector	b(0.0, 1.0 * config.mLatticeSpace, 0);

    Vector	delta((a+b)*0.5);

    for (unsigned int i = 0; i<numa; i++)
        for (unsigned int j = 0; j<numb; j++){
            Particle dummy;

            dummy.mPosition = i*a + j*b + delta;
            double alpha = RandomUniform(0.0, 2*M_PI);
            dummy.mSpeed = Vector(cos(alpha), sin(alpha), 0.0) * config.mParticleSpeed;

            mParticleVector.push_back(dummy);
        }

    config.mNumberOfParticles = numa*numb;
	config.mBoxWidth = a[0] * numa;
	config.mBoxHeight = b[1] * numb;
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
