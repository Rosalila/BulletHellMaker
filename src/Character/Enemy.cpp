#include "../include/Character/Enemy.h"

Enemy::Enemy(Sonido* sonido,Painter* painter,Receiver* receiver,std::string directory,Player*player)
{
    //Setting up the other variables
    this->sonido=sonido;
    this->painter=painter;
    this->receiver=receiver;
    this->player=player;
    this->active_patterns=new std::list<Pattern*>;
    this->shooting=true;
    this->orientation="idle";
    this->current_type="";

    //Enemy variables
    angle=180;
    velocity=0;
    angle_change=0;
    life_bar_x=0;
    life_bar_y=800-70;

    //Sprites animation
    this->animation_velocity=4;
    this->animation_iteration=0;
    this->current_sprite=0;

    loadFromXML(directory);
    loadModifiersFromXML(directory);
}

void Enemy::modifiersControl()
{
    bool flag_iterator_change=false;

    vector<VariableChange*>* current_changes = this->variable_changes[iteration];
    if(current_changes!=NULL)
    {
        for(int i=0;i<current_changes->size();i++)
        {
            VariableChange* variable_change=(*current_changes)[i];
            if(variable_change->variable=="velocity")
            {
                this->velocity=atoi(variable_change->value.c_str());
            }
            if(variable_change->variable=="angle")
            {
                this->angle=atoi(variable_change->value.c_str());
            }
            if(variable_change->variable=="angle_change")
            {
                this->angle_change=atoi(variable_change->value.c_str());
            }
            if(variable_change->variable=="iterator")
            {
                this->iteration=atoi(variable_change->value.c_str());
                flag_iterator_change=true;
            }
            if(variable_change->variable=="pattern_type")
            {
                this->current_type=variable_change->value;
            }
        }
    }
    angle+=angle_change;
    this->x += cos (angle*PI/180) * velocity;
    this->y -= sin (angle*PI/180) * velocity;

    if(!flag_iterator_change)
        iteration++;
}

void Enemy::logic(int stage_velocity)
{
    animationControl();
    spellControl(stage_velocity);

    if(this->hp!=0)
        modifiersControl();
}

void Enemy::render()
{
    painter->drawRectangle(70,765,(370*hp)/max_hp,30,0,255,0,0,255,false);
    parrentRender();
}

void Enemy::loadModifiersFromXML(std::string directory)
{
    //Loading file
    std::string main_path=directory+"modifiers.xml";
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
        vector<VariableChange*>* temp_variable_changes=new vector<VariableChange*>();

        int at=atoi(modifier_node->ToElement()->Attribute("at"));

        if(modifier_node->ToElement()->Attribute("velocity")!=NULL)
        {
            std::string value=modifier_node->ToElement()->Attribute("velocity");
            temp_variable_changes->push_back(new VariableChange("velocity",value));
        }

        if(modifier_node->ToElement()->Attribute("angle")!=NULL)
        {
            std::string value=modifier_node->ToElement()->Attribute("angle");
            temp_variable_changes->push_back(new VariableChange("angle",value));
        }

        if(modifier_node->ToElement()->Attribute("pattern_type")!=NULL)
        {
            std::string value=modifier_node->ToElement()->Attribute("pattern_type");
            temp_variable_changes->push_back(new VariableChange("pattern_type",value));
        }

        if(modifier_node->ToElement()->Attribute("angle_change")!=NULL)
        {
            std::string value=modifier_node->ToElement()->Attribute("angle_change");
            temp_variable_changes->push_back(new VariableChange("angle_change",value));
        }

        if(modifier_node->ToElement()->Attribute("iterator")!=NULL)
        {
            std::string value=modifier_node->ToElement()->Attribute("iterator");
            temp_variable_changes->push_back(new VariableChange("iterator",value));
        }

        this->variable_changes[at]=temp_variable_changes;

        if(modifier_node->ToElement()->Attribute("repeat")!=NULL)
        {
            int repeats = atoi(modifier_node->ToElement()->Attribute("repeat"));
            int frequency = atoi(modifier_node->ToElement()->Attribute("repeat_frequency"));
            for(int i=0;i<repeats;i++)
            {
                this->variable_changes[at+frequency*(i+1)]=temp_variable_changes;
            }
        }
    }
}

void Enemy::addActivePattern(Pattern* pattern)
{
    Pattern* pattern_temp=new Pattern(pattern,this->x,this->y);
    float angle=pattern_temp->getAngle();
    angle+=pattern_temp->getRandomAngle();

    if(pattern->getAimPlayer())
    {
        double distance_x=player->x-this->x-pattern->offset_x;
        double distance_y=player->y-this->y+pattern->offset_y;
        angle-=atan2(distance_y,distance_x)*180/PI;
    }

    pattern_temp->setAngle(angle);

    active_patterns->push_back(pattern_temp);
}
