#include "Player.h"

Player::Player(std::string name,int sound_channel_base,vector<string> intro_input, vector<string> replay_input)
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
    this->slow_bar = NULL;
    this->shield_image = NULL;
    this->charge_image = NULL;
    this->parrying_image = NULL;
    this->parryed_image = NULL;

    this->life_bar = NULL;
    this->flat_shadow_texture = NULL;

    //Sprites animation
    this->animation_velocity=4;
    this->animation_iteration=0;
    this->current_sprite=0;
    this->replay_input=replay_input;
    this->intro_input=intro_input;

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
    parry_sprites.push_back(rosalila()->graphics->getTexture(assets_directory+directory+"sprites/parry/1.png"));
    parry_sprites.push_back(rosalila()->graphics->getTexture(assets_directory+directory+"sprites/parry/2.png"));
    parry_sprites.push_back(rosalila()->graphics->getTexture(assets_directory+directory+"sprites/parry/3.png"));
    //Charge
    rosalila()->sound->addSound("charge ready",assets_directory+directory+"/sounds/charge_ready.ogg");
    rosalila()->sound->addSound("charging",assets_directory+directory+"/sounds/charging.ogg");
    charging_sound_channel=rosalila()->sound->playSound("charging", 21, -1, this->x, true);
}

Player::~Player()
{
    if(this->slow_bar)
        delete this->slow_bar;
    if(this->shield_image)
        delete this->shield_image;
    if(this->charge_image)
        delete this->charge_image;
    if(this->parrying_image)
        delete this->parrying_image;
    if(this->parryed_image)
        delete this->parryed_image;

    for(int i=0; i<(int)parry_hitboxes.size();i++)
    {
        delete parry_hitboxes[i];
    }

    for(int i=0; i<(int)parry_sprites.size();i++)
    {
        delete parry_sprites[i];
    }
}

void Player::loadPlayerFromXML()
{
    loadFromXML();

    Node* root_node = rosalila()->parser->getNodes(assets_directory+directory+"main.xml");

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

    delete root_node;
}

