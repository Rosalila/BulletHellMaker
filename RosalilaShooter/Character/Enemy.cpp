#include "Enemy.h"

Enemy::Enemy(std::string name,Player*player,int sound_channel_base,bool is_mod)
{
    //Setting up the other variables
    this->name = name;
    this->is_mod = is_mod;
    if(!is_mod)
      this->directory="stages/"+name+"/Enemy/";
		else
      this->directory="mods/stages/"+name+"/Enemy/";
    this->player=player;
    this->active_patterns=new std::list<Pattern*>;
    this->shooting=true;
    this->orientation="idle";
    this->current_type="";
    this->visible=true;

    this->life_bar = NULL;
    this->flat_shadow_texture = NULL;

    //Enemy variables
    this->angle=180;
    this->velocity=0;
    this->angle_change=0;


    //Sprites animation
    this->animation_velocity=4;
    this->animation_iteration=0;
    this->current_sprite=0;

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

    loadFromXML();

    loadModifiersFromXML();
}

Enemy::~Enemy()
{
    for(map<int, vector<Modifier*> >::iterator i = modifiers.begin(); i!= modifiers.end();i++)
    {
        for(int j=0;(int)j<(int)(*i).second.size();j++)
        {
            delete (*i).second[j];
        }
    }
}

void Enemy::modifiersControl()
{
    bool flag_iterator_change=false;

    vector<Modifier*> current_modifiers = this->modifiers[iteration];
    for(int i=0;i<(int)current_modifiers.size();i++)
    {
        Modifier* modifier=current_modifiers[i];
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
        if(modifier->variable=="pattern_type")
        {
            //Reset cooldowns
            for(int i=0;i<(int)type[current_type].size();i++)
            {
                type[current_type][i]->current_cooldown=0;
                type[current_type][i]->current_startup=0;
                type[current_type][i]->iteration=0;
                type[current_type][i]->state="startup";
            }
            this->current_type=modifier->value;
        }
    }

    if(!flag_iterator_change && getIterateSlowdownFlag())
        iteration++;
}

void Enemy::logic(int stage_velocity, string stage_name)
{
    animationControl();
    spellControl(stage_velocity);

    for (std::list<Pattern*>::iterator pattern = active_patterns->begin(); pattern != active_patterns->end(); pattern++)
    {
        Pattern* p =  (Pattern*)*pattern;
        double distance_x= player->hitboxes[0]->x + player->x + - p->x;
        double distance_y= player->hitboxes[0]->y + player->y + - p->y;
        //.getPlacedHitbox(this->x,this->y)

        if (p->homing)
        {
            p->angle=-atan2(distance_y,distance_x)*180/PI;
            p->angle+=p->homing_angle;
        }
        else if(p->getAimPlayer())
        {
            p->angle=p->angle-atan2(distance_y,distance_x)*180/PI;
        }
    }


    if(!getGameOver())
        modifiersControl();
    else
    {
        if(orientation!="destroyed")
        {
            orientation="destroyed";
            if(rosalila()->sound->soundExists(name+".destroyed"))
                rosalila()->sound->playSound(name+".destroyed", 1, 0, this->x, true);

            for(int i=0;i<(int)hitboxes.size();i++)
                this->hitboxes[i]->setValues(0,0,0,0,0);
        }
    }

    this->angle+=this->angle_change / getSlowdown();

    this->x += cos (angle*PI/180) * velocity / getSlowdown() + stage_velocity;
    this->y -= sin (angle*PI/180) * velocity / getSlowdown();

    getIterateSlowdownFlag();

    current_color_effect_a = /*255-*/(255*hp)/max_hp;
}

void Enemy::bottomRender()
{
    Character::bottomRender();
}

void Enemy::topRender()
{
    Character::topRender();
}

void Enemy::loadModifiersFromXML()
{
    Node* root_node = rosalila()->parser->getNodes(assets_directory+directory+"modifiers.xml");

    vector<Node*> modifier_nodes = root_node->getNodesByName("Modifier");

    for(int i=0;i<(int)modifier_nodes.size();i++)
    {
        vector<Modifier*> temp_modifiers;

        int at=atoi(modifier_nodes[i]->attributes["at"].c_str());

        if(modifier_nodes[i]->hasAttribute("velocity"))
        {
            std::string value=modifier_nodes[i]->attributes["velocity"];
            temp_modifiers.push_back(new Modifier("velocity",value));
        }

        if(modifier_nodes[i]->hasAttribute("angle"))
        {
            std::string value=modifier_nodes[i]->attributes["angle"];
            temp_modifiers.push_back(new Modifier("angle",value));
        }

        if(modifier_nodes[i]->hasAttribute("pattern_type"))
        {
            std::string value=modifier_nodes[i]->attributes["pattern_type"];
            temp_modifiers.push_back(new Modifier("pattern_type",value));
        }

        if(modifier_nodes[i]->hasAttribute("angle_change"))
        {
            std::string value=modifier_nodes[i]->attributes["angle_change"];
            temp_modifiers.push_back(new Modifier("angle_change",value));
        }

        if(modifier_nodes[i]->hasAttribute("iterator"))
        {
            std::string value=modifier_nodes[i]->attributes["iterator"];
            temp_modifiers.push_back(new Modifier("iterator",value));
        }

        this->modifiers[at]=temp_modifiers;

        if(modifier_nodes[i]->hasAttribute("repeat"))
        {
            int repeats = atoi(modifier_nodes[i]->attributes["repeat"].c_str());
            int frequency = 1;
            if(modifier_nodes[i]->hasAttribute("repeat_frequency"))
                frequency = atoi(modifier_nodes[i]->attributes["repeat_frequency"].c_str());

            for(int i=0;i<repeats;i++)
            {
                this->modifiers[at+frequency*(i+1)]=temp_modifiers;
            }
        }
    }

    delete root_node;
}

void Enemy::addActivePattern(Pattern* pattern)
{
    if(getGameOver())
        return;
    Pattern* pattern_temp=new Pattern(pattern,this->x,this->y);
    float angle=pattern_temp->angle;
    angle+=pattern_temp->getRandomAngle();

    pattern_temp->angle=angle;

    if(pattern_temp->getAimPlayer())
    {
        double distance_x=player->x-pattern_temp->x;
        double distance_y=player->y-pattern_temp->y;
        pattern_temp->angle=pattern_temp->angle-atan2(distance_y,distance_x)*180/PI;
    }

    active_patterns->push_back(pattern_temp);
}
