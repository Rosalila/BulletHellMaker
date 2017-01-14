#include "Character.h"

Character::Character(std::string name,int sound_channel_base)
{
    //Setting up the other variables
    this->name=name;
    this->directory="";
    this->active_patterns=new std::list<Pattern*>;
    this->x=500;
    this->y=500;
    this->shooting=true;
    this->orientation="idle";
    this->current_type="1";
    this->visible=true;
    this->life_bar = NULL;
    this->flat_shadow_texture = NULL;

    //Sprites animation
    this->animation_iteration=0;
    this->current_sprite=0;

    this->life_bar_x=0;
    this->life_bar_y=0;

    this->iteration=0;

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

    this->sound_channel_base=sound_channel_base;

    //Flat Shadow
    flat_shadow_texture = NULL;

    loadFromXML();
}

Character::~Character()
{
    for(map< string,vector<Image*> >::iterator i = sprites.begin(); i!=sprites.end(); i++)
    {
        for(int j=0;j<(int)(*i).second.size();j++)
        {
            delete (*i).second[j];
        }
    }

    for(map<string,Bullet*>::iterator i = bullets.begin(); i!=bullets.end(); i++)
    {
        delete (*i).second;
    }

    for(map<string, vector<Pattern*> >::iterator i = type.begin(); i!=type.end(); i++)
    {
        for(int j=0;j<(int)(*i).second.size();j++)
        {
            delete (*i).second[j];
        }
    }

    for(std::list<Pattern*>::iterator i = (*active_patterns).begin(); i!=(*active_patterns).end(); i++)
    {
        delete (*i);
    }

    delete active_patterns;

    if(this->life_bar)
        delete this->life_bar;

    for(int i=0;i<(int)hitboxes.size();i++)
    {
        delete hitboxes[i];
    }

    if(this->flat_shadow_texture)
        delete this->flat_shadow_texture;

    for(int i=0;i<(int)shadow_align_points_left.size();i++)
    {
        delete shadow_align_points_left[i];
    }

    for(int i=0;i<(int)shadow_align_points_right.size();i++)
    {
        delete shadow_align_points_right[i];
    }

    for(int i=0;i<(int)shadow_align_points_top.size();i++)
    {
        delete shadow_align_points_top[i];
    }

    for(int i=0;i<(int)inbetween_shadow_align_points_left.size();i++)
    {
        delete inbetween_shadow_align_points_left[i];
    }

    for(int i=0;i<(int)inbetween_shadow_align_points_right.size();i++)
    {
        delete inbetween_shadow_align_points_right[i];
    }

    for(int i=0;i<(int)inbetween_shadow_align_points_top.size();i++)
    {
        delete inbetween_shadow_align_points_top[i];
    }
}

void Character::loadFromXML()
{
    loadMainXML();

    loadBulletsXML();

    loadPatternsXML();
}

