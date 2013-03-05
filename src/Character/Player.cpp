#include "../include/Character/Player.h"

Player::Player(Sonido* sonido,Painter* painter,Receiver* receiver,std::string directory)
{
    //Setting up the other variables
    this->sonido=sonido;
    this->painter=painter;
    this->receiver=receiver;
    this->active_patterns=new std::list<Pattern*>;
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

    if(receiver->IsKeyDownn(SDLK_DOWN)
       || receiver->IsJoyDown(-2,0))
        this->y+=velocity;
    if(receiver->IsKeyDownn(SDLK_UP)
       || receiver->IsJoyDown(-8,0))
        this->y-=velocity;
    if(receiver->IsKeyDownn(SDLK_LEFT)
       || receiver->IsJoyDown(-4,0))
        this->x-=velocity;
    if(receiver->IsKeyDownn(SDLK_RIGHT)
       || receiver->IsJoyDown(-6,0))
        this->x+=velocity;

    if(receiver->IsKeyDownn(SDLK_z)
       || receiver->IsJoyDown(0,0))
        this->shooting=true;
    else
        this->shooting=false;
}

void Player::logic(int stage_velocity)
{
    animationControl();
    if(this->hp!=0)
    {
        inputControl();
    }
    spellControl(stage_velocity);
}

void Player::render()
{
    painter->drawRectangle(70,35,(370*hp)/max_hp,30,0,0,255,0,255,false);
    parrentRender();
        painter->drawRectangle(this->getHitbox().getX(),
                               this->getHitbox().getY(),
                               hitbox.getWidth(),hitbox.getHeight(),
                               hitbox.getAngle(),100,0,0,100,true);
}
