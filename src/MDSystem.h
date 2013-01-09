#ifndef MDSYSTEM_H
#define MDSYSTEM_H

#include	<vector>
#include	<list>
#include	<boost/random/mersenne_twister.hpp>
#include	<boost/random/uniform_01.hpp>

#include	"Vector.hpp"

class	Configuration;
class	Particle;
class	Event;
class	EventCalendar;
class	CellSubdivision;

/**
Container class for the simulation environment.
**/
class	MDSystem{
	private:
		///random number generator
		boost::random::mt19937 mRNG;
		///current system time
		double	mSystemTime;

		EventCalendar*	mEventCalendar;

		CellSubdivision*	mCellSubdivision;

		std::vector< Vector >	mParticleStartPosition;

		std::list<Vector>	mPositionList;
		std::list<Vector>	mVelocityList;

		std::list<double>	mD;
		std::list<double>	mD2;

		std::list<double>	mTime;

		///updates inherent times of all particles to mSystemTime
		void	UpdateParticles();

		void	CorrectDistance(Vector& dist);

		void	CorrectPosition(Particle& part);
		void	CorrectPosition(Vector& vec);
		double	RandomUniform(const double min, const double max);

		void	InitParticlesOne();
		void	InitParticlesRectangular();
		void	InitParticlesTriangular();
		void	InitParticlesRandom();

		///creates cluster structure for initialised particles
		void	BuildCluster();

		void	SetCMSP0();
	public:
		///list of particles
		std::vector<Particle>	mParticleVector;
		///returns the number of entries of mParticleVector
		unsigned int	GetNumberOfParticles(); //inline
		///evolves the system until next event
		void	MoveToNextEvent();
		///runs actions needed to complete the event
		void	EvaluateEvent(Event& event);

		void	RenewEventsWithParticle(const int pid);


		///randomizes the position of each particle
		void	RandomizeParticles();
		///test for percolations
		bool	IsPercolatic();

		MDSystem();
		~MDSystem();

		void	Observe();
		void	DumpData();
};

inline
unsigned int	MDSystem::GetNumberOfParticles(){
	return	mParticleVector.size();
}

#endif
