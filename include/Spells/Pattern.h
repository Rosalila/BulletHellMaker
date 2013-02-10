#ifndef PATTERN_H
#define PATTERN_H

#include "TinyXml/tinyxml.h"
#include "Painter/Painter.h"
#include "Sonido/Sonido.h"
#include "Input/Input.h"
#include "Spells/Bullet.h"

#include <cmath>

#define PI 3.14159265

class Pattern
{
    Sonido* sonido;
    Painter* painter;
    Receiver* receiver;
    int x,y;
    int offset_x,offset_y;
    int velocity;
    int angle;
    Bullet* bullet;

    //Sprites animation
    int animation_velocity;
    int animation_iteration;
    int current_sprite;
    int startup,cooldown;
    int current_startup, current_cooldown;
    std::string state;
public:
    Pattern(Sonido* sonido,Painter* painter,Receiver* receiver,int velocity,int angle,int animation_velocity,Bullet* bullet,int offset_x,int offset_y,int startup,int cooldown);
    Pattern(Pattern*pattern,int x,int y);
    void logic(int stage_speed);
    void render();

    int getX();
    int getY();
    bool isReady();
    void setState(std::string state);
    void updateState();
};

#endif
