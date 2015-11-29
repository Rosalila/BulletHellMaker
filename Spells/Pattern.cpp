#include "Pattern.h"


Pattern::Pattern(Sound* sonido,RosalilaGraphics* painter,Receiver* receiver,int velocity,int max_velocity,int acceleration,int a_frequency,float angle,int angle_change,int stop_ac_at,int ac_frequency,int animation_velocity,
                 Bullet* bullet,int offset_x,int offset_y,int startup,int cooldown,int duration,int random_angle,bool aim_player,int bullet_rotation,int br_change,int independent_br,bool freeze, bool homing, std::map<int, vector<Modifier*>* >*modifiers,std::map<std::string,Bullet*> *bullets)
{
    this->sonido=sonido;
    this->painter=painter;
    this->receiver=receiver;

    this->freeze=freeze;
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
    this->homing = homing;

    //Sprites animation
    this->animation_iteration=0;
    this->current_sprite=0;
    this->state="startup";
    this->current_startup=0;
    this->current_cooldown=0;

    //Bullet rotation
    this->bullet_rotation=bullet_rotation;
    this->br_change=br_change;
    this->independent_br=independent_br;

    this->iteration=0;
    this->duration=duration;
    this->is_hit=false;
    this->delete_flag=false;

    //Modifiers
    this->modifiers=modifiers;
    this->bullets=bullets;
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
    this->freeze=pattern->freeze;
    this->homing = pattern->homing;

    this->iteration=0;
    this->duration=pattern->duration;

    //Sprites animation
    this->animation_iteration=0;
    this->current_sprite=0;
    this->is_hit=false;
    this->delete_flag=false;

    //Bullet rotation
    this->bullet_rotation=pattern->bullet_rotation;
    this->br_change=pattern->br_change;
    this->independent_br=pattern->independent_br;

    //Modifiers
    this->modifiers=pattern->modifiers;
    this->bullets=pattern->bullets;
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
    if(getIterateSlowdownFlag())
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
}

void Pattern::updateStateNotShouting()
{
    current_startup=0;
    current_cooldown=0;
    state="startup";
}


void Pattern::logic(int stage_speed)
{

    if(freeze){
        this->x += stage_speed;
        modifiersControl();
        return;
    }
    this->x += (cos (angle * PI / 180) * velocity ) / getSlowdown() + stage_speed;
    this->y -= sin (angle * PI / 180) * velocity / getSlowdown();

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

    if(getIterateSlowdownFlag()) {
        animation_iteration++;
        current_a_frequency++;
        current_stop_ac_at++;
    }

    if(current_a_frequency >= a_frequency) {
        current_a_frequency=0;
        velocity+=acceleration;
        if(velocity>max_velocity)
            velocity=max_velocity;
    }

    if(current_stop_ac_at<stop_ac_at || stop_ac_at==-1)
    {
        if(getIterateSlowdownFlag())
            current_ac_frequency++;
        if(current_ac_frequency>ac_frequency)
        {
            current_ac_frequency=0;
            angle+=angle_change/getSlowdown();
        }
    }

    bullet_rotation += br_change;

    modifiersControl();
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
            getBulletAngle(),
            false,
            0,0,
            Color(255,255,255,255),
            0,0,
            true,
            FlatShadow());
    }

    if(receiver->isKeyDown(SDLK_h))
    {
        for(int i=0;i<(int)bullet->getHitboxes().size();i++)
        {
            Hitbox h=this->bullet->getHitboxes()[i]->getPlacedHitbox(this->getX(),this->getY(),this->getBulletAngle());
            painter->drawRectangle(h.getX(),
                                   h.getY(),
                                   this->bullet->getHitboxes()[i]->getWidth(),this->bullet->getHitboxes()[i]->getHeight(),
                                   h.getAngle(),100,0,0,100,true);
        }
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

float Pattern::getBulletAngle()
{
    float angle_temp = angle + bullet_rotation;
    if(independent_br)
    {
        angle_temp = bullet_rotation;
    }
    return angle_temp;
}

float Pattern::getRandomAngle()
{
    if(random_angle==0)
        return 0;
    if(random_angle<0)
    {
        return -(rand()%(-random_angle));
    }
    return rand()%random_angle;
}

bool Pattern::getAimPlayer()
{
    if(!aim_player)
        return false;
    aim_player=false;
    return true;
}

int Pattern::getHoming() {
    return this->homing;
}

void aimTo(int x,int y)
{
//    double distance_x=player->x-this->x-pattern->y;
//    double distance_y=player->y-this->y+pattern->x;
//    angle-=atan2(distance_y,distance_x)*180/PI;
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
    bullet->playHitSound();
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
                this->bullet=(*this->bullets)[modifier->value];
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
                this->x+=this->offset_x;
            }
            if(modifier->variable=="offset_y")
            {
                this->offset_y=atoi(modifier->value.c_str());
                this->y+=this->offset_y;
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
                this->angle+=this->getRandomAngle();
            }
            if(modifier->variable=="aim_player")
            {
                this->aim_player=strcmp(modifier->value.c_str(),"yes")==0;
            }
            if(modifier->variable=="freeze")
            {
                this->freeze=modifier->value=="yes";
            }
            if(modifier->variable=="homing")
            {
                this->homing=modifier->value=="yes";
            }
            if(modifier->variable=="velocity")
            {
                this->velocity=atoi(modifier->value.c_str());
            }
            if(modifier->variable=="iterator")
            {
                this->iteration=atoi(modifier->value.c_str());
                flag_iterator_change=true;
            }
            if(modifier->variable=="bullet_rotation")
            {
                this->bullet_rotation=atoi(modifier->value.c_str());
            }
            if(modifier->variable=="br_change")
            {
                this->br_change=atoi(modifier->value.c_str());
            }
            if(modifier->variable=="independent_br")
            {
                this->independent_br=strcmp(modifier->value.c_str(),"yes")==0;
            }
        }
    }

    if( !flag_iterator_change && getIterateSlowdownFlag())
        iteration++;
}
