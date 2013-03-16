#ifndef ENEMY_H
#define ENEMY_H

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
#include "Character/Player.h"

class Enemy : public Character
{
    std::map<int, vector<Modifier*>* >modifiers;
    float angle;
    float angle_change;
    Player*player;

    Image*life_bar;
public:
    Enemy(Sound* sonido,Painter* painter,Receiver* receiver,std::string directory,Player*player);
    void logic(int stage_velocity);
    void modifiersControl();
    void render();
    void loadModifiersFromXML(std::string directory);
    virtual void addActivePattern(Pattern* pattern);
};

#endif
