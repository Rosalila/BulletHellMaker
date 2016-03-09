#ifndef PLAYER_H
#define PLAYER_H

#include <map>
#include <list>
#include <vector>

#include "../Rosalila/TinyXml/tinyxml.h"
#include "../Rosalila/RosalilaGraphics/RosalilaGraphics.h"
#include "../Rosalila/RosalilaSound/RosalilaSound.h"
#include "../Rosalila/RosalilaInputs/RosalilaInputs.h"
#include "../Spells/Bullet.h"
#include "../Spells/Pattern.h"
#include "Character.h"

class Player:public Character
{
    Image*life_bar;

    //Slow bar variables
    Image*slow_bar;
    int slow_decrement;
    int slow_increment;
    int slow_cooldown_increment;
    bool slow_in_cooldown;
    int slow_bar_x;
    int slow_bar_y;
    int slow_bar_rect_offset_x;
    int slow_bar_rect_offset_y;
    int slow_bar_rect_height;
    int slow_bar_rect_width;
    Color slow_bar_color;
    Color slow_bar_cooldown_color;

    //Shield
    int current_shield;
    int max_shield;
    int shield_fade;
    double proration;
    Image* shield_image;
    double getShieldPercentage();

    //Charge
    int current_charge;
    int max_charge;
    int charge_velocity;
    int charge_sprite_x;
    int charge_sprite_y;
    Image* charge_image;

    //Parry
    int current_parry_frame;
    int parry_duration;
public:
    Player(Sound* sonido,RosalilaGraphics* painter,Receiver* receiver,std::string name,int sound_channel_base);
    void logic(int stage_velocity);
    void inputControl();
    void bottomRender();
    void topRender();
    void loadPlayerFromXML();
    void hit(int damage);
    void loadFromXML();
    bool isParrying();
};

#endif
