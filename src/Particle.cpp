#include	<iostream>

#include	"Particle.hpp"
#include	"Configuration.hpp"

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
@return absolut time of collision. Negative values mean collision won't happen!
**/
double	Particle::CalcCollisionTimeWithWall(const Vector& point, const Vector& normal){
	if (dot(mSpeed, normal)>0) return -1;
	//normal constant of plane equation
	double	c = -dot(point, normal);
	return (mRadius - dot(normal, mPosition) - c)/(dot(normal, mSpeed)) + mInherentTime;
}
/**
@param point point within the wall
@param normal normal vector of the wall
@return absolut time of collision. Negative values mean collision won't happen!
**/
double	Particle::CalcCollisionTimeWithWallCenter(const Vector& point, const Vector& normal){
	if (dot(mSpeed, normal)>0) return -1;
	//normal constant of plane equation
	double	c = -dot(point, normal);
	return (- dot(normal, mPosition) - c)/(dot(normal, mSpeed)) + mInherentTime;
}
/**
@param particle second particle for the collision test
@param systemTime current system time, used to determine which collision time lies in the past
@return absolut time of collision. Negative values mean collision won't happen!
**/
double	Particle::CalcCollisionTimeWithParticle(const Particle& particle, const double systemTime){
	Vector	cor(0,0,0);
	Vector  dist = mPosition - particle.mPosition;
	if (dist[0]>gConfig.mBoxWidth*0.5) cor[0] = -gConfig.mBoxWidth;
	if (dist[0]<-gConfig.mBoxWidth*0.5) cor[0] = gConfig.mBoxWidth;
	if (dist[1]>gConfig.mBoxHeight*0.5) cor[1] = -gConfig.mBoxHeight;
	if (dist[1]<-gConfig.mBoxHeight*0.5) cor[1] = gConfig.mBoxHeight;

	mPosition += cor;

	double d = mRadius + particle.mRadius;
	Vector x = mPosition - mSpeed*mInherentTime - particle.mPosition + particle.mSpeed*particle.mInherentTime;
	Vector y = mSpeed - particle.mSpeed;

	mPosition -= cor;
	// |x + y * t| = d
	double	a = norm2(y);
	double	b = 2.0*dot(x,y);
	double	c = norm2(x) - d*d;
	double	D = b*b - 4.0*a*c;
	if	(D<0)	return -1.0;	//negative event times will be discarded
	double	t1 = (-b + sqrt(D) ) / 2.0 / a;
	double	t2 = (-b - sqrt(D) ) / 2.0 / a;
	if	( (t1<t2) && (t1>systemTime) )  return t1;
	if	( (t2>systemTime) )  return t2;
	return -1.0;
}

/**
@param point point in the wall plane
@param normal normal vector of the wall plane
**/
void	Particle::CollideWithWall(const Vector& point, const Vector& normal){
	mSpeed -= 2.0 * normal * dot(normal, mSpeed);
}
/**
The speed of both particles will be recalculated!
@param particle referenz to the other particle
**/
//http://www.spieleprogrammierung.net/2010/01/kollision-von-massenpunkten-im-3d-raum.html
void	Particle::CollideWithParticle(Particle& particle){
	Vector	cor(0,0,0);
	Vector  dist = mPosition - particle.mPosition;
	if (dist[0]>gConfig.mBoxWidth*0.5) cor[0] = -gConfig.mBoxWidth;
	if (dist[0]<-gConfig.mBoxWidth*0.5) cor[0] = gConfig.mBoxWidth;
	if (dist[1]>gConfig.mBoxHeight*0.5) cor[1] = -gConfig.mBoxHeight;
	if (dist[1]<-gConfig.mBoxHeight*0.5) cor[1] = gConfig.mBoxHeight;

	mPosition += cor;

    // Berechnung der Kollisionsachse (normierte Verbindungslinie zwischen
    // den kollidierenden Massepunkten):
    Vector CollisionAxis = particle.mPosition - mPosition;

	if ((dot(mSpeed, CollisionAxis)>0)||(dot(particle.mSpeed, CollisionAxis)<0)){

		CollisionAxis /= norm(CollisionAxis);

		// Berechnung der Anfangsgeschwindigkeitsbeträge entlang der
		// Kollisionsachse:
		double tempFactor1 = dot(mSpeed, CollisionAxis);
		double tempFactor2 = dot(particle.mSpeed, CollisionAxis);

		Vector CollisionAxisVelocity1 = tempFactor1*CollisionAxis;
		Vector CollisionAxisVelocity2 = tempFactor2*CollisionAxis;

		// Subtrahiert man nun diese Geschwindigkeitsbeträge
		// von den Anfangsgeschwindigkeiten, so erhält man die
		// Geschwindigkeitsanteile, die während der Kollision
		// unverändert bleiben:
		mSpeed -= CollisionAxisVelocity1;
		particle.mSpeed -= CollisionAxisVelocity2;

		// Berechnung der Geschwindigkeitsanteile entlang
		// der Kollisionsachse nach dem Stoß:
		double tempFactor3 = mMass + particle.mMass;
		double tempFactor4 = mMass - particle.mMass;

		double tempFactor5 = (2.0f*particle.mMass*tempFactor2 + tempFactor1*tempFactor4)/
							 tempFactor3;

		double tempFactor6 = (2.0f*mMass*tempFactor1 - tempFactor2*tempFactor4)/
							 tempFactor3;

		CollisionAxisVelocity1 = tempFactor5*CollisionAxis;
		CollisionAxisVelocity2 = tempFactor6*CollisionAxis;

		// Addiert man die Geschwindigkeitsanteile entlang
		// der Kollisionsachse nach dem Stoß zu den
		// Geschwindigkeitsanteilen, die während des Stoßes
		// unverändert bleiben, erhält man die
		// Endgeschwindigkeiten:
		mSpeed += CollisionAxisVelocity1;
		particle.mSpeed += CollisionAxisVelocity2;
	}

	mPosition -= cor;
}

