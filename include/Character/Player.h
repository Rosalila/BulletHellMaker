#ifndef PLAYER_H
#define PLAYER_H

#include <map>
#include <list>
#include <vector>

#include "TinyXml/tinyxml.h"
#include "Painter/Painter.h"
#include "RosalilaSound/RosalilaSound.h"
#include "Input/Input.h"
#include "Spells/Bullet.h"
#include "Spells/Pattern.h"
#include "Character/Character.h"

class Player:public Character
{
    Image*life_bar;
public:
    Player(Sound* sonido,Painter* painter,Receiver* receiver,std::string directory);
    void logic(int stage_velocity);
    void inputControl();
    void render();
};

#endif
