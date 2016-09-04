#ifndef CONTROLLERCONFIG_H
#define CONTROLLERCONFIG_H

#include <map>
#include "../Utility/Utility.h"
#include "../../Rosalila/RosalilaInputs/Button.h"

using namespace std;

map<string,Button*> ControllerConfig(bool reconfigure);

#endif
