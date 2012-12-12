#include	"Particle.hpp"

Particle::Particle() :
	mInherentTime(0.0),
	mPosition(0.0, 0.0, 0.0),
    mSpeed(0.0, 0.0, 0.0),
    mRadius(1.0),
    mMass(1.0),
    mColor(gWhite),
    mCellId(0),
    mBorderCrossingX(0),
    mBorderCrossingY(0){
}

/**
Moves the particle forward in time using only its speed. No collision detection is applied! 
Movement is calculated on the difference between system time and inherent time.
@param time system time
**/
void	Particle::MoveToTime(const double	time){
	mPosition += (time-mInherentTime) * mSpeed;
	mInherentTime = time;
}

/**
@param point point within the wall
@param normal normal vector of the wall
@return time of collision
**/
double	Particle::CalcCollisionTimeWithWall(const Vector& point, const Vector& normal){
	//normal constant of plane equation
	double	c = -dot(point, normal);
	return (mRadius - dot(normal, mPosition) - c)/(dot(normal, mSpeed)) + mInherentTime;
}
/**

**/
double	Particle::CalcCollisionTimeWithParticle(const Particle& particle){
	double d = mRadius + particle.mRadius;
	Vector x = mPosition - mSpeed*mInherentTime - particle.mPosition + particle.mSpeed*particle.mInherentTime;
	Vector y = mSpeed - particle.mSpeed;
	// |x + y * t| = d
	double	a = norm2(y);
	double	b = 2.0*dot(x,y);
	double	c = norm2(x) - d*d;
	double	D = b*b - 4.0*a*c;
	if	(D<0)	return -1.0;	//negative event times will be discarded
	double	t1 = (-b + sqrt(D) ) / 2.0 / a;
	double	t2 = (-b + sqrt(D) ) / 2.0 / a;
	if	((t1<t2)&&(t1>0)) return t1;
	if	(t2>0) return t2;
	return -1.0;
}

/**
**/
void	Particle::CollideWithWall(const Vector& point, const Vector& normal){
	mSpeed -= 2.0 * normal * dot(normal, mSpeed);
}
/**
**/
void	Particle::CollideWithParticle(Particle& particle){

}

