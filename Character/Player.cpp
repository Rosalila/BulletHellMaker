#include "Player.h"

Player::Player(Sound* sonido,RosalilaGraphics* painter,Receiver* receiver,std::string name)
{
    //Setting up the other variables
    this->name=name;
    this->directory="chars/"+name+"/";
    this->sonido=sonido;
    this->painter=painter;
    this->receiver=receiver;
    this->active_patterns=new std::list<Pattern*>;
    this->shooting=true;
    this->orientation="idle";
    this->current_type="1";
    this->visible=true;

    //Sprites animation
    this->animation_velocity=4;
    this->animation_iteration=0;
    this->current_sprite=0;

    this->iteration=0;

    loadFromXML();
    life_bar=painter->getTexture(directory+"life_bar.png");;
}

void Player::inputControl()
{
    if(receiver->IsKeyDownn(SDLK_DOWN))
    {
        if(orientation!="down" && this->sonido->soundExists(name+".down"))
            this->sonido->playSound(name+".down");
        orientation="down";
    }
    else if(receiver->IsKeyDownn(SDLK_UP))
    {
        if(orientation!="up" && this->sonido->soundExists(name+".up"))
            this->sonido->playSound(name+".up");
        orientation="up";
    }
    else if(receiver->IsKeyDownn(SDLK_LEFT))
    {
        if(orientation!="left" && this->sonido->soundExists(name+".left"))
            this->sonido->playSound(name+".left");
        orientation="left";
    }
    else if(receiver->IsKeyDownn(SDLK_RIGHT))
    {
        if(orientation!="right" && this->sonido->soundExists(name+".right"))
            this->sonido->playSound(name+".right");
        orientation="right";
    }
    else
    {
        if(orientation!="idle" && this->sonido->soundExists(name+".idle"))
            this->sonido->playSound(name+".idle");
        orientation="idle";
    }

    int slowdown = 1;
    if(isSlowPressed())
        slowdown = 3;

    if(receiver->IsKeyDownn(SDLK_DOWN)
       || receiver->IsJoyDown(-2,0))
        this->y+=velocity/slowdown;
    if(receiver->IsKeyDownn(SDLK_UP)
       || receiver->IsJoyDown(-8,0))
        this->y-=velocity/slowdown;
    if(receiver->IsKeyDownn(SDLK_LEFT)
       || receiver->IsJoyDown(-4,0))
        this->x-=velocity/slowdown;
    if(receiver->IsKeyDownn(SDLK_RIGHT)
       || receiver->IsJoyDown(-6,0))
        this->x+=velocity/slowdown;

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
    }else
    {
        if(orientation!="destroyed" && this->sonido->soundExists(name+".destroyed"))
            this->sonido->playSound(name+".destroyed");
        orientation="destroyed";
        this->hitbox.setValues(0,0,0,0,0);
    }
    spellControl(stage_velocity);

    slowExtraControl();
}

void Player::render()
{
    painter->drawRectangle(life_bar_x+life_bar_rect_offset_x,life_bar_y+life_bar_rect_offset_y,(life_bar_rect_width*hp)/max_hp,life_bar_rect_height,0,this->color.getRed(),this->color.getGreen(),this->color.getBlue(),this->color.getAlpha(),false);
    parrentRender();
//    painter->drawRectangle(this->getHitbox().getX(),
//                           this->getHitbox().getY(),
//                           hitbox.getWidth(),hitbox.getHeight(),
//                           hitbox.getAngle(),100,0,0,100,true);

    painter->draw2DImage
    (   life_bar,
        life_bar->getWidth(),life_bar->getHeight(),
        painter->camera_x+life_bar_x,life_bar_y,
        1.0,
        0.0,
        false,
        0,0,
        Color(255,255,255,255),
        true);
}
