#ifndef STAGE_MODS_SELECT_H
#define STAGE_MODS_SELECT_H

#include "../Utility/Utility.h"
#include "../Character/Player.h"
#include "../Character/Enemy.h"
#include "../STG.h"
#include "../Stage/Stage.h"
#include <algorithm>
#include <time.h>

std::vector<std::string> getStageModsNames();
std::vector<Image*> getStageModsImages(std::vector<std::string> stage_names);
void stageModsSelect();

#endif
