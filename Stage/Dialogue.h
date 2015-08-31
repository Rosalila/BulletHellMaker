#ifndef DIALOGUE_H
#define DIALOGUE_H

#include "../Rosalila/TinyXml/tinyxml.h"
#include "../Rosalila/RosalilaGraphics/RosalilaGraphics.h"
#include "../Rosalila/RosalilaSound/RosalilaSound.h"
#include "../Rosalila/RosalilaInputs/RosalilaInputs.h"

class Dialogue
{
private:
    RosalilaGraphics* painter;
    Sound* sonido;
    Receiver*receiver;
    std::string text;
    int iterator;
    int duration;
    Image*image;
public:
    Dialogue(RosalilaGraphics* painter,Sound* sonido,Receiver*receiver,std::string text,Image*image);
    std::string getText();
    void render(int x,int y);
    void logic();
    bool destroyFlag();
    ~Dialogue();
};

#endif
