#include "../include/Character/Enemy.h"

Enemy::Enemy(Sonido* sonido,Painter* painter,Receiver* receiver,std::string directory)
{
    //Setting up the other variables
    this->sonido=sonido;
    this->painter=painter;
    this->receiver=receiver;
    this->active_patterns=new std::list<Pattern*>;
    this->x=900;
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

void Enemy::logic(int stage_velocity)
{
    animationControl();
    inputControl();
    spellControl(stage_velocity);
}

void Enemy::inputControl()
{
}

void Enemy::render()
{
    parrentRender();
    painter->drawRectangle(300,650,(500*hp)/max_hp,50,0,0,255,0,255,false);
}
