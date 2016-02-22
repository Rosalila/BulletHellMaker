#ifndef STG_H
#define STG_H

#include "Rosalila/TinyXml/tinyxml.h"
#include "Rosalila/RosalilaGraphics/RosalilaGraphics.h"
#include "Rosalila/RosalilaSound/RosalilaSound.h"
#include "Rosalila/RosalilaInputs/RosalilaInputs.h"
#include "Character/Character.h"
#include "Character/Player.h"
#include "Character/Enemy.h"
#include "Stage/Stage.h"
#include "Rosalila/RosalilaGraphics/Animation.h"

class STG
{
    Sound* sonido;
    RosalilaGraphics* painter;
    Receiver* receiver;
    Player*player;
    Enemy*enemy;
    Stage*stage;

    string game_mode;

    string username;

    Animation you_win,you_loose;
    int iteration;
    int boss_fury_level;


    bool isOutOfBounds(int pos_x,int pos_y);
    void mainLoop();
    void logic();
    void render();
    void deletePatterns();
    void checkCharacterOutOfBounds();
public:
    bool playerWon();
    bool enemyWon();
    void stageSelectModeInit();
    STG(Sound* sonido,RosalilaGraphics* painter,Receiver* receiver,Player*player,Enemy*enemy,Stage*stage,string game_mode);
};

#endif
