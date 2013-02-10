#include "../include/Spells/Pattern.h"

Pattern::Pattern(Sonido* sonido,Painter* painter,Receiver* receiver,int velocity,int max_velocity,int acceleration,int a_frequency,int angle,int angle_change,int stop_ac_at,int ac_frequency,int animation_velocity,Bullet* bullet,int offset_x,int offset_y,int startup,int cooldown,int duration,Hitbox*hitbox)
{
    this->sonido=sonido;
    this->painter=painter;
    this->receiver=receiver;

    this->hitbox=hitbox;
    this->velocity=velocity;
    this->max_velocity=max_velocity;
    this->acceleration=acceleration;
    this->a_frequency=a_frequency;
    this->current_a_frequency=0;
    this->angle=angle;
    this->angle_change=angle_change;
    this->stop_ac_at=stop_ac_at;
    this->current_stop_ac_at=0;
    this->ac_frequency=ac_frequency;
    this->current_ac_frequency=0;
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

    this->iteration=0;
    this->duration=duration;
}

Pattern::Pattern(Pattern*pattern,int x,int y)
{
    this->sonido=pattern->sonido;
    this->painter=pattern->painter;
    this->receiver=pattern->receiver;

    this->hitbox=pattern->hitbox;
    this->velocity=pattern->velocity;
    this->max_velocity=pattern->max_velocity;
    this->acceleration=pattern->acceleration;
    this->a_frequency=pattern->a_frequency;
    this->current_a_frequency=0;
    this->angle=pattern->angle;
    this->angle_change=pattern->angle_change;
    this->stop_ac_at=pattern->stop_ac_at;
    this->current_stop_ac_at=0;
    this->ac_frequency=pattern->ac_frequency;
    this->current_ac_frequency=0;
    this->x=x+pattern->offset_x;
    this->y=y-pattern->offset_y;
    this->animation_velocity=pattern->animation_velocity;
    this->bullet=pattern->bullet;

    this->iteration=0;
    this->duration=pattern->duration;

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

void Pattern::updateStateShouting()
{
    if(state=="startup")
    {
        if(current_startup<startup)
        {
            current_startup++;
        }else
        {
            current_startup=0;
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
            current_cooldown=0;
            state="ready";
        }
    }
}

void Pattern::updateStateNotShouting()
{
    current_startup=0;
    current_cooldown=0;
    state="startup";
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

    current_a_frequency++;
    if(current_a_frequency>a_frequency)
    {
        current_a_frequency=0;
        velocity+=acceleration;
        if(velocity>max_velocity)
            velocity=max_velocity;
    }

    current_stop_ac_at++;
    if(current_stop_ac_at<stop_ac_at&& stop_ac_at>0)
    {
        current_ac_frequency++;
        if(current_ac_frequency>ac_frequency)
        {
            current_ac_frequency=0;
            angle+=angle_change;
        }
    }

    iteration++;
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

    painter->drawRectangle(this->x+hitbox->getX()-hitbox->getWidth()/2,
                           this->y+hitbox->getY()-hitbox->getHeight()/2,
                           hitbox->getWidth(),hitbox->getHeight(),
                           angle+hitbox->getAngle(),100,0,0,100,true);
}

int Pattern::getX()
{
    return this->x;
}
int Pattern::getY()
{
    return this->y;
}

bool Pattern::destroyFlag()
{
    if(duration<0)
        return false;
    return iteration>duration;
}
