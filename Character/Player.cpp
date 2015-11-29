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

    this->slow_in_cooldown=false;

    life_bar=painter->getTexture(assets_directory+directory+"life_bar.png");

    loadPlayerFromXML();
}

void Player::loadPlayerFromXML()
{
    loadFromXML();

    //Loading file
    std::string main_path=assets_directory+directory+"main.xml";
    TiXmlDocument doc_t(main_path.c_str());
    doc_t.LoadFile();
    TiXmlDocument *doc;
    doc=&doc_t;
    TiXmlNode *main_file=doc->FirstChild("MainFile");

    this->current_slow=0;
    this->max_slow=-1;
    TiXmlElement *attributes=main_file->FirstChild("Attributes")->ToElement();
    if(attributes->Attribute("slow")!=NULL)
    {
        this->current_slow=atoi(attributes->Attribute("slow"));
        this->max_slow=atoi(attributes->Attribute("slow"));
    }

    this->slow_decrement=3;
    if(attributes->Attribute("slow_decrement")!=NULL)
    {
        this->slow_decrement=atoi(attributes->Attribute("slow_decrement"));
    }

    this->slow_increment=1;
    if(attributes->Attribute("slow_increment")!=NULL)
    {
        this->slow_increment=atoi(attributes->Attribute("slow_increment"));
    }

    this->slow_cooldown_increment=2;
    if(attributes->Attribute("slow_cooldown_increment")!=NULL)
    {
        this->slow_cooldown_increment=atoi(attributes->Attribute("slow_cooldown_increment"));
    }

    this->slow_bar_x=0;
    this->slow_bar_y=0;
    this->slow_bar_rect_offset_x=0;
    this->slow_bar_rect_offset_y=0;
    this->slow_bar_rect_height=0;
    this->slow_bar_rect_width=0;
    this->slow_bar_color.red=0;
    this->slow_bar_color.green=0;
    this->slow_bar_color.blue=0;
    this->slow_bar_color.alpha=255;
    this->slow_bar_cooldown_color.red=0;
    this->slow_bar_cooldown_color.green=0;
    this->slow_bar_cooldown_color.blue=0;
    this->slow_bar_cooldown_color.alpha=128;

    if(main_file->FirstChild("SlowBar")!=NULL)
    {
        TiXmlElement *slow_bar=main_file->FirstChild("SlowBar")->ToElement();
        if(slow_bar->Attribute("x")!=NULL)
            this->slow_bar_x=atoi(slow_bar->Attribute("x"));
        if(slow_bar->Attribute("y")!=NULL)

            this->slow_bar_y=atoi(slow_bar->Attribute("y"));
        if(slow_bar->Attribute("color_r")!=NULL)
            this->slow_bar_color.red=atoi(slow_bar->Attribute("color_r"));
        if(slow_bar->Attribute("color_g")!=NULL)
            this->slow_bar_color.green=atoi(slow_bar->Attribute("color_g"));
        if(slow_bar->Attribute("color_b")!=NULL)
            this->slow_bar_color.blue=atoi(slow_bar->Attribute("color_b"));
        if(slow_bar->Attribute("color_a")!=NULL)
            this->slow_bar_color.alpha=atoi(slow_bar->Attribute("color_a"));

        if(slow_bar->Attribute("cooldown_color_r")!=NULL)
            this->slow_bar_cooldown_color.red=atoi(slow_bar->Attribute("cooldown_color_r"));
        if(slow_bar->Attribute("cooldown_color_g")!=NULL)
            this->slow_bar_cooldown_color.green=atoi(slow_bar->Attribute("cooldown_color_g"));
        if(slow_bar->Attribute("cooldown_color_b")!=NULL)
            this->slow_bar_cooldown_color.blue=atoi(slow_bar->Attribute("cooldown_color_b"));
        if(slow_bar->Attribute("cooldown_color_a")!=NULL)
            this->slow_bar_cooldown_color.alpha=atoi(slow_bar->Attribute("cooldown_color_a"));

        if(slow_bar->Attribute("rect_offset_x")!=NULL)
            this->slow_bar_rect_offset_x=atoi(slow_bar->Attribute("rect_offset_x"));
        if(slow_bar->Attribute("rect_offset_y")!=NULL)
            this->slow_bar_rect_offset_y=atoi(slow_bar->Attribute("rect_offset_y"));
        if(slow_bar->Attribute("rect_height")!=NULL)
            this->slow_bar_rect_height=atoi(slow_bar->Attribute("rect_height"));
        if(slow_bar->Attribute("rect_width")!=NULL)
            this->slow_bar_rect_width=atoi(slow_bar->Attribute("rect_width"));
    }
}

