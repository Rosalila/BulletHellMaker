#ifndef PLAYER_H
#define PLAYER_H

#include <map>
#include <list>
#include <vector>

#include "TinyXml/tinyxml.h"
#include "RosalilaGraphics/RosalilaGraphics.h"
#include "RosalilaSound/RosalilaSound.h"
#include "RosalilaInputs/RosalilaInputs.h"
#include "../Spells/Bullet.h"
#include "../Spells/Pattern.h"
#include "Character.h"

class Player:public Character
{
    Image*life_bar;
public:
    Player(Sound* sonido,RosalilaGraphics* painter,Receiver* receiver,std::string name);
    void logic(int stage_velocity);
    void inputControl();
    void render();
};

#endif
