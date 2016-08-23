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
    RosalilaInit();
    Rosalila()->Utility->clearLog();

    map<string,Button*> controls = ControllerConfig();
    intro(controls);
    stageSelect(controls);

    return 0;
}
