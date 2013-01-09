#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include	<iostream>
#include	<sstream>
#include	<iomanip>
#include	<string>
#include    <stdexcept>

enum LatticeTypes {
    onlyone,
    rectangular,
    triangular,
    random,
    individual
};

inline std::ostream &operator << (std::ostream &stream, const LatticeTypes lattice)
{
    if (lattice == onlyone)
        stream << "onlyone lattice";
    else if (lattice == rectangular)
        stream << "rectangular lattice";
    else if (lattice == triangular)
        stream << "triangular lattice";
    else if (lattice == random)
        stream << "random positioning of the particles";
    else if (lattice == individual)
        stream << "individual configuration chosen";
    else {
        std::cerr << "unkown lattice type" << std::endl;
        throw std::logic_error("unkown lattice type, abort program!");
    }
    return stream;
}
class Configuration{
	public:
		unsigned int mRandomSeed;				///<seed of the random number generator

		unsigned int mNumberOfParticles;		///<number of particles in the simulation
		double	mLatticeSpace;					///<space between particles
		double	mAxialRatio;					///<v/h
		double	mBoxWidth;						///<width  of the  simulation area
		double	mBoxHeight;						///<height of the simulation area

		double	mParticleSpeed;					///<starting velocity of each particle

		LatticeTypes	mLatticeType;			///<geometrical construction of lattice

		double	mL;								///<length of the simulation box in random mode

		unsigned int	mRuns;					///<number of simulation runs, only activ in nogui-mode
		bool	mNoGUI;							///<run without a graphical user interface

		std::string mLogName;					///<filename of the logfile

		///load basic configuration
		Configuration();
		///read out the command line parameters
		void	ReadCommandLineParameters( unsigned int argc, char **argv );
		void	SaveConfiguration();
};

extern	Configuration	gConfig;

///overloaded << operator to show easily the configuration on the screen
std::ostream &operator << (std::ostream &stream, const Configuration& config);

#endif
