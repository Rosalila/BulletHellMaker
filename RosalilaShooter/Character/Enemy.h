#ifndef ENEMY_H
#define ENEMY_H

#include <map>
#include <list>
#include <vector>

#include "../../Rosalila/Rosalila.h"
#include "Bullet.h"
#include "Pattern.h"
#include "Character.h"
#include "Player.h"

class Enemy : public Character
{
public:
    Player*player;
    map<int, vector<Modifier*> >modifiers;
    double angle;
    double angle_change;
    bool is_mod;

    Enemy(string name,Player*player,int sound_channel_base,bool is_mod);
    ~Enemy();
    void loadModifiersFromXML();
    void logic(int stage_velocity, string stage_name);
    void modifiersControl();
    virtual void addActivePattern(Pattern* pattern);
    void bottomRender();
    void topRender();
};

#endif