void Character::loadMainXML()
{
    Node* root_node = rosalila()->parser->getNodes(assets_directory+directory+"main.xml");

    this->velocity=5;

    Node* attributes_node = root_node->getNodeByName("Attributes");

    if(attributes_node->hasAttribute("velocity"))
    {
        this->velocity=atoi(attributes_node->attributes["velocity"].c_str());
    }

    this->animation_velocity=5;
    if(attributes_node->hasAttribute("animation_velocity"))
    {
        this->animation_velocity=atoi(attributes_node->attributes["animation_velocity"].c_str());
    }

    this->max_hp=100;
    this->hp=100;
    if(attributes_node->hasAttribute("hp"))
    {
        this->max_hp=atoi(attributes_node->attributes["hp"].c_str());
    }
    this->hp=this->max_hp;

    this->x=100;
    if(attributes_node->hasAttribute("initial_x"))
    {
        this->x=atoi(attributes_node->attributes["initial_x"].c_str());
    }

    this->y=500;
    if(attributes_node->hasAttribute("initial_y"))
    {
        this->y=atoi(attributes_node->attributes["initial_y"].c_str());
    }

    this->life_bar_x=0;
    this->life_bar_y=0;
    this->life_bar_rect_offset_x=0;
    this->life_bar_rect_offset_y=0;
    this->life_bar_rect_height=0;
    this->life_bar_rect_width=0;
    this->color.red=0;
    this->color.green=0;
    this->color.blue=0;
    this->color.alpha=255;

    Node* life_bar_node = root_node->getNodeByName("LifeBar");

    if(life_bar_node!=NULL)
    {
        if(life_bar_node->hasAttribute("x"))
            this->life_bar_x=atoi(life_bar_node->attributes["x"].c_str());

        if(life_bar_node->hasAttribute("y"))
            this->life_bar_y=atoi(life_bar_node->attributes["y"].c_str());

        if(life_bar_node->hasAttribute("color_r"))
            this->color.red=atoi(life_bar_node->attributes["color_r"].c_str());

        if(life_bar_node->hasAttribute("color_g"))
            this->color.green=atoi(life_bar_node->attributes["color_g"].c_str());

        if(life_bar_node->hasAttribute("color_b"))
            this->color.blue=atoi(life_bar_node->attributes["color_b"].c_str());

        if(life_bar_node->hasAttribute("color_a"))
            this->color.alpha=atoi(life_bar_node->attributes["color_a"].c_str());

        if(life_bar_node->hasAttribute("rect_offset_x"))
            this->life_bar_rect_offset_x=atoi(life_bar_node->attributes["color_offset_x"].c_str());

        if(life_bar_node->hasAttribute("rect_offset_y"))
            this->life_bar_rect_offset_y=atoi(life_bar_node->attributes["color_offset_y"].c_str());

        if(life_bar_node->hasAttribute("rect_height"))
            this->life_bar_rect_height=atoi(life_bar_node->attributes["rect_height"].c_str());

        if(life_bar_node->hasAttribute("rect_width"))
            this->life_bar_rect_width=atoi(life_bar_node->attributes["rect_width"].c_str());

        if(life_bar_node->hasAttribute("image"))
        {
            this->life_bar=rosalila()->graphics->getTexture(assets_directory+directory+life_bar_node->attributes["image"]);
        }
    }

    Node* hitboxes_node = root_node->getNodeByName("Hitboxes");

    for(map<string,Node*>::iterator hitbox_iterator=hitboxes_node->nodes.begin();
        hitbox_iterator!=hitboxes_node->nodes.end();
        hitbox_iterator++)
    {
        Node* hitbox_node=hitbox_iterator->second;

        int hitbox_x=atoi(hitbox_node->attributes["x"].c_str());
        int hitbox_y=atoi(hitbox_node->attributes["y"].c_str());
        int hitbox_width=atoi(hitbox_node->attributes["width"].c_str());
        int hitbox_height=atoi(hitbox_node->attributes["height"].c_str());
        int hitbox_angle=atoi(hitbox_node->attributes["angle"].c_str());

        Hitbox* hitbox=new Hitbox(hitbox_x,hitbox_y,hitbox_width,hitbox_height,hitbox_angle);
        this->hitboxes.push_back(hitbox);

    }


    Node* sounds_node = root_node->getNodeByName("Sounds");

    if(sounds_node)
    {
        if(sounds_node->hasAttribute("hit"))
        {
            rosalila()->sound->addSound(this->name+".hit",assets_directory+directory+"/sounds/"+sounds_node->attributes["hit"]);
        }
    }

    vector<Node*> sprites_nodes = root_node->getNodesByName("Sprites");

    for(int i=0;i<(int)sprites_nodes.size();i++)
    {
        std::vector<Image*>sprites_vector;

        std::string sprites_orientation=sprites_nodes[i]->attributes["orientation"];

        if(sprites_nodes[i]->hasAttribute("sound"))
        {
            std::string sprites_sound=sprites_nodes[i]->attributes["sound"];
            rosalila()->sound->addSound(name+"."+sprites_orientation,assets_directory+directory+"sounds/"+sprites_sound);
        }

        vector<Node*> sprite_nodes = sprites_nodes[i]->getNodesByName("Sprite");

        for(int j=0;j<(int)sprite_nodes.size();j++)
        {
            string sprite_path = sprite_nodes[j]->attributes["path"];
            sprites_vector.push_back(rosalila()->graphics->getTexture(assets_directory+directory+"sprites/"+sprite_path));
        }

        sprites[sprites_orientation]=sprites_vector;
    }

    Node* flat_shadow_node = root_node->getNodeByName("FlatShadow");

    if(flat_shadow_node)
    {
        flat_shadow_texture = rosalila()->graphics->getTexture(assets_directory+directory+"sprites/"+flat_shadow_node->attributes["image_path"]);

        Node* case_right_node = flat_shadow_node->getNodeByName("CaseRight");

        if(case_right_node)
        {
            vector<Node*> case_right_point_nodes = case_right_node->getNodesByName("Point");

            for(int i=0;i<(int)case_right_point_nodes.size();i++)
            {
                int x=atoi(case_right_point_nodes[i]->attributes["x"].c_str());
                int y=atoi(case_right_point_nodes[i]->attributes["y"].c_str());
                shadow_align_points_right.push_back(new Point(x,y));
            }

            Node* case_right_inbetween_node = case_right_node->getNodeByName("Inbetween");

            if(case_right_inbetween_node)
            {
                vector<Node*> case_right_inbetween_point_nodes = case_right_inbetween_node->getNodesByName("Point");

                for(int i=0;i<(int)case_right_inbetween_point_nodes.size();i++)
                {
                    int x=atoi(case_right_inbetween_point_nodes[i]->attributes["x"].c_str());
                    int y=atoi(case_right_inbetween_point_nodes[i]->attributes["y"].c_str());
                    inbetween_shadow_align_points_right.push_back(new Point(x,y));
                }
            }
        }

        Node* case_left_node = flat_shadow_node->getNodeByName("CaseLeft");

        if(case_left_node)
        {
            vector<Node*> case_left_point_nodes = case_left_node->getNodesByName("Point");

            for(int i=0;i<(int)case_left_point_nodes.size();i++)
            {
                int x=atoi(case_left_point_nodes[i]->attributes["x"].c_str());
                int y=atoi(case_left_point_nodes[i]->attributes["y"].c_str());
                shadow_align_points_left.push_back(new Point(x,y));
            }

            Node* case_left_inbetween_node = case_left_node->getNodeByName("Inbetween");

            if(case_left_inbetween_node)
            {
                vector<Node*> case_left_inbetween_point_nodes = case_left_inbetween_node->getNodesByName("Point");

                for(int i=0;i<(int)case_left_inbetween_point_nodes.size();i++)
                {
                    int x=atoi(case_left_inbetween_point_nodes[i]->attributes["x"].c_str());
                    int y=atoi(case_left_inbetween_point_nodes[i]->attributes["y"].c_str());
                    inbetween_shadow_align_points_left.push_back(new Point(x,y));
                }
            }
        }

        Node* case_top_node = flat_shadow_node->getNodeByName("CaseTop");

        if(case_top_node)
        {
            vector<Node*> case_top_point_nodes = case_top_node->getNodesByName("Point");

            for(int i=0;i<(int)case_top_point_nodes.size();i++)
            {
                int x=atoi(case_top_point_nodes[i]->attributes["x"].c_str());
                int y=atoi(case_top_point_nodes[i]->attributes["y"].c_str());
                shadow_align_points_top.push_back(new Point(x,y));
            }

            Node* case_top_inbetween_node = case_top_node->getNodeByName("Inbetween");

            if(case_top_inbetween_node)
            {
                vector<Node*> case_top_inbetween_point_nodes = case_top_node->getNodeByName("Inbetween")->getNodesByName("Point");

                for(int i=0;i<(int)case_top_inbetween_point_nodes.size();i++)
                {
                    int x=atoi(case_top_inbetween_point_nodes[i]->attributes["x"].c_str());
                    int y=atoi(case_top_inbetween_point_nodes[i]->attributes["y"].c_str());
                    inbetween_shadow_align_points_top.push_back(new Point(x,y));
                }
            }
        }
    }

    delete root_node;
}

