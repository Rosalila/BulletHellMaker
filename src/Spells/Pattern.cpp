#include "../include/Spells/Pattern.h"

Pattern::Pattern(Sound* sonido,Painter* painter,Receiver* receiver,int velocity,int max_velocity,int acceleration,int a_frequency,float angle,int angle_change,int stop_ac_at,int ac_frequency,int animation_velocity,Bullet* bullet,int offset_x,int offset_y,int startup,int cooldown,int duration,int random_angle,bool aim_player,std::map<int, vector<Modifier*>* >*modifiers)
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
    this->random_angle=random_angle;
    this->aim_player=aim_player;

    //Sprites animation
    this->animation_iteration=0;
    this->current_sprite=0;
    this->state="startup";
    this->current_startup=0;
    this->current_cooldown=0;

    this->iteration=0;
    this->duration=duration;
    this->is_hit=false;
    this->delete_flag=false;

    //Modifiers
    this->modifiers=modifiers;
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
    this->random_angle=pattern->random_angle;
    this->aim_player=pattern->aim_player;

    this->iteration=0;
    this->duration=pattern->duration;

    //Sprites animation
    this->animation_iteration=0;
    this->current_sprite=0;
    this->is_hit=false;
    this->delete_flag=false;

    //Modifiers
    this->modifiers=pattern->modifiers;
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
        if(is_hit)
        {
            if(current_sprite>=bullet->spritesOnHitSize())
            {
                delete_flag=true;
                current_sprite=0;
            }
        }else
        {
            if(current_sprite>=bullet->spritesSize())
                current_sprite=0;
        }

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

    modifiersControl();

    //iteration++;
}

void Pattern::render()
{
    Image*image;
    if(is_hit)
    {
        image=bullet->getOnHitImage(current_sprite);
    }else
    {
        image=bullet->getImage(current_sprite);
    }
    if(image!=NULL)
    {
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

    if(receiver->IsKeyDownn(SDLK_h))
    {
        painter->drawRectangle(this->getHitbox().getX(),
                               this->getHitbox().getY(),
                               this->getHitbox().getWidth(),this->getHitbox().getHeight(),
                               this->getHitbox().getAngle(),100,0,0,100,true);
    }
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
    return hitbox.getPlacedHitbox(Point(this->x,this->y),this->angle);
}

bool Pattern::destroyFlag()
{
    if(delete_flag)
        return true;
    if(duration<0)
        return false;
    return iteration>duration;
}

float Pattern::getAngle()
{
    return angle;
}

float Pattern::getRandomAngle()
{
    if(random_angle==0)
        return 0;
    return rand()%random_angle;
}

bool Pattern::getAimPlayer()
{
    return aim_player;
}

Bullet* Pattern::getBullet()
{
    return this->bullet;
}

int Pattern::getDamage()
{
    return this->getBullet()->getDamage();
}

void Pattern::setAngle(float angle)
{
    this->angle=angle;
}

Pattern::~Pattern()
{

}

void Pattern::hit()
{
    is_hit=true;
    current_sprite=0;
    velocity=0;
    angle_change=0;
    acceleration=0;
}

bool Pattern::isHit()
{
    return is_hit;
}

void Pattern::modifiersControl()
{
    bool flag_iterator_change=false;

    vector<Modifier*>* current_modifiers = (*this->modifiers)[iteration];
    if(current_modifiers!=NULL)
    {
        for(int i=0;i<(int)current_modifiers->size();i++)
        {
            Modifier* modifier=(*current_modifiers)[i];

            if(modifier->variable=="bullet")
            {
                //!!!!
                //this->velocity=atoi(modifier->value.c_str());
            }

            if(modifier->variable=="velocity")
            {
                this->velocity=atoi(modifier->value.c_str());
            }
            if(modifier->variable=="max_velocity")
            {
                this->max_velocity=atoi(modifier->value.c_str());
            }
            if(modifier->variable=="acceleration")
            {
                this->acceleration=atoi(modifier->value.c_str());
            }
            if(modifier->variable=="a_frequency")
            {
                this->a_frequency=atoi(modifier->value.c_str());
            }
            if(modifier->variable=="angle")
            {
                this->angle=atoi(modifier->value.c_str());
            }
            if(modifier->variable=="angle_change")
            {
                this->angle_change=atoi(modifier->value.c_str());
            }
            if(modifier->variable=="stop_ac_at")
            {
                this->stop_ac_at=atoi(modifier->value.c_str());
            }
            if(modifier->variable=="ac_frequency")
            {
                this->ac_frequency=atoi(modifier->value.c_str());
            }
            if(modifier->variable=="animation_velocity")
            {
                this->animation_velocity=atoi(modifier->value.c_str());
            }
            if(modifier->variable=="offset_x")
            {
                this->offset_x=atoi(modifier->value.c_str());
            }
            if(modifier->variable=="offset_y")
            {
                this->offset_y=atoi(modifier->value.c_str());
            }
            if(modifier->variable=="startup")
            {
                this->startup=atoi(modifier->value.c_str());
            }
            if(modifier->variable=="cooldown")
            {
                this->cooldown=atoi(modifier->value.c_str());
            }
            if(modifier->variable=="duration")
            {
                this->duration=atoi(modifier->value.c_str());
            }
            if(modifier->variable=="random_angle")
            {
                this->random_angle=atoi(modifier->value.c_str());
            }
            if(modifier->variable=="aim_player")
            {
                this->aim_player=strcmp(modifier->value.c_str(),"yes")==0;
            }
            if(modifier->variable=="velocity")
            {
                this->velocity=atoi(modifier->value.c_str());
            }
            if(modifier->variable=="angle")
            {
                this->angle=atoi(modifier->value.c_str());
            }
            if(modifier->variable=="angle_change")
            {
                this->angle_change=atoi(modifier->value.c_str());
            }
            if(modifier->variable=="iterator")
            {
                this->iteration=atoi(modifier->value.c_str());
                flag_iterator_change=true;
            }
        }
    }
    angle+=angle_change;
    this->x += cos (angle*PI/180) * velocity;
    this->y -= sin (angle*PI/180) * velocity;

    if(!flag_iterator_change)
        iteration++;
}
