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
    initShooterUtility();

    Rosalila()->ApiIntegrator->unlockAchievement("A");

    Rosalila()->ApiIntegrator->setCurrentControllerActionSet("MenuControls");


    if(!Rosalila()->ApiIntegrator->isUsingSteamController())
        Rosalila()->Receiver->controls = ControllerConfig(input_config);

    intro();

    stageSelect();

    return 0;
}