void Character::loadBulletsXML()
{
    Node* root_node = rosalila()->parser->getNodes(assets_directory+directory+"bullets.xml");

    vector<Node*> bullet_nodes = root_node->getNodesByName("Bullet");

    for(int i=0;i<(int)bullet_nodes.size();i++)
    {
        std::string node_name=bullet_nodes[i]->attributes["name"];
        vector<string> random_sounds;

        if(bullet_nodes[i]->hasAttribute("sound"))
        {
            std::string sound=assets_directory+directory+"sounds/"+bullet_nodes[i]->attributes["sound"];
            rosalila()->sound->addSound("bullet."+node_name,sound);
            random_sounds.push_back("bullet."+node_name);
        }

        if(bullet_nodes[i]->hasAttribute("sound_hit"))
        {
            std::string sound_hit=assets_directory+directory+"sounds/"+bullet_nodes[i]->attributes["sound_hit"];
            rosalila()->sound->addSound("bullet_hit."+node_name,sound_hit);
        }

        int randomize_sound_frequency=1;
        if(bullet_nodes[i]->hasAttribute("randomize_sound_frequency"))
        {
            randomize_sound_frequency=atoi(bullet_nodes[i]->attributes["randomize_sound_frequency"].c_str());
        }

        int arpeggio_length=1;
        if(bullet_nodes[i]->hasAttribute("arpeggio_length"))
        {
            arpeggio_length=atoi(bullet_nodes[i]->attributes["arpeggio_length"].c_str());
        }

        int sound_channel=0;
        if(bullet_nodes[i]->hasAttribute("sound_channel"))
        {
            sound_channel=atoi(bullet_nodes[i]->attributes["sound_channel"].c_str());
            if(sound_channel!=-1)
                sound_channel+=sound_channel_base;
        }

        int damage=1;
        if(bullet_nodes[i]->hasAttribute("damage"))
        {
            damage=atoi(bullet_nodes[i]->attributes["damage"].c_str());
        }
        vector<Node*> sprite_nodes = bullet_nodes[i]->getNodesByName("Sprite");
        vector<Image*>sprites_temp;
        for(int j=0;j<(int)sprite_nodes.size();j++)
        {
            sprites_temp.push_back(rosalila()->graphics->getTexture(assets_directory+directory+"sprites/"+sprite_nodes[j]->attributes["path"]));
        }
        vector<Node*> hitbox_nodes = bullet_nodes[i]->getNodesByName("Hitbox");
        vector<Hitbox*>hitboxes_temp;
        for(int j=0;j<(int)hitbox_nodes.size();j++)
        {
            int x=0;
            if(hitbox_nodes[j]->hasAttribute("x"))
            {
                x=atoi(hitbox_nodes[j]->attributes["x"].c_str());
            }

            int y=0;
            if(hitbox_nodes[j]->hasAttribute("y"))
            {
                y=atoi(hitbox_nodes[j]->attributes["y"].c_str());
            }

            int width=0;
            if(hitbox_nodes[j]->hasAttribute("width"))
            {
                width=atoi(hitbox_nodes[j]->attributes["width"].c_str());
            }

            int height=0;
            if(hitbox_nodes[j]->hasAttribute("height"))
            {
                height=atoi(hitbox_nodes[j]->attributes["height"].c_str());
            }

            int angle=0;
            if(hitbox_nodes[j]->hasAttribute("angle"))
            {
                angle=atoi(hitbox_nodes[j]->attributes["angle"].c_str());
            }

            hitboxes_temp.push_back(new Hitbox(x,y,width,height,angle));
        }

        Node* onhit_node = bullet_nodes[i]->getNodeByName("OnHit");
        vector<Image*>sprites_onhit_temp;
        if(onhit_node)
        {
            vector<Node*> sprite_node = onhit_node->getNodesByName("Sprite");
            for(int j=0;j<(int)sprite_node.size();j++)
            {
                sprites_onhit_temp.push_back(rosalila()->graphics->getTexture(assets_directory+directory+"sprites/"+sprite_node[j]->attributes["path"]));
            }
        }
        Node* random_sound_node = bullet_nodes[i]->getNodeByName("RandomSound");

        if(random_sound_node)
        {
            vector<Node*> sound_nodes = random_sound_node->getNodesByName("Sound");

            for(int j=0;j<(int)sound_nodes.size();j++)
            {
                std::string sound=assets_directory+directory+"sounds/"+sound_nodes[j]->attributes["path"];
                rosalila()->sound->addSound("bullet."+node_name+sound_nodes[j]->attributes["path"],sound);
                random_sounds.push_back("bullet."+node_name+sound_nodes[j]->attributes["path"]);
            }
        }

        int width=0;
        if(bullet_nodes[i]->hasAttribute("width"))
        {
            width=atoi(bullet_nodes[i]->attributes["width"].c_str());
        }

        int height=0;
        if(bullet_nodes[i]->hasAttribute("height"))
        {
            height=atoi(bullet_nodes[i]->attributes["height"].c_str());
        }

        Node* color_node = bullet_nodes[i]->getNodeByName("Color");

        int red=0;
        int green=0;
        int blue=0;
        int alpha=255;
        if(color_node)
        {
            if(color_node->hasAttribute("red"))
                red=atoi(color_node->attributes["red"].c_str());
            if(color_node->hasAttribute("green"))
                green=atoi(color_node->attributes["green"].c_str());
            if(color_node->hasAttribute("blue"))
                blue=atoi(color_node->attributes["blue"].c_str());
            if(color_node->hasAttribute("alpha"))
                alpha=atoi(color_node->attributes["alpha"].c_str());
        }


        bullets[node_name]=new Bullet(node_name,sprites_temp,sprites_onhit_temp,width,height,Color(red,green,blue,alpha),hitboxes_temp,random_sounds,randomize_sound_frequency,arpeggio_length,damage,sound_channel);
    }

    delete root_node;
}

