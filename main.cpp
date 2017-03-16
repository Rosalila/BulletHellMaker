#include <stdio.h>
#include <stdlib.h>
#include <sstream>

#include <string>

#include "Rosalila/Rosalila.h"
#include "RosalilaShooter/Utility/Utility.h"
#include "RosalilaShooter/Menus/ControllerConfig.h"
#include "RosalilaShooter/Menus/Intro.h"
#include "RosalilaShooter/Menus/StageSelect.h"
#include "RosalilaShooter/Menus/StageModsSelect.h"

#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    bool input_config=false;
    if(argc>1 && strcmp(argv[1],"config")==0)
    {
        input_config=true;
    }

    rosalila()->init();
    initShooterUtility();

    rosalila()->sound->addSound("Menu.back",assets_directory+"menu/audio/back.ogg");
    rosalila()->sound->addSound("Menu.confirm",assets_directory+"menu/audio/confirm.ogg");
    rosalila()->sound->addSound("Menu.down",assets_directory+"menu/audio/down.ogg");
    rosalila()->sound->addSound("Menu.left",assets_directory+"menu/audio/left.ogg");
    rosalila()->sound->addSound("Menu.right",assets_directory+"menu/audio/right.ogg");
    rosalila()->sound->addSound("Menu.up",assets_directory+"menu/audio/up.ogg");
    rosalila()->sound->addSound("Menu.invalid",assets_directory+"menu/audio/invalid.ogg");

    rosalila()->api_integrator->setCurrentControllerActionSet("MenuControls");


    if(rosalila()->api_integrator->isUsingSteamController())
        rosalila()->api_integrator->showControllerBindingPanel();
    else
        rosalila()->receiver->controls = ControllerConfig(input_config);

    intro();

    stageSelect();

    return 0;
}
