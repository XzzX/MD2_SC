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

#include <SFML/Graphics.hpp>

int main( unsigned int argc, char **argv ) {
	boost::timer::auto_cpu_timer autoTimer;

	gConfig.ReadCommandLineParameters(argc, argv);

    const Vector main_size(800,600);
    const Vector secondary_size(400,300);

    Window* pMainWindow;

    if (!gConfig.mNoGUI)
		pMainWindow = new Window("event driven molecular dynamics simulation", main_size, 10);

	std::cout << gConfig << std::endl;

	MDSystem	mdSystem;

	std::cout << "#Perkolation:\t" <<  mdSystem.IsPercolatic() << std::endl;

    if (!gConfig.mNoGUI)
        pMainWindow->SetCameraPosition(Vector(-gConfig.mBoxWidth*0.5, -gConfig.mBoxHeight*0.5, -gConfig.mBoxHeight*0.5));

	if (!gConfig.mNoGUI){

		pMainWindow->DrawRectangle(Vector(gConfig.mBoxWidth*0.5, gConfig.mBoxHeight*0.5, 0.0), gConfig.mBoxWidth, gConfig.mBoxHeight, gRed);

		for(unsigned int i=0; i < gConfig.mNumberOfParticles-1; i++){
			pMainWindow->DrawCircle(mdSystem.mParticleVector[i].mPosition, mdSystem.mParticleVector[i].mColor, mdSystem.mParticleVector[i].mRadius);
			if (mdSystem.mParticleVector[i].mPosition[0]<1) pMainWindow->DrawCircle(mdSystem.mParticleVector[i].mPosition+Vector(gConfig.mBoxWidth,0,0), mdSystem.mParticleVector[i].mColor, mdSystem.mParticleVector[i].mRadius);
			if (mdSystem.mParticleVector[i].mPosition[0]>gConfig.mBoxWidth-1) pMainWindow->DrawCircle(mdSystem.mParticleVector[i].mPosition+Vector(-gConfig.mBoxWidth,0,0), mdSystem.mParticleVector[i].mColor, mdSystem.mParticleVector[i].mRadius);
			if (mdSystem.mParticleVector[i].mPosition[1]<1) pMainWindow->DrawCircle(mdSystem.mParticleVector[i].mPosition+Vector(0,gConfig.mBoxHeight,0), mdSystem.mParticleVector[i].mColor, mdSystem.mParticleVector[i].mRadius);
			if (mdSystem.mParticleVector[i].mPosition[1]>gConfig.mBoxHeight-1) pMainWindow->DrawCircle(mdSystem.mParticleVector[i].mPosition+Vector(0,-gConfig.mBoxHeight,0), mdSystem.mParticleVector[i].mColor, mdSystem.mParticleVector[i].mRadius);
		}
	}

	MeanVar	meanRuntime;
	MeanVar	systemRuntime;

	boost::timer::cpu_timer timer;
	boost::timer::cpu_timer timer2;

	std::cout << "#starting simulation" << std::endl;

    while (gConfig.mRuns>0){
		timer.start();

        if (!gConfig.mNoGUI){
            //pMainWindow->Clear();
            pMainWindow->ProcessEvents();
        }

		timer2.start();
        mdSystem.MoveToNextEvent();
		timer2.stop();
		systemRuntime.Add(timer2.elapsed().user);

        if (!gConfig.mNoGUI){

			pMainWindow->DrawRectangle(Vector(gConfig.mBoxWidth*0.5, gConfig.mBoxHeight*0.5, 0.0), gConfig.mBoxWidth, gConfig.mBoxHeight, gRed);

			for (unsigned int i=gConfig.mNumberOfParticles; i<mdSystem.GetNumberOfParticles(); i++)
				pMainWindow->DrawCircle(mdSystem.mParticleVector[i].mPosition, mdSystem.mParticleVector[i].mColor, mdSystem.mParticleVector[i].mRadius);

            pMainWindow->Display();
        }

        timer.stop();
        meanRuntime.Add(timer.elapsed().user);
    }

	gConfig.SaveConfiguration();
	mdSystem.DumpData();

	//sf::Image Screen = pMainWindow->App->Capture();
	//Screen.SaveToFile((gConfig.mLogName+".jpg").c_str());

	std::cout << systemRuntime.Mean() << "\t" << systemRuntime.Error() << std::endl;
	std::cout << "#mean runtime per cycle: \t" << meanRuntime.Mean() << std::endl;



	std::cout << "Finished" << std::endl;
    return 0;
}
