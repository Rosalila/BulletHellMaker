#include "../include/Spells/Pattern.h"

Pattern::Pattern(Sonido* sonido,Painter* painter,Receiver* receiver,int velocity,int max_velocity,int acceleration,int a_frequency,float angle,int angle_change,int stop_ac_at,int ac_frequency,int animation_velocity,Bullet* bullet,int offset_x,int offset_y,int startup,int cooldown,int duration)
{
    this->sonido=sonido;
    this->painter=painter;
    this->receiver=receiver;

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


//    //Move the universe (the point b)
//    float s = sin((angle+this->getHitbox().getAngle())*PI/180);
//    float c = cos((angle+this->getHitbox().getAngle())*PI/180);
//
//    // translate point back to origin:
//    int res_x = this->getHitbox().getX();
//    int res_y = this->getHitbox().getY();
//
//    // rotate point
//    float xnew = res_x * c + res_y * s;
//    float ynew = -res_x * s + res_y * c;
//
//    // translate point back:
//    res_x = xnew + this->x;
//    res_y = ynew + this->y;

//    Point rotated=rotateAroundPoint(Point(this->getHitbox().getX(),this->getHitbox().getY()),
//                                          Point(this->x,this->y),angle+this->getHitbox().getAngle());
//
//    painter->drawRectangle(rotated.x,
//                           rotated.y,
//                           this->getHitbox().getWidth(),this->getHitbox().getHeight(),
//                           angle+this->getHitbox().getAngle(),100,0,0,100,true);

//    painter->drawRectangle(this->getX()+this->getHitbox().getX(),
//                           this->getY()+this->getHitbox().getY(),
//                           this->getHitbox().getWidth(),this->getHitbox().getHeight(),
//                           this->getHitbox().getAngle(),100,0,0,100,true);

    painter->drawRectangle(this->getHitbox().getX(),
                           this->getHitbox().getY(),
                           this->getHitbox().getWidth(),this->getHitbox().getHeight(),
                           this->getHitbox().getAngle(),100,0,0,100,true);
}

int Pattern::getX()
{
    return this->x;
}
int Pattern::getY()
{
    return this->y;
}

Hitbox Pattern::getHitbox()
{
    Hitbox hitbox = bullet->getHitbox();
//    Point rotated=rotateAroundPoint(Point(hitbox.getX(),hitbox.getY()),
//                                    Point(this->x,this->y),
//                                    this->angle+hitbox.getAngle());
//    hitbox.setX(rotated.x);
//    hitbox.setY(rotated.y);
//    hitbox.setAngle(this->angle+hitbox.getAngle());
//    return hitbox;
    return hitbox.getPlacedHitbox(Point(this->x,this->y),this->angle);
}

bool Pattern::destroyFlag()
{
    if(duration<0)
        return false;
    return iteration>duration;
}

float Pattern::getAngle()
{
    return angle;
}
