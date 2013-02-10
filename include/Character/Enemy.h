#ifndef ENEMY_H
#define ENEMY_H

#include <map>
#include <list>
#include <vector>

#include "TinyXml/tinyxml.h"
#include "Painter/Painter.h"
#include "Sonido/Sonido.h"
#include "Input/Input.h"
#include "Spells/Bullet.h"
#include "Spells/Pattern.h"

class Enemy
{
    Sonido* sonido;
    Painter* painter;
    Receiver* receiver;
    int x,y;
    int velocity;
    bool shooting;
    std::string orientation;
    std::string current_type;

    //Sprites animation
    int animation_velocity;
    int animation_iteration;
    int current_sprite;
    std::map< std::string,std::vector<Image*> >sprites;

    //Patterns and bullets
    std::map<std::string,Bullet*> bullets;
    std::map<std::string, std::vector<Pattern*> > type;
    std::list<Pattern*>* active_patterns;

public:
    Enemy(Sonido* sonido,Painter* painter,Receiver* receiver,std::string directory);
    void logic(int stage_velocity);
    //logic sub functions
    void animationControl();
    void inputControl();
    void spellControl(int stage_velocity);

    void render();
    void setX(int x);
    void setY(int y);
    int getX();
    int getY();
    void setType(std::string type);
    std::list<Pattern*>* getActivePatterns();
};

#endif
