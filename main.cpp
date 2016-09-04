#include <stdio.h>
#include <stdlib.h>
#include <sstream>

#include <string>

#include "Rosalila/Rosalila.h"
#include "RosalilaShooter/Utility/Utility.h"
#include "RosalilaShooter/Menus/ControllerConfig.h"
#include "RosalilaShooter/Menus/Intro.h"
#include "RosalilaShooter/Menus/StageSelect.h"

#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    bool input_config=false;
    if(argc>1 && strcmp(argv[1],"config")==0)
    {
        input_config=true;
    }

    RosalilaInit();
    Rosalila()->Utility->clearLog();

//    Rosalila()->ApiIntegrator->unlockAchievement("papu");
//
//    Rosalila()->ApiIntegrator->setStat("statiti",10);
//
//    cout<<Rosalila()->ApiIntegrator->getStat("statiti")<<endl;
//
//    Rosalila()->ApiIntegrator->setScore("liditi", 666,"this is a replay",13);

    //Rosalila()->ApiIntegrator->storeData("miau","tus");

    //cout<<Rosalila()->ApiIntegrator->getData("miau")<<endl;


    map<string,Button*> controls = ControllerConfig(input_config);

    intro(controls);

    stageSelect(controls);

    return 0;
}
