#include "../include/Spells/Pattern.h"

Pattern::Pattern(Sonido* sonido,Painter* painter,Receiver* receiver,int velocity,int angle,int animation_velocity,Bullet* bullet,int offset_x,int offset_y,int startup,int cooldown)
{
    this->sonido=sonido;
    this->painter=painter;
    this->receiver=receiver;

    this->velocity=velocity;
    this->angle=angle;
    this->offset_x=offset_x;
    this->offset_y=offset_y;
    this->startup=startup;
    this->cooldown=cooldown;
    this->x=0;
    this->y=0;
    this->animation_velocity=animation_velocity;
    this->bullet=bullet;

    //Sprites animation
    this->animation_iteration=0;
    this->current_sprite=0;
    this->state="startup";
    this->current_startup=0;
    this->current_cooldown=0;
}

Pattern::Pattern(Pattern*pattern,int x,int y)
{
    this->sonido=pattern->sonido;
    this->painter=pattern->painter;
    this->receiver=pattern->receiver;

    this->velocity=pattern->velocity;
    this->angle=pattern->angle;
    this->x=x+pattern->offset_x;
    this->y=y-pattern->offset_y;
    this->animation_velocity=pattern->animation_velocity;
    this->bullet=pattern->bullet;

    //Sprites animation
    this->animation_iteration=0;
    this->current_sprite=0;
}

bool Pattern::isReady()
{
    if(state=="ready")
    {
        state="cooldown";
        current_startup=0;
        current_cooldown=0;
        return true;
    }
    return false;
}

void Pattern::setState(std::string state)
{
    this->state=state;
}

void Pattern::updateState()
{
    if(state=="startup")
    {
        if(current_startup<startup)
        {
            current_startup++;
        }else
        {
            state="ready";
        }
    }

    if(state=="cooldown")
    {
        if(current_cooldown<cooldown)
        {
            current_cooldown++;
        }else
        {
            state="startup";
        }
    }
}


void Pattern::logic(int stage_speed)
{
    this->x += cos (angle*PI/180) * velocity + stage_speed;
    this->y -= sin (angle*PI/180) * velocity;

    if(animation_iteration>=animation_velocity)
    {
        current_sprite++;
        if(current_sprite>=bullet->spritesSize())
            current_sprite=0;

        animation_iteration=0;
    }
    animation_iteration++;
}

void Pattern::render()
{
    Image*image=bullet->getImage(current_sprite);
    painter->draw2DImage
    (   image,
        image->getWidth(),image->getHeight(),
        this->x-image->getWidth()/2,this->y-image->getHeight()/2,
        1.0,
        (float)angle,
        false,
        0,0,
        Color(255,255,255,255),
        true);
}

int Pattern::getX()
{
    return this->x;
}
int Pattern::getY()
{
    return this->y;
}
