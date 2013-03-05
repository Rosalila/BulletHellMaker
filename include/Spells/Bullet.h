#ifndef BULLET_H
#define BULLET_H

#include "TinyXml/tinyxml.h"
#include "Painter/Painter.h"
#include "Sonido/Sonido.h"
#include "Input/Input.h"
#include "Spells/Hitbox.h"

class Bullet
{
    Sonido* sonido;
    Painter* painter;
    Receiver* receiver;
    vector<Image*>sprites;
    vector<Image*>sprites_on_hit;
    Hitbox hitbox;
    int damage;
    std::string name;
    bool has_sound;
public:
    Bullet(){};
    Bullet(Sonido* sonido,Painter* painter,Receiver* receiver,std::string name,vector<Image*>sprites,vector<Image*>sprites_on_hit,Hitbox hitbox,int damage,bool has_sound);
    void logic();
    void render();
    Hitbox getHitbox();
    int getDamage();
    void playSound();
    Image*getImage(int position);
    Image*getOnHitImage(int position);
    int spritesSize();
    int spritesOnHitSize();
};

#endif
