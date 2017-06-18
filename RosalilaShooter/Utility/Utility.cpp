#include "Utility.h"

int iterate_slowdown_flag=0;
int current_slowdown_iteration=0;
bool slow_enabled=false;
bool game_over;
bool player_won;
bool is_first_win=false;

Image* loading_image;
Image* error_image;
Image* sucess_image;

vector<int>* color_palette_r;
vector<int>* color_palette_g;
vector<int>* color_palette_b;

void initShooterUtility()
{
    loading_image = rosalila()->graphics->getTexture(assets_directory+"misc/messages/loading.png");
    error_image = rosalila()->graphics->getTexture(assets_directory+"misc/messages/error.png");
    sucess_image = rosalila()->graphics->getTexture(assets_directory+"misc/messages/sucess.png");

    color_palette_r = new vector<int>();
    color_palette_g = new vector<int>();
    color_palette_b = new vector<int>();

    color_palette_r->push_back(244);
    color_palette_g->push_back(67);
    color_palette_b->push_back(54);

    color_palette_r->push_back(102);
    color_palette_g->push_back(58);
    color_palette_b->push_back(182);

    color_palette_r->push_back(3);
    color_palette_g->push_back(167);
    color_palette_b->push_back(244);

    color_palette_r->push_back(76);
    color_palette_g->push_back(175);
    color_palette_b->push_back(80);

    color_palette_r->push_back(255);
    color_palette_g->push_back(235);
    color_palette_b->push_back(59);

    color_palette_r->push_back(255);
    color_palette_g->push_back(87);
    color_palette_b->push_back(34);

    color_palette_r->push_back(233);
    color_palette_g->push_back(30);
    color_palette_b->push_back(99);

    color_palette_r->push_back(63);
    color_palette_g->push_back(81);
    color_palette_b->push_back(181);

    color_palette_r->push_back(0);
    color_palette_g->push_back(188);
    color_palette_b->push_back(212);

    color_palette_r->push_back(139);
    color_palette_g->push_back(195);
    color_palette_b->push_back(74);

    color_palette_r->push_back(255);
    color_palette_g->push_back(193);
    color_palette_b->push_back(7);

    color_palette_r->push_back(121);
    color_palette_g->push_back(85);
    color_palette_b->push_back(72);//Too brown?

    color_palette_r->push_back(156);
    color_palette_g->push_back(39);
    color_palette_b->push_back(176);

    color_palette_r->push_back(33);
    color_palette_g->push_back(150);
    color_palette_b->push_back(243);

    color_palette_r->push_back(0);
    color_palette_g->push_back(150);
    color_palette_b->push_back(136);

    color_palette_r->push_back(205);
    color_palette_g->push_back(220);
    color_palette_b->push_back(57);

    color_palette_r->push_back(255);
    color_palette_g->push_back(152);
    color_palette_b->push_back(0);
}

bool getGameOver()
{
    return game_over;
}

void setGameOver(bool game_over_param)
{
    game_over=game_over_param;
}

bool getPlayerWon()
{
    return player_won;
}

bool getIsFirstWin()
{
    return is_first_win;
}

void setPlayerWon(bool player_won_param)
{
    player_won=player_won_param;
}

void setIsFirstWin(bool is_first_win_param)
{
    is_first_win=is_first_win_param;
}

bool getIterateSlowdownFlag()
{
    return !isSlowActive() || iterate_slowdown_flag;
}

void slowExtraControl()
{
    //slow extra control
    if(isSlowActive())
    {
        iterate_slowdown_flag=false;
        current_slowdown_iteration++;
        if(current_slowdown_iteration>=3)
        {
            current_slowdown_iteration=0;
            iterate_slowdown_flag=true;
        }
    }
}

bool isSlowPressed()
{
    return rosalila()->receiver->isKeyDown(SDLK_x) || rosalila()->receiver->isJoyDown(5,0);
}

void disableSlow()
{
    slow_enabled=false;
}
void enableSlow()
{
    slow_enabled=true;
}
bool isSlowEnabled()
{
    return slow_enabled;
}
bool isSlowActive()
{
    return isSlowEnabled() && isSlowPressed();
}

double getSlowdown()
{
    int slowdown = 1.0;
    if(isSlowPressed() && isSlowEnabled())
        slowdown = 3.0;
    return slowdown;
}

Image* getLoadingImage()
{
    return loading_image;
}

Image* getErrorImage()
{
    return error_image;
}

Image* getSuccessImage()
{
    return sucess_image;
}

int getNotificationDuration()
{
    return 50;
}

vector<int>* getColorPaletteR()
{
    return color_palette_r;
}

vector<int>* getColorPaletteG()
{
    return color_palette_g;
}

vector<int>* getColorPaletteB()
{
    return color_palette_b;
}