Pattern* Character::loadPatternXML(Node* pattern_node)
{
    std::string bullet_name=pattern_node->attributes["bullet"];
    Bullet*bullet=bullets[bullet_name];

    int velocity=0;
    if(pattern_node->hasAttribute("velocity"))
        velocity=atoi(pattern_node->attributes["velocity"].c_str());

    int max_velocity=9999999;
    if(pattern_node->hasAttribute("max_velocity"))
        max_velocity=atoi(pattern_node->attributes["max_velocity"].c_str());

    int acceleration=0;
    if(pattern_node->hasAttribute("acceleration"))
        acceleration=atoi(pattern_node->attributes["acceleration"].c_str());

    int a_frequency=0;
    if(pattern_node->hasAttribute("a_frequency"))
        a_frequency=atoi(pattern_node->attributes["a_frequency"].c_str());

    int angle=0;
    if(pattern_node->hasAttribute("angle"))
        angle=atoi(pattern_node->attributes["angle"].c_str());

    int angle_change=0;
    if(pattern_node->hasAttribute("angle_change"))
        angle_change=atoi(pattern_node->attributes["angle_change"].c_str());

    int stop_ac_at=-1;
    if(pattern_node->hasAttribute("stop_ac_at"))
        stop_ac_at=atoi(pattern_node->attributes["stop_ac_at"].c_str());

    int ac_frequency=0;
    if(pattern_node->hasAttribute("ac_frequency"))
        ac_frequency=atoi(pattern_node->attributes["ac_frequency"].c_str());

    int offset_x=0;
    if(pattern_node->hasAttribute("offset_x"))
        offset_x=atoi(pattern_node->attributes["offset_x"].c_str());

    int offset_y=0;
    if(pattern_node->hasAttribute("offset_y"))
        offset_y=atoi(pattern_node->attributes["offset_y"].c_str());

    int startup=0;
    if(pattern_node->hasAttribute("startup"))
        startup=atoi(pattern_node->attributes["startup"].c_str());

    int cooldown=0;
    if(pattern_node->hasAttribute("cooldown"))
        cooldown=atoi(pattern_node->attributes["cooldown"].c_str());

    int animation_velocity=0;
    if(pattern_node->hasAttribute("animation_velocity"))
        animation_velocity=atoi(pattern_node->attributes["animation_velocity"].c_str());

    double auto_scale=0;
    if(pattern_node->hasAttribute("auto_scale"))
        auto_scale=atof(pattern_node->attributes["auto_scale"].c_str());

    int duration=-1;
    if(pattern_node->hasAttribute("duration"))
        duration=atoi(pattern_node->attributes["duration"].c_str());

    int random_angle=0;
    if(pattern_node->hasAttribute("random_angle"))
        random_angle=atoi(pattern_node->attributes["random_angle"].c_str());

    bool aim_player=false;
    if(pattern_node->hasAttribute("aim_player"))
        aim_player=pattern_node->attributes["aim_player"]=="yes";

    int bullet_rotation=0;
    if(pattern_node->hasAttribute("bullet_rotation"))
        bullet_rotation=atoi(pattern_node->attributes["bullet_rotation"].c_str());

    int br_change=0;
    if(pattern_node->hasAttribute("br_change"))
        br_change=atoi(pattern_node->attributes["br_change"].c_str());

    bool independent_br=false;
    if(pattern_node->hasAttribute("independent_br"))
        independent_br=pattern_node->attributes["independent_br"]=="yes";

    bool freeze=false;
    if(pattern_node->hasAttribute("freeze"))
        freeze=pattern_node->attributes["freeze"]=="yes";

    bool homing = false;
    if (pattern_node->hasAttribute("homing"))
         homing = pattern_node->attributes["homing"]=="yes";

    bool collides_bullets=false;
    if(pattern_node->hasAttribute("collides_bullets"))
        collides_bullets=pattern_node->attributes["collides_bullets"]=="yes";

    bool collides_opponent=true;
    if(pattern_node->hasAttribute("collides_opponent"))
        collides_opponent=pattern_node->attributes["collides_opponent"]=="yes";

    bool undestructable=false;
    if(pattern_node->hasAttribute("undestructable"))
        undestructable=pattern_node->attributes["undestructable"]=="yes";

    std::map<int, vector<Modifier*> >*pattern_modifiers=new std::map<int, vector<Modifier*> >();

    vector<Node*> modifier_nodes = pattern_node->getNodesByName("Modifier");
    for(int i=0;i<(int)modifier_nodes.size();i++)
    {
        int at=atoi(modifier_nodes[i]->attributes["at"].c_str());
        (*pattern_modifiers)[at]=loadModifierXML(modifier_nodes[i]);
    }

    return new Pattern(velocity,max_velocity,acceleration,a_frequency,angle,angle_change,stop_ac_at,ac_frequency,animation_velocity, auto_scale,bullet,offset_x,offset_y,
                                           startup,cooldown,duration,random_angle,aim_player,bullet_rotation,br_change,independent_br,freeze, homing,collides_bullets,collides_opponent,undestructable,pattern_modifiers,&bullets);
}

