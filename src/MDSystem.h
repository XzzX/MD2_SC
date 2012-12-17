#ifndef MDSYSTEM_H
#define MDSYSTEM_H

#include	<vector>
#include	<boost/random/mersenne_twister.hpp>
#include	<boost/random/uniform_01.hpp>

class	Configuration;
class	Particle;
class	Event;

/**
Container class for the simulation environment.
**/
class	MDSystem{
	private:
		///random number generator
		boost::random::mt19937 mRNG;
		///width of the simulation box
		double	mBoxWidth;
		///height of the simulation box
		double	mBoxHeight;
		///current system time
		double	mSystemTime;

		///updates inherent times of all particles to mSystemTime
		void	UpdateParticles();

		double	RandomUniform(const double min, const double max);
	public:
		///list of particles
		std::vector<Particle>	mParticleVector;
		///returns the number of entries of mParticleVector
		unsigned int	GetNumberOfParticles(); //inline
		///evolves the system until next event
		void	MoveToNextEvent();
		///runs actions needed to complete the event
		void	EvaluateEvent(const Event& event);

		MDSystem(Configuration&	config);

		void	InitParticlesOne(Configuration& config);
		void	InitParticlesRectangular(Configuration& config);
};

inline
unsigned int	MDSystem::GetNumberOfParticles(){
	return	mParticleVector.size();
}

#endif
