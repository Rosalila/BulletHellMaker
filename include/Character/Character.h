#ifndef CHARACTER_H
#define CHARACTER_H

#include <map>
#include <list>
#include <vector>

#include "TinyXml/tinyxml.h"
#include "Painter/Painter.h"
#include "Sonido/Sonido.h"
#include "Input/Input.h"
#include "Spells/Bullet.h"
#include "Spells/Pattern.h"
#include "Utility/Utility.h"
#include "Character/VariableChange.h"

class Character
{
protected:
    Sonido* sonido;
    Painter* painter;
    Receiver* receiver;
    int hp;
    int max_hp;
    int iteration;
    float x,y;
    int velocity;
    Hitbox hitbox;
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
    Character(){}
    Character(Sonido* sonido,Painter* painter,Receiver* receiver,std::string directory);
    void loadFromXML(std::string directory);
    void logic(int stage_velocity);
    //logic sub functions
    void animationControl();
    void spellControl(int stage_velocity);

    void parrentRender();
    void render();
    void setX(int x);
    void setY(int y);
    int getX();
    int getY();
    int getHP();
    Hitbox getHitbox();
    void setType(std::string type);
    std::list<Pattern*>* getActivePatterns();
    bool collides(Hitbox hitbox,int hitbox_x,int hitbox_y,float hitbox_angle);
    void hit(int damage);
};

#endif