vector<Modifier*> Character::loadModifierXML(Node* modifier_node)
{
    vector<Modifier*> temp_modifiers;

    if(modifier_node->hasAttribute("bullet"))
    {
        std::string value=modifier_node->attributes["bullet"];
        temp_modifiers.push_back(new Modifier("bullet",value));
    }

    if(modifier_node->hasAttribute("velocity"))
    {
        std::string value=modifier_node->attributes["velocity"];
        temp_modifiers.push_back(new Modifier("velocity",value));
    }

    if(modifier_node->hasAttribute("max_velocity"))
    {
        std::string value=modifier_node->attributes["max_velocity"];
        temp_modifiers.push_back(new Modifier("max_velocity",value));
    }

    if(modifier_node->hasAttribute("acceleration"))
    {
        std::string value=modifier_node->attributes["acceleration"];
        temp_modifiers.push_back(new Modifier("acceleration",value));
    }

    if(modifier_node->hasAttribute("a_frequency"))
    {
        std::string value=modifier_node->attributes["a_frequency"];
        temp_modifiers.push_back(new Modifier("a_frequency",value));
    }

    if(modifier_node->hasAttribute("angle"))
    {
        std::string value=modifier_node->attributes["angle"];
        temp_modifiers.push_back(new Modifier("angle",value));
    }

    if(modifier_node->hasAttribute("angle_change"))
    {
        std::string value=modifier_node->attributes["angle_change"];
        temp_modifiers.push_back(new Modifier("angle_change",value));
    }

    if(modifier_node->hasAttribute("stop_ac_at"))
    {
        std::string value=modifier_node->attributes["stop_ac_at"];
        temp_modifiers.push_back(new Modifier("stop_ac_at",value));
    }

    if(modifier_node->hasAttribute("ac_frequency"))
    {
        std::string value=modifier_node->attributes["ac_frequency"];
        temp_modifiers.push_back(new Modifier("ac_frequency",value));
    }

    if(modifier_node->hasAttribute("animation_velocity"))
    {
        std::string value=modifier_node->attributes["animation_velocity"];
        temp_modifiers.push_back(new Modifier("animation_velocity",value));
    }

    if(modifier_node->hasAttribute("offset_x"))
    {
        std::string value=modifier_node->attributes["offset_x"];
        temp_modifiers.push_back(new Modifier("offset_x",value));
    }

    if(modifier_node->hasAttribute("offset_y"))
    {
        std::string value=modifier_node->attributes["offset_y"];
        temp_modifiers.push_back(new Modifier("offset_y",value));
    }

    if(modifier_node->hasAttribute("startup"))
    {
        std::string value=modifier_node->attributes["startup"];
        temp_modifiers.push_back(new Modifier("startup",value));
    }

    if(modifier_node->hasAttribute("cooldown"))
    {
        std::string value=modifier_node->attributes["cooldown"];
        temp_modifiers.push_back(new Modifier("cooldown",value));
    }

    if(modifier_node->hasAttribute("duration"))
    {
        std::string value=modifier_node->attributes["duration"];
        temp_modifiers.push_back(new Modifier("duration",value));
    }

    if(modifier_node->hasAttribute("random_angle"))
    {
        std::string value=modifier_node->attributes["random_angle"];
        temp_modifiers.push_back(new Modifier("random_angle",value));
    }

    if(modifier_node->hasAttribute("aim_player"))
    {
        std::string value=modifier_node->attributes["aim_player"];
        temp_modifiers.push_back(new Modifier("aim_player",value));
    }

    if(modifier_node->hasAttribute("bullet_rotation"))
    {
        std::string value=modifier_node->attributes["bullet_rotation"];
        temp_modifiers.push_back(new Modifier("bullet_rotation",value));
    }

    if(modifier_node->hasAttribute("br_change"))
    {
        std::string value=modifier_node->attributes["br_change"];
        temp_modifiers.push_back(new Modifier("br_change",value));
    }

    if(modifier_node->hasAttribute("independent_br"))
    {
        std::string value=modifier_node->attributes["independent_br"];
        temp_modifiers.push_back(new Modifier("independent_br",value));
    }
    if(modifier_node->hasAttribute("freeze"))
    {
        std::string value=modifier_node->attributes["freeze"];
        temp_modifiers.push_back(new Modifier("freeze",value));
    }

    if(modifier_node->hasAttribute("homing"))
    {
        std::string value=modifier_node->attributes["homing"];
        temp_modifiers.push_back(new Modifier("homing",value));
    }

    return temp_modifiers;
}

