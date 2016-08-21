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
    RosalilaGraphics* painter;
    RosalilaSound* sonido;
    Receiver*receiver;
    std::vector<Layer*> back,front;
    std::string music_path;
    int bound_x1,bound_y1,bound_x2,bound_y2;
    int velocity;
    int iterator;
    string name;
    bool iterate_slowdown_flag;
    int current_slowdown_iteration;

    Stage(RosalilaGraphics* painter,RosalilaSound* sonido,Receiver*receiver);
    void dibujarBack();
    void dibujarFront();
    void drawLayer(Layer*layer);
    void loadFromXML(std::string name);
    void logic();
    void playMusic();
    ~Stage();
};

#endif
