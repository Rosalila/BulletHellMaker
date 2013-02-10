#include "../include/Character/Player.h"

Player::Player(Sonido* sonido,Painter* painter,Receiver* receiver,std::string directory)
{
    //Setting up the other variables
    this->sonido=sonido;
    this->painter=painter;
    this->receiver=receiver;
    this->active_patterns=new std::list<Pattern*>;
    this->x=100;
    this->y=500;
    this->shooting=true;
    this->orientation="idle";
    this->current_type="1";

    //Sprites animation
    this->animation_velocity=4;
    this->animation_iteration=0;
    this->current_sprite=0;

    loadFromXML(directory);
}

void Player::inputControl()
{
    if(receiver->IsKeyDownn(SDLK_DOWN))
        orientation="down";
    else if(receiver->IsKeyDownn(SDLK_UP))
        orientation="up";
    else if(receiver->IsKeyDownn(SDLK_LEFT))
        orientation="left";
    else if(receiver->IsKeyDownn(SDLK_RIGHT))
        orientation="right";
    else
        orientation="idle";

    if(receiver->IsKeyDownn(SDLK_DOWN))
        this->y+=velocity;
    if(receiver->IsKeyDownn(SDLK_UP))
        this->y-=velocity;
    if(receiver->IsKeyDownn(SDLK_LEFT))
        this->x-=velocity;
    if(receiver->IsKeyDownn(SDLK_RIGHT))
        this->x+=velocity;

    if(receiver->IsKeyDownn(SDLK_z))
        this->shooting=true;
    else
        this->shooting=false;
}

void Player::logic(int stage_velocity)
{
    animationControl();
    inputControl();
    spellControl(stage_velocity);
}
