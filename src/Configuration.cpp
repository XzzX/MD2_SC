#include	"Configuration.hpp"

#include	<fstream>

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
	stream << "#box length:          " << config.mL << std::endl;
	stream << "#obstacle radius:     " << config.mRadius << std::endl;
	stream << "#test particle radius:" << config.mSigma << std::endl;
	stream << "#number of test part: " << config.mNumberOfTestParticles << std::endl;
	stream << "#lattice type:        " << config.mLatticeType << std::endl;
	stream << "#particle speed:		 " << config.mParticleSpeed << std::endl;
	stream << "#runs:                " << config.mRuns << std::endl;
	stream << "#nogui:               " << config.mNoGUI << std::endl;
	stream << "#logname:             " << config.mLogName << std::endl;
    return stream;
}

Configuration::Configuration() :
	mRandomSeed(13579059208),
	mNumberOfParticles(250),
	mLatticeSpace(3.0),
	mAxialRatio(1.0),
	mBoxWidth(50.0),
	mBoxHeight(50.0),
	mL(50.0),
	mRadius(1.0),
	mSigma(0.1),
	mNumberOfTestParticles(100),
	mLatticeType(randomL),
	mParticleSpeed(2.0),
	mRuns(3000),
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
		else if (cmd.compare("-L") == 0){
			i++;
            mL = StringTo<double>(argv[i]);
		}
		else if (cmd.compare("-radius") == 0){
			i++;
            mRadius = StringTo<double>(argv[i]);
		}
		else if (cmd.compare("-sigma") == 0){
			i++;
            mSigma = StringTo<double>(argv[i]);
		}
		else if (cmd.compare("-Ntest") == 0){
			i++;
            mNumberOfTestParticles = StringTo<double>(argv[i]);
        }
    }
}

void Configuration::SaveConfiguration(){
    std::fstream fout((gConfig.mLogName+"_conf.txt").c_str(), std::fstream::out);
	fout << gConfig;

	fout << mRandomSeed << std::endl;
	fout << mNumberOfParticles << std::endl;
	fout << mLatticeSpace << std::endl;
	fout << mAxialRatio << std::endl;
	fout << mBoxWidth << std::endl;
	fout << mBoxHeight << std::endl;
	fout << mL << std::endl;
	fout << mRadius << std::endl;
	fout << mSigma << std::endl;
	fout << mNumberOfTestParticles << std::endl;
	//fout << mLatticeType << std::endl;
	fout << mParticleSpeed << std::endl;
	fout << mRuns << std::endl;
	fout << mNoGUI << std::endl;
	//fout << mLogName << std::endl;

    fout.close();
}
