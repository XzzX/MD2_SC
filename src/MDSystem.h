#ifndef MDSYSTEM_H
#define MDSYSTEM_H

#include	<vector>

class	Configuration;
class	Particle;

class	MDSystem{
	private:
		double	mBoxWidth;
		double	mBoxHeight;
		double	mSystemTime;

		void	UpdateParticles();
	public:
		///list of particles
		std::vector<Particle>	mParticleVector;
		///returns the number of entries of mParticleVector
		unsigned int	GetNumberOfParticles(); //inline

		void	MoveToNextEvent();

		MDSystem(Configuration&	config);
};

inline
unsigned int	MDSystem::GetNumberOfParticles(){
	return	mParticleVector.size();
}

#endif