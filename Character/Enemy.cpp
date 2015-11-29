#include "Enemy.h"

Enemy::Enemy(Sound* sonido,RosalilaGraphics* painter,Receiver* receiver,std::string name,Player*player)
{
    //Setting up the other variables
    this->name=name;
    this->directory="stages/"+name+"/Enemy/";
    this->sonido=sonido;
    this->painter=painter;
    this->receiver=receiver;
    this->player=player;
    this->active_patterns=new std::list<Pattern*>;
    this->shooting=true;
    this->orientation="idle";
    this->current_type="";
    this->visible=true;

    //Enemy variables
    this->angle=180;
    this->velocity=0;
    this->angle_change=0;


    //Sprites animation
    this->animation_velocity=4;
    this->animation_iteration=0;
    this->current_sprite=0;

    this->iteration=0;

    this->score_upload_message="";
    bool flag_begin_upload=false;

    loadFromXML();

    loadModifiersFromXML();
    life_bar=painter->getTexture(assets_directory+directory+"life_bar.png");
}

void Enemy::modifiersControl()
{
    bool flag_iterator_change=false;

    vector<Modifier*>* current_modifiers = this->modifiers[iteration];
    if(current_modifiers!=NULL)
    {
        for(int i=0;i<(int)current_modifiers->size();i++)
        {
            Modifier* modifier=(*current_modifiers)[i];
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
                for(int i=0;i<type[current_type].size();i++)
                {
                    type[current_type][i]->current_cooldown=0;
                    type[current_type][i]->current_startup=0;
                    type[current_type][i]->iteration=0;
                    type[current_type][i]->state="startup";
                }
                this->current_type=modifier->value;
            }
        }
    }

    if(!flag_iterator_change && getIterateSlowdownFlag())
        iteration++;
}

void Enemy::logic(int stage_velocity, string stage_name, int global_iteration, string username)
{
    animationControl();
    spellControl(stage_velocity);

    for (std::list<Pattern*>::iterator pattern = active_patterns->begin(); pattern != active_patterns->end(); pattern++) {
        Pattern* p =  (Pattern*)*pattern;
        double distance_x= player->getHitbox().getX() - p->getX();
        double distance_y= player->getHitbox().getY() - p->getY();

        if (p->getHoming() != 0)
        {
            p->setAngle(-atan2(distance_y,distance_x)*180/PI);
        }
        else if(p->getAimPlayer())
        {
            p->setAngle(p->getAngle()-atan2(distance_y,distance_x)*180/PI);
        }


    }


    if(this->hp>0)
        modifiersControl();
    else
    {
        if(orientation!="destroyed" && flag_begin_upload)
        {
            orientation="destroyed";
            if(this->sonido->soundExists(name+".destroyed"))
                this->sonido->playSound(name+".destroyed");

            this->hitbox.setValues(0,0,0,0,0);

            //Delete bullets
            std::list<Pattern*>* active_patterns=getActivePatterns();
            std::list<Pattern*>::iterator i = active_patterns->begin();
            while (i != active_patterns->end())
            {
                Pattern*p=(Pattern*)*i;
                active_patterns->erase(i++);
                delete p;
            }

//            RosalilaNetwork network(painter);
//            //score_upload_message = network.runTcpClientSendScore(31716, "108.59.1.187",stage_name, username, global_iteration);
//            score_upload_message = network.runTcpClientSendScore(31716, "localhost",stage_name, username, global_iteration);
        }
    }

    this->angle+=this->angle_change / getSlowdown();

    this->x += cos (angle*PI/180) * velocity / getSlowdown() + stage_velocity;
    this->y -= sin (angle*PI/180) * velocity / getSlowdown();

    getIterateSlowdownFlag();
}

void Enemy::render()
{
    painter->drawRectangle(life_bar_x+life_bar_rect_offset_x,life_bar_y+life_bar_rect_offset_y,(life_bar_rect_width*hp)/max_hp,life_bar_rect_height,0,this->color.getRed(),this->color.getGreen(),this->color.getBlue(),this->color.getAlpha(),false);
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
    if(this->hp<=0)
    {
        painter->drawText("Uploading score.",0,80);
        painter->drawText(score_upload_message,0,95);
        flag_begin_upload = true;
    }
}

void Enemy::loadModifiersFromXML()
{
    //Loading file
    std::string main_path=assets_directory+directory+"modifiers.xml";
    TiXmlDocument doc_t(main_path.c_str());
    doc_t.LoadFile();
    TiXmlDocument *doc;
    doc=&doc_t;
    TiXmlNode *modifiers_file=doc->FirstChild("ModifiersFile");
    //Loading sprites
    for(TiXmlNode* modifier_node=modifiers_file->FirstChild("Modifier");
            modifier_node!=NULL;
            modifier_node=modifier_node->NextSibling("Modifier"))
    {
        vector<Modifier*>* temp_modifiers=new vector<Modifier*>();

        int at=atoi(modifier_node->ToElement()->Attribute("at"));

        if(modifier_node->ToElement()->Attribute("velocity")!=NULL)
        {
            std::string value=modifier_node->ToElement()->Attribute("velocity");
            temp_modifiers->push_back(new Modifier("velocity",value));
        }

        if(modifier_node->ToElement()->Attribute("angle")!=NULL)
        {
            std::string value=modifier_node->ToElement()->Attribute("angle");
            temp_modifiers->push_back(new Modifier("angle",value));
        }

        if(modifier_node->ToElement()->Attribute("pattern_type")!=NULL)
        {
            std::string value=modifier_node->ToElement()->Attribute("pattern_type");
            temp_modifiers->push_back(new Modifier("pattern_type",value));
        }

        if(modifier_node->ToElement()->Attribute("angle_change")!=NULL)
        {
            std::string value=modifier_node->ToElement()->Attribute("angle_change");
            temp_modifiers->push_back(new Modifier("angle_change",value));
        }

        if(modifier_node->ToElement()->Attribute("iterator")!=NULL)
        {
            std::string value=modifier_node->ToElement()->Attribute("iterator");
            temp_modifiers->push_back(new Modifier("iterator",value));
        }

        this->modifiers[at]=temp_modifiers;

        if(modifier_node->ToElement()->Attribute("repeat")!=NULL)
        {
            int repeats = atoi(modifier_node->ToElement()->Attribute("repeat"));
            int frequency = 1;
            if(modifier_node->ToElement()->Attribute("repeat_frequency")!=NULL)
                frequency = atoi(modifier_node->ToElement()->Attribute("repeat_frequency"));
            for(int i=0;i<repeats;i++)
            {
                this->modifiers[at+frequency*(i+1)]=temp_modifiers;
            }
        }
    }
}

void Enemy::addActivePattern(Pattern* pattern)
{
    Pattern* pattern_temp=new Pattern(pattern,this->x,this->y);
    float angle=pattern_temp->getAngle();
    angle+=pattern_temp->getRandomAngle();

    pattern_temp->setAngle(angle);

    if(pattern_temp->getAimPlayer())
    {
        double distance_x=player->x-pattern_temp->getX();
        double distance_y=player->y-pattern_temp->getY();
        pattern_temp->setAngle(pattern_temp->getAngle()-atan2(distance_y,distance_x)*180/PI);
    }

    active_patterns->push_back(pattern_temp);
}
