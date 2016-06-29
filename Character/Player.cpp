#include "Player.h"

Player::Player(Sound* sonido,RosalilaGraphics* painter,Receiver* receiver,std::string name,int sound_channel_base)
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
    this->charge_ready=false;
    this->frame=0;
    this->charging_sound_channel=-1;

    //Sprites animation
    this->animation_velocity=4;
    this->animation_iteration=0;
    this->current_sprite=0;

    //Color effect
    current_color_effect_r=255;
    current_color_effect_g=255;
    current_color_effect_b=255;
    current_color_effect_a=255;

    //Shake
    current_screen_shake_x=0;
    current_screen_shake_y=0;
    shake_time=0;
    shake_magnitude=0;

    this->iteration=0;

    this->slow_in_cooldown=false;

    this->sound_channel_base=sound_channel_base;

    loadPlayerFromXML();

    current_shield=max_shield;

    //Parry
    this->current_parry_frame=parry_duration+1;

    //Acceleration
    this->invulnerable_frames_left=0;

    parries_left=3;
    parry_sprites.push_back(painter->getTexture(assets_directory+directory+"sprites/parry/1.png"));
    parry_sprites.push_back(painter->getTexture(assets_directory+directory+"sprites/parry/2.png"));
    parry_sprites.push_back(painter->getTexture(assets_directory+directory+"sprites/parry/3.png"));

    //Move sound
    move_sound_channel=-1;
    string move_sound_path = assets_directory+directory+"/sounds/move.wav";
    this->sonido->addSound("move init",assets_directory+directory+"/sounds/move_init.wav");
    this->sonido->addSound("move",assets_directory+directory+"/sounds/move.wav");
    move_sound_channel = this->sonido->playSound("move",20, -1);
    Mix_Volume(move_sound_channel, 0);
    move_sound_volume=0;

    //Charge
    this->sonido->addSound("charge ready",assets_directory+directory+"/sounds/charge_ready.wav");
    this->sonido->addSound("charging",assets_directory+directory+"/sounds/charging.wav");
    charging_sound_channel=this->sonido->playSound("charging",21,-1);
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
    bool up_pressed,down_pressed,left_pressed,right_pressed;
    up_pressed=down_pressed=left_pressed=right_pressed=false;

    bool started_moving = false;

    if(receiver->isKeyDown(SDL_SCANCODE_DOWN)
       || receiver->isJoyDown(-2,0))
    {
        if(orientation!="down")
            started_moving=true;
        down_pressed=true;
    }
    if(receiver->isKeyDown(SDL_SCANCODE_UP)
       || receiver->isJoyDown(-8,0))
    {
        if(orientation!="up")
            started_moving=true;
        up_pressed=true;
    }
    if(receiver->isKeyDown(SDL_SCANCODE_LEFT)
       || receiver->isJoyDown(-4,0))
    {
        if(orientation!="left")
            started_moving=true;
        left_pressed=true;
    }
    if(receiver->isKeyDown(SDL_SCANCODE_RIGHT)
       || receiver->isJoyDown(-6,0))
    {
        if(orientation!="right")
            started_moving=true;
        right_pressed=true;
    }

    if(down_pressed||up_pressed||left_pressed||right_pressed)
    {
        move_sound_volume+=50;
        if(move_sound_volume>128)
            move_sound_volume=128;
    }else
    {
        //move_sound_volume=0;
    }

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

    int velocity_boost=invulnerable_frames_left;

    Mix_Volume(move_sound_channel, move_sound_volume);
    move_sound_volume-=20;
    if(move_sound_volume<0)
        move_sound_volume=0;

    bool diagonal_slowdown=2;

    double delta_y=0;
    double delta_x=0;

    if(up_pressed && !down_pressed && !left_pressed && !right_pressed)//8
    {
        delta_y = -(velocity+velocity_boost)/getSlowdown();
    }
    if(!up_pressed && down_pressed && !left_pressed && !right_pressed)//2
    {
        delta_y = (velocity+velocity_boost)/getSlowdown();
    }
    if(!up_pressed && !down_pressed && left_pressed && !right_pressed)//4
    {
        delta_x = -(velocity+velocity_boost)/getSlowdown();
    }
    if(!up_pressed && !down_pressed && !left_pressed && right_pressed)//6
    {
        delta_x = (velocity+velocity_boost)/getSlowdown();
    }

    if(!up_pressed && down_pressed && left_pressed && !right_pressed)//1
    {
        delta_x = cos (225 * PI / 180) * (velocity+velocity_boost) / getSlowdown();
        delta_y = - sin (225 * PI / 180) * (velocity+velocity_boost) / getSlowdown();

    }
    if(!up_pressed && down_pressed && !left_pressed && right_pressed)//3
    {
        delta_x = cos (315 * PI / 180) * (velocity+velocity_boost) / getSlowdown();
        delta_y = - sin (315 * PI / 180) * (velocity+velocity_boost) / getSlowdown();
    }
    if(up_pressed && !down_pressed && left_pressed && !right_pressed)//7
    {
        delta_x = cos (135 * PI / 180) * (velocity+velocity_boost) / getSlowdown();
        delta_y = - sin (135 * PI / 180) * (velocity+velocity_boost) / getSlowdown();
    }
    if(up_pressed && !down_pressed && !left_pressed && right_pressed)//9
    {
        delta_x = cos (45 * PI / 180) * (velocity+velocity_boost) / getSlowdown();
        delta_y = - sin (45 * PI / 180) * (velocity+velocity_boost) / getSlowdown();
    }

    this->x+=delta_x;
    this->y+=delta_y;

    if(delta_x!=last_delta_x || delta_y!=last_delta_y)
    {
        if(delta_x!=0 || delta_y!=0)
            this->sonido->playSound("move init",-1,0);
    }

    last_delta_x=delta_x;
    last_delta_y=delta_y;

    if(receiver->isKeyDown(SDLK_z)
       || receiver->isJoyDown(0,0))
    {
        if(!this->shooting && !isParrying() && parries_left>0)
        {
            current_parry_frame=0;
        }
        this->shooting=true;
        if(max_charge!=0 && current_charge==max_charge)
        {
            std::vector<Pattern*> patterns=type["bomb"];
            patterns[0]->getBullet()->playSound();
            this->addActivePattern(patterns[0]);
        }
        current_charge=0;
    }
    else
    {
        this->shooting=false;
    }
}

