#ifndef STG_H
#define STG_H

#include "Rosalila/RosalilaGraphics/RosalilaGraphics.h"
#include "Rosalila/RosalilaSound/RosalilaSound.h"
#include "Rosalila/RosalilaInputs/RosalilaInputs.h"
#include "Character/Character.h"
#include "Character/Player.h"
#include "Character/Enemy.h"
#include "Stage/Stage.h"

class STG
{
public:
    Player*player;
    Enemy*enemy;
    Stage*stage;

    string game_mode;
    
    int frame;

    //Image*image_upload_error;
    Image* you_win;
    Image* you_lose;

    int initial_ticks;

    int current_player_best_score;

    string api_state;

    int game_over_timeout;

    bool player_is_immortal;

    STG(Player*player,Enemy*enemy,Stage*stage,string game_mode,int current_player_best_score);
    ~STG();
    bool isOutOfBounds(int pos_x,int pos_y);
    void mainLoop();
    bool logic();
    void render();
    void deletePatterns();
    bool playerWon();
    bool enemyWon();
    void onWin();
    void lose();
    void uploadScore();
    void uploadReplay();
    bool uploadErrorLoop();
    void findLeaderboard();
};

#endif
