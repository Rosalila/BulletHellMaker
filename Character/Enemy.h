#ifndef ENEMY_H
#define ENEMY_H

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
#include "Player.h"

class Enemy : public Character
{
    std::map<int, vector<Modifier*>* >modifiers;
    double angle;
    double angle_change;
    Player*player;
    Image*life_bar;

    string score_upload_message;
    bool flag_begin_upload;

public:
    Enemy(Sound* sonido,RosalilaGraphics* painter,Receiver* receiver,std::string name,Player*player);
    void logic(int stage_velocity, string stage_name, int global_iteration, string username);
    void modifiersControl();
    void render();
    void loadModifiersFromXML();
    virtual void addActivePattern(Pattern* pattern);
    void aimPlayer(Pattern* pattern);
};

#endif
