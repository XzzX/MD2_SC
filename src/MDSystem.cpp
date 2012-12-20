#include	"MDSystem.h"

#include	<fstream>

#include	"Configuration.hpp"
#include	"Particle.hpp"
#include	"EventCalendar.h"
#include	"Event.h"
#include	"CellSubdivision.h"

MDSystem::MDSystem():
	mSystemTime(0.0){

	mRNG.seed(gConfig.mRandomSeed);

	if (gConfig.mLatticeType == onlyone) InitParticlesOne();
	if (gConfig.mLatticeType == rectangular) InitParticlesRectangular();
	if (gConfig.mLatticeType == triangular) InitParticlesTriangular();

	SetCMSP0();

	mCellSubdivision = new CellSubdivision(gConfig.mBoxWidth, gConfig.mBoxHeight, 5.0);

	mEventCalendar = new EventCalendar(gConfig.mNumberOfParticles);
	mEventCalendar->InsertEvent(mSystemTime+0.01, -1, Event::UPDATE);
	for (unsigned int i=0; i<gConfig.mNumberOfParticles; i++){
		mParticleStartPosition.push_back(mParticleVector[i].mPosition);
		mParticleVector[i].mCellId = mCellSubdivision->InsertParticle(mParticleVector[i].mPosition + mParticleVector[i].mSpeed*0.001, i);
		mParticleVector[i].mColor = mCellSubdivision->GetCellColor(mParticleVector[i].mCellId);
		RenewEventsWithParticle(i);
	}
}
MDSystem::~MDSystem(){
	delete mEventCalendar;
}

/**
Calculates the minimum distance between two particles in correspondence to boundary conditions.
@param dist plain distance between two particles
**/
void	MDSystem::CorrectDistance(Vector& dist){
	if (dist[0]>(gConfig.mBoxWidth*0.5)) dist[0]-=gConfig.mBoxWidth; else
		if (dist[0]<(-gConfig.mBoxWidth*0.5)) dist[0]+=gConfig.mBoxWidth;
	if (dist[1]>(gConfig.mBoxHeight*0.5)) dist[1]-=gConfig.mBoxHeight; else
		if (dist[1]<(gConfig.mBoxHeight*0.5)) dist[1]+=gConfig.mBoxHeight;
}

/**
Calculates the position correction with paying attention to boundary conditions. Also saves the count of border crossings.
@param part particle which should be corrected
**/
void	MDSystem::CorrectPosition(Particle& part){
	while (part.mPosition[0]>(gConfig.mBoxWidth)){
	    part.mPosition[0] -= gConfig.mBoxWidth;
	    part.mBorderCrossingX++;
    }
	while (part.mPosition[0]<(0.0)){
        part.mPosition[0] += gConfig.mBoxWidth;
        part.mBorderCrossingX--;
    }
	while (part.mPosition[1]>(gConfig.mBoxHeight)){
	    part.mPosition[1] -= gConfig.mBoxHeight;
	    part.mBorderCrossingY++;
    }
	while (part.mPosition[1]<(0.0)){
        part.mPosition[1] += gConfig.mBoxHeight;
        part.mBorderCrossingY--;
    }
}

void	MDSystem::CorrectPosition(Vector& vec){
	if (vec[0]>(gConfig.mBoxWidth)){
	    vec[0] -= gConfig.mBoxWidth;
    }else if (vec[0]<(0.0)){
        vec[0] += gConfig.mBoxWidth;
    }
	if (vec[1]>(gConfig.mBoxHeight)){
	    vec[1] -= gConfig.mBoxHeight;
    }else if (vec[1]<(0.0)){
        vec[1] += gConfig.mBoxHeight;
    }
}

double	MDSystem::RandomUniform(const double min, const double max){
	boost::random::uniform_01<> uni;
	return	uni(mRNG) * (max-min) + min;
}

