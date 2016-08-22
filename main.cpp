#include <stdio.h>
#include <stdlib.h>
#include <sstream>

//SDL
#include <string>

#include "Rosalila/Rosalila.h"
#include "Rosalila/RosalilaInputs/RosalilaInputs.h"
#include "Rosalila/RosalilaGraphics/RosalilaGraphics.h"
#include "Rosalila/RosalilaSound/RosalilaSound.h"
#include "STGUtility/STGUtility.h"
#include "STGMenu/ControllerConfig.h"
#include "STGMenu/Intro.h"
#include "STGMenu/StageSelect.h"
#include "Rosalila/RosalilaUtility/RosalilaUtility.h"

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
