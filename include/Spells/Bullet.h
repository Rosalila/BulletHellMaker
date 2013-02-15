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
    Hitbox hitbox;
    int damage;
public:
    Bullet(){};
    Bullet(Sonido* sonido,Painter* painter,Receiver* receiver,vector<Image*>sprites,Hitbox hitbox,int damage);
    void logic();
    void render();
    Hitbox getHitbox();
    int getDamage();

    Image*getImage(int position);
    int spritesSize();
};

#endif