/**
Initialises one particle
**/
void MDSystem::InitParticlesOne(){
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
void MDSystem::InitParticlesRectangular(){
    //axial_ratio = v/h

	unsigned int	numa(floor(sqrt(static_cast<double>(gConfig.mNumberOfParticles))));
    unsigned int	numb(numa);

	Vector	a(1.0 * gConfig.mLatticeSpace, 0, 0);
    Vector	b(0.0, 1.0 * gConfig.mLatticeSpace, 0);

    Vector	delta((a+b)*0.5);

    for (unsigned int i = 0; i<numa; i++)
        for (unsigned int j = 0; j<numb; j++){
            Particle dummy;

            dummy.mPosition = i*a + j*b + delta;
            double alpha = RandomUniform(0.0, 2*M_PI);
            dummy.mSpeed = Vector(cos(alpha), sin(alpha), 0.0) * gConfig.mParticleSpeed;

            mParticleVector.push_back(dummy);
        }

    gConfig.mNumberOfParticles = numa*numb;
    gConfig.mBoxHeight = b[1] * (numb-1) + gConfig.mLatticeSpace;
    gConfig.mBoxWidth  = a[0] * (numa-1) + gConfig.mLatticeSpace;
}

/**
Tries to initialise m_config.number_particles particles in a triangular pattern.
**/
void MDSystem::InitParticlesTriangular(){
    //axial_ratio = v/h
    Vector	a(1.0, 0, 0);
    Vector	b(cos(M_PI/3.0), sin(M_PI/3.0), 0);

    a *= gConfig.mLatticeSpace;
    b *= gConfig.mLatticeSpace;

    unsigned int	numa(floor(sqrt(static_cast<double>(gConfig.mNumberOfParticles)/gConfig.mAxialRatio)));
    unsigned int	numb(floor(numa * gConfig.mAxialRatio));

    Vector delta(gConfig.mLatticeSpace * 0.5, gConfig.mLatticeSpace * 0.5, 0.0);

    for (unsigned int i = 0; i<numa; i++)
        for (unsigned int j = 0; j<numb; j++){
            Particle dummy;

            dummy.mPosition = i*a + j*b - (j/2)*a + delta;
            double alpha = RandomUniform(0.0, 2*M_PI);
            dummy.mSpeed = Vector(cos(alpha), sin(alpha), 0.0) * gConfig.mParticleSpeed;

            mParticleVector.push_back(dummy);
        }

    gConfig.mBoxHeight = b[1] * (numb-1) + gConfig.mLatticeSpace;
    gConfig.mBoxWidth  = a[0] * (numa-1) + gConfig.mLatticeSpace;
	gConfig.mNumberOfParticles = numa*numb;
}

void MDSystem::UpdateParticles(){
	for (unsigned int i=0; i<GetNumberOfParticles(); i++){
		mParticleVector[i].MoveToTime(mSystemTime);
		CorrectPosition(mParticleVector[i]);
		/*mCellSubdivision->DeleteParticle(mParticleVector[i].mCellId, i);
		mParticleVector[i].mCellId = mCellSubdivision->InsertParticle(mParticleVector[i].mPosition, i);
		mParticleVector[i].mColor = mCellSubdivision->GetCellColor(mParticleVector[i].mCellId);*/
	}
}

void	MDSystem::MoveToNextEvent(){
	EvaluateEvent(* mEventCalendar->NextEvent());
}

void	MDSystem::EvaluateEvent(Event& event){
	mSystemTime = event.mTime;

	if (event.mJ>=0){
		mParticleVector[event.mI].MoveToTime(mSystemTime);
		mParticleVector[event.mJ].MoveToTime(mSystemTime);
		CorrectPosition(mParticleVector[event.mI]);
		CorrectPosition(mParticleVector[event.mJ]);
		mParticleVector[event.mI].CollideWithParticle(mParticleVector[event.mJ]);
		unsigned int pid = event.mJ;
		RenewEventsWithParticle(event.mI);
		RenewEventsWithParticle(pid);
		return;
	}

	Vector temp;

	switch	(event.mJ){
		case	Event::NONE:
			return;
		case	Event::UPDATE:
			UpdateParticles();
			mEventCalendar->RemoveEvent(&event);
			if (!gConfig.mNoGUI)
				mEventCalendar->InsertEvent(mSystemTime+0.01, -1, Event::UPDATE);
			break;
		case	Event::COLLISION_WITH_BOTTOM_WALL:
			mParticleVector[event.mI].MoveToTime(mSystemTime);
			mParticleVector[event.mI].CollideWithWall(Vector(0.0,0.0,0.0), Vector(0.0,1.0,0.0));
			RenewEventsWithParticle(event.mI);
			break;
		case	Event::COLLISION_WITH_LEFT_WALL:
			mParticleVector[event.mI].MoveToTime(mSystemTime);
			mParticleVector[event.mI].CollideWithWall(Vector(0.0,0.0,0.0), Vector(1.0,0.0,0.0));
			RenewEventsWithParticle(event.mI);
			break;
		case	Event::COLLISION_WITH_TOP_WALL:
			mParticleVector[event.mI].MoveToTime(mSystemTime);
			mParticleVector[event.mI].CollideWithWall(Vector(0.0,gConfig.mBoxHeight,0.0), Vector(0.0,-1.0,0.0));
			RenewEventsWithParticle(event.mI);
			break;
		case	Event::COLLISION_WITH_RIGHT_WALL:
			mParticleVector[event.mI].MoveToTime(mSystemTime);
			mParticleVector[event.mI].CollideWithWall(Vector(gConfig.mBoxWidth,0.0,0.0), Vector(-1.0,0.0,0.0));
			RenewEventsWithParticle(event.mI);
			break;
		case	Event::COLLISION_WITH_BOTTOM_CELLWALL:
			mParticleVector[event.mI].MoveToTime(mSystemTime);
			CorrectPosition(mParticleVector[event.mI]);
			mCellSubdivision->DeleteParticle(mParticleVector[event.mI].mCellId, event.mI);
			temp = (mParticleVector[event.mI].mPosition + Vector(0,-0.0001,0));
			CorrectPosition(temp);
			mParticleVector[event.mI].mCellId = mCellSubdivision->InsertParticle(temp, event.mI);
			mParticleVector[event.mI].mColor = mCellSubdivision->GetCellColor(mParticleVector[event.mI].mCellId);
			RenewEventsWithParticle(event.mI);
			break;
		case	Event::COLLISION_WITH_LEFT_CELLWALL:
			mParticleVector[event.mI].MoveToTime(mSystemTime);
			CorrectPosition(mParticleVector[event.mI]);
			mCellSubdivision->DeleteParticle(mParticleVector[event.mI].mCellId, event.mI);
			temp = (mParticleVector[event.mI].mPosition + Vector(-0.0001,0,0));
			CorrectPosition(temp);
			mParticleVector[event.mI].mCellId = mCellSubdivision->InsertParticle(temp, event.mI);
			mParticleVector[event.mI].mColor = mCellSubdivision->GetCellColor(mParticleVector[event.mI].mCellId);
			RenewEventsWithParticle(event.mI);
			break;
		case	Event::COLLISION_WITH_TOP_CELLWALL:
			mParticleVector[event.mI].MoveToTime(mSystemTime);
			CorrectPosition(mParticleVector[event.mI]);
			mCellSubdivision->DeleteParticle(mParticleVector[event.mI].mCellId, event.mI);
			temp = (mParticleVector[event.mI].mPosition + Vector(0,0.0001,0));
			CorrectPosition(temp);
			mParticleVector[event.mI].mCellId = mCellSubdivision->InsertParticle(temp, event.mI);
			mParticleVector[event.mI].mColor = mCellSubdivision->GetCellColor(mParticleVector[event.mI].mCellId);
			RenewEventsWithParticle(event.mI);
			break;
		case	Event::COLLISION_WITH_RIGHT_CELLWALL:
			mParticleVector[event.mI].MoveToTime(mSystemTime);
			CorrectPosition(mParticleVector[event.mI]);
			mCellSubdivision->DeleteParticle(mParticleVector[event.mI].mCellId, event.mI);
			temp = (mParticleVector[event.mI].mPosition + Vector(0.0001,0,0));
			CorrectPosition(temp);
			mParticleVector[event.mI].mCellId = mCellSubdivision->InsertParticle(temp, event.mI);
			mParticleVector[event.mI].mColor = mCellSubdivision->GetCellColor(mParticleVector[event.mI].mCellId);
			RenewEventsWithParticle(event.mI);
			break;
	}
}

void	MDSystem::RenewEventsWithParticle(const int pid){
	assert(pid>=0);

	mEventCalendar->RemoveAllEventsWithParticle(pid);

	double t;

	/*
	t = mParticleVector[pid].CalcCollisionTimeWithWall(Vector(0.0,0.0,0.0), Vector(0.0,1.0,0.0));
	if (t>mSystemTime) 
		mEventCalendar->InsertEvent(t, pid, Event::COLLISION_WITH_BOTTOM_WALL);
	t = mParticleVector[pid].CalcCollisionTimeWithWall(Vector(0.0,0.0,0.0), Vector(1.0,0.0,0.0));
	if (t>mSystemTime) 
		mEventCalendar->InsertEvent(t, pid, Event::COLLISION_WITH_LEFT_WALL);
	t = mParticleVector[pid].CalcCollisionTimeWithWall(Vector(0.0,gConfig.mBoxHeight,0.0), Vector(0.0,-1.0,0.0));
	if (t>mSystemTime) 
		mEventCalendar->InsertEvent(t, pid, Event::COLLISION_WITH_TOP_WALL);
	t = mParticleVector[pid].CalcCollisionTimeWithWall(Vector(gConfig.mBoxWidth,0.0,0.0), Vector(-1.0,0.0,0.0));
	if (t>mSystemTime) 
		mEventCalendar->InsertEvent(t, pid, Event::COLLISION_WITH_RIGHT_WALL);
	*/

	Vector point;
	Vector normal;
	mCellSubdivision->GetCellWallBottom(mParticleVector[pid].mCellId, point, normal);
	t = mParticleVector[pid].CalcCollisionTimeWithWallCenter(point, normal);
	if (t>mSystemTime) 
		mEventCalendar->InsertEvent(t, pid, Event::COLLISION_WITH_BOTTOM_CELLWALL);

	mCellSubdivision->GetCellWallLeft(mParticleVector[pid].mCellId, point, normal);
	t = mParticleVector[pid].CalcCollisionTimeWithWallCenter(point, normal);
	if (t>mSystemTime) 
		mEventCalendar->InsertEvent(t, pid, Event::COLLISION_WITH_LEFT_CELLWALL);

	mCellSubdivision->GetCellWallTop(mParticleVector[pid].mCellId, point, normal);
	t = mParticleVector[pid].CalcCollisionTimeWithWallCenter(point, normal);
	if (t>mSystemTime) 
		mEventCalendar->InsertEvent(t, pid, Event::COLLISION_WITH_TOP_CELLWALL);

	mCellSubdivision->GetCellWallRight(mParticleVector[pid].mCellId, point, normal);
	t = mParticleVector[pid].CalcCollisionTimeWithWallCenter(point, normal);
	if (t>mSystemTime) 
		mEventCalendar->InsertEvent(t, pid, Event::COLLISION_WITH_RIGHT_CELLWALL);

	/*for (unsigned int i = 0; i<GetNumberOfParticles(); i++){
		if (i!=pid){
			t = mParticleVector[pid].CalcCollisionTimeWithParticle(mParticleVector[i], mSystemTime);
			if (t>mSystemTime) 
				mEventCalendar->InsertEvent(t, pid, i);
		}
	}*/
	list<unsigned int>::iterator    it;
    mCellSubdivision->GetNeighbours(mParticleVector[pid].mCellId, pid);
    for (it=mCellSubdivision->GetNeighbourBegin(); it!=mCellSubdivision->GetNeighbourEnd(); it++){
		t = mParticleVector[pid].CalcCollisionTimeWithParticle(mParticleVector[*it], mSystemTime);
		if (t>mSystemTime) 
			mEventCalendar->InsertEvent(t, pid, *it);
	}
}

void MDSystem::SetCMSP0(){
    //berechne Gesamtimpuls
    Vector p(0,0,0);

    for (unsigned int i=0; i<mParticleVector.size(); i++){
        p += mParticleVector[i].mSpeed * mParticleVector[i].mMass;
    }

    p /= static_cast<double>(mParticleVector.size());

    std::cout << "Gesamtimpuls: " << p << std::endl;

    //Neuberechnung der Impulse

    for (unsigned int i=0; i<mParticleVector.size(); i++){
        mParticleVector[i].mSpeed -= p / mParticleVector[i].mMass;
    }
}

void MDSystem::Observe(){

    Vector pos(0,0,0);
    Vector v(0,0,0);
    double  D = 0.0;
    double  D2 = 0.0;
    Vector  a(gConfig.mBoxWidth,0.0,0.0);
    Vector  b(0.0,gConfig.mBoxHeight,0.0);
    for (unsigned int i = 0; i < GetNumberOfParticles(); i++){
        pos += mParticleVector[i].mPosition;
		v += mParticleVector[i].mSpeed;
		double  d = norm(mParticleVector[i].mPosition+mParticleVector[i].mBorderCrossingX*a+mParticleVector[i].mBorderCrossingY*b - mParticleStartPosition[i]);
		D += d;
		D2 += d*d;
    }
	mTime.push_back(mSystemTime);

    mPositionList.push_back(pos / double(GetNumberOfParticles()));
    mVelocityList.push_back(v / double(GetNumberOfParticles()));

    mD.push_back(D/double(GetNumberOfParticles()));
    mD2.push_back(D2/double(GetNumberOfParticles()));
}

/**
Dumps observed data to file. Observed informations are:
time
position
speed
kinetic energy
potential energy
distance to start position
distance to start position ^2
@param filename filename of the dump file
**/
void    MDSystem::DumpData(){
    fstream fout(gConfig.mLogName+".txt", fstream::out);

	fout << gConfig;

	list<double>::iterator it0 = mTime.begin();
    list<Vector>::iterator it1 = mPositionList.begin();
    list<Vector>::iterator it2 = mVelocityList.begin();
    list<double>::iterator it3 = mD.begin();
    list<double>::iterator it4 = mD2.begin();

    for(unsigned int i=0; i<mPositionList.size(); i++, it0++, it1++, it2++, it3++, it4++){
        fout << *it0 << "\t";
        fout << *it1 << "\t";
		fout << *it2 << "\t";
		fout << *it3 << "\t";
		fout << *it4 << endl;
    }
    fout.close();

    fout.open(gConfig.mLogName+"_correlation.txt", fstream::out);
	fout << gConfig;

    for (unsigned int i = 0; i < GetNumberOfParticles(); i++)
        for (unsigned int j = i+1; j < GetNumberOfParticles(); j++){
            Vector rij(mParticleVector[j].mPosition - mParticleVector[i].mPosition);
            CorrectDistance(rij);
            fout << norm(rij) << std::endl;
        }
    fout.close();

    fout.open(gConfig.mLogName+"_speed.txt", fstream::out);
	fout << gConfig;

	for (unsigned int i=0; i<GetNumberOfParticles(); i++){
        fout << norm(mParticleVector[i].mSpeed) << std::endl;
	}

	fout.close();
}