void Character::loadPatternsXML()
{
    Node* root_node = rosalila()->parser->getNodes(assets_directory+directory+"patterns.xml");

    vector<Node*> type_nodes = root_node->getNodesByName("Type");

    for(int i=0;i<(int)type_nodes.size();i++)
    {
        std::string type_name=type_nodes[i]->attributes["name"];

        std::vector<Pattern*> patterns;

        vector<Node*> pattern_nodes = type_nodes[i]->getNodesByName("Pattern");

        for(int j=0;j<(int)pattern_nodes.size();j++)
        {
            patterns.push_back(loadPatternXML(pattern_nodes[j]));
        }

        vector<Node*> repeat_nodes = type_nodes[i]->getNodesByName("Repeat");

        for(int j=0;j<(int)repeat_nodes.size();j++)
        {
            int amount = atoi(repeat_nodes[j]->attributes["amount"].c_str());

            vector<Node*> pattern_nodes = repeat_nodes[j]->getNodesByName("Pattern");

            for(int k=0;k<(int)pattern_nodes.size();k++)
            {
                for(int l=0;l<amount;l++)
                {
                    Pattern* p = loadPatternXML(pattern_nodes[k]);

                    if(repeat_nodes[j]->hasAttribute("velocity"))
                        p->velocity+=atoi(repeat_nodes[j]->attributes["velocity"].c_str())*l;

                    if(repeat_nodes[j]->hasAttribute("max_velocity"))
                        p->max_velocity+=atoi(repeat_nodes[j]->attributes["max_velocity"].c_str())*l;

                    if(repeat_nodes[j]->hasAttribute("acceleration"))
                        p->acceleration+=atoi(repeat_nodes[j]->attributes["acceleration"].c_str())*l;

                    if(repeat_nodes[j]->hasAttribute("a_frequency"))
                        p->a_frequency+=atoi(repeat_nodes[j]->attributes["a_frequency"].c_str())*l;

                    if(repeat_nodes[j]->hasAttribute("angle"))
                        p->angle+=atoi(repeat_nodes[j]->attributes["angle"].c_str())*l;

                    if(repeat_nodes[j]->hasAttribute("angle_change"))
                        p->angle_change+=atoi(repeat_nodes[j]->attributes["angle_change"].c_str())*l;

                    if(repeat_nodes[j]->hasAttribute("stop_ac_at"))
                        p->stop_ac_at+=atoi(repeat_nodes[j]->attributes["stop_ac_at"].c_str())*l;

                    if(repeat_nodes[j]->hasAttribute("ac_frequency"))
                        p->ac_frequency+=atoi(repeat_nodes[j]->attributes["ac_frequency"].c_str())*l;

                    if(repeat_nodes[j]->hasAttribute("offset_x"))
                        p->offset_x+=atoi(repeat_nodes[j]->attributes["offset_x"].c_str())*l;

                    if(repeat_nodes[j]->hasAttribute("offset_y"))
                        p->offset_y+=atoi(repeat_nodes[j]->attributes["offset_y"].c_str())*l;

                    if(repeat_nodes[j]->hasAttribute("startup"))
                        p->startup+=atoi(repeat_nodes[j]->attributes["startup"].c_str())*l;

                    if(repeat_nodes[j]->hasAttribute("cooldown"))
                        p->cooldown+=atoi(repeat_nodes[j]->attributes["cooldown"].c_str())*l;

                    if(repeat_nodes[j]->hasAttribute("animation_velocity"))
                        p->animation_velocity+=atoi(repeat_nodes[j]->attributes["animation_velocity"].c_str())*l;

                    if(repeat_nodes[j]->hasAttribute("random_angle"))
                        p->random_angle+=atoi(repeat_nodes[j]->attributes["random_angle"].c_str())*l;

                    if(repeat_nodes[j]->hasAttribute("bullet_rotation"))
                        p->bullet_rotation+=atoi(repeat_nodes[j]->attributes["bullet_rotation"].c_str())*l;

                    if(repeat_nodes[j]->hasAttribute("br_change"))
                        p->br_change+=atoi(repeat_nodes[j]->attributes["br_change"].c_str())*l;

                    patterns.push_back(p);

                }
            }
        }

        type[type_name]=patterns;
    }

    delete root_node;
}

