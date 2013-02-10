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
public:
    Dialogue(Painter* painter,Sonido* sonido,Receiver*receiver,std::string text);
    std::string getText();
    void render();
    void logic();
    bool destroyFlag();
    ~Dialogue();
};

#endif