void Player::logic(int stage_velocity)
{
    current_parry_frame++;

    invulnerable_frames_left-=1;
    if(invulnerable_frames_left<0)
        invulnerable_frames_left=0;

    if(!shooting)
    {
        invulnerable_frames_left=0;
    }

    if(invulnerable_frames_left>0)
    {
        enableSlow();
    }else
    {
        disableSlow();
    }

    animationControl();
    if(this->hp!=0)
    {
        inputControl();
    }else
    {
        if(orientation!="destroyed" && this->sonido->soundExists(name+".destroyed"))
            this->sonido->playSound(name+".destroyed");
        orientation="destroyed";
        //this->hitbox.setValues(0,0,0,0,0);
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

    current_color_effect_a = (255*hp)/max_hp;

    current_shield-=shield_fade;
    if(current_shield<0)
        current_shield=0;

    current_charge+=charge_velocity;
    if(current_charge>=max_charge)
    {
        current_charge=max_charge;
        if(!charge_ready)
        {
            this->sonido->playSound("charge ready",-1, 0);
        }
        charge_ready=true;
    }else
    {
        charge_ready=false;
    }

    if(!charge_ready && !shooting)
        Mix_Volume(charging_sound_channel, 128);
    else
        Mix_Volume(charging_sound_channel, 0);
    //current_color_effect_b = (255*hp)/max_hp;

    //Color effect
//    if(current_color_effect_r<255)
//        current_color_effect_r++;
//    if(current_color_effect_g<255)
//        current_color_effect_g++;
//    if(current_color_effect_b<255)
//        current_color_effect_b++;
//    if(current_color_effect_a<255)
//        current_color_effect_a++;
    frame++;
}

void Player::bottomRender()
{
    Character::bottomRender();

    if(current_shield>0)
    {
        double shield_transparency = 255.0*getShieldPercentage();

        if(shield_image)
        painter->draw2DImage
            (   shield_image,
                shield_image->getWidth(),shield_image->getHeight(),
                this->x-shield_image->getWidth()/2+current_screen_shake_x,
                this->y-shield_image->getHeight()/2+current_screen_shake_y,
                1.0,
                0.0,
                false,
                0,0,
                Color(255,255,255,shield_transparency),
                0,0,
                true,
                FlatShadow());
    }

    if(current_charge>0)
    {
        int transparency_divider=1;
        if(getGameOver())
            transparency_divider=8;

        int charge_transparency_effect=255;
        if(charge_ready)
            charge_transparency_effect=(frame*10)%255;

        if(charge_image)
        painter->draw2DImage
            (   charge_image,
                charge_image->getWidth(),charge_image->getHeight()*((double)current_charge/(double)max_charge),
                this->x+charge_sprite_x,
                this->y+charge_sprite_y-charge_image->getHeight()*
                    ((double)current_charge/(double)max_charge)/2,
                1.0,
                0.0,
                false,
                0,0,
                Color(255,255,255,charge_transparency_effect/transparency_divider),
                0,0,
                true,
                FlatShadow());
    }

    //HP
//    painter->drawRectangle(life_bar_x+life_bar_rect_offset_x,life_bar_y+life_bar_rect_offset_y,(life_bar_rect_width*hp)/max_hp,life_bar_rect_height,0,this->color.getRed(),this->color.getGreen(),this->color.getBlue(),this->color.getAlpha(),false);
//    if(!slow_in_cooldown)
//        painter->drawRectangle(slow_bar_x+slow_bar_rect_offset_x,slow_bar_y+slow_bar_rect_offset_y,(slow_bar_rect_width*current_slow)/max_slow,slow_bar_rect_height,0,this->slow_bar_color.getRed(),this->slow_bar_color.getGreen(),this->slow_bar_color.getBlue(),this->slow_bar_color.getAlpha(),false);
//    else
//        painter->drawRectangle(slow_bar_x+slow_bar_rect_offset_x,slow_bar_y+slow_bar_rect_offset_y,(slow_bar_rect_width*current_slow)/max_slow,slow_bar_rect_height,0,this->slow_bar_cooldown_color.getRed(),this->slow_bar_cooldown_color.getGreen(),this->slow_bar_cooldown_color.getBlue(),this->slow_bar_cooldown_color.getAlpha(),false);
    //parrentRender();
//
//    painter->draw2DImage
//    (   life_bar,
//        life_bar->getWidth(),life_bar->getHeight(),
//        painter->camera_x+life_bar_x,life_bar_y,
//        1.0,
//        0.0,
//        false,
//        0,0,
//        Color(255,255,255,255),
//        0,0,
//        true,
//        FlatShadow());
//
//    if(isSlowActive())
//    {
//        painter->draw3DCube(this->getHitbox().getX(),this->getHitbox().getY(),2.0,Color(255,0,0,180));
//    }else
//    {
//        painter->draw3DCube(this->getHitbox().getX(),this->getHitbox().getY(),2.0,Color(255,0,0,100));
//    }

//    if(shooting)
//    {
//        if(iteration%10==0)
//            painter->addExplosion(this->x,this->y);
//    }
    painter->draw3D();
}

void Player::topRender()
{
    Character::topRender();

    if(isParrying() && parries_left>=1)
    {
        Image *image=parry_sprites[parries_left-1];
        painter->draw2DImage
            (   image,
                image->getWidth(),image->getHeight(),
                this->x+parrying_x,
                this->y+parrying_y,
                1.0,
                0.0,
                false,
                0,0,
                Color(255,255,255,255),
                0,0,
                true,
                FlatShadow());
    }

if(parrying_image!=NULL && false)
if(isParrying())
    painter->draw2DImage
        (   parrying_image,
            parrying_image->getWidth(),parrying_image->getHeight(),
            this->x+parrying_x,
            this->y+parrying_y,
            1.0,
            0.0,
            false,
            0,0,
            Color(255,255,255,255),
            0,0,
            true,
            FlatShadow());

if(parryed_image!=NULL)
if(invulnerable_frames_left>0)
    painter->draw2DImage
        (   parryed_image,
            parryed_image->getWidth(),parryed_image->getHeight(),
            this->x+parryed_x,
            this->y+parryed_y,
            1.0,
            0.0,
            false,
            0,0,
            Color(255,255,255,255),
            0,0,
            true,
            FlatShadow());

        if(receiver->isKeyDown(SDLK_h))
        for(int i=0;i<parry_hitboxes.size();i++)
        {
            painter->drawRectangle(parry_hitboxes[i]->getX()+x,
                                   parry_hitboxes[i]->getY()+y,
                                   parry_hitboxes[i]->getWidth(),parry_hitboxes[i]->getHeight(),
                                   parry_hitboxes[i]->getAngle(),100,0,0,100,true);
        }

}

void Player::hit(int damage)
{
    if(current_shield==0.0)
    {
        Character::hit(damage);
    }else
    {
        double prorated_damage = ((double)damage)*proration*getShieldPercentage();
        Character::hit(prorated_damage);
    }
    current_shield=max_shield;
}

double Player::getShieldPercentage()
{
    if(current_shield==0 || max_shield==0)
        return 1.0;
    return (double)current_shield/(double)max_shield;
}

void Player::loadFromXML()
{
    Character::loadFromXML();
    //Loading file
    std::string main_path=assets_directory+directory+"main.xml";
    TiXmlDocument doc_t(main_path.c_str());
    doc_t.LoadFile();
    TiXmlDocument *doc;
    doc=&doc_t;
    TiXmlNode *main_file=doc->FirstChild("MainFile");

    //Loading attributes
    this->max_shield=0;
    this->shield_fade=0;
    this->proration=0;
    this->shield_image=NULL;
    if(main_file->FirstChild("Shield"))
    {
        TiXmlElement *attributes=main_file->FirstChild("Shield")->ToElement();
        if(attributes->Attribute("max_shield")!=NULL)
        {
            this->max_shield=atoi(attributes->Attribute("max_shield"));
        }

        if(attributes->Attribute("shield_fade")!=NULL)
        {
            this->shield_fade=atoi(attributes->Attribute("shield_fade"));
        }

        if(attributes->Attribute("shield_fade")!=NULL)
        {
            this->proration=((double)atoi(attributes->Attribute("shield_fade")))/100.0;
        }

        if(attributes->Attribute("sprite")!=NULL)
        {
            this->shield_image=painter->getTexture(assets_directory+directory+"/sprites/"+attributes->Attribute("sprite"));
        }
    }

    charge_image=NULL;
    current_charge=0;
    max_charge=0;
    charge_velocity=0;
    charge_sprite_x=0;
    charge_sprite_y=0;

    if(main_file->FirstChild("Charge"))
    {
        TiXmlElement *attributes=main_file->FirstChild("Charge")->ToElement();
        if(attributes->Attribute("max_charge")!=NULL)
        {
            this->max_charge=atoi(attributes->Attribute("max_charge"));
        }

        if(attributes->Attribute("charge_velocity")!=NULL)
        {
            this->charge_velocity=atoi(attributes->Attribute("charge_velocity"));
        }

        if(attributes->Attribute("x")!=NULL)
        {
            this->charge_sprite_x=atoi(attributes->Attribute("x"));
        }

        if(attributes->Attribute("y")!=NULL)
        {
            this->charge_sprite_y=atoi(attributes->Attribute("y"));
        }

        if(attributes->Attribute("sprite")!=NULL)
        {
            this->charge_image=painter->getTexture(assets_directory+directory+"/sprites/"+attributes->Attribute("sprite"));
        }
    }


    this->parry_duration=0;

    this->parrying_image=NULL;
    this->parrying_x=0;
    this->parrying_y=0;
    this->parryed_image=NULL;
    this->parryed_x=0;
    this->parryed_y=0;

    TiXmlNode* parry_node=main_file->FirstChild("Parry");
    if(parry_node)
    {
        TiXmlElement *attributes=parry_node->ToElement();
        if(attributes->Attribute("duration")!=NULL)
        {
            this->parry_duration=atoi(attributes->Attribute("duration"));
        }

        if(attributes->Attribute("sound")!=NULL)
        {
            std::string sprites_sound=attributes->Attribute("sound");
            this->sonido->addSound(name+".parry",assets_directory+directory+"sounds/"+sprites_sound);
        }

        TiXmlNode* parrying_node=parry_node->FirstChild("Parrying");
        if(parrying_node)
        {
            TiXmlElement *parrying_attributes=parrying_node->ToElement();
            if(parrying_attributes->Attribute("sprite")!=NULL)
            {
                this->parrying_image=painter->getTexture(assets_directory+directory+"/sprites/"+parrying_attributes->Attribute("sprite"));
            }
            if(parrying_attributes->Attribute("x")!=NULL)
            {
                this->parrying_x=atoi(parrying_attributes->Attribute("x"));
            }
            if(parrying_attributes->Attribute("y")!=NULL)
            {
                this->parrying_y=atoi(parrying_attributes->Attribute("y"));
            }
        }

        TiXmlNode* parryed_node=parry_node->FirstChild("Parryed");
        if(parryed_node)
        {
            TiXmlElement *parryed_attributes=parryed_node->ToElement();
            if(parryed_attributes->Attribute("sprite")!=NULL)
            {
                this->parryed_image=painter->getTexture(assets_directory+directory+"/sprites/"+parryed_attributes->Attribute("sprite"));
            }
            if(parryed_attributes->Attribute("x")!=NULL)
            {
                this->parryed_x=atoi(parryed_attributes->Attribute("x"));
            }
            if(parryed_attributes->Attribute("y")!=NULL)
            {
                this->parryed_y=atoi(parryed_attributes->Attribute("y"));
            }
        }

        TiXmlNode* hitboxes_node=parry_node->FirstChild("Hitboxes");
        if(hitboxes_node)
        {
            for(TiXmlNode* hitbox_node=hitboxes_node->FirstChild("Hitbox");
                    hitbox_node!=NULL;
                    hitbox_node=hitbox_node->NextSibling("Hitbox"))
            {
                TiXmlElement *hitbox_element=hitbox_node->ToElement();
                int hitbox_x=atoi(hitbox_element->Attribute("x"));
                int hitbox_y=atoi(hitbox_element->Attribute("y"));
                int hitbox_width=atoi(hitbox_element->Attribute("width"));
                int hitbox_height=atoi(hitbox_element->Attribute("height"));
                int hitbox_angle=atoi(hitbox_element->Attribute("angle"));
                Hitbox* hitbox=new Hitbox(hitbox_x,hitbox_y,hitbox_width,hitbox_height,hitbox_angle);
                this->parry_hitboxes.push_back(hitbox);
            }
        }
    }

}

bool Player::isParrying()
{
    return current_parry_frame<parry_duration;
}

bool Player::isInvulnerable()
{
    return invulnerable_frames_left>0;
}

void Player::parry(bool infinite_parries)
{
    if(!infinite_parries && invulnerable_frames_left==0)
    {
        parries_left-=1;
    }
    invulnerable_frames_left=15;
    if(this->sonido->soundExists(this->getName()+".parry"))
    {
        this->sonido->playSound(this->getName()+".parry",sound_channel_base+1,0);
    }
}

bool Player::collidesParry(Hitbox hitbox,int hitbox_x,int hitbox_y,float hitbox_angle)
{
    if(!visible)
        return false;
    for(int i=0;i<parry_hitboxes.size();i++)
        if(parry_hitboxes[i]->getPlacedHitbox(this->x,this->y).collides(hitbox))
            return true;
    return false;
}

void Player::exit()
{
    Mix_Volume(charging_sound_channel, 0);
}
