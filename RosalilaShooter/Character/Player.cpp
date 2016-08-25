#include "Player.h"

Player::Player(std::string name,int sound_channel_base,map<string,Button*>controls)
{
    //Setting up the other variables
    this->name=name;
    this->directory="chars/"+name+"/";
    this->active_patterns=new std::list<Pattern*>;
    this->shooting=true;
    this->orientation="idle";
    this->current_type="1";
    this->visible=true;
    this->charge_ready=false;
    this->frame=0;
    this->charging_sound_channel=-1;
    this->controls=controls;

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
    parry_sprites.push_back(Rosalila()->Graphics->getTexture(assets_directory+directory+"sprites/parry/1.png"));
    parry_sprites.push_back(Rosalila()->Graphics->getTexture(assets_directory+directory+"sprites/parry/2.png"));
    parry_sprites.push_back(Rosalila()->Graphics->getTexture(assets_directory+directory+"sprites/parry/3.png"));
    //Charge
    Rosalila()->Sound->addSound("charge ready",assets_directory+directory+"/sounds/charge_ready.wav");
    Rosalila()->Sound->addSound("charging",assets_directory+directory+"/sounds/charging.wav");
    charging_sound_channel=Rosalila()->Sound->playSound("charging",21,-1);
}

void Player::loadPlayerFromXML()
{
    loadFromXML();

    //Loading file
    Node* root_node = Rosalila()->Parser->getNodes(assets_directory+directory+"main.xml");

    this->current_slow=0;
    this->max_slow=-1;
    Node* attributes_node = root_node->getNodeByName("Attributes");
    if(attributes_node->hasAttribute("slow"))
    {
        this->current_slow=atoi(attributes_node->attributes["slow"].c_str());
        this->max_slow=atoi(attributes_node->attributes["slow"].c_str());
    }

    this->slow_decrement=3;
    if(attributes_node->hasAttribute("slow_decrement"))
    {
        this->slow_decrement=atoi(attributes_node->attributes["slow_decrement"].c_str());
    }

    this->slow_increment=1;
    if(attributes_node->hasAttribute("slow_increment"))
    {
        this->slow_increment=atoi(attributes_node->attributes["slow_increment"].c_str());
    }

    this->slow_cooldown_increment=2;
    if(attributes_node->hasAttribute("slow_cooldown_increment"))
    {
        this->slow_cooldown_increment=atoi(attributes_node->attributes["slow_cooldown_increment"].c_str());
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

    Node* slow_bar_node = root_node->getNodeByName("SlowBar");
    if(slow_bar_node)
    {
        if(slow_bar_node->hasAttribute("x"))
            this->slow_bar_x=atoi(slow_bar_node->attributes["x"].c_str());
        if(slow_bar_node->hasAttribute("y"))
            this->slow_bar_y=atoi(slow_bar_node->attributes["y"].c_str());
        if(slow_bar_node->hasAttribute("color_r"))
            this->slow_bar_color.red=atoi(slow_bar_node->attributes["color_r"].c_str());
        if(slow_bar_node->hasAttribute("color_g"))
            this->slow_bar_color.green=atoi(slow_bar_node->attributes["color_g"].c_str());
        if(slow_bar_node->hasAttribute("color_b"))
            this->slow_bar_color.blue=atoi(slow_bar_node->attributes["color_b"].c_str());
        if(slow_bar_node->hasAttribute("color_a"))
            this->slow_bar_color.alpha=atoi(slow_bar_node->attributes["color_a"].c_str());

        if(slow_bar_node->hasAttribute("cooldown_color_r"))
            this->slow_bar_cooldown_color.red=atoi(slow_bar_node->attributes["cooldown_color_r"].c_str());
        if(slow_bar_node->hasAttribute("cooldown_color_g"))
            this->slow_bar_cooldown_color.green=atoi(slow_bar_node->attributes["cooldown_color_g"].c_str());
        if(slow_bar_node->hasAttribute("cooldown_color_b"))
            this->slow_bar_cooldown_color.blue=atoi(slow_bar_node->attributes["cooldown_color_b"].c_str());
        if(slow_bar_node->hasAttribute("cooldown_color_a"))
            this->slow_bar_cooldown_color.alpha=atoi(slow_bar_node->attributes["cooldown_color_a"].c_str());

        if(slow_bar_node->hasAttribute("rect_offset_x"))
            this->slow_bar_rect_offset_x=atoi(slow_bar_node->attributes["rect_offset_x"].c_str());
        if(slow_bar_node->hasAttribute("rect_offset_y"))
            this->slow_bar_rect_offset_y=atoi(slow_bar_node->attributes["rect_offset_y"].c_str());
        if(slow_bar_node->hasAttribute("rect_height"))
            this->slow_bar_rect_height=atoi(slow_bar_node->attributes["rect_height"].c_str());
        if(slow_bar_node->hasAttribute("rect_width"))
            this->slow_bar_rect_width=atoi(slow_bar_node->attributes["rect_width"].c_str());
    }
}

void Player::inputControl()
{
    bool up_pressed,down_pressed,left_pressed,right_pressed;
    up_pressed=down_pressed=left_pressed=right_pressed=false;

    if(controls["2"]->isDown())
    {
        down_pressed=true;
    }
    if(controls["8"]->isDown())
    {
        up_pressed=true;
    }
    if(controls["4"]->isDown())
    {
        left_pressed=true;
    }
    if(controls["6"]->isDown())
    {
        right_pressed=true;
    }

    if(controls["2"]->isDown())
    {
        if(orientation!="down" && Rosalila()->Sound->soundExists(name+".down"))
            Rosalila()->Sound->playSound(name+".down",1,0);
        orientation="down";
    }
    else if(controls["8"]->isDown())
    {
        if(orientation!="up" && Rosalila()->Sound->soundExists(name+".up"))
            Rosalila()->Sound->playSound(name+".up",1,0);
        orientation="up";
    }
    else if(controls["4"]->isDown())
    {
        if(orientation!="left" && Rosalila()->Sound->soundExists(name+".left"))
            Rosalila()->Sound->playSound(name+".left",1,0);
        orientation="left";
    }
    else if(controls["6"]->isDown())
    {
        if(orientation!="right" && Rosalila()->Sound->soundExists(name+".right"))
            Rosalila()->Sound->playSound(name+".right",1,0);
        orientation="right";
    }
    else
    {
        if(orientation!="idle" && Rosalila()->Sound->soundExists(name+".idle"))
            Rosalila()->Sound->playSound(name+".idle",1,0);
        orientation="idle";
    }

    int velocity_boost=invulnerable_frames_left;

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

    if(controls["a"]->isDown())
    {
        if(!this->shooting && !isParrying() && parries_left>0)
        {
            current_parry_frame=0;
        }
        this->shooting=true;
        if(max_charge!=0 && current_charge==max_charge)
        {
            std::vector<Pattern*> patterns=type["bomb"];
            patterns[0]->bullet->playSound();
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
        if(orientation!="destroyed" && Rosalila()->Sound->soundExists(name+".destroyed"))
            Rosalila()->Sound->playSound(name+".destroyed",1,0);
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
            Rosalila()->Sound->playSound("charge ready",-1, 0);
        }
        charge_ready=true;
    }else
    {
        charge_ready=false;
    }

    if(!charge_ready && !shooting && !getGameOver())
            Mix_Volume(charging_sound_channel, 128);
    else
        Mix_Volume(charging_sound_channel, 0);

    frame++;
}

void Player::bottomRender()
{
    Character::bottomRender();

    if(current_shield>0)
    {
        double shield_transparency = 255.0*getShieldPercentage();

        if(shield_image)
        Rosalila()->Graphics->draw2DImage
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
        Rosalila()->Graphics->draw2DImage
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
}

void Player::topRender()
{
    Character::topRender();

    if(isParrying() && parries_left>=1)
    {
        Image *image=parry_sprites[parries_left-1];
        Rosalila()->Graphics->draw2DImage
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
    Rosalila()->Graphics->draw2DImage
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
    Rosalila()->Graphics->draw2DImage
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

        if(Rosalila()->Receiver->isKeyDown(SDLK_h))
        for(int i=0;i<(int)parry_hitboxes.size();i++)
        {
            Rosalila()->Graphics->drawRectangle(parry_hitboxes[i]->getX()+x,
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
            this->shield_image=Rosalila()->Graphics->getTexture(assets_directory+directory+"/sprites/"+attributes->Attribute("sprite"));
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
            this->charge_image=Rosalila()->Graphics->getTexture(assets_directory+directory+"/sprites/"+attributes->Attribute("sprite"));
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
            Rosalila()->Sound->addSound(name+".parry",assets_directory+directory+"sounds/"+sprites_sound);
        }

        TiXmlNode* parrying_node=parry_node->FirstChild("Parrying");
        if(parrying_node)
        {
            TiXmlElement *parrying_attributes=parrying_node->ToElement();
            if(parrying_attributes->Attribute("sprite")!=NULL)
            {
                this->parrying_image=Rosalila()->Graphics->getTexture(assets_directory+directory+"/sprites/"+parrying_attributes->Attribute("sprite"));
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
                this->parryed_image=Rosalila()->Graphics->getTexture(assets_directory+directory+"/sprites/"+parryed_attributes->Attribute("sprite"));
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
    if(invulnerable_frames_left==0)
    {
        invulnerable_frames_left=15;
        if(!infinite_parries)
        {
            parries_left-=1;
        }
    }
    if(Rosalila()->Sound->soundExists(this->name+".parry"))
    {
        Rosalila()->Sound->playSound(this->name+".parry",sound_channel_base+1,0);
    }
}

bool Player::collidesParry(Hitbox hitbox,int hitbox_x,int hitbox_y,float hitbox_angle)
{
    if(!visible)
        return false;
    for(int i=0;i<(int)parry_hitboxes.size();i++)
        if(parry_hitboxes[i]->getPlacedHitbox(this->x,this->y).collides(hitbox))
            return true;
    return false;
}

void Player::exit()
{
    Mix_Volume(charging_sound_channel, 0);
}
