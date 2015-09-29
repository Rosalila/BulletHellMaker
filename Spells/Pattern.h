#ifndef PATTERN_H
#define PATTERN_H

#include "../Rosalila/TinyXml/tinyxml.h"
#include "../Rosalila/RosalilaGraphics/RosalilaGraphics.h"
#include "../Rosalila/RosalilaSound/RosalilaSound.h"
#include "../Rosalila/RosalilaInputs/RosalilaInputs.h"
#include "Bullet.h"
#include "../Rosalila/RosalilaUtility/Hitbox.h"
#include "../Character/Modifier.h"
#include "../STGUtility/STGUtility.h"

#include <cmath>

#define PI 3.14159265

class Pattern
{
    Sound* sonido;
    RosalilaGraphics* painter;
    Receiver* receiver;
    double x,y;
public:
    int velocity;
    int max_velocity;
    int acceleration;
    int a_frequency;
    int current_a_frequency;
    float angle;
    int angle_change;
    int stop_ac_at;
    int current_stop_ac_at;
    int ac_frequency;
    int current_ac_frequency;
    Bullet* bullet;
    int iteration;
    int duration;
    int random_angle;
    bool aim_player;
    bool freeze;
    bool homing;

    //Sprites animation
    int animation_velocity;
    int animation_iteration;
    int current_sprite;
    int startup,cooldown;
    int current_startup, current_cooldown;
    std::string state;
    bool is_hit;
    bool delete_flag;

    //Bullet rotations
    float bullet_rotation;
    float br_change;
    bool independent_br;

    //Bullets pointer
    std::map<std::string,Bullet*> *bullets;

    //Modifiers
    std::map<int, vector<Modifier*>* >*modifiers;
    void modifiersControl();

public:
int offset_x,offset_y;
    Pattern(Sound* sonido,RosalilaGraphics* painter,Receiver* receiver,int velocity,int max_velocity,int acceleration,int a_frequency,float angle,int angle_change,int stop_ac_at,int ac_frequency,int animation_velocity,
            Bullet* bullet,int offset_x,int offset_y,int startup,int cooldown,int duration,int random_angle,bool aim_player,int bullet_rotation,int br_change,int independent_br,bool freeze, bool homing, std::map<int, vector<Modifier*>* >*modifiers,std::map<std::string,Bullet*> *bullets);
    ~Pattern();
    Pattern(Pattern*pattern,int x,int y);
    void logic(int stage_speed);
    void render();

    int getX();
    int getY();
    float getAngle();
    float getBulletAngle();
    float getRandomAngle();
    bool getAimPlayer();
    void aimTo(int x,int y);
    Bullet* getBullet();
    int getDamage();
    bool isReady();
    void setAngle(float angle);
    void setState(std::string state);
    void updateStateShouting();
    void updateStateNotShouting();
    bool destroyFlag();
    void hit();
    bool isHit();
    int getHoming();
};

#endif
