/**
 *  \file		Particle.hpp
 *  \brief		Implements a class which represents a particle used in simulation.
 *  \details	Used type is double
 *  \author		Sebastian Eibl
 *  \version	1.0
 *  \date		16/12/2012
 *  \pre		none
 *  \bug		none
 *  \warning	none
 *  \copyright	Sebastian Eibl
 */

#ifndef PARTICLES_HPP
#define PARTICLES_HPP
#include	<iomanip>

#include	"Color.h"
#include	"Vector.hpp"

///structure which contains all the necessary informations about the particle
class Particle {
	public:
		///actual time of the particle which is not equivalent to the system time!
		double	mInherentTime;
		///position of the particle
		Vector mPosition;
		///speed of the particle
		Vector mSpeed;
		///radius of the particle
		double mRadius;
		///mass of the particle
		double mMass;
		///color of the particle
		Color mColor;
		///cellid of the particle in the CellSubdivision class
		unsigned int mCellId;
		///number of border crossings of the simulation box in x direction
		int mBorderCrossingX;
		///number of border crossings of the simulation box in y direction
		int mBorderCrossingY;

		///next element in cluster
		Particle*	mClusterNext;
		///previous element in cluster
		Particle*	mClusterPrev;

		///initialise everything with basic data
		Particle();

		///moves the particle forward in time
		void	MoveToTime(const double	time);
		///restricts particle to box dimensions
		void	PositionCorrecton(const double width, const double height);

		///calculates the moment of the collision with a wall
		double	CalcCollisionTimeWithWall(const Vector& point, const Vector& normal);
		///calculates the moment of the collision with a wall without radius
		double	CalcCollisionTimeWithWallCenter(const Vector& point, const Vector& normal);
		///calculates the moment of the collision with a particle
		double	CalcCollisionTimeWithParticle(const Particle& particle, const double systemTime);
		///calculates if two particles overlap
		bool	OverlapWithParticle(const Particle& particle);

		bool	IsConnected(const Particle& particle);
		///calculates the new speed after the collision with a wall
		void	CollideWithWall(const Vector& point, const Vector& normal);
		///calculates the new speed after the collision with another particle
		void	CollideWithParticle(Particle& particle);
};


#endif
