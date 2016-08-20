#ifndef STAGE_H
#define STAGE_H

#include "../Rosalila/TinyXml/tinyxml.h"
#include "../Rosalila/RosalilaGraphics/RosalilaGraphics.h"
#include "../Rosalila/RosalilaSound/RosalilaSound.h"
#include "../Character/Character.h"
#include "Layer.h"
#include "../STGUtility/STGUtility.h"
#include <map>

class Stage
{
public:
    std::vector<Layer*> back,front;
    std::string music_path;
    RosalilaGraphics* painter;
    Sound* sonido;
    Receiver*receiver;
    int bound_x1,bound_y1,bound_x2,bound_y2;
    int velocity;
    int iterator;
    string name;

    //slow extra control
    bool iterate_slowdown_flag;
    int current_slowdown_iteration;

    Stage(RosalilaGraphics* painter,Sound* sonido,Receiver*receiver);
    void dibujarBack();
    void dibujarFront();
    void drawLayer(Layer*layer);
    void loadFromXML(std::string name);
    int getBoundX1();
    int getBoundY1();
    int getBoundX2();
    int getBoundY2();
    int getVelocity();
    string getName();
    void setName(string name);
    void logic();
    void playMusic();
    ~Stage();
};

#endif
