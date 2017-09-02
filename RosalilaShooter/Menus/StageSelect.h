#ifndef STAGE_SELECT_H
#define STAGE_SELECT_H

#include "../Utility/Utility.h"
#include "../Character/Player.h"
#include "../Character/Enemy.h"
#include "../STG.h"
#include "../Stage/Stage.h"
#include "StageModsSelect.h"
#ifdef SECRET
#include "StageSecretSelect.h"
#endif
#include <algorithm>
#include <time.h>

std::vector<std::string> getStageNames();
std::vector<Image*> getStageImages(std::vector<std::string> stage_names);
void stageSelect();

#endif
