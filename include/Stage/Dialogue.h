#ifndef DIALOGUE_H
#define DIALOGUE_H

#include "TinyXml/tinyxml.h"
#include "Painter/Painter.h"
#include "Sonido/Sonido.h"
#include "Input/Input.h"

class Dialogue
{
private:
    Painter* painter;
    Sonido* sonido;
    Receiver*receiver;
    std::string text;
    int iterator;
    int duration;
    Image*image;
public:
    Dialogue(Painter* painter,Sonido* sonido,Receiver*receiver,std::string text,Image*image);
    std::string getText();
    void render(int x,int y);
    void logic();
    bool destroyFlag();
    ~Dialogue();
};

#endif