void Player::inputControl()
{
    bool up_pressed,down_pressed,left_pressed,right_pressed;
    up_pressed=down_pressed=left_pressed=right_pressed=false;
    string current_input_replay_store;

    if(isDownWrapper("2"))
    {
        down_pressed=true;
        current_input_replay_store+="2";
    }
    if(isDownWrapper("8"))
    {
        up_pressed=true;
        current_input_replay_store+="8";
    }
    if(isDownWrapper("4"))
    {
        left_pressed=true;
        current_input_replay_store+="4";
    }
    if(isDownWrapper("6"))
    {
        right_pressed=true;
        current_input_replay_store+="6";
    }

    if(isDownWrapper("2"))
    {
        if(orientation!="down" && rosalila()->sound->soundExists(name+".down"))
            rosalila()->sound->playSound(name+".down", 1, 0, this->x, true);
        orientation="down";
    }
    else if(isDownWrapper("8"))
    {
        if(orientation!="up" && rosalila()->sound->soundExists(name+".up"))
            rosalila()->sound->playSound(name+".up", 1, 0, this->x, true);
        orientation="up";
    }
    else if(isDownWrapper("4"))
    {
        if(orientation!="left" && rosalila()->sound->soundExists(name+".left"))
            rosalila()->sound->playSound(name+".left", 1, 0, this->x, true);
        orientation="left";
    }
    else if(isDownWrapper("6"))
    {
        if(orientation!="right" && rosalila()->sound->soundExists(name+".right"))
            rosalila()->sound->playSound(name+".right", 1, 0, this->x, true);
        orientation="right";
    }
    else
    {
        if(orientation!="idle" && rosalila()->sound->soundExists(name+".idle"))
            rosalila()->sound->playSound(name+".idle", 1, 0, this->x, true);
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

    if(isDownWrapper("a"))
    {
        current_input_replay_store+="a";
        if(!this->shooting && !isParrying() && parries_left>0)
        {
            current_parry_frame=0;
        }
        this->shooting=true;
        if(max_charge!=0 && current_charge==max_charge)
        {
            if(!isOnIntro())
            {
                int counter = rosalila()->api_integrator->getStat("TotalCharges") + 1;
                rosalila()->api_integrator->setStat("TotalCharges",counter);

                if(counter >= 3)
                {
                    rosalila()->api_integrator->unlockAchievement("Charge1");
                }
                if(counter >= 20)
                {
                    rosalila()->api_integrator->unlockAchievement("Charge2");
                }
                if(counter >= 100)
                {
                    rosalila()->api_integrator->unlockAchievement("Charge3");
                }
                if(counter >= 200)
                {
                    rosalila()->api_integrator->unlockAchievement("Charge4");
                }
            }

            std::vector<Pattern*> patterns=type["bomb"];
            patterns[0]->bullet->playSound(patterns[0]->x + this->x, true);
            this->addActivePattern(patterns[0]);
        }
        current_charge=0;
    }
    else
    {
        this->shooting=false;
    }

    if(!getGameOver())
        replay_storage.push_back(current_input_replay_store);
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
        if(orientation!="destroyed" && rosalila()->sound->soundExists(name+".destroyed"))
            rosalila()->sound->playSound(name+".destroyed", 1, 0, this->x, true);
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
            rosalila()->sound->playSound("charge ready", -1, 0, this->x, true);
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
        rosalila()->graphics->draw2DImage
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
        rosalila()->graphics->draw2DImage
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
        rosalila()->graphics->draw2DImage
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
    rosalila()->graphics->draw2DImage
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
    rosalila()->graphics->draw2DImage
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

        if(rosalila()->receiver->isKeyDown(SDLK_h))
        for(int i=0;i<(int)parry_hitboxes.size();i++)
        {
            rosalila()->graphics->drawRectangle(parry_hitboxes[i]->getX()+x,
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

    Node* root_node = rosalila()->parser->getNodes(assets_directory+directory+"main.xml");

    this->max_shield=0;
    this->shield_fade=0;
    this->proration=0;
    this->shield_image=NULL;

    Node* shield_node = root_node->getNodeByName("Shield");

    if(shield_node)
    {
        if(shield_node->hasAttribute("max_shield"))
        {
            this->max_shield=atoi(shield_node->attributes["max_shield"].c_str());
        }

        if(shield_node->hasAttribute("shield_fade"))
        {
            this->shield_fade=atoi(shield_node->attributes["shield_fade"].c_str());
        }

        if(shield_node->hasAttribute("shield_fade"))
        {
            this->proration=((double)atoi(shield_node->attributes["shield_fade"].c_str()))/100.0;
        }

        if(shield_node->hasAttribute("sprite"))
        {
            this->shield_image=rosalila()->graphics->getTexture(assets_directory+directory+"/sprites/"+shield_node->attributes["sprite"]);
        }
    }

    charge_image=NULL;
    current_charge=0;
    max_charge=0;
    charge_velocity=0;
    charge_sprite_x=0;
    charge_sprite_y=0;

    Node* charge_node = root_node->getNodeByName("Charge");

    if(charge_node)
    {
        if(charge_node->hasAttribute("max_charge"))
        {
            this->max_charge=atoi(charge_node->attributes["max_charge"].c_str());
        }

        if(charge_node->hasAttribute("charge_velocity"))
        {
            this->charge_velocity=atoi(charge_node->attributes["charge_velocity"].c_str());
        }

        if(charge_node->hasAttribute("x"))
        {
            this->charge_sprite_x=atoi(charge_node->attributes["x"].c_str());
        }

        if(charge_node->hasAttribute("y"))
        {
            this->charge_sprite_y=atoi(charge_node->attributes["y"].c_str());
        }

        if(charge_node->hasAttribute("sprite"))
        {
            this->charge_image=rosalila()->graphics->getTexture(assets_directory+directory+"/sprites/"+charge_node->attributes["sprite"]);
        }
    }


    this->parry_duration=0;

    this->parrying_image=NULL;
    this->parrying_x=0;
    this->parrying_y=0;
    this->parryed_image=NULL;
    this->parryed_x=0;
    this->parryed_y=0;

    Node* parry_node = root_node->getNodeByName("Parry");

    if(parry_node)
    {
        if(parry_node->hasAttribute("duration"))
        {
            this->parry_duration=atoi(parry_node->attributes["duration"].c_str());
        }

        if(parry_node->hasAttribute("sound"))
        {
            std::string sprites_sound=parry_node->attributes["sound"];
            rosalila()->sound->addSound(name+".parry",assets_directory+directory+"sounds/"+sprites_sound);
        }

        Node* parrying_node = parry_node->getNodeByName("Parrying");

        if(parrying_node)
        {
            if(parrying_node->hasAttribute("sprite"))
            {
                this->parrying_image=rosalila()->graphics->getTexture(assets_directory+directory+"/sprites/"+parrying_node->attributes["sprite"]);
            }
            if(parrying_node->hasAttribute("x"))
            {
                this->parrying_x=atoi(parrying_node->attributes["x"].c_str());
            }
            if(parrying_node->hasAttribute("y"))
            {
                this->parrying_y=atoi(parrying_node->attributes["y"].c_str());
            }
        }

        Node* parryed_node = parry_node->getNodeByName("Parryed");

        if(parryed_node)
        {
            if(parryed_node->hasAttribute("sprite"))
            {
                this->parryed_image=rosalila()->graphics->getTexture(assets_directory+directory+"/sprites/"+parryed_node->attributes["sprite"]);
            }
            if(parryed_node->hasAttribute("x"))
            {
                this->parryed_x=atoi(parryed_node->attributes["x"].c_str());
            }
            if(parryed_node->hasAttribute("y"))
            {
                this->parryed_y=atoi(parryed_node->attributes["y"].c_str());
            }
        }

        vector<Node*> hitboxes_nodes = parry_node->getNodesByName("Hitboxes");

        for(int i=0;i<(int)hitboxes_nodes.size();i++)
        {
            int hitbox_x=atoi(hitboxes_nodes[i]->attributes["x"].c_str());
            int hitbox_y=atoi(hitboxes_nodes[i]->attributes["y"].c_str());
            int hitbox_width=atoi(hitboxes_nodes[i]->attributes["width"].c_str());
            int hitbox_height=atoi(hitboxes_nodes[i]->attributes["height"].c_str());
            int hitbox_angle=atoi(hitboxes_nodes[i]->attributes["angle"].c_str());
            Hitbox* hitbox=new Hitbox(hitbox_x,hitbox_y,hitbox_width,hitbox_height,hitbox_angle);
            this->parry_hitboxes.push_back(hitbox);
        }
    }

    delete root_node;

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
    if(rosalila()->sound->soundExists(this->name+".parry"))
    {
        rosalila()->sound->playSound(this->name+".parry", sound_channel_base+1, 0, this->x, true);
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

bool Player::isDownWrapper(string button_map)
{
    if(replay_input.size()>0)
    {
        if(frame>=(int)replay_input.size())
            return false;
        for(int i=0;i<(int)replay_input[frame].size();i++)
            if(replay_input[frame][i]==button_map[0])
                return true;
        return false;
    }else if(intro_input.size()>0)
    {
        if(frame>= (int)intro_input.size())
        {
            rosalila()->graphics->grayscale_effect.set(1,0.003);
            return rosalila()->receiver->isDown(button_map);
        }

        for(int i=0;i<(int)intro_input[frame].size();i++)
            if(intro_input[frame][i]==button_map[0])
                return true;
        return false;
    }
    return rosalila()->receiver->isDown(button_map);
}

bool Player::isOnIntro()
{
    return this->frame<(int)this->intro_input.size();
}
