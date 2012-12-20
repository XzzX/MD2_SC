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
	gConfig.ReadCommandLineParameters(argc, argv);

    const Vector main_size(800,600);
    const Vector secondary_size(400,300);

    Window* pMainWindow;

    if (!gConfig.mNoGUI)
        pMainWindow = new Window("event driven molecular dynamics simulation", main_size, 10);

    MDSystem	mdSystem;

    std::cout << gConfig << std::endl;

    if (!gConfig.mNoGUI)
        pMainWindow->SetCameraPosition(Vector(-gConfig.mBoxWidth*0.5, -gConfig.mBoxHeight*0.5, -gConfig.mBoxHeight*0.5));

	MeanVar	meanRuntime;
	MeanVar	systemRuntime;

	boost::timer::cpu_timer timer;
	boost::timer::cpu_timer timer2;

    while (gConfig.mRuns>0){
		gConfig.mRuns--;
		timer.start();

        if (!gConfig.mNoGUI){
            if (pMainWindow->IsOpen()) gConfig.mRuns = 1;
            pMainWindow->Clear();
            pMainWindow->ProcessEvents();
        }

		//md_system.Observe();
		timer2.start();
        mdSystem.MoveToNextEvent();
		timer2.stop();
		systemRuntime.Add(timer2.elapsed().user);

        if (!gConfig.mNoGUI){

			pMainWindow->DrawRectangle(Vector(gConfig.mBoxWidth*0.5, gConfig.mBoxHeight*0.5, 0.0), gConfig.mBoxWidth, gConfig.mBoxHeight, gRed);

            for(unsigned int i=0; i < mdSystem.GetNumberOfParticles(); i++)
                pMainWindow->DrawCircle(mdSystem.mParticleVector[i].mPosition, mdSystem.mParticleVector[i].mColor, mdSystem.mParticleVector[i].mRadius);

            pMainWindow->Display();
        }

        timer.stop();
        meanRuntime.Add(timer.elapsed().user);
    }
	
	std::cout << systemRuntime.Mean() << "\t" << systemRuntime.Error() << std::endl;
	std::cout << "#mean runtime per cycle: \t" << meanRuntime.Mean() << std::endl;

	std::cout << "Finished" << std::endl;
    return 0;
}
