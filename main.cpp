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
    clearLog();

    RosalilaInit();

    string path_menu = assets_directory+"menu/main_menu.svg";
    map<string,Button*> controls = ControllerConfig();
    intro(controls);
    stageSelect(controls);

    return 0;
}
