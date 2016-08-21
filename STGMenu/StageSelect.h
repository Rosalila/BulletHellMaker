#ifndef STAGE_SELECT_H
#define STAGE_SELECT_H

#include "../STGUtility/STGUtility.h"
#include "../Character/Player.h"
#include "../Character/Enemy.h"
#include "../STG.h"
#include "../Stage/Stage.h"

std::vector<std::string> getStageNames();
std::vector<Image*> getStageImages(std::vector<std::string> stage_names);
void stageSelect(map<string,Button*> controls);

#endif
