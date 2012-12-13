#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>

#include <boost/timer/timer.hpp>

#include	"Window.hpp"
#include	"Vector.hpp"
#include	"Configuration.hpp"
#include	"Particle.hpp"
#include	"MeanVar.h"
#include	"util.h"
#include	"MDSystem.h"

int main( unsigned int argc, char **argv ) {
	Configuration	config;

    unsigned int i = 1;
    while (i<argc-1){
        std::string cmd = argv[i];

        if (cmd.compare("-N") == 0){
            config.mNumberOfParticles = StringTo<unsigned int>(argv[i+1]);
			i += 2;
        }
        else if (cmd.compare("-seed") == 0){
            config.mRandomSeed = StringTo<unsigned int>(argv[i+1]);
			i += 2;
		}
        else if (cmd.compare("-v") == 0){
            config.mParticleSpeed = StringTo<double>(argv[i+1]);
			i += 2;
        }
		else if (cmd.compare("-gui") == 0){
            if (StringTo<int>(argv[i+1])==0)
                config.mNoGUI = true;
			i += 2;
        }
        else if (cmd.compare("-log") == 0){
            config.mLogName = argv[i+1];
			i += 2;
        }
    }

    const Vector main_size(800,600);
    const Vector secondary_size(400,300);

    Window* pMainWindow;

    if (!config.mNoGUI)
        pMainWindow = new Window("event driven molecular dynamics simulation", main_size, 10);

    MDSystem	mdSystem(config);

    std::cout << config << std::endl;

    if (!config.mNoGUI)
        pMainWindow->SetCameraPosition(Vector(-config.mBoxWidth*0.5, -config.mBoxHeight*0.5, -config.mBoxHeight*0.5));

	MeanVar	meanRuntime;

	boost::timer::cpu_timer timer;


    while (pMainWindow->IsOpen()){
		timer.start();

        if (!config.mNoGUI){
            //if (pMainWindow->isOpen()) my_config.runs = 1;
            pMainWindow->Clear();
            pMainWindow->ProcessEvents();
        }

		//md_system.Observe();
        mdSystem.MoveToNextEvent();

        if (!config.mNoGUI){

			pMainWindow->DrawRectangle(Vector(config.mBoxWidth*0.5, config.mBoxHeight*0.5, 0.0), config.mBoxWidth, config.mBoxHeight, gRed);

            for(unsigned int i=0; i < mdSystem.GetNumberOfParticles(); i++)
                pMainWindow->DrawCircle(mdSystem.mParticleVector[i].mPosition, mdSystem.mParticleVector[i].mColor, mdSystem.mParticleVector[i].mRadius);

            pMainWindow->Display();
        }

        timer.stop();
        meanRuntime.Add(timer.elapsed().user);
    }

	std::cout << "#mean runtime per cycle: \t" << meanRuntime.Mean() << std::endl;

	std::cout << "Finished" << std::endl;
    return 0;
}
