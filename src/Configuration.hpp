#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include	<iostream>
#include	<sstream>
#include	<iomanip>
#include	<string>
#include    <stdexcept>

enum lattice_types {
    onlyone,
    rectangular,
    triangular,
    random_lattice,
    individual
};

inline std::ostream &operator << (std::ostream &stream, const lattice_types lattice)
{
    if (lattice == onlyone)
        stream << "onlyone lattice";
    else if (lattice == rectangular)
        stream << "rectangular lattice";
    else if (lattice == triangular)
        stream << "triangular lattice";
    else if (lattice == random_lattice)
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
		unsigned int mRandomSeed;				///seed of the random number generator

		unsigned int mNumberOfParticles;		///number of particles in the simulation
		double	mBoxWidth;						///width  of the  simulation area
		double	mBoxHeight;						///height of the simulation area

		double	mParticleSpeed;					///starting velocity of each particle

		bool	mNoGUI;							///run without a graphical user interface

		std::string mLogName;					///filename of the logfile

		///load basic configuration
		Configuration();
};

//overloaded << operator to show easily the configuration on the screen
inline std::ostream &operator << (std::ostream &stream, const Configuration& config)
{
	stream << "#CONFIGURATION" << std::endl;
	stream << "#seed:                " << config.mRandomSeed << std::endl;
	stream << "#number of particles: " << config.mNumberOfParticles << std::endl;
	stream << "#box_width:           " << config.mBoxWidth << std::endl;
	stream << "#box height:          " << config.mBoxHeight << std::endl;
	stream << "#logname:             " << config.mLogName << std::endl;
    return stream;
}

#endif
