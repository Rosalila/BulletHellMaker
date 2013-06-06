#ifndef STG_H
#define STG_H

#include "TinyXml/tinyxml.h"
#include "RosalilaGraphics/RosalilaGraphics.h"
#include "RosalilaSound/RosalilaSound.h"
#include "RosalilaInputs/RosalilaInputs.h"
#include "Character/Character.h"
#include "Character/Player.h"
#include "Character/Enemy.h"
#include "Stage/Stage.h"
#include "RosalilaGraphics/Animation.h"

class STG
{
    Sound* sonido;
    RosalilaGraphics* painter;
    Receiver* receiver;
    Player*player;
    Enemy*enemy;
    Stage*stage;

    string username;

    Animation you_win,you_loose;
    int iteration;

    bool isOutOfBounds(int pos_x,int pos_y);
    void mainLoop();
    void logic();
    void render();
    void deletePatterns(int stage_bound_x1,int stage_bound_y1,int stage_bound_x2,int stage_bound_y2);
    void checkCharacterOutOfBounds();
public:
    STG(Sound* sonido,RosalilaGraphics* painter,Receiver* receiver,Player*player,Enemy*enemy,Stage*stage);
};

#endif