void Character::logic(int stage_velocity)
{
    animationControl();
    spellControl(stage_velocity);
    if(current_color_effect_r<255)
        current_color_effect_r++;
    if(current_color_effect_g<255)
        current_color_effect_g++;
    if(current_color_effect_b<255)
        current_color_effect_b++;
    if(current_color_effect_a<255)
        current_color_effect_a++;
}

void Character::animationControl()
{
    if(orientation=="destroyed")
        visible=false;
    if(animation_iteration>=animation_velocity)
    {
        current_sprite++;
        if(current_sprite>=(int)sprites[orientation].size())
        {
            current_sprite=0;
        }
        animation_iteration=0;
    }
    if(getIterateSlowdownFlag())
        animation_iteration++;
}

void Character::spellControl(int stage_velocity)
{
    std::vector<Pattern*> patterns=type[current_type];
    for(int i=0;i<(int)patterns.size();i++)
    {
        if(shooting && this->hp!=0)
        {
            patterns[i]->updateStateShouting();
            if(patterns[i]->isReady())
            {
                patterns[i]->bullet->playSound(patterns[i]->x+this->x, true);
                this->addActivePattern(patterns[i]);
            }
        }else
        {
            patterns[i]->updateStateNotShouting();
        }
    }

    for (std::list<Pattern*>::iterator pattern = active_patterns->begin(); pattern != active_patterns->end(); pattern++)
        ((Pattern*)*pattern)->logic(stage_velocity);
}

