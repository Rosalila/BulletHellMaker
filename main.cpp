#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include <stdio.h>
#include <stdlib.h>
#include <sstream>

#include <string>

#include "Rosalila/Rosalila.h"
#include "Utility/Utility.h"
#include "Menus/Intro.h"
#include "Menus/StageSelect.h"
#include "Menus/StageModsSelect.h"

#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    std::cout<<"A"<<std::endl;
    bool input_config=false;
    if(argc>1 && strcmp(argv[1],"config")==0)
    {
        input_config=true;
    }
    std::cout<<"B"<<assets_directory<<std::endl;

    std::string config_file_path = std::string(assets_directory)+"config.json";
    std::cout<<"CP:"<<config_file_path<<std::endl;
    rosalila()->init(config_file_path);
    initShooterUtility();
    std::cout<<"C"<<std::endl;

    rosalila()->sound->addSound("Menu.back",std::string(assets_directory)+"menu/audio/back.ogg");
    rosalila()->sound->addSound("Menu.confirm",std::string(assets_directory)+"menu/audio/confirm.ogg");
    rosalila()->sound->addSound("Menu.down",std::string(assets_directory)+"menu/audio/down.ogg");
    rosalila()->sound->addSound("Menu.left",std::string(assets_directory)+"menu/audio/left.ogg");
    rosalila()->sound->addSound("Menu.right",std::string(assets_directory)+"menu/audio/right.ogg");
    rosalila()->sound->addSound("Menu.up",std::string(assets_directory)+"menu/audio/up.ogg");
    rosalila()->sound->addSound("Menu.invalid",std::string(assets_directory)+"menu/audio/invalid.ogg");

    rosalila()->api_integrator->setCurrentControllerActionSet("MenuControls");
    std::cout<<"D"<<std::endl;

    intro();

    stageSelect();

    return 0;
}
