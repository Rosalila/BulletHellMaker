#ifndef BULLET_H
#define BULLET_H

#include "TinyXml/tinyxml.h"
#include "Painter/Painter.h"
#include "Sonido/Sonido.h"
#include "Input/Input.h"

class Bullet
{
    Sonido* sonido;
    Painter* painter;
    Receiver* receiver;
    vector<Image*>sprites;
public:
    Bullet(){};
    Bullet(Sonido* sonido,Painter* painter,Receiver* receiver,vector<Image*>sprites);
    void logic();
    void render();

    Image*getImage(int position);
    int spritesSize();
};

#endif