void Character::bottomRender()
{
    if(shake_time>0)
    {
        shake_time--;
        if(shake_time==0)
        {
            current_screen_shake_x = 0;
            current_screen_shake_y = 0;
        }else
        {
            current_screen_shake_x = (rand()*10000)%shake_magnitude;
            current_screen_shake_y = (rand()*10000)%shake_magnitude;
        }
    }

    rosalila()->graphics->draw2DImage
    (   sprites[orientation][current_sprite],
        sprites[orientation][current_sprite]->getWidth(),sprites[orientation][current_sprite]->getHeight(),
        this->x-sprites[orientation][current_sprite]->getWidth()/2+current_screen_shake_x,
        this->y-sprites[orientation][current_sprite]->getHeight()/2+current_screen_shake_y,
        1.0,
        0.0,
        false,
        0,0,
        Color(current_color_effect_r,current_color_effect_g,current_color_effect_b,current_color_effect_a),
        0,0,
        true,
        FlatShadow(flat_shadow_texture,2,60,0,700,-500,
                   shadow_align_points_left,shadow_align_points_right,shadow_align_points_top,
                   inbetween_shadow_align_points_left,inbetween_shadow_align_points_right,inbetween_shadow_align_points_top));

    if(rosalila()->receiver->isKeyDown(SDLK_h))
    {
        for(int i=0;i<(int)hitboxes.size();i++)
        {
            rosalila()->graphics->drawRectangle(hitboxes[i]->getX()+x,
                                   hitboxes[i]->getY()+y,
                                   hitboxes[i]->getWidth(),hitboxes[i]->getHeight(),
                                   hitboxes[i]->angle,100,0,0,100,true);
        }
    }
}

void Character::topRender()
{
//    for (std::list<Pattern*>::iterator pattern = active_patterns->begin(); pattern != active_patterns->end(); pattern++)
//        ((Pattern*)*pattern)->render();

    vector<DrawableRectangle*>rectangles;
    for (std::list<Pattern*>::iterator pattern = active_patterns->begin(); pattern != active_patterns->end(); pattern++)
    {
        if(!(*pattern)->is_hit)
        {
            double bullet_width = (*pattern)->bullet->width;
            bullet_width *= 1-((*pattern)->frame * (*pattern)->auto_scale);

            double bullet_heigth = (*pattern)->bullet->height;

            DrawableRectangle* rectangle = new DrawableRectangle(
                                                 (*pattern)->x - bullet_width/2,
                                                 (*pattern)->y - bullet_heigth/2,
                                                 bullet_width,
                                                 bullet_heigth,
                                                 (*pattern)->angle+(*pattern)->bullet_rotation,
                                                 (*pattern)->bullet->color
                                                 );
            rectangles.push_back(rectangle);
        }

//        for(int i=0;i<(*pattern)->bullet->hitboxes.size();i++)
//        {
//            if(!(*pattern)->is_hit)
//            {
//                Hitbox *hitbox=(*pattern)->bullet->hitboxes[i];
//
//                DrawableRectangle* rectangle2 = new DrawableRectangle(hitbox->getX()+(*pattern)->x,hitbox->getY()+(*pattern)->y,
//                                                     hitbox->getWidth(),hitbox->getHeight(),
//                                                     (*pattern)->angle+hitbox->angle+(*pattern)->bullet_rotation,
//                                                     Color(255,255,255,255)
//                                                     );
//                rectangles.push_back(rectangle2);
//            }
//        }
    }

    rosalila()->graphics->drawRectangles(rectangles,0,0,true);

    for(int i=0;i<(int)rectangles.size();i++)
        delete rectangles[i];

}

void Character::setOrientation(string orientation)
{
    this->orientation=orientation;
    this->current_sprite=0;
}

bool Character::collides(Hitbox hitbox,int hitbox_x,int hitbox_y,float hitbox_angle)
{
    if(!visible)
        return false;
    for(int i=0;i<(int)hitboxes.size();i++)
        if(hitboxes[i]->getPlacedHitbox(this->x,this->y).collides(hitbox))
            return true;
    return false;
}

void Character::hit(int damage)
{
    this->hp-=damage;
    if(hp<0)
        hp=0;
}

void Character::addActivePattern(Pattern* pattern)
{
    Pattern* pattern_temp=new Pattern(pattern,this->x,this->y);
    float angle=pattern_temp->angle;
    angle+=pattern_temp->getRandomAngle();
    pattern_temp->angle=angle;

    active_patterns->push_back(pattern_temp);
}

void Character::shakeScreen(int shake_magnitude, int shake_time)
{
    this->shake_magnitude=shake_magnitude;
    this->shake_time=shake_time;
}

void Character::deleteActivePatterns()
{
    std::list<Pattern*>::iterator i = active_patterns->begin();
    while (i != active_patterns->end())
    {
        Pattern*p=(Pattern*)*i;
        p->hit(sound_channel_base+1,true);
        i++;
    }
}
