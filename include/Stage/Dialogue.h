#ifndef DIALOGUE_H
#define DIALOGUE_H

#include "TinyXml/tinyxml.h"
#include "Painter/Painter.h"
#include "RosalilaSound/RosalilaSound.h"
#include "Input/Input.h"

class Dialogue
{
private:
    Painter* painter;
    Sound* sonido;
    Receiver*receiver;
    std::string text;
    int iterator;
    int duration;
    Image*image;
public:
    Dialogue(Painter* painter,Sound* sonido,Receiver*receiver,std::string text,Image*image);
    std::string getText();
    void render(int x,int y);
    void logic();
    bool destroyFlag();
    ~Dialogue();
};

#endif
