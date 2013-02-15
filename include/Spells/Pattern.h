#ifndef PATTERN_H
#define PATTERN_H

#include "TinyXml/tinyxml.h"
#include "Painter/Painter.h"
#include "Sonido/Sonido.h"
#include "Input/Input.h"
#include "Spells/Bullet.h"
#include "Spells/Hitbox.h"

#include <cmath>

#define PI 3.14159265

class Pattern
{
    Sonido* sonido;
    Painter* painter;
    Receiver* receiver;
    double x,y;
    int offset_x,offset_y;
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

    //Sprites animation
    int animation_velocity;
    int animation_iteration;
    int current_sprite;
    int startup,cooldown;
    int current_startup, current_cooldown;
    std::string state;
public:
    Pattern(Sonido* sonido,Painter* painter,Receiver* receiver,int velocity,int max_velocity,int acceleration,int a_frequency,float angle,int angle_change,int stop_ac_at,int ac_frequency,int animation_velocity,Bullet* bullet,int offset_x,int offset_y,int startup,int cooldown,int duration);
    Pattern(Pattern*pattern,int x,int y);
    void logic(int stage_speed);
    void render();

    int getX();
    int getY();
    float getAngle();
    Hitbox getHitbox();
    Bullet* getBullet();
    int getDamage();
    bool isReady();
    void setState(std::string state);
    void updateStateShouting();
    void updateStateNotShouting();
    bool destroyFlag();
};

#endif
