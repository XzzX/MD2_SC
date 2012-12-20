#include	"Configuration.hpp"

#include	"util.h"

Configuration	gConfig;

std::ostream &operator << (std::ostream &stream, const Configuration& config){
	stream << "#CONFIGURATION" << std::endl;
	stream << "#seed:                " << config.mRandomSeed << std::endl;
	stream << "#number of particles: " << config.mNumberOfParticles << std::endl;
	stream << "#lattice space        " << config.mLatticeSpace << std::endl;
	stream << "#axial ratio          " << config.mAxialRatio << std::endl;
	stream << "#box_width:           " << config.mBoxWidth << std::endl;
	stream << "#box height:          " << config.mBoxHeight << std::endl;
	stream << "#lattice type:		 " << config.mLatticeType << std::endl;
	stream << "#particle speed:		 " << config.mParticleSpeed << std::endl;
	stream << "#runs:			     " << config.mRuns << std::endl;
	stream << "#nogui:			     " << config.mNoGUI << std::endl;
	stream << "#logname:             " << config.mLogName << std::endl;
    return stream;
}

Configuration::Configuration() :
	mRandomSeed(time(0)),
	mNumberOfParticles(100),
	mLatticeSpace(3.0),
	mAxialRatio(1.0),
	mBoxWidth(50.0),
	mBoxHeight(50.0),
	mLatticeType(triangular),
	mParticleSpeed(2.0),
	mRuns(1000),
	mNoGUI(false),
	mLogName("data")
{}

void	Configuration::ReadCommandLineParameters( unsigned int argc, char **argv ){
    for (unsigned int i=1; i<argc; i++){

        std::string cmd = argv[i];

        if (cmd.compare("-N") == 0){
			i++;
            mNumberOfParticles = StringTo<unsigned int>(argv[i]);
        }
        else if (cmd.compare("-lattice") == 0){
			i++;
            mLatticeSpace = StringTo<double>(argv[i]);
		}
        else if (cmd.compare("-seed") == 0){
			i++;
            mRandomSeed = StringTo<unsigned int>(argv[i]);
		}
        else if (cmd.compare("-v") == 0){
			i++;
            mParticleSpeed = StringTo<double>(argv[i]);
        }
        else if (cmd.compare("-runs") == 0){
			i++;
            mRuns = StringTo<unsigned int>(argv[i]);
        }
		else if (cmd.compare("-gui") == 0){
			i++;
            if (StringTo<int>(argv[i])==0)
                mNoGUI = true;
        }
        else if (cmd.compare("-log") == 0){
			i++;
            mLogName = argv[i];
        }
    }
}