void Player::inputControl()
{
    if(receiver->isKeyDown(SDL_SCANCODE_DOWN)
       || receiver->isJoyDown(-2,0))
    {
        if(orientation!="down" && this->sonido->soundExists(name+".down"))
            this->sonido->playSound(name+".down");
        orientation="down";
    }
    else if(receiver->isKeyDown(SDL_SCANCODE_UP)
       || receiver->isJoyDown(-8,0))
    {
        if(orientation!="up" && this->sonido->soundExists(name+".up"))
            this->sonido->playSound(name+".up");
        orientation="up";
    }
    else if(receiver->isKeyDown(SDL_SCANCODE_LEFT)
       || receiver->isJoyDown(-4,0))
    {
        if(orientation!="left" && this->sonido->soundExists(name+".left"))
            this->sonido->playSound(name+".left");
        orientation="left";
    }
    else if(receiver->isKeyDown(SDL_SCANCODE_RIGHT)
       || receiver->isJoyDown(-6,0))
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

    if(receiver->isKeyDown(SDL_SCANCODE_DOWN)
       || receiver->isJoyDown(-2,0))
    {
        this->y+=velocity/getSlowdown();
    }
    if(receiver->isKeyDown(SDL_SCANCODE_UP)
       || receiver->isJoyDown(-8,0))
    {
        this->y-=velocity/getSlowdown();
    }
    if(receiver->isKeyDown(SDL_SCANCODE_LEFT)
       || receiver->isJoyDown(-4,0))
    {
        this->x-=velocity/getSlowdown();
    }
    if(receiver->isKeyDown(SDL_SCANCODE_RIGHT)
       || receiver->isJoyDown(-6,0))
    {
       this->x+=velocity/getSlowdown();
    }

    if(receiver->isKeyDown(SDLK_z)
       || receiver->isJoyDown(0,0))
    {
        this->shooting=true;
    }
    else
    {
        this->shooting=false;
    }
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
    //Enable or disable slow
    if(isSlowPressed() && !slow_in_cooldown)
    {
        enableSlow();
        current_slow-=slow_decrement;
    }else
    {
        disableSlow();
        if(slow_in_cooldown)
            current_slow+=slow_cooldown_increment;
        else
            current_slow+=slow_increment;
    }

    //Check max and min slow
    if(current_slow<=0)
    {
        current_slow=0;
    }
    if(current_slow>max_slow)
    {
        current_slow=max_slow;
    }

    //Slow cooldown
    if(slow_in_cooldown && current_slow>=max_slow)
    {
        slow_in_cooldown=false;
    }
    if(!slow_in_cooldown && current_slow<=0)
    {
        slow_in_cooldown=true;
    }

    spellControl(stage_velocity);

    iteration++;
}

void Player::render()
{
    //HP
    painter->drawRectangle(life_bar_x+life_bar_rect_offset_x,life_bar_y+life_bar_rect_offset_y,(life_bar_rect_width*hp)/max_hp,life_bar_rect_height,0,this->color.getRed(),this->color.getGreen(),this->color.getBlue(),this->color.getAlpha(),false);
    if(!slow_in_cooldown)
        painter->drawRectangle(slow_bar_x+slow_bar_rect_offset_x,slow_bar_y+slow_bar_rect_offset_y,(slow_bar_rect_width*current_slow)/max_slow,slow_bar_rect_height,0,this->slow_bar_color.getRed(),this->slow_bar_color.getGreen(),this->slow_bar_color.getBlue(),this->slow_bar_color.getAlpha(),false);
    else
        painter->drawRectangle(slow_bar_x+slow_bar_rect_offset_x,slow_bar_y+slow_bar_rect_offset_y,(slow_bar_rect_width*current_slow)/max_slow,slow_bar_rect_height,0,this->slow_bar_cooldown_color.getRed(),this->slow_bar_cooldown_color.getGreen(),this->slow_bar_cooldown_color.getBlue(),this->slow_bar_cooldown_color.getAlpha(),false);
    parrentRender();

    painter->draw2DImage
    (   life_bar,
        life_bar->getWidth(),life_bar->getHeight(),
        painter->camera_x+life_bar_x,life_bar_y,
        1.0,
        0.0,
        false,
        0,0,
        Color(255,255,255,255),
        0,0,
        true,
        FlatShadow());

    if(isSlowActive())
    {
        painter->draw3DCube(this->getHitbox().getX(),this->getHitbox().getY(),2.0,Color(255,0,0,180));
    }else
    {
        painter->draw3DCube(this->getHitbox().getX(),this->getHitbox().getY(),2.0,Color(255,0,0,100));
    }

//    if(shooting)
//    {
//        if(iteration%10==0)
//            painter->addExplosion(this->x,this->y);
//    }
    painter->draw3D();
}
